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
http://mbed.org/users/Byrn/code/SSD1306/file/1d9df877c90a/ssd1306.cpp
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

/**
Pin Assignment

1   VCC     |   2       GND
3   CS      |   4       RES
5   D/C     |   6       R/W
7   E/RD    |   8       D0
9   D1      |   10      D2
11  D3      |   12      D4
13  D5      |   14      D6
15  D7      |   16      NC

**/

/**
Bus Interface

        I2C     6800    8080    4-SPI   3-SPI
BS0     0       0       0       0       1
BS1     1       0       1       0       0
BS2     0       1       1       0       0

**/

#ifndef __SSD1306_C
#define __SSD1306_C

#include <SSD1306.h>

// Screen buffer
#if (DISPLAY & SSD1306_128X64)
    #include <logo/pinguino128x64.h>
#else
    //#include <logo/pinguino128x32.h>        // to do
    //#pragma udata largeram logo
    u8 logo[128*4];
#endif

// Pinguino standards
//#include <string.h>
#include <digitalw.c>
#include <typedef.h>
#include <const.h>
#include <macro.h>
#include <delayms.c>
#include <delayus.c>

// Printf
#ifdef SSD1306PRINTF
    #include <stdarg.h>
    #ifdef __PIC32MX__
        #include <printf.c>
    #else
        #include <stdio.c>
    #endif
#endif

// Graphics Library
#ifdef SSD1306GRAPHICS
    #include <graphics.c>
#endif

/*
#if defined(SSD1306DRAWLINE)   || defined(SSD1306DRAWLINE) || \
    defined(SSD1306DRAWCIRCLE) || defined(SSD1306FILLCIRCLE)
    #include <graphics.c>
#endif
*/

#if   (DISPLAY & SSD1306_SPI3) || (DISPLAY & SSD1306_SPI4)
    #include <spi.c>
#elif (DISPLAY & SSD1306_I2C)
    #include <i2c.c>
#elif (DISPLAY & SSD1306_6800) || (DISPLAY & SSD1306_8080)
    #if (DISPLAY & SSD1306_PMP)
        #include <pmp.c>
    #endif
#endif

///	--------------------------------------------------------------------
/// Globals
///	--------------------------------------------------------------------

u8 *SSD1306_buffer = logo;
//*SSD1306_bufferptr = SSD1306_buffer;

///	--------------------------------------------------------------------
/// Core functions
///	--------------------------------------------------------------------

/**
        The parallel 6800-series interface consists of :
        * 8 bi-directional data pins (D[7:0])
        * R/W#, D/C#, E and CS#.
        A LOW in R/W# indicates WRITE operation
        A HIGH in R/W# indicates READ operation.
        A LOW in D/C# indicates COMMAND read/write
        A HIGH in D/C# indicates DATA read/write.
        The E input serves as data latch signal while CS# is LOW.
        Data is latched at the falling edge of E signal.

        The parallel 8080-series interface consists of :
        * 8 bi-directional data pins (DB[7:0])
        * RD#, WR#, DC# and CS#.
        A LOW  in DC# indicates COMMAND read/write
        A HIGH in DC# indicates DATA read/write.
        A rising edge of RD# input serves as a data READ latch signal while CS# is kept LOW.
        A rising edge of WR# input serves as a data/command WRITE latch signal while CS# is kept LOW.
**/

