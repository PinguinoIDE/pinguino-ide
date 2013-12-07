// analogic input library for pinguino
// Jean-Pierre MANDON 2008
// added 18F4550 support 2009/08/10
// 2012-07-10 regis blanchot added 18F26J50 support
// 2012-11-19 regis blanchot added 18F1220 and 18F1230 support
// 2013-06-26 regis blanchot fixed analogWrite()
// 2013-09-09 regis blanchot added 18F47J53 support

#ifndef __ANALOG_C
#define __ANALOG_C

#define DEFAULT		0
#define	EXTERNAL	1

#include <pic18fregs.h>
#include <typedef.h>
#include <pin.h>
//#include <macro.h>
//#include <digitalw.c>

/*  --------------------------------------------------------------------
    Init
    ------------------------------------------------------------------*/

void analog_init(void)
{

    #if defined(PINGUINO1220) || defined(PINGUINO1320)

	TRISA=TRISA | 0x1F; // 0b00011111 = RA0,1,2,3,4 = AN0 to AN4 are INPUT
	ADCON1=0x1F;        // 0b00001000 = 0, 0, VRef-=VSS, VRef+=VDD, AN0 to AN4 enabled 
	ADCON2=0xBD;        // 0b10111101 = Right justified, 0, 20 TAD, FOSC/16

    #elif defined(PINGUINO4550)

	TRISA=TRISA | 0x2F;
	TRISE=TRISE | 0x07;	
	ADCON1=0x07;
	ADCON2=0xBD;

    #elif defined(PICUNO_EQUO)

	TRISA=TRISA | 0x2F;	//RA0..2, RA5
	TRISE=TRISE | 0x03;	//RE0..1
	ADCON1=0x08;		//AN0-AN6, Vref+ = VDD, RA4 as Digital o/p
	ADCON2=0xBD;		//Right justified, 20TAD, FOSC/16

    #elif defined(PINGUINO26J50)

	TRISA=TRISA | 0x2F;	// 0b00101111 = RA0,1,2,3 and RA5 = AN0 to AN4 are INPUT
    //1 = Pin configured as a digital port
    //0 = Pin configured as an analog channel – digital input is disabled and reads ‘0’
	ANCON0=0xE0;//0x1F; // 0b11100000 = AN0 to AN4 enabled, AN5 to AN7 disabled
	ANCON1|=0x1F;//0x0A;// 0b00111111 = AN8 to AN12 disabled (1=digital/0=analog)
	ADCON0=0x00;        // 0b00000000 = VRef-=VSS, VRef+=VDD, No channel selected yet 
	ADCON1=0xBD;		// 0b10111101 = Right justified, Calibration Normal, 20TAD, FOSC/16

    #elif defined(PINGUINO47J53A)
    // RB 09/09/2013: Analog Conversion Mode is set to 12-bit in Bootloader Config file
    // #pragma config ADCSEL = BIT12 // 12-bit conversion mode is enabled

	TRISA=TRISA|0b00011111;	// RA0 (AN0) to RA4 (AN4) are INPUT
	TRISE=TRISE|0b00000111;	// RE0 (AN5) to RE2 (AN7) are INPUT

    //1 = Pin configured as a digital port
    //0 = Pin configured as an analog channel – digital input is disabled and reads ‘0’
	ANCON0=0;           // AN0 to AN7 enabled
	ANCON1=0b01111111;  // VBGEN=0, AN8 to AN12 disabled (1=digital/0=analog)

	ADCON0=0x00;        // 0b00000000 = VRef-=VSS, VRef+=VDD, No channel selected yet 
	ADCON1=0b10111110;	// Right justified, Calibration Normal, 20TAD, FOSC/64

    #else // PINGUINO2550

	TRISA=TRISA | 0x2F; // 0b00101111 = RA0,1,2,3 and RA5 = AN0 to AN4 are INPUT
	ADCON1=0x0A;        // 0b00001000 = 0, 0, VRef-=VSS, VRef+=VDD, AN0 to AN4 enabled 
	ADCON2=0xBD;        // 0b10111101 = Right justified, 0, 20 TAD, FOSC/16

    #endif
}

