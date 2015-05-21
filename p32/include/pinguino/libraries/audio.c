/*  --------------------------------------------------------------------
    FILE:           audio.c
    PROJECT:        pinguino
    PURPOSE:        playing sound
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:   5 Jan. 2010
    LAST RELEASE:   24 Feb. 2015
    --------------------------------------------------------------------
    CHANGELOG:
    * 15 Feb. 2015  Régis Blanchot - added Sine Wave Generator
    * 16 Feb. 2015  Régis Blanchot - added Audio.init(SAMPLERATE)
    * 17 Feb. 2015  Régis Blanchot - added Direct Digital Synthesis
    * 17 Feb. 2015  Régis Blanchot - renamed library to audio.c
    * 17 Feb. 2015  Régis Blanchot - added Audio.staccato() and Audio.legato()
    * 24 Feb. 2015  Régis Blanchot - Pinguino 32 version
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

    #include <p32xxxx.h>        // PIC32 registers
    #include <typedef.h>        // Pinguino's type (u8, u16, u32, ...)
    #include <pin.h>            // Pinguino's pin definitions
    #include <audio.h>          // Notes frequencies
    #include <delay.c>
    #include <digitalw.c>
    #include <system.c>         // getPeripheralClock
    #include <interrupt.c>      // interrupts routines
    
    // PWM registers pointers
    volatile u16* pOCxCON;
    volatile u16* pOCxR;
    volatile u16* pOCxRS;

    // Global variables
    //volatile u32 gDuty50;
    volatile u32 gPeriodPlus1;
    volatile u16 gPhase = 0;    // 16-bit accumulator
    volatile u16 gFreq1Inc = 0;
    volatile u16 gFreq2Inc = 0;
             u32 gSampleRate;
             u8  gStaccato = true;

    // Duty cycle waveform table
    volatile const u16 sine64[64] = {
        50,54,59,64,68,73,77,81,85,88,91,93,95,97,98,99,99,99,98,97,95,
        93,91,88,85,81,77,73,68,64,59,54,50,45,40,35,31,26,22,18,14,11,
        8,6,4,2,1,0,0,0,1,2,4,6,8,11,14,18,22,26,31,35,40,45 };

    // Prototypes
    void Audio_init(u32 samplerate);
    void Audio_tone(u8 pin, u32 freq, u32 duration);
    void Audio_DTMF(u8 pin, u32 freq1, u32 freq2, u32 duration);
    void Audio_noTone(u8 pin);

/*  --------------------------------------------------------------------
    Audio_init
    --------------------------------------------------------------------
    @descr :            Generate a sample PWM period signal
                        Enable TIMER2 interrupt
    @param :            Samplerate frequency in Hertz
    @note  :            This function computes the best values for the 
                        TIMER2 prescaler and PR2 register depending on
                        current Peripheral Frequency.
    @return:            none
    @usage :            Audio.init(CDQUALITY);
    ------------------------------------------------------------------*/

    void Audio_init(u32 samplerate)
    {
        u8  prescaler;
        u32 resolution = 0x400;
        
        gSampleRate = samplerate;

        // TIMER2 period calculation
        gPeriodPlus1 = GetPeripheralClock() / samplerate;

        // The PWM period must not exceed the width of the Period Register
        
        if (gPeriodPlus1 < (resolution * 1))
        {
            prescaler = 0b000;          // prescaler is 1
        }
        else if (gPeriodPlus1 < (resolution * 2))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 1; // divided by 2
            prescaler = 0b001;          // prescaler is 2
        }
        else if (gPeriodPlus1 < (resolution * 4))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 2; // divided by 4
            prescaler = 0b010;          // prescaler is 4
        }
        else if (gPeriodPlus1 < (resolution * 8))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 3; // divided by 8
            prescaler = 0b011;          // prescaler is 8
        }
        else if (gPeriodPlus1 < (resolution * 16))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 4; // divided by 16
            prescaler = 0b100;          // prescaler is 16
        }
        else if (gPeriodPlus1 < (resolution * 32))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 5; // divided by 32
            prescaler = 0b101;          // prescaler is 32
        }
        else if (gPeriodPlus1 < (resolution * 64))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 6; // divided by 64
            prescaler = 0b110;          // prescaler is 64
        }
        else if (gPeriodPlus1 < (resolution * 256))
        {
            gPeriodPlus1 = gPeriodPlus1 >> 8; // divided by 256
            prescaler = 0b111;          // prescaler is 256
        }
        else
        {
            return;                     // period is too high
        }

        // Timer configuration
        PR2 = gPeriodPlus1 - 1;             // TIMER2 period
        TMR2  = 0;
        T2CON = 0x8000 | (prescaler<<4);    // Set prescaler and enable TIMER2

        // TIMER2 interrupt configuration
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntSetVectorPriority(INT_TIMER2_VECTOR, 7, 3);
        IntClearFlag(INT_TIMER2);
        IntEnable(INT_TIMER2);
    }

