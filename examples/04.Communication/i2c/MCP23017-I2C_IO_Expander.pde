/*
 * File			MCP23017-I2C_IO_Expander.pde
 *
 * Sample test program for
 * Microchip MCP23017 16-Bit I/O Expander with I2C Interface
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License		This software is released under the terms of the Mozilla Public License (MPL) version 2.0
 * 			Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 *
 *   21/03/2014 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it )
*/



/* 
MCP23017 = 16-Bit I/O Expander with I2C Interface  
Pin 12 SCL -> Pin 5 UEXT
Pin 13 SDA -> Pin 6 UEXT
Pin 15 - 16 - 16 (A0 - A1 - A2) to GND
Pin 18 RESET -> Con1.1 (31)
Pin 1 GPB0 DIP SWITCH to GND
Pin 2 GPB1 DIP SWITCH to GND
Pin 3 GPB2 DIP SWITCH to GND
Pin 4 GPB3 DIP SWITCH to GND
Pin 22 GPA2 LED to GND
Pin 24 GPA4 LED to GND
Pin 26 GPA6 LED to GND
Pin 28 GPA8 LED to GND
*/




/*
Support for Microchip MCP23017 16-Bit I/O Expander with I2C Interface 

#define MCP23017_IODIRA 0x00
#define MCP23017_IPOLA 0x02
#define MCP23017_GPINTENA 0x04
#define MCP23017_DEFVALA 0x06
#define MCP23017_INTCONA 0x08
#define MCP23017_IOCONA 0x0A
#define MCP23017_GPPUA 0x0C
#define MCP23017_INTFA 0x0E
#define MCP23017_INTCAPA 0x10
#define MCP23017_GPIOA 0x12
#define MCP23017_OLATA 0x14


#define MCP23017_IODIRB 0x01
#define MCP23017_IPOLB 0x03
#define MCP23017_GPINTENB 0x05
#define MCP23017_DEFVALB 0x07
#define MCP23017_INTCONB 0x09
#define MCP23017_IOCONB 0x0B
#define MCP23017_GPPUB 0x0D
#define MCP23017_INTFB 0x0F
#define MCP23017_INTCAPB 0x11
#define MCP23017_GPIOB 0x13
#define MCP23017_OLATB 0x15



//IOCON Flag 
//Unimplemented: Read as ‘0’.
#define MCP23017_B_NotUse 1
//INTPOL: This bit sets the polarity of the INT output pin.
//1 = Active-high.
//0 = Active-low.
#define MCP23017_B_INTPOL 2
//ODR: This bit configures the INT pin as an open-drain output.
//1 = Open-drain output (overrides the INTPOL bit).
//0 = Active driver output (INTPOL bit sets the polarity).
#define MCP23017_B_ODR    4
//HAEN: Hardware Address Enable bit (MCP23S17 only).
//Address pins are always enabled on MCP23017.
//1 = Enables the MCP23017 address pins.
//0 = Disables the MCP23017 address pins.
#define MCP23017_B_HAEN   8
//DISSLW: Slew Rate control bit for SDA output.
//1 = Slew rate disabled.
//0 = Slew rate enabled.
#define MCP23017_B_DISSLW 16
//SEQOP: Sequential Operation mode bit.
//1 = Sequential operation disabled, address pointer does not increment.
//0 = Sequential operation enabled, address pointer increments.
#define MCP23017_B_SEQOP  32
//MIRROR: INT Pins Mirror bit
//1 = The INT pins are internally connected
//0 = The INT pins are not connected. INTA is associated with PortA and INTB is associated with PortB
#define MCP23017_B_MIRROR 64
//BANK: Controls how the registers are addressed
//1 = The registers associated with each port are separated into different banks
//0 = The registers are in the same bank (addresses are sequential)
#define MCP23017_B_BANK   128

typedef union tagMCP23017U
{
 u8 B[2];
 u16 W;
} MCP23017U;
  
typedef struct tagMCP23017
 {
  u8 I2CAddr;
  MCP23017U IODIR;
  MCP23017U GPIO;
  MCP23017U GPPU;
  MCP23017U IPOL;
  MCP23017U GINP;
 } MCP23017; 
*/

