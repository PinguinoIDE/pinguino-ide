/*	----------------------------------------------------------------------------
	FILE:				math.c
	PROJECT:			pinguino - http://www.pinguino.cc/
	PURPOSE:			various mathematics functions
	PROGRAMER:			regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:		12 apr. 2011
	LAST RELEASE:		12 sept. 2011
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

#ifndef __MATH_C
#define __MATH_C

#include <typedef.h>
#include <stdlib.h>

/**	----------------------------------------------------------------------------
	---------- random
 	----------------------------------------------------------------------------
	Description	: returns pseudo random number between mini and maxi
	Parameters	:
	Returns		: integer between mini and maxi
    Nota        : random number generator must be initialize with randomSeed function
 	--------------------------------------------------------------------------*/

long random(long mini, long maxi)
{
    return ((rand()%maxi) + mini);
}

/*
long abs(long x)
{
    return ((x)>0?(x):-(x));
}
*/

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

long bounds(long x, long _min, long _max)
{
	long temp;
	
	if (_max < _min)
	{
		temp = _max;
		_max = _min;
		_min = temp;
	}
	
	if (x > _max) return _max;
	if (x < _min) return _min;
	return x;
}

#endif	/* __MATH_C */