void SSD1306_sendCommand(u8 val)
{
    #if (DISPLAY & SSD1306_PMP)

	    #if ((DISPLAY & SSD1306_6800) || (DISPLAY & SSD1306_8080))

            PMP_wait();             // wait for PMP to be available
            PMADDRH = 0;
            PMADDRL = 0;
            PMDIN1L = val;
            
        #else
        
            #error " --------------------------------------- "
            #error " The Parallel Port can only drive the    "
            #error " 6800 or 8080 parallel interface.        "
            #error " --------------------------------------- "

        #endif

	#else

        #if (DISPLAY & SSD1306_8080)

		    Low(CS);            // Chip select
		    Low(DC);            // COMMAND read/write

		    Low(WR);
		    DATA = val;         // WR should stay low at least 60ns
		    High(WR);           // A rising edge of WR enables Write mode

		    High(CS);           // Chip deselected

        #elif (DISPLAY & SSD1306_6800)

		    Low(CS);            // Chip select
		    Low(WR);            // WRITE
		    Low(DC);            // COMMAND

		    High(E);
		    DATA = val;         // WR should stay low at least 60ns
		    Low(E);             // A rising edge of WR enables Write mode

		    High(CS);           // Chip deselected

        #elif (DISPLAY & SSD1306_I2C)

            I2C_start();
            if (I2C_write(SSD1306_i2c_address | I2C_WRITE))
            {
                I2C_write(0x00); // Co = 0, D/C = 0
                I2C_write(val);
            }
            I2C_stop();

        #endif

    #endif
}

void SSD1306_sendData(u8 val)
{
    #if (DISPLAY & SSD1306_PMP)

        #if ((DISPLAY & SSD1306_6800) || (DISPLAY & SSD1306_8080))

            PMP_wait();     // wait for PMP to be available
            PMADDRH = 0;    // High8(DC);
            PMADDRL = DC;   // low8(DC);
            PMDIN1L = val;
            
        #else
        
            #error " --------------------------------------- "
            #error " The Parallel Port can only drive the    "
            #error " 6800 or 8080 parallel interface.        "
            #error " --------------------------------------- "

        #endif

    #else

		#if (DISPLAY & SSD1306_8080)

		    Low(CS);            // Chip select
		    High(DC);           // DATA read/write

		    Low(WR);
		    DATA = val;         // WR should stay low at least 60ns
		    High(WR);           // A rising edge of WR enables Write mode

		    High(CS);           // Chip deselected

		#elif (DISPLAY & SSD1306_6800)

		    Low(CS);            // Chip select
		    Low(WR);            // WRITE
		    High(DC);           // DATA

		    High(E);
            DATA = val;         // WR should stay low at least 60ns
		    Low(E);             // A rising edge of WR enables Write mode

		    High(CS);           // Chip deselected

        #elif (DISPLAY & SSD1306_I2C)

            I2C_start();
            if (I2C_write(SSD1306_i2c_address | I2C_WRITE))
            {
                I2C_write(0x40); // Co = 0, D/C = 1
                I2C_write(val);
            }
            I2C_stop();

        #endif

    #endif
}

///	--------------------------------------------------------------------
/// Basic functions (TODO : replace functions with #define) 
///	--------------------------------------------------------------------

void SSD1306_displayOff()
{
    SSD1306_sendCommand(0xAE);
}

void SSD1306_displayOn()
{
    SSD1306_sendCommand(0xAF);
}

void SSD1306_sleep()
{
    SSD1306_sendCommand(0xAE);
}

void SSD1306_wake()
{
    SSD1306_sendCommand(0xAF);
}

void SSD1306_setInverse(u8 value)
{
    SSD1306_sendCommand(value ? 0xA7 : 0xA6);
}

void SSD1306_setDisplayOffset(u8 value)
{
    SSD1306_sendCommand(0xD3);
    SSD1306_sendCommand(value & 0x3F); 
}

void SSD1306_setContrast(u8 value) 
{
    SSD1306_sendCommand(0x81);
    SSD1306_sendCommand(value);
}

void SSD1306_setDisplayStartLine(u8 value)
{
    SSD1306_sendCommand(0x40 | value);
}

void SSD1306_setSegmentRemap(u8 value)
{
    SSD1306_sendCommand(value ? 0xA1 : 0xA0);
}

void SSD1306_setMultiplexRatio(u8 value)
{
    SSD1306_sendCommand(0xA8);
    SSD1306_sendCommand(value & 0x3F);
}

void SSD1306_setComOutputScanDirection(u8 value)
{
    SSD1306_sendCommand(value ? 0xC8 : 0xC0);
}