/***********************************************************************************
BOOL MCP23017.write_Byte(MCP23017 * Mcp,u8 Addr,u8 Val)

Write One BYTE in the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = 8 bit value to write

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.write_Word(MCP23017 * Mcp,u8 Addr,u16 Val)

Write One WORD (LSB FIRST) in the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = 16 bit value to write

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.write_Array(MCP23017 * Mcp,u8 Addr,u16 Len,char * Val)

Write a variable number of Bytes in the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Len = Numeber of bytes to write
Val = Pointer to a write buffer

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.read_Byte(MCP23017 * Mcp,u8 Addr,u8 * Val)

Read One BYTE from the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = Pointer to a 8 bit read buffer

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.read_Word(MCP23017 * Mcp,u8 Addr,u16 * Val)

Read One WORD (LSB FIRST) from the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = Pointer to a 16 bit read buffer

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.read_Array(MCP23017 * Mcp,u8 Addr,u8 Len,char * Val)

Read a variable number of BYTE from the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Len = Numeber of bytes to read
Val = Pointer to a read buffer

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.init(MCP23017 * Mcp,u8 I2CAddr)

Init a MCP23017 device and structure

Mcp = Pointer to a  user declared MCP23017 structure
I2CAddr = Internal adress of the MCP23017 device

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.readGPIO_A(u8 * Val, MCP23017 * Mcp)

Read input port A

Val = Pointer to a 8 bit read buffer
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.readGPIO_B(u8 * Val, MCP23017 * Mcp)

Read input port B

Val = Pointer to a 8 bit read buffer
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.readGPIO_AB(u8 * Val, MCP23017 * Mcp)

Read input port AB

Val = Pointer to a 16 bit read buffer
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeGPIO_A(u8 Val, MCP23017 * Mcp)

Write output port A

Val = 8 bit value to write
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeGPIO_B(u8 Val, MCP23017 * Mcp)

Write output port B

Val = 8 bit value to write
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeGPIO_AB(u8 Val, MCP23017 * Mcp)

Write output port AB

Val = 16 bit value to write
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeIODIR_A(u8 Val, MCP23017 * Mcp)

Write Input/Output direction register of Port A

Val = 8 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeIODIR_B(u8 Val, MCP23017 * Mcp)

Write Input/Output direction register of Port B

Val = 8 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeIODIR_AB(u8 Val, MCP23017 * Mcp)

Write Input/Output direction register of Port AB

Val = 16 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeGPPU_A(u8 Val, MCP23017 * Mcp)

Write Resistor PullUp register of Port A

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeGPPU_B(u8 Val, MCP23017 * Mcp)

Write Resistor PullUp register of Port B

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeGPPU_AB(u8 Val, MCP23017 * Mcp)

Write Resistor PullUp register of Port AB

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeIPOL_A(u8 Val, MCP23017 * Mcp)

Write INPUT POLARITY register of Port A

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeIPOL_B(u8 Val, MCP23017 * Mcp)

Write INPUT POLARITY register of Port B

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.writeIPOL_AB(u8 Val, MCP23017 * Mcp)

Write INPUT POLARITY register of Port AB

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
u8 MCP23017.digitalRead(u8 Pin, MCP23017 * Mcp)

Read the status of a Input

Pin = Input to read (from 1 to 16)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN Input Status 0 = LOW, 1 = HIGH, 0xFF=Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.pinMode(u8 Pin, u8 OutIn,MCP23017 * Mcp)

Define if a pin is an Inpur or a Output

Pin = Pin on which operate (from 1 to 16)
OutIn = (INPUT = Input Pin, OUTPUT = Output Pin)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.pullUp(u8 Pin, u8 SPull, MCP23017 * Mcp)

Define the status of a Resistor PullUp

Pin = Pin on which operate (from 1 to 16)
SPull = (HIGH = PullUp Enabled, LOW = PullUp Disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.inputPol(u8 Pin, u8 SInPol, MCP23017 * Mcp)

Define the status of a Input Polarity

Pin = Pin on which operate (from 1 to 16)
SInPol = (HIGH = Input Inverted, LOW = Input NOT Inverted)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP23017.digitalWrite(u8 Pin, u8 SOut, MCP23017 * Mcp)

Write the status of a Output

Pin = Output to write (from 1 to 16)
SOut = (HIGH = Output high, LOW = Output low)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/









#define CS  30
#define RESET 31
#define ledPinY			    10 // the number of the yellow LED pin 

            
MCP23017 Mcp1;


                                        
//#define BLINK_OUTW   1

#define BLINK_OUTB   1

//#define INPUT_STATEW   1  

//#define INPUT_STATEB   1  
                                                            
void setup()
{  
 pinMode(ledPinY, OUTPUT); 
 digitalWrite(ledPinY,LOW);  
 pinMode(RESET, OUTPUT); 
 digitalWrite(RESET,LOW);  
 pinMode(CS, OUTPUT); 
 digitalWrite(CS,HIGH); 
 delay(20); 
 digitalWrite(RESET,HIGH);  
 
 MCP23017.init(&Mcp1,0);                    // use default address 0


#ifdef BLINK_OUTW
 MCP23017.writeIODIR_AB(0x0000,&Mcp1); //PORTB output PORTA output 
#endif



#ifdef BLINK_OUTB
MCP23017.pinMode(8, OUTPUT,&Mcp1);
MCP23017.pinMode(6, OUTPUT,&Mcp1);
MCP23017.pinMode(4, OUTPUT,&Mcp1);
MCP23017.pinMode(2, OUTPUT,&Mcp1);
#endif



#ifdef INPUT_STATEW
 MCP23017.writeIODIR_AB(0xff00,&Mcp1); //PORTA output PORTB input 
 MCP23017.writeGPPU_AB(0xff00,&Mcp1);  //PORTB PullUp ON
#endif



#ifdef INPUT_STATEB
MCP23017.pinMode(8, OUTPUT,&Mcp1);
MCP23017.pinMode(6, OUTPUT,&Mcp1);
MCP23017.pinMode(4, OUTPUT,&Mcp1);
MCP23017.pinMode(2, OUTPUT,&Mcp1);
MCP23017.pinMode(9, INPUT,&Mcp1);
MCP23017.pinMode(10, INPUT,&Mcp1);
MCP23017.pinMode(11, INPUT,&Mcp1);
MCP23017.pinMode(12, INPUT,&Mcp1);
MCP23017.pullUp(9, HIGH,&Mcp1);
MCP23017.pullUp(10, HIGH,&Mcp1);
MCP23017.pullUp(11, HIGH,&Mcp1);
MCP23017.pullUp(12, HIGH,&Mcp1);
#endif
}




void loop()
{
 u16 Valo,Valo1;
 u8 ValoB;
 char Str[20];

 toggle(ledPinY); 
 delay(100);



#ifdef BLINK_OUTW
 MCP23017.writeGPIO_AB(0x0033,&Mcp1);
 delay(100);
 MCP23017.writeGPIO_AB(0x00CC,&Mcp1);
#endif



#ifdef BLINK_OUTB
MCP23017.digitalWrite(8, HIGH,&Mcp1);
MCP23017.digitalWrite(6, LOW,&Mcp1);
MCP23017.digitalWrite(4, HIGH,&Mcp1);
MCP23017.digitalWrite(2, LOW,&Mcp1);
delay(100);
MCP23017.digitalWrite(8, LOW,&Mcp1);
MCP23017.digitalWrite(6, HIGH,&Mcp1);
MCP23017.digitalWrite(4, LOW,&Mcp1);
MCP23017.digitalWrite(2, HIGH,&Mcp1);
#endif



#ifdef INPUT_STATEW
 MCP23017.readGPIO_AB(&Valo,&Mcp1);
 Valo1 = 0;
 if ( (Valo & 0x0100) == 0)
  Valo1 |= 0x0080;
 if ( (Valo & 0x0200) == 0)
  Valo1 |= 0x0020;
 if ( (Valo & 0x0400) == 0)
  Valo1 |= 0x0008;
 if ( (Valo & 0x0800) == 0)
  Valo1 |= 0x0002;
 MCP23017.writeGPIO_AB(Valo1,&Mcp1);
#endif



#ifdef INPUT_STATEB
MCP23017.digitalWrite(8, !MCP23017.digitalRead(9,&Mcp1),&Mcp1);
MCP23017.digitalWrite(6, !MCP23017.digitalRead(10,&Mcp1),&Mcp1);
MCP23017.digitalWrite(4, !MCP23017.digitalRead(11,&Mcp1),&Mcp1);
MCP23017.digitalWrite(2, !MCP23017.digitalRead(12,&Mcp1),&Mcp1);
#endif
}
