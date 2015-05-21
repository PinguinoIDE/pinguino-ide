/*	----------------------------------------------------------------------------
    FILE:				onevent.c
    PROJECT:			pinguino 32
    PURPOSE:			event management
    PROGRAMER:			regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:		14 Jan. 2015
    LAST RELEASE:		21 Jan. 2015
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

#ifndef __ONEVENT__
#define __ONEVENT__

#include <p32xxxx.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>
#include <interrupt.h>
#include <interrupt.c>
#include <system.c>

#define TXCKPS256 0b111 // 1:256 prescale value
#define TXCKPS64  0b110 // 1:64  prescale value
#define TXCKPS32  0b101 // 1:32  prescale value
#define TXCKPS16  0b100 // 1:16  prescale value
#define TXCKPS8   0b011 // 1:8   prescale value
#define TXCKPS4   0b010 // 1:4   prescale value
#define TXCKPS2   0b001 // 1:2   prescale value
#define TXCKPS1   0b000 // 1:1   prescale value

#define T1CKPS256 0b11  // 1:256 prescale value
#define T1CKPS64  0b10  // 1:64  prescale value
#define T1CKPS8   0b01  // 1:8   prescale value
#define T1CKPS1   0b00  // 1:1   prescale value

typedef void (*callback) (void); // type of: void callback()

callback intFunction[INT_NUM];
u32 intUsed[INT_NUM];

// OnTimerX only
#if defined(TMR1INT) || defined(TMR2INT) || \
    defined(TMR3INT) || defined(TMR4INT) || \
    defined(TMR5INT)

    volatile u32 intCount[6];
    volatile u32 intCountLimit[6];

    u32 prescalerx[] = {1, 2, 4, 8, 16, 32, 64, 256};
    u32 prescaler1[] = {1, 8, 64, 256};
    
#endif

/*  --------------------------------------------------------------------
    OnTimer1
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure timer X to execute function func every delay ms, us or sec
    @param		timediv:	INT_MICROSEC, INT_MILLISEC, INT_SEC
                func:		function called when interrupt occures
                delay:		delay before overload
    @return     INT_USED or INT_NOT_USED
    ------------------------------------------------------------------*/

#ifdef TMR1INT
u32 OnTimer1(callback func, u32 timediv, u32 delay)
{
    u32 tckps=0, freqhz, period;

    if (intUsed[INT_TIMER1] == INT_NOT_USED)
    {
        intUsed[INT_TIMER1] = INT_USED;
        intFunction[INT_TIMER1] = func;
        intCount[1] = 0;
        intCountLimit[1] = delay;

        // TMR1 Count register increments on every PBCLK clock cycle
        freqhz = GetPeripheralClock();
    
        // Freq (Hz) = Nb ticks/sec.
        switch(timediv)
        {
            case INT_SEC:      period = freqhz;           break;
            case INT_MILLISEC: period = freqhz / 1000;    break;
            case INT_MICROSEC: period = freqhz / 1000000; break;
        }

        // Timer1 period is 16-bit, only 4 prescaler values
        while ((period > 0xFFFF) & (tckps < 5))
        {
            tckps += 1;
            period /= prescaler1[tckps];
        }

        if (tckps == 4)
        {
            tckps = 3; // divided per 256
            intCountLimit[1] = delay * 8;
        }
        
        // Configure interrupt
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_TIMER1_VECTOR, 7, 3);
        IntClearFlag(INT_TIMER1);
        IntEnable(INT_TIMER1);

        // Configure Timer1
        T1CON    = tckps << 4;              // set prescaler (bit 5-4)
        TMR1     = 0;                       // clear timer register
        PR1      = period;                  // load period register
        T1CONSET = 0x8000;                  // start timer 1

        return INT_TIMER1;
    }
    
    else
    {
        #ifdef DEBUG
        debug("Error : TIMER1 interrupt is already used !");
        #endif
        return false;
    }
}

/*  --------------------------------------------------------------------
    Timer 1 interrupt (Vector 4)
    see also ISRwrapper.S
    ------------------------------------------------------------------*/

void Timer1Interrupt()
{
    if (IntGetFlag(INT_TIMER1))
    {
        if (intCount[1]++ >= intCountLimit[1])
        {
            // reset the counter
            intCount[1] = 0;
            // call user's routine
            intFunction[INT_TIMER1]();
        }
        IntClearFlag(INT_TIMER1);
    }
}
#endif /* TMR1INT */

/*  --------------------------------------------------------------------
    OnTimer2
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure timer X to execute function func every delay ms, us or sec
    @param		timediv:	INT_MICROSEC, INT_MILLISEC, INT_SEC
                func:		function called when interrupt occures
                delay:		delay before overload
    @return     INT_USED or INT_NOT_USED
    ------------------------------------------------------------------*/