void SSD1306_setComPinsHardwareConfiguration(u8 sequential, u8 lr_remap)
{
    SSD1306_sendCommand(0xDA);
    SSD1306_sendCommand(0x02 | ((sequential & 1) << 4) | ((lr_remap & 1) << 5));
}

void SSD1306_pamSetStartAddress(u8 address)
{
    // "Set Lower Column Start Address for Page Addressing Mode"
    SSD1306_sendCommand(address & 0x0F);

    // "Set Higher Column Start Address for Page Addressing Mode"
    SSD1306_sendCommand((address << 4) & 0x0F);
}

void SSD1306_setMemoryAddressingMode(u8 mode)
{
    SSD1306_sendCommand(0x20);
    SSD1306_sendCommand(mode & 0x3);
}

void SSD1306_hvSetColumnAddress(u8 start, u8 end)
{
    SSD1306_sendCommand(0x21);
    SSD1306_sendCommand(start & DISPLAY_WIDTH_MASK);
    SSD1306_sendCommand(end & DISPLAY_WIDTH_MASK);
}

void SSD1306_hvSetPageAddress(u8 start, u8 end)
{
    SSD1306_sendCommand(0x22);
    SSD1306_sendCommand(start & DISPLAY_ROW_MASK);
    SSD1306_sendCommand(end & DISPLAY_ROW_MASK);
}

void SSD1306_pamSetPageStart(u8 address)
{
    SSD1306_sendCommand(0xB0 | (address & DISPLAY_ROW_MASK));
}

void SSD1306_setDisplayClockRatioAndFrequency(u8 ratio, u8 frequency)
{
    SSD1306_sendCommand(0xD5);
    SSD1306_sendCommand((ratio & 0x0F) | ((frequency & 0x0F) << 4));
}

void SSD1306_setPrechargePeriod(u8 phase1, u8 phase2)
{
    SSD1306_sendCommand(0xD9);
    SSD1306_sendCommand((phase1 & 0x0F) | ((phase2 & 0x0F ) << 4));
}

void SSD1306_setVcomhDeselectLevel(u8 level)
{
    SSD1306_sendCommand(0xDB);
    SSD1306_sendCommand((level & 0x03) << 4);
}

void SSD1306_nop()
{
    SSD1306_sendCommand(0xE3);
}

