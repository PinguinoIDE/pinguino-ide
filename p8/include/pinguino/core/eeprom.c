/*	----------------------------------------------------------------------------
	FILE:			eeprom.c
	PROJECT:		pinguino
	PURPOSE:		Routines d'écriture et de lecture en eeprom
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
					EEPROM_read8 & EEPROM_write8 are (c) Raphael Wimmer.
					Licensed under GNU GPL v2 or higher
	FIRST RELEASE:	09 avril 2010
	LAST RELEASE:	14 jan. 2011
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

#ifndef __EEPROM_C
	#define __EEPROM_C

    #if !defined (__18f2455)  && !defined (__18f2550)  && !defined (__18f4550)  && \
        !defined (__18f25k50) && !defined (__18f45k50)
        #error "Error : this library is intended to be used only with 8-bit Pinguino with EEPROM" 
    #endif

	#include <pic18fregs.h>
	#include <typedef.h>
	#include <macro.h>

/**	----------------------------------------------------------------------------
	---------- myEEPROM_read8()
	----------------------------------------------------------------------------
	---------- Routine de lecture en eeprom
	---------- (c) Raphael Wimmer. Licensed under GNU GPL v2 or higher
	--------------------------------------------------------------------------*/

u8 EEPROM_read8(u8 address)
{
	EEADR = address;
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.RD = 1;
	return EEDATA;
}

/**	----------------------------------------------------------------------------
	---------- myEEPROM_read16()
	----------------------------------------------------------------------------
	---------- Routine de lecture d'un 16-bit en eeprom
	--------------------------------------------------------------------------*/

u16 EEPROM_read16(u8 address)
{
	u8 hibyte;
	u8 lobyte;
	u16 mydata;

	hibyte = EEPROM_read8(address);
	lobyte = EEPROM_read8(address+1);
	mydata = (hibyte << 8) | lobyte;
	return mydata;
}

/**	----------------------------------------------------------------------------
	---------- myEEPROM_write8()
	----------------------------------------------------------------------------
	---------- Routine d'écriture d'un 8-bit en eeprom
	---------- (c) Raphael Wimmer. Licensed under GNU GPL v2 or higher
	--------------------------------------------------------------------------*/

void EEPROM_write8(u8 address, u8 mydata)
{
	EEADR = address;
	EEDATA = mydata;
	// start write sequence as described in datasheet, page 91
	EECON1bits.EEPGD = 0;
	EECON1bits.CFGS = 0;
	EECON1bits.WREN = 1;	// enable writes to data EEPROM
	INTCONbits.GIE = 0;		// disable interrupts
	EECON2 = 0x55;
	EECON2 = 0x0AA;
	EECON1bits.WR = 1;		// start writing
	while (EECON1bits.WR)
	{
		Nop();
	}
	if (EECON1bits.WRERR)
	{
	#ifdef DEBUG
		serial_printf("Writing Error !");
	#endif
	}
	EECON1bits.WREN = 0;
	INTCONbits.GIE = 1;	// enable interrupts
}

/**	----------------------------------------------------------------------------
	---------- myEEPROM_write16()
	----------------------------------------------------------------------------
	---------- Routine d'écriture d'un 16-bit en eeprom
	--------------------------------------------------------------------------*/

void EEPROM_write16(u8 address, u16 mydata)
{
	//u8 hibyte;
	//u8 lobyte;

	//lobyte = (u8)mydata;
	//hibyte = (u8)(mydata >> 8);
	EEPROM_write8(address,   high8(mydata));
	EEPROM_write8(address+1, low8(mydata));
}

#endif
