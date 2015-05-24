/*****************************************************************
    ███████╗ █████╗ ███████╗██╗  ██╗██╗  ██╗██╗      ██████╗██████╗  *
    ██╔════╝██╔══██╗██╔════╝██║  ██║██║  ██║██║     ██╔════╝██╔══██╗ *
    █████╗  ╚█████╔╝███████╗███████║███████║██║     ██║     ██║  ██║ *
    ██╔══╝  ██╔══██╗╚════██║╚════██║╚════██║██║     ██║     ██║  ██║ *
    ██║     ╚█████╔╝███████║     ██║     ██║███████╗╚██████╗██████╔╝ *
    ╚═╝      ╚════╝ ╚══════╝     ╚═╝     ╚═╝╚══════╝ ╚═════╝╚═════   *
-------------------------------------------------------Alpha version *
 ******************************************************************
 PCD8544.c :
 * C library for Monochrome Nokia LCD
 * with PCD8544 controler
 * for pinguino boards

    ->File        : PCD8544.c
    ->Revision    : 0.01 Alpha
    ->Last update : March 2014
    ->Description : f8544lcd core code.
    ->Author      : Thomas Missonier (sourcezax@users.sourceforge.net). 

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions, the others licences below, and the following disclaimer.
- Redistributions in binary form must reproduce the above notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

---------------------------------------------------------------------------
*********************************************************************/

#ifndef __PCD8544H
#define __PCD8544H

#include <typedef.h>            // Pinguino's type : u8, u8, ..., and bool
#include <macro.h>              // BitSet, BitClear

/**	--------------------------------------------------------------------
    Display interfaces
    ------------------------------------------------------------------*/

#define PCD8544_SPIHW           1<<0
#define PCD8544_SPISW           1<<1
#define PCD8544_PORTB           1<<2
#ifndef PCD8544_INTERFACE
    #define PCD8544_INTERFACE PCD8544_SPISW
#endif

#if (PCD8544_INTERFACE & PCD8544_PORTB)
    #define PCD8544_RST         0  // LCD RST 
    #define PCD8544_SCE         1  // LCD CS/CE  
    #define PCD8544_DC          2  // LCD Dat/Com
    #define PCD8544_SDIN        3  // LCD SPIDat/DIN/NOKIA_SDIN
    #define PCD8544_SCLK        4  // LCD SPIClk/CLK 
    #define PCD8544_VCC         5  // LCD NOKIA_VCC 3.3V 
    #define PCD8544_LIGHT       6  // LCD BACKNOKIA_LIGHT : GROUND or NOKIA_VCC 3.3V depends on models                                      
    #define PCD8544_GND         7  // LCD GROUND 
#endif

/**	--------------------------------------------------------------------
    Display sizes
    ------------------------------------------------------------------*/

#define DISPLAY_WIDTH  84
#define DISPLAY_HEIGHT 48
#define DISPLAY_SIZE   (DISPLAY_WIDTH * DISPLAY_HEIGHT / 8)
#define DISPLAY_ROWS   ((DISPLAY_HEIGHT / 8) - 1)

/**	--------------------------------------------------------------------
    Display commands
    ------------------------------------------------------------------*/

#define PCD8544_POWERDOWN 0x04
#define PCD8544_ENTRYMODE 0x02
#define PCD8544_EXTENDEDINSTRUCTION 0x01

#define PCD8544_DISPLAYBLANK 0x0
#define PCD8544_DISPLAYNORMAL 0x4
#define PCD8544_DISPLAYALLON 0x1
#define PCD8544_DISPLAYINVERTED 0x5

// H = 0
#define PCD8544_FUNCTIONSET 0x20
#define PCD8544_DISPLAYCONTROL 0x08
#define PCD8544_SETYADDR 0x40
#define PCD8544_SETXADDR 0x80

// H = 1
#define PCD8544_SETTEMP 0x04
#define PCD8544_SETBIAS 0x10
#define PCD8544_SETVOP 0x80

//Colors
#define BLACK 1
#define WHITE 0

//Orientations
#define PORTRAIT 1
#define LANDSCAPE 0

#ifdef _PCD8544_USE_ROUND_RECT
    #define _PCD8544_USE_CIRCLE
    #define _PCD8544_USE_RECT
#endif

/**	--------------------------------------------------------------------
    Typedef.
    ------------------------------------------------------------------*/

/*
    typedef struct
    {
        u8 _din, _sclk, _dc, _rst, _cs;
        volatile u8 *mosiport, *clkport, *csport, *dcport;
        u8 mosipinmask, clkpinmask, cspinmask, dcpinmask;

        const u8   WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
        u8  _width, _height, // Display w/h as modified by current rotation
        cursor_x, cursor_y;
        u8 textcolor, textbgcolor;
        u8  textsize,  rotation;
        boolean  wrap;
      // the memory buffer for the LCD
    } PCD8544_t;
*/

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
        u8 _din, _sclk, _dc, _rst, _cs;
        u8 orientation;
        u8  textsize;
        rect_t screen;
        color_t bcolor;
        color_t color;
        coord_t cursor;
        font_t font;
    } lcd_t;

