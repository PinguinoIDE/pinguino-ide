/*	----------------------------------------------------------------------------
    FILE:  			rtcc.c
    PROJECT: 		pinguino32
    PURPOSE: 		Real Time Clock and Calendar functions
    PROGRAMER: 		regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	11 Apr 2011
    LAST RELEASE:	02 Jan 2012
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

#include <typedef.h>
#include <const.h>
#include <macro.h>
#include <system.c>
#include <interrupt.c>
#include <bcd.c>

// RTCC definitions

typedef void (*callback) (void);	// type of: void callback()
static callback intFunction;

// union/structure for read/write of time into the RTCC device
typedef union
{
    struct
    {
        unsigned char	rsvd;		// reserved for future use. should be 0
        unsigned char	seconds;		// BCD codification for seconds, 00-59
        unsigned char	minutes;		// BCD codification for minutes, 00-59
        unsigned char	hours;		// BCD codification for hours, 00-24
    };								// field access
    unsigned char		b[4];		// byte access
    unsigned short		w[2];		// 16 bits access
    unsigned long		l;			// 32 bits access
}rtccTime;

// union/structure for read/write of date into the RTCC device
typedef union
{
    struct
    {
        unsigned char	dayofweek;		// BCD codification for day of the week, 00-06
        unsigned char	dayofmonth;		// BCD codification for day of the month, 01-31
        unsigned char	month;		// BCD codification for month, 01-12
        unsigned char	year;		// BCD codification for years, 00-99
    };								// field access
    unsigned char		b[4];		// byte access
    unsigned short		w[2];		// 16 bits access
    unsigned long		l;			// 32 bits access
}rtccDate;

// results returned by initialization functions
typedef enum
{
    RTCC_SOSC_ON,					// success, SOSC is running
    RTCC_SOSC_NRDY,				// SOSC not running
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

// Prototypes
rtccTime RTCC_ConvertTime(rtccTime *);
rtccDate RTCC_ConvertDate(rtccDate *);
void RTCC_SetWriteEnable(void);
void RTCC_SetWriteDisable(void);
int RTCC_GetWriteEnable(void);
void RTCC_Enable(void);
void RTCC_Disable(void);
int RTCC_GetEnable(void);
rtccRes RTCC_GetSOSCstatus(void);
void RTCC_SOSCenable(void);
void RTCC_SOSCdisable(void);
void RTCC_SetTime(unsigned long);
rtccTime RTCC_GetTime(void);
void RTCC_SetDate(unsigned long);
rtccDate RTCC_GetDate(void);
void RTCC_SetTimeDate(unsigned long , unsigned long);
void RTCC_GetTimeDate(rtccTime*, rtccDate*);
void RTCC_SetCalibration(int);
int RTCC_GetCalibration(void);
void RTCC_SetAlarmEnable(int);
void RTCC_AlarmEnable(void);
void RTCC_AlarmDisable(void);
int RTCC_GetAlarmEnable(void);
void RTCC_SetChimeEnable(int);
void RTCC_ChimeEnable(void);
void RTCC_ChimeDisable(void);
int RTCC_GetChimeEnable(void);
void RTCC_SetAlarmRepeat(int);
void RTCC_AlarmEveryHalfSecond(void);
void RTCC_AlarmEverySecond(void);
void RTCC_AlarmEveryTenSeconds(void);
void RTCC_AlarmEveryMinute(void);
void RTCC_AlarmEveryTenMinutes(void);
void RTCC_AlarmEveryHour(void);
void RTCC_AlarmEveryDay(void);
void RTCC_AlarmEveryWeek(void);
void RTCC_AlarmEveryMonth(void);
void RTCC_AlarmEveryYear(void);
int RTCC_GetAlarmRepeat(void);
void RTCC_SetAlarmRepeatCount(char);
char RTCC_GetAlarmRepeatCount(void);
void RTCC_SetAlarmTime(unsigned long);
rtccTime RTCC_GetAlarmTime(void);
void RTCC_SetAlarmDate(unsigned long);
rtccDate RTCC_GetAlarmDate(void);
void RTCC_SetAlarmTimeDate(unsigned long, unsigned long);
void RTCC_GetAlarmTimeDate(rtccTime*, rtccDate*);
void RTCC_SetSync(int);
int RTCC_GetSync(void);
void RTCC_SetAlarmSync(int);
int RTCC_GetAlarmSync(void);
int RTCC_GetHalfSecond(void);
void RTCC_SecondsPulseOutput(void);
void RTCC_AlarmPulseOutput(void);
void RTCC_SetAlarmInitialPulse(int);
void RTCC_AlarmInitialPulseHigh(void);
void RTCC_AlarmInitialPulseLow(void);
int RTCC_GetAlarmInitialPulse(void);
void RTCC_AlarmInitialPulseToggle(void);
void RTCC_SetOutputEnable(int);
void RTCC_OutputEnable(void);
void RTCC_OutputDisable(void);
int RTCC_GetOutputEnable(void);
void RTCC_SetAlarmIntEnable(int);
void RTCC_AlarmIntEnable(void);
void RTCC_AlarmIntDisable(void);
void OnRTCC(callback);
void RTCCInterrupt(void);
void RTCC_Shutdown(void);
//void RTCC_init(void);
//rtccRes RTCC_Open(unsigned long, unsigned long, int);
rtccRes RTCC_init(unsigned long, unsigned long, int);

/*	----------------------------------------------------------------------------
    RTCC REGISTERS
    ----------------------------------------------------------------------------
    RTCCON
    RTCALRM
    RTCTIME
    RTCDATE
    ALRMTIME
    ALRMDATE 
    --------------------------------------------------------------------------*/