/*  --------------------------------------------------------------------
    Audio_tone
    --------------------------------------------------------------------
    Play sound with a certain frequency for a certain duration
    @param pin:         pin number where buzzer or loudspeaker is connected
    @param freq:        note frequency
    @param duration:    Duration in ms
    @return:            none
    @usage:             Audio.tone(PWM4, 440, 100); // LA 440Hz for 100 ms
    ------------------------------------------------------------------*/

    void Audio_tone(u8 pin, u32 freq, u32 duration)
    {
        switch (pin)
        {
            ///*********************************************************
            #if defined(PIC32_PINGUINO_220)
            ///*********************************************************

            case  2: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  3: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case 11: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case 12: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;
            case 13: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;

            ///*********************************************************
            #elif defined(PINGUINO32MX220) || \
                  defined(PINGUINO32MX250) || \
                  defined(PINGUINO32MX270)
            ///*********************************************************

            case  1: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  2: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  6: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case  7: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;
            case  8: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;

            ///*********************************************************
            #elif defined(PIC32_PINGUINO_MICRO)
            ///*********************************************************

            case 10: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case 11: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case 12: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case 13: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;

            ///*********************************************************
            #elif defined(PIC32_PINGUINO)     || \
                  defined(PIC32_PINGUINO_OTG)
            ///*********************************************************

            case  2: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
            case  1: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  0: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;

            ///*********************************************************
            #elif defined(EMPEROR460) || \
                  defined(EMPEROR795)
            ///*********************************************************

            case  0:
            case 72: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
            case  1:
            case 69: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case  2:
            case 68: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  3:
            case 67: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  4:
            case 66: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;

            ///*********************************************************
            #elif defined(UBW32_460) || \
                  defined(UBW32_795) || \
                  defined(PIC32_PINGUINO_T795)
            ///*********************************************************

            case  0:
            case 24:
            case 40: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
            case  1:
            case  9:
            case 43: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case  2:
            case  8:
            case 44: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  3:
            case  7:
            case 45: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  4:
            case 25:
            case 60: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;

            ///*********************************************************
            #else
            
                #error "Your board is not supported"
            ///*********************************************************

            #endif
        }

        // 16-bit accumulator
        // It will automatically goes back to zero once it reaches 0xFFFF
        gPhase = 0;
        
        // 16-bit accumulator's increment value = 65536 / (gSampleRate/freq)
        gFreq1Inc = 65536 * freq / gSampleRate;

        //pinmode(pin, OUTPUT);   // PWM pin as OUTPUT

        *pOCxCON = PWMMODE;     // PWM On
        
        Delayms(duration);
        
        if (gStaccato)
            *pOCxCON = 0;      // staccato (PWM Off)
    }

