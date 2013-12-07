/*	----------------------------------------------------------------------------
	FILE:			18b20.c
	PROJECT:		Pinguino
	PURPOSE:		One wire driver to use with DS18B20 digital temperature sensor.
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

	// fixed a bug in decimal part of the measure jp.mandon 02 june. 2011
	
#ifndef __DS18B20_C
	#define __DS18B20_C

	#include <macro.h>
	#include <typedef.h>
	#include <1wire.c>

	typedef struct
	{
		u8  sign;		// sign (1=negative)
		u8  integer;	// integer part
		u16 fraction;	// fractional part
	} DS18B20_Temperature;

/*	----------------------------------------------------------------------------
	---------- GLOBAL VARIABLES
	--------------------------------------------------------------------------*/

	u8 DS18B20Rom[6][8];	// table of found ROM codes
	u8 ROM[8];				// ROM Bit
	u8 lastDiscrep = 0;	// last discrepancy
	u8 doneFlag = 0;		// Done flag
	u8 numROMs;
	u8 dowcrc;
	const u8 dscrc_table[] = {
		0, 94,188,226, 97, 63,221,131,194,156,126, 32,163,253, 31, 65,
		157,195, 33,127,252,162, 64, 30, 95, 1,227,189, 62, 96,130,220,
		35,125,159,193, 66, 28,254,160,225,191, 93, 3,128,222, 60, 98,
		190,224, 2, 92,223,129, 99, 61,124, 34,192,158, 29, 67,161,255,
		70, 24,250,164, 39,121,155,197,132,218, 56,102,229,187, 89, 7,
		219,133,103, 57,186,228, 6, 88, 25, 71,165,251,120, 38,196,154,
		101, 59,217,135, 4, 90,184,230,167,249, 27, 69,198,152,122, 36,
		248,166, 68, 26,153,199, 37,123, 58,100,134,216, 91, 5,231,185,
		140,210, 48,110,237,179, 81, 15, 78, 16,242,172, 47,113,147,205,
		17, 79,173,243,112, 46,204,146,211,141,111, 49,178,236, 14, 80,
		175,241, 19, 77,206,144,114, 44,109, 51,209,143, 12, 82,176,238,
		50,108,142,208, 83, 13,239,177,240,174, 76, 18,145,207, 45,115,
		202,148,118, 40,171,245, 23, 73, 8, 86,180,234,105, 55,213,139,
		87, 9,235,181, 54,104,138,212,149,203, 41,119,244,170, 72, 22,
		233,183, 85, 11,136,214, 52,106, 43,117,151,201, 74, 20,246,168,
		116, 42,200,150, 21, 75,169,247,182,232, 10, 84,215,137,107, 53};

	/// DS18B20 ROM COMMANDS

	#define SEARCHROM				0xF0	//
	#define READROM				0x33	//
	#define MATCHROM				0x55	//
	#define SKIPROM				0xCC	//
	#define ALARM_SEARCH			0xEC	//

	/// DS18B20 FUNCTION COMMANDS

	#define CONVERT_T				0x44	// Initiates temperature conversion
	#define WRITE_SCRATCHPAD	0x4E	// Writes data into scratchpad bytes 2, 3, and 4 (TH, TL and configuration registers)
	#define READ_SCRATCHPAD		0xBE	// Reads the entire scratchpad including the CRC byte
	#define COPY_SCRATCHPAD		0x48	// Copies TH, TL, and configuration register data from the scratchpad to EEPROM
	#define RECALL_E2				0xB8	// Recalls TH, TL, and configuration register data from EEPROM to the scratchpad
	#define READ_POWER_SUPPLY	0xB4	// Signals DS18B20 power supply mode to the master

	/// MODES

	#define RES12BIT	1				// 12-bit resolution (slowest mode)
	#define RES11BIT	2				// 11-bit resolution
	#define RES10BIT	3				// 10-bit resolution
	#define  RES9BIT	4				//  9-bit resolution (quickest mode)

	/// PROTOTYPES

	u8 DS18B20Read(u8, u8, u8, DS18B20_Temperature *);
	u8 DS18B20MatchRom(u8, u8);
	void DS18B20ReadRom(u8, u8 *);
	u8 DS18B20Configure(u8, u8, u8, u8, u8);
	void DS18B20Find(u8);
	u8 DS18B20GetFirst(u8);
	u8 DS18B20GetNext(u8);
	u8 DS18B20_crc(u8);

