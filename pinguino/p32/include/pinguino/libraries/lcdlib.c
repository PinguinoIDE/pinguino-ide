/*	----------------------------------------------------------------------------
	FILE:  			lcdlib.c
	PROJECT: 		pinguino32
	PURPOSE: 		LCD routines for use with pinguino 32X board
						Based on LiquidCrystal lib from Arduino project.
	PROGRAMER: 		Port by Marcus Fazzi (anunakin@ieee.org)
	FIRST RELEASE:	30 May 2011
	Updated:		05 Mar 2012 - Marcus Fazzi
					Changed function lcd to _lcd_pins & prefixed all other
					function names with _lcd_
	Updated:		29 Apr 2012 - R. Blanchot
					Changed _lcd_begin() to get same syntax as Arduino's lib.
					Changed variable types unsigned char and unsigned long
					to Pinguino types u8 and u16 respectively and added
					#include <typedef.h>
	Updated:		01 May 2012 - M Harper
					Changed to deal more consistently with single line displays
						(changes identified by dated comments in code)
	Updated:		26 May 2012 - M Harper
					Reverted _lcd_begin() to syntax prior to x.3 r359 29 Apr 2012
					for consistency with P8 and fix broken LCD examples
	----------------------------------------------------------------------------
	LiquidCrystal original Arduino site: 
			http://www.arduino.cc/en/Tutorial/LiquidCrystal by David A. Mellis
	Pins, Schematics and more info: 
			http://pinguino.koocotte.org/index.php/LCD_Example
 			http://www.fazzi.eng.br
*/

#ifndef __LCDLIB_C__
#define __LCDLIB_C__

#include <delay.c>		// Arduino like delays
#include <digitalw.c>	// Arduino like DigitalWrite and Read
#include <typedef.h>	// Pinguino's types (u8, u16, ...) 01 May 2012
#include <printf.c>		// Pinguino printf (low footprint)
#include <stdarg.h>		// Variable Arguments

#ifndef __LCDLIB_H__
#include <lcdlib.h>
#endif

/** Positive pulse on E */
void _lcd_pulseEnable(void) {
  digitalwrite(_enable_pin, LOW);
  Delayus(1);
  digitalwrite(_enable_pin, HIGH);
  Delayus(1);    // enable pulse must be >450ns
  digitalwrite(_enable_pin, LOW);
  Delayus(50);   // commands need > 37us to settle
}

/** Write using 4bits mode */
void _lcd_write4bits(u8 value) {
	int i;
	for (i = 0; i < 4; i++) {		
		digitalwrite(_data_pins[i], (value >> i) & 0x01);
	}
	_lcd_pulseEnable();
}

/** Write using 8bits mode */
void _lcd_write8bits(u8 value) {
	int i;
	for (i = 0; i < 8; i++) {		
		digitalwrite(_data_pins[i], (value >> i) & 0x01);
	}
	_lcd_pulseEnable();
}

/** Send data to LCD 8 or 4 bits */
void _lcd_send(u8 value, u8 mode) {
	digitalwrite(_rs_pin, mode);
    if (_displayfunction & LCD_8BITMODE) {
		_lcd_write8bits(value);
	} else {
		_lcd_write4bits(value>>4);
		_lcd_write4bits(value);
	}
}

/** Write a data character on LCD */
void _lcd_write(u8 value) {
	_lcd_send(value, HIGH);
}

 
/** Write a control command on LCD */
void _lcd_command(u8 value) {
	_lcd_send(value, LOW);
}

