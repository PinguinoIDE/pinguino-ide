/*
  ITDB02_Graph.h - Arduino library support for ITDB02 LCD Board
  Copyright (C)2010-2011 Henning Karlsen. All right reserved
  
  Basic functionality of this library are based on the demo-code provided by
  ITead studio. You can find the latest version of the library at
  http://www.henningkarlsen.com/electronics

  This library has been made especially for the 2.4" TFT LCD Screen Module: 
  ITDB02-2.4 by ITead studio. This library has been designed to use 8bit mode, 
  so it will not work with the 3.2" Module.

  If you make any modifications or improvements to the code, I would appreciate
  that you share the code with me so that I might include it in the next release.
  I can be contacted through http://www.henningkarlsen.com/electronics/contact.php

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Version:   1.0  - Jul  10 2010  - initial release
			 1.01 - Aug  11 2010  - Fixed a small bug with the color green.
									Thanks to Thomas finding and fixing the bug.
			 2.0  - Aug  13 2010  - Added the possibility to use the display in
                                    Landscape mode. Also added a larger font by
									request.
			 2.1  - Sep  30 2010  - Added Arduino Mega compatibility.
                                    Fixed a bug with CENTER and RIGHT in LANDSCAPE mode.
									Fixed a bug in printNumI and printNumF when the 
									number to be printed was 0.
			 2.2  - Oct  14 2010  - Added drawBitmap() with its associated tool
			 2.3  - Nov  24 2010  - Added Arduino Mega2560 compatibility
									Added support for rotating text and bitmaps
			 2.4  - Jan  18 2011  - Fixed an error in the requirements
			 2.5  - Jan  30 2011  - Added loadBitmap()
									Optimized drawBitmap() when not using rotation
		     2.5.1- Mar  04 2011  - Port to Pinguino 32X, By Marcus Fazzi
			 2.6  - Mar  08 2011  - Fixed a bug in printNumF when the number to be printed
									was (-)0.something
             2.6.1 - Jun  09 2011 - Support ITDB02 16bit bus version, PORTD on PIC32 has 16 pins! ;-)
             4.1   - Jun  11 2011 - Ported Arduino 4.1 upgrades
             4.1.1 - Jun  16 2011 - Added more speed-ups as C macros for pixel drive
*/


#ifndef ITDB02_Graph16_h
#define ITDB02_Graph16_h

// #define _NO_BIG_FONT_

// Uncommenting the previous line will disable the large font, but will also decrease
// the library's memory footprint by ~3400 chars. Uncomment if you dont need the large
// font and want to reduce memory usage.

// #define _ENABLE_tinyFAT_INTEGRATION_

// Uncommenting the previous line will enable integration with my tinyFAT library.
// By enabling this integration you will be able to use loadBitmap().

#define LEFT 0
#define RIGHT 9999
#define CENTER 9998

#define PORTRAIT 0
#define LANDSCAPE 1

#define FONT_SMALL 1
#define FONT_LARGE 2

#define ASPECT_4x3	0
#define ASPECT_16x9	1

//Pinguino32X, UBW32, EMPEROR and Minimum boards
#if defined(UBW32_460) || defined(UBW32_795) || defined(EMPEROR460) || defined(EMPEROR795)

#define LCD_DATA_DIR 	TRISD		//Data Direction Register for Data Port
#define LCD_DATA_BUS 	PORTD		//Data Bus

#define LCD_CMD_SET		PORTESET
#define LCD_CMD_CLR		PORTECLR
#define LCD_SET_OUT		TRISECLR

//Pins for Emperor Board are: 54 47 45 and 44 (emeperor) (RE0, 4,5 and RE6) look on digitalw.c for pinmasks and ports
//Using inline/defines is a bit more fast and use less RAM and Flash, then use digitalw.c pinMode/pinmask array
#define LCD_RS 			0x1
#define LCD_WR 			0x10
#define LCD_CS 			0x20
#define LCD_REST 		0x40

#endif

