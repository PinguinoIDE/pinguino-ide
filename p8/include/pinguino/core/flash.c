/*	----------------------------------------------------------------------------
	FILE:			flash.c
	PROJECT:		pinguino
	PURPOSE:		flash memory functions
	PROGRAMER:		jean-pierre mandon <jpmandon@gmail.com>
	FIRST RELEASE:	2008
	LAST RELEASE:	Fixed for Beta 9 2010
	----------------------------------------------------------------------------
	TODO : PIC18Fx6j50 
    * erase bloack must be 1024 bytes long not 64
    * write block can 2- or 32-bytes long
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

#ifndef __FLASH_C__
#define __FLASH_C__

#include <pic18fregs.h>
#include <typedef.h>

void EraseFlash(u16 address)
{
	TBLPTRU=0;
	TBLPTRH=address>>8;
	TBLPTRL=address;
	 __asm
		bsf     _EECON1, 7
		bcf     _EECON1, 6
		bsf     _EECON1, 2
		bsf     _EECON1, 4
		bcf     _INTCON, 7
        movlw   0x55
		movwf   _EECON2
		movlw   0xaa
		movwf   _EECON2
		bsf     _EECON1, 1
	__endasm;
}

u16 ReadFlash(u16 address)
{
	u8 h8,l8;

	TBLPTRU=0;
	TBLPTRH=address>>8;
	TBLPTRL=address;
	__asm tblrd*+ __endasm;
	l8=TABLAT;
	__asm tblrd*+ __endasm;
	h8=TABLAT;
	return((h8<<8)+l8);
}

void WriteFlash(u16 destination_add, u8 *destination)
{
	static u8 i;

	TBLPTRU=0;
	TBLPTRH=destination_add>>8;
	TBLPTRL=destination_add;

	 __asm 
		tblrd*-
	__endasm;
		
	for(i = 0; i < 32; i++)
	{
		TABLAT = *destination;
		destination++;
		__asm
		    tblwt+*
		__endasm;
	} 

	__asm
		bsf     _EECON1, 7
		bcf     _EECON1, 6
		bsf     _EECON1, 2
		bcf     _INTCON, 7
		
		movlw   0x55
		movwf   _EECON2
		movlw   0xaa
		movwf   _EECON2
		bsf     _EECON1, 1
	__endasm;

}

#endif