/*	----------------------------------------------------------------------------
	---------- DS18B20Read()
	----------------------------------------------------------------------------
	* Description:	reads the ds18x20 device on the 1-wire bus and returns the temperature
	* Arguments:	pin = pin number where one wire bus is connected.
					num = index of the sensor or SKIPROM
					resolution = 9 to 12 bit resolution
					t = temperature pointer
	--------------------------------------------------------------------------*/

	u8 DS18B20Read(u8 pin, u8 num, u8 resolution, DS18B20_Temperature * t)
	{
		u8 res, busy = LOW;
		u8 temp_lsb, temp_msb;

		switch (resolution)
		{
			case RES12BIT:	res = 0b01100000;	break;	// 12-bit resolution
			case RES11BIT:	res = 0b01000000;	break;	// 11-bit resolution
			case RES10BIT:	res = 0b00100000;	break;	// 10-bit resolution
			case  RES9BIT:	res = 0b00000000;	break;	//  9-bit resolution
			default:		res = 0b00000000;	break;	//  9-bit resolution
			/// NB: The power-up default of these bits is R0 = 1 and R1 = 1 (12-bit resolution)
		}
		
		if (!DS18B20Configure(pin, num, 0, 0, res)) return FALSE; // no alarm

		if (OneWireReset(pin)) return FALSE;

		if (num == SKIPROM)
		{
			// Skip ROM, address all devices
			OneWireWrite(pin, SKIPROM);
		}
		else
		{
			// Talk to a particular device
			if (!DS18B20MatchRom(pin, num)) return FALSE;
		}

		OneWireWrite(pin, CONVERT_T);		// Start temperature conversion

		while (busy == LOW)					// Wait while busy ( = bus is low)
			busy = OneWireRead(pin);

		if (OneWireReset(pin)) return FALSE;

		if (num == SKIPROM)
		{
			// Skip ROM, address all devices
			OneWireWrite(pin, SKIPROM);
		}
		else
		{
			// Talk to a particular device
			if (!DS18B20MatchRom(pin, num)) return FALSE;
		}

		OneWireWrite(pin, READ_SCRATCHPAD);// Read scratchpad

		temp_lsb = OneWireRead(pin);		// byte 0 of scratchpad : temperature lsb
		temp_msb = OneWireRead(pin);		// byte 1 of scratchpad : temperature msb

		if (OneWireReset(pin)) return FALSE;

		// Calculation
		// ---------------------------------------------------------------------
		//	Temperature Register Format
		//			BIT7	BIT6	BIT5	BIT4	BIT3	BIT2	BIT1	BIT0
		//	LS BYTE 2^3		2^2		2^1		2^0		2^-1	2^-2	2^-3	2^-4
		//			BIT15	BIT14	BIT13	BIT12	BIT11	BIT10	BIT9	BIT8
		//	MS BYTE S		S		S		S		S		2^6		2^5 	2^4
		//	S = SIGN

		if (temp_msb >= 0b11111000)		// test if temperature is negative
		{		
			t->sign = 1;
			temp_msb -= 0b11111000;
		}
		else
		{
			t->sign = 0;
		}

		t->integer = temp_lsb >> 4;			// fractional part is removed, it remains only integer part
		t->integer |= (temp_msb << 4);		// integer part from temp_msb is added

/*	
		t->fraction = 0;					// fractional part
		if (BitRead(temp_lsb, 0)) t->fraction +=  625;
		if (BitRead(temp_lsb, 1)) t->fraction += 1250;
		if (BitRead(temp_lsb, 2)) t->fraction += 2500;
		if (BitRead(temp_lsb, 3)) t->fraction += 5000;
*/
		t->fraction = (temp_lsb & 0x0F) * 625;
		t->fraction /= 100;					// two digits after decimal 

		return TRUE;
	}

