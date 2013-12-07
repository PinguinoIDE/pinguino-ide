/*	----------------------------------------------------------------------------
	FILE:			pcf8574.c
	PROJECT:		pinguino - http://www.pinguino.cc/
	PURPOSE:		driving pcf8574 i/o expander
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	29 jul. 2008
	LAST RELEASE:	22 apr. 2011
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

/*	----------------------------------------------------------------------------
	---------- PCF8574P
	----------------------------------------------------------------------------

	+5V		A0		-|o |-		VDD	    +5V
	+5V		A1		-|	|-		SDA	    pull-up 1K8 au +5V
	+5V		A2		-|	|-		SCL 	pull-up 1K8 au +5V
            P0		-|	|-		INT
            P1		-|	|-		P7
            P2		-|	|-		P6
            P3		-|	|-		P5
	GRND	VSS	    -|	|-		P4

	SYMBOL 	PIN	    DESCRIPTION					NB
	A0		1		address input 0				adress = 0 1 0 0 A2 A1 A0 0
	A1		2		address input 1				A0, A1 et A2 relies au +5V
	A2		3		address input 2				donc adress = 01001110 = 0x4E
	P0		4		quasi-bidirectional I/O 0
	P1		5		quasi-bidirectional I/O 1
	P2		6		quasi-bidirectional I/O 2
	P3		7		quasi-bidirectional I/O 3
	VSS		8		supply ground
	P4		9		quasi-bidirectional I/O 4
	P5		10		quasi-bidirectional I/O 5
	P6		11		quasi-bidirectional I/O 6
	P7		12		quasi-bidirectional I/O 7
	INT		13		interrupt output (active LOW)
	SCL		14		serial clock line			to PIC SCL pin
	SDA		15		serial data line			to PIC SDA pin
	VDD		16		supply voltage
	--------------------------------------------------------------------------*/

#ifndef __PCF8574_C
#define __PCF8574_C

#include <typedef.h>
#include <macro.h>
#include <pinguinoi2c.c>
#include <pcf8574.h>

/**	----------------------------------------------------------------------------
	Init. I2C bus
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

void PCF8574_init()
{
    // PCF8574 SCL clock frequency = 100 kHz max.
    I2C_init(I2C_MASTER_MODE, I2C_100KHZ);
}

/**	----------------------------------------------------------------------------
	Write a byte on the PCF8574
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

u8 PCF8574_write(u8 address, u8 value)
{
    u8 r=0;

	I2C_start();
    if (I2C_writechar(address | I2C_WRITE))
    {
        r = I2C_writechar(value);
    }
	I2C_stop();

	return r;
	//return I2C_send(address, value);
}

/**	----------------------------------------------------------------------------
	Read a byte from the PCF8574
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

u8 PCF8574_read(u8 address)
{
	return I2C_get(address);
}

#endif
