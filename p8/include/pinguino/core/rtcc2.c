/*	----------------------------------------------------------------------------
	FILE:  			rtcc2.c
	PROJECT: 		pinguino
	PURPOSE: 		Alarm Real Time Clock and Calendar funtions
	PROGRAMER: 		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	15 jun 2012
	LAST RELEASE:	23 Jan 2013
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

#ifndef __RTCC2__
#define __RTCC2__

#if !defined(__18f26j50) && !defined(__18f46j50) && \
    !defined(__18f27j53) && !defined(__18f47j53)
        #error "****************************************************"
        #error "*** Your Pinguino doesn't have a RTCC module     ***"
        #error "****************************************************"
#endif

#include <typedef.h>
#include <const.h>
#include <macro.h>
//#include <interrupt.h>
//#include <interrupt.c>
#include <rtcc.h>
#include <rtcc.c>
#include <bcd.c>

/*	-----------------------------------------------------------------------------
	ALRMEN: Alarm Enable bit(2,3)
	1 = Alarm is enabled
	0 = Alarm is disabled
	NB: Hardware clears the ALRMEN bit anytime the alarm event occurs
        ALRMEN bit can not be cleared if ALRMRPT <> 0 or CHIME = 1.
	(3)This field should not be written when the RTCC ON bit = ‘1’ (RTCCON<15>) and ALRMSYNC = 1.
	---------------------------------------------------------------------------*/

//Enable Alarm
#ifdef RTCCALARMENABLE
#define RTCC_AlarmEnable() do { ALRMCFGbits.ALRMEN = 1; } while(0)
#endif

// Disable Alarm
#ifdef RTCCALARMDISABLE
#define RTCC_AlarmDisable() do { ALRMCFGbits.ALRMEN = 0; } while(0)
#endif

#ifdef RTCCISALARMENABLE
u8 RTCC_IsAlarmEnable(void)
{
    return ALRMCFGbits.ALRMEN != 0;
}
#endif

/*	-----------------------------------------------------------------------------
    enable/disable indefinite repeatitions
	CHIME: Chime Enable bit(3)
	1 = Chime is enabled – ARPT<7:0> is allowed to rollover from 0x00 to 0xFF
	0 = Chime is disabled – ARPT<7:0> stops once it reaches 0x00
	(3)CHIME should not be written when the RTCC ON bit = ‘1’ (RTCCON<15>) and ALRMSYNC = 1.
	---------------------------------------------------------------------------*/

// Enable Chime
#ifdef RTCCCHIMEENABLE
#define RTCC_ChimeEnable() do { RTCC_Wait(); ALRMCFGbits.CHIME = 1; } while(0)
#endif

// Disable Chime
#ifdef RTCCCHIMEDISABLE
#define RTCC_ChimeDisable() do { RTCC_Wait(); ALRMCFGbits.CHIME = 0; } while(0)
#endif

#ifdef RTCCISCHIMEENABLE
u8 RTCC_IsChimeEnable(void)
{
    RTCC_Wait();
    return ALRMCFGbits.CHIME != 0;
}
#endif

/*	-----------------------------------------------------------------------------
	alarm repeat every year, month, week, day, hour minute ...
	---------------------------------------------------------------------------*/

#ifdef RTCCALARMEVERYHALFSECOND
#define RTCC_AlarmEveryHalfSecond() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_HALF_SECOND; } while (0)
#endif

#ifdef RTCCALARMEVERYSECOND
#define RTCC_AlarmEverySecond() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_SECOND; } while (0)
#endif

#ifdef RTCCALARMEVERYTENSECONDS
#define RTCC_AlarmEveryTenSeconds() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_TEN_SECONDS; } while (0)
#endif

#ifdef RTCCALARMEVERYMINUTE
#define RTCC_AlarmEveryMinute() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_MINUTE; } while (0)
#endif

#ifdef RTCCALARMEVERYTENMINUTES
#define RTCC_AlarmEveryTenMinutes() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_TEN_MINUTES; } while (0)
#endif

#ifdef RTCCALARMEVERYHOUR
#define RTCC_AlarmEveryHour() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_HOUR; } while (0)
#endif

#ifdef RTCCALARMEVERYDAY
#define RTCC_AlarmEveryDay() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_DAY; } while (0)
#endif

#ifdef RTCCALARMEVERYWEEK
#define RTCC_AlarmEveryWeek() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_WEEK; } while (0)
#endif

#ifdef RTCCALARMEVERYMONTH
#define RTCC_AlarmEveryMonth() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_MONTH; } while (0)
#endif

#ifdef RTCCALARMEVERYYEAR
#define RTCC_AlarmEveryYear() do { ALRMCFGbits.AMASK = RTCC_ALARM_EVERY_YEAR; } while (0)
#endif

/*	-----------------------------------------------------------------------------
    TODO : find another name
	---------------------------------------------------------------------------*/