/*  --------------------------------------------------------------------
    dualTone (DTMF - dual-tone multi-frequency)
    --------------------------------------------------------------------
    Play sound with a certain frequency for a certain duration
    @param pin:         pin number where buzzer or loudspeaker is connected
    @param freq1:       1st frequency
    @param freq2:       2nd frequency
    @param duration:    Duration in ms
    ------------------------------------------------------------------*/

    #ifdef AUDIODTMF

    void Audio_dualTone(u8 pin, u32 freq1, u32 freq2, u32 duration)
    {
        switch (pin)
        {
            ///*********************************************************
            #if defined(PIC32_PINGUINO_220)
            ///*********************************************************
            
            case  2: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  3: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case 11: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case 12: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;
            case 13: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;

            ///*********************************************************
            #elif defined(PINGUINO32MX220) || \
                  defined(PINGUINO32MX250) || \
                  defined(PINGUINO32MX270)
            ///*********************************************************
        
            case  1: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  2: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  6: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case  7: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;
            case  8: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
    
            ///*********************************************************
            #elif defined(PIC32_PINGUINO_MICRO)
            ///*********************************************************

            case 10: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case 11: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case 12: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case 13: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;

            ///*********************************************************
            #elif defined(PIC32_PINGUINO) || \
                  defined(PIC32_PINGUINO_OTG)
            ///*********************************************************

            case  2: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
            case  1: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  0: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;

            ///*********************************************************
            #elif defined(EMPEROR460) || \
                  defined(EMPEROR795)
            ///*********************************************************

            case  0:
            case 72: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
            case  1:
            case 69: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case  2:
            case 68: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  3:
            case 67: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  4:
            case 66: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;

            ///*********************************************************
            #elif defined(UBW32_460) || \
                  defined(UBW32_795) || \
                  defined(PIC32_PINGUINO_T795)
            ///*********************************************************

            case  0:
            case 24:
            case 40: pOCxCON = (u16*)&OC1CON; pOCxR = (u16*)&OC1R; pOCxRS = (u16*)&OC1RS; break;
            case  1:
            case  9:
            case 43: pOCxCON = (u16*)&OC2CON; pOCxR = (u16*)&OC2R; pOCxRS = (u16*)&OC2RS; break;
            case  2:
            case  8:
            case 44: pOCxCON = (u16*)&OC3CON; pOCxR = (u16*)&OC3R; pOCxRS = (u16*)&OC3RS; break;
            case  3:
            case  7:
            case 45: pOCxCON = (u16*)&OC4CON; pOCxR = (u16*)&OC4R; pOCxRS = (u16*)&OC4RS; break;
            case  4:
            case 25:
            case 60: pOCxCON = (u16*)&OC5CON; pOCxR = (u16*)&OC5R; pOCxRS = (u16*)&OC5RS; break;

            ///*********************************************************
            #else
                #error "Your board is not supported"
            ///*********************************************************

            #endif
        }

        // 16-bit accumulator, will automatically go back to zero
        gPhase = 0;
        // 16-bit accumulator's increment value = 65536 / (gSampleRate/freq)
        gFreq1Inc = 65536 * freq1 / gSampleRate;
        gFreq2Inc = 65536 * freq2 / gSampleRate;

        //pinmode(pin, OUTPUT);   // PWM pin as OUTPUT
        //IntEnable(INT_TIMER2);  // Enable TIMER2 interrupt
        *pOCxCON = PWMMODE;     // PWM On
        
        Delayms(duration);
        
        if (gStaccato)
            *pOCxCON = 0;      // staccato
    }

    #endif // AUDIODTMF

