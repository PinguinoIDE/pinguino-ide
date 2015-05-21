/*

Copyright (c) 2010, Embedded Adventures, www.embeddedadventures.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

Contact us at admin@embeddedadventures.com

*/

// 25 may. 2011 adapted to PIC32 pinguino by Jean-Pierre Mandon

#include "mrf24j40.h"

#ifndef __PIC32MX__
        #include "pic_utils.h"
        #include "spi_hw.h"
        #include "pic_serial.h"
#endif

#ifdef __PIC32MX__
        #include <spi.c>
        #include <typedef.h>
        #include <delay.c>
        #include <macro.h>
        #include <digitalw.c>
        #include "ieee154.h"
        // define for Microchip ZigBee UEXT
			#ifdef PIC32_PINGUINO_220
				#define ZIGINT		PORTAbits.RA8			// SDI1
				#define ZIGINTIN	TRISAbits.TRISA8=1		// Input
				#define ZIGRESET	PORTAbits.RA9			// SDO1
				#define ZIGRESETOUT	TRISAbits.TRISA9=0		// Output
				#define ZIGCS		PORTAbits.RA7			// CS
				#define ZIGCSOUT	TRISAbits.TRISA7=0		// Output
			#endif
			#if defined(PIC32_PINGUINO_OTG) || defined(PIC32_PINGUINO)
				#define ZIGINT		PORTGbits.RG7			// SDI2
				#define ZIGINTIN	TRISGbits.TRISG7=1		// Input
				//#define ZIGINT		PORTDbits.RD10
				//#define ZIGINTIN	TRISDbits.TRISD10=1
				#define ZIGRESET	PORTGbits.RG8			// SDO2
				#define ZIGRESETOUT	TRISGbits.TRISG8=0		// Output
				//#define ZIGRESET 	PORTFbits.RF5
				//#define ZIGRESETOUT	TRISFbits.TRISF5=0
				#define ZIGCS		PORTFbits.RF0			// CS
				#define ZIGCSOUT	TRISFbits.TRISF0=0		// Output
			#endif

        // define for macro compatibility
        #define set_bit BitSet
        #define test_bit BitTest
#endif        

// Global variables

uns8 data_sequence_number;	// WPAN data packet sequence
uns16 pan_id = 0xffff;	// Identifier of the PAN we are part of
uns8 extended_address[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};	// Our extended address
uns16 short_address = 0xffff;	// Our short address
uns8 current_channel = 0;	// Current channel, 0 = not set (normal range 11-26)


void mrf24j40_flush_receive_buffer() {
	
	uns8 rxflush;

	rxflush = mrf24j40_short_addr_read(RXFLUSH);	// Get rxflush
	set_bit(rxflush, RXFLUSH_RXFLUSH);	// Set flush bit
	mrf24j40_short_addr_write(RXFLUSH, rxflush);	// Push it back to the mrf
}



void mrf24j40_set_channel(uns8 _channel)
{
	current_channel = _channel;
	_channel = _channel - 11;
	_channel = 0x02 + 0x10 * _channel;
	
	mrf24j40_long_addr_write(RFCON0, _channel);	// Set channel
	mrf24j40_short_addr_write(RFCTL, 0x04);	// RFCTL (0x36) = 0x04 – Reset RF state machine.
	mrf24j40_short_addr_write(RFCTL, 0x00);	// RFCTL (0x36) = 0x00
        
    #ifndef __PIC32MX__
        	delay_us(200);	// Delay at least 192us			
	#endif
	#ifdef __PIC32MX__
        	Delayus(200);
    #endif        	
}	


uns8 mrf24j40_scan_for_lowest_channel_ed() {

uns8 rssi;
uns8 _channel;
uns16 scan_count;
uns8 highest_on_channel;
uns8 lowest_channel = MRF_LAST_CHANNEL;
uns8 lowest_ed = 0xff;
uns8 bbreg6;	

	// We should really ignore all packets here (do to)
	
	#if defined(ENABLE_PA_LNA)
		mrf24j40_long_addr_write(TESTMODE, 0x08);          // Disable automatic switch on PA/LNA
		mrf24j40_short_addr_write(TRISGPIO, 0x0F);        // Set GPIO direction
		mrf24j40_short_addr_write(GPIO, 0x0C);           // Enable LNA
	#endif

	
	for (_channel = MRF_FIRST_CHANNEL; _channel <= MRF_LAST_CHANNEL; _channel++) {
		
		// switch channel
		mrf24j40_set_channel(_channel);
		
		highest_on_channel = 0;
		for (scan_count = 0; scan_count < 1000; scan_count++) {
			mrf24j40_short_addr_write(BBREG6, 1 << BBREG6_RSSIMODE1);
			do {
				bbreg6 = mrf24j40_short_addr_read(BBREG6);
			} while (!test_bit(bbreg6, BBREG6_RSSIRDY));
			rssi = mrf24j40_long_addr_read(RSSI);
			if (rssi > highest_on_channel) {
				highest_on_channel = rssi;
			}	
		}
		
		if (highest_on_channel < lowest_ed) {
			lowest_ed = highest_on_channel;
			lowest_channel = _channel;
		}	
	}
	mrf24j40_short_addr_write(BBREG6, 1 << BBREG6_RSSIMODE2);	// Back to mode 2 (rssi in pkt)
	
	// Should stop ignoring all packets now if we started ignoring them earlier
	// (to do)
	
	#if defined(ENABLE_PA_LNA)
		mrf24j40_short_addr_write(GPIO, 0);
		mrf24j40_short_addr_write(TRISGPIO, 0x00);
		mrf24j40_long_addr_write(TESTMODE, 0x0F);
	#endif

	
	return lowest_channel;
}