/*	----------------------------------------------------------------------------
	---------- DS18B20Configure()
	----------------------------------------------------------------------------
	* Description: writes configuration data to the DS18x20 device
	* Arguments:
					pin = pin number where one wire bus is connected.
					num = index of the sensor
					TH = Alarm Trigger High
					TL = Alarm Trigger Low
					config = configuration
	* Data must be transmitted least significant bit first
	--------------------------------------------------------------------------*/

	u8 DS18B20Configure(u8 pin, u8 num, u8 TH, u8 TL, u8 config)
	{
		if (OneWireReset(pin)) return FALSE;
		if (num == SKIPROM)
		{
			// Skip ROM, address all devices
			OneWireWrite(pin, SKIPROM);
		}
		else
		{
			// Talk to a particular device
			DS18B20MatchRom(pin, num);
		}
		OneWireWrite(pin, WRITE_SCRATCHPAD);	// Allows the master to write 3 bytes of data to the scratchpad
		OneWireWrite(pin, TH);				// The first data byte is written into the TH register (byte 2 of the scratchpad)
		OneWireWrite(pin, TL);				// The second byte is written into the TL register (byte 3)
		OneWireWrite(pin, config);			// The third byte is written into the configuration register (byte 4)
		return TRUE;
	}

/*	----------------------------------------------------------------------------
	---------- Address a specific slave device on a multidrop or single-drop bus
	----------------------------------------------------------------------------
	* Arguments:	pin = pin number where one wire bus is connected.
					num = index of the sensor
	* Description:	reads and returns a byte of data from the device.
	----------------------------------------------------------------------------
	The match ROM command followed by a 64-bit ROM code sequence allows the bus
	master to address a specific slave device on a multidrop or single-drop bus.
	Only the slave that exactly matches the 64-bit ROM code sequence will respond
	to the function command issued by the master; all other slaves on the bus
	will wait for a reset pulse.
	--------------------------------------------------------------------------*/

	u8 DS18B20MatchRom(u8 pin, u8 num)
	{
		u8 i;
		if (OneWireReset(pin)) return FALSE;
		OneWireWrite(pin, MATCHROM);	// Match Rom
		for (i = 0; i < 8; i++)				// Send the Address ROM Code.
			OneWireWrite(pin, DS18B20Rom[num][i]);
		return TRUE;
	}

/*	----------------------------------------------------------------------------
	---------- Reads the ROM Code from a device (when there is only one)
	----------------------------------------------------------------------------
	* Arguments:	pin = pin number where one wire bus is connected.
					romcode = identification code of device.
	* Description: reads and returns a byte of data from the device.
	----------------------------------------------------------------------------
	This command can only be used when there is only ONE slave on the bus.
	It allows the bus master to read the slave’s 64-bit ROM code without using the Search ROM procedure.
	If this command is used when there is more than one slave present on the bus,
	a data collision will occur when all the slaves attempt to respond at the same time.
	--------------------------------------------------------------------------*/

	void DS18B20ReadRom(u8 pin, u8 *romcode)
	{
		u8 i;
		if (!OneWireReset(pin))
		{
			OneWireWrite(pin, READROM);
			for (i = 0; i < 8; i++)		// Reads the ROM Code from a device
				romcode[i] = OneWireRead(pin);
		}
	}

/*	----------------------------------------------------------------------------
	---------- Find Devices on the one-wire bus
	----------------------------------------------------------------------------
	* Arguments: pin number where one wire bus is connected.
	* Description: detects devices and print their rom code.
	--------------------------------------------------------------------------*/

	void DS18B20Find(u8 pin)
	{
		u8 m;

		if (!OneWireReset(pin))	// Detects presence of devices
		{
			if (DS18B20GetFirst(pin))	// Begins when at least one part is found
			{
				numROMs=0;
				do {
					numROMs++;
					// serialprint("Device #");
					for(m = 0; m < 8; m++)
					{
						// Identifies ROM number on found device
						DS18B20Rom[numROMs][m] = ROM[m];
					}
				//Continues until no additional devices are found
				} while (DS18B20GetNext(pin)&&(numROMs<10));
			}
		}
	}