/*  --------------------------------------------------------------------
    analogReference
    ------------------------------------------------------------------*/

#ifdef ANALOGREFERENCE

void analogreference(u8 Type)
{
    #if !defined(PINGUINO26J50) && !defined(PINGUINO46J50) && \
        !defined(PINGUINO27J53) && !defined(PINGUINO47J53A)   

    if(Type == DEFAULT)			//the default analog reference of 5 volts (on 5V Arduino boards) or 3.3 volts (on 3.3V Arduino boards)
        ADCON1|=0x00;			//Vref+ = VDD
    else if(Type == EXTERNAL)	//the voltage applied to the AREF pin (0 to 5V only) is used as the reference.
        ADCON1|=0x10;			//Vref+ = External source

    #else

    if(Type == DEFAULT)			//the default analog reference of 5 volts (on 5V Arduino boards) or 3.3 volts (on 3.3V Arduino boards)
        ADCON0|=0x00;			//Vref+ = VDD
    else if(Type == EXTERNAL)	//the voltage applied to the AREF pin (0 to 5V only) is used as the reference.
        ADCON0|=0x40;			//Vref+ = External source

    #endif
}

#endif /* ANALOGREFERENCE */

/*  --------------------------------------------------------------------
    analogRead
    ------------------------------------------------------------------*/

#ifdef ANALOGREAD

// The A/D conversion requires 11 TAD per 10-bit conversion
// and 13 TAD per 12-bit conversion.

u16 analogread(u8 channel)
{
    u16 result=0;
    // #if defined(PINGUINO4550) || defined(PICUNO_EQUO)
    // ADCON1=0x07;
    // #else
    // ADCON1=0x0A;
    // #endif

    #ifdef PICUNO_EQUO
        if(channel>=14 && channel<=16)
            ADCON0=(channel-14)*4;
        else if(channel>=17 && channel<=19)
            ADCON0=(channel-13)*4;

    #elif defined(PINGUINO47J53A)
        if(channel>=8 && channel<=14)
            ADCON0=(channel-8) << 2;             // A0 = 8, A1 = 9, ...
        else if(channel<=7)
            ADCON0 = channel << 2;              // A0 = 0, A1 = 1, ...

    #else
        if(channel>=13 && channel<=17)
            ADCON0=(channel-13)*4;             // A0 = 13, A1 = 14, ...
        else if(channel<=5)
            ADCON0 = channel * 4;              // A0 = 0, A1 = 1, ...
    #endif

    ADCON0bits.ADON=1;                          // A/D Converter module is enabled

    for (result=1;result<10;result++)           // Acquisition time
        __asm NOP __endasm;

    ADCON0bits.GO=1;                            // Start A/D Conversion

    while (ADCON0bits.GO);                      // Wait for conversion stop

    result = ADRESH << 8;
    result += ADRESL;

    ADCON0bits.ADON = 0;                        // A/D Converter module is disabled

    return(result);
}

#endif /* ANALOGREAD */

/*  --------------------------------------------------------------------
    analogWrite

    analogWrite() uses PWM module.
    The maximum PWM resolution (bits) for a given PWM frequency is given by the equation :

    PWM Resolution (max) = Log ( Fosc / Fpwm ) / Log ( 2 )

    If 10-bit (max) resolution is needed, then Fpwm must be <=  Fosc / 1024.
    For ex. if Fosc = 48 MHz then Fpwm must be <= 46875 Hz

    If Fosc = 48 MHz and Fpwm = 46875 Hz and Prescaler = 1, then : 
    PWM Period = 1 / Fpwm = [(PR2) + 1] • 4 • TOSC •(TMR2 Prescale Value)
    PR2 = ( Fosc / Fpwm / 4 / TMR2 Prescale Value ) - 1
    PR2 = 255
    ------------------------------------------------------------------*/

#ifdef ANALOGWRITE

// Set the PWM period by writing to the PR2 register.
// Set the TMR2 prescale value, then enable Timer2 by writing to T2CON.
// PIC18F47J53 Family :
// The assignment of a particular timer to a module is determined by the
// Timer to CCP enable bits in the CCPTMRSx registers.
// The CCPTMRS1 register selects the timers for CCP modules, 7, 6, 5 and 4,
// and the CCPTMRS2 register selects the timers for CCP modules, 10, 9 and 8

