/*  ----------------------------------------------------------------------------
    FILE:           watchdog.c
    PROJECT:        pinguino
    PURPOSE:        pinguino system functions
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  5 Jan. 2011
    LAST RELEASE:   2 Feb. 2013
    ----------------------------------------------------------------------------
    CHANGELOG:
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

#ifndef __WATCHDOG_C
#define __WATCHDOG_C

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>

/*  ----------------------------------------------------------------------------
    Enable watchdog timer
    --------------------------------------------------------------------------*/
#ifdef SYSTEMWATCHDOG
#define SystemWatchdog()        do { WDTCONbits.SWDTEN = 1; } while (0)
#endif

/*  ----------------------------------------------------------------------------
    Disable watchdog timer
    --------------------------------------------------------------------------*/
#ifdef SYSTEMNOWATCHDOG
#define SystemNoWatchdog()      do { WDTCONbits.SWDTEN = 0; } while (0)
#endif

/*  ----------------------------------------------------------------------------
    Clear watchdog timer
    --------------------------------------------------------------------------*/
#ifdef SYSTEMCLEARWATCHDOG
#define SystemClearWatchdog()   clrwdt()
#endif

/*  ----------------------------------------------------------------------------
    Enable deep sleep watchdog timer
    --------------------------------------------------------------------------*/
#if defined(__18f26j50) || defined(__18f46j50)
#ifdef SYSTEMDEEPWATCHDOG
#define SystemDeepWatchdog()        do { WDTCONbits.SWDTEN = 1; } while (0)
#endif
#endif

/*  ----------------------------------------------------------------------------
    Disable deep sleep watchdog timer
    --------------------------------------------------------------------------*/
#if defined(__18f26j50) || defined(__18f46j50)
#ifdef SYSTEMNODEEPWATCHDOG
#define SystemNoDeepWatchdog()      do { WDTCONbits.SWDTEN = 0; } while (0)
#endif
#endif

/*  ----------------------------------------------------------------------------
    Clear watchdog timer
    --------------------------------------------------------------------------*/
#if defined(__18f26j50) || defined(__18f46j50)
#ifdef SYSTEMCLEARDEEPWATCHDOG
#define SystemClearDeepWatchdog()   clrwdt()
#endif
#endif

#endif /* __WATCHDOG_C */