/*	----------------------------------------------------------------------------
	---------- First
	----------------------------------------------------------------------------
	* Arguments: pin number where one wire bus is connected.
	* Description: resets the current state of a ROM search and calls Next to
	find the first device on the 1-wire bus.
	--------------------------------------------------------------------------*/

	u8 DS18B20GetFirst(u8 pin)
	{
		lastDiscrep = 0;			// reset the rom search last discrepancy global
		doneFlag = FALSE;
		return DS18B20GetNext(pin);	// call Next and return its return value
	}

/*	----------------------------------------------------------------------------
	---------- Next
	----------------------------------------------------------------------------
	* Arguments: pin number where one wire bus is connected.
	* Description: searches for the next device on the 1-wire bus. If
	there are no more devices on the 1-wire then false is returned.
	--------------------------------------------------------------------------*/

	u8 DS18B20GetNext(u8 pin)
	{
		u8 m = 1;					// ROM Bit index
		u8 n = 0;					// ROM Byte index
		u8 k = 1;					// bit mask
		u8 x = 0;
		u8 discrepMarker = 0;				// discrepancy marker
		u8 g;						// Output bit
		u8 nxt;						// return value
		int flag;

		nxt = FALSE;					// set the next flag to false
		dowcrc = 0;					// reset the dowcrc

		flag = OneWireReset(pin);			// reset the 1-wire
		if(flag||doneFlag)				// no parts -> return false
		{
			lastDiscrep = 0;			// reset the search
			return FALSE;
		}
		// send SearchROM command for all eight bytes
		OneWireWrite(pin, SEARCHROM);
		do {
			x = 0;
			if(OneWireReadBit(pin) == 1) x = 2;
			//myDelay_10us(12);
			if(OneWireReadBit(pin) == 1 ) x |= 1;
			if(x == 3)
				break;
			else
			{
				if(x > 0)				// all devices coupled have 0 or 1
					g = x >> 1;			// bit write value for search
				else
				{
					// if this discrepancy is before the last
					// discrepancy on a previous Next then pick
					// the same as last time
					if(m < lastDiscrep)
						g = ( (ROM[n] & k) > 0);
					else			// if equal to last pick 1
						g = (m == lastDiscrep);	// if not then pick 0
						// if 0 was picked then record position with mask k
						if (g == 0) discrepMarker = m;
				}
				if (g == 1)	// isolate bit in ROM[n] with mask k
					ROM[n] |= k;
				else
					ROM[n] &= ~k;
				OneWireWriteBit(pin, g);		// ROM search write
				m++;								// increment bit counter m
				k = k << 1;							// and shift the bit mask k
				if(k == 0)							// if the mask is 0 then go to new ROM
				{									// byte n and reset mask
					DS18B20_crc(ROM[n]);			// accumulate the CRC
					n++;
					k++;
				}
			}
		} while (n < 8);							// loop until through all ROM bytes 0-7
		if(m < 65 || dowcrc)						// if search was unsuccessful then
			lastDiscrep=0;							// reset the last discrepancy to 0
		else										// search was successful, so set lastDiscrep, lastOne, nxt
		{
			lastDiscrep = discrepMarker;
			doneFlag = (lastDiscrep == 0);
			nxt = TRUE;								// indicates search is not complete yet, more parts remain
		}
		return nxt;
	}

/*	----------------------------------------------------------------------------
	---------- CRC
	----------------------------------------------------------------------------
	* Arguments:
	* Description:	Update the CRC for transmitted and received data using
					the CCITT 16bit algorithm (X^16 + X^12 + X^5 + 1).
	--------------------------------------------------------------------------*/

	u8 DS18B20_crc(u8 x)
	{
/*		TODO:
		unsigned char ser_data;
		static unsigned int crc;
		crc = (unsigned char)(crc >> 8) | (crc << 8);
		crc ^= ser_data;
		crc ^= (unsigned char)(crc & 0xff) >> 4;
		crc ^= (crc << 8) << 4;
		crc ^= ((crc & 0xff) << 4) << 1;
*/
		dowcrc = dscrc_table[dowcrc^x];
		return dowcrc;
	}

#endif