/*	-----------------------------------------------------------------------------
    Conversion routines from bcd to decimal format
    ---------------------------------------------------------------------------*/

rtccTime RTCC_ConvertTime(rtccTime *pTm)
{
    rtccTime t0;
    
    t0.hours = bcd2bin(pTm->hours);
    t0.minutes  = bcd2bin(pTm->minutes);
    t0.seconds  = bcd2bin(pTm->seconds);
    return t0;
}

rtccDate RTCC_ConvertDate(rtccDate *pDt)
{
    rtccDate d0;
    
    d0.dayofweek = bcd2bin(pDt->dayofweek);
    d0.dayofmonth = bcd2bin(pDt->dayofmonth);
    d0.month  = bcd2bin(pDt->month);
    d0.year = bcd2bin(pDt->year);
    return d0;
}

/*	-----------------------------------------------------------------------------
    Allow a writing into the clock registers
    In order to perform a write to any of the RTCC timer registers, the RTCWREN bit (RTCCON<3>)
    must be set. Setting of the RTCWREN bit is only allowed once the device level unlocking
    sequence has been executed.
    ---------------------------------------------------------------------------*/

void RTCC_SetWriteEnable(void)
{
    // assume interrupts are disabled
    // assume the DMA controller is suspended
    // assume the device is locked
    SystemUnlock();
    RTCCONSET = 0x8; // set RTCWREN (bit 3) in RTCCON
    SystemLock();
    // re-enable interrupts
    // re-enable the DMA controller
}

void RTCC_SetWriteDisable(void)
{
    // assume interrupts are disabled
    // assume the DMA controller is suspended
    // assume the device is locked
    SystemUnlock();
    RTCCONCLR = 0x8; // clear RTCWREN (bit 3) in RTCCON
    SystemLock();
    // re-enable interrupts
    // re-enable the DMA controller
}

/*	-----------------------------------------------------------------------------
    Check that RTCC is writable
    ---------------------------------------------------------------------------*/

int RTCC_GetWriteEnable(void)
{
     return RTCCONbits.RTCWREN!=0;
}

