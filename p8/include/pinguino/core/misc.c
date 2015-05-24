/*	----------------------------------------------------------------------------
	FILE:			misc.c
	PROJECT:		pinguino
	PURPOSE:		misc. functions
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	14 jan. 2011
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

#ifndef __MISC_C
	#define __MISC_C

	#include <typedef.h>
	#include <delay.c>
	#include <digitalw.c>

u8 Keyboard_get(u8 pin)
{

	u8 etat1 = 0;
	u8 etat2 = 0;

	while (1)
	{
		etat1 = (digitalread(pin) == 1 ? 1 : 0);
		Delayms(50);					// 50 milliseconds
		etat2 = (digitalread(pin) == 1 ? 1 : 0);
		if(etat2 == etat1)
			return etat1;				// if state is stable at least 50 ms
	}
}

#endif
