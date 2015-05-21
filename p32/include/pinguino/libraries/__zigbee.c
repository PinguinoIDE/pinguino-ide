/*	----------------------------------------------------------------------------
	FILE:			__zigbee.c
	PROJECT:		pinguinoX
	PURPOSE:		
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
	FIRST RELEASE:	30 may. 2011
	LAST RELEASE:	
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	--------------------------------------------------------------------------*/

#ifndef __ZIGBEE
#define __ZIGBEE

#include <typedef.h>
#include <macro.h>
#include <zigbee/mrf24j40.c>

unsigned char ZIGrx[128];               // this is the receive buffer
unsigned char zwpointer,zrpointer;      // write and read pointer

u16 ZIGdestpan;							// dest PAN in received frame
u16 ZIGsrcpan;							// src PAN in received frame
u16 ZIGdestadd;							// dest short address in received frame
u16 ZIGsrcadd;							// src short address in received frame

// init Zigbee MRF24J40 module

void init_zigbee(u8 _channel, u16 pan_id, u16 short_address)
{
	mrf24j40_setup_io();
	mrf24j40_init();
    // RB : 30-08-2013 - Uncommented following lines
	mrf24j40_set_pan_id(pan_id);
	mrf24j40_set_short_address(short_address);
	mrf24j40_set_channel(_channel);
}

// ISR routine for zigbee. todo

void ZIGinterrupt(void)
{
	Nop();
}

// transmit callback is here to make pic_pack_lib happy

void mrf24j40_transmit_callback(uns8 status,uns8 retries,uns8 channel_busy)	
{
	Nop();
}

// receive callback if Zigbee is interrupt managed

void mrf24j40_receive_callback()
{
	unsigned char length,i,character;
	unsigned char znewwp;
	unsigned char data[128];

		length=mrf24j40_long_addr_read(0x300);
		for (i=0;i<length;i++)
			ZIGrx[i]=mrf24j40_long_addr_read(0x301+i);
		mrf24j40_flush_receive_buffer();	
/*	
	length=mrf24j40_receive(data, 128);
	if (length>0)
		for (i=0;i<length;i++)
			{
					if (zwpointer!=127)				// if not last place in buffer
						znewwp=zwpointer+1;			// place=place+1
					else
						znewwp=1;					// else place=1

					if (zrpointer!=znewwp)			// if read pointer!=write pointer
						ZIGrx[zwpointer++]=data[i];	// store received char

					if (zwpointer==128)				// if write pointer=length buffer
						zwpointer=1;				// write pointer = 1
			}
*/
}

// Send a string to short address
 
void ZIGputs(u16 short_address, u8 *zigstr, u8 length)
{
	mrf24j40_transmit_to_short_address(FTDATA, pan_id, short_address, zigstr, length, 1);
}

// new character received ?
// works only if ZIGBEE is managed by interrupt. Else return ERROR -1
	
int ZIGavailable()
{
	#ifdef INTZIG
	return(zwpointer!=zrpointer);
	#else
	return(-1);
	#endif
}

u8 ZIGgets(u8 *zigstr)
{
	unsigned char length,i;
	unsigned char intstat;
	
	#ifndef INTZIG				// Zigbee not interrupt managed
	// read INSTAT Register
	intstat = mrf24j40_short_addr_read(INTSTAT);
	// test if we received something 
	if (test_bit(intstat, INTSTAT_RXIF))
	{
		length=mrf24j40_receive(zigstr,128);
		ZIGdestpan=(zigstr[4]<<8)+zigstr[3];
		ZIGsrcpan=(zigstr[8]<<8)+zigstr[7];
		ZIGsrcadd=(zigstr[10]<<8)+zigstr[9];
		ZIGdestadd=(zigstr[6]<<8)+zigstr[5];
		for (i=0;i<(length-15);i++)
			zigstr[i]=zigstr[i+11];
		zigstr[i]=0;
		return(length-15);
	}
	else return(0);
	#else
	Nop();						// todo interrupt managed
	#endif	
}

#endif
