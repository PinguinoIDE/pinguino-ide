/*	----------------------------------------------------------------------------
	FILE:			lcdi2c.h
	PROJECT:		pinguino - http://www.pinguino.cc/
	PURPOSE:		driving lcd display through i2c pcf8574 i/o expander
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	29 jul. 2008
	LAST RELEASE:	06 apr. 2011
	----------------------------------------------------------------------------
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
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	--------------------------------------------------------------------------*/

/*	----------------------------------------------------------------------------
	---------- LCD 2x16 (GDM1602A with build-in Samsung KS0066/S6A0069)
	----------------------------------------------------------------------------

	01 - VSS (GRND)
	02 - VDD (+5V)
	03 - Vo (R = 1KOhm à la masse)
	04 - RS
	05 - RW (reliée à la masse donc RW = 0 = écriture)
	06 - EN
	07 a 10 - D0 a D3 du LCD doivent rester « en l'air » (non connectées ou à la masse).
	11 a 16 - D4 a D7 du LCD sont reliées au PIC ou PCF8574
	15 - LED+ ???
	16 - LED- ???
	--------------------------------------------------------------------------*/

/*	----------------------------------------------------------------------------
	---------- PCF8574P
	----------------------------------------------------------------------------

	+5V		A0		-|o |-		VDD	    +5V
	+5V		A1		-|	|-		SDA	    pull-up 1K8 au +5V
	+5V		A2		-|	|-		SCL 	pull-up 1K8 au +5V
	LCD_BL	P0		-|	|-		INT
	LCD_RS	P1		-|	|-		P7		LCD_D7
	LCD_RW	P2		-|	|-		P6		LCD_D6
	LCD_EN	P3		-|	|-		P5		LCD_D5
	GRND	VSS 	-|	|-		P4		LCD_D4

	SYMBOL 	PIN	    DESCRIPTION					NB
	A0		1		address input 0				adress = 0 1 0 0 A2 A1 A0 0
	A1		2		address input 1				A0, A1 et A2 relies au +5V
	A2		3		address input 2				donc adress = 01001110 = 0x4E
	P0		4		quasi-bidirectional I/O 0	LCD_BL
	P1		5		quasi-bidirectional I/O 1	LCD_RS
	P2		6		quasi-bidirectional I/O 2	LCD_RW
	P3		7		quasi-bidirectional I/O 3	LCD_EN
	VSS		8		supply ground
	P4		9		quasi-bidirectional I/O 4	LCD_D4
	P5		10		quasi-bidirectional I/O 5	LCD_D5
	P6		11		quasi-bidirectional I/O 6	LCD_D6
	P7		12		quasi-bidirectional I/O 7	LCD_D7
	INT		13		interrupt output (active LOW)
	SCL		14		serial clock line			uC_SCL
	SDA		15		serial data line			uC_SDA
	VDD		16		supply voltage
	--------------------------------------------------------------------------*/

#ifndef __LCDI2C_H
#define __LCDI2C_H

#include <typedef.h>
//#include <delay.c>

typedef union
{
    u8 val;
    struct
    {
        unsigned bit0 :1;
        unsigned bit1 :1;
        unsigned bit2 :1;
        unsigned bit3 :1;
        unsigned bit4 :1;
        unsigned bit5 :1;
        unsigned bit6 :1;
        unsigned bit7 :1;
    } bits;
} _Byte_;

extern _Byte_ PCF8574_data;

#define LCD_MASK		        0b11110000	// On ne commande que D7 a D4

#define LCD_BL	PCF8574_data.bits.bit0	// P0
#define LCD_RS	PCF8574_data.bits.bit1	// P1
#define LCD_RW	PCF8574_data.bits.bit2	// P2
#define LCD_EN	PCF8574_data.bits.bit3	// P3
#define LCD_D4	PCF8574_data.bits.bit4	// P4
#define LCD_D5	PCF8574_data.bits.bit5	// P5
#define LCD_D6	PCF8574_data.bits.bit6	// P6
#define LCD_D7	PCF8574_data.bits.bit7	// P7

