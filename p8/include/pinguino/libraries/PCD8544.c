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

#ifndef __PCD8544_C
#define __PCD8544_C

#include <PCD8544.h>
#include <const.h>              // false, true, ...
#include <macro.h>              // BitSet, BitClear, ...
#include <typedef.h>            // u8, u16, ...
#include <delayms.c>            // Delayms, Delayus
//#include <string.h>             // memset
#ifdef _PCD8544_USE_GRAPHICS
#include <graphics.c>           // graphic routines
#endif
#include <logo/pinguino84x48.h> // Pinguino Logo

#if !(PCD8544_INTERFACE & PCD8544_PORTB)
/*
    #ifdef DIGITALWRITE
        #define DIGITALWRITE
    #endif
    #ifdef PINMODE
        #define PINMODE
    #endif
*/
    #include <digitalw.c>       // pinmode, digitalwrite
    #include <digitalp.c>       // pinmode, digitalwrite
#endif

// Printf
#ifdef PCD8544PRINTF
    #include <stdarg.h>
    #ifdef __PIC32MX__
        #include <printf.c>
    #else
        #include <stdio.c>
    #endif
#endif

#if (PCD8544_INTERFACE & PCD8544_SPIHW)
/*
    #ifdef SPIBEGIN
        #define SPIBEGIN
    #endif
    #ifdef SPISETMODE
        #define SPISETMODE
    #endif
    #ifdef SPISETDATAMODE
        #define SPISETDATAMODE
    #endif
    #ifdef SPIWRITE
        #define SPIWRITE
    #endif
*/
    #include <spi.c>
#endif

///	--------------------------------------------------------------------
/// Core functions
///	--------------------------------------------------------------------

// regis : removed LSBFIRST test (always MSBFIRST here)
#if !(PCD8544_INTERFACE & PCD8544_SPIHW)
void PCD8544_shiftOut(u8 c)
{
    u8 i;

    for (i = 0; i < 8; i++)
    {
        #if (PCD8544_INTERFACE & PCD8544_SPISW)
            #if (PCD8544_INTERFACE & PCD8544_PORTB)
                if (c & 0x80)
                    High(PCD8544_SDIN);
                else
                    Low(PCD8544_SDIN);
                High(PCD8544_SCLK);
                Low(PCD8544_SCLK);
            #else
                digitalwrite(PCD8544._din, (c & 0x80) ? HIGH : LOW);
                digitalwrite(PCD8544._sclk, HIGH);
                digitalwrite(PCD8544._sclk, LOW);
            #endif
            c <<= 1;
        #endif
    }
}
#endif

void PCD8544_command(u8 c)
{
    #if (PCD8544_INTERFACE & PCD8544_SPISW)
        #if (PCD8544_INTERFACE & PCD8544_PORTB)
            Low(PCD8544_DC);
            Low(PCD8544_SCE);
            PCD8544_shiftOut(c);
            High(PCD8544_SCE); 
        #else
            // DC = LOW => COMMAND
            digitalwrite(PCD8544._dc, LOW);
            if (PCD8544._cs > 0)
                digitalwrite(PCD8544._cs, LOW);
                
            PCD8544_shiftOut(c);
            
            if (PCD8544._cs > 0)
                digitalwrite(PCD8544._cs, HIGH); 
        #endif
    #elif (PCD8544_INTERFACE & PCD8544_SPIHW)
        // DC = LOW => COMMAND
        digitalwrite(PCD8544._dc, LOW);
        SELECT();
        SPI_write(c);
        DESELECT();
    #endif
}
 
void PCD8544_data(u8 c)
{
    #if (PCD8544_INTERFACE & PCD8544_SPISW)
        #if (PCD8544_INTERFACE & PCD8544_PORTB)
            High(PCD8544_DC);
            Low(PCD8544_SCE);
            PCD8544_shiftOut(c);
            High(PCD8544_SCE); 
        #else
            // DC = HIGH => DATA
            digitalwrite(PCD8544._dc, HIGH);
            if (PCD8544._cs > 0)
                digitalwrite(PCD8544._cs, LOW);
                
            PCD8544_shiftOut(c);
            
            if (PCD8544._cs > 0)
                digitalwrite(PCD8544._cs, HIGH);
        #endif
    #elif (PCD8544_INTERFACE & PCD8544_SPIHW)
        // DC = HIGH => DATA
        digitalwrite(PCD8544._dc, HIGH);
        SELECT();
        SPI_write(c);
        DESELECT();
    #endif
}

