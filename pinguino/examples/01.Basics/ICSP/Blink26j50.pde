/*	----------------------------------------------------------------------------
    Blink a LED connected to RC2 on a Pinguino 26j50
    ----------------------------------------------------------------------------
    author:	Régis Blanchot
    ----------------------------------------------------------------------------
    READ THIS CAREFULLY :
    This is intended only for people who don't want to use USB bootloader
    but an ICSP programmer instead.
    --------------------------------------------------------------------------*/

/// Config Bits

#pragma config OSC = INTOSCPLL      // internal RC oscillator, PLL enabled, HSPLL used by USB
#pragma config PLLDIV = 2           // 8MHz/2 = The PLL requires a 4 MHz input and it produces a 96 MHz output.
#pragma config CPUDIV = OSC1        // 48 MHz (No CPU system clock divide)
#pragma config WDTEN = OFF          // WDT disabled (enabled by SWDTEN bit)
#pragma config STVREN = OFF         // stack overflow/underflow reset enabled
#pragma config XINST = OFF          // Extended instruction set disabled
#pragma config DEBUG = OFF          // Background Debug disabled
#pragma config CP0 = OFF            // Program memory is not code-protected
#pragma config IESO = OFF           // Two-Speed Start-up disabled
#pragma config FCMEN = OFF          // Fail-Safe Clock Monitor disabled
#pragma config LPT1OSC = OFF        // high power Timer1 mode
#pragma config T1DIG = OFF          // Sec Osc clock source may be selected
#pragma config WDTPS = 256          // 1:256 (1 second)
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
    TRISCbits.TRISC2 = OUTPUT;
}

void loop()
{
    LATCbits.LATC2 ^= 1;
    delay(50);
}
