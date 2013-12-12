/*	----------------------------------------------------------------------------
    FILE:			ili9325.h
    PROJECT:		pinguino
    PURPOSE:		ILI9325 Registers
    PROGRAMER:		regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:	2 Sep. 2012
    LAST RELEASE:	7 Dec. 2013
    ----------------------------------------------------------------------------
    TODO : 
    ----------------------------------------------------------------------------
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
    --------------------------------------------------------------------------*/

#ifndef __ILI9325_H
#define __ILI9325_H

#include <digitalw.c>
#include <macro.h>
#ifdef __PMP__
#include <pmp.h>
#endif

/**	--------------------------------------------------------------------
    Registers
    ------------------------------------------------------------------*/

    #define DriverCodeRead              		0x00
    #define DriverOutputControl				    0x01
    #define LCDDrivingWaveControl			    0x02
    #define EntryMode						    0x03
    #define ResizingControlRegister			    0x04
    #define DisplayControl1             		0x07
    #define DisplayControl2             		0x08
    #define DisplayControl3             		0x09
    #define DisplayControl4             		0x0A
    #define RGBDisplayInterfaceControl1         0x0C
    #define FrameMarkerPosition         		0x0D
    #define RGBDisplayInterfaceControl2 		0x0F

    #define PowerControl1					    0x10
    #define PowerControl2					    0x11
    #define PowerControl3					    0x12
    #define PowerControl4					    0x13

    #define GRAMHorizontalAddressSet		    0x20
    #define GRAMVerticalAddressSet 			    0x21
    #define WriteDatatoGRAM					    0x22
    #define ReadDatatoGRAM					    0x22
    #define PowerControl7               		0x29
    #define FrameRateandColorControl    		0x2B

    #define GammaControl1               		0x30
    #define GammaControl2               		0x31
    #define GammaControl3		                0x32
    #define GammaControl4	                	0x35
    #define GammaControl5		                0x36
    #define GammaControl6   	                0x37
    #define GammaControl7		                0x38
    #define GammaControl8		                0x39
    #define GammaControl9               		0x3C
    #define GammaControl10		                0x3D

    #define HorizontalRAMStartAddressPosition	0x50
    #define HorizontalRAMEndAddressPosition		0x51
    #define VerticalRAMStartAddressPosition		0x52
    #define VerticalRAMEndAddressPosition		0x53

    #define DriverOutputControl2         		0x60
    #define BaseImageDisplayControl     		0x61
    #define VerticalScrollControl        		0x6A

    #define PartialImage1DisplayPosition		0x80
    #define PartialImage1RAMStartAddress		0x81
    #define PartialImage1RAMEndAddress  		0x82
    #define PartialImage2DisplayPosition		0x83
    #define PartialImage2RAMStartAddress		0x84
    #define PartialImage2RAMEndAddress  		0x85

    #define PanelInterfaceControl1      		0x90
    #define PanelInterfaceControl2      		0x92
    #define PanelInterfaceControl3      		0x93
    #define PanelInterfaceControl4      		0x95
    #define PanelInterfaceControl5      		0x97
    #define PanelInterfaceControl6      		0x98

    #define OTPVCMProgrammingControl            0xA1
    #define OTPVCMStatusandEnable               0xA2
    #define OTPProgrammingIDKey                 0xA5

/**	--------------------------------------------------------------------
    Constants
    ------------------------------------------------------------------*/

    #define PORTRAIT	100
    #define LANDSCAPE	101

    #define White		0xFFFF
    #define Black		0x0000
    #define Grey		0xF7DE
    #define Blue		0x001F
    #define Blue2		0x051F
    #define Red			0xF800
    #define Magenta		0xF81F
    #define Green		0x07E0
    #define Cyan		0x7FFF
    #define Yellow		0xFFE0

    #define BLACK              0b0000000000000000
    #define BRIGHTBLUE         0b0000000000011111
    #define BRIGHTGREEN        0b0000011111100000
    #define BRIGHTCYAN         0b0000011111111111
    #define BRIGHTRED          0b1111100000000000
    #define BRIGHTMAGENTA      0b1111100000011111
    #define BRIGHTYELLOW       0b1111111111100000
    #define BLUE               0b0000000000010000
    #define GREEN              0b0000010000000000
    #define CYAN               0b0000010000010000
    #define RED                0b1000000000000000
    #define MAGENTA            0b1000000000010000
    #define BROWN              0b1111110000000000
    #define LIGHTGRAY          0b1000010000010000
    #define DARKGRAY           0b0100001000001000
    #define LIGHTBLUE          0b1000010000011111
    #define LIGHTGREEN         0b1000011111110000
    #define LIGHTCYAN          0b1000011111111111
    #define LIGHTRED           0b1111110000010000
    #define LIGHTMAGENTA       0b1111110000011111
    #define YELLOW             0b1111111111110000
    #define ORANGE			   0b1111101111100000
    #define WHITE              0b1111111111111111

    #define GRAY0       	   0b1110011100011100
    #define GRAY1         	   0b1100011000011000   
    #define GRAY2              0b1010010100010100   
    #define GRAY3              0b1000010000010000
    #define GRAY4              0b0110001100001100
    #define GRAY5              0b0100001000001000
    #define GRAY6	           0b0010000100000100

