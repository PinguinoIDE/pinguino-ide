/*  --------------------------------------------------------------------
    FILE:           audio.c
    PROJECT:        pinguino
    PURPOSE:        playing sound
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:   5 Jan. 2010
    LAST RELEASE:   15 Feb. 2015
    --------------------------------------------------------------------
    CHANGELOG:
    * 15 Feb. 2015  Régis Blanchot - added Sine Wave Generator
    * 16 Feb. 2015  Régis Blanchot - added Audio.init(SAMPLERATE)
    * 17 Feb. 2015  Régis Blanchot - added Direct Digital Synthesis
    * 17 Feb. 2015  Régis Blanchot - renamed library to audio.c
    * 17 Feb. 2015  Régis Blanchot - added Audio.staccato() and Audio.legato()
    TODO:
    * DTMF (dual-tone multi-frequency)
    * Wav decoder
    READINGS :
    * http://www.romanblack.com/one_sec.htm#BDA 
    * http://www.electricdruid.net/index.php?page=info.dds
    --------------------------------------------------------------------
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    ------------------------------------------------------------------*/

#ifndef __AUDIO_C
    #define __AUDIO_C
    #define __AUDIO__

    #include <typedef.h>
    #include <pin.h>            // CCPx pin definitions
    #include <audio.h>
    #include <delayms.c>
    #include <digitalp.c>
    #include <oscillator.c>     // System_getPeripheralFrequency
    
    // PWM mode
    #define PWMMODE         0b00001100

    // The hearing range of human ears is roughly 20 Hz to 20000 Hz.
    // The Nyquist–Shannon sampling theorem says the sampling frequency
    // must be greater than twice the maximum frequency we wish to reproduce.
    
    //Common sampling rates :
    #define CDQUALITY       44100
    #define TAPEQUALITY     22050
    #define RADIOQUALITY    11025
    #define TELQUALITY      8820

    u8 *pCCPxCON;               // CCPxCON
    u8 *pCCPRxL;                // CCPRxL

    // global variables
    volatile u16 gPeriodPlus1;
    volatile u16 gSampleRate;
    volatile u16 gPhase;
    volatile u16 gFreq1Inc, gFreq2Inc;
             u8  gStaccato = true;

    // Waveform table
    const u8 sine64[64] = {
        50,54,59,64,68,73,77,81,85,88,91,93,95,97,98,99,99,99,98,97,95,
        93,91,88,85,81,77,73,68,64,59,54,50,45,40,35,31,26,22,18,14,11,
        8,6,4,2,1,0,0,0,1,2,4,6,8,11,14,18,22,26,31,35,40,45 };

    // Prototypes
    void Audio_init(u16 samplerate);
    void Audio_tone(u8 pin, u16 freq, u16 duration);
    void Audio_noTone(u8 pin);

/*  --------------------------------------------------------------------
    init
    --------------------------------------------------------------------
    @descr :            Generate a sample PWM gPeriodPlus1 signal and enable
                        Timer2 interrupt
    @param :            none
    @note  :            PWM_setFrequency() computes the best value for
                        T2CON prescaler and PR2 depending on actual
                        Peripheral Frequency.
                        It returns the value of PR2.
    ------------------------------------------------------------------*/

    void Audio_init(u16 samplerate)
    {
        u8 prescaler;
        
        // PR2+1 calculation
        // Timer2 clock input is the peripheral clock (FOSC/4). 

        gPeriodPlus1 = System_getPeripheralFrequency() / samplerate;

        // stops interrupt
        INTCONbits.GIEH = 0;         // disable global HP interrupts
        INTCONbits.GIEL = 0;         // disable global LP interrupts

        // configures Timer2 interrut
        IPR1bits.TMR2IP = 1;         // interrupt has high priority
        PIR1bits.TMR2IF = 0;         // reset interrupt flag

        // Timer2 prescaler calculation
        // PR2 max value is 255, so PR2+1 max value is 256
        // only 3 possible prescaler value : 1, 4 or 16
        // so gPeriodPlus1 can not be > to 16 * 256 = 4096
        // and frequency can not be < 2929Hz (12MHZ/4096)
        
        if (gPeriodPlus1 <= 4096)          // check if it's not too high
        {
            if (gPeriodPlus1 <= 256)       // no needs of any prescaler
            {
                prescaler = 0;             // prescaler is 1, Timer2 On
            }
            else if (gPeriodPlus1 <= 1024) // needs prescaler 1:4
            {
                gPeriodPlus1 = gPeriodPlus1 >> 2;// divided by 4
                prescaler = 1;            // prescaler is 4, Timer2 On
            }
            else                          // needs prescaler 1:6
            {
                gPeriodPlus1 = gPeriodPlus1 >> 4;// divided by 16
                prescaler = 2;           // prescaler is 16, Timer2 On
            }
        }

        gSampleRate = samplerate;

        TMR2  = 0;
        PR2   = gPeriodPlus1 - 1;
        T2CON = prescaler | _TMR2ON;

        // (re-)starts interrupt
        INTCONbits.GIEL = 1;         // enable global LP interrupts
        INTCONbits.GIEH = 1;         // enable global HP interrupts
    }