#ifdef TMR2INT
u32 OnTimer2(callback func, u32 timediv, u32 delay)
{
    u32 tckps=0, freqhz, period;

    if (intUsed[INT_TIMER2] == INT_NOT_USED)
    {
        intUsed[INT_TIMER2] = INT_USED;
        intFunction[INT_TIMER2] = func;
        intCount[2] = 0;
        intCountLimit[2] = delay;

        // TMR2 Count register increments on every PBCLK clock cycle
        freqhz = GetPeripheralClock();
    
        // Freq (Hz) = Nb ticks/sec.
        switch(timediv)
        {
            case INT_SEC:      period = freqhz;           break;
            case INT_MILLISEC: period = freqhz / 1000;    break;
            case INT_MICROSEC: period = freqhz / 1000000; break;
        }

        // Timer2 period is 16-bit, 8 prescaler values
        while ((period > 0xFFFF) & (tckps <= 8))
        {
            tckps += 1;
            period /= prescalerx[tckps];
        }

        if (tckps == 8)
        {
            tckps = 7; // divided per 256
            intCountLimit[2] = delay * 8;
        }
        
        // Configure interrupt
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_TIMER2_VECTOR, 7, 3);
        IntClearFlag(INT_TIMER2);
        IntEnable(INT_TIMER2);

        // Configure Timer2
        T2CON    = tckps << 4;              // set prescaler (bit 6-4)
        TMR2     = 0;                       // clear timer register
        PR2      = period;                  // load period register
        T2CONSET = 0x8000;                  // start timer 1

        return INT_TIMER2;
    }
    
    else
    {
        #ifdef DEBUG
        debug("Error : TIMER2 interrupt is already used !");
        #endif
        return false;
    }
}

/*  --------------------------------------------------------------------
    Timer 2 interrupt (Vector 8)
    see also ISRwrapper.S
    ------------------------------------------------------------------*/

void Timer2Interrupt()
{
    if (IntGetFlag(INT_TIMER2))
    {
        if (intCount[2]++ >= intCountLimit[2])
        {
            // reset the counter
            intCount[2] = 0;
            // call user's routine
            intFunction[INT_TIMER2]();
        }
        IntClearFlag(INT_TIMER2);
    }
}
#endif /* TMR2INT */

/*  --------------------------------------------------------------------
    OnTimer3
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure timer X to execute function func every delay ms, us or sec
    @param		timediv:	INT_MICROSEC, INT_MILLISEC, INT_SEC
                func:		function called when interrupt occures
                delay:		delay before overload
    @return     INT_USED or INT_NOT_USED
    ------------------------------------------------------------------*/

#ifdef TMR3INT
u32 OnTimer3(callback func, u32 timediv, u32 delay)
{
    u32 tckps=0, freqhz, period;

    if (intUsed[INT_TIMER3] == INT_NOT_USED)
    {
        intUsed[INT_TIMER3] = INT_USED;
        intFunction[INT_TIMER3] = func;
        intCount[3] = 0;
        intCountLimit[3] = delay;

        // TMR3 Count register increments on every PBCLK clock cycle
        freqhz = GetPeripheralClock();
    
        // Freq (Hz) = Nb ticks/sec.
        switch(timediv)
        {
            case INT_SEC:      period = freqhz;           break;
            case INT_MILLISEC: period = freqhz / 1000;    break;
            case INT_MICROSEC: period = freqhz / 1000000; break;
        }

        // Timer3 period is 16-bit, 8 prescaler values
        while ((period > 0xFFFF) & (tckps <= 8))
        {
            tckps += 1;
            period /= prescalerx[tckps];
        }

        if (tckps == 8)
        {
            tckps = 7; // divided per 256
            intCountLimit[3] = delay * 8;
        }
        
        // Configure interrupt
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_TIMER3_VECTOR, 7, 3);
        IntClearFlag(INT_TIMER3);
        IntEnable(INT_TIMER3);

        // Configure Timer3
        T3CON    = tckps << 4;              // set prescaler (bit 6-4)
        TMR3     = 0;                       // clear timer register
        PR3      = period;                  // load period register
        T3CONSET = 0x8000;                  // start timer 1

        return INT_TIMER3;
    }
    
    else
    {
        #ifdef DEBUG
        debug("Error : TIMER3 interrupt is already used !");
        #endif
        return false;
    }
}

/*  --------------------------------------------------------------------
    Timer 3 interrupt (Vector 12)
    see also ISRwrapper.S
    ------------------------------------------------------------------*/

void Timer3Interrupt()
{
    if (IntGetFlag(INT_TIMER3))
    {
        if (intCount[3]++ >= intCountLimit[3])
        {
            // reset the counter
            intCount[3] = 0;
            // call user's routine
            intFunction[INT_TIMER3]();
        }
        IntClearFlag(INT_TIMER3);
    }
}
#endif /* TMR3INT */

/*  --------------------------------------------------------------------
    OnTimer4
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure timer X to execute function func every delay ms, us or sec
    @param		timediv:	INT_MICROSEC, INT_MILLISEC, INT_SEC
                func:		function called when interrupt occures
                delay:		delay before overload
    @return     INT_USED or INT_NOT_USED
    ------------------------------------------------------------------*/

