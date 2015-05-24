 /*	----------------------------------------------------------------------------
	FILE:  			lcdlib.c
	PROJECT: 		pinguino8
	PURPOSE: 		LCD routines for use with pinguino board, 
					based on LiquidCrystal lib from Arduino project.
	PROGRAMER: 		Orignal port by Marcus Fazzi (marcus@fazzi.eng.br)
	Updated - 28 Jan 2011 : lcdprintf by RegisBlanchot (rblanchot@gmail.com)
	Updated - 03 Mar 2012 : Library updated, new function names
							: lcdlib.pdl updated
							: LiquidCrystal examples updated
	Updated:		26 May 2012 - M Harper
					Changed to deal more consistently with single line displays
					as included in P32 lcdlib.c at x.3 r363.
					(changes identified by dated comments in code)
 	----------------------------------------------------------------------------
	LiquidCrystal original Arduino site: 
			http://www.arduino.cc/en/Tutorial/LiquidCrystal by David A. Mellis
	Pins, Schematics and more info: 
			http://pinguino.koocotte.org/index.php/LCD_Example
 			http://www.fazzi.eng.br
*/

#ifndef __LCDLIB_C__
#define __LCDLIB_C__

#include <delayms.c>		// Delayms
#include <delayus.c>		// Delayus
#include <digitalw.c>   	// digitalwrite
#include <digitalp.c>   	// pinmode
#include <const.h>   	    // HIGH, LOW, OUTPUT, ...
#include <lcdlib.h>
#ifdef LCDPRINTF
#include <stdio.c>
#include <stdarg.h>
#endif


/** Positive pulse on E */
void lcd_pulseEnable(void)
{
	//digitalwrite(_enable_pin, LOW);
//	Delayus(1);    
	digitalwrite(_enable_pin, HIGH);
//	Delayus(1);    // enable pulse must be >450ns
	digitalwrite(_enable_pin, LOW);
//	Delayus(100);   // commands need > 37us to settle
}

/** Write using 4bits mode */
void lcd_write4bits(u8 value)
{
	u8 i;
	for (i = 0; i < 4; i++)
		digitalwrite(_data_pins[i], (value >> i) & 0x01);
	lcd_pulseEnable();
}

/** Write using 8bits mode */
void lcd_write8bits(u8 value)
{
	u8 i;
	for (i = 0; i < 8; i++)
		digitalwrite(_data_pins[i], (value >> i) & 0x01);
	lcd_pulseEnable();
}

/** Send data to LCD 8 or 4 bits */
void lcd_send(u8 value, u8 mode)
{
	digitalwrite(_rs_pin, mode);
  
	if (_displayfunction & LCD_8BITMODE)
	{
		lcd_write8bits(value); 
	}
	else
	{
		lcd_write4bits(value >> 4);	// Upper 4 bits first
		lcd_write4bits(value);	    // Lower 4 bits second
	}
}

/** Write a data character on LCD */
void lcd_write(u8 value)
{
	lcd_send(value, HIGH);
}

/** Write a control command on LCD */
void lcd_command(u8 value)
{
	lcd_send(value, LOW);
}

/** Setup line x column on LCD */
#ifdef LCDSETCURSOR
void lcd_setCursor(u8 col, u8 row)
{
	u8 row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
	/* Added 26 May 2012 by MFH
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
	/* Removed 26 May 2012 by MFH as did not treat row
		starts consistently for n x 2 and n x 4 displays
	if ( row > _numlines ) 
		row = _numlines-1;    // we count rows starting w/0
	*/
	lcd_command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}
#endif

/** Print a string on LCD */
#ifdef LCDPRINT
void lcd_print(char *string)
{
	u8 i;
	for( i=0; string[i]; i++)
		lcd_write(string[i]);
}
#endif

/** Write formated string on LCD **/
#ifdef LCDPRINTF
//  added 28/01/2011 rblanchot@gmail.com
void lcd_printf(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	pprintf(lcd_write, fmt, args);
	va_end(args);
}
#endif

/** Print a number on LCD */
#if defined(LCDPRINTNUMBER) || defined(LCDPRINTFLOAT)
void lcd_printNumber(u16 n, u8 base)
{  
	u8 buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
	u16 i = 0;

	if (n == 0)
	{
		lcd_write('0');
		return;
	} 

	while (n > 0)
	{
		buf[i++] = n % base;
		n /= base;
	}

	for (; i > 0; i--)
		lcd_write((char) (buf[i - 1] < 10 ? '0' + buf[i - 1] : 'A' + buf[i - 1] - 10));
}
#endif

/** Print a float number to LCD */
#ifdef LCDPRINTFLOAT
void lcd_printFloat(float number, u8 digits)
{ 
	u8 i, toPrint;
	u16 int_part;
	float rounding, remainder;

	// Handle negative numbers
	if (number < 0.0)
	{
		lcd_write('-');
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
	lcd_printNumber(int_part, 10);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		lcd_write('.'); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
		lcd_printNumber(toPrint, 10);
		remainder -= toPrint; 
	}
}
#endif

/** Move cursor to Home position */
#ifdef LCDHOME
void lcd_home()
{
	lcd_command(LCD_RETURNHOME);
Delayms(2);									// Wait for more than 4.1 ms
//	Delayus(2000);
}
#endif

/** Clear LCD */
#ifdef LCDCLEAR
void lcd_clear()
{
    lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    Delayms(2);									// Wait for more than 4.1 ms
    //	Delayus(2000);  // this command takes a long time!
}
#endif

