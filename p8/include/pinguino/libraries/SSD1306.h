/*	--------------------------------------------------------------------
	FILE:			SSD1306.c
	PROJECT:		pinguino
	PURPOSE:		Drive 0.96" 128x64 Oled display (SSD1306 controller)
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	17 Oct. 2013
	LAST RELEASE:	25 Mar. 2014
	----------------------------------------------------------------------------
    * Done : 8-BIT 68XX/80XX PARALLEL
    * Todo : 3-/4-WIRE SPI
    * Todo : I2C
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
	------------------------------------------------------------------*/

#ifndef __SSD1306_H
#define __SSD1306_H

#include <typedef.h>

/**	--------------------------------------------------------------------
    Display interfaces
    ------------------------------------------------------------------*/

    #define SSD1306_PMP                 1<<0    // Parallel port
    
    #define SSD1306_I2C                 1<<1
    #define SSD1306_6800                1<<2
    #define SSD1306_8080                1<<3    // 
    #define SSD1306_SPI3                1<<4
    #define SSD1306_SPI4                1<<5

/**	--------------------------------------------------------------------
    Display sizes
    ------------------------------------------------------------------*/

    #define SSD1306_128X32              1<<6
    #define SSD1306_128X64              1<<7
    
/**	--------------------------------------------------------------------
    Display commands
    ------------------------------------------------------------------*/

    #define SSD1306_SETCONTRAST 0x81
    #define SSD1306_DISPLAYALLON_RESUME 0xA4
    #define SSD1306_DISPLAYALLON 0xA5
    #define SSD1306_NORMALDISPLAY 0xA6
    #define SSD1306_INVERTDISPLAY 0xA7
    #define SSD1306_DISPLAYOFF 0xAE
    #define SSD1306_DISPLAYON 0xAF

    #define SSD1306_SETDISPLAYOFFSET 0xD3
    #define SSD1306_SETCOMPINS 0xDA

    #define SSD1306_SETVCOMDETECT 0xDB

    #define SSD1306_SETDISPLAYCLOCKDIV 0xD5
    #define SSD1306_SETPRECHARGE 0xD9

    #define SSD1306_SETMULTIPLEX 0xA8

    #define SSD1306_SETLOWCOLUMN 0x00
    #define SSD1306_SETHIGHCOLUMN 0x10

    #define SSD1306_SETSTARTLINE 0x40

    #define SSD1306_MEMORYMODE 0x20

    #define SSD1306_COMSCANINC 0xC0
    #define SSD1306_COMSCANDEC 0xC8

    #define SSD1306_SEGREMAP 0xA0
    #define SSD1306_SEGREMAPINV 0xA1

    #define SSD1306_CHARGEPUMP 0x8D

    #define SSD1306_EXTERNALVCC 0x1
    #define SSD1306_SWITCHCAPVCC 0x2

    // Scrolling #defines
    #define SSD1306_ACTIVATE_SCROLL 0x2F
    #define SSD1306_DEACTIVATE_SCROLL 0x2E
    #define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
    #define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
    #define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
    #define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
    #define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

/**	--------------------------------------------------------------------
    Display constants
    ------------------------------------------------------------------*/

    #define DISPLAY_WIDTH 128
    #define DISPLAY_WIDTH_BITS 7
    #define DISPLAY_WIDTH_MASK 0x7f
    
    #if (DISPLAY & SSD1306_128X64)
        #define DISPLAY_ROWS 8
        #define DISPLAY_ROW_BITS 3
        #define DISPLAY_ROW_MASK 0x07
    #else // SSD1306_128X32
        #define DISPLAY_ROWS 4
        #define DISPLAY_ROW_BITS 2
        #define DISPLAY_ROW_MASK 0x03
    #endif

    #define DISPLAY_HEIGHT  (DISPLAY_ROWS * 8)
    #define DISPLAY_SIZE    (DISPLAY_WIDTH * DISPLAY_ROWS)
    #define PORTRAIT        100
    #define LANDSCAPE       101

    // only 2 colors available on these type of screen
    #define Blue            0x001F
    #define Yellow          0xFFE0