#ifdef TMR4INT
u32 OnTimer4(callback func, u32 timediv, u32 delay)
{
    u32 tckps=0, freqhz, period;

    if (intUsed[INT_TIMER4] == INT_NOT_USED)
    {
        intUsed[INT_TIMER4] = INT_USED;
        intFunction[INT_TIMER4] = func;
        intCount[4] = 0;
        intCountLimit[4] = delay;

        // TMR4 Count register increments on every PBCLK clock cycle
        freqhz = GetPeripheralClock();
    
        // Freq (Hz) = Nb ticks/sec.
        switch(timediv)
        {
            case INT_SEC:      period = freqhz;           break;
            case INT_MILLISEC: period = freqhz / 1000;    break;
            case INT_MICROSEC: period = freqhz / 1000000; break;
        }

        // Timer2 period is 16-bit, 8 prescaler values
        while ((period > 0xFFFF) & (tckps <= 8))
        {
            tckps += 1;
            period /= prescalerx[tckps];
        }

        if (tckps == 8)
        {
            tckps = 7; // divided per 256
            intCountLimit[4] = delay * 8;
        }
        
        // Configure interrupt
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_TIMER4_VECTOR, 7, 3);
        IntClearFlag(INT_TIMER4);
        IntEnable(INT_TIMER4);

        // Configure Timer4
        T4CON    = tckps << 4;              // set prescaler (bit 6-4)
        TMR4     = 0;                       // clear timer register
        PR4      = period;                  // load period register
        T4CONSET = 0x8000;                  // start timer 1

        return INT_TIMER4;
    }
    
    else
    {
        #ifdef DEBUG
        debug("Error : TIMER4 interrupt is already used !");
        #endif
        return false;
    }
}

/*  --------------------------------------------------------------------
    Timer 4 interrupt (Vector 16)
    see also ISRwrapper.S
    ------------------------------------------------------------------*/

void Timer4Interrupt()
{
    if (IntGetFlag(INT_TIMER4))
    {
        if (intCount[4]++ >= intCountLimit[4])
        {
            // reset the counter
            intCount[4] = 0;
            // call user's routine
            intFunction[INT_TIMER4]();
        }
        IntClearFlag(INT_TIMER4);
    }
}
#endif /* TMR4INT */

/*  --------------------------------------------------------------------
    OnTimer5
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure timer X to execute function func every delay ms, us or sec
    @param		timediv:	INT_MICROSEC, INT_MILLISEC, INT_SEC
                func:		function called when interrupt occures
                delay:		delay before overload
    @return     INT_USED or INT_NOT_USED
    ------------------------------------------------------------------*/

#ifdef TMR5INT
u32 OnTimer5(callback func, u32 timediv, u32 delay)
{
    u32 tckps=0, freqhz, period;

    if (intUsed[INT_TIMER5] == INT_NOT_USED)
    {
        intUsed[INT_TIMER5] = INT_USED;
        intFunction[INT_TIMER5] = func;
        intCount[5] = 0;
        intCountLimit[5] = delay;

        // TMR5 Count register increments on every PBCLK clock cycle
        freqhz = GetPeripheralClock();
    
        // Freq (Hz) = Nb ticks/sec.
        switch(timediv)
        {
            case INT_SEC:      period = freqhz;           break;
            case INT_MILLISEC: period = freqhz / 1000;    break;
            case INT_MICROSEC: period = freqhz / 1000000; break;
        }

        // Timer5 period is 16-bit, 8 prescaler values
        while ((period > 0xFFFF) & (tckps <= 8))
        {
            tckps += 1;
            period /= prescalerx[tckps];
        }

        if (tckps == 8)
        {
            tckps = 7; // divided per 256
            intCountLimit[5] = delay * 8;
        }
        
        // Configure interrupt
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_TIMER5_VECTOR, 7, 3);
        IntClearFlag(INT_TIMER5);
        IntEnable(INT_TIMER5);

        // Configure Timer5
        T5CON    = tckps << 4;              // set prescaler (bit 6-4)
        TMR5     = 0;                       // clear timer register
        PR5      = period;                  // load period register
        T5CONSET = 0x8000;                  // start timer 1

        return INT_TIMER5;
    }
    
    else
    {
        #ifdef DEBUG
        debug("Error : TIMER5 interrupt is already used !");
        #endif
        return false;
    }
}

/*  --------------------------------------------------------------------
    Timer 5 interrupt (Vector 20)
    see also ISRwrapper.S
    ------------------------------------------------------------------*/

void Timer5Interrupt()
{
    if (IntGetFlag(INT_TIMER5))
    {
        if (intCount[5]++ >= intCountLimit[5])
        {
            // reset the counter
            intCount[5] = 0;
            // call user's routine
            intFunction[INT_TIMER5]();
        }
        IntClearFlag(INT_TIMER5);
    }
}
#endif /* TMR5INT */

#endif /* __ONEVENT_C */
