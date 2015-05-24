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
 
/** \file mrf24j40.h
    \brief Microchip mrf24j40 IEEE 802.15.4 module routines

*/
/*
Put the following into your config.h

// ------------------------
// mrf24j40 defines
// ------------------------


// interrupt (input)
#define mrf24j40_int_port PORTB
#define mrf24j40_int_pin  0

// Chip Select (output, active low)
#define mrf24j40_cs_port PORTA
#define mrf24j40_cs_pin  2

// Reset (output, active low, weak pull-up)
#define mrf24j40_reset_port PORTA
#define mrf24j40_reset_pin  3

// Wake (output, disabled by default)
#define mrf24j40_wake_port PORTA
#define mrf24j40_wake_pin  4

// Depending on your module

// Define if you want to enable the PA and LNA on the MRF24J40MB module
#define ENABLE_PA_LNA

// Define if you have an MRF24J40MB module (as opposed to the MRF24J40MA)
#define MRF24J40MB

// Define application site to set power output appropriately
#define APPLICATION_SITE LOC_UNITED_STATES

*/

#ifndef __mrf24j40_h
#define __mrf24j40_h

#ifndef __PIC32MX__
        #include "pic_utils.h"
        #include "config.h"
#endif

#ifdef __PIC32MX__
        #include <typedef.h>
        #define uns8 u8
        #define uns16 u16
#endif
        
#include "mrf24j40_defines.h"

/** First available mrf channel */
#define MRF_FIRST_CHANNEL 11
/** Last availablre mrf channel */
#define MRF_LAST_CHANNEL  26

/** Send mrf packet and request acknowledgement */
#define MRF_ACK	1
/** Send mrf packet without acknowledgement */
#define MRF_NO_ACK 0

/** Module located in undefined location (full power) */
#define LOC_UNDEFINED		0x00
/** Module located in United States (?? power) */
#define LOC_UNITED_STATES   0x01
/** Module located in Canada (?? power) */
#define LOC_CANADA          0x02
/** Module located in Europe (-14.9dB power) */
#define LOC_EUROPE          0x03

/** 
 
    \brief Flush receive buffer of mrf24j40
 
    No need to call this routine normally, except according to mrf24j40
    errata (promiscuous mode)
 
*/

void mrf24j40_flush_receive_buffer();

/** 
 
    \brief Read data from short address of memory location in mrf24j40
 
    Returns the value in the memory location specified.
    
    \param addr Short address memory location (see mrf24h40_defines.h)
 
*/
uns8 mrf24j40_short_addr_read(uns8 addr);

/** 
 
    \brief Write data to short address memory location
 
    Sets the memory location to the value specified
    
    \param addr Short address memory location (see mrf24h40_defines.h)
    \param data Value that the memory location should be set to
 
*/
void mrf24j40_short_addr_write(uns8 addr, uns8 data);

/** 
 
    \brief Read data from long address of memory location in mrf24j40
 
    Returns the value in the memory location specified.
    
    \param addr Long address memory location (see mrf24h40_defines.h)
 
*/
uns8 mrf24j40_long_addr_read(uns16 addr);

/** 
 
    \brief Write data to long address memory location
 
    Sets the memory location to the value specified
    
    \param addr Long address memory location (see mrf24h40_defines.h)
    \param data Value that the memory location should be set to
 
*/
void mrf24j40_long_addr_write(uns16 addr, uns8 data);

/** 
 
    \brief Setup ports/pins as inputs/outputs ready for use
 
    Sets int pin as input and cs pin as output.
     
*/
void mrf24j40_setup_io();

/** 
 
    \brief Initialises mrf24j40 chip ready for use
 
    Sets int pin as input and cs pin as output.
     
*/
void mrf24j40_init();

/** 
 
    \brief Transmit raw data
 
    Transmit a raw packet - this assumes you have already created
    an 802.15.4 compatible packet.  Normally you would use
    transmit_to_extended_adddress or transmit_to_short_address instead.
     
*/
void mrf24j40_transmit(uns8 *data, uns8 bytes_to_transmit);