/**	--------------------------------------------------------------------
    Globals
    ------------------------------------------------------------------*/

lcd_t PCD8544;
u8 * PCD8544_buffer = logo;  // screen buffer points on logo[]

/**	--------------------------------------------------------------------
    Prototypes
    ------------------------------------------------------------------*/

#if (PCD8544_INTERFACE & PCD8544_SPISW)
    #if (PCD8544_INTERFACE & PCD8544_PORTB)
    void PCD8544_init();
    #else
    void PCD8544_init(u8 SCLK, u8 DIN, u8 DC, u8 CS, u8 RST);
    #endif
#else // PCD8544_SPIHW
    void PCD8544_init(u8 DC, u8 RST);
#endif

void PCD8544_command(u8 c);
void PCD8544_data(u8 c);
void PCD8544_setContrast(u8 val);
void PCD8544_clearScreen();
void PCD8544_refresh();
void PCD8544_drawPixel(u8 x, u8 y);//, u8 color);
void PCD8544_clearPixel(u8 x, u8 y);//, u8 color);
u8 PCD8544_getPixel(u8 x, u8 y);

#ifdef _PCD8544_USE_GRAPHICS
//LINE
void PCD8544_drawLine(u8 x0, u8 y0, u8 x1, u8 y1);//, u8 color);
void PCD8544_drawFastVLine(u8 x, u8 y, u8 h, u8 color);
void PCD8544_drawFastHLine(u8 x, u8 y, u8 w, u8 color);
//TRIANGLE
void PCD8544_drawTriangle(u8 x0, u8 y0, u8 x1, u8 y1,u8 x2, u8 y2, u8 color);
void PCD8544_fillTriangle(u8 x0, u8 y0, u8 x1, u8 y1,u8 x2, u8 y2, u8 color);
//RECT
void PCD8544_drawRect(u8 x, u8 y, u8 w, u8 h, u8 color);
void PCD8544_fillRect(u8 x, u8 y, u8 w, u8 h, u8 color);
#define PCD8544_fillScreen(color) PCD8544_fillRect(0, 0,PCD8544.screen.width,PCD8544..screen.height, (color))
//ROUND_RECT
void PCD8544_drawRoundRect(u8 x0, u8 y0, u8 w, u8 h,u8 radius, u8 color);
void PCD8544_fillRoundRect(u8 x, u8 y, u8 w,u8 h, u8 r, u8 color);
//CIRCLE
void PCD8544_drawCircle(u8 x0, u8 y0, u8 r0);//, u8 color);
void PCD8544_drawCircleHelper(u8 x0, u8 y0, u8 r, u8 cornername,u8 color);
void PCD8544_fillCircle(u8 x0, u8 y0, u8 r);//, u8 color);
void PCD8544_fillCircleHelper(u8 x0, u8 y0, u8 r, u8 cornername,u8 delta, u8 color);
//BITMAP
void PCD8544_drawBitmap(u8 x, u8 y, const u8 *bitmap,u8 w, u8 h, u8 color);
#endif

void PCD8544_setCursor(u8 x, u8 y);
#ifdef  _PCD8544_USE_TEXT
void PCD8544_setTextColor(u8 c);
void PCD8544_setTextColor2(u8 c, u8 bg);
void PCD8544_setTextSize(u8 s);
#endif

#ifdef _PCD8544_USE_ORIENTATION
void PCD8544_setOrientation(u8 r);
#define PCD8544_getOrientation() (PCD8544.orientation)
#endif

#ifdef  _PCD8544_USE_INVERT
void PCD8544_invertDisplay(bool i);
#endif

#ifdef enablePartialUpdate
static void PCD8544_updateBoundingBox(u8 xmin, u8 ymin, u8 xmax, u8 ymax);
#endif

#if !(PCD8544_INTERFACE & PCD8544_SPIHW)
//Local Shifout implementation
void PCD8544_shiftOut(u8 c);
#endif

//Print functions
//#ifdef _PCD8544_USE_TEXT
void PCD8544_printChar(u8 c);
void PCD8544_print(u8 *string);
void PCD8544_println(u8 *string);
void PCD8544_printf(const u8 *fmt, ...);
void PCD8544_printNumber(long value, u8 base);
void PCD8544_printFloat(float number, u8 digits);
//#endif

#define PCD8544_getFontWidth()  (PCD8544.font.width)
#define PCD8544_getFontHeight() (PCD8544.font.height)

#ifdef _PCD8544_USE_TEXT
#define PCD8544_setTextWrap(w) PCD8544.wrap = (w)
#endif 

#if (PCD8544_INTERFACE & PCD8544_SPISW)
    #if (PCD8544_INTERFACE & PCD8544_PORTB)
        //#define Low(x)      BitSet(LATB, x)
        //#define High(x)     BitClear(LATB, x)
        #define Low(x)      do { __asm bcf _LATB,x  __endasm; } while(0)
        #define High(x)     do { __asm bsf _LATB,x  __endasm; } while(0)
        //#define Output(x)   do { __asm bcf _TRISB,x __endasm; } while(0)
        //#define Input(x)    do { __asm bsf _TRISB,x __endasm; } while(0)
    #endif
#endif

#endif // __PCD8544H