#define LCD_WRITE		        0
#define LCD_READ		        1
#define LCD_DATA		        1
#define LCD_CMD		            0

#define LCD_DISPLAY_CLEAR		0b00000001 	// 0x01
#define LCD_CURSOR_HOME			0b00000010 	// 0x02
#define LCD_ENTRY_MODE_SET		0b00000110 	// 0x06 Increment + Display not shifted
// Display : 00001DCB avec D=Display, C=Cursor et B=Blinking
#define LCD_DISPLAY_ON			0b00001100 	// 0x0C Display ON + Cursor OFF + Blinking OFF
#define LCD_DISPLAY_OFF			0b00001000 	// 0x08 Display OFF + Cursor OFF + Blinking OFF
#define LCD_SYSTEM_SET_4BITS	0b00101000 	// 0x28 Mode 4 bits - 2 lignes - 5x7
#define LCD_SYSTEM_SET_8BITS	0b00111000 	// 0x38 Mode 8 bits - 2 lignes - 5x7
#define LCD_ADRESS_LINE1		0b10000000 	// DB7=1 pour adresser DD-RAM + Adresse Ligne 1 (0x00) = 0x80
#define LCD_ADRESS_LINE2		0b11000000 	// DB7=1 pour adresser DD-RAM + Adresse Ligne 2 (0x40) = 0xC0
#define LCD_ADRESS_NUMBER		0b00110000 	// Adresse RAM du code ASCII pour les chiffres
#define LCD_ADRESS_CGRAM		0b01000000 	// 0x40 Adresse CGRAM

// commands
#define LCD_CLEARDISPLAY		0x01
#define LCD_RETURNHOME			0x02
#define LCD_ENTRYMODESET		0x04
#define LCD_DISPLAYCONTROL		0x08
#define LCD_CURSORSHIFT			0x10
#define LCD_FUNCTIONSET			0x20
#define LCD_SETCGRAMADDR		0x40
#define LCD_SETDDRAMADDR		0x80

// flags for display entry mode
#define LCD_ENTRYRIGHT			0x00
#define LCD_ENTRYLEFT			0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
#define LCD_DISPLAYON			0x04
#define LCD_DISPLAYOFF			0x00
#define LCD_CURSORON			0x02
#define LCD_CURSOROFF			0x00
#define LCD_BLINKON				0x01
#define LCD_BLINKOFF			0x00

// flags for display/cursor shift
#define LCD_DISPLAYMOVE			0x08
#define LCD_CURSORMOVE			0x00
#define LCD_MOVERIGHT			0x04
#define LCD_MOVELEFT			0x00

// flags for function set
#define LCD_8BITMODE			0x10
#define LCD_4BITMODE			0x00
#define LCD_2LINE				0x08
#define LCD_1LINE				0x00
#define LCD_5x10DOTS			0x04
#define LCD_5x8DOTS				0x00

#define LCD_CENTER				101
#define LCD_RIGHT				102
#define LCD_LEFT				103

#define DEGREE					0b11011111 	// Code ASCII pour le symbole degre
#define SIGMA					0b11100101 	// Code ASCII pour le symbole sigma
#define MICRO					0b11100100 	// Code ASCII pour le symbole micro
#define INFINITE				0b11110011 	// Code ASCII pour le symbole infini
#define SPACE					0x20		// Code ASCII pour un espace

#define HEXADECIMAL				16
#define DECIMAL					10
#define OCTAL					8
#define BINAIRE					2

#define ACIRC					0		// â
#define AGRAVE					1		// à

#define CCEDIL					2		// ç

#define EACUTE					3		// é
#define EGRAVE					4		// è
#define ECIRC					5		// ê
#define ETREMA					6		// ë
#define EURO					7		// €

#define ICIRC					8		// î
#define ITREMA					9		// ï

#define OCIRC					10		// ô

#define UGRAVE					11		// ù
#define UCIRC					12		// û

/// VARIABLES GLOBALES