/** Setup line x column on LCD */
void _lcd_setCursor(u8 col, u8 row) {
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	/* Added 01 May 2012 by MFH
		sets row_offsets for a single line display so that 
		80 column space divided in 4 equal 20 column sections.
		This means that if an n x 4 display is set to behave as
		a single line display lines 1 and 2 are displayed and
		lines 3 and 4 are 20 characters to the right.*/
	if (_numlines==1) {
		row_offsets[1] = 0x14;
		row_offsets[2] = 0x28;
		row_offsets[3] = 0x3C;
		}
	/* Removed 01 May 2012 by MFH as did not treat row
		starts consistently for n x 2 and n x 4 displays
	if ( row > _numlines ) {
		row = _numlines-1;    // we count rows starting w/0
	}
*/
	_lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

/** Print a string on LCD */
void _lcd_print(char *string) {
	int i;
	for( i=0; string[i]; i++) {
		_lcd_write(string[i]);
	}
}

/** Write formated string on LCD **/
//  added 23/02/2011 rblanchot@gmail.com
void _lcd_printf(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	pprintf(_lcd_write, fmt, args);
	va_end(args);
}

/** Print a number on LCD */
void _lcd_printNumber(u32 n, u8 base) {  
  u8 buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
  u32 i = 0;

  if (n == 0) {
    _lcd_write('0');
    return;
  } 

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    _lcd_write((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}

/** Print a float number to LCD */
void _lcd_printFloat(float number, u8 digits) { 
  u8 i, toPrint;
  u32 int_part;
  float rounding, remainder;
  
  // Handle negative numbers
  if (number < 0.0) {
     _lcd_write('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"  
  rounding = 0.5;
  for (i=0; i<digits; ++i)
    rounding /= 10.0;
  
  number += rounding;

  // Extract the integer part of the number and print it  
  int_part = (u32)number;
  remainder = number - (float)int_part;
  _lcd_printNumber(int_part, 10);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    _lcd_write('.'); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    toPrint = (u16)remainder;	//Integer part without use of math.h lib, 
								//I think better! (Fazzi)
    _lcd_printNumber(toPrint, 10);
    remainder -= toPrint; 
  }
}

/** Move cursor to Home position */
void _lcd_home(){
	_lcd_command(LCD_RETURNHOME);
	Delayus(2000);
}

/** Clear LCD */
void _lcd_clear() {
  _lcd_command(LCD_CLEARDISPLAY);	// clear display, set cursor position to zero
  Delayus(2000);  					// this command takes a long time!
}

/** Turn the display on/off (quickly) */
void _lcd_noDisplay() {
  _displaycontrol &= ~LCD_DISPLAYON;
  _lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void _lcd_display() {
  _displaycontrol |= LCD_DISPLAYON;
  _lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/** Turns the underline cursor on/off */
void _lcd_noCursor() {
  _displaycontrol &= ~LCD_CURSORON;
  _lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void _lcd_cursor() {
  _displaycontrol |= LCD_CURSORON;
  _lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/** Turn on and off the blinking cursor */
void _lcd_noBlink() {
  _displaycontrol &= ~LCD_BLINKON;
  _lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
void _lcd_blink() {
  _displaycontrol |= LCD_BLINKON;
  _lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}

/** These commands scroll the display without changing the RAM */
void _lcd_scrollDisplayLeft(void) {
  _lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
void _lcd_scrollDisplayRight(void) {
  _lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

/** This is for text that flows Left to Right */
void _lcd_leftToRight(void) {
  _displaymode |= LCD_ENTRYLEFT;
  _lcd_command(LCD_ENTRYMODESET | _displaymode);
}

/** This is for text that flows Right to Left */
void _lcd_rightToLeft(void) {
  _displaymode &= ~LCD_ENTRYLEFT;
  _lcd_command(LCD_ENTRYMODESET | _displaymode);
}

/** This will 'right justify' text from the cursor */
void _lcd_autoscroll(void) {
  _displaymode |= LCD_ENTRYSHIFTINCREMENT;
  _lcd_command(LCD_ENTRYMODESET | _displaymode);
}

/** This will 'left justify' text from the cursor */
void _lcd_noAutoscroll(void) {
  _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
  _lcd_command(LCD_ENTRYMODESET | _displaymode);
}

/** Initial Display settings! */
void _lcd_begin(u8 lines, u8 dotsize) {
  if (lines > 1) {
    _displayfunction |= LCD_2LINE;
  }
  _numlines = lines;
  _currline = 0;

  // Some one line displays can select 10 pixel high font
  if ((dotsize != 0) && (lines == 1)) {
    _displayfunction |= LCD_5x10DOTS;
  }

  Delayus(15); //Pinguino needs it? long delay on startup time!
  // Now we pull both RS and R/W low to begin commands
  digitalwrite(_rs_pin, LOW);
  digitalwrite(_enable_pin, LOW);
  
  //put the LCD into 4 bit or 8 bit mode
  if (! (_displayfunction & LCD_8BITMODE)) {
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    _lcd_write4bits(0x03);
    Delayus(4500); // wait min 4.1ms

    // second try
    _lcd_write4bits(0x03);
    Delayus(4500); // wait min 4.1ms
    
    // third go!
    _lcd_write4bits(0x03); 
    Delayus(150);

    // finally, set to 8-bit interface
    _lcd_write4bits(0x02); 
  } else {
    // this is according to the hitachi HD44780 datasheet
    // page 45 figure 23

    // Send function set command sequence
    _lcd_command(LCD_FUNCTIONSET | _displayfunction);
    Delayus(4500);  // wait more than 4.1ms

    // second try
    _lcd_command(LCD_FUNCTIONSET | _displayfunction);
    Delayus(150);

    // third go
    _lcd_command(LCD_FUNCTIONSET | _displayfunction);
  }

  // finally, set # lines, font size, etc.
  _lcd_command(LCD_FUNCTIONSET | _displayfunction);  

  // turn the display on with no cursor or blinking default
  _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
  _lcd_display();

  // clear it off
  _lcd_clear();

  // Initialize to default text direction (for romance languages)
  _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
  // set the entry mode
  _lcd_command(LCD_ENTRYMODESET | _displaymode);

}

/** Init LCD 
 * mode 	=> 1 => 4 bits // 0 => 8 bits
 * rs , rw, enable
 * pins => D0 ~ D7.
 */
void _lcd_init(u8 fourbitmode, u8 rs, u8 rw, u8 enable, 
			u8 d0, u8 d1, u8 d2, u8 d3,
			u8 d4, u8 d5, u8 d6, u8 d7){
  int i;
  _rs_pin = rs;
  _rw_pin = rw;
  _enable_pin = enable;
  
  _data_pins[0] = d0;
  _data_pins[1] = d1;
  _data_pins[2] = d2;
  _data_pins[3] = d3; 
  _data_pins[4] = d4;
  _data_pins[5] = d5;
  _data_pins[6] = d6;
  _data_pins[7] = d7; 

  pinmode(_rs_pin, OUTPUT);
  
  pinmode(_enable_pin, OUTPUT);
  
  if (fourbitmode){
	_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
	for (i = 0; i < 4; i++) {
		pinmode(_data_pins[i], OUTPUT);
		}
  }
  else {
    _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
    for (i = 0; i < 8; i++) {
		pinmode(_data_pins[i], OUTPUT);
	}
  }
  
}

/** LCD 8 bits mode */
void _lcd_pins(u8 rs, u8 enable, u8 d0, u8 d1, u8 d2, u8 d3, 
			u8 d4, u8 d5, u8 d6, u8 d7) {
	
	_lcd_init(((d4 + d5 + d6 + d7)==0), rs, 99, enable, d0, d1, d2, d3, d4, d5, d6, d7);
	
}
#endif