/*  --------------------------------------------------------------------
    noTone
    --------------------------------------------------------------------
    @param pin:         pin number where loudspeaker is connected
    ------------------------------------------------------------------*/

    void Audio_noTone(u8 pin)
    {
        //pinmode(pin, INPUT);    // PWM pin as INPUT
        
        // We don't stop TIMER2 interrupt here
        // because user can use more than one PWM at a time
        //IntDisable(INT_TIMER2); // Disable TIMER2 interrupt

        switch (pin)            // PWM Off
        {
            ///*********************************************************
            #if defined(PIC32_PINGUINO_220)
            ///*********************************************************
            
            case 2:  OC3CON = 0; break;
            case 3:  OC4CON = 0; break;
            case 11: OC2CON = 0; break;
            case 12: OC5CON = 0; break;
            case 13: OC1CON = 0; break;

            ///*********************************************************
            #elif defined(PINGUINO32MX220) || \
                  defined(PINGUINO32MX250) || \
                  defined(PINGUINO32MX270)
            ///*********************************************************
        
            case 1:  OC3CON = 0; break;
            case 2:  OC4CON = 0; break;
            case 6:  OC2CON = 0; break;
            case 7:  OC5CON = 0; break;
            case 8:  OC1CON = 0; break;
    
            ///*********************************************************
            #elif defined(PIC32_PINGUINO_MICRO)
            ///*********************************************************

            case 10: OC2CON = 0; break;
            case 11: OC3CON = 0; break;
            case 12: OC4CON = 0; break;
            case 13: OC5CON = 0; break;

            ///*********************************************************
            #elif defined(PIC32_PINGUINO) || \
                  defined(PIC32_PINGUINO_OTG)
            ///*********************************************************

            case 2:  OC1CON = 0; break;
            case 1:  OC4CON = 0; break;
            case 0:  OC3CON = 0; break;

            ///*********************************************************
            #elif defined(EMPEROR460) || \
                  defined(EMPEROR795)
            ///*********************************************************

            case  0:
            case 72: OC1CON = 0; break;
            case  1:
            case 69: OC2CON = 0; break;
            case  2:
            case 68: OC3CON = 0; break;
            case  3:
            case 67: OC4CON = 0; break;
            case  4:
            case 66: OC5CON = 0; break;

            ///*********************************************************
            #elif defined(UBW32_460) || \
                  defined(UBW32_795) || \
                  defined(PIC32_PINGUINO_T795)
            ///*********************************************************

            case  0:
            case 24:
            case 40: OC1CON = 0; break;
            case  1:
            case  9:
            case 43: OC2CON = 0; break;
            case  2:
            case  8:
            case 44: OC3CON = 0; break;
            case  3:
            case  7:
            case 45: OC4CON = 0; break;
            case  4:
            case 25:
            case 60: OC5CON = 0; break;

            ///*********************************************************
            #else
            
                #error "Your board is not supported"
            ///*********************************************************

            #endif
        }
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
    Interrupt Service Routine
    --------------------------------------------------------------------
    In PWM mode, the OCxR register is a read-only slave duty cycle
    register and OCxRS is a buffer register that is written by the user
    to update the PWM duty cycle.

    On every timer to period register match event (end of PWM period),
    the duty cycle register, OCxR, is loaded with the contents of OCxRS.

    The TyIF interrupt flag is asserted at each PWM period boundary.

    The new PWM value must be computed in less than the PWM cycle gPeriodPlus1.
    If sample rate = CDQUALITY    -> Tpwm =  22 us (1/44100 sec.)
    If sample rate = TAPEQUALITY  -> Tpwm =  45 us
    If sample rate = RADIOQUALITY -> Tpwm =  90 us
    If sample rate = TELQUALITY   -> Tpwm = 113 us

    To generate a single clean tone we load the duty cycle register
    according to a pure sinusoid
    ------------------------------------------------------------------*/

void Timer2Interrupt(void)
{
    // Enable interrupt again
    IFS0CLR = 1 << INT_TIMER2;

    // Increment the 16-bit phase accumulator
    gPhase += gFreq1Inc;
    
    // Generates a new PWM output
    *pOCxCON = 0;           // PWM Off
    *pOCxR  = sine64[ gPhase & 0x3F ] * gPeriodPlus1 / 100;
    *pOCxRS = sine64[ gPhase & 0x3F ] * gPeriodPlus1 / 100;
    *pOCxCON = PWMMODE;     // PWM On
}

#endif // __AUDIO_C
