/*	----------------------------------------------------------------------------
	FILE:  			rtcc.h
	PROJECT: 		pinguino
	PURPOSE: 		Real Time Clock and Calendar functions
	PROGRAMER: 		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	15 Jun 2012
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

#ifndef __RTCC_H__
#define __RTCC_H__

#if !defined(__18f26j50) && !defined(__18f46j50) && \
    !defined(__18f27j53) && !defined(__18f47j53)
        #error "****************************************************"
        #error "*** Your Pinguino doesn't have a RTCC module     ***"
        #error "****************************************************"
#endif

#include <typedef.h>

// RTCC definitions

//typedef void (*callback) (void);	// type of: void callback()
//static callback intFunction;

// union/structure for read/write of time into the RTCC device
typedef union
{
    struct
    {
        u8	seconds;	// BCD codification for seconds, 00-59
        u8	minutes;	// BCD codification for minutes, 00-59
        u8	hours;		// BCD codification for hours, 00-24
        u8  nc;         // not used
    };					// field access
    u8		b[4];		// byte access
    u16		w[2];		// 16 bits access
    u32		l;			// 32 bits access
}rtccTime;

// union/structure for read/write of date into the RTCC device
typedef union
{
    struct
    {
        u8	dayofweek;	// BCD codification for day of the week, 00-06
        u8	dayofmonth;	// BCD codification for day of the month, 01-31
        u8	month;		// BCD codification for month, 01-12
        u8	year;		// BCD codification for years, 00-99
    };								// field access
    u8		b[4];		// byte access
    u16		w[2];		// 16 bits access
    u32		l;			// 32 bits access
}rtccDate;

// results returned by initialization functions
typedef enum
{
    RTCC_SOSC_ON,					// success, SOSC is running
    RTCC_SOSC_NRDY,				    // SOSC not running
    RTCC_CLK_NRDY,					// RTCC clock not running
    RTCC_WR_DSBL,					// WR is disabled
}rtccRes;

// Repeat alarm every ...
#define RTCC_ALARM_EVERY_HALF_SECOND	0b0000
#define RTCC_ALARM_EVERY_SECOND 		0b0001
#define RTCC_ALARM_EVERY_TEN_SECONDS	0b0010
#define RTCC_ALARM_EVERY_MINUTE 		0b0011
#define RTCC_ALARM_EVERY_TEN_MINUTES	0b0100
#define RTCC_ALARM_EVERY_HOUR 			0b0101
#define RTCC_ALARM_EVERY_DAY  			0b0110
#define RTCC_ALARM_EVERY_WEEK 			0b0111
#define RTCC_ALARM_EVERY_MONTH 			0b1000
#define RTCC_ALARM_EVERY_YEAR 			0b1001 // except when configured for February 29th, once every 4 years

// Prototypes RTCC

void RTCC_ConvertTime(rtccTime*);
void RTCC_ConvertDate(rtccDate*);

void RTCC_SetWriteEnable(void);
void RTCC_SetWriteDisable(void);
u8 RTCC_IsWriteEnable(void);
void RTCC_Enable(void);
void RTCC_Disable(void);
u8 RTCC_IsEnable(void);
void RTCC_Wait(void);

void RTCC_Shutdown(void);

// Prototypes RTCC1

void RTCC_init(u32, u32, s16);

void RTCC_SetTime(u32);
void RTCC_SetDate(u32);
void RTCC_SetTimeDate(u32, u32);

void RTCC_GetTime(rtccTime*);
void RTCC_GetDate(rtccDate*);
void RTCC_GetTimeDate(rtccTime*, rtccDate*);

void RTCC_SetCalibration(s16);
s16 RTCC_GetCalibration(void);

// Prototypes RTCC2

//void RTCC_AlarmEnable(void);
//void RTCC_AlarmDisable(void);
u8 RTCC_IsAlarmEnable(void);

//void RTCC_ChimeEnable(void);
//void RTCC_ChimeDisable(void);
u8 RTCC_IsChimeEnable(void);
/*
void RTCC_AlarmRepeatEveryHalfSecond(void);
void RTCC_AlarmRepeatEverySecond(void);
void RTCC_AlarmRepeatEveryTenSeconds(void);
void RTCC_AlarmRepeatEveryMinute(void);
void RTCC_AlarmRepeatEveryTenMinutes(void);
void RTCC_AlarmRepeatEveryHour(void);
void RTCC_AlarmRepeatEveryDay(void);
void RTCC_AlarmRepeatEveryWeek(void);
void RTCC_AlarmRepeatEveryMonth(void);
void RTCC_AlarmRepeatEveryYear(void);
*/
u8 RTCC_GetAlarmRepeat(void);
void RTCC_SetAlarmRepeatCount(char);
u8 RTCC_GetAlarmRepeatCount(void);

void RTCC_SetAlarmTime(u32);
void RTCC_GetAlarmTime(rtccTime*);
void RTCC_SetAlarmDate(u32);
void RTCC_GetAlarmDate(rtccDate*);
void RTCC_SetAlarmTimeDate(u32, u32);
void RTCC_GetAlarmTimeDate(rtccTime*, rtccDate*);

u8 RTCC_GetHalfSecond(void);

//void RTCC_ClockPulseOutput(void);
//void RTCC_SecondsPulseOutput(void);
//void RTCC_AlarmPulseOutput(void);

//void RTCC_OutputEnable(void);
//void RTCC_OutputDisable(void);
u8 RTCC_GetOutputEnable(void);

void RTCC_AlarmIntEnable(void);
//void RTCC_AlarmIntDisable(void);

#endif	/* __RTCC_H__ */
