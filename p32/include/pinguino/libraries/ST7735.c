/*	--------------------------------------------------------------------
FILE:			ST7735[module].c
PROJECT:		pinguino
PURPOSE:		Drive 1.8" 128x160 TFT display
PROGRAMER:		regis blanchot <rblanchot@gmail.com>
FIRST RELEASE:	11 Dec. 2014
LAST RELEASE:	12 Dec. 2014
------------------------------------------------------------------------
http://w8bh.net/pi/TFT1.pdf to TFT5.pdf
------------------------------------------------------------------------
* Todo : scroll functions
------------------------------------------------------------------------
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
----------------------------------------------------------------------*/

/**	--------------------------------------------------------------------
    Pin Assignment

            ST7735             PIC32MX     32MX250
    1       LED                 A1
    2       ST7735_SCKPIN
    3       ST7735_SDAPIN
    4       A0
    5       RESET               NC          NC
    6       ST7735[module].pin.cs
    7       GND                 GND         GND
    8       VCC                 VCC         VCC
    ------------------------------------------------------------------*/

#ifndef __ST7735_C
#define __ST7735_C

#include <typedef.h>
#include <macro.h>
#include <spi.h>
#include <ST7735.h>
#include <digitalw.c>
#include <delay.c>
#include <spi.c>

// Printf
#ifdef ST7735PRINTF
    #include <stdarg.h>
    #ifdef __PIC32MX__
        #include <printf.c>
    #else
        #include <stdio.c>
    #endif
#endif

// Graphics Library
#ifdef ST7735GRAPHICS
    #include <graphics.c>
#endif

///	--------------------------------------------------------------------
/// Core functions
///	--------------------------------------------------------------------

void ST7735_sendCommand(u8 module, u8 val)
{
    ST7735_low(ST7735[module].pin.dc);            // COMMAND = 0
    SPI_write(module, val);
}