/**	--------------------------------------------------------------------
    Typedef.
    ------------------------------------------------------------------*/

    typedef struct
    {
        u16 x;
        u16 y;
    } coord_t;

    typedef struct
    {
        u8 width;
        u8 height;
    } font_t;

    typedef struct
    {
        u8 r;			// 8/8/8 representation
        u8 g;
        u8 b;
        u16 c;			// 5/6/5 representation
    } color_t;

    typedef struct
    {
        u16 startx;
        u16 starty;
        u16 endx;
        u16 endy;
        u16 width;
        u16 height;
    } rect_t;

    typedef union
    {
        u16 w;
        struct
        {
            u8 h8;
            u8 l8;
        };
    } word_t;

    typedef struct
    {
        u8 orientation;
        rect_t screen;
        color_t bcolor;
        color_t color;
        coord_t cursor;
        font_t font;
    } lcd_t;

/**	----------------------------------------------------------------------------
    PIC     26J50   47J53A  32MX250 EMPEROR LCD pin
    ----------------------------------------------------------------------------
    VSS     GND     GND     GND     GND     GND
    +5V     5V      VIN     VDD     5V      5V
    -       -       -       -       -       nc
    ----------------------------------------------------------------------------
    PMA0    8       5               26      RS  Register Selection
    PMWR    9       14              64      RW  Write: Low=Enable, High=Disable
    PMRD    10      13              63      RD  Read:  Low=Enable, High=Disable
    ----------------------------------------------------------------------------
    PMD0    0       24              54      D0  Display data, LSB
    PMD1    1       25              53      D1  Display data
    PMD2    2       26              49      D2  Display data
    PMD3    3       27              48      D3  Display data
    PMD4    4       28              47      D4  Display data
    PMD5    5       29              45      D5  Display data
    PMD6    6       30              44      D6  Display data
    PMD7    7       31              43      D7  Display data, MSB
    ----------------------------------------------------------------------------
    PMA1    11      4               74      CS  Low=Select, High=Deselect
    -       -       -               -       nc
    PMA2    12      4                       RST Low=Reset, High=no Reset
    -       -       -               -       nc
    --------------------------------------------------------------------------*/

#if defined(__18f2455)  || defined(__18f2550)  || \
    defined(__18f25k50) || defined(__18f26j50) || \
    defined(__18f26j53) || defined(__18f27j53)

    #define DATA		LATB    // RB0 to RB7
    #define dDATA		TRISB
    #define CMD			LATA
    #define dCMD		TRISA   // RS, WR, RD, CS and RST

    #define RS			0       // RA0
    #define WR			1       // RA1
    #define RD			2       // RA2
    #define CS			3       // RA3
    #define RST			5       // RA5

    #define LowRS		BitClear(CMD,RS)// Register Mode
    #define HighRS		BitSet(CMD,RS)// Data Mode

    #define LowWR		BitClear(CMD,WR)// Enable Write
    #define HighWR		BitSet(CMD,WR)// Disable Write

    #define LowRD		BitClear(CMD,RD)// Enable Read
    #define HighRD		BitSet(CMD,RD)// Disable Read
    
    #define LowCS		BitClear(CMD,CS)// Enable LCD
    #define HighCS		BitSet(CMD,CS)// Disbale LCD
    
    #define LowRST		BitClear(CMD,RST)// Reset
    #define HighRST		BitSet(CMD,RST)// End Reset
/*
    #define LowRS		__asm bcf CMD,RS __endasm;// Register Mode
    #define HighRS		__asm bsf CMD,RS __endasm;// Data Mode

    #define LowWR		__asm bcf CMD,WR __endasm;// Enable Write
    #define HighWR		__asm bsf CMD,WR __endasm;// Disable Write

    #define LowRD		__asm bcf CMD,RD __endasm;// Enable Read
    #define HighRD		__asm bsf CMD,RD __endasm;// Disable Read
    
    #define LowCS		__asm bcf CMD,CS __endasm;// Enable LCD
    #define HighCS		__asm bsf CMD,CS __endasm;// Disbale LCD
    
    #define LowRST		__asm bcf CMD,RST __endasm;// Reset
    #define HighRST		__asm bsf CMD,RST __endasm;// End Reset
*/