///	--------------------------------------------------------------------
/// Display functions
///	--------------------------------------------------------------------

#ifdef enablePartialUpdate
static u8 xUpdateMin, xUpdateMax, yUpdateMin, yUpdateMax;

static void PCD8544_updateBoundingBox(u8 xmin, u8 ymin, u8 xmax, u8 ymax)
{
  if (xmin < xUpdateMin) xUpdateMin = xmin;
  if (xmax > xUpdateMax) xUpdateMax = xmax;
  if (ymin < yUpdateMin) yUpdateMin = ymin;
  if (ymax > yUpdateMax) yUpdateMax = ymax;
}
#endif

#if (PCD8544_INTERFACE & PCD8544_SPISW)
    #if (PCD8544_INTERFACE & PCD8544_PORTB)
    void PCD8544_init()
    #else
    void PCD8544_init(u8 SCLK, u8 DIN, u8 DC, u8 CS, u8 RST)
    #endif
#elif (PCD8544_INTERFACE & PCD8544_SPIHW)
    void PCD8544_init(u8 DC, u8 RST)
#endif
{
    //pins
    #if (PCD8544_INTERFACE & PCD8544_SPISW)
        #if !(PCD8544_INTERFACE & PCD8544_PORTB)
            PCD8544._rst  = RST;
            PCD8544._cs   = CS;
            PCD8544._dc   = DC;
            PCD8544._din  = DIN;
            PCD8544._sclk = SCLK;
        #endif
    #elif (PCD8544_INTERFACE & PCD8544_SPIHW)
        PCD8544._rst  = RST;
        PCD8544._dc   = DC;
    #endif

    //GFX
    PCD8544.screen.width    = DISPLAY_WIDTH;
    PCD8544.screen.height   = DISPLAY_HEIGHT;
    PCD8544.orientation     = PORTRAIT;
    PCD8544.cursor.y        = 0;
    PCD8544.cursor.x        = 0;
    PCD8544.textsize        = 1;
    PCD8544.color.c         = BLACK;
    PCD8544.bcolor.c        = WHITE;
    //PCD8544.wrap = true;

    #if (PCD8544_INTERFACE & PCD8544_SPISW)
        #if (PCD8544_INTERFACE & PCD8544_PORTB)
            LATB  = 0b00111111; // GND to GND, backlight On, VCC to 3.3V 
            TRISB = 0b00000000; // all pins are output
        #else
            digitalwrite(PCD8544._dc,   HIGH);
            digitalwrite(PCD8544._cs,   HIGH);
            digitalwrite(PCD8544._rst,  HIGH);
            digitalwrite(PCD8544._din,  HIGH);
            digitalwrite(PCD8544._sclk, HIGH);

            // set pin directions
            pinmode(PCD8544._din,  OUTPUT);
            pinmode(PCD8544._sclk, OUTPUT);
            pinmode(PCD8544._dc,   OUTPUT);
            if (PCD8544._rst > 0)
            pinmode(PCD8544._rst,  OUTPUT);
            if (PCD8544._cs > 0)
            pinmode(PCD8544._cs,   OUTPUT);
        #endif
    #elif (PCD8544_INTERFACE & PCD8544_SPIHW)
        digitalwrite(PCD8544._dc,   HIGH);
        digitalwrite(PCD8544._rst,  HIGH);
        pinmode(PCD8544._dc,   OUTPUT);
        if (PCD8544._rst > 0)
            pinmode(PCD8544._rst,  OUTPUT);
        // SPI_MASTER_FOSC_4, SPI_MODE0, SPI_SMPEND;
        SPI_setMode(SPI_MASTER_FOSC_16);
        SPI_setDataMode(SPI_MODE0);
        SPI_begin();
    #endif

    // toggle RST low to reset
    #if (PCD8544_INTERFACE & PCD8544_PORTB)
        Low(PCD8544_RST);
        Delayms(50);
        High(PCD8544_RST);
    #else
        if (PCD8544._rst > 0)
        {
            digitalwrite(PCD8544._rst, LOW);
            Delayms(30);
            digitalwrite(PCD8544._rst, HIGH);
        }
    #endif

    // get into the EXTENDED mode!
    PCD8544_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );

    // LCD bias select (4 is optimal?)
    PCD8544_command(PCD8544_SETBIAS | 0x4);

    // set VOP
    PCD8544_command(PCD8544_SETVOP | 0x7F); // contrast set to max by default

    // back to normal mode
    PCD8544_command(PCD8544_FUNCTIONSET);

    // Set display to Normal
    PCD8544_command(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL);

    // set up a bounding box for screen updates

    #ifdef enablePartialUpdate
    PCD8544_updateBoundingBox(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);
    #endif
    
    // Push out PCD8544_buffer to the Display (will show the AFI logo)
    PCD8544_refresh();
}

