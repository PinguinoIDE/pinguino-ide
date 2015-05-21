/*	----------------------------------------------------------------------------
	FILE:			lcdi2c.c
	PROJECT:		pinguino - http://www.pinguino.cc/
	PURPOSE:		driving lcd display through i2c pcf8574 i/o expander
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	29 jul. 2008
	LAST RELEASE:	11 apr. 2011
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
	05 - RW (can be connected to GND so that RW = 0 = write)
	06 - EN
	07 a 10 - D0 to D3 connected to GND (4-bit mode).
	11 a 16 - D4 to D7 connected to PCF8574's pins (see below)
	15 - LED+ R330
	16 - LED- GND or Backlight pin in PCF8574
	--------------------------------------------------------------------------*/

/*	----------------------------------------------------------------------------
	---------- PCF8574P
	----------------------------------------------------------------------------

	+5V		A0		-|o |-		VDD		+5V
	+5V		A1		-|	|-		SDA		pull-up 1K8 au +5V
	+5V		A2		-|	|-		SCL 	pull-up 1K8 au +5V
	LCD_BL	P0		-|	|-		INT
	LCD_RS	P1		-|	|-		P7		LCD_D7
	LCD_RW	P2		-|	|-		P6		LCD_D6
	LCD_EN	P3		-|	|-		P5		LCD_D5
	GRND	VSS		-|	|-		P4		LCD_D4

	SYMBOL 	PIN		DESCRIPTION					NB
	A0		1		address input 0				adress = 0 1 0 0 A2 A1 A0 0
	A1		2		address input 1				A0, A1 et A2 connected to +5V
	A2		3		address input 2				then address is 01001110 = 0x4E
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

#ifndef __LCDI2C_C
#define __LCDI2C_C

#include <lcdi2c.h>
#include <pcf8574.h>
#include <stdarg.h>
#include <delay.c>
#include <i2c.c>
#include <printf.c>

/*	----------------------------------------------------------------------------
	---------- Défintion des caractères spéciaux
	----------------------------------------------------------------------------
	---------- â, à, ç, é, î, ô, ù, ê, è, ë, ï, û, €
	---------- usage :
	---------- 1/ réservation de l'emplacement 0 (max. 7) pour la lettre "é" : lcdi2c_newchar(car3, 0);
	---------- 2/ écriture du nouveau caractère sur le LCD : lcdi2c_write(0);
	--------------------------------------------------------------------------*/
#ifndef __32MX220F032D__
	const u8 car0[8]={
	    0b00000100,      //â
	    0b00001010,
	    0b00001110,
	    0b00000001,
	    0b00001111,
	    0b00010001,
	    0b00001111,
	    0b00000000
	};
	const u8 car1[8]={
	    0b00000100,     //à
	    0b00000010,
	    0b00001110,
	    0b00000001,
	    0b00001111,
	    0b00010001,
	    0b00001111,
	    0b00000000
	};
	const u8 car2[8]={
	    0b00001110,     //ç
	    0b00010000,
	    0b00010000,
	    0b00010001,
	    0b00001110,
	    0b00000100,
	    0b00001100,
	    0b00000000
	};
	const u8 car3[8]={
	    0b00000100,     //é
	    0b00001000,
	    0b00001110,
	    0b00010001,
	    0b00011111,
	    0b00010000,
	    0b00001110,
	    0b00000000
	};
	const u8 car4[8]={
	    0b00000100,     //è
	    0b00000010,
	    0b00001110,
	    0b00010001,
	    0b00011111,
	    0b00010000,
	    0b00001110,
	    0b00000000
	};
	const u8 car5[8]={
	    0b00000100,     //ê
	    0b00001010,
	    0b00001110,
	    0b00010001,
	    0b00011111,
	    0b00010000,
	    0b00001110,
	    0b00000000
	};
	const u8 car6[8]={
	    0b00001010,     //ë
	    0b00000000,
	    0b00001110,
	    0b00010001,
	    0b00011111,
	    0b00010000,
	    0b00001110,
	    0b00000000
	};
	const u8 car7[8]={
	    0b00000111,     //€
	    0b00001000,
	    0b00011110,
	    0b00001000,
	    0b00011110,
	    0b00001000,
	    0b00000111,
	    0b00000000
	};
	const u8 car8[8]={
	    0b00000100,     //î
	    0b00001010,
	    0b00001100,
	    0b00000100,
	    0b00000100,
	    0b00000100,
	    0b00001110,
	    0b00000000
	};
	const u8 car9[8]={
	    0b00001010,     //ï
	    0b00000000,
	    0b00001100,
	    0b00000100,
	    0b00000100,
	    0b00000100,
	    0b00001110,
	    0b00000000
	};
	const u8 car10[8]={
	    0b00000100,     //ô
	    0b00001010,
	    0b00001110,
	    0b00010001,
	    0b00010001,
	    0b00010001,
	    0b00001110,
	    0b00000000
	};
	const u8 car11[8]={
	    0b00000100,     //ù
	    0b00000010,
	    0b00010001,
	    0b00010001,
	    0b00010001,
	    0b00010011,
	    0b00001101,
	    0b00000000
	};
	const u8 car12[8]={
	    0b00000100,     //û
	    0b00001010,
	    0b00010001,
	    0b00010001,
	    0b00010001,
	    0b00010011,
	    0b00001101,
	    0b00000000
	};
