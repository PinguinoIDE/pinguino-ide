/*******************************************************************************
	Title:	USB Pinguino Bootloader
	File:	config.h
	Descr.: configuration bits for supported PIC18F
	Author:	Régis Blanchot <rblanchot@gmail.com>

	This file is part of Pinguino (http://www.pinguino.cc)
	Released under the LGPL license (http://www.gnu.org/licenses/lgpl.html)
*******************************************************************************/

#include <pic18fregs.h>

/**********************************************************************/
#if   defined(__18f13k50) || defined(__18f14k50)
/**********************************************************************/

    #error "    ---------------------------------    "
    #error "    Config. Words still to do for this chip"
    #error "    ---------------------------------    "

/**********************************************************************/
#elif defined(__18f4685)
/**********************************************************************/

	#pragma config IESO = OFF	// Internal/external switchover
	#pragma config FCMEN = OFF	// Fail-safe clock monitor
	#pragma config BORV = 3		// Brown-out Voltage set to 2.1V
	#pragma config BOREN = OFF	// Brown-out Reset bit
	#pragma config PWRT = OFF	// Power-up Timer
	#pragma config WDT = OFF	// Watch Dog Timer
	#pragma config MCLRE = ON	// MCLR Pin Enable bit
	#pragma config LPT1OSC = OFF	// Low Power Timer1 Oscilator
	#pragma config PBADEN = ON	// PORTB pins configured as digital I/O
	#pragma config DEBUG = OFF	// Background Debugger Enable bit
	#pragma config XINST = OFF	// Extended Instruction Set bit
	#pragma config BBSIZ = 1024	// Boot Block Size
	#pragma config LVP = ON		// RB5 is PGM pin
	#pragma config STVREN = ON	// Stack Full/Underflow will cause reset
	#pragma config OSC = IRCIO67	// Internal oscilator block, port
					// function on RA6 and RA7

/**********************************************************************/
#elif defined(__18f2550) || defined(__18f4550) || \
      defined(__18f2455) || defined(__18f4455)
/**********************************************************************/

    #if (SPEED == LOW_SPEED)
    
        #error "    ---------------------------------    "
        #error "    No low-speed mode on this device.    "
        #error "    ---------------------------------    "

    #elif (CRYSTAL == INTOSC)

        #error "    ---------------------------------    "
        #error "    Internal Crystal CAN NOT DRIVE USB Clock.     "
        #error "    ---------------------------------    "

    #else

        #if   (CRYSTAL == 4)
            #pragma config PLLDIV = 1
        #elif (CRYSTAL == 8)
            #pragma config PLLDIV = 2
        #elif (CRYSTAL == 12)
            #pragma config PLLDIV = 3
        #elif (CRYSTAL == 16)
            #pragma config PLLDIV = 4
        #elif (CRYSTAL == 20)
            #pragma config PLLDIV = 5
        #elif (CRYSTAL == 24)
            #pragma config PLLDIV = 6
        #elif (CRYSTAL == 40)
            #pragma config PLLDIV = 10
        #elif (CRYSTAL == 48)
            #pragma config PLLDIV = 12
        #else    
                #error "    ---------------------------------    "
                #error "    Crystal Frequency Not supported.     "
                #error "    ---------------------------------    "
        #endif

        #pragma config CPUDIV = OSC1_PLL2	// CPU_clk = PLL/2
        #pragma config USBDIV = 2			// USB_clk = PLL/2
        #pragma config FOSC = HSPLL_HS		// HS osc PLL

        #pragma config FCMEN = ON			// Fail Safe Clock Monitor
        #pragma config IESO = OFF			// Int/Ext switchover mode
        #pragma config PWRT = ON			// PowerUp Timer
        #pragma config BOR = ON 			// Brown Out
        #pragma config VREGEN = ON			// Int Voltage Regulator
        #pragma config WDT = OFF			// WatchDog Timer
        #pragma config MCLRE = ON			// MCLR
        #pragma config LPT1OSC = OFF		// Low Power OSC
        #pragma config PBADEN = OFF			// PORTB<4:0> A/D
        #pragma config CCP2MX = ON			// CCP2 Mux RC1
        #pragma config STVREN = ON			// Stack Overflow Reset
        #if (LVP == 0)
            #pragma config LVP = OFF		// High Voltage Programming
        #else
            #pragma config LVP = ON 		// Low Voltage Programming
        #endif
        #if defined(__18f4550)
            #pragma config ICPRT = OFF			// ICP
        #endif
        #pragma config XINST = OFF			// Ext CPU Instruction Set
        #pragma config DEBUG = OFF			// Background Debugging
        #pragma config CP0 = OFF			// Code Protect
        #pragma config CP1 = OFF
        #pragma config CP2 = OFF
        #pragma config CPB = OFF  			// Boot Sect Code Protect
        #pragma config CPD = OFF  			// EEPROM Data Protect
        #pragma config WRT0 = OFF 			// Table Write Protect
        #pragma config WRT1 = OFF
        #pragma config WRT2 = OFF 
        #pragma config WRTB = OFF  			// Boot Table Write Protect
        #pragma config WRTC = OFF  			// CONFIG Write Protect
        #pragma config WRTD = OFF 			// EEPROM Write Protect
        #pragma config EBTR0 = OFF			// Ext Table Read Protect
        #pragma config EBTR1 = OFF
        #pragma config EBTR2 = OFF
        #pragma config EBTRB = OFF 			// Boot Table Read Protect

    #endif

