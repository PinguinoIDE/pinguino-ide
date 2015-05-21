/*	----------------------------------------------------------------------------
	FILE:			mathlib.c
	PROJECT:		pinguino
	PURPOSE:		math. functions
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	2009 ?
	LAST RELEASE:	2013 Mai 22
	----------------------------------------------------------------------------
	CHANGELOG:
    * 2012 Jul. 10  regis blanchot  fixed bug (functions already defined in macro.h)
    * 2013 Mai  22  regis blanchot  added random function
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
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
	--------------------------------------------------------------------------*/

#ifndef __MATHLIB_C
#define __MATHLIB_C

//#include <typedef.h>
#include <macro.h>
#include <stdlib.h>

int abs(int v)
{
	return (v < 0) ? -(unsigned)v : v;
}

/**	----------------------------------------------------------------------------
	---------- random
 	----------------------------------------------------------------------------
	Description	: returns pseudo random number between mini and maxi
	Parameters	:
	Returns		: integer between mini and maxi
    Nota        : random number generator must be initialize with randomSeed function
 	--------------------------------------------------------------------------*/

int random(int mini, int maxi)
{
    return ( (rand() % maxi ) - mini );
}

/**	----------------------------------------------------------------------------
	---------- Map
 	----------------------------------------------------------------------------
	Description	: map(value, fromLow, fromHigh, toLow, toHigh)
	Parameters	:
	Returns		:
 	--------------------------------------------------------------------------*/

int map(int x, int in_min, int in_max, int out_min, int out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

#endif