void PCD8544_refresh()
{
    u8 col, p;
     
    #if (PCD8544_INTERFACE & PCD8544_SPISW)
        #if (PCD8544_INTERFACE & PCD8544_PORTB)
            for(p = 0; p <= DISPLAY_ROWS; p++) 
            {
                col = 0;
                PCD8544_command(PCD8544_SETYADDR | p);
                PCD8544_command(PCD8544_SETXADDR | col);
                // send data in DDRAM
                High(PCD8544_DC);
                Low(PCD8544_SCE);
                for(; col < DISPLAY_WIDTH; col++) 
                    PCD8544_shiftOut(PCD8544_buffer[(DISPLAY_WIDTH*p)+col]);
                High(PCD8544_SCE);
            }
        #else
            for(p = 0; p <= DISPLAY_ROWS; p++) 
            {
                col = 0;
                PCD8544_command(PCD8544_SETYADDR | p);
                PCD8544_command(PCD8544_SETXADDR | col);
                // send data in DDRAM
                digitalwrite(PCD8544._dc, HIGH);
                if (PCD8544._cs > 0)
                    digitalwrite(PCD8544._cs, LOW);
                for(; col < DISPLAY_WIDTH; col++) 
                    PCD8544_shiftOut(PCD8544_buffer[(DISPLAY_WIDTH*p)+col]);
                if (PCD8544._cs > 0)
                    digitalwrite(PCD8544._cs, HIGH);
            }
        #endif
    #elif (PCD8544_INTERFACE & PCD8544_SPIHW)
        for(p = 0; p <= DISPLAY_ROWS; p++) 
        {
            col = 0;
            PCD8544_command(PCD8544_SETYADDR | p);
            PCD8544_command(PCD8544_SETXADDR | col);
            // send data in DDRAM
            digitalwrite(PCD8544._dc, HIGH);
            SELECT();
            for(; col < DISPLAY_WIDTH; col++) 
                SPI_write(PCD8544_buffer[(DISPLAY_WIDTH*p)+col]);
            DESELECT();
        }
    #endif

    // no idea why this is necessary but it is to finish the last byte?
    PCD8544_command(PCD8544_SETYADDR );

    #ifdef enablePartialUpdate
      xUpdateMin = DISPLAY_WIDTH - 1;
      xUpdateMax = 0;
      yUpdateMin = DISPLAY_HEIGHT-1;
      yUpdateMax = 0;
    #endif
}

void PCD8544_clearScreen()
{   
    //memset(PCD8544_buffer, 0, DISPLAY_SIZE);

    u16 i;

    for (i = 0; i < DISPLAY_SIZE; i++)
        PCD8544_buffer[i] = 0;

    #ifdef enablePartialUpdate
    PCD8544_updateBoundingBox(0, 0, DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1);
    #endif

    PCD8544.cursor.y = 0;
    PCD8544.cursor.x = 0;
}

#ifdef _PCD8544_USE_DISPLAYONOFF
void PCD8544_displayOff(void)
{
    // First, fill RAM with zeroes to ensure minimum specified current consumption
    PCD8544_clearScreen();
    PCD8544_refresh();
    PCD8544_command(PCD8544_FUNCTIONSET|PCD8544_POWERDOWN);
}

void PCD8544_displayOn(void)
{
    PCD8544_command(PCD8544_FUNCTIONSET);
}
#endif

void PCD8544_setContrast(u8 val)
{
    PCD8544_command(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION );
    PCD8544_command(PCD8544_SETVOP | (val & 0x7F)); 
    PCD8544_command(PCD8544_FUNCTIONSET);
}

///	--------------------------------------------------------------------
/// Scroll functions
///	--------------------------------------------------------------------