/*
// This routine is incomplete
void mrf24j40_active_channel_scan() {

uns8 rxflush;
uns16 prev_pan_id;
uns8 channel;
uns8 highest_on_channel;
uns16 scan_count;
uns8 bbreg6;
uns8 rssi;

	// receive only beacon frames
	
	rxflush = mrf24j40_short_addr_read(RXFLUSH);
	set_bit(rxflush, RXFLUSH_BCNONLY);
	mrf24j40_short_addr_write(RXFLUSH, rxflush);
	
	
	// ignore pending data
		
	
	// store pan id
	prev_pan_id = pan_id;
	
	// set pan id to ffff
	mrf24j40_set_pan_id(0xffff);
	
	for (channel = MRF_FIRST_CHANNEL; channel <= MRF_LAST_CHANNEL; channel++) {
		serial_print_str("Searching on channel: ");
		serial_print_int(channel);
		serial_print_nl();
		
		// switch channel
		mrf24j40_set_channel(channel);

		// send beacon request
		uns8 fc_msb = 0b11001100;	// 64 bit dest (10,11) 64 bit src (14,15)
		             // 0b00000011;
		uns8 fc_lsb = 0b00000001;	// data, no pan id compression
	
		data_sequence_number++;
		uns8 bytes_to_transmit = 5;//??
		uns8 header_length = 3+8+8+2+2;	// Just two bytes of frame control + sequence number
		uns8 frame_length = header_length + bytes_to_transmit;
        
         //       TxBuffer[0] = 0x03;	// fc lsb
         //       TxBuffer[1] = 0x08;	// fc msb
         //       TxBuffer[2] = IEEESeqNum++;     //sequence number
         //       TxBuffer[3] = 0xFF;
         //       TxBuffer[4] = 0xFF;
          //      TxBuffer[5] = 0xFF;
          //      TxBuffer[6] = 0xFF;
           //     TxBuffer[7] = 0x07;
	
	mrf24j40_long_addr_write(0x00, header_length);
	mrf24j40_long_addr_write(0x01, frame_length);
	mrf24j40_long_addr_write(0x02, fc_lsb);	// swapped
	mrf24j40_long_addr_write(0x03, fc_msb);
	mrf24j40_long_addr_write(0x04, data_sequence_number);
	
	mrf24j40_long_addr_write(0x05, 0x05);	// dest pan id LSB
	mrf24j40_long_addr_write(0x06, 0x00);	// MSB

		
		// wait [aBaseSuperframeDuration * (2^n + 1)] symbols, n=scanduration parameter
		// store info on pan in pan description structure
		// beacon is unique if pan id and source address haven't been seen before on current channel
		
		highest_on_channel = 0;
		for (scan_count = 0; scan_count < 1000; scan_count++) {
			mrf24j40_short_addr_write(BBREG6, 1 << BBREG6_RSSIMODE1);
			do {
				bbreg6 = mrf24j40_short_addr_read(BBREG6);
			} while (!test_bit(bbreg6, BBREG6_RSSIRDY));
			rssi = mrf24j40_long_addr_read(RSSI);
			if (rssi > highest_on_channel) {
				highest_on_channel = rssi;
			}	
		}
		serial_print_str("Highest on channel = ");
		serial_print_int(highest_on_channel);
		serial_print_nl();
	
		
		
	}	
	// restore pan id
	// switch channel
	//.. receive all frames
	serial_print_str("/-----\n");
	
}
*/

void mrf24j40_orphan_channel_scan() {
}			

void mrf24j40_start_pan() {
	
}

void mrf24j40_realign_pan() {
}

void mrf24j40_associate_to_pan() {
	// send associate to pan command
	// wait macResponseWaitTime
	
}

void mrf24h40_pan_association_requested() {
	// allocate 16bit short address
		// fffe stuff in 7.5.3.1
	// generate association response command (see 7.3.2)
	// send it indirect, using 7.5.6.3 method
	//  add to pending transactions
}
	

	
void mrf24j40_set_pan_id(uns16 _pan_id)
{
	pan_id = _pan_id;
	mrf24j40_short_addr_write(PANIDL, pan_id & 0xff);
	mrf24j40_short_addr_write(PANIDH, pan_id >> 8);
}