#elif defined(__18f4455)  || defined(__18f4550)  || \
      defined(__18f45k50) || defined(__18f46j50) || \
      defined(__18f46j53) || defined(__18f47j53)

    #ifdef __PMP__

        #define RS          PMA0//5
        #define CS          PMCS1//15
        #define RST         4//PMA1//4 
        
    #else

        #define DATA        LATD    // RD0 to RD7
        #define dDATA       TRISD
        #define CMD1        LATE
        #define dCMD1       TRISE   // WR, RD and CS
        #define CMD2        LATB
        #define dCMD2       TRISB   // RS and RST

        #define RS          5       // PMA0  = RB5
        #define WR          1       // PMWR  = RE1
        #define RD          0       // PMRD  = RE0
        #define CS          2       // PMCS1 = RE2
        #define RST         4       //         RB4

        #define LowRS       LATBbits.LATB5 = 0  // Register Mode
        #define HighRS      LATBbits.LATB5 = 1  // Data Mode

        #define LowWR       LATEbits.LATE1 = 0  // Enable Write
        #define HighWR      LATEbits.LATE1 = 1  // Disable Write

        #define LowRD       LATEbits.LATE0 = 0  // Enable Read
        #define HighRD      LATEbits.LATE0 = 1  // Disable Read
        
        #define LowCS       LATEbits.LATE2 = 0  // Enable LCD
        #define HighCS      LATEbits.LATE2 = 1  // Disbale LCD
        
        #define LowRST      LATBbits.LATB4 = 0  // Reset
        #define HighRST     LATBbits.LATB4 = 1  // End Reset

        /*
        #define LowRS       BitClear(CMD2,RS)   // Register Mode
        #define HighRS      BitSet(CMD2,RS)     // Data Mode

        #define LowWR       BitClear(CMD1,WR)   // Enable Write
        #define HighWR      BitSet(CMD1,WR)     // Disable Write

        #define LowRD       BitClear(CMD1,RD)   // Enable Read
        #define HighRD      BitSet(CMD1,RD)     // Disable Read
        
        #define LowCS       BitClear(CMD1,CS)   // Enable LCD
        #define HighCS      BitSet(CMD1,CS)     // Disbale LCD
        
        #define LowRST      BitClear(CMD2,RST)  // Reset
        #define HighRST     BitSet(CMD2,RST)    // End Reset

        #define LowRS       __asm bcf _LATB,RS __endasm;// Register Mode
        #define HighRS      __asm bsf _LATB,RS __endasm;// Data Mode

        #define LowWR       __asm bcf _LATE,WR __endasm;// Enable Write
        #define HighWR      __asm bsf _LATE,WR __endasm;// Disable Write

        #define LowRD       __asm bcf _LATE,RD __endasm;// Enable Read
        #define HighRD      __asm bsf _LATE,RD __endasm;// Disable Read
        
        #define LowCS       __asm bcf _LATE,CS __endasm;// Enable LCD
        #define HighCS      __asm bsf _LATE,CS __endasm;// Disbale LCD
        
        #define LowRST      __asm bcf _LATB,RST __endasm;// Reset
        #define HighRST     __asm bsf _LATB,RST __endasm;// End Reset
        */

    #endif

#endif

/**	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

    // private

    #ifdef __PMP__
    
    void ILI9325_writeRegister(u16);
    void ILI9325_writeData(u16);
    void ILI9325_write(u16, u16);
    u16  ILI9325_readData();
    u16  ILI9325_read(u16);
    
    #else
    
    void ILI9325_writeRegister(u8);
    void ILI9325_writeData(u16);
    void ILI9325_write(u8, u16);
    u16  ILI9325_readData();
    u16  ILI9325_read(u8);
    
    #endif

    // public

    u16  ILI9325_getDevice();
    u16  ILI9325_color565(u8, u8, u8);
    void ILI9325_setColor(u16);
    void ILI9325_setBackgroundColor(u16);
    void ILI9325_setCursor(u16, u16);
    void ILI9325_setWindow(u16, u16, u16, u16);
    void ILI9325_setPortrait();
    void ILI9325_setLandscape();
    u16  ILI9325_getScreenWidth();
    u16  ILI9325_getScreenHeight();
    const u8 ILI9325_getFontWidth();
    const u8 ILI9325_getFontHeight();
    void ILI9325_drawPixel(u16, u16);
    u16  ILI9325_readPixel(u16, u16);
    void ILI9325_clearScreen();
    void ILI9325_scroll(u16);
    void ILI9325_init();
    void ILI9325_enterSleep();
    void ILI9325_exitSleep();

/**	--------------------------------------------------------------------
    Globals
    ------------------------------------------------------------------*/

    lcd_t ILI9325;
    
#endif /* __ILI9325_H */
