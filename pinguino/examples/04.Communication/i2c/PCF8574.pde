/*	----------------------------------------------------------------------------
	FILE:           pcf8574.pde
	PROJECT:        pinguino
	PURPOSE:        driving led through i2c pcf8574 i/o expander
	PROGRAMER:      regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:  06 apr. 2011
	LAST RELEASE:   22 jun. 2012
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
	---------- PCF8574P
	----------------------------------------------------------------------------

							      +5V		        A0		-|o |-		VDD	+5V
							      +5V		        A1		-|	 |-		SDA	pull-up 1K8 au +5V
							      +5V		        A2		-|	 |-		SCL 	pull-up 1K8 au +5V
	+5V-----(+LED-)-----330R-----  P0		-|	 |-		INT
							                    P1		-|	 |-		P7		
							                    P2		-|	 |-		P6		
							                    P3		-|	 |-		P5		
				    			GRND            VSS		-|	 |-		P4		

	SYMBOL  PIN     DESCRIPTION	                COMMENT
	A0      1       address input 0             address = 0 1 0 0 A2 A1 A0
	A1      2       address input 1             A0, A1 et A2 connected to +5V
	A2      3       address input 2             then adress is 01001110 = 0x4E
	P0      4       quasi-bidirectional I/O     0
	P1      5       quasi-bidirectional I/O     1
	P2      6       quasi-bidirectional I/O     2
	P3      7       quasi-bidirectional I/O     3
	VSS     8       supply ground
	P4      9       quasi-bidirectional I/O     4
	P5      10      quasi-bidirectional I/O     5
	P6      11      quasi-bidirectional I/O     6
	P7      12      quasi-bidirectional I/O     7
	INT     13      interrupt output (active LOW)
	SCL     14      serial clock line           Pinguino SCL
	SDA     15      serial data line            Pinguino SDA
	VDD     16      supply voltage
	
  Pinguino  SCL             SDA
  2550      pin 1 (RB1)     pin 0 (RB0)
  26j50     pin 4 (RB4)     pin 5 (RB5)
	--------------------------------------------------------------------------*/

u8 i2cAddr = 0b0100111;    // PCF8574's I2C address
void setup()
{
    pinMode(12, OUTPUT);
    I2C.master(I2C_100KHZ);             // or I2C.master(100);
}

void loop()
{
    I2C.start();                                // All I2C commands must begin with a Start condition

    I2C.send((i2cAddr << 1) & 0xFE);            // write operation (bit 0 set to 0)
    
    digitalWrite(USERLED, LOW);
    I2C.send(0b00000000);                       // set all bits of PCF8574 register to 0
    delay(500);                                 // wait for 500 ms
    
    digitalWrite(USERLED, HIGH);
    I2C.send(0b00000001);                       // set bit 0 of PCF8574 register to 1
    delay(500);                                 // wait for 500 ms

    I2C.stop();                                 // Terminate the write sequence
}