/**********************************************************************/
#elif defined(__18f26j50) || defined(__18f46j50)
/**********************************************************************/

    #if (CRYSTAL == INTOSC)

        #pragma config OSC = INTOSCPLL  // internal RC oscillator, PLL enabled, HSPLL used by USB
        #pragma config PLLDIV = 2       // 8MHz/2 = The PLL requires a 4 MHz input and it produces a 96 MHz output.

    #else

        #pragma config OSC = HSPLL      // external oscillator, PLL enabled, HSPLL used by USB

        //#pragma config PLLDIV = CRYSTAL / 4
        
        #if   (CRYSTAL == 4)
            #pragma config PLLDIV = 1
        #elif (CRYSTAL == 8)
            #pragma config PLLDIV = 2
        #elif (CRYSTAL == 12)
            #pragma config PLLDIV = 3
        #elif (CRYSTAL == 16)
            #pragma config PLLDIV = 4
        #elif (CRYSTAL == 20)
            #pragma config PLLDIV = 5
        #elif (CRYSTAL == 24)
            #pragma config PLLDIV = 6
        #elif (CRYSTAL == 40)
            #pragma config PLLDIV = 10
        #elif (CRYSTAL == 48)
            #pragma config PLLDIV = 12
        #else    
                #error "    ---------------------------------    "
                #error "    Crystal Frequency Not supported.     "
                #error "    ---------------------------------    "
                #error "*** BREAKPOINT ***"
        #endif

    #endif

    // DS39931D-page 40 - 2011 Microchip Technology Inc.
    // PIC18F46J50 family core must run at 24 MHz in order for the USB module
    // to get the 6 MHz clock needed for low-speed USB operation.

    #if (SPEED == LOW_SPEED)
        #pragma config CPUDIV = OSC2_PLL2// 24 MHz (CPU system clock divided by 2)
    #else
        #pragma config CPUDIV = OSC1    // 48 MHz (No CPU system clock divide)
    #endif
    
    #pragma config WDTEN = OFF          // WDT disabled (enabled by SWDTEN bit)
    #pragma config STVREN = ON          // stack overflow/underflow reset enabled
    #pragma config XINST = OFF          // Extended instruction set disabled
    #pragma config CP0 = OFF            // Program memory is not code-protected
    #pragma config IESO = OFF           // Two-Speed Start-up disabled
    #pragma config FCMEN = OFF          // Fail-Safe Clock Monitor disabled
    #pragma config LPT1OSC = OFF        // high power Timer1 mode
    #pragma config T1DIG = ON           // Sec Osc clock source may be selected
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

    // GPUTILS  rev. 995 - bug #269 (https://sourceforge.net/p/gputils/bugs/269/)
    // fixed in rev. 996
    #pragma config WPDIS = OFF          // WPFP, WPEND, and WPCFG bits ignored 

