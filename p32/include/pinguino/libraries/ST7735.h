/*	--------------------------------------------------------------------
	FILE:			ST7735.c
	PROJECT:		pinguino
	PURPOSE:		Drive 0.96" 128x64 Oled display (ST7735 controller)
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

#ifndef __ST7735_H
#define __ST7735_H

#include <typedef.h>

#define __ST7735__

/**	--------------------------------------------------------------------
    Display size (default orientation is landscape)
    ------------------------------------------------------------------*/

#define ST7735_WIDTH        160
#define ST7735_HEIGHT       128
#define ST7735_SIZE         (ST7735_WIDTH * ST7735_HEIGHT)
#define ST7735_TABSIZE      4
/**	--------------------------------------------------------------------
    Display commands
    ------------------------------------------------------------------*/

#define ST7735_NOP          0x00    // No Operation
#define ST7735_SWRESET      0x01    // Software Reset
/*
#define ST7735_RDDID        0x04    // Read Display ID
#define ST7735_RDDST        0x09    // Read Display Status
#define ST7735_RDDPM        0x0A    // Read Display Power Mode
#define ST7735_RDDMADCTL    0x0B    // Read Display MADCTL
#define ST7735_RDDCOLMOD    0x0C    // Read Display Pixel Format
#define ST7735_RDDIM        0x0D    // Read Display Image Mode
#define ST7735_RDDSM        0x0E    // Read Display Signal Mode
*/
#define ST7735_SLPIN        0x10    // Sleep In
#define ST7735_SLPOUT       0x11    // Sleep Out 
#define ST7735_PTLON        0x12    // Partial Display Mode On
#define ST7735_NORON        0x13    // Normal Display Mode On
#define ST7735_INVOFF       0x20    // Display Inversion Off
#define ST7735_INVON        0x21    // Display Inversion On
#define ST7735_GAMSET       0x26    // Gamma Set
#define ST7735_DISPOFF      0x28    // Display Off
#define ST7735_DISPON       0x29    // Display On
#define ST7735_CASET        0x2A    // Column Address Set
#define ST7735_RASET        0x2B    // Row Address Set
#define ST7735_RAMWR        0x2C    // Memory Write
#define ST7735_RAMRD        0x2E    // Memory Read
#define ST7735_PTLAR        0x30    // Partial Area
#define ST7735_TEOFF        0x34    // Tearing Effect Line Off
#define ST7735_TEON         0x35    // Tearing Effect Line On
#define ST7735_MADCTL       0x36    // Memory Data Access Control
#define ST7735_MADCTL_MY    0x80
#define ST7735_MADCTL_MX    0x40
#define ST7735_MADCTL_MV    0x20
#define ST7735_MADCTL_ML    0x10
#define ST7735_MADCTL_RGB   0x00
#define ST7735_MADCTL_BGR   0x08
#define ST7735_MADCTL_MH    0x04
#define ST7735_IDMOFF       0X38    // Idle Mode Off
#define ST7735_IDMOn        0X39    // Idle Mode On
#define ST7735_COLMOD       0x3A    // Interface Pixel Format
#define ST7735_FRMCTR1      0xB1    // Frame Rate Control (In normal mode/ Full colors)
#define ST7735_FRMCTR2      0xB2    // Frame Rate Control (In Idle mode/ 8-colors) 
#define ST7735_FRMCTR3      0xB3    // Frame Rate Control (In Partial mode/ full colors)
#define ST7735_INVCTR       0xB4    // Display Inversion Control
#define ST7735_DISSET5      0xB6    // Display Function set 5
#define ST7735_PWCTR1       0xC0    // Power Control 1
#define ST7735_PWCTR2       0xC1    // Power Control 2
#define ST7735_PWCTR3       0xC2    // Power Control 3
#define ST7735_PWCTR4       0xC3    // Power Control 4
#define ST7735_PWCTR5       0xC4    // Power Control 5
#define ST7735_VMCTR1       0xC5
#define ST7735_VMOFCTR      0xC7
#define ST7735_WRID2        0xD1
#define ST7735_WRID3        0xD2
#define ST7735_PWCTR6       0xFC
#define ST7735_NVCTR1       0xD9
#define ST7735_NVCTR2       0xDE
#define ST7735_NVCTR3       0xDF
/*
#define ST7735_RDID1        0xDA    // Read ID1 Value
#define ST7735_RDID2        0xDB    // Read ID2 Value
#define ST7735_RDID3        0xDC    // Read ID3 Value
#define ST7735_RDID4        0xDD    // Read ID4 Value
*/
#define ST7735_GAMCTRP1     0xE0
#define ST7735_GAMCTRN1     0xE1
#define ST7735_EXTCTRL      0xF0
#define ST7735_VCOM4L       0xFF