/*	-----------------------------------------------------------------------------
    RTCC module is enabled
    The write operations have to be enabled in order to be able to toggle the ON control bit.
    The function doesn't wait for the RTC clock to be on.
    ---------------------------------------------------------------------------*/

void RTCC_Enable(void)
{
    RTCC_SetWriteEnable();
    //RTCCON |= (1 << 15);
    RTCCONSET = 0x8000;			// turn on the RTCC
    while (!(RTCCON & 0x40));	// wait for clock to be turned on
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------	
    RTCC module is disabled
    The write operations have to be enabled in order to be able to toggle the ON control bit.
    When ON control bit is set to 0, RTCCON.RTCSYNC, RTCCON.HALFSEC and RTCCON.RTCOE are asynchronously reset
    The function waits for the RTC clock to be off.
    ---------------------------------------------------------------------------*/

void RTCC_Disable(void)
{
    RTCC_SetWriteEnable();
    //RTCCON |= !(1 << 15);
    RTCCONCLR = 0x8000;			// turn off the RTCC
    // TODO: RTCCON.RTCSYNC, RTCCON.HALFSEC and RTCCON.RTCOE
    while (RTCCON & 0x40);		// wait for clock to be turned off
    // bit 6 RTCCLKON: RTCC Clock Enable Status bit
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------
    TODO: check if it works
    ---------------------------------------------------------------------------*/

int RTCC_GetEnable(void)
{
    if (RTCCON & 0x8000)
        return 1;
    else
        return 0;
}

/*	-----------------------------------------------------------------------------
    Check that the Secondary Oscillator (SOSC) is running.
    ---------------------------------------------------------------------------*/

rtccRes RTCC_GetSOSCstatus(void)
{
    // 01-01-2012 bug fixed by Mark Harper
    if ( (!(OSCCONbits.SOSCEN)) || (!(OSCCONbits.SOSCRDY)) )
        return RTCC_SOSC_NRDY;
    /*
    else if (!(RTCCONbits.RTCCLKON))	// At start up RTCC Clock is not enabled so this
                                        // test will ALWAYS fail.
                                        // It is NOT related to SOSC running or not running
    return RTCC_CLK_NRDY;
    */
    return RTCC_SOSC_ON;
}

/*	-----------------------------------------------------------------------------
    Enable the secondary oscillator (SOSC) for the RTCC
    To allow the RTCC to be clocked by an external 32.768 kHz crystal, the SOSCEN bit
    (OSCCON<1>) must be set (refer to Register 6-1 in Section 6. “Oscillators” (DS61112)) in the
    "PIC32 Family Reference Manual”. This is the only bit outside of the RTCC module with which
    the user must be concerned for enabling the RTCC. 
    ----------------------------------------------------------------------------
    01-01-2012	Mark Harper		function renamed from RTCC_StartSOSC to RTCC_SOSCenable
    ---------------------------------------------------------------------------*/

void RTCC_SOSCenable(void)
{
    SystemUnlock();
    OSCCONbits.SOSCEN = 1;
    SystemLock();
    // Wait for stable SOCSC oscillator output before enabling the RTCC.
    // This typically requires a 32 ms delay between enabling the SOSC and enabling the RTCC.
    // 1st option
    //	Delayms(50);
    // 2nd option
    // Wait for the SOSC to run
    while (RTCC_GetSOSCstatus() != RTCC_SOSC_ON);
}

/*	-----------------------------------------------------------------------------
    Disable the secondary oscillator (SOSC) for the RTCC
    ----------------------------------------------------------------------------
    01-01-2012	Mark Harper		function renamed from RTCC_StopClock to RTCC_SOSCdisable
    ---------------------------------------------------------------------------*/

void RTCC_SOSCdisable(void)
{
    SystemUnlock();
    OSCCONbits.SOSCEN = 0;
    SystemLock();
}

/*	-----------------------------------------------------------------------------
    The write is successful only if Write Enable is set.
    The function will enable the write itself.
    The routine wait for the CLK to be running before returning.
    The routine could disable the interrupts for a very short time to be able
    to update the time and date registers.
    ----------------------------------------------------------------------------
    02-01-2012	Régis Blanchot		replaced Delayus(50); by while (RTCCON & 0x40);
    ---------------------------------------------------------------------------*/

void RTCC_SetTime(unsigned long tm)
{
    RTCC_SetWriteEnable();
    RTCCONCLR = 0x8000;			// turn off the RTCC
    //Delayus(50);
    while (RTCCON & 0x40);		// wait for clock to be turned off
    RTCTIME = tm;				// Set time
    RTCCONSET = 0x8000;			// turn on the RTCC
    //Delayus(50);
    while (!(RTCCON & 0x40));	// wait for clock to be turned on
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

rtccTime RTCC_GetTime(void)
{
    rtccTime t0, t1;
    do
    {
        t0.l = RTCTIME;
        t1.l = RTCTIME;
    } while (t0.l != t1.l);
    return t0;
}

/*	-----------------------------------------------------------------------------
    The write is successful only if Write Enable is set.
    The function will enable the write itself.
    The routine wait for the CLK to be running before returning.
    The routine could disable the interrupts for a very short time to be able
    to update the time and date registers.
    ----------------------------------------------------------------------------
    02-01-2012	Régis Blanchot		replaced Delayus(50); by while (RTCCON & 0x40);
    ---------------------------------------------------------------------------*/

void RTCC_SetDate(unsigned long dt)
{
    RTCC_SetWriteEnable();
    RTCCONCLR = 0x8000;			// turn off the RTCC
    //Delayus(50);
    while (RTCCON & 0x40);		// wait for clock to be turned off
    RTCDATE = dt;				// Set date
    RTCCONSET = 0x8000;			// turn on the RTCC
    //Delayus(50);
    while (!(RTCCON & 0x40));	// wait for clock to be turned on
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

rtccDate RTCC_GetDate(void)
{
    rtccDate d0, d1;
    do
    {
        d0.l = RTCDATE;
        d1.l = RTCDATE;
    } while (d0.l != d1.l);
    return d0;
}

/*	-----------------------------------------------------------------------------
    The write is successful only if Write Enable is set.
    The function will enable the write itself.
    The routine wait for the CLK to be running before returning.
    The routine could disable the interrupts for a very short time to be able
    to update the time and date registers.
    ----------------------------------------------------------------------------
    02-01-2012	Régis Blanchot		replaced Delayus(50); by while (RTCCON & 0x40);
    ---------------------------------------------------------------------------*/

void RTCC_SetTimeDate(unsigned long tm, unsigned long dt)
{
    RTCC_SetWriteEnable();
    RTCCONCLR = 0x8000;			// turn off the RTCC
    //Delayus(50);
    while (RTCCON & 0x40);		// wait for clock to be turned off
    RTCTIME = tm;				// Set time
    RTCDATE = dt;				// Set date
    RTCCONSET = 0x8000;			// turn on the RTCC
    //Delayus(50);
    while (!(RTCCON & 0x40));	// wait for clock to be turned on
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

void RTCC_GetTimeDate(rtccTime* pTm, rtccDate* pDt)
{
    rtccTime	t0;
    rtccDate	d0;
    do
    {
        d0.l = RTCDATE;
        t0.l = RTCTIME;
        pTm->l = RTCTIME;
        pDt->l = RTCDATE;
    } while ((d0.l != pDt->l) || (t0.l != pTm->l)); // update the user requested data
}

/*	-----------------------------------------------------------------------------
    adjust RTCC from +511 to -512 clock pulses every one minute
    0111111111 = Maximum positive adjustment, adds 511 RTC clock pulses every one minute
    1000000000 = Minimum negative adjustment, subtracts 512 clock pulses every one minute
    10 dec 2011 : bug fixed thanks to Mark Harper
    ---------------------------------------------------------------------------*/

void RTCC_SetCalibration(int cal)
{
    rtccTime t0;

    if (cal < -512)	cal = -512;
    if (cal >  511)	cal =  511;
    if (RTCCON & 0x8000)					// if RTCC is ON
    {
        t0 = RTCC_GetTime();
        if ((t0.seconds & 0xFF) == 00)			// we're at second 00, wait auto-adjust to be performed
            while(!(RTCCON & 0x2));			// wait until second half...
    }
    RTCC_SetWriteEnable();
    RTCCONCLR = 0x03FF0000;					// clear the calibration
    RTCCONbits.CAL = cal;
    RTCC_SetWriteDisable();
}

/*	-----------------------------------------------------------------------------
    Return 10-bit clock pulse calibration
    ---------------------------------------------------------------------------*/

int RTCC_GetCalibration(void)
{
     return RTCCONbits.CAL;
}

/*	-----------------------------------------------------------------------------
    ALRMEN: Alarm Enable bit(2,3)
    1 = Alarm is enabled
    0 = Alarm is disabled
    (2)Hardware clears the ALRMEN bit anytime the alarm event occurs,
    when ARPT<7:0> = 00 and	CHIME = 0.
    (3)This field should not be written when the RTCC ON bit = ‘1’ (RTCCON<15>) and ALRMSYNC = 1.
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmEnable(int enable)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCC_Disable();
    if (enable)
        RTCALRMbits.ALRMEN = 1;   	// Alarm is enabled
    else
        RTCALRMbits.ALRMEN = 0;   	// Alarm is disabled
    RTCC_Enable();
}

void RTCC_AlarmEnable(void)
{
    RTCC_SetAlarmEnable(True);
}

void RTCC_AlarmDisable(void)
{
    RTCC_SetAlarmEnable(False);
}

int RTCC_GetAlarmEnable(void)
{
    int	isAlrm0, isAlrm1;
    do
    {
        isAlrm0=RTCALRMbits.ALRMEN;
        isAlrm1=RTCALRMbits.ALRMEN;
    } while (isAlrm0 != isAlrm1);

    return isAlrm0;
}

/*	-----------------------------------------------------------------------------
    CHIME: Chime Enable bit(3)
    1 = Chime is enabled – ARPT<7:0> is allowed to rollover from 0x00 to 0xFF
    0 = Chime is disabled – ARPT<7:0> stops once it reaches 0x00
    (3)CHIME should not be written when the RTCC ON bit = ‘1’ (RTCCON<15>) and ALRMSYNC = 1.
    ---------------------------------------------------------------------------*/

void RTCC_SetChimeEnable(int enable)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    //RTCC_Disable();
    if (enable)
        RTCALRMbits.CHIME = 1;   	// chime is on
    else
        RTCALRMbits.CHIME = 0;   	// chime is off
    //RTCC_Enable();
}

void RTCC_ChimeEnable(void)
{
    RTCC_SetChimeEnable(True);
}

void RTCC_ChimeDisable(void)
{
    RTCC_SetChimeEnable(False);
}

int RTCC_GetChimeEnable(void)
{
    int	ch0, ch1;
    do
    {
        ch0 = RTCALRMbits.CHIME;
        ch1 = RTCALRMbits.CHIME;
    }while (ch0 != ch1);

    return ch0;
}

/*	-----------------------------------------------------------------------------
    alarm repeat every year, month, week, day, hour minute ...
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmRepeat(int rpt)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCALRMbits.AMASK = rpt;
}

void RTCC_AlarmEveryHalfSecond(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_HALF_SECOND);
}
void RTCC_AlarmEverySecond(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_SECOND);
}
void RTCC_AlarmEveryTenSeconds(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_TEN_SECONDS);
}
void RTCC_AlarmEveryMinute(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_MINUTE);
}
void RTCC_AlarmEveryTenMinutes(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_TEN_MINUTES);
}
void RTCC_AlarmEveryHour(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_HOUR);
}
void RTCC_AlarmEveryDay(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_DAY);
}
void RTCC_AlarmEveryWeek(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_WEEK);
}
void RTCC_AlarmEveryMonth(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_MONTH);
}
void RTCC_AlarmEveryYear(void)
{
    RTCC_SetAlarmRepeat(RTCC_ALARM_EVERY_YEAR);
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

int RTCC_GetAlarmRepeat(void)
{
    int r0, r1;

    do
    {
        r0 = RTCALRMbits.AMASK;
        r1 = RTCALRMbits.AMASK;
    }while (r0 != r1);

    return r0;
}

/*	-----------------------------------------------------------------------------
    Alarm will trigger rptCnt times
    rptCnt has to be a value less than 256
    ----------------------------------------------------------------------------
    02-01-2012	Régis Blanchot		check if rptCnt is less than 256 by changing type int to type char
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmRepeatCount(char rptCnt)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCALRMbits.ARPT = rptCnt;
}

/*	----------------------------------------------------------------------------
    Return Alarm Repeat Counter Value
    ----------------------------------------------------------------------------
    02-01-2012	Régis Blanchot		changed type int to type char (ARPT<7:0>)
    --------------------------------------------------------------------------*/

char RTCC_GetAlarmRepeatCount(void)
{
    char rpt0, rpt1;
    do
    {
        rpt0 = RTCALRMbits.ARPT;
        rpt1 = RTCALRMbits.ARPT;
    } while (rpt0 != rpt1);

    return rpt0;
}

/*	-----------------------------------------------------------------------------
    The function might wait for the proper Alarm window to safely perform
    the update of the ALRMTIME register.
    Interrupts are disabled shortly when properly probing the ALRMSYNC needed.
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmTime(unsigned long alTime)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCALRMCLR = 0xCFFF;		// clear the ALRMEN, CHIME, AMASK and ARPT;
    ALRMTIME = alTime;
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

rtccTime RTCC_GetAlarmTime(void)
{
    rtccTime t0;
    t0.l = ALRMTIME;
    return t0;
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmDate(unsigned long alDate)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCALRMCLR = 0xCFFF;			// clear the ALRMEN, CHIME, AMASK and ARPT;
    ALRMDATE = alDate;
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

rtccDate RTCC_GetAlarmDate(void)
{
    rtccDate d0;
    d0.l = ALRMDATE;
    return d0;
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmTimeDate(unsigned long alTime, unsigned long alDate)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCALRMCLR = 0xCFFF;			// clear the ALRMEN, CHIME, AMASK and ARPT;
    ALRMTIME = alTime;
    ALRMDATE = alDate;
}

/*	-----------------------------------------------------------------------------
    ---------------------------------------------------------------------------*/

void RTCC_GetAlarmTimeDate(rtccTime* pTm, rtccDate* pDt)
{
    pTm->l = ALRMTIME;
    pDt->l = ALRMDATE;
}

/*	-----------------------------------------------------------------------------
    RTCSYNC: RTCC Value Registers Read Synchronization bit
    1 = RTC Value registers can change while reading, due to a rollover ripple
    that results in an invalid	data read
    If the register is read twice and results in the same data, the data can be
    assumed to be valid
    0 = RTC Value registers can be read without concern about a rollover ripple
    ---------------------------------------------------------------------------*/

void RTCC_SetSync(int sync)
{
    if (sync)
        RTCCONbits.RTCSYNC = 1;
    else
        RTCCONbits.RTCSYNC = 0;
}

int RTCC_GetSync(void)
{
     return RTCCONbits.RTCSYNC!=0;
}

void RTCC_SetAlarmSync(int sync)
{
    if (sync)
        RTCALRMbits.ALRMSYNC = 1;
    else
        RTCALRMbits.ALRMSYNC = 0;
}

int RTCC_GetAlarmSync(void)
{
    return RTCALRMbits.ALRMSYNC!=0;
}

/*	-----------------------------------------------------------------------------
    TRUE if the RTCC is in the second HALF SECOND  interval, FALSE otherwise
    HALFSEC: Half-Second Status bit(7)
    1 = Second half period of a second
    0 = First half period of a second
    (7)This bit is read-only.
    It is cleared to ‘0’ on a write to the seconds bit fields (RTCTIME<14:8>).
    ---------------------------------------------------------------------------*/

int RTCC_GetHalfSecond(void)
{
     return RTCCONbits.HALFSEC!=0;
}

/*	-----------------------------------------------------------------------------
    RTSECSEL: RTCC Seconds Clock Output Select bit(5)
    1 = RTCC Seconds Clock is selected for the RTCC pin
    0 = RTCC Alarm Pulse is selected for the RTCC pin
    TODO:(5) Requires RTCOE = 1 (RTCCON<0>) for the output to be active.
    ---------------------------------------------------------------------------*/

void RTCC_SecondsPulseOutput(void)
{
        RTCCONbits.RTSECSEL = 1;	// RTCC Seconds Clock is selected for the RTCC pin
}

void RTCC_AlarmPulseOutput(void)
{
        RTCCONbits.RTSECSEL = 0;	// RTCC Alarm Pulse is selected for the RTCC pin
}

/*	-----------------------------------------------------------------------------
    PIV: Alarm Pulse Initial Value bit(3)
    When ALRMEN = 0, PIV is writable and determines the initial value of the Alarm Pulse.
    When ALRMEN = 1, PIV is read-only and returns the state of the Alarm Pulse.
    (3)This field should not be written when the RTCC ON bit = ‘1’ (RTCCON<15>) and ALRMSYNC = 1.
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmInitialPulse(int enable)
{
    while (RTCALRM & 0x1000);	// wait ALRMSYNC to be off
    RTCC_Disable();				// 
    RTCC_SetAlarmEnable(False);// Alarm is disabled, PIV is writable
    if (enable)
        RTCALRMbits.PIV = 1;		// the initial value of the Alarm Pulse is high
    else
        RTCALRMbits.PIV = 0;		// the initial value of the Alarm Pulse is low
    RTCC_SetAlarmEnable(True);	// Alarm is enabled
    RTCC_Enable();					// 
}

/*	-----------------------------------------------------------------------------
    1. The RTCC has to be enabled for the output to actually be active.
    2. This Alarm Pulse output is writable only when the alarm is disabled.
    3. The function might wait for the proper Alarm window
        to safely perform the update of the RTCALRM register.
    4. Interrupts are disabled shortly when properly probing the ALRMSYNC needed.
    ---------------------------------------------------------------------------*/
 
void RTCC_AlarmInitialPulseHigh(void)
{
    RTCC_SetAlarmInitialPulse(HIGH); 
}

void RTCC_AlarmInitialPulseLow(void)
{
    RTCC_SetAlarmInitialPulse(LOW);
}

int RTCC_GetAlarmInitialPulse(void)
{
    return RTCALRMbits.PIV!=0;
}

void RTCC_AlarmInitialPulseToggle(void)
{
    if (RTCC_GetAlarmInitialPulse())
        RTCC_SetAlarmInitialPulse(LOW);
    else
        RTCC_SetAlarmInitialPulse(HIGH);
}

/*	-----------------------------------------------------------------------------
    RTCOE: RTCC Output Enable bit(8)
    1 = RTCC clock output enabled – clock presented onto an I/O
    0 = RTCC clock output disabled
    (8)This bit is ANDed with the ON bit (RTCCON<15>) to produce the effective
    RTCC output enable.
    ---------------------------------------------------------------------------*/

void RTCC_SetOutputEnable(int enable)
{
    if(enable)
        RTCCONbits.RTCOE = 1;
    else
        RTCCONbits.RTCOE = 0;
}

void RTCC_OutputEnable(void)
{
    RTCC_SetOutputEnable(True);
}

void RTCC_OutputDisable(void)
{
    RTCC_SetOutputEnable(False);
}

int RTCC_GetOutputEnable(void)
{
     return RTCCONbits.RTCOE!=0;
}

/*	-----------------------------------------------------------------------------
    RTCC Interrupt Initialization
    ---------------------------------------------------------------------------*/

void RTCC_SetAlarmIntEnable(int enable)
{
    if (enable)
    {
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_RTCC_VECTOR, INT_PRIORITY_3, INT_SUBPRIORITY_1);
        IntClearFlag(INT_REAL_TIME_CLOCK);
        IntEnable(INT_REAL_TIME_CLOCK);
    }
    else
    {
        IntDisable(INT_REAL_TIME_CLOCK);
    }
}

void RTCC_AlarmIntEnable(void)
{
    RTCC_SetAlarmIntEnable(True);
}

void RTCC_AlarmIntDisable(void)
{
    RTCC_SetAlarmIntEnable(False);
}

void OnRTCC(callback userfunc)
{
    intFunction = userfunc;
}

/*	-----------------------------------------------------------------------------
    RTCC Interrupt Routine
    This one is private
    ---------------------------------------------------------------------------*/

void RTCCInterrupt(void)
{
    if (IntGetFlag(INT_REAL_TIME_CLOCK))
    {
        IntClearFlag(INT_REAL_TIME_CLOCK);
        intFunction();
    }
}

/*	-----------------------------------------------------------------------------
    The function initializes the RTCC device.
        *enables the secondary oscillator (SOSC),
        *enables the RTCC,
        *disables RTCC write,
        *disables the Alarm and the RTCC clock ouput (RTCOE=0),
        *clears the alarm interrupt flag.
    ---------------------------------------------------------------------------*/
/*
void RTCC_init(void)
{
    RTCC_SOSCenable();
    RTCC_Enable();
    RTCC_SetWriteDisable();
    RTCC_AlarmDisable();
    RTCC_OutputDisable();
    IntClearFlag(INT_REAL_TIME_CLOCK);
}
*/
/*	-----------------------------------------------------------------------------
    The function shutdowns the RTCC device.
        *It disables the secondary oscillator (SOSC),
        *disables the RTCC
        *disables RTCC write
        *disables the Alarm and the RTCC clock output (RTCOE=0).
        *clears the alarm interrupt flag.
    ---------------------------------------------------------------------------*/

void RTCC_Shutdown(void)
{
    RTCC_SOSCdisable();
    RTCC_Disable();
    RTCC_SetWriteDisable();
    RTCC_AlarmDisable();
    RTCC_OutputDisable();
    IntClearFlag(INT_REAL_TIME_CLOCK);
}

/*	-----------------------------------------------------------------------------
    The function initializes the RTCC device.
        *It enables the secondary oscillator (SOSC),
        *sets the desired time, date and calibration
        *enables the RTCC,
        *disables the Alarm and the RTCC clock output (RTCOE=0), 
        *disables RTCC writes.
        *clears the alarm interrupt flag.
    ---------------------------------------------------------------------------*/

rtccRes RTCC_init(unsigned long tm, unsigned long dt, int drift)
{
    RTCC_SOSCenable();
    RTCC_SetTime(tm);
    RTCC_SetDate(dt);
    RTCC_SetCalibration(drift);
    RTCC_Enable();
    RTCC_AlarmDisable();
    RTCC_OutputDisable();
    RTCC_SetWriteDisable();
    IntClearFlag(INT_REAL_TIME_CLOCK);
}

#endif	/* __RTCC__ */