extern u8 gBacklight;
/*
extern const u8 car0[8];
extern const u8 car1[8];
extern const u8 car2[8];
extern const u8 car3[8];
extern const u8 car4[8];
extern const u8 car5[8];
extern const u8 car6[8];
extern const u8 car7[8];
extern const u8 car8[8];
extern const u8 car9[8];
extern const u8 car10[8];
extern const u8 car11[8];
extern const u8 car12[8];
*/
/// PROTOTYPES

// private
static void lcdi2c_send4(u8, u8);
static void lcdi2c_send8(u8, u8);

// public
void lcdi2c_init(u8, u8, u8);
void lcdi2c_backlight();
void lcdi2c_noBacklight();
//void lcdi2c_clear();
void lcdi2c_clearLine(u8);
//void lcdi2c_home();
//void lcdi2c_noAutoscroll();
//void lcdi2c_autoscroll();
//void lcdi2c_rightToLeft();
//void lcdi2c_leftToRight();
//void lcdi2c_scrollDisplayRight();
//void lcdi2c_scrollDisplayLeft();
//void lcdi2c_blink();
//void lcdi2c_noBlink();
//void lcdi2c_cursor();
//void lcdi2c_noCursor();
//void lcdi2c_display();
//void lcdi2c_noDisplay();
void lcdi2c_setCursor(u8, u8);
void lcdi2c_write(u8);
void lcdi2c_printf(char*, ...);
void lcdi2c_newchar(const u8 *, u8);
//void lcdi2c_newpattern();

#if defined(LCDI2CCLEAR)
#define lcdi2c_clear()              do { lcdi2c_send8(LCD_DISPLAY_CLEAR, LCD_CMD); Delayms(2); } while(0)
#endif

#if defined(LCDI2CHOME)
#define lcdi2c_home()               do { lcdi2c_send8(LCD_CURSOR_HOME, LCD_CMD); Delayms(2); } while(0)
#endif

#if defined(LCDI2CNOAUTOSCROLL)
#define lcdi2c_noAutoscroll()       lcdi2c_send8(LCD_ENTRYSHIFTDECREMENT, LCD_CMD)
#endif

#if defined(LCDI2CAUTOSCROLL)
#define lcdi2c_autoscroll()         lcdi2c_send8(LCD_ENTRYSHIFTINCREMENT, LCD_CMD)
#endif

#if defined(LCDI2CRIGHTTOLEFT)
#define lcdi2c_rightToLeft()        lcdi2c_send8(LCD_ENTRYRIGHT, LCD_CMD)
#endif

#if defined(LCDI2CLEFTTORIGHT)
#define lcdi2c_leftToRight()        lcdi2c_send8(LCD_ENTRYLEFT, LCD_CMD)
#endif

#if defined(LCDI2CSCROLLDISPLAYRIGHT)
#define lcdi2c_scrollDisplayRight() lcdi2c_send8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, LCD_CMD)
#endif

#if defined(LCDI2CSCROLLDISPLAYLEFT)
#define lcdi2c_scrollDisplayLeft()  lcdi2c_send8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, LCD_CMD)
#endif

#if defined(LCDI2CBLINK)
#define lcdi2c_blink()              lcdi2c_send8(LCD_BLINKON, LCD_CMD)
#endif

#if defined(LCDI2CNOBLINK)
#define lcdi2c_noBlink()            lcdi2c_send8(LCD_BLINKOFF, LCD_CMD)
#endif

#if defined(LCDI2CCURSOR)
#define lcdi2c_cursor()             lcdi2c_send8(LCD_CURSORON, LCD_CMD)
#endif

#if defined(LCDI2CNOCURSOR)
#define lcdi2c_noCursor()           lcdi2c_send8(LCD_CURSOROFF, LCD_CMD)
#endif

#if defined(LCDI2CDISPLAY)
#define lcdi2c_display()            lcdi2c_send8(LCD_DISPLAYON, LCD_CMD)
#endif

#if defined(LCDI2CNODISPLAY)
#define lcdi2c_noDisplay()          lcdi2c_send8(LCD_DISPLAYOFF, LCD_CMD)
#endif

#endif