#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
//TODO: Test on OLIMEX boards <<<< NOT IMPLEMENTED >>>>
#define LCD_DATA_DIR 	TRISA		//Data Direction Register for Data Port
#define LCD_DATA_BUS 	PORTA		//Data Bus
#define LCD_CMD_SET		PORTDSET
#define LCD_CMD_CLR		PORTDCLR

#endif

//Very fast digitalWrite/pinmode, similar to cbi/sbi functions, but more fast
#define fastWriteHigh(_pin_) (LCD_CMD_SET = _pin_)
#define fastWriteLow(_pin_) (LCD_CMD_CLR = _pin_)

//Fast way on set a pin as OUTPUT
#define fastSetOutputMode(_pin_) (LCD_SET_OUT = _pin_)

//Fast delay, new compiler is faster and we need it
#define fastDelay(){ \
     asm("nop"); \
}

//Fast way for drive pixels
#define fastSetPixel(_r_,_g_,_b_) { \
    fastWriteHigh(LCD_RS); \
    fastDelay(); \
    LCD_DATA_BUS = (((_r_ & 0xF8)<<8) + ((_g_ & 0xFC)<<3) + ((_b_ & 0xF8)>>3)); \
    fastDelay(); \
    fastWriteLow(LCD_WR); \
    fastDelay(); \
    fastWriteHigh(LCD_WR); \
    fastDelay(); \
}

#define fastWriteData(_data_) { \
    fastWriteHigh(LCD_RS); \
    fastDelay(); \
    LCD_DATA_BUS = _data_; \
    fastDelay(); \
    fastWriteLow(LCD_WR); \
    fastDelay(); \
    fastWriteHigh(LCD_WR); \
    fastDelay(); \
}

#define fastWriteCom(_data_) { \
    fastWriteLow(LCD_RS); \
    fastDelay(); \
    LCD_DATA_BUS = _data_; \
    fastDelay(); \
    fastWriteLow(LCD_WR); \
    fastDelay(); \
    fastWriteHigh(LCD_WR); \
    fastDelay(); \
}

struct _current_font
{
	uint8_t* font;
	uint8_t x_size;
	uint8_t y_size;
	uint8_t offset;
	uint8_t numchars;
};

//TODO: Finish it!
#if defined(PIC32_PINGUINO) || defined(PIC32_PINGUINO_OTG)
void pinguinoDATABUS(int data);
void pinguinoDATADIR(int data);
#endif

void InitLCD(char orientation);
void clrScr();
void drawPixel(int x, int y);
void drawLine(int x1, int y1, int x2, int y2);
void fillScr(char r, char g, char b);
void drawRect(int x1, int y1, int x2, int y2);
void drawRoundRect(int x1, int y1, int x2, int y2);
void fillRect(int x1, int y1, int x2, int y2);
void fillRoundRect(int x1, int y1, int x2, int y2);
void drawCircle(int x, int y, int radius);
void fillCircle(int x, int y, int radius);
void setColor(unsigned char r, unsigned char g, unsigned char b);
void setBackColor(unsigned char r, unsigned char g, unsigned char b);
void myGLCD16_print(char *st, int x, int y, int deg);
void printNumI(long num, int x, int y);
void printNumF(double num, char dec, int x, int y);
void fontSize(char size);
void drawBitmap(int x, int y, int sx, int sy, unsigned int* data, int scale);
void drawBitmapR(int x, int y, int sx, int sy, unsigned int* data, int deg, int rox, int roy);

//private
unsigned char fcolorr,fcolorg,fcolorb;
unsigned char bcolorr,bcolorg,bcolorb;
unsigned char orient;
unsigned char fsize;    
void LCD_Writ_Bus(int data);
void LCD_Write_COM(int data);
void LCD_Write_DATA(int data);
//void main_W_com_data(int com1,int dat1);
void setPixel(int r,int g,int b);
void drawHLine(int x, int y, int l);
void drawVLine(int x, int y, int l);
void printChar(char c, int x, int y);
void setXY(int x1, int y1, int x2, int y2);
void rotateChar(char c, int x, int y, int pos, int deg);
    
#endif
