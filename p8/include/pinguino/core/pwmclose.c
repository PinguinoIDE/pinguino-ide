/*	--------------------------------------------------------------------
	FILE:			digitalw.c
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

#ifndef __PWMCLOSE_C_
#define __PWMCLOSE_C_

#include <pic18fregs.h>
#include <pin.h>
#include <digital.h>
//#include <typedef.h>

//#if defined(ANALOGWRITE) || defined(__PWM__)
void PWM_close(unsigned char pin)
{
    switch (pin)
    {
        #if defined(__18f26j53) || defined(__18f46j53) || \
            defined(__18f27j53) || defined(__18f47j53)
            case CCP4:  CCP4CON  = 0; break;
            case CCP5:  CCP5CON  = 0; break;
            case CCP6:  CCP6CON  = 0; break;
            case CCP7:  CCP7CON  = 0; break;
            case CCP8:  CCP8CON  = 0; break;
            case CCP9:  CCP9CON  = 0; break;
            case CCP10: CCP10CON = 0; break;
        #else
            case CCP1:  CCP1CON  = 0; break;
            case CCP2:  CCP2CON  = 0; break;
        #endif
    }
}
//#endif

#endif /* __PWMCLOSE_C_ */