void ST7735_sendData(u8 module, u8 val)
{
    ST7735_high(ST7735[module].pin.dc);           // DATA = 1
    SPI_write(module, val);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Initialize the graphical display
    PARAMETERS:
    RETURNS:
    REMARKS:
        Orientation set to portrait by default
------------------------------------------------------------------*/

void ST7735_init(u8 module, u8 cs, u8 dc, u8 sda, u8 sck)
{
    ST7735_SPI = module;
    
    ST7735[module].pin.dc = dc;
    ST7735[module].pin.cs = cs;
    output(ST7735[module].pin.dc);
    output(ST7735[module].pin.cs);
    
    // init SPI communication

    if (module == SPISW)
    {
        SPI_setPin(ST7735_SPI, sda, sck);
        SPI_setBitOrder(ST7735_SPI, SPI_MSBFIRST);
    }
    else
    {
        SPI_setMode(ST7735_SPI, SPI_MASTER8);
        SPI_setDataMode(ST7735_SPI, SPI_MODE1);
        //maximum baud rate possible = FPB/2
        SPI_setClockDivider(ST7735_SPI, SPI_PBCLOCK_DIV2);
        SPI_begin(ST7735_SPI);
    }
    
    // default Screen Values

    ST7735[module].cursor.x      = 0;
    ST7735[module].cursor.y      = 0;
    ST7735[module].cursor.page   = 0;
    ST7735[module].screen.startx = 0;
    ST7735[module].screen.starty = 0;
    ST7735[module].screen.endx   = ST7735_WIDTH  - 1;
    ST7735[module].screen.endy   = ST7735_HEIGHT - 1;
    ST7735[module].screen.width  = ST7735_WIDTH;
    ST7735[module].screen.height = ST7735_HEIGHT;
    //ST7735[module].font.width    = 1;
    //ST7735[module].font.height   = 1;

    // Software reset and minimal init.
    
    ST7735_low(ST7735[module].pin.cs);             // Chip select
    
    ST7735_sendCommand(module,(u8)ST7735_SWRESET); // software reset, puts display into sleep
    Delayms(150);
    ST7735_sendCommand(module,(u8)ST7735_SLPOUT);  // wake from sleep
    Delayms(500);
    ST7735_sendCommand(module,(u8)ST7735_COLMOD);  // set color mode to 16-bit
    ST7735_sendData(module,0x05);
    Delayms(10);
    ST7735_sendCommand(module,(u8)ST7735_MADCTL);  // RGB
    ST7735_sendData(module, 0x60|ST7735_MADCTL_RGB);
    Delayms(10);
    ST7735_sendCommand(module,(u8)ST7735_DISPON);  // display on!
    Delayms(100);
    ST7735_sendCommand(module,(u8)ST7735_NORON);   // normal display
    Delayms(10);
    
    ST7735_high(ST7735[module].pin.cs);            // Chip deselected
    
    // Default colors and orientation
    
    //ST7735_setOrientation(module, 90);             // landscape orientation
    ST7735_setBackgroundColor(module, ST7735_BLACK);
    ST7735_setColor(module, ST7735_WHITE);
    ST7735_clearScreen(module);
}

///	--------------------------------------------------------------------
/// Set the display orientation to 0, 90, 180, or 270 degrees
/// 90
///	--------------------------------------------------------------------

void ST7735_setOrientation(u8 module, s16 degrees)
{
    u8 arg;
    
    if (degrees < 0)
        degrees = 360 + degrees;
        
    ST7735[module].orientation = (u16)degrees;

    switch (degrees)
    {
        case  90:
            ST7735[module].screen.endx   = ST7735_HEIGHT - 1;
            ST7735[module].screen.endy   = ST7735_WIDTH  - 1;
            ST7735[module].screen.width  = ST7735_HEIGHT;
            ST7735[module].screen.height = ST7735_WIDTH;
            arg = ST7735_MADCTL_MX | ST7735_MADCTL_MV | ST7735_MADCTL_RGB; // 0x60;
            break;
        case 180:
            ST7735[module].screen.endx   = ST7735_WIDTH - 1;
            ST7735[module].screen.endy   = ST7735_HEIGHT  - 1;
            ST7735[module].screen.width  = ST7735_WIDTH;
            ST7735[module].screen.height = ST7735_HEIGHT;
            arg = ST7735_MADCTL_MX | ST7735_MADCTL_MY | ST7735_MADCTL_RGB; // 0xC0;
            break;
        case 270:
            ST7735[module].screen.endx   = ST7735_HEIGHT - 1;
            ST7735[module].screen.endy   = ST7735_WIDTH  - 1;
            ST7735[module].screen.width  = ST7735_HEIGHT;
            ST7735[module].screen.height = ST7735_WIDTH;
            arg = ST7735_MADCTL_MY | ST7735_MADCTL_MV | ST7735_MADCTL_RGB; // 0xA0;
            break;
        default:
            ST7735[module].screen.endx   = ST7735_WIDTH  - 1;
            ST7735[module].screen.endy   = ST7735_HEIGHT - 1;
            ST7735[module].screen.width  = ST7735_WIDTH;
            ST7735[module].screen.height = ST7735_HEIGHT;
            arg = ST7735_MADCTL_RGB; // 0x00;
            break;
    }
    
    ST7735_low(ST7735[module].pin.cs);                     // Chip select

    ST7735_sendCommand(module,ST7735_MADCTL);
    ST7735_sendData(module,arg);

    ST7735_high(ST7735[module].pin.cs);                    // Chip deselected
    
    ST7735[module].cursor.xmax  = ST7735[module].screen.width / ST7735[module].font.width;
    ST7735[module].cursor.ymax  = ST7735[module].screen.height / ST7735[module].font.height;
}

///	--------------------------------------------------------------------
/// Sets a rectangular display window into which pixel data is placed
///	--------------------------------------------------------------------

void ST7735_setWindow(u8 module, u8 x0, u8 y0, u8 x1, u8 y1)
{
    ST7735_low(ST7735[module].pin.cs);       // Chip select
    
    ST7735_low(ST7735[module].pin.dc);       // COMMAND = 0
    SPI_write(module, ST7735_CASET);         // set column range (x0,x1)

    ST7735_high(ST7735[module].pin.dc);      // DATA = 1
    SPI_write(module,0);
    SPI_write(module,x0);
    SPI_write(module,0);
    SPI_write(module,x1);

    ST7735_low(ST7735[module].pin.dc);       // COMMAND = 0
    SPI_write(module,ST7735_RASET);          // set row range (y0,y1)

    ST7735_high(ST7735[module].pin.dc);      // DATA = 1
    SPI_write(module,0);
    SPI_write(module,y0);
    SPI_write(module,0);
    SPI_write(module,y1);

    ST7735_high(ST7735[module].pin.cs);      // Chip deselected
}

///	--------------------------------------------------------------------
/// Sets current color
///	--------------------------------------------------------------------

void ST7735_setColor(u8 module, u16 c)
{
    ST7735[module].color.c = c;
}

///	--------------------------------------------------------------------
/// Sets background color
///	--------------------------------------------------------------------

void ST7735_setBackgroundColor(u8 module, u16 c)
{
    ST7735[module].bcolor.c = c;
}

///	--------------------------------------------------------------------
/// Gets pixel color
///	--------------------------------------------------------------------

color_t ST7735_getColor(u8 module, u8 x, u8 y)
{
    color_t color;
    u8 ch, cl;
    
    if ( x >= ST7735_WIDTH || y >= (ST7735_HEIGHT) ) return;

    ST7735_low(ST7735[module].pin.cs);             // Chip select
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_CASET);                // set column range (x0,x1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,x);
    SPI_write(module,0x00);
    SPI_write(module,x);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RASET);                // set row range (y0,y1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,y);
    SPI_write(module,0x00);
    SPI_write(module,y);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RAMRD);                // Read RAM
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    ch = SPI_read(module);
    cl = SPI_read(module);
    color.c = make16(ch, cl);
    
    ST7735_high(ST7735[module].pin.cs);            // Chip deselected
    
    return color;
}

