/*****************************************
    Go to Deep Sleep for 1 second
    Toggle built-in led at wake-up
*****************************************/

/* ----- Config. Bits ----- */

#pragma config OSC = INTOSCPLL      // internal RC oscillator, PLL enabled, HSPLL used by USB
#pragma config PLLDIV = 2           // 8MHz/2 = The PLL requires a 4 MHz input and it produces a 96 MHz output.
#pragma config CPUDIV = OSC1        // 48 MHz (No CPU system clock divide)
#pragma config WDTEN = ON           // WDT disabled (enabled by SWDTEN bit)
#pragma config STVREN = OFF         // stack overflow/underflow reset enabled
#pragma config XINST = OFF          // Extended instruction set disabled
#pragma config DEBUG = OFF          // Background Debug disabled
#pragma config CP0 = OFF            // Program memory is not code-protected
#pragma config IESO = OFF           // Two-Speed Start-up disabled
#pragma config FCMEN = OFF          // Fail-Safe Clock Monitor disabled
#pragma config LPT1OSC = OFF        // high power Timer1 mode
//#pragma config T1DIG = OFF          // Sec Osc clock source may be selected
#pragma config WDTPS = 1024          // 1:256 (1 second)
#pragma config DSWDTPS = 8192       // 1:8192 (8.5 seconds)
#pragma config DSWDTEN = OFF        // Disabled
#pragma config DSBOREN = OFF        // Zero-Power BOR disabled in Deep Sleep
#pragma config RTCOSC = INTOSCREF   // RTCC uses INTOSC as clock
#pragma config DSWDTOSC = INTOSCREF // DSWDT uses INTOSC as clock
#pragma config MSSP7B_EN = MSK7     // 7 Bit address masking
#pragma config IOL1WAY = OFF        // IOLOCK bit can be set and cleared
#pragma config WPCFG = OFF          // Write/Erase last page protect Disabled
#pragma config WPEND = PAGE_0       // Start protection at page 0
#pragma config WPFP = PAGE_1        // Write Protect Program Flash Page 0
#pragma config WPDIS = OFF          // WPFP[5:0], WPEND, and WPCFG bits ignored 

void setup()
{
    /* ----- Internal oscillator init (optional) ----- */

    //System.setIntOsc(_48MHZ_);
    
    /* ----- First start init ----- */
  
    if ( !System.deepSleepWakeUp() )
    { 
        RTCC.setTime(0x00000000);
        RTCC.setAlarmTime(0x00000000);
    }
  
    /* ----- RTCC Alarm init ----- */
    
    RTCC.alarmEverySecond();            // allow alarm at every second
    RTCC.chimeEnable();                 // allow indefinite alarm repetition
    RTCC.alarmEnable();                 // enable alarm
    RTCC.alarmIntEnable();              // enable alarm interrupt to wake-up from deep sleep mode
    RTCC.enable();                      // enable RTCC module

    /* ----- Toggle built-in led ----- */
    
    pinMode(USERLED, OUTPUT);
    toggle(USERLED);

    /* ----- Deep sleep ----- */

    // Go to deep sleep until RTCC alarm occurs.
    // If RTCC alarm occurs do a Power-on Reset.
    // The program will then restart from the beginning (vector 0x00).
    
    System.deepSleep();
}

void loop()
{
}