/*  --------------------------------------------------------------------
    staccato
    --------------------------------------------------------------------
    Separates note from the note that may follow by silence
    @param:             none
    ------------------------------------------------------------------*/

    #define Audio_staccato() { gStaccato = true; }
    
/*  --------------------------------------------------------------------
    legato
    --------------------------------------------------------------------
    Plays note with the shortest silence between notes
    @param:             none
    ------------------------------------------------------------------*/

    #define Audio_legato()  { gStaccato = false; }

/*  --------------------------------------------------------------------
    tone
    --------------------------------------------------------------------
    Play sound with a certain frequency for a certain duration
    @param pin:         pin number where buzzer or loudspeaker is connected
    @param note:        note frequency
    @param duration:    Duration in ms
    ------------------------------------------------------------------*/

    void Audio_tone(u8 pin, u16 freq, u16 duration)
    {
        switch (pin)
        {
            #if defined(__18f26j53) || defined(__18f46j53) || \
                defined(__18f27j53) || defined(__18f47j53)

            case CCP4 : pCCPxCON = &CCP4CON;  pCCPRxL = &CCPR4L;  break;
            case CCP5 : pCCPxCON = &CCP5CON;  pCCPRxL = &CCPR5L;  break;
            case CCP6 : pCCPxCON = &CCP6CON;  pCCPRxL = &CCPR6L;  break;
            case CCP7 : pCCPxCON = &CCP7CON;  pCCPRxL = &CCPR7L;  break;
            case CCP8 : pCCPxCON = &CCP8CON;  pCCPRxL = &CCPR8L;  break;
            case CCP9 : pCCPxCON = &CCP9CON;  pCCPRxL = &CCPR9L;  break;
            case CCP10: pCCPxCON = &CCP10CON; pCCPRxL = &CCPR10L; break;

            #else

            case CCP1 : pCCPxCON = &CCP1CON;  pCCPRxL = &CCPR1L;  break;
            case CCP2 : pCCPxCON = &CCP2CON;  pCCPRxL = &CCPR2L;  break;

            #endif
        }

        // 16-bit accumulator's increment value.
        // the accumulator will go back to zero after (gSampleRate/freq) ticks
        gPhase = 0;
        gFreq1Inc = 65536 * freq / gSampleRate;

        *pCCPxCON = PWMMODE;
        
        pinmode(pin, OUTPUT);   // PWM pin as OUTPUT
        PIE1bits.TMR2IE = 1;    // enable interrupt
        
        Delayms(duration);
        
        if (gStaccato)
            *pCCPxCON = 0;      // staccato
    }