/**	--------------------------------------------------------------------
    Typedef.
    ------------------------------------------------------------------*/

    typedef struct
    {
        u16 x;
        u16 y;
        u16 page;
    } coord_t;

    typedef struct
    {
        u8 width;
        u8 height;
        const u8 *address;
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
            u8 l8;
            u8 h8;
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

/**	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

#if   (DISPLAY & SSD1306_6800) || (DISPLAY & SSD1306_8080)
    #if   (DISPLAY & SSD1306_PMP)
        void SSD1306_init(u8 rst, u16 dc);
    #else
        void SSD1306_init(u8 rst, u16 dc, u8 d0, u8 d1, u8 d2, u8 d3, u8 d4, u8 d5, u8 d6, u8 d7);
    #endif
#elif (DISPLAY & SSD1306_SPI3) || (DISPLAY & SSD1306_SPI4)
    // TODO
#elif (DISPLAY & SSD1306_I2C)
        void SSD1306_init(u8 addr);
#endif

void SSD1306_sendCommand(u8 _cmd);
void SSD1306_sendData(u8 _data);
void SSD1306_displayOff();
void SSD1306_displayOn();
void SSD1306_sleep();
void SSD1306_wake();
void SSD1306_setInverse(u8 value);
void SSD1306_setDisplayOffset(u8 value);
void SSD1306_setContrast(u8 value);
void SSD1306_setDisplayStartLine(u8 value);
void SSD1306_setSegmentRemap(u8 value);
void SSD1306_setMultiplexRatio(u8 value);
void SSD1306_setComOutputScanDirection(u8 value);
void SSD1306_setComPinsHardwareConfiguration(u8 sequential, u8 lr_remap);
void SSD1306_startHorizontalScroll(u8 direction, u8 start, u8 end, u16 interval); 
void SSD1306_startVerticalAndHorizontalScroll(u8 direction, u8 start, u8 end, u16 interval, u8 vertical_offset);
void SSD1306_stopScroll();
void SSD1306_pamSetStartAddress(u8 address);
void SSD1306_setMemoryAddressingMode(u8 mode);
void SSD1306_hvSetColumnAddress(u8 start, u8 end);
void SSD1306_hvSetPageAddress(u8 start, u8 end);
void SSD1306_pamSetPageStart(u8 address);
void SSD1306_setDisplayClockRatioAndFrequency(u8 ratio, u8 frequency);
void SSD1306_setPrechargePeriod(u8 phase1, u8 phase2);
void SSD1306_setVcomhDeselectLevel(u8 level);
void SSD1306_nop();
void SSD1306_setChargePumpEnable(u8 enable);
void SSD1306_refresh();
void SSD1306_clearScreen(void);
void SSD1306_scrollRight();
void SSD1306_scrollLeft();
void SSD1306_scrollUp();
void SSD1306_scrollDown();
void SSD1306_setFont(const u8 *font);
void SSD1306_printChar(u8 c);
void SSD1306_print(u8 *string);
void SSD1306_println(u8 *string);
void SSD1306_printNumber(long value, u8 base);
void SSD1306_printFloat(float number, u8 digits);
void SSD1306_printf(const u8 *fmt, ...);
void SSD1306_setCursor(u8 x, u8 y);
void SSD1306_drawPixel(u8 x, u8 y);
void SSD1306_clearPixel(u8 x, u8 y);
u8 SSD1306_getColor(u8 x, u8 y);
void SSD1306_drawBitmap(u16 x, u16 y, u16 w, u16 h, u16* bitmap);
void drawPixel(u16 x, u16 y);
void SSD1306_drawCircle(u16 x, u16 y, u16 radius);
void SSD1306_fillCircle(u16 x, u16 y, u16 radius);
void SSD1306_drawLine(u16 x0, u16 y0, u16 x1, u16 y1);

#define SSD1306_getFontWidth()  (SSD1306.font.width)
#define SSD1306_getFontHeight() (SSD1306.font.height)
#define SSD1306_invertDisplay() SSD1306_sendCommand(SSD1306_INVERTDISPLAY)
#define SSD1306_normalDisplay() SSD1306_sendCommand(SSD1306_NORMALDISPLAY)

/**	--------------------------------------------------------------------
    Globals
    ------------------------------------------------------------------*/

lcd_t SSD1306;

//extern u8 SSD1306_buffer[(DISPLAY_WIDTH * DISPLAY_ROWS)];
u8* SSD1306_bufferptr;

/**	--------------------------------------------------------------------
    Pin Assignment
                                ----------PINGUINO 47J53----------
            SSD1306             6800 w/PMP              6800 wo/PMP
    1       VCC (3 to 6V)       VIN                     VIN
    2       GND                 GND                     GND
    3       CS                  GND                     GND
    4       RES                             D3          D3
    5       D/C                 PMA1/B4     D4          D4
    6       R/W                 PMRD/E0     D13         D5
    7       E/RD                GND         GND         D6
    8       D0                  PMD0/D0     D24         D24
    9       D1                  PMD1        D25         D25
    10      D2                  PMD2        D26         D26
    11      D3                  PMD3        D27         D27
    12      D4                  PMD4        D28         D28
    13      D5                  PMD5        D29         D29
    14      D6                  PMD6        D30         D30
    15      D7                  PMD7        D31         D31
    16      NC
     
                                PMBE        D1
    ------------------------------------------------------------------*/

    //#define swap(i, j) {int t = i; i = j; j = t;}

    #if (DISPLAY & SSD1306_8080) || (DISPLAY & SSD1306_6800)

        #if (DISPLAY & SSD1306_PMP)

            u8 RES;
            u16 DC;
        
        #else

            u8 RES;
            u16 CS, DC, RW, E, RD;
            u8 D0, D1, D2, D3, D4, D5, D6, D7;

            #define DATA		LATD    // RB0 to RB7
            #define dDATA		TRISD

            #define CMD			LATB
            #define dCMD		TRISB   // RS, WR, RD, CS and RST

            #define CS          2       // Chip Select
            #define RST         3       // Reset
            #define DC          4       // Command / Data
            #define WR          5       // Write (8080) or Read / Write (6800)
            #define RD          6       // Read (8080)
            #define E           6       // Enable (6800)

        #endif

        #define Low(x)		BitClear(CMD,x)
        #define High(x)		BitSet(CMD,x)
        /*
        #define Low(x)      do { __asm bcf _LATB,x  __endasm; } while(0)
        #define High(x)     do { __asm bsf _LATB,x  __endasm; } while(0)
        #define Output(x)   do { __asm bcf _TRISB,x __endasm; } while(0)
        #define Input(x)    do { __asm bsf _TRISB,x __endasm; } while(0)
        */
        
    #elif (DISPLAY & SSD1306_SPI3) || (DISPLAY & SSD1306_SPI4)
    
        // TODO
    
    #elif (DISPLAY & SSD1306_I2C)

        u8 SSD1306_i2c_address;

    #endif

#endif /* __SSD1306_H */