void mrf24j40_set_extended_address(uns8 *_extended_address) {
	uns8 count;
	uns8 mem_pos = EADR7;
	#ifndef __PIC32MX__
        	serial_print_str("Setting EA to: ");
    #endif        	
	for (count = 0; count < 8; count++) {
		extended_address[count] = _extended_address[count];
		mrf24j40_short_addr_write(mem_pos--, extended_address[count]);
		#ifndef __PIC32MX__
        		serial_print_int_hex(extended_address[count]);
	        	serial_putc(' ');
	    #endif	
	}
	#ifndef __PIC32MX__
        	serial_print_nl();
    #endif        	
}	

void mrf24j40_set_short_address(uns16 _short_address)
{
	// Keep hold of the short address
	short_address = _short_address;
	
	// Tell the mrf about it
	mrf24j40_short_addr_write(SADRL, short_address & 0xff);
	mrf24j40_short_addr_write(SADRH, short_address >> 8);
}




void mrf24j40_handle_isr() {

uns8 intstat;

	// first we need to get a copy of intstat to find out what happened
	intstat = mrf24j40_short_addr_read(INTSTAT);
	
	// mrf24j40 intstat register is cleared on read
	if (test_bit(intstat, INTSTAT_RXIF)) {
	        #ifndef __PIC32MX__
        		serial_print_str("R");
                #endif        		
		// handle receive
		mrf24j40_receive_callback();
	}
	if (test_bit(intstat, INTSTAT_TXNIF)) {
	        #ifndef __PIC32MX__	
        		serial_print_str("Tx complete txstat=0x");
        	#endif	
		uns8 stat = mrf24j40_short_addr_read(TXSTAT);
		mrf24j40_transmit_callback(stat & 0b00000001,	// success = 0
		                                  stat >> 6,	// retries
		                                  test_bit(stat, TXSTAT_CCAFAIL)); // due to cca failure?
	        #ifndef __PIC32MX__		                                  
        		serial_print_int_hex(stat);
	        	serial_print_nl();
                #endif	        	
	}

}	

uns8 mrf24j40_receive(uns8 *data, uns8 bytes_to_receive) {

uns8 frame_length;
uns16 frame_pos;
uns8 buffer_count;
/*
1. Receive RXIF interrupt.
2. Disable host microcontroller interrupts.
3. Set RXDECINV = 1; disable receiving packets off air.
4. Read address, 0x300; get RXFIFO frame length value.
5. Read RXFIFO addresses, 0x301 through (0x300 + Frame Length + 2); read packet data plus LQI and RSSI.
6. Clear RXDECINV = 0; enable receiving packets.
7. Enable host microcontroller interrupts.
*/
        #ifndef __PIC32MX__        
        	serial_putc('a');
        #endif        	
	// Disable reading packets off air
	mrf24j40_short_addr_write(BBREG1, 1 << BBREG1_RXDECINV);
        #ifndef __PIC32MX__        	
        	serial_putc('b');
        #endif        	
	
	frame_pos = 0x300;
	frame_length = mrf24j40_long_addr_read(frame_pos++);	
	buffer_count = 0;
        #ifndef __PIC32MX__	
        	serial_putc('c');
        #endif        	
	
	while ((buffer_count <= bytes_to_receive) && (buffer_count <= frame_length + 2)) {
		//0x301 through (0x300 + Frame Length + 2 ); read packet data plus LQI and RSSI.
		data[buffer_count++] = mrf24j40_long_addr_read(frame_pos++);
	}
        #ifndef __PIC32MX__	
        	serial_putc('d');
        #endif        	
	
	// Re-enable reading packets off air
	mrf24j40_short_addr_write(BBREG1, 0);
        #ifndef __PIC32MX__	
        	serial_putc('e');
        #endif        	

	return buffer_count - 1;
	
}

