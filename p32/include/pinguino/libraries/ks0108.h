/*
 * Copyright:      Fabian Maximilian Thiele  mailto:me@apetech.de
 * Author:         Fabian Maximilian Thiele
 * Pinguino Port:  Marcus Fazzi mailto:anunakin@vivaphp.net
 * known Problems: none
 * Version:        1.6.3
 * Description:    Graphic Library for KS0108- (and compatible) based LCDs
 * 
 * The port for pinguino, was based on the original version with some modifications 
 * based on version 1.1 and 2.0 of GLCD library for Arduino.
 * 
 * Pinguino 32X port by Marcus Fazzi <anunakin@ieee.org>
 * 23-02-2011 - Modified FAZZI
 * - Using PORTD for data bits
 * - Remake of ASM 8bits for 32bits
 */

// #pragma std_sdcc99
// #pragma std_c99

// Uncomment for slow drawing
// #define DEBUG

//Try it if you have a fast display, not my case, dont know if it works!
// #define FAST_DISPLAY

/**
 * Fonts for displat Text
 * If you get error: multiple sections using address 0x6500 
 * on load more then one truetype font,  try modify font.h file to load at another address, AT YOUR RISC!
 * "The whole range of flash memory 0 to 8000H . Be careful, bootloader is from 0 to 2000H and user program is from 2000H to ??
 *   So for security, i use flash starting at 6000H, with this configuration you can store 8192 bytes."
 *
 *   Fixed Fonts starts at 0x6000
 *   True Type Fonts at 0x6500
 *   Bitmaps at 0x7600
*/

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

//Panel controller chips
#define CHIP_WIDTH     64  // pixels per chip 

//Using PORTD for UBW32
#define LCD_DATA_IN			PORTD		// Data Input Register, same as output, or (PORTD or LATD ?)
#define LCD_DATA_OUT		PORTD		// Data Output Register
#define LCD_DATA_DIR		TRISD		// Data Direction Register for Data Port

// Command port bits PORT relative positions
// Look at digital.c
// GLCD RESET Pin works better if connected to Pinguino reset, between 10K resistor and pinguino reset button

//Use PORTA pins for UBW32
#define LCD_CMD_PORT		PORTA		// Command Output Register
#define LCD_CMD_DIR			TRISA		// Data Direction Register for Command Port
#define LCD_CMD_SET			PORTASET
#define LCD_CMD_CLR			PORTACLR
//0x04,0x08,0x10,0x20,0x4000 Look on digitalw.c for masks!
#define D_I			0x04	//PIN 30 D/I or RS Bit Number
#define R_W			0x08	//PIN 31 R/W Bit Number
#define EN			0x10	//PIN 32 EN Bit Number
#define CSEL1		0x20	//PIN 33 CS1 Bit Number
#define CSEL2		0x4000	//PIN 34 CS2 Bit Number

// Chips
#define CHIP1       0x00
#define CHIP2       0x01

// Commands
#define LCD_ON				0x3F
#define LCD_OFF				0x3E
#define LCD_SET_ADD			0x40
#define LCD_SET_PAGE		0xB8
#define LCD_DISP_START		0xC0
#define LCD_BUSY_FLAG		0x80

// Colors
#define BLACK				0xFF
#define WHITE				0x00

// useful user contants
#define NON_INVERTED 0
#define INVERTED     1

// Font Indices
#define FONT_LENGTH			0
#define FONT_FIXED_WIDTH	2
#define FONT_HEIGHT			3
#define FONT_FIRST_CHAR		4
#define FONT_CHAR_COUNT		5
#define FONT_WIDTH_TABLE	6

#if (DISPLAY_WIDTH / CHIP_WIDTH  == 2) 
   unsigned char chipSelect[] = {1,2};        // this is for 128 pixel displays
#elif (DISPLAY_WIDTH / CHIP_WIDTH  == 3)
   //byte chipSelect[] = {0, 1, 2};  // this is for 192 pixel displays
   unsigned char chipSelect[] = {0, 2, 1};  // this is for 192 pixel displays on sanguino only
#endif

typedef struct {
	unsigned char x;
	unsigned char y;
	unsigned char page;
} lcdCoord;

//
// Function Prototypes
//

// Graphic Functions
void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color);
void DrawRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char color);
void DrawRoundRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char radius, unsigned char color);
void FillRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height, unsigned char color);
void InvertRect(unsigned char x, unsigned char y, unsigned char width, unsigned char height);
void SetInverted(unsigned char invert);
void SetDot(unsigned char x, unsigned char y, unsigned char color);

void ClearPage(unsigned char page, unsigned char color);
void ClearScreen(unsigned char color);

#define DrawVertLine(x, y, length, color) FillRect(x, y, 0, length, color)
#define DrawHoriLine(x, y, length, color) FillRect(x, y, length, 0, color)
#define DrawCircle(xCenter, yCenter, radius, color) DrawRoundRect(xCenter-radius, yCenter-radius, 2*radius, 2*radius, radius, color)
#define ClearScreenX() FillRect(0, 0, (DISPLAY_WIDTH-1), (DISPLAY_HEIGHT-1), WHITE)
#define ClearSysTextLine(_line) FillRect(0, (line*8), (DISPLAY_WIDTH-1), ((line*8)+ 7), WHITE )

//Pinguino, for now, not needs more ports like arduino project
//Fastest Displays use Macros, more code but a bit more fast
#ifndef SLOW_DISPLAY
#define fastWriteHigh(_pin_) (LCD_CMD_SET = _pin_)
#define fastWriteLow(_pin_) (LCD_CMD_CLR = _pin_)
#else
//Slow Display use functions
void fastWriteHigh(unsigned int pin);
void fastWriteLow(unsigned int pin);
#endif

// Font Functions
unsigned char ReadFontData(const unsigned char* ptr);		//Standard Read Callback
void SelectFont(const char* font, unsigned char color);
void PrintNumber(long n);
#ifdef USEFLOAT
void PrintFloat(float number, unsigned char digits);
#endif
int PutChar(char c);
void Puts(char* str);
unsigned char CharWidth(char c);
unsigned int StringWidth(char* str);

// Control Functions
void CursorTo( unsigned char x, unsigned char y);
void GotoXY(unsigned char x, unsigned char y);
void Init(unsigned char invert);
void Enable(void);
unsigned char DoReadData(unsigned char first);
unsigned char ReadData(void);
void WriteCommand(unsigned char cmd, unsigned char chip);
void delay450ns(void);
void WriteData(unsigned char data);
void DrawBitmap(const unsigned char * bitmap, unsigned char x, unsigned char y, unsigned char color);