///	--------------------------------------------------------------------
/// Packs individual red,green,blue color components into 16bit color
/// 16-bit color format is rrrrrggg.gggbbbbb
/// Max values: Red 31, Green 63, Blue 31
///	--------------------------------------------------------------------

color_t ST7735_packColor(u8 red, u8 green, u8 blue)
{
    u8 r, g, b;
    color_t color;
    
    color.r = red & 0x1F;
    color.g = green & 0x3F;
    color.b = blue & 0x1F;
    color.c = (r<<11)+(g<<5)+b;
    return color;
}

///	--------------------------------------------------------------------
/// Reduces 16-bit color into component r,g,b values
///	--------------------------------------------------------------------

void ST7735_unpackColor(color_t color)
{
    color.r = color.c>>11;
    color.g = (color.c & 0x07E0)>>5;
    color.b = color.c & 0x001F;
}

///	--------------------------------------------------------------------
/// Clear the display
///	--------------------------------------------------------------------

void ST7735_clearScreen(u8 module)
{
    u16 i;
    u8 ch = ST7735[module].bcolor.c >> 8;
    u8 cl = ST7735[module].bcolor.c & 0xFF;
    
    ST7735_low(ST7735[module].pin.cs);             // Chip select

    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_CASET);                // set column range (x0,x1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,0x00);
    SPI_write(module,0x00);
    SPI_write(module,ST7735[module].screen.endx);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RASET);                // set row range (y0,y1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,0x00);
    SPI_write(module,0x00);
    SPI_write(module,ST7735[module].screen.endy);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RAMWR);                // Write to RAM
        
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    for (i = 0; i < ST7735_SIZE; i++)
    {
        SPI_write(module,ch);
        SPI_write(module,cl);
    }

    ST7735_high(ST7735[module].pin.cs);            // Chip deselected

    ST7735[module].cursor.x    = 0;
    ST7735[module].cursor.y    = 0;
}

///	--------------------------------------------------------------------
/// Clear the window
///	--------------------------------------------------------------------

void ST7735_clearWindow(u8 module, u8 x0, u8 y0, u8 x1, u8 y1)
{
    u8 x, y;
    u8 ch = ST7735[module].bcolor.c >> 8;
    u8 cl = ST7735[module].bcolor.c & 0xFF;
    
    ST7735_low(ST7735[module].pin.cs);             // Chip select

    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_CASET);                // set column range (x0,x1)

    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,x0);
    SPI_write(module,0x00);
    SPI_write(module,x1);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RASET);                // set row range (y0,y1)

    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,y0);
    SPI_write(module,0x00);
    SPI_write(module,y1);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RAMWR);                // Write to RAM
        
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    for (x = x0; x < x1; x++)
    {
        for (y = y0; y < y1; y++)
        {
            SPI_write(module,ch);
            SPI_write(module,cl);
        }
    }

    ST7735_high(ST7735[module].pin.cs);            // Chip deselected

    ST7735[module].cursor.x = 0;
    ST7735[module].cursor.y = 0;
}

