/*	----------------------------------------------------------------------------
	FILE:			1wire.c
	PROJECT:		pinguino
	PURPOSE:		Serial Peripheral Interface functions
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	28 sept. 2010
	LAST RELEASE:	14 jan. 2011
	----------------------------------------------------------------------------
	this file is based on Maxim AN162 and Microchip AN1199
	----------------------------------------------------------------------------
	TODO : 
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

#ifndef __ONEWIRE_C
	#define __ONEWIRE_C

	//#include <typedef.h>
	#include <macro.h>
	#include <delay.c>
	#include <digitalw.c>

	// private
	static void OneWireLow(u8);
	static void OneWireHigh(u8);
	// public
	u8 OneWireReset(u8);
	u8 OneWireReadBit(u8);
	void OneWireWriteBit(u8, u8);
	u8 OneWireReadByte(u8);
	void OneWireWriteByte(u8, u8);

/*	----------------------------------------------------------------------------
	---------- Force the DQ line to a logic low
	--------------------------------------------------------------------------*/

	static void OneWireLow(u8 DQpin)
	{
		digitalwrite(DQpin, LOW);
		pinmode(DQpin, OUTPUT);
	}

/*	----------------------------------------------------------------------------
	---------- Force the DQ line into a high impedance state
	--------------------------------------------------------------------------*/

	static void OneWireHigh(u8 DQpin)
	{
		pinmode(DQpin, INPUT);
	}

/*	----------------------------------------------------------------------------
	---------- Initiates the one wire bus
	----------------------------------------------------------------------------
	Performs a reset on the one-wire bus and returns the presence detect.
	Reset is 2*480us long, presence checked another 60us later.
	returns 0=presence, 1=no part
	--------------------------------------------------------------------------*/

	u8 OneWireReset(u8 DQpin)
	{
		u8 presence;

		OneWireLow(DQpin);			// pull DQ line low
		Delayus(480);				// leave it low for min. 480us
		OneWireHigh(DQpin);			// allow line to return high
		Delayus(60);				// takes 15 to 60 us
		presence = digitalread(DQpin);	// get presence signal
		Delayus(420);				// wait for end of timeslot (960-480-60=420)
		return (presence);
	}

/*	----------------------------------------------------------------------------
	---------- Read a bit from the one-wire bus and return it.
	--------------------------------------------------------------------------*/

	u8 OneWireReadBit(u8 DQpin)
	{
		u8 dq;

		OneWireLow(DQpin);			// pull DQ line low to start time slot
		//myDelay_us(1);				// 1us
		OneWireHigh(DQpin);			// allow line to return high
		//myDelay_us(5);					// Read within 15uS from start of time slot
		dq = digitalread(DQpin);		// get presence signal
		Delayus(50);				// wait for rest of timeslot
		return (dq);					// return value of DQ line
	}

/*	----------------------------------------------------------------------------
	---------- Read a byte from the one-wire bus and return it.
	--------------------------------------------------------------------------*/

	u8 OneWireRead(u8 DQpin)
	{
		u8 i, value = 0;
		for (i=0; i<8; i++)
		{
			if (OneWireReadBit(DQpin))	// reads byte in, one bit at a time
			{
				value |= 0x01 << i;			// and then shifts it left
			}
		}
		return(value);
	}

/*	----------------------------------------------------------------------------
	---------- Writes a bit to the one-wire bus, passed in bitval.
	--------------------------------------------------------------------------*/

	void OneWireWriteBit(u8 DQpin, u8 bitval)
	{
		if(bitval == 1)
		{
			OneWireLow(DQpin);				// pull DQ line low
			//myDelay_us(5);					// for max. 15us
			OneWireHigh(DQpin);				// allow line to return high
			Delayus(60);
		}
		else
		{
			OneWireLow(DQpin);				// pull DQ line low
			Delayus(60);					// for the whole time slot
			OneWireHigh(DQpin);				// allow line to return high
		}
	}

/*	----------------------------------------------------------------------------
	---------- Writes a byte to the one-wire bus.
	--------------------------------------------------------------------------*/

	void OneWireWrite(u8 DQpin, u8 val)
	{
		u8 i, temp;
		for (i=0; i<8; i++)					// writes byte, one bit at a time
		{
			temp = val >> i;				// shifts val right 'i' spaces
			temp &= 0x01;					// copy that bit to temp
			OneWireWriteBit(DQpin, temp);	// write bit in temp into
		}
	}

#endif
