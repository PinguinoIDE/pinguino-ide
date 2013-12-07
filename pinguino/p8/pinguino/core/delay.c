/*	----------------------------------------------------------------------------
	FILE:			delay.c
	PROJECT:		pinguino
	PURPOSE:		pinguino delays functions
	PROGRAMER:		jean-pierre mandon
	FIRST RELEASE:	2008
	LAST RELEASE:	2013-01-17
	----------------------------------------------------------------------------
	CHANGELOG:
    * 2017-01-17    rblanchot - delays are now based on SystemGetClock()
    TODO:
    * check rountines are interuptible
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

#ifndef __DELAY_C__
#define __DELAY_C__

#include <typedef.h>
//#include <macro.h>
//#include <system.c>
#include <delay.h>

/*
    the delayNNtcy family of functions performs a delay of NN cycles.
    Possible values for NN are:
    10      10*n cycles delay
    100     100*n cycles delay
    1k      1000*n cycles delay
    10k     10000*n cycles delay
    100k    100000*n cycles delay
    1m      1000000*n cycles delay
*/

/*
    3100 Hz < Clock < 64MHz
    7750 < Cycles per second = Clock / 4 < 16.000.000
    8 < Cycles per millisecond < 16.000
    0 < Cycles per microsecond < 16
*/
/*
void Delayms(u16 p)
{
    u16 _cycles_per_millisecond_ = SystemGetInstructionClock() / 1000;

    if (_cycles_per_millisecond_ <= 2550)
    {
        while(p--) delay10tcy(_cycles_per_millisecond_ >> 4);
        return;
    }
    if (_cycles_per_millisecond_ <= 25500)
    {
        while(p--) delay100tcy(_cycles_per_millisecond_ >> 7);
        return;
    }
}

void Delayus(u16 p)
{
    //u16 _cycles_per_microsecond_ = SystemGetInstructionClock() / 1000 / 1000;
    while (p--);
}
*/

void Delayms(unsigned int milliseconds)
{
    unsigned int i;
    
    for (i=0; i<milliseconds; i++)
        delay1ktcy(12);
}

void Delayus(unsigned int microseconds)
{
    unsigned int i;
    
    for (i=0; i<microseconds; i++)
        ;
}

#endif /* __DELAY_C__ */
