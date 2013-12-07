/*	----------------------------------------------------------------------------
	FILE:  			rtcc.c
	PROJECT: 		pinguino
	PURPOSE: 		Time and Date Real Time Clock and Calendar functions
	PROGRAMER: 		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	15 Jun 2012
	LAST RELEASE:	23 Jan 2013
	----------------------------------------------------------------------------
    CHANGELOG :
    * 30 Jun 2012   Regis Blanchot  Changed rtccTime and rtccDate to match dcf77
    * 23 Jan 2013   Regis Blanchot  RTCC Source is INTRC oscillator (cf. Bootloader v4.x CONFIG3L<1>)
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

#ifndef __RTCC1__
#define __RTCC1__

#if !defined(__18f26j50) && !defined(__18f46j50) && \
    !defined(__18f27j53) && !defined(__18f47j53)
    #error "Error : your Pinguino has no RTCC module." 
#endif

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>
#include <rtcc.h>
#include <rtcc.c>
#include <bcd.c>
#include <system.c>

/*	-----------------------------------------------------------------------------
	The write is successful only if Write Enable is set.
	---------------------------------------------------------------------------*/

#if defined(RTCCSETTIME) || defined(RTCCSETTIMEDATE) || defined(RTCCINIT)
void RTCC_SetTime(u32 tm)
{
    rtccTime t0;
    u8 dummy;
        
    t0.l = tm;
    //RTCC_ConvertTime(&t0);
    RTCC_SetWriteEnable();
    RTCCFGbits.RTCPTR1 = 0;     
    RTCCFGbits.RTCPTR0 = 1;     
    // RTCPTR = 01 points on RTCVALH = Weekday and RTCVALL = Hours
    RTCC_Wait();
    RTCVALL = t0.hours;
    RTCC_Wait();
    dummy = RTCVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    // RTCPTR = 00 = Minutes/Seconds
    RTCC_Wait();
    RTCVALL = t0.seconds;
    RTCC_Wait();
    RTCVALH = t0.minutes;   
    RTCC_SetWriteDisable();
}
#endif

/*	-----------------------------------------------------------------------------
    The write is successful only if Write Enable is set.
    RTCPTR value decrements on every read or write of RTCVALH until it reaches ‘00’
---------------------------------------------------------------------------*/

#if defined(RTCCSETDATE) || defined(RTCCSETTIMEDATE) || defined(RTCCINIT)
void RTCC_SetDate(u32 dt)
{
    rtccDate d0;
    u8 dummy;
        
    d0.l = dt;
    RTCC_SetWriteEnable();
    RTCCFGbits.RTCPTR1 = 1;
    RTCCFGbits.RTCPTR0 = 1; 
    // RTCPTR = 11 = Reserved/Year
    RTCC_Wait();
    RTCVALL = d0.year;
    RTCC_Wait();
    dummy = RTCVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    // RTCPTR = 10 = Month/Day
    RTCC_Wait();
    RTCVALL = d0.dayofmonth;      
    RTCC_Wait();
    RTCVALH = d0.month;   
    // RTCPTR = 01 = Weekday/Hours
    RTCC_Wait();
    RTCVALH = d0.dayofweek;
    // RTCPTR = 00 = Minutes/Seconds
    RTCC_SetWriteDisable();
}
#endif

/*	-----------------------------------------------------------------------------
	The write is successful only if Write Enable is set.
	---------------------------------------------------------------------------*/

#ifdef RTCCSETTIMEDATE
void RTCC_SetTimeDate(u32 tm, u32 dt)
{
    RTCC_SetTime(tm);
    RTCC_SetDate(dt);
}
#endif

/*	-----------------------------------------------------------------------------
    Whether RTCSYNC = 1 or 0, the user should employ a
    firmware solution to ensure that the data read did not
    fall on a rollover boundary, resulting in an invalid or
    partial read. This firmware solution would consist of
    reading each register twice and then comparing the two
    values. If the two values matched, then, a rollover did
    not occur.
	---------------------------------------------------------------------------*/

