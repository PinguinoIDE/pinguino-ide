/*  ----------------------------------------------------------------------------
    FILE:           lowpower.c
    PROJECT:        pinguino
    PURPOSE:        pinguino system functions
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  5 Jan. 2011
    LAST RELEASE:   2 Jan. 2013
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

#ifndef __LOWPOWER_C
#define __LOWPOWER_C

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>

#define DEEPSLEEPFAULTWAKEUP   1<<7
#define ULTRALOWPOWERWAKEUP    1<<5
#define WATCHDOGWAKEUP         1<<4
#define RTCCWAKEUP             1<<3
#define MCLRWAKEUP             1<<2
#define PINWAKEUP              1<<1
#define POWERONWAKEUP          1<<0

/*  ----------------------------------------------------------------------------
    Device and on-chip regulator enter sleep mode on SLEEP instruction
    --------------------------------------------------------------------------*/
#ifdef SYSTEMSLEEP
#define SystemSleep()           do { WDTCONbits.REGSLP = 1; OSCCONbits.IDLEN = 0; sleep(); } while (0)
#endif

/*  ----------------------------------------------------------------------------
    Device enters idle mode on SLEEP instruction
    --------------------------------------------------------------------------*/
#ifdef SYSTEMIDLE
#define SystemIdle()            do { OSCCONbits.IDLEN = 1; sleep(); } while (0)
#endif

/*  ----------------------------------------------------------------------------
    Device enters run mode (normal mode)
    --------------------------------------------------------------------------*/
#ifdef SYSTEMRUN
#define SystemRun()             do { DSCONHbits.DSEN = 0; OSCCONbits.SCS = 0; } while (0)
#endif

/*  ----------------------------------------------------------------------------
    Save 16-bit context in deep sleep persistent general purpose registers
    --------------------------------------------------------------------------*/

#if defined(__18f26j50) || defined(__18f46j50)

#ifdef SYSTEMDEEPSLEEPSAVECONTEXT
void SystemDeepSleepSaveContext(u16 save)
{
    DSGPR0 = highByte(save);  // deep sleep persistent general purpose register
    DSGPR1 = lowByte(save);   // use to save context
}
#endif

#endif /* defined(__18f26j50) || defined(__18f46j50) */

/*  ----------------------------------------------------------------------------
    Restore 16-bit context from deep sleep persistent general purpose registers
    --------------------------------------------------------------------------*/

#if defined(__18f26j50) || defined(__18f46j50)

#ifdef SYSTEMDEEPSLEEPRESTORECONTEXT
u16 SystemDeepSleepRestoreContext()
{
    u8 saveL, saveH;
    
    saveL = DSGPR0;      // deep sleep persistent general purpose register
    saveH = DSGPR1;      // return context
    return make16(saveL, saveH);
}
#endif

#endif /* defined(__18f26j50) || defined(__18f46j50) */

/*  ----------------------------------------------------------------------------
    Device enters deep sleep mode on SLEEP instruction
    TODO : give Wake-up source (not only RTCC)
    --------------------------------------------------------------------------*/

#if defined(__18f26j50) || defined(__18f46j50)

#ifdef SYSTEMDEEPSLEEP
void SystemDeepSleep()
{
    /// Interrupts
//    INTCONbits.GIEH = 0;     // disable HP interrupt
//    PIR3bits.RTCCIF = 0;     // Clear RTCC interrupt flag
//    PIE3bits.RTCCIE = 1;     // Enable RTCC interrupt

    /// Watchdog
    WDTCONbits.SWDTEN = 0;   // turn off the watch dog timer
    WDTCONbits.REGSLP = 1;   // on-chip regulator enter sleep mode on sleep instruction
    WDTCONbits.DS = 0;       // clear deep sleep wake-up bit (must be manually cleared)

    /// Wake up configuration 
    DSCONHbits.DSULPEN = 0;  // disable ULP pull up
    DSCONHbits.RTCWDIS = 0;  // enable RTCC as source of wake up 
    DSCONLbits.ULPWDIS = 1;  // disable ULP wake up
    DSCONLbits.DSBOR = 0;    // disable Brownout wake up
    DSCONLbits.RELEASE = 0;  // 

    /// Enter Deep Sleep mode
    OSCCONbits.IDLEN = 0;    // enter sleep or deep sleep mode (not idle) 

    while (RTCCFGbits.RTCSYNC); // Wait for Alarm interrupt (PIC18F46J50 Errata)
/*
    DSCONHbits.DSEN = 1;     // set the deep sleep enable bit 
    sleep();
*/
    // Time-critical section must be in ASM  

    __asm
    enterdeepsleep:
        movlb   0x0F                ; banked
        bsf     _DSCONH, 7, 1       ; deep sleep mode
        nop
        sleep
        goto    enterdeepsleep      ; should never be reached unless deep sleep fails
    __endasm;

    /// Device is now in deep sleep mode.           ///
    /// It will Wake-up when RTCC alarm occurs.     ///
    /// Wake-up trigger a Power-on Reset,           ///
    /// which means program will re-start from the  ///
    /// beginning NOT at the instruction after      ///
    /// the deep sleep instruction.                 ///
}
#endif

#endif /* defined(__18f26j50) || defined(__18f46j50) */

/*  ----------------------------------------------------------------------------
    Handle stuff specific to wake up from deep sleep by a power on reset
    TODO : return Wake-up source
    --------------------------------------------------------------------------*/

#if defined(__18f26j50) || defined(__18f46j50)

#if defined(SYSTEMDEEPSLEEPWAKEUP)
u8 SystemDeepSleepWakeUp()
{
    u8 b;
    
    DSCONLbits.RELEASE = 0;     // Release the Deep sleep IO config. lock out
    
    if (WDTCONbits.DS)          // Reset was caused by wake-up from deep sleep
    {
        WDTCONbits.DS = 0;      // Allow Deep Sleep Wake-up bit again (must be manually cleared)

        for (b=0; b<=7; b++)    // Detect source of reset
        {
            if ( (DSWAKEL >> b) & 1 )
            {
                DSWAKEL = 0x00;
                return 1<<b;    // Return code of source
            }
        }

        if (DSWAKEH == 1)
        {
            DSWAKEH = 0x00;
            return PINWAKEUP;   // Return code of source
        }
    }
    else
    {
        return 0;
    }
}

#endif

#endif /* defined(__18f26j50) || defined(__18f46j50) */

#endif /* __LOWPOWER_C */
