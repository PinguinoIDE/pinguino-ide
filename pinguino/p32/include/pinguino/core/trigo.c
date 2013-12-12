/*	----------------------------------------------------------------------------
	FILE:			trigo.c
	PROJECT:		pinguino
	PURPOSE:		optimized trigonometric calculation
                    http://www.dattalo.com/technical/theory/sinewave.html
	PROGRAMER:		Regis Blanchot
	FIRST RELEASE:	07 Apr. 2012
	LAST RELEASE:	08 Feb. 2013
	----------------------------------------------------------------------------
	CHANGELOG : 
	Apr 07 2012 - initial release, sin and cos
    Feb 08 2013 - added some comments for better understanding
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
	--------------------------------------------------------------------------*/

#ifndef __TRIGO_C
#define __TRIGO_C

#include <typedef.h>
#include <macro.h>

/*  --------------------------------------------------------------------
    First 90 values (1/4's of the circle) of precomputed sinus table * 256
    To obtain the other 3/4's of the circle, we use the symmetry of sine wave.
    Each quadrant is obtained as follows:
      0 <= x < 90      y =  sine[ x ];
     90 <= x < 180     y =  sine[ 180 - x ];
    180 <= x < 270     y = -sine[ x - 180 ];
    270 <= x < 360     y = -sine[ 360 - x ];
    ------------------------------------------------------------------*/

const u8 sine[90]= {
	0,4,8,13,17,22,26,31,35,40,44,48,53,57,61,66,
	70,74,79,83,87,91,95,100,104,108,112,116,120,124,
	128,131,135,139,143,146,150,154,157,161,164,167,
	171,174,177,181,184,187,190,193,196,198,201,204,
	207,209,212,214,217,219,221,223,226,228,230,232,
	233,235,237,238,240,242,243,244,246,247,248,249,
	250,251,252,252,253,254,254,255,255,255,255,255
};

/*  --------------------------------------------------------------------
    sinus function
    alpha:  angle in degree
    return: sinus x 256
    ------------------------------------------------------------------*/

s32 sinr(s16 alpha)
{   
	u8 sign = 0; // positive

	// normalize the angle
	if (alpha < 0)
		alpha = 360 - ((-alpha) % 360);
	else
		alpha %= 360;

	// sin(a+180) = - sin(a)
	if (alpha >= 180)
	{
		alpha -= 180;
		sign = 1; // negative 
	}

	// now a < 180
    // sin(180-a) == sin(a);
	if (alpha > 90)
		alpha = 180 - alpha;

	// use the table to approximate the angle
	//return (sign * sine[alpha]);
	if (sign)
        return -sine[alpha];
    else
        return  sine[alpha];
}

/*  --------------------------------------------------------------------
    cosinus function
    based on cos(a) = sin (a + 90)
    alpha:  angle in degree
    return: cosinus x 256
    ------------------------------------------------------------------*/

s32 cosr(s16 alpha)
{
	return sinr(alpha + 90);
}

#endif /* __TRIGO_C */