/*  --------------------------------------------------------------------
    DTMF (dual-tone multi-frequency)
    --------------------------------------------------------------------
    Play sound with a certain frequency for a certain duration
    @param pin:         pin number where buzzer or loudspeaker is connected
    @param note:        note frequency
    @param duration:    Duration in ms
    ------------------------------------------------------------------*/

    void Audio_DTMF(u8 pin, u16 freq1, u16 freq2, u16 duration)
    {
        switch (pin)
        {
            #if defined(__18f26j53) || defined(__18f46j53) || \
                defined(__18f27j53) || defined(__18f47j53)

            case CCP4 : pCCPxCON = &CCP4CON;  pCCPRxL = &CCPR4L;  break;
            case CCP5 : pCCPxCON = &CCP5CON;  pCCPRxL = &CCPR5L;  break;
            case CCP6 : pCCPxCON = &CCP6CON;  pCCPRxL = &CCPR6L;  break;
            case CCP7 : pCCPxCON = &CCP7CON;  pCCPRxL = &CCPR7L;  break;
            case CCP8 : pCCPxCON = &CCP8CON;  pCCPRxL = &CCPR8L;  break;
            case CCP9 : pCCPxCON = &CCP9CON;  pCCPRxL = &CCPR9L;  break;
            case CCP10: pCCPxCON = &CCP10CON; pCCPRxL = &CCPR10L; break;

            #else

            case CCP1 : pCCPxCON = &CCP1CON;  pCCPRxL = &CCPR1L;  break;
            case CCP2 : pCCPxCON = &CCP2CON;  pCCPRxL = &CCPR2L;  break;

            #endif
        }

        // 16-bit accumulator's increment value.
        // the accumulator will go back to zero after (gSampleRate/freq) ticks
        gPhase = 0;
        gFreq1Inc = 65536 * freq1 / gSampleRate;
        gFreq2Inc = 65536 * freq2 / gSampleRate;

        *pCCPxCON = PWMMODE;
        
        pinmode(pin, OUTPUT);   // PWM pin as OUTPUT
        PIE1bits.TMR2IE = 1;    // enable interrupt
        
        Delayms(duration);
        
        if (gStaccato)
            *pCCPxCON = 0;      // staccato
    }

/*  --------------------------------------------------------------------
    noTone
    --------------------------------------------------------------------
    @param pin:         pin number where loudspeaker is connected
    ------------------------------------------------------------------*/

    void Audio_noTone(u8 pin)
    {
        pinmode(pin, INPUT);    // PWM pin as INPUT
        PIE1bits.TMR2IE = 0;    // disable interrupt 

        switch (pin)            // PWM mode disable
        {
            #if defined(__18f26j53) || defined(__18f46j53) || \
                defined(__18f27j53) || defined(__18f47j53)

            case CCP4 : CCP4CON  = 0; break;
            case CCP5 : CCP5CON  = 0; break;
            case CCP6 : CCP6CON  = 0; break;
            case CCP7 : CCP7CON  = 0; break;
            case CCP8 : CCP8CON  = 0; break;
            case CCP9 : CCP9CON  = 0; break;
            case CCP10: CCP10CON = 0; break;

            #else

            case CCP1 : CCP1CON  = 0; break;
            case CCP2 : CCP2CON  = 0; break;

            #endif
        }
    }

/*  --------------------------------------------------------------------
    Interrupt Service Routine
    --------------------------------------------------------------------
    The new PWM value must be computed in less than the PWM cycle gPeriodPlus1.
    If sample rate = CDQUALITY    -> Tpwm =  22 us
    If sample rate = TAPEQUALITY  -> Tpwm =  45 us
    If sample rate = RADIOQUALITY -> Tpwm =  90 us
    If sample rate = TELQUALITY   -> Tpwm = 113 us
    To generate a single clean tone we load the duty cycle register
    according to a pure sinusoid
    ------------------------------------------------------------------*/

void pwm_interrupt()
{
    u16 duty;
    
    if (PIR1bits.TMR2IF)
    {
        // Clear interrupt flag
        PIR1bits.TMR2IF = 0;

        // Increment the 16-bit phase accumulator
        gPhase += gFreq1Inc;
        
        // The signal level must be offset so that the zero level
        // generates a PWM output with a 50% duty cycle
        //*pCCPRxL = sine64[ gPhase & 0x3F ];// * gPeriodPlus1 / 100;
        //duty = sine64[ gPhase & 0x3F ] * gPeriodPlus1 / 100;
        duty = sine64[ gPhase & 0x3F ] * 100;
        //duty = duty * 4;

        // Load the duty cycle register according to the sine table
        //PWM_setDutyCycle(gPin, duty);
        *pCCPRxL   = (duty >> 2) & 0xFF;          // 8 MSB
        *pCCPxCON |= ((u8)duty & 0x03) << 4;      // 2 LSB in <5:4>
    }
}

#endif // __AUDIO_C
