/*
 * File			MCP23S17.c
 *
 * Description		Support for Microchip MCP23S17 16-Bit I/O Expander with SPI Interface
 *			Only basic IO procedures are available.
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
#ifndef _MCP23S17_H_
#define _MCP23S17_H_

#include <digitalw.c>
#include <spi.c>
#include <SpiLoc.c>


#define MCP23S17_ADDRESS 0x20

#define MCP23S17_IODIRA 0x00
#define MCP23S17_IPOLA 0x02
#define MCP23S17_GPINTENA 0x04
#define MCP23S17_DEFVALA 0x06
#define MCP23S17_INTCONA 0x08
#define MCP23S17_IOCONA 0x0A
#define MCP23S17_GPPUA 0x0C
#define MCP23S17_INTFA 0x0E
#define MCP23S17_INTCAPA 0x10
#define MCP23S17_GPIOA 0x12
#define MCP23S17_OLATA 0x14


#define MCP23S17_IODIRB 0x01
#define MCP23S17_IPOLB 0x03
#define MCP23S17_GPINTENB 0x05
#define MCP23S17_DEFVALB 0x07
#define MCP23S17_INTCONB 0x09
#define MCP23S17_IOCONB 0x0B
#define MCP23S17_GPPUB 0x0D
#define MCP23S17_INTFB 0x0F
#define MCP23S17_INTCAPB 0x11
#define MCP23S17_GPIOB 0x13
#define MCP23S17_OLATB 0x15



//IOCON Flag 
//Unimplemented: Read as 0.
#define MCP23S17_B_NotUse 1
//INTPOL: This bit sets the polarity of the INT output pin.
//1 = Active-high.
//0 = Active-low.
#define MCP23S17_B_INTPOL 2
//ODR: This bit configures the INT pin as an open-drain output.
//1 = Open-drain output (overrides the INTPOL bit).
//0 = Active driver output (INTPOL bit sets the polarity).
#define MCP23S17_B_ODR    4
//HAEN: Hardware Address Enable bit (MCP23S17 only).
//1 = Enables the MCP23S17 address pins.
//0 = Disables the MCP23S17 address pins.
#define MCP23S17_B_HAEN   8
//DISSLW: Slew Rate control bit for SDA output.
//1 = Slew rate disabled.
//0 = Slew rate enabled.
#define MCP23S17_B_DISSLW 16
//SEQOP: Sequential Operation mode bit.
//1 = Sequential operation disabled, address pointer does not increment.
//0 = Sequential operation enabled, address pointer increments.
#define MCP23S17_B_SEQOP  32
//MIRROR: INT Pins Mirror bit
//1 = The INT pins are internally connected
//0 = The INT pins are not connected. INTA is associated with PortA and INTB is associated with PortB
#define MCP23S17_B_MIRROR 64
//BANK: Controls how the registers are addressed
//1 = The registers associated with each port are separated into different banks
//0 = The registers are in the same bank (addresses are sequential)
#define MCP23S17_B_BANK   128

typedef union tagMCP23S17U
{
 u8 B[2];
 u16 W;
} MCP23S17U;

typedef struct tagMCP23S17
 {
  u8 SPIAddr;
  u8 CSPin;
  BOOL UseIntAdd;
  u8 SPIChannel;
  SPI_Loc_Conf SPILoc;
  MCP23S17U IODIR;
  MCP23S17U GPIO;
  MCP23S17U GPPU;
  MCP23S17U IPOL;
  MCP23S17U GINP;
 } MCP23S17;


#endif 



/***********************************************************************************
BOOL MCP23S17_write_B(MCP23S17 * Mcp,u8 Addr,u8 Val)

Write One BYTE in the registers

Mcp = Pointer to a  user declared MCP23S17 structure
Addr = Register Address
Val = 8 bit value to write

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_write_B(MCP23S17 * Mcp,u8 Addr,u8 Val)
{
 digitalwrite(Mcp->CSPin, LOW);
 switch (Mcp->SPIChannel)
  {
   case 0:
    SPILoc_Write(&Mcp->SPILoc,((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)&~1);		//Write Into Device
    SPILoc_Write(&Mcp->SPILoc,Addr);
    SPILoc_Write(&Mcp->SPILoc,Val);
    break;
   case 1:
   case 2:
    SPI_write(((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)&~1);		//Write Into Device
    SPI_write(Addr);
    SPI_write(Val);
    break;
  }
 digitalwrite(Mcp->CSPin, HIGH);
 return(TRUE);
}


/***********************************************************************************
BOOL MCP23S17_write_W(MCP23S17 * Mcp,u8 Addr,u16 Val)

Write One WORD (LSB FIRST) in the registers

Mcp = Pointer to a  user declared MCP23S17 structure
Addr = Register Address
Val = 16 bit value to write

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_write_W(MCP23S17 * Mcp,u8 Addr,u16 Val)
{
 digitalwrite(Mcp->CSPin, LOW);
 switch (Mcp->SPIChannel)
  {
   case 0:
    SPILoc_Write(&Mcp->SPILoc,((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)&~1);		//Write Into Device
    SPILoc_Write(&Mcp->SPILoc,Addr);
    SPILoc_Write(&Mcp->SPILoc,Val & 0xFF);
    SPILoc_Write(&Mcp->SPILoc,Val >> 8);
    break;
   case 1:
   case 2:
    SPI_write(((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)&~1);		//Write Into Device
    SPI_write(Addr);
    SPI_write(Val & 0xFF);
    SPI_write(Val >> 8);
    break;
  }
 digitalwrite(Mcp->CSPin, HIGH);
 return(TRUE);
}


/***********************************************************************************
BOOL MCP23S17_write_A(MCP23S17 * Mcp,u8 Addr,u16 Len,char * Val)

Write a variable number of Bytes in the registers

Mcp = Pointer to a  user declared MCP23S17 structure
Addr = Register Address
Len = Numeber of bytes to write
Val = Pointer to a write buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_write_A(MCP23S17 * Mcp,u8 Addr,u16 Len,char * Val)
{
 u16 Ctn;
 if ((Len == 0) || (Val == NULL))
  return(FALSE);
 digitalwrite(Mcp->CSPin, LOW);
 switch (Mcp->SPIChannel)
  {
   case 0:
    SPILoc_Write(&Mcp->SPILoc,((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)&~1);		//Write Into Device
    SPILoc_Write(&Mcp->SPILoc,Addr);
    for (Ctn = 0 ; Ctn < Len ; Ctn++)
    SPILoc_Write(&Mcp->SPILoc,Val[Ctn]);
    break;
   case 1:
   case 2:
    SPI_write(((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)&~1);		//Write Into Device
    SPI_write(Addr);
    for (Ctn = 0 ; Ctn < Len ; Ctn++)
    SPI_write(Val[Ctn]);
    break;
  }
 digitalwrite(Mcp->CSPin, HIGH);
 return(TRUE);
}



/***********************************************************************************
BOOL MCP23S17_read_B(MCP23S17 * Mcp,u8 Addr,u8 * Val)

Read One BYTE from the registers

Mcp = Pointer to a  user declared MCP23S17 structure
Addr = Register Address
Val = Pointer to a 8 bit read buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_read_B(MCP23S17 * Mcp,u8 Addr,u8 * Val)
{
 if (Val == NULL)
  return(FALSE);

 digitalwrite(Mcp->CSPin, LOW);
 switch (Mcp->SPIChannel)
  {
   case 0:
    SPILoc_Write(&Mcp->SPILoc,((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)|1);		//Write Into Device
    SPILoc_Write(&Mcp->SPILoc,Addr);
    *Val = SPILoc_Read(&Mcp->SPILoc);
    break;
   case 1:
   case 2:
    SPI_write(((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)|1);		//Write Into Device
    SPI_write(Addr);
    *Val = SPI_read();
    break;
  }
 digitalwrite(Mcp->CSPin, HIGH);
 return(TRUE);
}

/***********************************************************************************
BOOL MCP23S17_read_W(MCP23S17 * Mcp,u8 Addr,u16 * Val)

Read One WORD (LSB FIRST) from the registers

Mcp = Pointer to a  user declared MCP23S17 structure
Addr = Register Address
Val = Pointer to a 16 bit read buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_read_W(MCP23S17 * Mcp,u8 Addr,u16 * Val)
{
 u16 TVal;
 u8 BVal;
char Str[20];

 if (Val == NULL)
  return(FALSE);

 digitalwrite(Mcp->CSPin, LOW);
 switch (Mcp->SPIChannel)
  {
   case 0:
    SPILoc_Write(&Mcp->SPILoc,((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)|1);		//Write Into Device
    SPILoc_Write(&Mcp->SPILoc,Addr);
    BVal = SPILoc_Read(&Mcp->SPILoc);
    TVal = SPILoc_Read(&Mcp->SPILoc);
    break;
   case 1:
   case 2:
    SPI_write(((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)|1);		//Write Into Device
    SPI_write(Addr);
    BVal = SPI_read();
    TVal = SPI_read();
    break;
  }
 digitalwrite(Mcp->CSPin, HIGH);
 TVal <<= 8;
 TVal |= BVal;
 *Val = TVal;
 return(TRUE);
}


/***********************************************************************************
void MCP23S17_read_A(MCP23S17 * Mcp,u8 Addr,u8 Len,char * Val)

Read a variable number of BYTE from the registers

Mcp = Pointer to a  user declared MCP23S17 structure
Addr = Register Address
Len = Numeber of bytes to read
Val = Pointer to a read buffer

RETURN FALSE on Error
***********************************************************************************/
void MCP23S17_read_A(MCP23S17 * Mcp,u8 Addr,u8 Len,char * Val)
{
 u16 Ctn;
 if ((Len == 0) || (Val == NULL))
  return(FALSE);

 digitalwrite(Mcp->CSPin, LOW);
 switch (Mcp->SPIChannel)
  {
   case 0:
    SPILoc_Write(&Mcp->SPILoc,((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)|1);		//Write Into Device
    SPILoc_Write(&Mcp->SPILoc,Addr);
    for (Ctn = 0 ; Ctn < Len ; Ctn++)
     Val[Ctn] = SPILoc_Read(&Mcp->SPILoc);
    break;
   case 1:
   case 2:
    SPI_write(((MCP23S17_ADDRESS | (Mcp->SPIAddr&7))<<1)|1);		//Write Into Device
    SPI_write(Addr);
    for (Ctn = 0 ; Ctn < Len ; Ctn++)
     Val[Ctn] = SPI_read();
    break;
  }
 digitalwrite(Mcp->CSPin, HIGH);
 return(TRUE);
}







/***********************************************************************************
BOOL MCP23S17_init(MCP23S17 * Mcp, u8 SPIAddr, u8 CSPin, BOOL UseIntAdd, u8 SPIChannel, u8 LoSCK, u8 LoSDO, u8 LoSDI)

Init a MCP23S17 device and structure

Mcp = Pointer to a  user declared MCP23S17 structure
SPIAddr = Internal adress of the MCP23S17 device
CSPin = CS Pin of the device
UseIntAdd = Enable use of Internal Address (TRUE = YES)
SPIChannel = SPI Channel to use (0 = Emulated SPI, 1 = SPI1)
LoSCK = SCK Pin of emulated SPI port
LoSDO = SDO Pin of emulated SPI port
LoSDI = SDI Pin of emulated SPI port

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_init(MCP23S17 * Mcp, u8 SPIAddr, u8 CSPin, BOOL UseIntAdd, u8 SPIChannel, u8 LoSCK, u8 LoSDO, u8 LoSDI)
{
 u8 Val;

 Mcp->SPIAddr = SPIAddr;
 Mcp->CSPin = CSPin;
 Mcp->UseIntAdd = UseIntAdd;
 Mcp->SPIChannel = SPIChannel;

 // Initialize the chip enable, HIGH
 digitalwrite(Mcp->CSPin, HIGH);
 pinmode(Mcp->CSPin, OUTPUT);
 switch (Mcp->SPIChannel)
  {
   case 0:
    Mcp->SPILoc.CPOL = FALSE;    // Clock Polarity
    Mcp->SPILoc.CPHA = TRUE;    // Sample position
    Mcp->SPILoc.DelayuS = 0;    // Delay Clock uS
    Mcp->SPILoc.LoSCK = LoSCK;
    Mcp->SPILoc.LoSDO = LoSDO;
    Mcp->SPILoc.LoSDI = LoSDI;
    SPILoc_Init (&Mcp->SPILoc);
    break;
   case 1:
   case 2:
    SPI_init();
    break;
  }
 Mcp->IODIR.W = 0xFFFF;
 Mcp->GPIO.W = 0;
 Mcp->GPPU.W = 0;
 Mcp->IPOL.W = 0;
 Val = 0;
 if (UseIntAdd == TRUE)
  Val |= MCP23S17_B_HAEN;
 if(MCP23S17_write_B(Mcp,MCP23S17_INTCONA,Val) == FALSE)
  return(FALSE);
 if(MCP23S17_write_W(Mcp,MCP23S17_IODIRA,0xFFFF) == FALSE)
  return(FALSE);
 return(TRUE);
}





/***********************************************************************************
BOOL MCP23S17_readGPIO_A(u8 * Val, MCP23S17 * Mcp)

Read input port A

Val = Pointer to a 8 bit read buffer
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_readGPIO_A(u8 * Val, MCP23S17 * Mcp)
{
 BOOL Risu;
 Risu = MCP23S17_read_B(Mcp,MCP23S17_GPIOA,Val);
 Mcp->GINP.B[0] = Val;
 return (Risu);
}

/***********************************************************************************
BOOL MCP23S17_readGPIO_B(u8 * Val, MCP23S17 * Mcp)

Read input port B

Val = Pointer to a 8 bit read buffer
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_readGPIO_B(u8 * Val, MCP23S17 * Mcp)
{
 BOOL Risu;
 Risu = MCP23S17_read_B(Mcp,MCP23S17_GPIOB,Val);
 Mcp->GINP.B[1] = Val;
 return (Risu);
}

/***********************************************************************************
BOOL MCP23S17_readGPIO_AB(u8 * Val, MCP23S17 * Mcp)

Read input port AB

Val = Pointer to a 16 bit read buffer
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_readGPIO_AB(u16 * Val, MCP23S17 * Mcp)
{
 BOOL Risu;
 Risu = MCP23S17_read_W(Mcp,MCP23S17_GPIOA,Val);
 Mcp->GINP.W = Val;
 return (Risu);
}





/***********************************************************************************
BOOL MCP23S17_writeGPIO_A(u8 Val, MCP23S17 * Mcp)

Write output port A

Val = 8 bit value to write
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeGPIO_A(u8 Val, MCP23S17 * Mcp)
{
 Mcp->GPIO.B[0] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_GPIOA,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeGPIO_B(u8 Val, MCP23S17 * Mcp)

Write output port B

Val = 8 bit value to write
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeGPIO_B(u8 Val, MCP23S17 * Mcp)
{
 Mcp->GPIO.B[1] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_GPIOB,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeGPIO_AB(u8 Val, MCP23S17 * Mcp)

Write output port AB

Val = 16 bit value to write
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeGPIO_AB(u16 Val, MCP23S17 * Mcp)
{
 Mcp->GPIO.W = Val;
 return (MCP23S17_write_W(Mcp,MCP23S17_GPIOA,Val));
}





/***********************************************************************************
BOOL MCP23S17_writeIODIR_A(u8 Val, MCP23S17 * Mcp)

Write Input/Output direction register of Port A

Val = 8 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeIODIR_A(u8 Val, MCP23S17 * Mcp)
{
 Mcp->IODIR.B[0] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_IODIRA,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeIODIR_B(u8 Val, MCP23S17 * Mcp)

Write Input/Output direction register of Port B

Val = 8 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeIODIR_B(u8 Val, MCP23S17 * Mcp)
{
 Mcp->IODIR.B[1] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_IODIRB,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeIODIR_AB(u8 Val, MCP23S17 * Mcp)

Write Input/Output direction register of Port AB

Val = 16 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeIODIR_AB(u16 Val, MCP23S17 * Mcp)
{
 Mcp->IODIR.W = Val;
 return (MCP23S17_write_W(Mcp,MCP23S17_IODIRA,Val));
}





/***********************************************************************************
BOOL MCP23S17_writeGPPU_A(u8 Val, MCP23S17 * Mcp)

Write Resistor PullUp register of Port A

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeGPPU_A(u8 Val, MCP23S17 * Mcp)
{
 Mcp->GPPU.B[0] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_GPPUA,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeGPPU_B(u8 Val, MCP23S17 * Mcp)

Write Resistor PullUp register of Port B

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeGPPU_B(u8 Val, MCP23S17 * Mcp)
{
 Mcp->GPPU.B[1] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_GPPUB,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeGPPU_AB(u8 Val, MCP23S17 * Mcp)

Write Resistor PullUp register of Port AB

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeGPPU_AB(u16 Val, MCP23S17 * Mcp)
{
 Mcp->GPPU.W = Val;
 return (MCP23S17_write_W(Mcp,MCP23S17_GPPUA,Val));
}




/***********************************************************************************
BOOL MCP23S17_writeIPOL_A(u8 Val, MCP23S17 * Mcp)

Write INPUT POLARITY register of Port A

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeIPOL_A(u8 Val, MCP23S17 * Mcp)
{
 Mcp->IPOL.B[0] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_IPOLA,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeIPOL_B(u8 Val, MCP23S17 * Mcp)

Write INPUT POLARITY register of Port B

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeIPOL_B(u8 Val, MCP23S17 * Mcp)
{
 Mcp->IPOL.B[1] = Val;
 return (MCP23S17_write_B(Mcp,MCP23S17_IPOLB,Val));
}

/***********************************************************************************
BOOL MCP23S17_writeIPOL_AB(u8 Val, MCP23S17 * Mcp)

Write INPUT POLARITY register of Port AB

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_writeIPOL_AB(u16 Val, MCP23S17 * Mcp)
{
 Mcp->IPOL.W = Val;
 return (MCP23S17_write_W(Mcp,MCP23S17_IPOLA,Val));
}






/***********************************************************************************
u8 MCP23S17_digitalRead(u8 Pin, MCP23S17 * Mcp)

Read the status of a Input

Pin = Input to read (from 1 to 16)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN Input Status 0 = LOW, 1 = HIGH, 0xFF=Error
***********************************************************************************/
u8 MCP23S17_digitalRead(u8 Pin, MCP23S17 * Mcp)
{

 Pin--;
 if (Pin > 15)
  return(0xFF);
 if (Pin < 8)
  {
   if (MCP23S17_read_B(Mcp,MCP23S17_GPIOA,&(Mcp->GINP.B[0])) == FALSE)
    return(0xFF);
   return ((Mcp->GINP.B[0] >> Pin) & 0x1);
  }
 else
  {
   Pin -= 8;
   if (MCP23S17_read_B(Mcp,MCP23S17_GPIOB,&(Mcp->GINP.B[1])) == FALSE)
    return(0xFF);
   return ((Mcp->GINP.B[1] >> Pin) & 0x1);
  }
}







/***********************************************************************************
BOOL MCP23S17_pinMode(u8 Pin, u8 OutIn,MCP23S17 * Mcp)

Define if a pin is an Inpur or a Output

Pin = Pin on which operate (from 1 to 16)
OutIn = (INPUT = Input Pin, OUTPUT = Output Pin)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_pinMode(u8 Pin, u8 OutIn,MCP23S17 * Mcp)
{
 Pin--;
 if (Pin > 15)
  return(FALSE);
 if (Pin < 8)
  {
   if (OutIn == INPUT)
    Mcp->IODIR.B[0] |= 1 << Pin; 
   else
    Mcp->IODIR.B[0] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_IODIRA,Mcp->IODIR.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (OutIn == INPUT)
    Mcp->IODIR.B[1] |= 1 << Pin; 
   else
    Mcp->IODIR.B[1] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_IODIRB,Mcp->IODIR.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}



/***********************************************************************************
BOOL MCP23S17_pullUp(u8 Pin, u8 SPull, MCP23S17 * Mcp)

Define the status of a Resistor PullUp

Pin = Pin on which operate (from 1 to 16)
SPull = (HIGH = PullUp Enabled, LOW = PullUp Disabled)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_pullUp(u8 Pin, u8 SPull, MCP23S17 * Mcp)
{
 Pin--;
 if (Pin > 15)
  return(FALSE);
 if (Pin < 8)
  {
   if (SPull == HIGH)
    Mcp->GPPU.B[0] |= 1 << Pin; 
   else
    Mcp->GPPU.B[0] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_GPPUA,Mcp->GPPU.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (SPull == HIGH)
    Mcp->GPPU.B[1] |= 1 << Pin; 
   else
    Mcp->GPPU.B[1] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_GPPUB,Mcp->GPPU.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}



/***********************************************************************************
BOOL MCP23S17_inputPol(u8 Pin, u8 SInPol, MCP23S17 * Mcp)

Define the status of a Input Polarity

Pin = Pin on which operate (from 1 to 16)
SInPol = (HIGH = Input Inverted, LOW = Input NOT Inverted)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_inputPol(u8 Pin, u8 SInPol, MCP23S17 * Mcp)
{
 Pin--;
 if (Pin > 15)
  return(FALSE);
 if (Pin < 8)
  {
   if (SInPol == HIGH)
    Mcp->IPOL.B[0] |= 1 << Pin; 
   else
    Mcp->IPOL.B[0] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_IPOLA,Mcp->IPOL.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (SInPol == HIGH)
    Mcp->IPOL.B[1] |= 1 << Pin; 
   else
    Mcp->IPOL.B[1] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_IPOLB,Mcp->IPOL.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}



/***********************************************************************************
BOOL MCP23S17_digitalWrite(u8 Pin, u8 SOut, MCP23S17 * Mcp)

Write the status of a Output

Pin = Output to write (from 1 to 16)
SOut = (HIGH = Output high, LOW = Output low)
Mcp = Pointer to a  user declared MCP23S17 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23S17_digitalWrite(u8 Pin, u8 SOut, MCP23S17 * Mcp)
{
 Pin--;
 if (Pin > 15)
  return(FALSE);
 if (Pin < 8)
  {
   if (SOut == HIGH)
    Mcp->GPIO.B[0] |= 1 << Pin; 
   else
    Mcp->GPIO.B[0] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_GPIOA,Mcp->GPIO.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (SOut == HIGH)
    Mcp->GPIO.B[1] |= 1 << Pin; 
   else
    Mcp->GPIO.B[1] &= ~(1 << Pin);
   if (MCP23S17_write_B(Mcp,MCP23S17_GPIOB,Mcp->GPIO.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}