void SSD1306_setChargePumpEnable(u8 enable)
{
    SSD1306_sendCommand(0x8D);
    SSD1306_sendCommand(enable ? 0x14 : 0x10);
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Initialize the graphical display
    PARAMETERS:
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

#if   (DISPLAY & SSD1306_6800) || (DISPLAY & SSD1306_8080)

    #if   (DISPLAY & SSD1306_PMP)

        void SSD1306_init(u8 res, u16 dc)
        {
            DC = dc;
            digitalwrite(res, HIGH);
            pinmode(res, OUTPUT);               // output

            #if   (DISPLAY & SSD1306_6800)
            PMP_setMode(PMP_MODE_MASTER1);      // Master mode 1 : PMCS1, PMRD/PMWR, PMENB 
            PMP_setControl(PMRD);               // PMRD-PMWR as WR
            #elif (DISPLAY & SSD1306_8080)
            PMP_setMode(PMP_MODE_MASTER2);      // Master mode 2 : PMCS1, PMRD, PMWR
            PMP_setControl(PMWR);               // PMWR as WR
            #endif
            PMP_setWidth(PMP_MODE_8BIT);        // PMD<7:0>
            PMP_setMux(PMP_MUX_OFF);            // PMA and PMD on separate lines
            PMP_setPolarity(PMP_ACTIVE_LOW);
            PMP_setAddress(DC);                 // DC
            PMP_autoIncrement(0);
            PMP_setWaitStates(4, 16, 4);        // WR strobe must be at least 60ns
            PMP_init();

    #else

        void SSD1306_init(u8 res, u16 dc, u8 d0, u8 d1, u8 d2, u8 d3, u8 d4, u8 d5, u8 d6, u8 d7)
        {
            dDATA = 0x00;           // output
            dCMD  = 0x00;           // output
            DATA  = 0x00;
            CMD   = 0xFF;           // every pin HIGH
            

    #endif

#elif (DISPLAY & SSD1306_SPI3) || (DISPLAY & SSD1306_SPI4)

    // TODO

#elif (DISPLAY & SSD1306_I2C)

        void SSD1306_init(u8 addr)
        {
            SSD1306_i2c_address = addr;
            I2C_init(I2C_MASTER_MODE, I2C_100KHZ);

#endif

    // default Screen Values

    //SSD1306.orientation   = PORTRAIT;
    SSD1306.cursor.x      = 0;
    SSD1306.cursor.y      = 0;
    SSD1306.cursor.page   = 0;
    SSD1306.screen.startx = 0;
    SSD1306.screen.starty = 0;
    SSD1306.screen.endx   = DISPLAY_WIDTH  - 1;
    SSD1306.screen.endy   = DISPLAY_HEIGHT - 1;
    SSD1306.screen.width  = DISPLAY_WIDTH;
    SSD1306.screen.height = DISPLAY_HEIGHT;

    /** reset device
    When RES input is low, the chip is initialized with the following status:
        1. Display is OFF.
        2. 128x64 MUX Display Mode.
        3. Normal segment and display data column address and row address mapping
           (SEG0 mapped to address 00h and COM0 mapped to address 00h).
        4. Shift register data clear in serial interface.
        5. Display start line is set at display RAM address 0.
        6. Column address counter is set at 0.
        7. Normal scan direction of the COM outputs.
        8. Contrast control register is set at 7Fh.
        9. Normal display mode (Equivalent to A4h command)
    **/

    #if !(DISPLAY & SSD1306_I2C)
    digitalwrite(res, LOW);    // initialized the chip
    Delayus(50);               // for at least 3us
    digitalwrite(res, HIGH);
    #endif

    SSD1306_displayOn();
    Delayms(100);               // wait for SEG/COM to be ON
    SSD1306_displayOff();

    SSD1306_setDisplayClockRatioAndFrequency(0, 8);
    SSD1306_setMultiplexRatio(0x3F); // 1/64 duty
    SSD1306_setPrechargePeriod(0xF, 0x01);
    SSD1306_setDisplayOffset(0);    
    SSD1306_setDisplayStartLine(0);  
    SSD1306_setChargePumpEnable(1);    
    SSD1306_setMemoryAddressingMode(0); // horizontal addressing mode; across then down
    SSD1306_setSegmentRemap(1);
    SSD1306_setComOutputScanDirection(1);
    SSD1306_setComPinsHardwareConfiguration(1, 0);
    SSD1306_setContrast(0xFF);
    SSD1306_setVcomhDeselectLevel(1);

    SSD1306_wake();
    SSD1306_setInverse(0);

    SSD1306_hvSetColumnAddress(0, 127);
    SSD1306_hvSetPageAddress(0, 7);

    SSD1306_pamSetStartAddress(0);
    SSD1306_pamSetPageStart(0);

    // SSD1306_setPrechargePeriod(2, 2);
}

///	--------------------------------------------------------------------
/// Update the display
///	--------------------------------------------------------------------

void SSD1306_refresh()
{
    u16 i;

    SSD1306_hvSetColumnAddress(0, DISPLAY_WIDTH-1);
    SSD1306_hvSetPageAddress(0, DISPLAY_ROWS-1);

    for (i = 0; i < DISPLAY_SIZE; i++)
        SSD1306_sendData(SSD1306_buffer[i]);
}

///	--------------------------------------------------------------------
/// Clear the display
///	--------------------------------------------------------------------

void SSD1306_clearScreen(void)
{
    u16 i;

    for (i = 0; i < DISPLAY_SIZE; i++)
        SSD1306_buffer[i] = 0;
        
    SSD1306.cursor.x    = 0;
    SSD1306.cursor.y    = 0;
    SSD1306.cursor.page = 0;
}

///	--------------------------------------------------------------------
/// Scroll functions
///	--------------------------------------------------------------------

void SSD1306_startHorizontalScroll(u8 direction, u8 start, u8 end, u16 interval) 
{
    // Before issuing this command the horizontal scroll must be deactivated (2Eh).
    // Otherwise, RAM content may be corrupted.
    // RB : this datasheet recommandation doesn't work
    //SSD1306_sendCommand(SSD1306_DEACTIVATE_SCROLL);

    SSD1306_sendCommand(direction);// ? 0x27 : 0x26);
    // Dummy byte (???)
    SSD1306_sendCommand(0x00);
    // Start page
    SSD1306_sendCommand(start & DISPLAY_ROW_MASK);

    // Time interval between each scroll step
    switch (interval)
    {
        case   2: SSD1306_sendCommand(0x07); break; // 111b
        case   3: SSD1306_sendCommand(0x04); break; // 100b
        case   4: SSD1306_sendCommand(0x05); break; // 101b
        case   5: SSD1306_sendCommand(0x00); break; // 000b
        case  25: SSD1306_sendCommand(0x06); break; // 110b
        case  64: SSD1306_sendCommand(0x01); break; // 001b
        case 128: SSD1306_sendCommand(0x02); break; // 010b
        case 256: SSD1306_sendCommand(0x03); break; // 011b
        default:  SSD1306_sendCommand(0x07); break; // default to 2 frame interval
    }
    
    // End page
    //SSD1306_sendCommand(0x00);
    SSD1306_sendCommand(end & DISPLAY_ROW_MASK);

    // ???
    SSD1306_sendCommand(0x00);
    SSD1306_sendCommand(0xFF);

    // activate scroll
    SSD1306_sendCommand(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_startVerticalAndHorizontalScroll(u8 direction, u8 start, u8 end, u16 interval, u8 vertical_offset)
{
    // Before issuing this command the horizontal scroll must be deactivated (2Eh).
    // Otherwise, RAM content may be corrupted.
    // RB : this datasheet recommandation doesn't work
    //SSD1306_sendCommand(SSD1306_DEACTIVATE_SCROLL);

    SSD1306_sendCommand(direction); // ? 0x2A : 0x29);
    // Dummy byte (???)
    SSD1306_sendCommand(0x00);
    // Start page
    SSD1306_sendCommand(start & DISPLAY_ROW_MASK);

    // Time interval between each scroll step
    switch (interval)
    {
        case   2: SSD1306_sendCommand(0x07); break; // 111b
        case   3: SSD1306_sendCommand(0x04); break; // 100b
        case   4: SSD1306_sendCommand(0x05); break; // 101b
        case   5: SSD1306_sendCommand(0x00); break; // 000b
        case  25: SSD1306_sendCommand(0x06); break; // 110b
        case  64: SSD1306_sendCommand(0x01); break; // 001b
        case 128: SSD1306_sendCommand(0x02); break; // 010b
        case 256: SSD1306_sendCommand(0x03); break; // 011b
        default:  SSD1306_sendCommand(0x07); break; // default to 2 frame interval
    }

    // End page
    SSD1306_sendCommand(end & DISPLAY_ROW_MASK);

    SSD1306_sendCommand(vertical_offset);    

    // activate scroll
    SSD1306_sendCommand(SSD1306_ACTIVATE_SCROLL);
}

void SSD1306_stopScroll()
{
    // all scroll configurations are removed from the display when executing this command.
    SSD1306_sendCommand(SSD1306_DEACTIVATE_SCROLL);
}

// Right handed 1-row scroll
// The display is 16 rows tall.
void SSD1306_scrollRight()
{
    SSD1306_startHorizontalScroll(SSD1306_RIGHT_HORIZONTAL_SCROLL, 0, 7, 2);
}

// Left handed 1-row scroll
// The display is 16 rows tall
void SSD1306_scrollLeft()
{
    SSD1306_startHorizontalScroll(SSD1306_LEFT_HORIZONTAL_SCROLL, 0, 7, 2);
}

// Up handed 1-row scroll
// The display is 16 rows tall.
void SSD1306_scrollUp()
{
    u8 x, y;

    for (y = 1; y <= 7; y++)
    {
        for (x = 0; x < 128; x++)
        {
            SSD1306_buffer[x + 128 * (y - 1)] = SSD1306_buffer[x + 128 * y];
        }
    }

    for (x = 0; x < 128; x++)
    {
        SSD1306_buffer[x + 128 * 7] = 0;
    }    

    SSD1306.cursor.y--;
}

// Down handed 1-row scroll
// The display is 16 rows tall
void SSD1306_scrollDown()
{
    SSD1306_startVerticalAndHorizontalScroll(SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL, 0, 64, 0, 1);
}

///	--------------------------------------------------------------------
/// Print functions
///	--------------------------------------------------------------------

void SSD1306_setFont(const u8 *font)
{
    SSD1306.font.address = font;
    //SSD1306.font.width   = SSD1306_getFontWidth()+1;
    //SSD1306.font.height  = SSD1306_getFontHeight();
    SSD1306.font.width   = font[0];
    SSD1306.font.height  = font[1];
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        write a char at (SSD1306.cursor.x, SSD1306.cursor.y)
    PARAMETERS:
        * c ascii code of the character to print
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

void SSD1306_printChar(u8 c)
{
    u8  b;

    while (SSD1306.cursor.x >= (DISPLAY_WIDTH / SSD1306.font.width))
    {
        SSD1306.cursor.x -= (DISPLAY_WIDTH / SSD1306.font.width);
        SSD1306.cursor.y++;            
    }

    while (SSD1306.cursor.y > 7) // replace by SSD1306.font.height ???
    {
        SSD1306_scrollUp();            
    }

    switch (c)
    {
        case '\n':
            SSD1306.cursor.y++;
            break;
            
        case '\r':
            SSD1306.cursor.x = 0;
            break;
            
        case '\t':
            SSD1306.cursor.x = (SSD1306.cursor.x + 4) % 4;
            break;
            
        default:
            for (b = 0; b < SSD1306.font.width; b++)
            {
                SSD1306_buffer[SSD1306.cursor.x * SSD1306.font.width + SSD1306.cursor.y * 128 + b] = 
                    SSD1306.font.address[2 + (c - 32) * SSD1306.font.width + b];
            }
        
        SSD1306.cursor.x++;
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

#if defined(SSD1306PRINT)       || defined(SSD1306PRINTLN)    || \
    defined(SSD1306PRINTNUMBER) || defined(SSD1306PRINTFLOAT)
void SSD1306_print(u8 *string)
{
    while (*string != 0)
        SSD1306_printChar(*string++);
}
#endif

#if defined(SSD1306PRINTLN)
void SSD1306_println(u8 *string)
{
    SSD1306_print(string);
    SSD1306_print("\n\r");
}
#endif

#if defined(SSD1306PRINTNUMBER) || defined(SSD1306PRINTFLOAT)
void SSD1306_printNumber(long value, u8 base)
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
        SSD1306_printChar('0');
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

    SSD1306_print(string);
}
#endif

#if defined(SSD1306PRINTFLOAT)
void SSD1306_printFloat(float number, u8 digits)
{ 
	u8 i, toPrint;
	u16 int_part;
	float rounding, remainder;

	// Handle negative numbers
	if (number < 0.0)
	{
		SSD1306_printChar('-');
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
	SSD1306_printNumber(int_part, 10);

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0)
		SSD1306_printChar('.'); 

	// Extract digits from the remainder one at a time
	while (digits-- > 0)
	{
		remainder *= 10.0;
		toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
		SSD1306_printNumber(toPrint, 10);
		remainder -= toPrint; 
	}
}
#endif

#ifdef SSD1306PRINTF
void SSD1306_printf(const u8 *fmt, ...)
{
    static u8 buffer[25];
    u8 *c=(char*)&buffer;
    va_list	args;

    va_start(args, fmt);
    psprintf2(buffer, fmt, args);
    va_end(args);

    while (*c != 0)
        SSD1306_printChar(*c++);
}
#endif

///	--------------------------------------------------------------------
/// Graphic functions
///	--------------------------------------------------------------------

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Sets the cursor to the specified x,y position
    PARAMETERS:
        0 < x < 319 if 0 < y < 239
    or	0 < x < 239 if 0 < y < 319
    RETURNS:
    REMARKS:
    TODO:
        check x and y
------------------------------------------------------------------*/

void SSD1306_setCursor(u8 x, u8 y)
{
    if ( x >= DISPLAY_WIDTH || y >= (DISPLAY_HEIGHT) ) return;

    SSD1306.cursor.x = x;
    SSD1306.cursor.y = y;
    SSD1306.cursor.page = x & DISPLAY_ROW_MASK;  // current page
}

#ifdef SSD1306GRAPHICS
/*	--------------------------------------------------------------------
    DESCRIPTION:
        Draws a pixel with current color.
    PARAMETERS:
        x,y coord.
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

void SSD1306_drawPixel(u8 x, u8 y)
{
    if ( x >= DISPLAY_WIDTH || y >= (DISPLAY_HEIGHT) ) return;

    SSD1306_buffer[x + (y / DISPLAY_ROWS) * DISPLAY_WIDTH] |= 1 << (y % DISPLAY_ROWS);
}

void SSD1306_clearPixel(u8 x, u8 y)
{
    if (x >= DISPLAY_WIDTH || y >= (DISPLAY_HEIGHT) ) return;
    
    SSD1306_buffer[x + (y / DISPLAY_ROWS) * DISPLAY_WIDTH] &= ~(1 << (y % DISPLAY_ROWS));
}

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Returns pixel color at x,y position.
    PARAMETERS:
        x,y coord.
    RETURNS:
        color
    REMARKS:
------------------------------------------------------------------*/

u8 SSD1306_getColor(u8 x, u8 y)
{
    u8 column,temp,bit_mask;//page;
    unsigned int page;
    u8* GDptr = SSD1306_bufferptr;
   
    // Convert coordinates
    column = x & DISPLAY_WIDTH_MASK; // 0-128
    //page = (y>>DISPLAY_ROW_BITS)&DISPLAY_ROW_MASK; // 0-8
    temp = y & DISPLAY_ROW_MASK;
    bit_mask = 1<<temp;

    //GDptr+= column+(DISPLAY_WIDTH*page);  

    // Optimise
    page = (y << (DISPLAY_WIDTH_BITS - DISPLAY_ROW_BITS)); // 0-8 * 128
    GDptr += column+page;

    // Set the bit
    if (*GDptr & bit_mask) // Pixel on
    {
            return 1;
    }
    else // Pixel off
    {
            return 0;
    }
}

/*
void SSD1306_drawBitmap(u16 x, u16 y, u16 w, u16 h, u16* bitmap)
{
}
*/

/*	--------------------------------------------------------------------
    DESCRIPTION:
        Graphic routines based on drawPixel in graphics.c
    PARAMETERS:
    RETURNS:
    REMARKS:
------------------------------------------------------------------*/

void drawPixel(u16 x, u16 y)
{
    SSD1306_drawPixel(x, y);
}

void SSD1306_drawCircle(u16 x, u16 y, u16 radius)
{
    drawCircle(x, y, radius);
}

void SSD1306_fillCircle(u16 x, u16 y, u16 radius)
{
    fillCircle(x, y, radius);
}

void SSD1306_drawLine(u16 x0, u16 y0, u16 x1, u16 y1)
{
    drawLine(x0, y0, x1, y1);
}
#endif // SSD1306GRAPHICS

#endif /* __SSD1306_C */
