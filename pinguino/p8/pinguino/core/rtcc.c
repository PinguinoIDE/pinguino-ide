/*	----------------------------------------------------------------------------
	FILE:  			rtcc.c
	PROJECT: 		pinguino
	PURPOSE: 		Basic Real Time Clock and Calendar functions
	PROGRAMER: 		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	15 Jun 2012
	LAST RELEASE:	23 Jan 2013
	----------------------------------------------------------------------------
    CHANGELOG :
    * 30 Jun 2012   Regis Blanchot  Changed rtccTime and rtccDate to match dcf77
    * 23 Jan 2013   Regis Blanchot
	----------------------------------------------------------------------------
	TODO :
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

#ifndef __RTCC__
#define __RTCC__

#if !defined(__18f26j50) && !defined(__18f46j50) && \
    !defined(__18f27j53) && !defined(__18f47j53)
    #error "Error : your Pinguino has no RTCC module." 
#endif

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>
#include <rtcc.h>
#include <bcd.c>
#include <system.c>

/*	-----------------------------------------------------------------------------
    The RTCSYNC bit indicates a time window during
    which the RTCC Clock Domain registers can be safely
    read and written without concern about a rollover.
    When RTCSYNC = 0, the registers can be safely
    accessed by the CPU.
	---------------------------------------------------------------------------*/

#define RTCC_Wait()    while (RTCCFGbits.RTCSYNC); // wait while RTCC registers are unsafe to read

/*	-----------------------------------------------------------------------------
	Conversion routines from bcd to decimal format
	---------------------------------------------------------------------------*/

void RTCC_ConvertTime(rtccTime *pTm)
{
	pTm->hours    = bcd2bin(pTm->hours);
	pTm->minutes  = bcd2bin(pTm->minutes);
	pTm->seconds  = bcd2bin(pTm->seconds);
}

void RTCC_ConvertDate(rtccDate *pDt)
{
	pDt->dayofweek  = bcd2bin(pDt->dayofweek);
	pDt->dayofmonth = bcd2bin(pDt->dayofmonth);
	pDt->month      = bcd2bin(pDt->month);
	pDt->year       = bcd2bin(pDt->year);
}

/*	-----------------------------------------------------------------------------
	Allow a writing into the clock registers
	In order to perform a write to any of the RTCC timer registers,
    the RTCWREN bit must be set.
	---------------------------------------------------------------------------*/

void RTCC_SetWriteEnable(void)
{
//    SystemUnlock();
    EECON2 = 0x55;          // magic sequence
    EECON2 = 0xAA;
    RTCCFGbits.RTCWREN = 1; // enable write
//    SystemLock();
}

void RTCC_SetWriteDisable(void)
{
//    SystemUnlock();
    EECON2 = 0x55;          // magic sequence
    EECON2 = 0xAA;
    RTCCFGbits.RTCWREN = 0; // disable write
//    SystemLock();
}

/*	-----------------------------------------------------------------------------
	Check that RTCC is writable
	---------------------------------------------------------------------------*/

u8 RTCC_IsWriteEnable(void)
{
    return RTCCFGbits.RTCWREN != 0;
}

/*	-----------------------------------------------------------------------------
	RTCC module is enabled
	The write operations have to be enabled first.
    * clears the alarm interrupt flag.
 	---------------------------------------------------------------------------*/

void RTCC_Enable(void)
{
    RTCC_SetWriteEnable();
    RTCCFGbits.RTCEN = 1;
    RTCC_SetWriteDisable();
    PIR3bits.RTCCIF = 0;
}

/*	-----------------------------------------------------------------------------	
	RTCC module is disabled
	The write operations have to be enabled first.
 	---------------------------------------------------------------------------*/

void RTCC_Disable(void)
{
    RTCC_SetWriteEnable();
    RTCCFGbits.RTCEN = 0;
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------
	Check if RTCC module is enable
	---------------------------------------------------------------------------*/

u8 RTCC_IsEnable(void)
{
    return RTCCFGbits.RTCEN != 0;
}

/*	-----------------------------------------------------------------------------
	The function shutdowns the RTCC device.
    *disables the RTCC
    *disables RTCC write
    *disables the Alarm and the RTCC clock output (RTCOE=0).
    *clears the alarm interrupt flag.
 	---------------------------------------------------------------------------*/

void RTCC_Shutdown(void)
{
    RTCC_Disable();
    PIR3bits.RTCCIF = 0;
}

#endif	/* __RTCC__ */