/**********************************************************************/
#elif defined(__18f25k50) || defined(__18f45k50)// Config. Words for Internal Crystal (16 MHz) use
/**********************************************************************/

    #pragma config CFGPLLEN = ON        // PLL Enable Configuration bit (PLL Enabled)
    #pragma config CPUDIV = NOCLKDIV    // 1:1 mode (for 48MHz CPU)

    #if (CRYSTAL == INTOSC)             // Internal 16 MHz Osc.
        #pragma config PLLSEL = PLL3X       // PLL Selection (3x clock multiplier) => 3 x 16 = 48 MHz
        #pragma config FOSC = INTOSCIO      // Oscillator Selection (Internal oscillator)

    #elif (CRYSTAL == 12)
        #pragma config PLLSEL = PLL4X       // PLL Selection (3x clock multiplier) => 3 x 16 = 48 MHz
        #pragma config FOSC = HSOSCIO      // Oscillator Selection (Internal oscillator)

    #elif (CRYSTAL == 16)
        #pragma config PLLSEL = PLL3X       // PLL Selection (3x clock multiplier) => 3 x 16 = 48 MHz
        #pragma config FOSC = HSOSCIO      // Oscillator Selection (Internal oscillator)

    #elif (CRYSTAL == 48)
        #pragma config PLLSEL = PLL3X      // Oscillator used directly
        #pragma config FOSC = HSOSCIO      // Oscillator Selection (Internal oscillator)

    #else    
        #error "    ---------------------------------    "
        #error "    Crystal Frequency Not supported.     "
        #error "    ---------------------------------    "

    #endif
    
    #pragma config LS48MHZ = SYS24X4// USB Low-speed clock at 24 MHz, USB clock divider is set to 4

    // CONFIG1H
    #pragma config PCLKEN = ON          // Primary Oscillator Shutdown (Primary oscillator enabled)
    #pragma config FCMEN = OFF          // Fail-Safe Clock Monitor (Fail-Safe Clock Monitor disabled)
    #pragma config IESO = OFF           // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)

    // CONFIG2L
    #pragma config nPWRTEN = OFF        // Power-up Timer Enable (Power up timer disabled)
    #pragma config BOREN = SBORDIS      // Brown-out Reset Enable (BOR enabled in hardware (SBOREN is ignored))
    #pragma config BORV = 190           // Brown-out Reset Voltage (BOR set to 1.9V nominal)
    #pragma config nLPBOR = OFF         // Low-Power Brown-out Reset (Low-Power Brown-out Reset disabled)

    // CONFIG2H
    #pragma config WDTEN = SWON         // Watchdog Timer Enable bits (WDT controlled by firmware (SWDTEN enabled))
    #pragma config WDTPS = 32768        // Watchdog Timer Postscaler (1:32768)

    // CONFIG3H
    #pragma config CCP2MX = RC1         // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
    #pragma config PBADEN = OFF         // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
    #pragma config T3CMX = RC0          // Timer3 Clock Input MUX bit (T3CKI function is on RC0)
    #pragma config SDOMX = RC7          // SDO Output MUX bit (SDO function is on RC7)
    #pragma config MCLRE = ON           // Master Clear Reset Pin Enable (MCLR pin enabled; RE3 input disabled)

    // CONFIG4L
    #pragma config STVREN = ON          // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
    #if (LVP == 0)
        #pragma config LVP = OFF		// High Voltage Programming
    #else
        #pragma config LVP = ON 		// Low Voltage Programming
    #endif
    #pragma config XINST = OFF          // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled)
    #if defined(__18f45k50)
    #pragma config ICPRT = OFF          // ICP
    #endif
    
    // CONFIG5L
    #pragma config CP0 = OFF            // Block 0 Code Protect (Block 0 is not code-protected)
    #pragma config CP1 = OFF            // Block 1 Code Protect (Block 1 is not code-protected)
    #pragma config CP2 = OFF            // Block 2 Code Protect (Block 2 is not code-protected)
    #pragma config CP3 = OFF            // Block 3 Code Protect (Block 3 is not code-protected)

    // CONFIG5H
    #pragma config CPB = OFF            // Boot Block Code Protect (Boot block is not code-protected)
    #pragma config CPD = OFF            // Data EEPROM Code Protect (Data EEPROM is not code-protected)

    // CONFIG6L
    #pragma config WRT0 = OFF           // Block 0 Write Protect (Block 0 (0800-1FFFh) is not write-protected)
    #pragma config WRT1 = OFF           // Block 1 Write Protect (Block 1 (2000-3FFFh) is not write-protected)
    #pragma config WRT2 = OFF           // Block 2 Write Protect (Block 2 (04000-5FFFh) is not write-protected)
    #pragma config WRT3 = OFF           // Block 3 Write Protect (Block 3 (06000-7FFFh) is not write-protected)

    // CONFIG6H
    #pragma config WRTC = OFF           // Configuration Registers Write Protect (Configuration registers (300000-3000FFh) are not write-protected)
    #pragma config WRTB = OFF           // Boot Block Write Protect (Boot block (0000-7FFh) is not write-protected)
    #pragma config WRTD = OFF           // Data EEPROM Write Protect (Data EEPROM is not write-protected)

    // CONFIG7L
    #pragma config EBTR0 = OFF          // Block 0 Table Read Protect (Block 0 is not protected from table reads executed in other blocks)
    #pragma config EBTR1 = OFF          // Block 1 Table Read Protect (Block 1 is not protected from table reads executed in other blocks)
    #pragma config EBTR2 = OFF          // Block 2 Table Read Protect (Block 2 is not protected from table reads executed in other blocks)
    #pragma config EBTR3 = OFF          // Block 3 Table Read Protect (Block 3 is not protected from table reads executed in other blocks)

    // CONFIG7H
    #pragma config EBTRB = OFF          // Boot Block Table Read Protect (Boot block is not protected from table reads executed in other blocks)

/**********************************************************************/
#elif defined(__18f26j53) || defined(__18f46j53)|| \
      defined(__18f27j53) || defined(__18f47j53)
