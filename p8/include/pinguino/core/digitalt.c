/*	--------------------------------------------------------------------
	FILE:			digitalt.c
	PROJECT:		pinguino
	PURPOSE:		Digital IO management
	PROGRAMER:		Jean-Pierre MANDON
	FIRST RELEASE:	2008
	LAST RELEASE:	2014/04/15
	----------------------------------------------------------------------------
	TODO : 
	----------------------------------------------------------------------------
    CHANGELOG :
        jean-pierre mandon : modification 2009/08/08 18F4550
        regis blanchot 2011/08/09 : FreeJALduino support
        regis blanchot 2012/02/14 : Pinguino 26J50 support
        regis blanchot 2012/09/28 : complete rewrite
        regis blanchot 2012/11/19 : Pinguino 1220 and 1320 support
        regis blanchot 2012/12/07 : Pinguino 25k50 and 45k50 support
        regis blanchot 2013/01/05 : fixed warnings about pointers in RAM
        andre gentric  2013/03/29 : fixed Pinguino4550 RA4 pin definition
        regis blanchot 2014/04/15 : one function / file
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	------------------------------------------------------------------*/

#ifndef __DIGITALT__
#define __DIGITALT__

#include <pic18fregs.h>
//#include <typedef.h>
#include <pin.h>
#include <digital.h>
#include <digitalw.c>       // digitalwrite
#include <digitalr.c>       // digitalread

void toggle(unsigned char pin)
{
    unsigned char state;
    state = digitalread(pin);
    digitalwrite(pin, state^1);
}

#endif /* __DIGITALT__ */