/** 
 
    \brief Pull received data from buffer
 
    Once an interrupt has occured and we know it is because a packet
    has been received, this routine will pull the data from the mrf
    receive buffer.  This needs to be done quickly so that the next
    packet is not lost.
     
*/
uns8 mrf24j40_receive(uns8 *data, uns8 bytes_to_receive);

/** 
 
    \brief Set IEEE 802.15.4 extended address
 
    Pass a pointer to an 8 byte uns8 array with the address embedded
    as MSB leftmost byte and LSB rightmost byte, eg
    
    uns8 EA_3[8] = { 0, 0, 0, 0, 0, 0, 0, 3 };
    
	// Set extended address to 0x0000000000000003 (64 bit address)
    mrf24j40_set_extended_address(&EA_3);
    
  
     
*/
void mrf24j40_set_extended_address(uns8 *_extended_address);

/** 
 
    \brief Transmit packet to extended address
 
    Requests that the mrf24j40 transmit the packet to the specified extended address
    
	\param frame_type 802.15.4 frame type
	\param dest_pan_id PAN id of destination
	\param dest_extended_address Pointer to uns8 array indicating extended address of destination
	\param data Pointer to uns8 array of bytes to transmit
	\param bytes_to_transmit
	\param ack Either MRF_ACK or MRF_NO_ACK depending if you want hardware acknowledgement
  
     
*/
void mrf24j40_transmit_to_extended_address(uns8 frame_type, uns16 dest_pan_id, uns8 *dest_extended_address,
                                           uns8 *data, uns8 data_length, uns8 ack);
/**
    \brief Transmit packet to short address
 
    Requests that the mrf24j40 transmit the packet to the specified short address
    
	\param frame_type 802.15.4 frame type
	\param dest_pan_id PAN id of destination
	\param dest_short_address 16 bit short address of destination
	\param data Pointer to uns8 array of bytes to transmit
	\param bytes_to_transmit
	\param ack Either MRF_ACK or MRF_NO_ACK depending if you want hardware acknowledgement
*/
void mrf24j40_transmit_to_short_address(uns8 frame_type, uns16 dest_pan_id, uns16 dest_short_address, uns8 *data, uns8 bytes_to_transmit, uns8 ack);

/**

	\brief Set PAN id
	
	Sets the 16 bit PAN id of the module.
	
*/	
void mrf24j40_set_pan_id(uns16 _pan_id);


/**

	\brief Set extended address
	
	Sets the 64 bit extended address of the module. Pass a pointer to an 8 byte
	uns8 array containing the address.
	
*/	
void mrf24j40_set_extended_address(uns8 *_extended_address);

/**

	\brief Set short address
	
	Sets the 16 bit short address of the module.
	
*/	
void mrf24j40_set_short_address(uns16 _short_address);

/**

	\brief Scan all channels for lowest RF energy
	
	Scans through all channels and reports the channel with the lowest
	Energy Detection level.
	
*/	
uns8 mrf24j40_scan_for_lowest_channel_ed();

/**

	\brief Change channels
	
	Change to the specified channel, in the range MRF_FIRST_CHANNEL to
	MRF_LAST_CHANNEL.
	
	\param channel Channel to change to.
	
*/	
void mrf24j40_set_channel(uns8);

/**

	\brief Interrupt service routine for mrf24j40 chip
	
	Call this routine when the mrf24j40 indicates an interrupt condition, or 
	called regularly.
	
	
*/
void mrf24j40_handle_isr();

/**

	\brief Callback is actioned when mrf24j40 has a packet received off air
	
	Callback indicating the mrf24j40 has a packet ready.
	
	
*/
void mrf24j40_receive_callback();


/**

	\brief Callback is actioned when mrf24j40 has finished transmitting a packet, or
	failed to do so.
	
	Callback indicating the mrf24j40 has finished the transmission sequence.
	
	\param status Set to 0 for success or 1 for failure
	\param retries Set to the number of retries
	\param channel_busy Set to 1 if failure was due to the channel being busy.
	
	
*/
void mrf24j40_transmit_callback(uns8 status,		// 0 = success
								uns8 retries,	// retries to transmit
								uns8 channel_busy);	// 1 if fail due to channel busy

#endif





/* notes
need to set frame pending subfield to 1 
beacons have their own sequence number

*/
