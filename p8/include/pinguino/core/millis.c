// millis library for pinguino
// Jean-Pierre MANDON 2009
// added interrupt.c functions (regis blanchot 2011)
// [14-05-12][jp.mandon changed long to u32 and Millis to millis / thanks mark harper]
// [31-01-13][r.blanchot use of System_getPeripheralFrequency()]

#ifndef _MILLIS_C_
#define _MILLIS_C_

#include <pic18fregs.h>
#include <typedef.h>
//#include <interrupt.h>
//#include <interrupt.c>
#include <oscillator.c>         // System_getPeripheralFrequency()

volatile u32 _millis;
volatile u32 _PR0_;

void updateMillisReloadValue(void )   /* Call from System_setIntOsc() */
{
    /* Atomic operation */
    INTCONbits.TMR0IE = 0;//INT_DISABLE;
    _PR0_ = 0xFFFF - System_getPeripheralFrequency() / 1000 ;
    INTCONbits.TMR0IE = 1;//INT_ENABLE;
}

void millis_init(void)
{
//    intUsed[INT_TMR0] = INT_USED;

    /*
    T0CON = 0x80;		// TMR0 on, 16 bits counter, prescaler=2
    INTCON |= 0xA0;		// set GIE and TMR0IE
    */
    
    // if Fosc = 48 MHz then Fosc/4 = 12MHz
    // which means 12.E-06 cycles/sec = 12.000 cycles/ms
    // if TMR0 is loaded with 65536 - 12000
    // overload will occur after 12.000 cycles = 1ms
    
    INTCONbits.GIEH    = 0;   // Disable global HP interrupts
    INTCONbits.GIEL    = 0;   // Disable global LP interrupts
    T0CON = 0b00001000;//T0_OFF | T0_16BIT | T0_SOURCE_INT | T0_PS_OFF;
    _PR0_ = 0xFFFF - System_getPeripheralFrequency() / 1000 ;
    TMR0H = high8(_PR0_);
    TMR0L =  low8(_PR0_);
    INTCON2bits.TMR0IP = 1;//INT_HIGH_PRIORITY;
    INTCONbits.TMR0IF  = 0;
    INTCONbits.TMR0IE  = 1;//INT_ENABLE;
    T0CONbits.TMR0ON   = 1;
    INTCONbits.GIEH    = 1;   // Enable global HP interrupts
    INTCONbits.GIEL    = 1;   // Enable global LP interrupts

    _millis = 0;
}

u32 millis()
{
    u32 temp;
    /* Atomic operation for multibyte value */
    INTCONbits.TMR0IE = 0;//INT_DISABLE;
    temp = _millis;
    INTCONbits.TMR0IE = 1;//INT_ENABLE;
    return(temp);
}

// called by interruption service routine in main.c    if (INTCONbits.TMR0IF)
void millis_interrupt(void)
{
    if (INTCONbits.TMR0IF)
    {
        INTCONbits.TMR0IF = 0;
        TMR0H = high8(_PR0_);
        TMR0L =  low8(_PR0_);
        _millis++;
    }
}

#endif /* _MILLIS_C_ */