/**	--------------------------------------------------------------------
    Color definitions
    ------------------------------------------------------------------*/

#define	ST7735_BLACK        0x0000
#define	ST7735_BLUE         0x001F
#define	ST7735_RED          0xF800
#define ST7735_GREEN        0x0400
#define ST7735_LIME         0x07E0
#define ST7735_CYAN         0x07FF
#define ST7735_MAGENTA      0xF81F
#define ST7735_YELLOW       0xFFE0
#define ST7735_WHITE        0xFFFF
#define ST7735_PURPLE       0x8010
#define ST7735_NAVY         0x0010
#define ST7735_TEAL         0x0410
#define ST7735_OLIVE        0x8400
#define ST7735_MAROON       0x8000
#define ST7735_SILVER       0xC618
#define ST7735_GRAY         0x8410

/**	--------------------------------------------------------------------
    Typedef.
    ------------------------------------------------------------------*/

    typedef struct
    {
        u16 val;
        u8 v[2];
        struct
        {
            u8 LB;
            u8 HB;
        } byte;
    } byte_t;
    
    typedef struct
    {
        u8 dc;
        u8 cs;
        u8 sda;
        u8 sck;
    } pin_t;

    typedef struct
    {
        u16 x;
        u16 y;
        u16 page;
        u16 xmax;
        u16 ymax;
    } coord_t;

    typedef struct
    {
        u8 width;
        u8 height;
        const u8 *address;
    } font_t;

    typedef struct
    {
        u8  r;			// 8/8/8 representation
        u8  g;
        u8  b;
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
        pin_t pin;
        rect_t screen;
        color_t bcolor;
        color_t color;
        coord_t cursor;
        font_t font;
    } lcd_t;

/**	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

void ST7735_sendCommand(u8, u8);
void ST7735_sendData(u8, u8);
void ST7735_init(u8, u8, u8, u8, u8);
void ST7735_setOrientation(u8, s16);
void ST7735_setWindow(u8, u8, u8, u8, u8);
void ST7735_setColor(u8, u16);
void ST7735_setBackgroundColor(u8, u16);
void ST7735_clearScreen(u8);
void ST7735_clearWindow(u8, u8, u8, u8, u8);
//void ST7735_scrollRight();
//void ST7735_scrollLeft();
//void ST7735_scrollUp();
//void ST7735_scrollDown();
void ST7735_setFont(u8, const u8*);
void ST7735_printChar(u8, u8);
void ST7735_print(u8, u8*);
void ST7735_println(u8, u8*);
void ST7735_printNumber(u8, long, u8);
void ST7735_printFloat(u8, float, u8);
void ST7735_printf(u8, const u8*, ...);
void ST7735_setCursor(u8, u8, u8);
void ST7735_drawPixel(u8, u8, u8);
void ST7735_clearPixel(u8, u8, u8);
void ST7735_drawBitmap(u8, u16, u16, u16, u16, u16*);
void ST7735_drawCircle(u8, u16, u16, u16);
void ST7735_fillCircle(u8, u16, u16, u16);
void ST7735_drawLine(u8, u16, u16, u16, u16);
void ST7735_drawVLine(u8, u16, u16, u16);
void ST7735_drawHLine(u8, u16, u16, u16);

void drawPixel(u16, u16);
void drawVLine(u16, u16, u16);
void drawHLine(u16, u16, u16);

color_t ST7735_getColor(u8, u8, u8);

/**	--------------------------------------------------------------------
    Macros
    ------------------------------------------------------------------*/

#define ST7735_getFontWidth(m)   (ST7735[m].font.width)
#define ST7735_getFontHeight(m)  (ST7735[m].font.height)
#define ST7735_invertDisplay(m)  ST7735_sendCommand(m, ST7735_INVON)
#define ST7735_normalDisplay(m)  ST7735_sendCommand(m, ST7735_INVOFF)
#define ST7735_displayOn(m)      ST7735_sendCommand(m, ST7735_DISPON)
#define ST7735_displayOff(m)     ST7735_sendCommand(m, ST7735_DISPOFF)
#define ST7735_low(x)            low(x)
#define ST7735_high(x)           high(x)

/**	--------------------------------------------------------------------
    Globals
    ------------------------------------------------------------------*/

u8 ST7735_SPI;

// SPISW, SPI1, SPI2, SPI3 and SPI4
#if defined(__32MX795F512L__) || \
    defined(__32MX795F512H__)
    
lcd_t ST7735[5];

// SPISW, SPI1 and SPI2
#else

lcd_t ST7735[3];

#endif

#endif /* __ST7735_H */
