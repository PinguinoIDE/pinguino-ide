/*  ----------------------------------------------------------------------------
    FILE:           lcdi2c.pde
    PROJECT:        pinguino
    PURPOSE:        driving lcd display through i2c pcf8574 i/o expander
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  06 apr. 2011
    LAST RELEASE:   12 jun. 2012
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

/*  ----------------------------------------------------------------------------
    ---------- PCF8574P
    ----------------------------------------------------------------------------

    +5V		A0	-|o |-		VDD		+5V
    +5V		A1	-|	 |-		SDA		pull-up 1K8 au +5V
    +5V		A2	-|	 |-		SCL 	pull-up 1K8 au +5V
    LCD_BL	P0	-|	 |-		INT
    LCD_RS	P1	-|	 |-		P7		LCD_D7
    LCD_RW	P2	-|	 |-		P6		LCD_D6
    LCD_EN	P3	-|	 |-		P5		LCD_D5
    GRND	VSS		-|	 |-		P4		LCD_D4

    SYMBOL  PIN DESCRIPTION					          NB
    A0		   1		address input 0				
    A1		   2		address input 1				        A0, A1 et A2 connected to +5V
    A2		   3		address input 2				
    P0		   4		quasi-bidirectional I/O 0    LCD_BL
    P1		   5		quasi-bidirectional I/O 1	  LCD_RS
    P2		   6		quasi-bidirectional I/O 2	  LCD_RW
    P3		   7		quasi-bidirectional I/O 3	  LCD_EN
    VSS	   8		supply ground
    P4		   9		quasi-bidirectional I/O 4	  LCD_D4
    P5		   10	  quasi-bidirectional I/O 5	  LCD_D5
    P6		   11	  quasi-bidirectional I/O 6	  LCD_D6
    P7		   12	  quasi-bidirectional I/O 7	  LCD_D7
    INT	   13	  interrupt output (active LOW)
    SCL	   14	  serial clock line			       To Pinguino SCL
    SDA	   15	  serial data line			       To Pinguino SDA
    VDD	   16	  supply voltage

    Pinguino    x550    x6j50
    SDA         DO      D5
    SDL         D1      D4

    ----------------------------------------------------------------------------
    ---------- LCD 2x16 (GDM1602A with build-in Samsung KS0066/S6A0069)
    ----------------------------------------------------------------------------

    01 - VSS (GND)
    02 - VDD (+5V)
    03 - Vo (R = 1K to GND)
    04 - RS (P1)
    05 - RW (P2)
    06 - EN (P3)
    07 a 10 - D0 to D3 connected to GND.
    11 a 16 - D4 to D7 connected to PCF8574
    15 - LED+ 380 Ohm to +5V
    16 - LED- to GND
    
    --------------------------------------------------------------------------*/

u16 i=0;

void setup()
{
    // PCF8574  : slave adress is 0 1 0 0 A2 A1 A0
    // PCF8574A : slave adress is 0 1 1 1 A2 A1 A0
    lcdi2c.init(16, 2, 0x27);           // display is 2x16, ic2 address is 0100111 (see above)
    lcdi2c.backlight();                 // turns backlight on
    lcdi2c.clear();                     // clear screen
    lcdi2c.home();                      // set cursor at (0,0)
    lcdi2c.printf("   lcdi2c demo  ");
}

void loop()
{
    lcdi2c.setCursor(0, 1);					// set cursor at line 1, col 0
    lcdi2c.printf("i=%u ", i++);
    delay(500);
}

/*
	available functions :
		void lcdi2c.init(u8, u8, u8);
		void lcdi2c.backlight();
		void lcdi2c.noBacklight();
		void lcdi2c.clear();
		void lcdi2c.clearLine(u8);
		void lcdi2c.home();
		void lcdi2c.noAutoscroll();
		void lcdi2c.autoscroll();
		void lcdi2c.rightToLeft();
		void lcdi2c.leftToRight();
		void lcdi2c.scrollDisplayRight();
		void lcdi2c.scrollDisplayLeft();
		void lcdi2c.blink();
		void lcdi2c.noBlink();
		void lcdi2c.cursor();
		void lcdi2c.noCursor();
		void lcdi2c.display();
		void lcdi2c.noDisplay();
		void lcdi2c.setCursor(u8, u8);
		void lcdi2c.write(char);
		void lcdi2c.printf(char*, ...);
		void lcdi2c.newchar(const u8 *, u8);
*/
