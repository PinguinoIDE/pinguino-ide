/*
 * File			MCP23017.c
 *
 * Description		Support for Microchip MCP23017 16-Bit I/O Expander with I2C Interface
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

#ifndef _MCP23017_H_
#define _MCP23017_H_


#define MCP23017_ADDRESS 0x20

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


#endif  

#include <i2c1.c>

u16 MCP23017_Fault_V;

//Stop I2C after error
BOOL MCP23017_Fault(u16 Valo)
{
 MCP23017_Fault_V = Valo;
 I2C1_stop();
 return(FALSE); 
}


/***********************************************************************************
BOOL MCP23017_write_B(MCP23017 * Mcp,u8 Addr,u8 Val)

Write One BYTE in the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = 8 bit value to write

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_write_B(MCP23017 * Mcp,u8 Addr,u8 Val)
{
 I2C1_start();
 if (I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP23017_Fault(1));
 if (I2C1_writechar(Addr) == FALSE)
  return(MCP23017_Fault(2));
 if (I2C1_writechar(Val) == FALSE)
  return(MCP23017_Fault(3));
 I2C1_stop();
 return(TRUE);
}


/***********************************************************************************
BOOL MCP23017_write_W(MCP23017 * Mcp,u8 Addr,u16 Val)

Write One WORD (LSB FIRST) in the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = 16 bit value to write

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_write_W(MCP23017 * Mcp,u8 Addr,u16 Val)
{
 I2C1_start();
 if (I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP23017_Fault(1));
 if (I2C1_writechar(Addr) == FALSE)
  return(MCP23017_Fault(2));
 if (I2C1_writechar(Val & 0xFF) == FALSE)
  return(MCP23017_Fault(3));
 if (I2C1_writechar(Val >> 8) == FALSE)
  return(MCP23017_Fault(4));
 I2C1_stop();
 return(TRUE);
}


/***********************************************************************************
BOOL MCP23017_write_A(MCP23017 * Mcp,u8 Addr,u16 Len,char * Val)

Write a variable number of Bytes in the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Len = Numeber of bytes to write
Val = Pointer to a write buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_write_A(MCP23017 * Mcp,u8 Addr,u16 Len,char * Val)
{
 u16 Ctn;
 if ((Len == 0) || (Val == NULL))
  return(FALSE);
 I2C1_start();
 if (I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP23017_Fault(1));
 if (I2C1_writechar(Addr) == FALSE)
  return(MCP23017_Fault(2));
 for (Ctn = 0 ; Ctn < Len ; Ctn++)
  if (I2C1_writechar((u8) Val[Ctn]) == FALSE)
   return(MCP23017_Fault(3));
 I2C1_stop();
 return(TRUE);
}



/***********************************************************************************
BOOL MCP23017_read_B(MCP23017 * Mcp,u8 Addr,u8 * Val)

Read One BYTE from the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = Pointer to a 8 bit read buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_read_B(MCP23017 * Mcp,u8 Addr,u8 * Val)
{
 if (Val == NULL)
  return(FALSE);
 I2C1_start();
 if (I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP23017_Fault(1));
 if (I2C1_writechar(Addr) == FALSE)	
  return(MCP23017_Fault(2));
 I2C1_restart();
 I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)|1);		//Read From Device		
 *Val = I2C1_readchar();
 I2C1_stop();
 return(TRUE);
}


/***********************************************************************************
BOOL MCP23017_read_W(MCP23017 * Mcp,u8 Addr,u16 * Val)

Read One WORD (LSB FIRST) from the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Val = Pointer to a 16 bit read buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_read_W(MCP23017 * Mcp,u8 Addr,u16 * Val)
{
 u16 TVal;
 u8 BVal;

 if (Val == NULL)
  return(FALSE);
 I2C1_start();
 if (I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP23017_Fault(1));
 if (I2C1_writechar(Addr) == FALSE)	
  return(MCP23017_Fault(2));
 I2C1_restart();
 I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)|1);		//Read From Device		
 BVal = I2C1_readcharAck();
 TVal = I2C1_readchar();
 I2C1_stop();
 TVal <<= 8;
 TVal |= BVal;
 *Val = TVal;
 return(TRUE);
}


/***********************************************************************************
BOOL MCP23017_read_A(MCP23017 * Mcp,u8 Addr,u8 Len,char * Val)

Read a variable number of BYTE from the registers

Mcp = Pointer to a  user declared MCP23017 structure
Addr = Register Address
Len = Numeber of bytes to read
Val = Pointer to a read buffer

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_read_A(MCP23017 * Mcp,u8 Addr,u8 Len,char * Val)
{
 u16 Ctn;
 if ((Len == 0) || (Val == NULL))
  return(FALSE);
 // read the current IODIR
 I2C1_start();
 if (I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP23017_Fault(1));
 if (I2C1_writechar(Addr) == FALSE)	
  return(MCP23017_Fault(2));
 I2C1_restart();
 I2C1_writechar(((MCP23017_ADDRESS | (Mcp->I2CAddr&7))<<1)|1);		//Read From Device		
 for (Ctn = 0 ; Ctn < Len ; Ctn++)
  {
   if (Ctn == Len-1)
    Val[Ctn] = I2C1_readchar();
   else
    Val[Ctn] = I2C1_readcharAck();
  }
 I2C1_stop();
 return(TRUE);
}








/***********************************************************************************
BOOL MCP23017_init(MCP23017 * Mcp,u8 I2CAddr)

Init a MCP23017 device and structure

Mcp = Pointer to a  user declared MCP23017 structure
I2CAddr = Internal adress of the MCP23017 device

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_init(MCP23017 * Mcp,u8 I2CAddr)
{
 Mcp->I2CAddr = I2CAddr;
 Mcp->IODIR.W = 0xFFFF;
 Mcp->GPIO.W = 0;
 Mcp->GPPU.W = 0;
 Mcp->IPOL.W = 0;
 I2C1_init();
 if(MCP23017_write_W(Mcp,MCP23017_IODIRA,0xFFFF) == FALSE)
  return(FALSE);
 return(TRUE);
}





/***********************************************************************************
BOOL MCP23017_readGPIO_A(u8 * Val, MCP23017 * Mcp)

Read input port A

Val = Pointer to a 8 bit read buffer
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_readGPIO_A(u8 * Val, MCP23017 * Mcp)
{
 BOOL Risu;
 Risu = MCP23017_read_B(Mcp,MCP23017_GPIOA,Val);
 Mcp->GINP.B[0] = Val;
 return (Risu);
}

/***********************************************************************************
BOOL MCP23017_readGPIO_B(u8 * Val, MCP23017 * Mcp)

Read input port B

Val = Pointer to a 8 bit read buffer
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_readGPIO_B(u8 * Val, MCP23017 * Mcp)
{
 BOOL Risu;
 Risu = MCP23017_read_B(Mcp,MCP23017_GPIOB,Val);
 Mcp->GINP.B[1] = Val;
 return (Risu);
}

/***********************************************************************************
BOOL MCP23017_readGPIO_AB(u8 * Val, MCP23017 * Mcp)

Read input port AB

Val = Pointer to a 16 bit read buffer
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_readGPIO_AB(u16 * Val, MCP23017 * Mcp)
{
 BOOL Risu;
 Risu = MCP23017_read_W(Mcp,MCP23017_GPIOA,Val);
 Mcp->GINP.W = Val;
 return (Risu);
}





/***********************************************************************************
BOOL MCP23017_writeGPIO_A(u8 Val, MCP23017 * Mcp)

Write output port A

Val = 8 bit value to write
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeGPIO_A(u8 Val, MCP23017 * Mcp)
{
 Mcp->GPIO.B[0] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_GPIOA,Val));
}

/***********************************************************************************
BOOL MCP23017_writeGPIO_B(u8 Val, MCP23017 * Mcp)

Write output port B

Val = 8 bit value to write
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeGPIO_B(u8 Val, MCP23017 * Mcp)
{
 Mcp->GPIO.B[1] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_GPIOB,Val));
}

/***********************************************************************************
BOOL MCP23017_writeGPIO_AB(u8 Val, MCP23017 * Mcp)

Write output port AB

Val = 16 bit value to write
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeGPIO_AB(u16 Val, MCP23017 * Mcp)
{
 Mcp->GPIO.W = Val;
 return (MCP23017_write_W(Mcp,MCP23017_GPIOA,Val));
}





/***********************************************************************************
BOOL MCP23017_writeIODIR_A(u8 Val, MCP23017 * Mcp)

Write Input/Output direction register of Port A

Val = 8 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeIODIR_A(u8 Val, MCP23017 * Mcp)
{
 Mcp->IODIR.B[0] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_IODIRA,Val));
}

/***********************************************************************************
BOOL MCP23017_writeIODIR_B(u8 Val, MCP23017 * Mcp)

Write Input/Output direction register of Port B

Val = 8 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeIODIR_B(u8 Val, MCP23017 * Mcp)
{
 Mcp->IODIR.B[1] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_IODIRB,Val));
}

/***********************************************************************************
BOOL MCP23017_writeIODIR_AB(u8 Val, MCP23017 * Mcp)

Write Input/Output direction register of Port AB

Val = 16 bit value to write (1 = Input, 0 = Output)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeIODIR_AB(u16 Val, MCP23017 * Mcp)
{
 Mcp->IODIR.W = Val;
 return (MCP23017_write_W(Mcp,MCP23017_IODIRA,Val));
}





/***********************************************************************************
BOOL MCP23017_writeGPPU_A(u8 Val, MCP23017 * Mcp)

Write Resistor PullUp register of Port A

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeGPPU_A(u8 Val, MCP23017 * Mcp)
{
 Mcp->GPPU.B[0] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_GPPUA,Val));
}

/***********************************************************************************
BOOL MCP23017_writeGPPU_B(u8 Val, MCP23017 * Mcp)

Write Resistor PullUp register of Port B

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeGPPU_B(u8 Val, MCP23017 * Mcp)
{
 Mcp->GPPU.B[1] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_GPPUB,Val));
}

/***********************************************************************************
BOOL MCP23017_writeGPPU_AB(u8 Val, MCP23017 * Mcp)

Write Resistor PullUp register of Port AB

Val = 8 bit value to write (1 = Pull-up enabled, 0 = Pull-up disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeGPPU_AB(u16 Val, MCP23017 * Mcp)
{
 Mcp->GPPU.W = Val;
 return (MCP23017_write_W(Mcp,MCP23017_GPPUA,Val));
}




/***********************************************************************************
BOOL MCP23017_writeIPOL_A(u8 Val, MCP23017 * Mcp)

Write INPUT POLARITY register of Port A

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeIPOL_A(u8 Val, MCP23017 * Mcp)
{
 Mcp->IPOL.B[0] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_IPOLA,Val));
}

/***********************************************************************************
BOOL MCP23017_writeIPOL_B(u8 Val, MCP23017 * Mcp)

Write INPUT POLARITY register of Port B

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeIPOL_B(u8 Val, MCP23017 * Mcp)
{
 Mcp->IPOL.B[1] = Val;
 return (MCP23017_write_B(Mcp,MCP23017_IPOLB,Val));
}

/***********************************************************************************
BOOL MCP23017_writeIPOL_AB(u8 Val, MCP23017 * Mcp)

Write INPUT POLARITY register of Port AB

Val = 8 bit value to write (1 = GPIO register bit will reflect the opposite logic state of the input pin, 0 = GPIO register bit will reflect the same logic state of the input pin.)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_writeIPOL_AB(u16 Val, MCP23017 * Mcp)
{
 Mcp->IPOL.W = Val;
 return (MCP23017_write_W(Mcp,MCP23017_IPOLA,Val));
}






/***********************************************************************************
u8 MCP23017_digitalRead(u8 Pin, MCP23017 * Mcp)

Read the status of a Input

Pin = Input to read (from 1 to 16)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN Input Status 0 = LOW, 1 = HIGH, 0xFF=Error
***********************************************************************************/
u8 MCP23017_digitalRead(u8 Pin, MCP23017 * Mcp)
{

 Pin--;
 if (Pin > 15)
  return(0xFF);
 if (Pin < 8)
  {
   if (MCP23017_read_B(Mcp,MCP23017_GPIOA,&(Mcp->GINP.B[0])) == FALSE)
    return(0xFF);
   return ((Mcp->GINP.B[0] >> Pin) & 0x1);
  }
 else
  {
   Pin -= 8;
   if (MCP23017_read_B(Mcp,MCP23017_GPIOB,&(Mcp->GINP.B[1])) == FALSE)
    return(0xFF);
   return ((Mcp->GINP.B[1] >> Pin) & 0x1);
  }

}







