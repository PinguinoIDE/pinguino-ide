/*	----------------------------------------------------------------------------
	FILE:				bcd.c
	PROJECT:			pinguino - http://www.pinguino.cc/
	PURPOSE:			various bcd conversion functions
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	29 june 2011
	LAST RELEASE:	29 june 2011
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

#ifndef __BCD_C
#define __BCD_C

#include <typedef.h>

/* ---------------------------------------------------------------------
   bin2bcd()
   --------------------------------------------------------------------- */

u8 bin2bcd(u8 dec)
{
	return ((dec/10) << 4) + (dec % 10);
}

/* ---------------------------------------------------------------------
   bcd2bin()
   --------------------------------------------------------------------- */

u8 bcd2bin(u8 bcd)
{
	return ((bcd >> 4) * 10) + bcd % 16;
}

#endif	/* __BCD_C */