#endif /* __32MX220F032D__ */

/*	----------------------------------------------------------------------------
	---------- global variables
	--------------------------------------------------------------------------*/

	u8 numcolmax;			// from 0 to 15 = 16
	u8 numlinemax;			// from 0 to 1 = 2
	u8 Backlight = 0;		// memorise l'etat du backlight du LCD
	u8 PCF8574_address;
	PCF8574_t PCF8574_data;

/*	----------------------------------------------------------------------------
	---------- Ecriture d'un quartet (mode 4 bits) dans le LCD
	----------------------------------------------------------------------------
	Envoie d'un quartet vers les pins :
	- D4 a D7 du LCD
	- P4 a P7 du PCF8574
	NB : quartet est en fait un 8 bits dont seuls les 4 bits de poids fort nous interessent
	@param quartet = 4 bits a envoyer au LCD
	@param mode = LCD Command (LCD_CMD) or Data (LCD_DATA) mode
	--------------------------------------------------------------------------*/

static void lcdi2c_send4(u8 quartet, u8 mode)
{
								// D7 D6 D5 D4 EN RW RS   BL
	PCF8574_data.val = quartet;	// x  x  x  x  0  0  0    0
	//LCD_EN = LOW;				// x  x  x  x  0  0  0    0
	LCD_RW = LCD_WRITE;			// x  x  x  x  0  0  0    0
	LCD_RS = mode;				// x  x  x  x  0  0  0/1  0
	LCD_BL = Backlight;			// x  x  x  x  0  0  0/1  0/1

	/// ---------- LCD Enable Cycle

	//LCD_EN = LOW;
	//I2C_send(I2C1, PCF8574_address, PCF8574_data.val);
	//Delayus(1);

	LCD_EN = HIGH;
	I2C_send(I2C1, PCF8574_address, PCF8574_data.val);
	Delayus(1);// E Pulse Width > 300ns

	LCD_EN = LOW;
	I2C_send(I2C1, PCF8574_address, PCF8574_data.val);
	Delayus(1);// E Enable Cycle > (300 + 200) = 500ns
}

/*	----------------------------------------------------------------------------
	---------- Ecriture d'un octet dans le LCD en mode 4 bits
	----------------------------------------------------------------------------
	Les données sont écrites en envoyant séquentiellement :
	1/ les quatre bits de poids fort
	2/ les quatre bits de poids faible
	NB : les poids sont stockes dans les quatre bits de poids fort
	qui correspondent aux pins D4 a D7 du LCD ou du PCF8574
	@param octet = octet a envoyer au LCD
	@param mode = LCD Command (LCD_CMD) or Data (LCD_DATA) mode
	--------------------------------------------------------------------------*/

static void lcdi2c_send8(u8 octet, u8 mode)
{
	lcdi2c_send4(octet & LCD_MASK, mode);			// envoie les "Upper 4 bits"
	lcdi2c_send4((octet << 4) & LCD_MASK, mode);	// envoie les "Lower 4 bits"
	Delayus(46);			 // Wait for instruction excution time (more than 46us)
}