/**********************************************************************/

    #if (CRYSTAL == INTOSC)             // Internal 8 MHz Osc.
        #pragma config OSC = INTOSCPLL  // Oscillator Selection (Internal oscillator)
        #pragma config PLLDIV = 2       // 8 MHz / 2 : The PLL requires a 4 MHz input and it produces a 96 MHz output.
    #else
        #pragma config OSC = HSPLL      // Oscillator Selection (External oscillator)

        #if   (CRYSTAL == 4)
            #pragma config PLLDIV = 1
        #elif (CRYSTAL == 8)
            #pragma config PLLDIV = 2
        #elif (CRYSTAL == 12)
            #pragma config PLLDIV = 3
        #elif (CRYSTAL == 16)
            #pragma config PLLDIV = 4
        #elif (CRYSTAL == 20)
            #pragma config PLLDIV = 5
        #elif (CRYSTAL == 24)
            #pragma config PLLDIV = 6
        #elif (CRYSTAL == 40)
            #pragma config PLLDIV = 10
        #elif (CRYSTAL == 48)
            #pragma config PLLDIV = 12
        #else    
                #error "    ---------------------------------    "
                #error "    Crystal Frequency Not supported.     "
                #error "    ---------------------------------    "
        #endif

    #endif
    
    // CONFIG1L
    //#pragma config DEBUG = OFF			// Background Debugging
    #pragma config XINST = OFF			// Ext CPU Instruction Set
    #pragma config STVREN = ON          // Stack Full/Underflow Reset (Stack full/underflow will cause Reset)
    #pragma config CFGPLLEN = ON        // PLL Enable Configuration bit (PLL Enabled)
    #pragma config WDTEN = OFF          // WDT disabled (enabled by SWDTEN bit)

    // CONFIG1H
    #pragma config CP0 = OFF            // Block 0 Code Protect (Block 0 is not code-protected)
    #pragma config CPUDIV = OSC1        // 48 MHz (No CPU system clock divide)

    // CONFIG2L
    #pragma config IESO = OFF           // Internal/External Oscillator Switchover (Oscillator Switchover mode disabled)
    #pragma config FCMEN = OFF          // Fail-Safe Clock Monitor disabled
    #pragma config CLKOEC = OFF         // CLKO output disabled on the RA6 pin
    //#pragma config SOSCSEL = HIGH       // Digital (SCLKI) mode selected

    // CONFIG2H
    #pragma config WDTPS = 32768        // Watchdog Timer Postscaler (1:32768)

    // CONFIG3L
    #pragma config DSWDTPS = 8192       // 1:8,192 (8.5 seconds)
    #pragma config DSWDTEN = OFF        // Disabled
    #pragma config DSBOREN = OFF        // Zero-Power BOR disabled in Deep Sleep

    #if (RTCC == INTOSC)
    #pragma config RTCOSC   = INTOSCREF // RTCC uses INTOSC as clock
    #pragma config DSWDTOSC = INTOSCREF // DSWDT uses INTOSC as clock
    #else
    #pragma config RTCOSC   = T1OSCREF  // RTCC uses T1OSC/T1CKI as clock
    #pragma config DSWDTOSC = T1OSCREF  // DSWDT uses T1OSC/T1CKI as clock
    #endif
    
    // CONFIG3H
    #pragma config MSSP7B_EN = MSK7     // 7 Bit address masking
    #pragma config ADCSEL = BIT12       // 12-bit conversion mode is enabled
    #pragma config IOL1WAY = OFF        // IOLOCK bit can be set and cleared
    
    // CONFIG4L
    #pragma config WPCFG = OFF          // Write/Erase last page protect Disabled
    #pragma config WPFP = PAGE_0        // Write Protect Program Flash Page 0

    // CONFIG4H
    #pragma config LS48MHZ = SYS24X4// USB Low-speed clock at 24 MHz, USB clock divider is set to 4
    #pragma config WPEND = PAGE_0       // Start protection at page 0
    #pragma config WPDIS = OFF          // WPFP[5:0], WPEND, and WPCFG bits ignored 

/*
    static __code char __at __CONFIG1L _c1l = 0x8C;
    static __code char __at __CONFIG1H _c1h = 0x03;
    static __code char __at __CONFIG2L _c2l = 0x1A;
    static __code char __at __CONFIG2H _c2h = 0x08;
    static __code char __at __CONFIG3L _c3l = 0x65;
    static __code char __at __CONFIG3H _c3h = 0x08;
    static __code char __at __CONFIG4L _c4l = 0x80;
    static __code char __at __CONFIG4H _c4h = 0x0B; //0xF1;
*/
    
/**********************************************************************/
#else
/**********************************************************************/

    #error "    -------------------------------    "
    #error "    NO CONFIG. WORDS FOR YOUR CHIP.    "
    #error "    -------------------------------    "
    
#endif