#ifdef RTCCGETALARMINTERVAL
u8 RTCC_GetAlarmInterval(void)
{
    return ALRMCFGbits.AMASK;
}
#endif

/*	-----------------------------------------------------------------------------
	Alarm will trigger rptCnt times
    repeat alarm rptCnt times = rptCnt interrupt events
	rptCnt has to be a value less than 256
	----------------------------------------------------------------------------
	02-01-2012	Régis Blanchot		check if rptCnt is less than 256 by changing type int to type char
	---------------------------------------------------------------------------*/

#ifdef RTCCSETALARMREPEAT
void RTCC_SetAlarmRepeat(char rptCnt)
{
    RTCC_Wait();
    ALRMRPT = rptCnt;
}
#endif

/*	----------------------------------------------------------------------------
	Return Alarm Repeat Counter Value
	----------------------------------------------------------------------------
	02-01-2012	Régis Blanchot		changed type int to type char (ARPT<7:0>)
	--------------------------------------------------------------------------*/

#ifdef RTCCGETALARMREPEAT
u8 RTCC_GetAlarmRepeat(void)
{
    RTCC_Wait();
    return ALRMRPT;
}
#endif

/*	-----------------------------------------------------------------------------
	The function might wait for the proper Alarm window to safely perform
	the update of the Alarm Time registers.
	Interrupts are disabled shortly when properly probing the RTCSYNC needed.
	---------------------------------------------------------------------------*/

#if defined(RTCCSETALARMTIME) || defined(RTCCSETALARMTIMEDATE)
void RTCC_SetAlarmTime(u32 alTime)
{
    rtccTime t0;
    u8 dummy;
        
    t0.l = alTime;
    RTCC_SetWriteEnable();
    RTCC_Wait();
    ALRMCFG = 0x00;     		// clear the ALRMEN, CHIME, AMASK and ARPT;
    ALRMRPT = 0;                // clear Alarm repeat counter
    ALRMCFGbits.ALRMPTR1 = 0;     
    ALRMCFGbits.ALRMPTR0 = 1;     
    // ALRMPTR = 01 = Weekday/Hours
    ALRMVALL = t0.hours;     
    dummy = ALRMVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    // ALRMPTR = 00 = Minutes/Seconds
    ALRMVALL = t0.seconds;
    ALRMVALH = t0.minutes;   
    RTCC_SetWriteDisable();
}
#endif

/*	-----------------------------------------------------------------------------
	---------------------------------------------------------------------------*/

#if defined(RTCCGETALARMTIME) || defined(RTCCGETALARMTIMEDATE)
void RTCC_GetAlarmTime(rtccTime* pAlTime)
{
    u8 dummy;

    RTCC_Wait();
    ALRMCFGbits.ALRMPTR1 = 0;
    ALRMCFGbits.ALRMPTR0 = 1;
    pAlTime->hours = ALRMVALL;     
    dummy = ALRMVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    pAlTime->seconds = ALRMVALL;
    pAlTime->minutes = ALRMVALH;   
    RTCC_ConvertTime(pAlTime);
}
#endif

/*	-----------------------------------------------------------------------------
	---------------------------------------------------------------------------*/

#if defined(RTCCSETALARMDATE) || defined(RTCCSETALARMTIMEDATE)
void RTCC_SetAlarmDate(u32 alDate)
{
    rtccDate d0;
    u8 dummy;
        
    d0.l = alDate;
    RTCC_SetWriteEnable();
    RTCC_Wait();
    ALRMCFG = 0x00;     		// clear the ALRMEN, CHIME, AMASK and ARPT;
    ALRMRPT = 0;                // clear Alarm repeat counter
    ALRMCFGbits.ALRMPTR1 = 1;
    ALRMCFGbits.ALRMPTR0 = 1; 
    // ALRMPTR = 11 = Reserved/Year
    ALRMVALL = d0.year;
    dummy = ALRMVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    // ALRMPTR = 10 = Month/Day
    ALRMVALL = d0.dayofmonth;      
    ALRMVALH = d0.month;   
    // ALRMPTR = 01 = Weekday/Hours
    ALRMVALH = d0.dayofweek;
    // ALRMPTR = 00 = Minutes/Seconds
    RTCC_SetWriteDisable();
}
#endif

/*	-----------------------------------------------------------------------------
	---------------------------------------------------------------------------*/

#if defined(RTCCGETALARMDATE) || defined(RTCCGETALARMTIMEDATE)
void RTCC_GetAlarmDate(rtccDate* pAlDate)
{
    u8 dummy;

    RTCC_Wait();
    ALRMCFGbits.ALRMPTR1 = 1;
    ALRMCFGbits.ALRMPTR0 = 1;
    pAlDate->year = ALRMVALL;
    dummy = ALRMVALH;        // dummy read of RTCVALH to auto-decrement RTCPTR    
    pAlDate->dayofmonth = ALRMVALL;
    pAlDate->month = ALRMVALH;
    dummy = ALRMVALL;
    pAlDate->dayofweek = ALRMVALH;
    RTCC_ConvertDate(pAlDate);
}
#endif