void PCD8544_scrollUp()
{
    u8 x, y;

    // copy line y to line y-1
    for (y = 1; y <= DISPLAY_ROWS; y++)
    {
        for (x = 0; x < DISPLAY_WIDTH; x++)
        {
            PCD8544_buffer[x + DISPLAY_WIDTH * (y - 1)] = 
            PCD8544_buffer[x + DISPLAY_WIDTH * y];
        }
    }

    // clear last line
    for (x = 0; x < DISPLAY_WIDTH; x++)
    {
        PCD8544_buffer[x + DISPLAY_WIDTH * DISPLAY_ROWS] = 0;
    }    

    PCD8544.cursor.y--;
}

///	--------------------------------------------------------------------
/// Print functions
///	--------------------------------------------------------------------

void PCD8544_setFont(const u8 *font)
{
    PCD8544.font.address = font;
    PCD8544.font.width   = font[0];
    PCD8544.font.height  = font[1];
}

void PCD8544_printChar(u8 c)
{
    u8  b;

    while (PCD8544.cursor.x >= (PCD8544.screen.width / PCD8544.font.width))
    {
        PCD8544.cursor.x -= (PCD8544.screen.width / PCD8544.font.width);
        PCD8544.cursor.y++;            
    }

    while (PCD8544.cursor.y > DISPLAY_ROWS)
    {
        PCD8544_scrollUp();            
    }

    switch (c)
    {
        case '\n':
            PCD8544.cursor.y++;
            break;
            
        case '\r':
            PCD8544.cursor.x = 0;
            break;
            
        case '\t':
            PCD8544.cursor.x = (PCD8544.cursor.x + 4) % 4;
            break;
            
        default:
            for (b = 0; b < PCD8544.font.width; b++)
            {
                PCD8544_buffer[PCD8544.cursor.x * PCD8544.font.width + PCD8544.cursor.y * PCD8544.screen.width + b] = 
                    PCD8544.font.address[2 + (c - 32) * PCD8544.font.width + b];
            }
        
        PCD8544.cursor.x++;
    }            
}

#if defined(PCD8544PRINT)       || defined(PCD8544PRINTLN)    || \
    defined(PCD8544PRINTNUMBER) || defined(PCD8544PRINTFLOAT)
void PCD8544_print(u8 *string)
{
    while (*string != 0)
        PCD8544_printChar(*string++);
}
#endif

#if defined(PCD8544PRINTLN)
void PCD8544_println(u8 *string)
{
    PCD8544_print(string);
    PCD8544_print("\n\r");
}
#endif

