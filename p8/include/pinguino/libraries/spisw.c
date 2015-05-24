/*	--------------------------------------------------------------------
    FILE:			spisw.c
    PROJECT:		pinguino
    PURPOSE:		SPI software function
    FIRST RELEASE:	Feb. 2014 - Régis Blanchot
    LAST RELEASE:	Feb. 2014
    --------------------------------------------------------------------
    This library is free software you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
    MA  02110-1301  USA
    ------------------------------------------------------------------*/

#ifndef _SPISW_C_
#define _SPISW_C_

#include <digitalw.c>

#define LSBFIRST        0
#define MSBFIRST        1

void shiftOut(u8 dataPin, u8 clockPin, u8 bitOrder, u8 val)
{
    u8 i;
    u8 bitMask;

    for (i = 0; i < 8; i++)
    {
        if (bitOrder == LSBFIRST)
            bitMask = (1 << i);
        else
            bitMask = (1 << (7 - i));

        digitalwrite(dataPin, (val & bitMask) ? HIGH : LOW);
        digitalwrite(clockPin, HIGH);
        digitalwrite(clockPin, LOW);            
    }
}

#endif /* _SPISW_C_ */