#if defined(RTCCGETTIME) || defined(RTCCGETTIMEDATE)
void RTCC_GetTime(rtccTime* pTm)
{
    u8 dummy;

    RTCCFGbits.RTCPTR1 = 0;
    RTCCFGbits.RTCPTR0 = 1;
    RTCC_Wait();
    pTm->hours = RTCVALL;     
    RTCC_Wait();
    dummy = RTCVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    RTCC_Wait();
    pTm->seconds = RTCVALL;
    RTCC_Wait();
    pTm->minutes = RTCVALH;   
    RTCC_ConvertTime(pTm);
}
#endif

#if defined(RTCCGETDATE) || defined(RTCCGETTIMEDATE)
void RTCC_GetDate(rtccDate* pDt)
{
    u8 dummy;

    RTCCFGbits.RTCPTR1 = 1;
    RTCCFGbits.RTCPTR0 = 1;
    RTCC_Wait();
    pDt->year = RTCVALL;
    RTCC_Wait();
    dummy = RTCVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    RTCC_Wait();
    pDt->dayofmonth = RTCVALL;
    RTCC_Wait();
    pDt->month = RTCVALH;
    RTCC_Wait();
    dummy = RTCVALL;
    RTCC_Wait();
    pDt->dayofweek = RTCVALH;
    RTCC_ConvertDate(pDt);
}
#endif

#ifdef RTCCGETTIMEDATE
void RTCC_GetTimeDate(rtccTime* pTm, rtccDate* pDt)
{
    rtccTime tm;
    rtccDate dt;

    RTCC_GetDate(&dt);
    RTCC_GetTime(&tm);
    pTm->l = tm.l;
    pDt->l = dt.l;
}
#endif

/*	-----------------------------------------------------------------------------
	adjust RTCC from +511 to -512 clock pulses every one minute
	0111111111 = Maximum positive adjustment, adds 511 RTC clock pulses every one minute
	1000000000 = Minimum negative adjustment, subtracts 512 clock pulses every one minute
 	10 dec 2011 : bug fixed thanks to Mark Harper
	---------------------------------------------------------------------------*/

#if defined(RTCCSETCALIBRATION) || defined(RTCCINIT)
void RTCC_SetCalibration(s16 cal)
{
    if (cal < -512)	cal = -512;
    if (cal >  511)	cal =  511;
    RTCC_SetWriteEnable();
    RTCCAL = cal;
    RTCC_SetWriteDisable();
}
#endif

/*	-----------------------------------------------------------------------------
	Return 10-bit clock pulse calibration
	---------------------------------------------------------------------------*/

#ifdef RTCCGETCALIBRATION
s16 RTCC_GetCalibration(void)
{
    return RTCCAL;
}
#endif

/*	-----------------------------------------------------------------------------
    The function initializes the RTCC device.
    *It enables the secondary oscillator (SOSC),
    *sets the desired time, date and calibration
    *enables the RTCC,
    *disables the Alarm and the RTCC clock output (RTCOE=0), 
    
    *NB : Pinguino x6j50 RTCC source is INTRC oscillator (cf. bootloader v4.x CONFIG3L<1>)
        : Pinguino 47j53 RTCC source is T1OSC oscillator (cf. bootloader v4.x CONFIG3L<1>)
	---------------------------------------------------------------------------*/

#ifdef RTCCINIT
void RTCC_init(u32 tm, u32 dt, s16 drift)
{
    #if defined(PINGUINO47J53A) || defined(PINGUINO47J53B)
    T1CONbits.T1OSCEN = 1;
    #endif
    
    RTCC_SetTime(tm);
    RTCC_SetDate(dt);
    RTCC_SetCalibration(drift);

    RTCC_Enable();
    //RTCC_AlarmDisable();
    //RTCC_OutputDisable();
}
#endif

#endif	/* __RTCC1__ */