/*	----------------------------------------------------------------------------
	---------- backlight
	--------------------------------------------------------------------------*/

void lcdi2c_backlight()
{
	Backlight = 1;	// 0 = ON since PCF8574 is logical inverted
	LCD_BL = Backlight;
	I2C_send(I2C1, PCF8574_address, PCF8574_data.val);
}

/*	----------------------------------------------------------------------------
	---------- noBacklight
	--------------------------------------------------------------------------*/

void lcdi2c_noBacklight()
{
	Backlight = 0;	// 1 = OFF since PCF8574 is logical inverted
	LCD_BL = Backlight;
	I2C_send(I2C1, PCF8574_address, PCF8574_data.val);
}

/*	----------------------------------------------------------------------------
	---------- Efface l'ecran
	--------------------------------------------------------------------------*/

void lcdi2c_clear()
{
	lcdi2c_send8(LCD_DISPLAY_CLEAR, LCD_CMD);
	Delayms(2);	// Wait for more than 1.64ms
}

/*	----------------------------------------------------------------------------
	---------- Retourne a la position [1,1]
	--------------------------------------------------------------------------*/

void lcdi2c_home()
{
	lcdi2c_send8(LCD_CURSOR_HOME, LCD_CMD);
	Delayms(2);	// Wait for more than 1.64ms
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_noAutoscroll()
{
	lcdi2c_send8(LCD_ENTRYSHIFTDECREMENT, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_autoscroll()
{
	lcdi2c_send8(LCD_ENTRYSHIFTINCREMENT, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_rightToLeft()
{
	lcdi2c_send8(LCD_ENTRYRIGHT, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_leftToRight()
{
	lcdi2c_send8(LCD_ENTRYLEFT, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_scrollDisplayRight()
{
	lcdi2c_send8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_scrollDisplayLeft()
{
	lcdi2c_send8(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_blink()
{
	lcdi2c_send8(LCD_BLINKON, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_noBlink()
{
	lcdi2c_send8(LCD_BLINKOFF, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_cursor()
{
	lcdi2c_send8(LCD_CURSORON, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_noCursor()
{
	lcdi2c_send8(LCD_CURSOROFF, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_display()
{
	lcdi2c_send8(LCD_DISPLAYON, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- 
	--------------------------------------------------------------------------*/

void lcdi2c_noDisplay()
{
	lcdi2c_send8(LCD_DISPLAYOFF, LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- Positionne le curseur sur le LCD
	----------------------------------------------------------------------------
	from (0,0) to (15,1)
	--------------------------------------------------------------------------*/

void lcdi2c_setCursor(u8 col, u8 line)
{
	int row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };

	if ( line > numlinemax )
		line = numlinemax - 1;	// we count rows starting w/0
	if ( col > numcolmax )
		col = numcolmax - 1;	// we count rows starting w/0
	lcdi2c_send8(LCD_SETDDRAMADDR | (col + row_offsets[line]), LCD_CMD);
}

/*	----------------------------------------------------------------------------
	---------- Efface une ligne
	--------------------------------------------------------------------------*/

void lcdi2c_clearLine(u8 line)
{
	u8 i;

	lcdi2c_setCursor(0, line);
	for (i = 0; i < numcolmax; i++)
		lcdi2c_write(ESPACE);  // affiche des espaces
}

/*	----------------------------------------------------------------------------
	---------- Affiche un caractere ASCII a la position courante du curseur
	----------------------------------------------------------------------------
	c = code ASCII du caractere
	--------------------------------------------------------------------------*/

void lcdi2c_write(char c)
{
	lcdi2c_send8(c, LCD_DATA);
}

/*	----------------------------------------------------------------------------
	---------- printf
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

void lcdi2c_printf(char *fmt, ...)
{
	va_list args;

	va_start(args, fmt);
	pprintf(lcdi2c_write, fmt, args);
	va_end(args);
}

/*	----------------------------------------------------------------------------
	---------- Définit un caractère personnalisé de 8x8 points.
	----------------------------------------------------------------------------
	d'après Nabil Al-HOSSRI <http://nalhossri.free.fr>
	----------------------------------------------------------------------------
	Le LCD utilisé admet au maximum 8 caractères spéciaux.
	char_code : code du caractère à définir (0 <= char_code <= 7)
	lcdi2c_newchar(car3, 0); Définit le caractère 'é' à l'adresse 0 de la mémoire CG RAM
	lcdi2c_newchar(car8, 1); Définit le caractère 'è' à l'adresse 1.
	--------------------------------------------------------------------------*/

void lcdi2c_newchar(const u8 *c, u8 char_code)
{
	u8 i, a;

	// les caractères sont logés dans le CGRAM du LCD à partir de l'adresse 0x40.
	a = (char_code << 3) | LCD_ADRESS_CGRAM;
	for(i=0; i<8; i++)
	{
		lcdi2c_send8(a, LCD_CMD);
		lcdi2c_send8(c[i], LCD_DATA);
		a++;
	};
}

/*	----------------------------------------------------------------------------
	---------- Définition de 8 nouveaux caractères
	--------------------------------------------------------------------------*/

void lcdi2c_newpattern()
{
	lcdi2c_newchar(car0,  ACIRC);		// â
	lcdi2c_newchar(car1,  AGRAVE);		// à

	lcdi2c_newchar(car2,  CCEDIL);		// ç

	lcdi2c_newchar(car3,  EACUTE);		// é
	lcdi2c_newchar(car4,  EGRAVE);		// è
	lcdi2c_newchar(car5,  ECIRC);		// ê
	lcdi2c_newchar(car6,  ETREMA);		// ë
	lcdi2c_newchar(car7,  EURO);		// €

	//lcdi2c_newchar(car8,  ICIRC);		// î
	//lcdi2c_newchar(car9, ITREMA);		// ï

	//lcdi2c_newchar(car10,  OCIRC);	// ô

	//lcdi2c_newchar(car11,  UGRAVE);	// ù
	//lcdi2c_newchar(car12, UCIRC);		// û
}

/*	----------------------------------------------------------------------------
	---------- Initialisation du LCD
	----------------------------------------------------------------------------
	This function must be called before any other function.
	No need to wait between 2 commands because i2c bus is quite slow.
	cf. Microchip AN587 Interfacing PICmicro® MCUs to an LCD Module
	----------------------------------------------------------------------------
	pcf8574 adress format is [0 1 0 0 A2 A1 A0 0]
	--------------------------------------------------------------------------*/

void lcdi2c_init(u8 numcol, u8 numline, u8 i2c_address)
{
	numcolmax  = numcol - 1;
	numlinemax = numline - 1;
	PCF8574_address = 0b01001110 | i2c_address;
	PCF8574_data.val = 0;

	//I2C_init(I2C1, I2C_MASTER_MODE, I2C_100KHZ);
	//I2C_init(I2C1, I2C_MASTER_MODE, I2C_400KHZ);
	I2C_init(I2C1, I2C_MASTER_MODE, I2C_1MHZ);

	Delayms(15);								// Wait more than 15 ms after VDD rises to 4.5V
	lcdi2c_send4(0x30, LCD_CMD);				// 0x30 - Mode 8 bits
	Delayms(5);									// Wait for more than 4.1 ms
	lcdi2c_send4(0x30, LCD_CMD);				// 0x30 - Mode 8 bits
	Delayus(100);								// Wait more than 100 μs
	lcdi2c_send4(0x30, LCD_CMD);				// 0x30 - Mode 8 bits
	Delayus(100);								// Wait more than 100 μs
	lcdi2c_send4(0x20, LCD_CMD);				// 0x20 - Mode 4 bits
	lcdi2c_send8(LCD_SYSTEM_SET_4BITS, LCD_CMD);// 0x28 - Mode 4 bits - 2 Lignes - 5x8
	lcdi2c_send8(LCD_ENTRY_MODE_SET, LCD_CMD);  // 0x06 - Increment + Display not shifted (Déplacement automatique du curseur)
	lcdi2c_backlight();							// backlight on
/*
	lcdi2c_display();
	lcdi2c_noBlink();
	lcdi2c_noCursor();
	lcdi2c_noAutoscroll();
	lcdi2c_setCursor(0, 0);
*/
	lcdi2c_clear();
	lcdi2c_home();
	#ifndef __32MX220F032D__
	//lcdi2c_newpattern();						// Implante les nouveaux caracteres
	#endif
}

#endif
