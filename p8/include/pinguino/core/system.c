/*  ----------------------------------------------------------------------------
    FILE:           system.c
    PROJECT:        pinguino
    PURPOSE:        pinguino system functions
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  5 Jan. 2011
    LAST RELEASE:   5 Feb. 2013
    ----------------------------------------------------------------------------
    CHANGELOG:
    21-11-2012        regis blanchot        added PINGUINO1220,1320,14k22 support
    07-12-2012        regis blanchot        added PINGUINO25K50 and 45K50 support
                                            added low power functions
    05-02-2013        regis blanchot        splitted system.c in watchdog.c, lowpower.c
                                            and oscilator.c
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

#ifndef __SYSTEM_C
#define __SYSTEM_C

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>

//u8 _gie_status_ = 0;

/*  ----------------------------------------------------------------------------
    Unlock some register
    --------------------------------------------------------------------------*/
#define SystemUnlock() do { EECON2 = 0x55; EECON2 = 0xAA;} while (0)
        
/*  ----------------------------------------------------------------------------
    Disable All Peripheral Interrupt
    --------------------------------------------------------------------------*/
#define SystemDisablePeripheralInterrupt() do { PIE1 = 0; PIE2 = 0;} while (0)

/*  ----------------------------------------------------------------------------
    Software Reset
    --------------------------------------------------------------------------*/
#define SystemReset() reset()

#endif /* __SYSTEM_C */