/***********************************************************************************
BOOL MCP23017_pinMode(u8 Pin, u8 OutIn,MCP23017 * Mcp)

Define if a pin is an Inpur or a Output

Pin = Pin on which operate (from 1 to 16)
OutIn = (INPUT = Input Pin, OUTPUT = Output Pin)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_pinMode(u8 Pin, u8 OutIn,MCP23017 * Mcp)
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
   if (MCP23017_write_B(Mcp,MCP23017_IODIRA,Mcp->IODIR.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (OutIn == INPUT)
    Mcp->IODIR.B[1] |= 1 << Pin; 
   else
    Mcp->IODIR.B[1] &= ~(1 << Pin);
   if (MCP23017_write_B(Mcp,MCP23017_IODIRB,Mcp->IODIR.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}



/***********************************************************************************
BOOL MCP23017_pullUp(u8 Pin, u8 SPull, MCP23017 * Mcp)

Define the status of a Resistor PullUp

Pin = Pin on which operate (from 1 to 16)
SPull = (HIGH = PullUp Enabled, LOW = PullUp Disabled)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_pullUp(u8 Pin, u8 SPull, MCP23017 * Mcp)
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
   if (MCP23017_write_B(Mcp,MCP23017_GPPUA,Mcp->GPPU.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (SPull == HIGH)
    Mcp->GPPU.B[1] |= 1 << Pin; 
   else
    Mcp->GPPU.B[1] &= ~(1 << Pin);
   if (MCP23017_write_B(Mcp,MCP23017_GPPUB,Mcp->GPPU.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}



/***********************************************************************************
BOOL MCP23017_inputPol(u8 Pin, u8 SInPol, MCP23017 * Mcp)

Define the status of a Input Polarity

Pin = Pin on which operate (from 1 to 16)
SInPol = (HIGH = Input Inverted, LOW = Input NOT Inverted)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_inputPol(u8 Pin, u8 SInPol, MCP23017 * Mcp)
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
   if (MCP23017_write_B(Mcp,MCP23017_IPOLA,Mcp->IPOL.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (SInPol == HIGH)
    Mcp->IPOL.B[1] |= 1 << Pin; 
   else
    Mcp->IPOL.B[1] &= ~(1 << Pin);
   if (MCP23017_write_B(Mcp,MCP23017_IPOLB,Mcp->IPOL.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}



/***********************************************************************************
BOOL MCP23017_digitalWrite(u8 Pin, u8 SOut, MCP23017 * Mcp)

Write the status of a Output

Pin = Output to write (from 1 to 16)
SOut = (HIGH = Output high, LOW = Output low)
Mcp = Pointer to a  user declared MCP23017 structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP23017_digitalWrite(u8 Pin, u8 SOut, MCP23017 * Mcp)
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
   if (MCP23017_write_B(Mcp,MCP23017_GPIOA,Mcp->GPIO.B[0]) == FALSE)
    return(FALSE);
  }
 else
  {
   Pin -= 8;
   if (SOut == HIGH)
    Mcp->GPIO.B[1] |= 1 << Pin; 
   else
    Mcp->GPIO.B[1] &= ~(1 << Pin);
   if (MCP23017_write_B(Mcp,MCP23017_GPIOB,Mcp->GPIO.B[1]) == FALSE)
    return(FALSE);
  }
 return (TRUE);
}