/*	-----------------------------------------------------------------------------
	---------------------------------------------------------------------------*/

#ifdef RTCCSETALARMTIMEDATE
void RTCC_SetAlarmTimeDate(u32 alTime, u32 alDate)
{
    RTCC_SetTime(alTime);
    RTCC_SetDate(alDate);
}
#endif

/*	-----------------------------------------------------------------------------
	---------------------------------------------------------------------------*/

#ifdef RTCCGETALARMTIMEDATE
void RTCC_GetAlarmTimeDate(rtccTime* pAlTime, rtccDate* pAlDate)
{
    RTCC_GetAlarmTime(pAlTime);
    RTCC_GetAlarmDate(pAlDate);
}
#endif

/*	-----------------------------------------------------------------------------
	RTCSYNC: RTCC Value Registers Read Synchronization bit
	1 = RTC Value registers can change while reading, due to a rollover ripple
	that results in an invalid	data read
	If the register is read twice and results in the same data, the data can be
	assumed to be valid
	0 = RTC Value registers can be read without concern about a rollover ripple
	---------------------------------------------------------------------------*/
/*
void RTCC_SetSync(int sync)
{
    if (sync)
        RTCCFGbits.RTCSYNC = 1;
    else
        RTCCFGbits.RTCSYNC = 0;
}

int RTCC_GetSync(void)
{
    return RTCCFGbits.RTCSYNC!=0;
}
*/
/*	-----------------------------------------------------------------------------
	TRUE if the RTCC is in the second HALF SECOND  interval, FALSE otherwise
	HALFSEC: Half-Second Status bit(7)
	1 = Second half period of a second
	0 = First half period of a second
	(7)This bit is read-only.
	It is cleared to ‘0’ on a write to the seconds bit fields (RTCTIME<14:8>).
	---------------------------------------------------------------------------*/

#ifdef RTCCGETHALFSECOND
u8 RTCC_GetHalfSecond(void)
{
    return RTCCFGbits.HALFSEC!=0;
}
#endif

/*	-----------------------------------------------------------------------------
	RTCOE: RTCC Output Enable bit
	1 = RTCC clock output enabled – clock presented onto an I/O
	0 = RTCC clock output disabled
	---------------------------------------------------------------------------*/

#ifdef RTCCOUTPUTENABLE
#define RTCC_OutputEnable() do { RTCCFGbits.RTCOE = 1; } while(0)
#endif

#ifdef RTCCOUTPUTDISABLE
#define RTCC_OutputDisable() do { RTCCFGbits.RTCOE = 0; } while(0)
#endif

#ifdef RTCCISOUTPUTENABLE
u8 RTCC_IsOutputEnable(void)
{
    return RTCCFGbits.RTCOE != 0;
}
#endif

/*	-----------------------------------------------------------------------------
	RTCC Seconds Clock Output Select
	10 = RTCC source clock is selected for the RTCC pin
    01 = RTCC Seconds Clock is selected for the RTCC pin
	00 = RTCC Alarm Pulse is selected for the RTCC pin
	---------------------------------------------------------------------------*/

#ifdef RTCCCLOCKPULSEOUTPUT
#define RTCC_ClockPulseOutput() do { PADCFG1bits.RTSECSEL1 = 1; PADCFG1bits.RTSECSEL0 = 0; } while(0)
#endif

#ifdef RTCCSECONDSPULSEOUTPUT
#define RTCC_SecondsPulseOutput() do { PADCFG1bits.RTSECSEL1 = 0; PADCFG1bits.RTSECSEL0 = 1; } while(0)
#endif

#ifdef RTCCALARMPULSEOUTPUT
#define RTCC_AlarmPulseOutput() do { PADCFG1bits.RTSECSEL1 = 0; PADCFG1bits.RTSECSEL0 = 0; } while(0)
#endif

/*	-----------------------------------------------------------------------------
	RTCC Interrupt Initialization
	---------------------------------------------------------------------------*/

#ifdef RTCCALARMINTENABLE
void RTCC_AlarmIntEnable(void)
{
    RCONbits.IPEN = 1;      // Enable use of HP/LP interrupts
    IPR3bits.RTCCIP = 1;    // RTCC interrupt has high priority
    PIR3bits.RTCCIF = 0;    // Clear RTCC interrupt flag
    PIE3bits.RTCCIE = 1;    // Enable RTCC interrupt
    INTCONbits.GIEH = 1;    // Enable global HP interrupts
    INTCONbits.GIEL = 1;    // Enable global LP interrupts
}

void rtcc_interrupt()
{
    if (PIR3bits.RTCCIF)
    {
        PIR3bits.RTCCIF = 0;
    }
}
#endif

#ifdef RTCCALARMINTDISABLE
#define RTCC_AlarmIntDisable() do { PIE3bits.RTCCIE = 0; } while(0)
#endif

#endif	/* __RTCC2__ */