/** Turn the display on/off (quickly) */
#ifdef LCDNODISPLAY
void lcd_noDisplay()
{
	_displaycontrol &= ~LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
#endif

#ifdef LCDDISPLAY
void lcd_display()
{
	_displaycontrol |= LCD_DISPLAYON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
#endif

/** Turns the underline cursor on/off */
#ifdef LCDNOCURSOR
void lcd_noCursor()
{
	_displaycontrol &= ~LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
#endif

#ifdef LCDCURSOR
void lcd_cursor()
{
	_displaycontrol |= LCD_CURSORON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
#endif

/** Turn on and off the blinking cursor */
#ifdef LCDNOBLINK
void lcd_noBlink()
{
	_displaycontrol &= ~LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
#endif

#ifdef LCDBLINK
void lcd_blink()
{
	_displaycontrol |= LCD_BLINKON;
	lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);
}
#endif

/** These commands scroll the display without changing the RAM */
#ifdef LCDSCROLLDISPLAYLEFT
void lcd_scrollDisplayLeft(void)
{
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}
#endif

#ifdef LCDSCROLLDISPLAYRIGHT
void lcd_scrollDisplayRight(void)
{
	lcd_command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}
#endif

/** This is for text that flows Left to Right */
#ifdef LCDLEFTTORIGHT
void lcd_leftToRight(void)
{
	_displaymode |= LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}
#endif

/** This is for text that flows Right to Left */
#ifdef LCDRIGHTTOLEFT
void lcd_rightToLeft(void)
{
	_displaymode &= ~LCD_ENTRYLEFT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}
#endif

/** This will 'right justify' text from the cursor */
#ifdef LCDAUTOSCROLL
void lcd_autoscroll(void) {
	_displaymode |= LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}
#endif

/** This will 'left justify' text from the cursor */
#ifdef LCDNOAUTOSCROLL
void lcd_noAutoscroll(void) {
	_displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
	lcd_command(LCD_ENTRYMODESET | _displaymode);
}
#endif

/** Initial Display settings! */
void lcd_begin(u8 lines, u8 dotsize)
{
    if (lines > 1)
        _displayfunction |= LCD_2LINE;

    _numlines = lines;
    _currline = 0;

    // Some one line displays can select 10 pixel high font
    if ((dotsize != 0) && (lines == 1))
        _displayfunction |= LCD_5x10DOTS;

    Delayms(15);								// Wait more than 15 ms after VDD rises to 4.5V

    // Now we pull both RS and R/W low to begin commands
    digitalwrite(_rs_pin, LOW);
    digitalwrite(_enable_pin, LOW);

    //put the LCD into 4 bit mode
    if (! (_displayfunction & LCD_8BITMODE) )
    {
        // this is according to the hitachi HD44780 datasheet p46, figure 24

        // we start in 8bit mode, try to set 4 bit mode
        lcd_write4bits(0x03);
        Delayms(5);									// Wait for more than 4.1 ms
        // second try
        lcd_write4bits(0x03);
        Delayus(150);								// Wait more than 100 μs
        // third go!
        lcd_write4bits(0x03); 
        Delayus(150);								// Wait more than 100 μs
        // finally, set to 8-bit interface
        lcd_write4bits(0x02); 
    }
    //put the LCD into 8 bit mode
    else
    {
        // this is according to the hitachi HD44780 datasheet p45, figure 23

        // Send function set command sequence
        lcd_command(LCD_FUNCTIONSET | _displayfunction);
        Delayms(5);									// Wait for more than 4.1 ms

        // second try
        lcd_command(LCD_FUNCTIONSET | _displayfunction);
        Delayms(5);									// Wait for more than 4.1 ms
        //		Delayus(150);

        // third go
        lcd_command(LCD_FUNCTIONSET | _displayfunction);
    }

    // finally, set # lines, font size, etc.
    lcd_command(LCD_FUNCTIONSET | _displayfunction);  

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;  
    lcd_command(LCD_DISPLAYCONTROL | _displaycontrol);

    // clear it off
    lcd_command(LCD_CLEARDISPLAY);  // clear display, set cursor position to zero
    Delayms(2);

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    lcd_command(LCD_ENTRYMODESET | _displaymode);
}

/** Init LCD 
 * mode 	=> 1 => 4 bits // 0 => 8 bits
 * rs , rw, enable
 * pins => D0 ~ D7.
 */
void lcd_init(u8 fourbitmode, u8 rs, u8 rw, u8 enable, 
			u8 d0, u8 d1, u8 d2, u8 d3,
			u8 d4, u8 d5, u8 d6, u8 d7)
{
	u8 i;

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

	if (fourbitmode)
	{
		_displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
		for (i = 0; i < 4; i++)
			pinmode(_data_pins[i], OUTPUT);
	}
	else
	{
		_displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
		for (i = 0; i < 8; i++)
			pinmode(_data_pins[i], OUTPUT);
	}
}

/** LCD 8 bits mode */
void lcd_pins(u8 rs, u8 enable, u8 d0, u8 d1, u8 d2, u8 d3, u8 d4, u8 d5, u8 d6, u8 d7)
{
	lcd_init(((d4 + d5 + d6 + d7)==0), rs, -1, enable, d0, d1, d2, d3, d4, d5, d6, d7);
}

#endif