void analogwrite_init()
{
    #if defined(PINGUINO47J53A)

    CCPTMRS1 = 0;                           // assign Timer2 to all CCP pins
    CCPTMRS2 = 0;
  
    #endif

    PR2 = 0xFF;                             // set PWM period to the max. to get 10-bit res.
    T2CON = 0b00000100;                     // Timer2 on, prescaler is 1
}

void analogwrite(u8 pin, u16 duty)
{
    #if !defined(PINGUINO47J53A)

    switch (pin)
    {
        case CCP1:
            CCP1CON  = 0b00001100;
            CCPR1L   = ( duty >> 2 ) & 0xFF;         // 8 LSB
            //CCP1CON |= ( (duty & 0x300) >> 8) << 4;  // 2 MSB in <5:4>
            CCP1CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;

        case CCP2:
            CCP2CON  = 0b00001100;
            CCPR2L   = ( duty >> 2 ) & 0xFF;                  // 8 LSB
            //CCP2CON |= ( (duty & 0x3FF) >> 8) << 4;  // 2 MSB in <5:4>
            CCP1CON |= (duty & 0x300) >> 4;  // 2 MSB in <5:4>
            break;
    }

    #else // PINGUINO47J53A

    /*
    On PIC18F47J53 CCPx pin are multiplexed with a PORTB data latch,
    the appropriate TRIS bit must be cleared by user to make the CCPx pin an output.
    */
    
    switch (pin)
    {

        case CCP4:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP4CON  = 0b00001100;           // Configure the CCPx module for PWM operation
            CCPR4L   = ( duty >> 2 ) & 0xFF; // Set the PWM duty cycle by writing to the CCPRxL register
            CCP4CON |= (duty & 0x300) >> 4;  // and CCPxCON<5:4> bits
            break;

        case CCP5:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP5CON  = 0b00001100;           // Configure the CCPx module for PWM operation
            CCPR5L   = ( duty >> 2 ) & 0xFF; // Set the PWM duty cycle by writing to the CCPRxL register
            CCP5CON |= (duty & 0x300) >> 4;  // and CCPxCON<5:4> bits
            break;

        case CCP6:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP6CON  = 0b00001100;           // Configure the CCPx module for PWM operation
            CCPR6L   = ( duty >> 2 ) & 0xFF; // Set the PWM duty cycle by writing to the CCPRxL register
            CCP6CON |= (duty & 0x300) >> 4;  // and CCPxCON<5:4> bits
            break;

        case CCP7:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP7CON  = 0b00001100;           // Configure the CCPx module for PWM operation
            CCPR7L   = ( duty >> 2 ) & 0xFF; // Set the PWM duty cycle by writing to the CCPRxL register
            CCP7CON |= (duty & 0x300) >> 4;  // and CCPxCON<5:4> bits
            break;

        case CCP8:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP8CON  = 0b00001100;           // Configure the CCPx module for PWM operation
            CCPR8L   = ( duty >> 2 ) & 0xFF; // Set the PWM duty cycle by writing to the CCPRxL register
            CCP8CON |= (duty & 0x300) >> 4;  // and CCPxCON<5:4> bits
            break;

        case CCP9:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP9CON  = 0b00001100;           // Configure the CCPx module for PWM operation
            CCPR9L   = ( duty >> 2 ) & 0xFF; // Set the PWM duty cycle by writing to the CCPRxL register
            CCP9CON |= (duty & 0x300) >> 4;  // and CCPxCON<5:4> bits
            break;

        case CCP10:
            //BitClear(TRISB, pin);            // Make the CCPx pin an output by clearing the appropriate TRIS bit.
            CCP10CON  = 0b00001100;          // Configure the CCPx module for PWM operation
            CCPR10L   = ( duty >> 2 ) & 0xFF;// Set the PWM duty cycle by writing to the CCPRxL register
            CCP10CON |= (duty & 0x300) >> 4; // and CCPxCON<5:4> bits
            break;
    }

    #endif

    PIR1bits.TMR2IF = 0;
}

#endif /* ANALOGWRITE */

#endif /* __ANALOG_C */
