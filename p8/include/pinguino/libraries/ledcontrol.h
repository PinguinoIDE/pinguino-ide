/*
 *    LedControl.h - A library for controling Leds with a MAX7219/MAX7221
 *    Copyright (c) 2007 Eberhard Fahle
 * 
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 * 
 *    This permission notice shall be included in all copies or 
 *    substantial portions of the Software.
 * 
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef LedControl_h
#define LedControl_h

#include <typedef.h>

#if defined(SCROLL) || defined(WRITESTRING) || defined(DISPLAYCHAR)
    #include <fonts/font8x8.h>
    u8 (*font)[8] = &font8x8[2];
#endif

#define LSBFIRST 100
#define MSBFIRST 101

/*
 * Segments to be switched on for characters and digits on
 * 7-Segment Displays
 */
 
#if defined(SETDIGIT) || defined(SETCHAR)
const static u8 charTable[128] = {
    0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,
    0b01111111,0b01111011,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b10000000,0b00000001,0b10000000,0b00000000,
    0b01111110,0b00110000,0b01101101,0b01111001,0b00110011,0b01011011,0b01011111,0b01110000,
    0b01111111,0b01111011,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,0b00000000,
    0b00110111,0b00000000,0b00000000,0b00000000,0b00001110,0b00000000,0b00000000,0b00000000,
    0b01100111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00001000,
    0b00000000,0b01110111,0b00011111,0b00001101,0b00111101,0b01001111,0b01000111,0b00000000,
    0b00110111,0b00000000,0b00000000,0b00000000,0b00001110,0b00000000,0b00000000,0b00000000,
    0b01100111,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,
    0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000,0b00000000
};
#endif

/// private:

#if defined(SCROLL)
    int scroll = 0;
#endif

/* The array for shifting the data to the devices */
u8 spidata[16];
/* We keep track of the led-status for all 8 devices in this array */
u8 status[64];
 /* Data is shifted out of this pin*/
u8 LEDCONTROL_SPI_MOSI;
/* The clock is signaled on this pin */
u8 LEDCONTROL_SPI_CLK;
/* This one is driven LOW for chip selectzion */
u8 LEDCONTROL_SPI_CS;
/* The maximum number of devices we use */
u8 maxDevices;
/* Send out a single command to the device */
void spiTransfer(u8 matrix, u8 opcode, u8 data);

/// public:

/* 
 * Create a new controler 
 * Params :
 * dataPin		pin on the Arduino where data gets shifted out
 * clockPin		pin for the clock
 * csPin		pin for selecting the device 
 * numDevices	maximum number of devices that can be controled
 */
void LedControl_init(u8 dataPin, u8 clkPin, u8 csPin, u8 numDevices);

/*
 * Gets the number of devices attached to this LedControl.
 * Returns :
 * u8	the number of devices on this LedControl
 */
u8 LedControl_getDeviceCount();

void LedControl_spiTransfer(u8 matrix, volatile u8 opcode, volatile u8 data);

/* 
 * Set the shutdown (power saving) mode for the device
 * Params :
 * matrix	The address of the display to control
 * status	If true the device goes into power-down mode. Set to false
 *		for normal operation.
 */
void LedControl_shutdown(u8 matrix, boolean status);

/* 
 * Set the number of digits (or rows) to be displayed.
 * See datasheet for sideeffects of the scanlimit on the brightness
 * of the display.
 * Params :
 * matrix	address of the display to control
 * limit	number of digits to be displayed (1..8)
 */
void LedControl_setScanLimit(u8 matrix, u8 limit);

/* 
 * Set the brightness of the display.
 * Params:
 * matrix		the address of the display to control
 * intensity	the brightness of the display. (0..15)
 */
void LedControl_setIntensity(u8 matrix, u8 intensity);

/* 
 * Switch all Leds on the display off. 
 * Params:
 * matrix	address of the display to control
 */
void LedControl_clearDisplay(u8 matrix);

void LedControl_clearAll();

/* 
 * Set the status of a single Led.
 * Params :
 * matrix	address of the display 
 * row	the row of the Led (0..7)
 * col	the column of the Led (0..7)
 * state	If true the led is switched on, 
 *		if false it is switched off
 */
void LedControl_setLed(u8 matrix, u8 row, u8 col, boolean state);

/* 
 * Set all 8 Led's in a row to a new state
 * Params:
 * matrix	address of the display
 * row	row which is to be set (0..7)
 * value	each bit set to 1 will light up the
 *		corresponding Led.
 */
void LedControl_setRow(u8 matrix, u8 row, u8 value);

/* 
 * Set all 8 Led's in a column to a new state
 * Params:
 * matrix	address of the display
 * col	column which is to be set (0..7)
 * value	each bit set to 1 will light up the
 *		corresponding Led.
 */
void LedControl_setColumn(u8 matrix, u8 col, u8 value);

/* 
 * Display a hexadecimal digit on a 7-Segment Display
 * Params:
 * matrix	address of the display
 * digit	the position of the digit on the display (0..7)
 * value	the value to be displayed. (0x00..0x0F)
 * dp	sets the decimal point.
 */
void LedControl_setDigit(u8 matrix, u8 digit, u8 value, boolean dp);

/* 
 * Display a character on a 7-Segment display.
 * There are only a few characters that make sense here :
 *	'0','1','2','3','4','5','6','7','8','9','0',
 *  'A','b','c','d','E','F','H','L','P',
 *  '.','-','_',' ' 
 * Params:
 * matrix	address of the display
 * digit	the position of the character on the display (0..7)
 * value	the character to be displayed. 
 * dp	sets the decimal point.
 */
void LedControl_setChar(u8 matrix, u8 digit, char value, boolean dp);

//Returns the array number in the alphabetBitmap array 
//u8 LedControl_getCharArrayPosition(char c);

void LedControl_writeString(const char * displayString);
void LedControl_displayChar(u8 matrix, u8 charIndex);

#if defined(SCROLL)
void LedControl_scroll(const char * displayString);
#endif

#endif	//LedControl.h