void mrf24j40_transmit_to_extended_address(uns8 frame_type, uns16 dest_pan_id, uns8 *dest_extended_address,
                                           uns8 *data, uns8 data_length, uns8 ack) {
	
	// See notes below on frame control bytes format:
	uns8 fc_msb = 0b11001100;	// 64 bit dest (10,11) 64 bit src (14,15)
	uns8 fc_lsb = 0b00000000 | frame_type;	// pan id compression=0, data
        #ifdef __PIC32MX__
                u8 count=0;
        #endif 
        
	if (ack) {
		set_bit(fc_lsb, 5);	// ack bit
	}
	
	data_sequence_number++;
	
	uns8 header_length = 3+8+8+2+2;
	uns8 frame_length = header_length + data_length;
	
	// Write out data to mrf
		
	mrf24j40_long_addr_write(0x00, header_length);
	mrf24j40_long_addr_write(0x01, frame_length);
	
	mrf24j40_long_addr_write(0x02, fc_lsb);	
	mrf24j40_long_addr_write(0x03, fc_msb);
	mrf24j40_long_addr_write(0x04, data_sequence_number);
	
	mrf24j40_long_addr_write(0x05, dest_pan_id & 0xff);	// dest pan id LSB
	mrf24j40_long_addr_write(0x06, dest_pan_id >> 8);	// MSB

	mrf24j40_long_addr_write(0x07, dest_extended_address[7]); // LSB
	mrf24j40_long_addr_write(0x08, dest_extended_address[6]);
	mrf24j40_long_addr_write(0x09, dest_extended_address[5]);
	mrf24j40_long_addr_write(0x0a, dest_extended_address[4]);
	mrf24j40_long_addr_write(0x0b, dest_extended_address[3]);
	mrf24j40_long_addr_write(0x0c, dest_extended_address[2]);
	mrf24j40_long_addr_write(0x0d, dest_extended_address[1]);
	mrf24j40_long_addr_write(0x0e, dest_extended_address[0]); // MSB
	
	mrf24j40_long_addr_write(0x0f, pan_id & 0xff);	// src pan id LSB
	mrf24j40_long_addr_write(0x10, pan_id >> 8);	// MSB

	
	mrf24j40_long_addr_write(0x11, extended_address[7]);	// LSB
	mrf24j40_long_addr_write(0x12, extended_address[6]);
	mrf24j40_long_addr_write(0x13, extended_address[5]);
	mrf24j40_long_addr_write(0x14, extended_address[4]);
	mrf24j40_long_addr_write(0x15, extended_address[3]);
	mrf24j40_long_addr_write(0x16, extended_address[2]);
	mrf24j40_long_addr_write(0x17, extended_address[1]);
	mrf24j40_long_addr_write(0x18, extended_address[0]);	// MSB
	#ifndef __PIC32MX__
	        for (uns8 count=0;  count < data_length; count++) {
		        mrf24j40_long_addr_write(count+header_length+2,  data[count]);
        #endif
        #ifdef __PIC32MX__
	        for (count=0;  count < data_length; count++) {
		        mrf24j40_long_addr_write(count+header_length+2,  data[count]);
        #endif        		        
	}
	
	uns8 txncon = mrf24j40_short_addr_read(TXNCON);
	
	set_bit(txncon, TXNCON_TXNTRIG);
	if (ack) {
		set_bit(txncon, TXNCON_TXNACKREQ);
	}	
	mrf24j40_short_addr_write(TXNCON, txncon);
	

}


void mrf24j40_transmit_to_short_address(uns8 frame_type, uns16 dest_pan_id, uns16 dest_short_address, uns8 *data, uns8 bytes_to_transmit, uns8 ack) {
	
	uns8 fc_msb = 0b10001000;	// short dest (10,11) short src (14,15)
	uns8 fc_lsb = 0b00000000 | frame_type;	// data, pan id compression (only have dest pan id)
        #ifdef __PIC32MX__
                u8 count=0;
        #endif                
	// To do:
	// Not smart enough for this yet:
	//if (dest_pan_id == pan_id) {
	//	set_bit(fc_lsb, 6); 	// pan compression
	//}	
	if (ack) {
		set_bit(fc_lsb, 5);	// ack bit
	}	
	
	data_sequence_number++;
	
	uns8 header_length = 3+2+2+2+2;
	uns8 frame_length = header_length + bytes_to_transmit;
		
	mrf24j40_long_addr_write(0x00, header_length);
	mrf24j40_long_addr_write(0x01, frame_length);
	
	mrf24j40_long_addr_write(0x02, fc_lsb);	
	mrf24j40_long_addr_write(0x03, fc_msb);
	mrf24j40_long_addr_write(0x04, data_sequence_number);
	
	mrf24j40_long_addr_write(0x05, dest_pan_id & 0xff);	// dest pan id  LSB
	mrf24j40_long_addr_write(0x06, dest_pan_id >> 8);	// MSB
	
	mrf24j40_long_addr_write(0x07, dest_short_address & 0xff); // LSB
	mrf24j40_long_addr_write(0x08, dest_short_address >> 8);	// MSB

	mrf24j40_long_addr_write(0x09, pan_id & 0xff);	// src pan id  (=ours) LSB
	mrf24j40_long_addr_write(0x0a, pan_id >> 8);	// MSB

	
	mrf24j40_long_addr_write(0x0b, short_address & 0xff);	// LSB
	mrf24j40_long_addr_write(0x0c, short_address >> 8);
        #ifndef __PIC32MX__	
	for (uns8 count=0;  count < bytes_to_transmit; count++) { // check format here
	#endif
	#ifdef __PIC32MX__
	for (count=0;  count < bytes_to_transmit; count++) { 
	#endif
		mrf24j40_long_addr_write(count+header_length+2,  data[count]);
	}
	
	uns8 txncon = mrf24j40_short_addr_read(TXNCON);
	set_bit(txncon, TXNCON_TXNTRIG);
	if (ack) {
		set_bit(txncon, TXNCON_TXNACKREQ);
	}	
	mrf24j40_short_addr_write(TXNCON, txncon);
	

}



void mrf24j40_transmit(uns8 *data, uns8 bytes_to_transmit) {

	
	uns8 fc_lsb = 0b01000001;
	uns8 fc_msb = 0b00000000;
        #ifdef __PIC32MX__
                u8 count=0;
        #endif                	
	
	
	data_sequence_number++;
	uns8 header_length = 3;	// Just two bytes of frame control + sequence number, no addrs
	uns8 frame_length = header_length + bytes_to_transmit;
	
	mrf24j40_long_addr_write(0x00, header_length);
	mrf24j40_long_addr_write(0x01, frame_length);
	mrf24j40_long_addr_write(0x02, fc_lsb);
	mrf24j40_long_addr_write(0x03, fc_msb);
	mrf24j40_long_addr_write(0x04, data_sequence_number);
	
	#ifndef __PIC32MX__
	        for (uns8 count=0;  count < bytes_to_transmit; count++) { // check format here
		        mrf24j40_long_addr_write(count+header_length+2,  data[count]);
        #endif
        #ifdef __PIC32MX__
	        for (count=0;  count < bytes_to_transmit; count++) { // check format here
		        mrf24j40_long_addr_write(count+header_length+2,  data[count]);
        #endif        		        
	}
	
	uns8 txncon = mrf24j40_short_addr_read(TXNCON);
	set_bit(txncon, TXNCON_TXNTRIG);
	mrf24j40_short_addr_write(TXNCON, txncon);

}


void mrf24j40_init()
{
	uns8 check;
	
	// Example steps to initialize the MRF24J40:
	
	// 1. SOFTRST (0x2A) = 0x07
	// Perform a software Reset.
	// The bits will be automatically cleared to 0 by hardware.
	mrf24j40_short_addr_write(SOFTRST, 0x07);
	
	// Wait for soft reset to complete

	do {
		check = mrf24j40_short_addr_read(SOFTRST);
	} while (check != 0);
	
	#if defined(ENABLE_PA_LNA) && defined(MRF24J40MB)
		// Turn on PA/LNA if we have one
		mrf24j40_long_addr_write(TESTMODE, 0x0f);
	#endif	
	
	// 2. PACON2 (0x18) = 0x98
	// Initialize FIFOEN = 1 and TXONTS = 0x6.
	mrf24j40_short_addr_write(PACON2, 0x98);
	// 3. TXSTBL (0x2E) = 0x95
	// Initialize RFSTBL = 0x9.
	mrf24j40_short_addr_write(TXSTBL, 0x95);
	
	// wait for mrf to be in receive mode
	
	do {
		check = mrf24j40_long_addr_read(RFSTATE);
	} while (check & 0xa0 != 0xa0);

	mrf24j40_long_addr_write(RFCON0, 0x03); // or three?
	// mrf24j40_long_addr_write(RFCON1, 0x02);	// VCO control mode (1 or 2?)
	// miwi stack source is at odds with data sheet here
	
	// 4. RFCON1 (0x201) = 0x01 – Initialize VCOOPT = 0x01.
	mrf24j40_long_addr_write(RFCON1, 0x01);
	// 5. RFCON2 (0x202) = 0x80 – Enable PLL (PLLEN = 1).
	mrf24j40_long_addr_write(RFCON2, 0x80);
	// 6. RFCON6 (0x206) = 0x90 – Initialize TXFIL = 1 and 20MRECVR = 1.
	mrf24j40_long_addr_write(RFCON6, 0x90);
	// 7. RFCON7 (0x207) = 0x80 – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
	mrf24j40_long_addr_write(RFCON7, 0x08);
	// 8. RFCON8 (0x208) = 0x10 – Initialize RFVCO = 1.
	mrf24j40_long_addr_write(RFCON8, 0x10);
	// 9. SLPCON1 (0x220) = 0x21 – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.
	mrf24j40_long_addr_write(SLPCON1, 0x21);
	
	//Configuration for nonbeacon-enabled devices
	//(see Section MRF datasheet 3.8 “Beacon-Enabled and Nonbeacon-Enabled Networks”):
	//10. BBREG2 (0x3A) = 0x80 – Set CCA mode to ED.
	mrf24j40_short_addr_write(BBREG2, 0x80);
	// 11. RSSITHCCA (0x3F) = 0x60 – Set CCA ED threshold.
	// IH: I think the datasheet is wrong, the mem address is "CCAEDTH", even though
	// it's referred to as RSSITHCCA and CCAMODE in the documentation...
	
	mrf24j40_short_addr_write(CCAEDTH, 0x60);
	// 12. BBREG6 (0x3E) = 0x40 – Set appended RSSI value to RXFIFO.
	mrf24j40_short_addr_write(BBREG6, 1 << BBREG6_RSSIMODE2);
	// 13. Enable interrupts – See Section 3.3 “Interrupts”.
	mrf24j40_short_addr_write(MRF_INTCON, (1 << MRF_INTCON_RXIE) & (1 << MRF_INTCON_TXNIE));
	// 14. Set channel – See Section 3.4 “Channel Selection”.
	mrf24j40_long_addr_write(RFCON0, 11);
        
    #if defined(ENABLE_PA_LNA) && defined(MRF24J40MB)
    // There are special MRF24J40 transceiver output power
    // setting for Microchip MRF24J40MB module.
    // To do: Correct settings for other locations
    
		#if APPLICATION_SITE == EUROPE
			// MRF24J40 output power set to be -14.9dB
            mrf24j40_long_addr_write(RFCON3, 0x70);
		#else
			// MRF24J40 output power set to be -1.9dB - Stock setting
            // mrf24j40_long_addr_writeRFCON3, 0x18);

            // MRF24J40 output power set to be -1.2dB
            // mrf24j40_long_addr_write(RFCON3, 0x10);

            // MRF24J40 output power set to be -0.5dB
            // mrf24j40_long_addr_write(RFCON3, 0x08);

            // MRF24J40 output power set to be -0dB -> 22,5 dBm output power
            mrf24j40_long_addr_write(RFCON3, 0x00);
        #endif
    #else
        // power level to be 0dBms
        mrf24j40_long_addr_write(RFCON3,0x00);
    #endif

	// 15. RFCTL (0x36) = 0x04 – Reset RF state machine.
	mrf24j40_short_addr_write(RFCTL, 0x04);
	
	// 16. RFCTL (0x36) = 0x00.
	mrf24j40_short_addr_write(RFCTL, 0x00);
	// 17. Delay at least 192 ìs.
	#ifndef __PIC32MX__	
        	delay_us(200);	
        #endif
        #ifdef __PIC32MX__
                Delayus(200);
        #endif                        	

	//	RXMCR.PANCOORD = 1 if coordinator, 0 is default (not coordinator)

	//mrf24j40_short_addr_write(RXMCR, 0x01); // promiscuous mode, no coord

	data_sequence_number = 0;

}

// This routine is incomplete (to do)
void mrf24j40_init_coordinator() {
uns8 val;
	
	// Example steps to initialize the MRF24J40:
	
	// 1. SOFTRST (0x2A) = 0x07 – Perform a software Reset. The bits will be automatically cleared to ‘0’ by hardware.
	mrf24j40_short_addr_write(SOFTRST, 0x07);
	// 2. PACON2 (0x18) = 0x98 – Initialize FIFOEN = 1 and TXONTS = 0x6.
	mrf24j40_short_addr_write(PACON2, 0x98);
	// 3. TXSTBL (0x2E) = 0x95 – Initialize RFSTBL = 0x9.
	mrf24j40_short_addr_write(TXSTBL, 0x95);
	// 4. RFCON1 (0x201) = 0x01 – Initialize VCOOPT = 0x01.
	mrf24j40_long_addr_write(RFCON1, 0x01);
	// 5. RFCON2 (0x202) = 0x80 – Enable PLL (PLLEN = 1).
	mrf24j40_long_addr_write(RFCON2, 0x008);
	// 6. RFCON6 (0x206) = 0x90 – Initialize TXFIL = 1 and 20MRECVR = 1.
	mrf24j40_long_addr_write(RFCON6, 0x90);
	// 7. RFCON7 (0x207) = 0x80 – Initialize SLPCLKSEL = 0x2 (100 kHz Internal oscillator).
	mrf24j40_long_addr_write(RFCON7, 0x08);
	// 8. RFCON8 (0x208) = 0x10 – Initialize RFVCO = 1.
	mrf24j40_long_addr_write(RFCON8, 0x10);
	// 9. SLPCON1 (0x220) = 0x21 – Initialize CLKOUTEN = 1 and SLPCLKDIV = 0x01.
	mrf24j40_long_addr_write(SLPCON1, 0x21);
	
	//Configuration for nonbeacon-enabled devices
	//(see Section MRF datasheet 3.8 “Beacon-Enabled and Nonbeacon-Enabled Networks”):
	//10. BBREG2 (0x3A) = 0x80 – Set CCA mode to ED.
	mrf24j40_short_addr_write(BBREG2, 0x80);
	// 11. RSSITHCCA (0x3F) = 0x60 – Set CCA ED threshold.
	// IH: I think the datasheet is wrong, the mem address is "CCAEDTH", even though
	// it's referred to as RSSITHCCA and CCAMODE in the documentation...
	mrf24j40_short_addr_write(CCAEDTH, 0x60);
	// 12. BBREG6 (0x3E) = 0x40 – Set appended RSSI value to RXFIFO.
	mrf24j40_short_addr_write(BBREG6, 0x40);
	// 13. Enable interrupts – See Section 3.3 “Interrupts”.
	mrf24j40_short_addr_write(MRF_INTCON, (1 << MRF_INTCON_RXIE) & (1 << MRF_INTCON_TXNIE));
	// 14. Set channel – See Section 3.4 “Channel Selection”.
	mrf24j40_long_addr_write(RFCON0, 11);
	// 15. RFCTL (0x36) = 0x04 – Reset RF state machine.
	mrf24j40_short_addr_write(RFCTL, 0x04);
	
	// 16. RFCTL (0x36) = 0x00.
	mrf24j40_short_addr_write(RFCTL, 0x00);
	// 17. Delay at least 192 ìs.	
	#ifndef __PIC32MX__	
        	delay_us(200);	
        #endif
        #ifdef __PIC32MX__
                Delayus(200);
        #endif    

//	RXMCR.PANCOORD = 1 if coordinator, 0 is default (not coordinator)
//	TXMCR.SLOTTED = 1 for slotted CSMA-CA mode, 0 is default (unslotted CSMA_CA mode)

	//mrf24j40_short_addr_write(RXMCR, 0x01); // promiscuous mode, no coord
	
	//Set the PANCOORD (RXMCR 0x00<3>) bit = 1 to configure as PAN coordinator.
	val = mrf24j40_short_addr_read(RXMCR);
	set_bit(val, RXMCR_PANCOORD);
	mrf24j40_short_addr_write(RXMCR, val);
	
	//2. Set the SLOTTED (TXMCR 0x11<5>) bit = 1 to use Slotted CSMA-CA mode.
	// turn on slotted
	val = mrf24j40_short_addr_read(TXMCR);
	set_bit(val, TXMCR_SLOTTED);
	mrf24j40_short_addr_write(TXMCR, val);

	// 3. Load the beacon frame into the TXBFIFO (0x080-0x0FF).
	uns8 header_length = 2+1+10;	
	uns8 frame_length = header_length + 6;
	uns8 superframe_spec_field_l = 0b10001000; // bo = 8, so = 8
	uns8 superframe_spec_field_h = 0b11001000; // AP/PC/R/BLE/final cap slot
	
	mrf24j40_long_addr_write(0x080+0, header_length);
	mrf24j40_long_addr_write(0x080+1, frame_length);
	mrf24j40_long_addr_write(0x080+2, 0x00);   //frame control - beacon
	mrf24j40_long_addr_write(0x080+3, 0x80);   //addressing mode - source only
	mrf24j40_long_addr_write(0x080+4, data_sequence_number++);   //seq number 
	mrf24j40_long_addr_write(0x080+5, 5);   //PANID LSB
	mrf24j40_long_addr_write(0x080+6, 0);   //PANID MSB
	mrf24j40_long_addr_write(0x080+7, extended_address[7]);	// LSB
	mrf24j40_long_addr_write(0x080+8, extended_address[6]);
	mrf24j40_long_addr_write(0x080+9, extended_address[5]);
	mrf24j40_long_addr_write(0x080+10, extended_address[4]);
	mrf24j40_long_addr_write(0x080+11, extended_address[3]);
	mrf24j40_long_addr_write(0x080+12, extended_address[2]);
	mrf24j40_long_addr_write(0x080+13, extended_address[1]);
	mrf24j40_long_addr_write(0x080+14, extended_address[0]);	// MSB
	
	mrf24j40_long_addr_write(0x080+15, superframe_spec_field_l);
	mrf24j40_long_addr_write(0x080+16, superframe_spec_field_h);
	
	mrf24j40_long_addr_write(0x080+17, 0);	// GTS
	mrf24j40_long_addr_write(0x080+18, 0);	// Pending addresses
	mrf24j40_long_addr_write(0x080+19, 0x4d);	// Protocol ID
	mrf24j40_long_addr_write(0x080+20, 0x10);	// version 
	// no payload
	

	// 4. Set the TXBMSK (TXBCON1 0x25<7>) bit = 1 to mask the beacon interrupt mask.
	//val = mrf24j40_short_addr_read(TXBCON1);
	//set_bit(val, TXBCON1_TXBMSK);
	//mrf24j40_short_addr_write(RXMCR);

	//5. Program the CAP end slot (ESLOTG1 0x13<3:0>) value. If the coordinator supports
	//Guaranteed Time Slot operation, refer to Section 3.8.1.5 “Configuring Beacon-Enabled
	//GTS Settings for PAN Coordinator” below.

	//6. Calibrate the Sleep Clock (SLPCLK) frequency. Refer to Section 3.15.1.2 “Sleep Clock Calibration”.
	//7. Set WAKECNT (SLPACK 0x35<6:0>) value = 0x5F to set the main oscillator (20 MHz)
	//start-up timer value.
	//8. Program the Beacon Interval into the Main Counter,
	//MAINCNT (0x229<1:0>, 0x228, 0x227, 0x226), 
	//and Remain Counter, REMCNT (0x225, 0x224), according to BO and SO values. Refer to
	//Section 3.15.1.3 “Sleep Mode Counters”.
	//9. Configure the BO (ORDER 0x10<7:4>) and SO (ORDER 0x10<3:0>) values. After configuring
	//BO and SO, the beacon frame will be sent immediately.
             //BO3(1) BO2(1) BO1(1) BO0(1) SO3(1) SO2(1) SO1(1) SO0(1)
	//mrf24j40_short_addr_write(ORDER, 0b10001000);
	
	data_sequence_number = 0;

}

uns8 mrf24j40_short_addr_read(uns8 addr)
{
        #ifndef __PIC32MX__
	        clear_pin(mrf24j40_cs_port, mrf24j40_cs_pin);
	        addr = addr & 0b00111111;	// <5:0> bits
	        addr = addr << 1;	// LSB = 0, means read
	        spi_hw_transmit(addr);
	        uns8 result = spi_hw_receive();
	        set_pin(mrf24j40_cs_port, mrf24j40_cs_pin);
        #endif
        #ifdef __PIC32MX__
	        addr = (addr << 1) & 0x7E;
	        ZIGCS=0; // CS must be held low while communicationg with MRF24J40
	        SPI_write(addr);
	        u8 result = SPI_read();
	        ZIGCS=1; // end of communication
        #endif	                	        
	return result;
}	

void mrf24j40_short_addr_write(uns8 addr, uns8 data)
{
        #ifndef __PIC32MX__
	        clear_pin(mrf24j40_cs_port, mrf24j40_cs_pin);
	        addr = addr & 0b00111111;	// <5:0> bits
	        addr = addr << 1;	// LSB = 0, means read
	        set_bit(addr, 0);	// write

	        spi_hw_transmit(addr);
	        spi_hw_transmit(data);

	        set_pin(mrf24j40_cs_port, mrf24j40_cs_pin);
        #endif
        #ifdef __PIC32MX__
        	addr=((addr<<1)&0x7F)|1;
			ZIGCS=0;
			SPI_write(addr);
			SPI_write(data);
			ZIGCS=1;
		#endif	
}	

uns8 mrf24j40_long_addr_read(uns16 addr){

	uns8 result;
        #ifndef __PIC32MX__	
	        clear_pin(mrf24j40_cs_port, mrf24j40_cs_pin);

	        addr = addr & 0b0000001111111111; 	// <9:0> bits
	        addr = addr << 5;
	        set_bit(addr, 15);	// long addresss
	        spi_hw_transmit(addr >> 8);
	        spi_hw_transmit(addr & 0x00ff);
	        result = spi_hw_receive();	

	        set_pin(mrf24j40_cs_port, mrf24j40_cs_pin);
        #endif
        #ifdef __PIC32MX__
			ZIGCS=0;
			addr=((addr<<1)&0x7FE)|0x800;
			addr<<=4;
			SPI_write(addr>>8);
			SPI_write(addr);
			result=SPI_read();
			ZIGCS=1;        
        #endif
	return result;
}	

void mrf24j40_long_addr_write(uns16 addr, uns8 data){

        #ifndef __PIC32MX__
	        clear_pin(mrf24j40_cs_port, mrf24j40_cs_pin);

	        addr = addr & 0b0000001111111111; 	// <9:0> bits
	        addr = addr << 5;

	        set_bit(addr, 15);	// long addresss
	        set_bit(addr, 4);	// set for write

	        spi_hw_transmit(addr >> 8 );
	        spi_hw_transmit(addr & 0x00ff);
	        spi_hw_transmit(data);
	
	        set_pin(mrf24j40_cs_port, mrf24j40_cs_pin);
        #endif
        #ifdef __PIC32MX__
			ZIGCS=0;
			addr=((addr<<1)&0x7FF)|0x801;
			addr<<=4;
			SPI_write(addr>>8);
			SPI_write(addr);
			SPI_write(data);
			ZIGCS=1;     
		#endif   	        
}


void mrf24j40_setup_io() {

        #ifndef __PIC32MX__	
	        make_input(mrf24j40_int_port, mrf24j40_int_pin);
	        set_pin(mrf24j40_cs_port, mrf24j40_cs_pin);	// keep high
	        make_output(mrf24j40_cs_port, mrf24j40_cs_pin);
        #endif	
        #ifdef __PIC32MX__
        	SPI_init();			// init SPI default (2 on PIC32-PINGUINO)
        						// only called here for test
        						// will be called later in main32.c
        	#ifndef PIC32_PINGUINO_220
				//pinmode(13,OUTPUT); // CLK
				//pinmode(11,OUTPUT); // SDO
				//pinmode(12,INPUT);	// SDI				
				ZIGRESETOUT;		// macro for RESET
				ZIGCSOUT;			// macro for CS
				ZIGINTIN;			// interrupt (not yet implemented)
        	#else
				ZIGRESETOUT;		// macro for RESET
				ZIGCSOUT;			// macro for CS
			#endif
        	ZIGRESET=0;
        	ZIGCS=1;
        	ZIGRESET=1;
        	Delayms(100);
		#endif        	
}