///	--------------------------------------------------------------------
/// Print functions
///	--------------------------------------------------------------------

void ST7735_setFont(u8 module, const u8 *font)
{
    ST7735[module].font.address = font;
    ST7735[module].font.width   = font[0];
    ST7735[module].font.height  = font[1];
    ST7735[module].cursor.xmax  = ST7735[module].screen.width / ST7735[module].font.width;
    ST7735[module].cursor.ymax  = ST7735[module].screen.height / ST7735[module].font.height;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        write a char at (ST7735[module].cursor.x, ST7735[module].cursor.y)
    PARAMETERS:
        * c ascii code of the character to print
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

void ST7735_printChar(u8 module, u8 c)
{
    u8 height, width;
    u8 b, tx, ty;

    while (ST7735[module].cursor.x >= ST7735[module].cursor.xmax)
    {
        ST7735[module].cursor.x = 0;
        ST7735[module].cursor.y++;            
    }

    while (ST7735[module].cursor.y > ST7735[module].cursor.ymax)
    {
        ST7735[module].cursor.y = 0;
        //ST7735_scrollUp();                      --->>> TODO
        ST7735_clearScreen(module);
    }

    switch (c)
    {
        case '\n':
            ST7735[module].cursor.y++;
            break;
            
        case '\r':
            ST7735[module].cursor.x = 0;
            break;
            
        case '\t':
            ST7735[module].cursor.x = (ST7735[module].cursor.x + ST7735_TABSIZE) % ST7735_TABSIZE;
            break;
            
        default:
            tx = ST7735[module].cursor.x * ST7735[module].font.width;
            ty = ST7735[module].cursor.y * ST7735[module].font.height;
            for (height = 0; height < ST7735[module].font.height; height++)
            {
                b = ST7735[module].font.address[2 + (c - 32) * ST7735[module].font.width + height];
                for (width = 0; width < ST7735[module].font.width; width++)
                {
                    if (b & 1)
                        ST7735_drawPixel(module, tx + height, ty + width);
                    else
                        ST7735_clearPixel(module, tx + height, ty + width);
                    b >>= 1;
                }
            }
            ST7735[module].cursor.x++;
    }            
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        write a formated string at curent cursor position
    PARAMETERS:
        *fmt pointer on a formated string
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

#if defined(ST7735PRINT)       || defined(ST7735PRINTLN)    || \
    defined(ST7735PRINTNUMBER) || defined(ST7735PRINTFLOAT)
void ST7735_print(u8 module, u8 *string)
{
    while (*string != 0)
        ST7735_printChar(module, *string++);
}
#endif

#if defined(ST7735PRINTLN)
void ST7735_println(u8 module, u8 *string)
{
    ST7735_print(module, string);
    ST7735_print(module, "\n\r");
}
#endif

#if defined(ST7735PRINTNUMBER) || defined(ST7735PRINTFLOAT)
void ST7735_printNumber(u8 module, long value, u8 base)
{  
    u8 sign;
    u8 length;

    long i;
    unsigned long v;            // absolute value

    u8 tmp[12];
    u8 *tp = tmp;               // pointer on tmp

    u8 string[12];
    u8 *sp = string;            // pointer on string

    if (value==0)
    {
        ST7735_printChar(module, '0');
        return;
    }
    
    sign = ( (base == 10) && (value < 0) );

    if (sign)
        v = -value;
    else
        v = (unsigned long)value;

    //while (v || tp == tmp)
    while (v)
    {
        i = v % base;
        v = v / base;
        
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'A' - 10;
    }

    // start of string
    if (sign)
        *sp++ = '-';

    length = sign + tp - tmp + 1;

    // backwards writing 
    while (tp > tmp)
        *sp++ = *--tp;

    // end of string
    *sp = 0;

    ST7735_print(module, string);
}
#endif

#if defined(ST7735PRINTFLOAT)
void ST7735_printFloat(u8 module, float number, u8 digits)
{ 
	u8 i, toPrint;
	u16 int_part;
	float rounding, remainder;

	// Handle negative numbers
	if (number < 0.0)
	{
		ST7735_printChar(module, '-');
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"  
	rounding = 0.5;
	for (i=0; i<digits; ++i)
		rounding /= 10.0;

	number += rounding;

	// Extract the integer part of the number and print it  
	int_part = (u16)number;
	remainder = number - (float)int_part;
	ST7735_printNumber(module, int_part, 10);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		ST7735_printChar(module, '.'); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
		ST7735_printNumber(module, toPrint, 10);
		remainder -= toPrint; 
	}
}
#endif

#if defined(ST7735PRINTF)
void ST7735_printf(u8 module, const u8 *fmt, ...)
{
    static u8 buffer[25];
    u8 *c=(char*)&buffer;
    va_list	args;

    va_start(args, fmt);
    psprintf2(buffer, fmt, args);
    va_end(args);

    while (*c != 0)
        ST7735_printChar(module, *c++);
}
#endif

///	--------------------------------------------------------------------
/// Graphic functions
///	--------------------------------------------------------------------

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Sets the cursor to the specified x,y position
    PARAMETERS:
        x,y coord.
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

void ST7735_setCursor(u8 module, u8 x, u8 y)
{
    if ( x >= ST7735_WIDTH || y >= (ST7735_HEIGHT) ) return;

    ST7735[module].cursor.x = x;
    ST7735[module].cursor.y = y;
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Draws a pixel with current color.
    PARAMETERS:
        x,y coord.
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

void ST7735_drawPixel(u8 module, u8 x, u8 y)
{
    if ( x >= ST7735_WIDTH || y >= (ST7735_HEIGHT) ) return;

    ST7735_low(ST7735[module].pin.cs);           // Chip select
    
    ST7735_low(ST7735[module].pin.dc);           // COMMAND = 0
    SPI_write(module,ST7735_CASET);              // set column range (x0,x1)
    
    ST7735_high(ST7735[module].pin.dc);          // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,x);
    //SPI_write(module,0x00);
    //SPI_write(module,x);
    
    ST7735_low(ST7735[module].pin.dc);           // COMMAND = 0
    SPI_write(module,ST7735_RASET);              // set row range (y0,y1)
    
    ST7735_high(ST7735[module].pin.dc);          // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,y);
    //SPI_write(module,0x00);
    //SPI_write(module,y);
    
    ST7735_low(ST7735[module].pin.dc);           // COMMAND = 0
    SPI_write(module,ST7735_RAMWR);
    
    ST7735_high(ST7735[module].pin.dc);          // DATA = 1
    SPI_write(module,ST7735[module].color.c >> 8);
    SPI_write(module,ST7735[module].color.c & 0xFF);
    
    ST7735_high(ST7735[module].pin.cs);          // Chip deselected
}

void ST7735_clearPixel(u8 module, u8 x, u8 y)
{
    if (x >= ST7735_WIDTH || y >= (ST7735_HEIGHT) ) return;
    
    ST7735_low(ST7735[module].pin.cs);           // Chip select
    
    ST7735_low(ST7735[module].pin.dc);           // COMMAND = 0
    SPI_write(module,ST7735_CASET);              // set column range (x0,x1)

    ST7735_high(ST7735[module].pin.dc);          // DATA = 1
    SPI_write(module,0);
    SPI_write(module,x);
    SPI_write(module,0);
    SPI_write(module,x);

    ST7735_low(ST7735[module].pin.dc);           // COMMAND = 0
    SPI_write(module,ST7735_RASET);              // set row range (y0,y1)

    ST7735_high(ST7735[module].pin.dc);          // DATA = 1
    SPI_write(module,0);
    SPI_write(module,y);
    SPI_write(module,0);
    SPI_write(module,y);

    ST7735_low(ST7735[module].pin.dc);           // COMMAND = 0
    SPI_write(module,ST7735_RAMWR);

    ST7735_high(ST7735[module].pin.dc);          // DATA = 1
    SPI_write(module,ST7735[module].bcolor.c >> 8);
    SPI_write(module,ST7735[module].bcolor.c & 0xFF);
    
    ST7735_high(ST7735[module].pin.cs);          // Chip deselected
}

#ifdef ST7735GRAPHICS

void ST7735_drawVLine(u8 module, u16 x, u16 y, u16 h)
{
    u8 ch = ST7735[module].color.c >> 8;
    u8 cl = ST7735[module].color.c & 0xFF;

    if ( x >= ST7735_WIDTH || y >= (ST7735_HEIGHT) ) return;

    if ((y+h-1) >= ST7735_HEIGHT)
        h = ST7735_HEIGHT - y;
        
    ST7735_low(ST7735[module].pin.cs);             // Chip select
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_CASET);                // set column range (x0,x1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,x);
    SPI_write(module,0x00);
    SPI_write(module,x);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RASET);                // set row range (y0,y1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,y);
    SPI_write(module,0x00);
    SPI_write(module,y+h-1);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RAMWR);
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    while (h--)
    {
        SPI_write(module,ch);
        SPI_write(module,cl);
    }
    
    ST7735_high(ST7735[module].pin.cs);            // Chip deselected
}