#if defined(PCD8544PRINTNUMBER) || defined(PCD8544PRINTFLOAT)
void PCD8544_printNumber(long value, u8 base)
{  
    u8 sign;
    u8 length;

    long i;
    unsigned long v;    // absolute value

    u8 tmp[12];
    u8 *tp = tmp;       // pointer on tmp

    u8 string[12];
    u8 *sp = string;    // pointer on string

    if (value==0)
    {
        PCD8544_printChar('0');
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

    PCD8544_print(string);
}
#endif

#if defined(PCD8544PRINTFLOAT)
void PCD8544_printFloat(float number, u8 digits)
{ 
	u8 i, toPrint;
	u16 int_part;
	float rounding, remainder;

	// Handle negative numbers
	if (number < 0.0)
	{
		PCD8544_printChar('-');
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
	PCD8544_printNumber(int_part, 10);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		PCD8544_printChar('.'); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
		PCD8544_printNumber(toPrint, 10);
		remainder -= toPrint; 
	}
}
#endif

#if defined(PCD8544PRINTF)
void PCD8544_printf(const u8 *fmt, ...)
{
    static u8 buffer[25];
    u8 *c=(char*)&buffer;
    va_list	args;

    va_start(args, fmt);
    psprintf2(buffer, fmt, args);
    va_end(args);

    while (*c != 0)
        PCD8544_printChar(*c++);
}
#endif

///	--------------------------------------------------------------------
/// Graphic functions
///	--------------------------------------------------------------------

#ifdef _PCD8544_USE_GRAPHICS

//draw Pixel on the buffer
void PCD8544_drawPixel(u8 x, u8 y)//, u8 color)
{
    if (x >= DISPLAY_WIDTH)  return;
    if (y >= DISPLAY_HEIGHT) return;

    PCD8544_buffer[x + (y >> 3) * DISPLAY_WIDTH] |=  1 << (y % 8);
} 

// defined as extern void drawPixel(u16 x, u16 y); in graphics.c
void drawPixel(u16 x, u16 y)
{
    PCD8544_drawPixel((u8)x, (u8)y);
}

//clear Pixel on the buffer
void PCD8544_clearPixel(u8 x, u8 y)
{
    if (x >= DISPLAY_WIDTH)  return;
    if (y >= DISPLAY_HEIGHT) return;

    PCD8544_buffer[x + (y >> 3) * DISPLAY_WIDTH] &= ~(1 << (y % 8)); 
} 

u8 PCD8544_getPixel(u8 x, u8 y)
{
    if (x >= DISPLAY_WIDTH)  return;
    if (y >= DISPLAY_HEIGHT) return;

    return (PCD8544_buffer[x+ (y>>3)*DISPLAY_WIDTH] >> (y%8)) & 0x1;  
}

void PCD8544_drawCircle(u8 x, u8 y, u8 radius)
{
    drawCircle(x, y, radius);
}

void PCD8544_fillCircle(u8 x, u8 y, u8 radius)
{
    fillCircle(x, y, radius);
}

void PCD8544_drawLine(u8 x0, u8 y0, u8 x1, u8 y1)
{
    drawLine(x0, y0, x1, y1);
}

void PCD8544_drawFastVLine(u8 x, u8 y, u8 h, u8 color)
{
    drawLine(x, y, x, y+h-1);//, color);
}

void PCD8544_drawFastHLine(u8 x, u8 y, u8 w, u8 color)
{
    drawLine(x, y, x+w-1, y);//, color);
}

#endif

/*
void PCD8544_drawBitmap(u8 x, u8 y, u8 w, u8 h, u8* bitmap)
{
}
*/


/***                       ***/
/*** TO MOVE TO GRAPHICS.C ***/
/***                       ***/

/*
#ifdef  _PCD8544_USE_CIRCLE
void PCD8544_drawCircle(u8 x0, u8 y0, u8 r, u8 color)
{
 u8 x,y,radiusError;
 x=r;
 y=0;
 radiusError =1-x;
 while(x >= y)
  {
    PCD8544_drawPixel(x + x0, y + y0, color);
    PCD8544_drawPixel(y + x0, x + y0, color);
    PCD8544_drawPixel(-x + x0, y + y0, color);
    PCD8544_drawPixel(-y + x0, x + y0, color);
    PCD8544_drawPixel(-x + x0, -y + y0, color);
    PCD8544_drawPixel(-y + x0, -x + y0, color);
    PCD8544_drawPixel(x + x0, -y + y0, color);
    PCD8544_drawPixel(y + x0, -x + y0, color);
    y++;
    if (radiusError<0)
    {
      radiusError += (y<<1) + 1;
    } else {
      x--;
      radiusError+= ((y - x + 1)<<1);
    }
  }
 }
 
  void PCD8544_drawCircleHelper(u8 x0, u8 y0, u8 r, u8 cornername,u8 color)
  {
      u8 f     = 1 - r;
      u8 ddF_x = 1;
      u8 ddF_y = -2 * r;
      u8 x     = 0;
      u8 y     = r;

      while (x<y) 
      {
          if (f >= 0) 
          {
               y--;
               ddF_y += 2;
               f+= ddF_y;
          }
          x++;
          ddF_x += 2;
          f+= ddF_x;
          if (cornername & 0x4) {
              PCD8544_drawPixel(x0 + x, y0 + y, color);
              PCD8544_drawPixel(x0 + y, y0 + x, color);
            } 
          if (cornername & 0x2) {
              PCD8544_drawPixel(x0 + x, y0 - y, color);
              PCD8544_drawPixel(x0 + y, y0 - x, color);
            }
          if (cornername & 0x8) {
              PCD8544_drawPixel(x0 - y, y0 + x, color);
              PCD8544_drawPixel(x0 - x, y0 + y, color);
            }
          if (cornername & 0x1) {
             PCD8544_drawPixel(x0 - y, y0 - x, color);
              PCD8544_drawPixel(x0 - x, y0 - y, color);
            }
      }
  }

  void PCD8544_fillCircle(u8 x0, u8 y0, u8 r, u8 color)
  {
      PCD8544_drawFastVLine(x0, y0-r, (r<<1)+1, color);
      PCD8544_fillCircleHelper(x0, y0, r, 3, 0, color);
  }

void PCD8544_fillCircleHelper(u8 x0, u8 y0, u8 r, u8 cornername,u8 delta, u8 color)
{
    u8 f     = 1 - r;
    u8 ddF_x = 1;
    u8 ddF_y = -2 * r;
    u8 x     = 0;
    u8 y     = r;
     while (x<y) 
    {
        if (f >= 0) 
          {
              y--;
              ddF_y += 2;
              f += ddF_y;
          }
         x++;
         ddF_x += 2;
         f+= ddF_x;
         if (cornername & 0x1)
         {
              PCD8544_drawFastVLine(x0+x, y0-y, (y<<1)+1+delta, color);
              PCD8544_drawFastVLine(x0+y, y0-x, (x<<1)+1+delta, color);
         }
        if (cornername & 0x2) 
        {
            PCD8544_drawFastVLine(x0-x, y0-y, (y<<1)+1+delta, color);
            PCD8544_drawFastVLine(x0-y, y0-x, (x<<1)+1+delta, color);
        }
         
    }
}  
#endif
   void PCD8544_drawLine(u8 x0, u8 y0, u8 x1, u8 y1, u8 color)
   {
      u8 dx, dy,steep,err,ystep;
      steep = PCD8544_abs(y1 - y0) > PCD8544_abs(x1 - x0);
      if (steep) 
      {
        swap(x0, y0);
        swap(x1, y1);
      }

      if (x0 > x1) 
      {
        swap(x0, x1);
        swap(y0, y1);
      }

      
      dx = x1 - x0;
      dy = PCD8544_abs(y1 - y0);

      err = (dx>>1);
     

      if (y0 < y1) 
      {
        ystep = 1;
      } 
      else 
      {
        ystep = -1;
      }

      for (; x0<=x1; x0++) 
      {
            if (steep) 
            {
              PCD8544_drawPixel(y0, x0, color);
            } 
            else 
            {
              PCD8544_drawPixel(x0, y0, color);
            }
          err -= dy;
          if (err < 0) 
          {
          y0 += ystep;
           err += dx;
          }
      }
   }
   #ifdef _PCD8544_USE_RECT
void PCD8544_drawRect(u8 x, u8 y, u8 w, u8 h, u8 color)
{
    PCD8544_drawFastHLine(x, y, w, color);
    PCD8544_drawFastHLine(x, y+h-1, w, color);
    PCD8544_drawFastVLine(x, y, h, color);
    PCD8544_drawFastVLine(x+w-1, y, h, color);
}
void PCD8544_fillRect(u8 x, u8 y, u8 w, u8 h, u8 color)
 {
    u8 i;
    for (i=x; i<x+w; i++) 
    {
        PCD8544_drawFastVLine(i, y, h, color);
    }
 }
 #endif
*/

/*
void PCD8544_fillScreen(u8 color)
 {
    PCD8544_fillRect(0, 0,PCD8544.screen.width,PCD8544..screen.height, color);
 }

#ifdef _PCD8544_USE_ROUND_RECT
 void PCD8544_drawRoundRect(u8 x, u8 y, u8 w,u8 h, u8 r, u8 color)
 {
     PCD8544_drawFastHLine(x+r  , y    , w-(r<<1), color); // Top
     PCD8544_drawFastHLine(x+r  , y+h-1, w-(r<<1), color); // Bottom
     PCD8544_drawFastVLine(x    , y+r  , h-(r<<1), color); // Left
     PCD8544_drawFastVLine(x+w-1, y+r  , h-(r<<1), color); // Right
      // draw four corners
     PCD8544_drawCircleHelper(x+r    , y+r    , r, 1, color);
     PCD8544_drawCircleHelper(x+w-r-1, y+r    , r, 2, color);
     PCD8544_drawCircleHelper(x+w-r-1, y+h-r-1, r, 4, color);
     PCD8544_drawCircleHelper(x+r    , y+h-r-1, r, 8, color);
 }
 
 void PCD8544_fillRoundRect(u8 x, u8 y, u8 w,u8 h, u8 r, u8 color)
 {
     // smarter version
     PCD8544_fillRect(x+r, y, w-2*r, h, color);
     // draw four corners
     PCD8544_fillCircleHelper(x+w-r-1, y+r, r, 1, h-(r<<1)-1, color);
     PCD8544_fillCircleHelper(x+r    , y+r, r, 2, h-(r<<1)-1, color);
}
#endif

#ifdef  _PCD8544_USE_TRIANGLE
void PCD8544_drawTriangle(u8 x0, u8 y0, u8 x1, u8 y1,u8 x2, u8 y2, u8 color)
{
    PCD8544_drawLine(x0, y0, x1, y1, color);
    PCD8544_drawLine(x1, y1, x2, y2, color);
    PCD8544_drawLine(x2, y2, x0, y0, color);
}
void PCD8544_fillTriangle(u8 x0, u8 y0, u8 x1, u8 y1,u8 x2, u8 y2, u8 color)
{
 u8 a, b, y, last;
 u8 dx01,dy01,dx02,dy02,dx12,sa,sb,dy12; 
  // Sort coordinates by Y order (y2 >= y1 >= y0)
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }
  if (y1 > y2) {
    swap(y2, y1); swap(x2, x1);
  }
  if (y0 > y1) {
    swap(y0, y1); swap(x0, x1);
  }

  if(y0 == y2) { // Handle awkward all-on-same-line case as its own thing
    a = b = x0;
    if(x1 < a)      a = x1;
    else if(x1 > b) b = x1;
    if(x2 < a)      a = x2;
    else if(x2 > b) b = x2;
    PCD8544_drawFastHLine(a, y0, b-a+1, color);
    return;
  }

  
    dx01 = x1 - x0;
    dy01 = y1 - y0;
    dx02 = x2 - x0;
    dy02 = y2 - y0;
    dx12 = x2 - x1;
    dy12 = y2 - y1;
    sa   = 0;
    sb   = 0;

  // For upper part of triangle, find scanline crossings for segments
  // 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
  // is included here (and second loop will be skipped, avoiding a /0
  // error there), otherwise scanline y1 is skipped here and handled
  // in the second loop...which also avoids a /0 error here if y0=y1
  // (flat-topped triangle).
  if(y1 == y2) last = y1;   // Include y1 scanline
  else         last = y1-1; // Skip it

  for(y=y0; y<=last; y++) {
    a   = x0 + sa / dy01;
    b   = x0 + sb / dy02;
    sa += dx01;
    sb += dx02;
    // longhand:
    //a = x0 + (x1 - x0) * (y - y0) / (y1 - y0);
    //b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    if(a > b) swap(a,b);
    PCD8544_drawFastHLine(a, y, b-a+1, color);
  }

  // For lower part of triangle, find scanline crossings for segments
  // 0-2 and 1-2.  This loop is skipped if y1=y2.
  sa = dx12 * (y - y1);
  sb = dx02 * (y - y0);
  for(; y<=y2; y++) {
    a   = x1 + sa / dy12;
    b   = x0 + sb / dy02;
    sa += dx12;
    sb += dx02;
    // longhand:
    //a = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
    //b = x0 + (x2 - x0) * (y - y0) / (y2 - y0);
    if(a > b) swap(a,b);
    PCD8544_drawFastHLine(a, y, b-a+1, color);
  }
}
#endif
#ifdef  _PCD8544_USE_BITMAP
void PCD8544_drawBitmap(u8 x, u8 y, const u8 *bitmap,u8 w, u8 h, u8 color)
{
    u8 i, j, byteWidth;
    byteWidth= (w + 7) / 8;

    for(j=0; j<h; j++) 
      {
          for(i=0; i<w; i++ ) 
          {
                  if((bitmap[(j * byteWidth + i / 8)]) & (128 >> (i & 7))) 
                  {	
                    PCD8544_drawPixel(x+i, y+j, color);
                  }
          }
      }
}
#endif
*/

/*
void PCD8544_drawChar(u8 x, u8 y, u8 c, u8 color,u8 bg, u8 size)
{
    u8 i,j,k;
    u8 line;

    // test also done in drawPixel
    if((x >= PCD8544.screen.width)       || // Clip right
        (y >= PCD8544..screen.height)     || // Clip bottom
        ((x + 6 * size - 1) < 0)   || // Clip left
        ((y + (size>>3) - 1) < 0))    // Clip top
        return;
    
    if (c<0x20)return;
    if (c>0x7f)return;
  
    for (i=0; i<6; i++ )
    {
        //if (i == 5) 
        //    line = 0x0;
        //else //line = pgm_read_byte(font+(c*5)+i);
            line = PCD8544.font.address[ 2 + (c - 32) * 6 + i ];

        for (j = 0; j<8; j++)
        {
            if (line & 0x01)
            {
                if (size == 1) // default size
                {
                    //PCD8544_drawPixel(x+i, y+j, color);
                    PCD8544_drawPixel(x+i, y+j);
                }
                else
                {
                    // big size
                    //PCD8544_fillRect(x+(i*size), y+(j*size), size, size, color);
                    for (k=(x+(i*size)); k<x+(i*size)+size; k++) 
                        PCD8544_drawFastVLine(k, y+(j*size), size, color);
                } 
            }
            
            else if (bg != color)
            {
                if (size == 1) // default size
                {
                    //PCD8544_drawPixel(x+i, y+j, bg);
                    PCD8544_drawPixel(x+i, y+j);
                }
                else
                {
                    // big size
                    //PCD8544_fillRect(x+(i*size), y+(j*size), size, size, bg);
                    for (k=(x+(i*size)); k<x+(i*size)+size; k++) 
                        PCD8544_drawFastVLine(k, y+(j*size), size, bg);
                }
            }

            line >>= 1;
        }
    }
}
*/

#ifdef _PCD8544_USE_SETCURSOR
void PCD8544_setCursor(u8 x, u8 y)
{
     PCD8544.cursor.x = x;
     PCD8544.cursor.y = y;
}
#endif

#ifdef _PCD8544_USE_SETTEXTSIZE
void PCD8544_setTextSize(u8 s)
{
    PCD8544.textsize = (s > 0) ? s : 1;
}
#endif

#ifdef _PCD8544_USE_SETTEXTCOLOR
void PCD8544_setTextColor(u8 c)
{
    PCD8544.color.c = c;
    PCD8544.bcolor.c = c;
}
#endif

#ifdef _PCD8544_USE_SETTEXTCOLOR2
void PCD8544_setTextColor2(u8 c, u8 bg)
{
    PCD8544.color.c   = c;
    PCD8544.bcolor.c = bg; 
}
#endif

#ifdef _PCD8544_USE_ORIENTATION
void PCD8544_setOrientation(u8 x)
{
    PCD8544.orientation = (x & 3);
    switch(PCD8544.orientation) 
    {
        case 0:
        case 2:
            PCD8544.screen.width  = PCD8544.WIDTH;
            PCD8544..screen.height = PCD8544.HEIGHT;
            break;
        case 1:
        case 3:
            PCD8544.screen.width  = PCD8544.HEIGHT;
            PCD8544..screen.height = PCD8544.WIDTH;
            break;
    }
}
#endif
/* Replaced by define to optimize code 
void PCD8544_setTextWrap(boolean w)
{
    PCD8544.wrap = w;
}
u8 PCD8544_getOrientation()
{
    return PCD8544.orientation;
}
u8 PCD8544_width()
{
    return PCD8544.screen.width;
}
u8 PCD8544.screen.height()
{
    return PCD8544..screen.height;
} */
#ifdef  _PCD8544_USE_INVERT
void PCD8544_invertDisplay(bool i) {
  // Do nothing, must be subclassed if supported
}
#endif

//Print functions 

/*
#ifdef _PCD8544_USE_TEXT
void PCD8544_print(char *chaine)
{
    u8 counter;
    for( counter=0; counter<strlen(chaine); counter++)
        //PCD8544_printChar((chaine[counter - 1] < 10 ? '0' + chaine[counter - 1] : 'A' + chaine[counter - 1] - 10));
        PCD8544_printChar(chaine[counter]);
}

// taken from lcdlib.h Print a number on LCD
void PCD8544_printNumber(u8 n, u8 base)
{  
    u8 buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
    u8 i = 0;

    if (n == 0)
    {
        PCD8544_printChar('0');
        return;
    } 

    while (n > 0)
    {
        buf[i++] = n % base;
        n /= base;
    }

    for (; i > 0; i--)
        PCD8544_printChar((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}

#endif
*/

//Abs function
// Goal : Not using Math lib for one function
/*
u8 PCD8544_abs(u8 nb)
{
if (nb<0) nb=-nb;
return nb;
} */

#endif /* __PCD8544_C */