void ST7735_drawHLine(u8 module, u16 x, u16 y, u16 w)
{
    u8 ch = ST7735[module].color.c >> 8;
    u8 cl = ST7735[module].color.c & 0xFF;

    if ( x >= ST7735_WIDTH || y >= (ST7735_HEIGHT) ) return;

    if ((x+w-1) >= ST7735_WIDTH)
        w = ST7735_WIDTH - x;
        
    ST7735_low(ST7735[module].pin.cs);             // Chip select
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_CASET);                // set column range (x0,x1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,x);
    SPI_write(module,0x00);
    SPI_write(module,x+w-1);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RASET);                // set row range (y0,y1)
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    SPI_write(module,0x00);
    SPI_write(module,y);
    SPI_write(module,0x00);
    SPI_write(module,y);
    
    ST7735_low(ST7735[module].pin.dc);             // COMMAND = 0
    SPI_write(module,ST7735_RAMWR);
    
    ST7735_high(ST7735[module].pin.dc);            // DATA = 1
    while (w--)
    {
        SPI_write(module,ch);
        SPI_write(module,cl);
    }
    
    ST7735_high(ST7735[module].pin.cs);            // Chip deselected
}
#endif

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Graphic routines based on drawPixel in graphics.c
    PARAMETERS:
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

#ifdef ST7735GRAPHICS

void drawPixel(u16 x, u16 y)
{
    ST7735_drawPixel(ST7735_SPI, x, y);
}

void drawHLine(u16 x, u16 y, u16 w)
{
    ST7735_drawHLine(ST7735_SPI, x, y, w);
}

void drawVLine(u16 x, u16 y, u16 h)
{
    ST7735_drawVLine(ST7735_SPI, x, y, h);
}

void ST7735_drawLine(u8 module, u16 x0, u16 y0, u16 x1, u16 y1)
{
    ST7735_SPI = module;
    drawLine(x0, y0, x1, y1);
}

void ST7735_drawRect(u8 module, u16 x1, u16 y1, u16 x2, u16 y2)
{
    ST7735_SPI = module;
    drawRect(x1, y1, x2, y2);
}

void ST7735_drawRoundRect(u8 module, u16 x1, u16 y1, u16 x2, u16 y2)
{
    ST7735_SPI = module;
    drawRoundRect(x1, y1, x2, y2);
}

void ST7735_drawCircle(u8 module, u16 x, u16 y, u16 radius)
{
    ST7735_SPI = module;
    drawCircle(x, y, radius);
}

void ST7735_fillCircle(u8 module, u16 x, u16 y, u16 radius)
{
    ST7735_SPI = module;
    fillCircle(x, y, radius);
}

#endif // ST7735GRAPHICS

void ST7735_fillRect(u8 module, u16 x1, u16 y1, u16 x2, u16 y2)
{
    ST7735_SPI = module;
    fillRect(x1, y1, x2, y2);
}

#ifdef ST7735GRAPHICS

void ST7735_fillRoundRect(u8 module, u16 x1, u16 y1, u16 x2, u16 y2)
{
    ST7735_SPI = module;
    fillRoundRect(x1, y1, x2, y2);
}

#if 0 // Still to do
void ST7735_drawBitmap(u8 module, u16 x, u16 y, u16 w, u16 h, u16* bitmap)
{
    ST7735_SPI = module;
}
#endif

#endif // ST7735GRAPHICS

#endif // __ST7735_C
