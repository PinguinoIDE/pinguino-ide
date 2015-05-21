/***********************************************************************************

 * File			MCP342x.c
 *
 * Description		Support for Microchip MCP34x 1/2/4 Channels 12/14/16/18 Bit differential Delta-Sigma ADC with I2C interface
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License		This software is released under the terms of the Mozilla Public License (MPL) version 2.0
 * 			Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 *
 *   27/03/2014 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it ) 
 
 SUPPORTED DEVICES
 
MCP3421
1 channel 12 / 14 / 16 / 18 bit conversion (No HW Address, Use Addr 0)
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en520011
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22003e.pdf

MCP3422
2 channel 12 / 14 / 16 / 18 bit conversion (No HW Address, Use Addr 0)
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en538232
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22088c.pdf

MCP3423
2 channel 12 / 14 / 16 / 18 bit conversion
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en538233
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22088c.pdf

MCP3424
4 channel 12 / 14 / 16 / 18 bit conversion
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en536354
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22088c.pdf

MCP3425
1 channel 12 / 14 / 16 bit conversion (No HW Address, Use Addr 0)
!!!! DO NOT USE MCP342x_18_BIT !!!!
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en533561
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22072b.pdf

MCP3426
2 channel 12 / 14 / 16 bit conversion (No HW Address, Use Addr 0)
!!!! DO NOT USE MCP342x_18_BIT !!!!
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en545863
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22226a.pdf

MCP3427
2 channel 12 / 14 / 16 bit conversion
!!!! DO NOT USE MCP342x_18_BIT !!!!
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en545864
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22226a.pdf

MCP3428
4 channel 12 / 14 / 16 bit conversion
!!!! DO NOT USE MCP342x_18_BIT !!!!
http://www.microchip.com/wwwproducts/Devices.aspx?dDocName=en545865
DataSheet http://ww1.microchip.com/downloads/en/DeviceDoc/22226a.pdf
***********************************************************************************/


#ifndef MCP342x_h
#define MCP342x_h

#include <i2c1.c> 
#include <millis.c>

#ifdef DEBUGI2C
#include <__cdc.c> 
#endif

#define MCP342x_ADDRESS 0X68

#define MCP342x_2048mVFS    0X00 // PGA Gain 1 Max +-2048mV
#define MCP342x_1024mVFS    0X01 // PGA Gain 2 Max +-1024mV
#define MCP342x_512mVFS     0X02 // PGA Gain 4 Max +-512mV
#define MCP342x_256mVFS     0X03 // PGA Gain 8 Max +-256mV

#define MCP342x_12_BIT     0X00*4 // 12-bit 240 SPS
#define MCP342x_14_BIT     0X01*4 // 14-bit 60 SPS
#define MCP342x_16_BIT     0X02*4 // 16-bit 15 SPS
#define MCP342x_18_BIT     0X03*4 // 18-bit 3.75 SPS

#define MCP342x_12_BIT_NV  1000000 // 12-bit nV Point
#define MCP342x_14_BIT_NV   250000 // 14-bit nV Point
#define MCP342x_16_BIT_NV    62500 // 16-bit nV Point
#define MCP342x_18_BIT_NV    15625 // 18-bit nV Point

#define MCP342x_12_BIT_MS        5 // 12-bit mS Conversion Time
#define MCP342x_14_BIT_MS       17 // 14-bit mS Conversion Time
#define MCP342x_16_BIT_MS       67 // 16-bit mS Conversion Time
#define MCP342x_18_BIT_MS      267 // 18-bit mS Conversion Time

#define MCP342x_CONTINUOUS 0X10 // 1 = continuous, 0 = one-shot

#define MCP342x_CHANNEL_1  0X00 // select MUX channel 1
#define MCP342x_CHANNEL_2  0X20 // select MUX channel 2
#define MCP342x_CHANNEL_3  0X40 // select MUX channel 3
#define MCP342x_CHANNEL_4  0X60 // select MUX channel 4

#define MCP342x_START      0X80 // write: start a conversion
#define MCP342x_BUSY       0X80 // read: output not ready

#define MCP342x_NORE_P     0x00FFFFFF
#define MCP342x_NORE_N     2100000000

#define MCP342x_POINTS     TRUE
#define MCP342x_NV         FALSE

#endif


typedef struct tagMCP342xCH
 {
  BOOL On;              //Channel in use
  u8 Gain;              //Selected Gain MCP342x_GAIN_X1, MCP342x_GAIN_X2, MCP342x_GAIN_X4 or MCP342x_GAIN_X8
  u8 Res;               //Channel resolution MCP342x_12_BIT, MCP342x_14_BIT, MCP342x_16_BIT or MCP342x_18_BIT
  int Points;           //Last lecture in points (0x00FFFFFF = No Lecture)
  int nV;               //Last lecture in nV (2.100.000.000 = No Lecture)
 } MCP342xCH; 

typedef struct tagMCP342x
 {
  BOOL On;              //Conversion Enabled
  BOOL InSample;
  u8 NumCh;             //Number of Channels (1, 2 or 4)
  u8 I2CAddr;           //Device I2C Address
  u64 MilliSec;         //Timer
  u8 CtErr;             //Timeout Counter
  char FrameRx[4];      //Last received frame
  
  MCP342xCH Ch[4];      //Channels configuration and Data
  
  u8 ActCh;             //Actual reading channel
 } MCP342x; 


//Stop I2C after error
BOOL MCP342x_Fault(u16 Valo)
{
 I2C1_stop();
 return(FALSE); 
}

 
/***********************************************************************************
BOOL MCP342x_write(MCP342x * Mcp,u8 Val)

Write One BYTE in the registers

Mcp = Pointer to a  user declared MCP342x structure
Val = 8 bit value to write

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP342x_write(MCP342x * Mcp,u8 Val)
{
 I2C1_start();
 if (I2C1_writechar(((MCP342x_ADDRESS | (Mcp->I2CAddr&7))<<1)&~1) == FALSE)		//Write Into Device
  return(MCP342x_Fault(1));
 if (I2C1_writechar(Val) == FALSE)
  return(MCP342x_Fault(2));
 I2C1_stop();
 return(TRUE);
}

 
/***********************************************************************************
BOOL MCP342x_read_Data(MCP342x * Mcp)

Read a variable number of BYTE from the registers

Mcp = Pointer to a  user declared MCP342x structure

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP342x_read_Data(MCP342x * Mcp)
{
 u8 Va;
// char Str[20];
 u16 Ctn;

 I2C1_start();
 if (I2C1_writechar(((MCP342x_ADDRESS | (Mcp->I2CAddr&7))<<1)|1) == FALSE)		//Write Into Device
  return(MCP342x_Fault(1));
 Mcp->FrameRx[3] = I2C1_readcharAck();
 Mcp->FrameRx[2] = I2C1_readcharAck();
 Mcp->FrameRx[1] = I2C1_readcharAck();
 Mcp->FrameRx[0] = I2C1_readchar();
 I2C1_stop();
 return(TRUE);
} 


/***********************************************************************************
BOOL MCP342x_init(MCP342x * Mcp, u8 I2CAddr, u8 NumCh)

Init a MCP342x device

Mcp = Pointer to a  user declared MCP342x structure
I2CAddr = I2C device address
NumCh = Number of channels (MCP3421 =  1 , MCP3422 =  2 , MCP3423 = 2 , MCP3424 = 4 , MCP3425 =  1 , MCP3426 =  2 , MCP3427 = 2 , MCP3428 = 4)

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP342x_init(MCP342x * Mcp, u8 I2CAddr, u8 NumCh)
 {
  if ( (Mcp == NULL) || (I2CAddr > 7) || (NumCh > 4)  || (NumCh < 1))
   return (FALSE);
  Mcp->On = FALSE;
  Mcp->InSample = FALSE;
  Mcp->I2CAddr = I2CAddr;
  Mcp->NumCh = NumCh;
  Mcp->ActCh = 0;
  Mcp->Ch[0].On = Mcp->Ch[1].On = Mcp->Ch[2].On = Mcp->Ch[3].On = FALSE;
  Mcp->Ch[0].Gain = Mcp->Ch[1].Gain = Mcp->Ch[2].Gain = Mcp->Ch[3].Gain = MCP342x_2048mVFS;
  Mcp->Ch[0].Res = Mcp->Ch[1].Res = Mcp->Ch[2].Res = Mcp->Ch[3].Res = MCP342x_12_BIT;
  Mcp->Ch[0].Points = Mcp->Ch[1].Points = Mcp->Ch[2].Points = Mcp->Ch[3].Points = MCP342x_NORE_P;
  Mcp->Ch[0].nV = Mcp->Ch[1].nV = Mcp->Ch[2].nV = Mcp->Ch[3].nV = MCP342x_NORE_N;
  I2C1_init(); 
  return(TRUE);
 }


/***********************************************************************************
BOOL MCP342x_startCh(MCP342x * Mcp, u8 Ch, u8 Gain, u8 Res)

Start sampling on the Ch channel

Mcp = Pointer to a  user declared MCP342x structure
Ch = Channel to stop (1 > 4)
Gain = Gain to use for the channel (MCP342x_2048mVFS , MCP342x_1024mVFS , MCP342x_512mVFS , MCP342x_256mVFS)
Res = Resolution to use for the channel (MCP342x_12_BIT , MCP342x_14_BIT , MCP342x_16_BIT , MCP342x_18_BIT)

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP342x_startCh(MCP342x * Mcp, u8 Ch, u8 Gain, u8 Res)
 {
  if ( (Mcp == NULL) || (Ch > 4) || (Ch < 1) || (Ch > Mcp->NumCh) )
   return (FALSE);
  Mcp->Ch[Ch-1].Points = MCP342x_NORE_P;
  Mcp->Ch[Ch-1].nV = MCP342x_NORE_N;
  switch (Gain)
   {
    case MCP342x_2048mVFS:
    case MCP342x_1024mVFS:
    case MCP342x_512mVFS:
    case MCP342x_256mVFS:
     Mcp->Ch[Ch-1].Gain = Gain;
     break;
    default:
     return(FALSE);
   }
  switch (Res)
   {
    case MCP342x_12_BIT:
    case MCP342x_14_BIT:
    case MCP342x_16_BIT:
    case MCP342x_18_BIT:
     Mcp->Ch[Ch-1].Res = Res;
     break;
    default:
     return(FALSE);
   }
  Mcp->Ch[Ch-1].On = TRUE;
  if(Mcp->On == FALSE)
   {
    Mcp->On = TRUE;
    Mcp->MilliSec = millis()+10;
   }
  return(TRUE);
 }


/***********************************************************************************
BOOL MCP342x_stopCh(MCP342x * Mcp, u8 Ch)

Stop sampling on the Ch channel

Mcp = Pointer to a  user declared MCP342x structure
Ch = Channel to stop (1 > 4)

RETURN FALSE on Error
***********************************************************************************/
BOOL MCP342x_stopCh(MCP342x * Mcp, u8 Ch)
 {
  if ( (Mcp == NULL) || (Ch > 4)  || (Ch < 1) || (Ch > Mcp->NumCh) )
   return (FALSE);
  Mcp->Ch[Ch-1].On = FALSE;
  Mcp->Ch[Ch-1].Points = MCP342x_NORE_P;
  Mcp->Ch[Ch-1].nV = MCP342x_NORE_N;
  if( (Mcp->Ch[0].On == FALSE) && (Mcp->Ch[1].On == FALSE) && (Mcp->Ch[2].On == FALSE) && (Mcp->Ch[3].On == FALSE) )
   {
    Mcp->On = FALSE;
    Mcp->InSample = FALSE;
    Mcp->CtErr = 0;
   }
 }


/***********************************************************************************
BOOL MCP342x_loop(MCP342x * Mcp)

MCP342x device management loop

Mcp = Pointer to a  user declared MCP342x structure

RETURN 
 3 = New conversion started
 2 = Data not ready
 1 = Nothing to do
-1 = Not valid pointer
-2 = Error reading I2C data
-3 = Conversion timeout
-4 = No channel found
-5 = Error writing I2C data
***********************************************************************************/
int MCP342x_loop(MCP342x * Mcp)
 {
  int Valo;
  u8 *Vb = (u8 *)&Valo; 
  u8 Ctn, Found,ValTx;
    
  // No valid structure return error
  if (Mcp == NULL)
   return (-1);
   
  if ( (Mcp->On == FALSE) || (millis() < Mcp->MilliSec) )
   return (1);
   
  //Test if sampling started
  if (Mcp->InSample == TRUE)
   {
    //Read Data from I2C port
    if (MCP342x_read_Data(Mcp) == FALSE)
     return(-2);
    //Test if data not available
    if ((Mcp->FrameRx[0] & MCP342x_BUSY) != 0)
     {
      //Inc error count
      Mcp->CtErr++;
      //if too many error abort sampling
      if (Mcp->CtErr > 10)
       {
        Mcp->ActCh = 0;
        Mcp->MilliSec = millis() + 2;
        Mcp->InSample = FALSE;
        return(-3);
       }
      else
       {
        //else wait 50mS
        Mcp->MilliSec = millis() + 50;
        return(2);
       }
     }
    else 
     {
      //Data available
      Mcp->CtErr = 0;
      //propagate sign
      Vb[3] = Mcp->FrameRx[3] & 0X80 ? 0XFF : 0; 
      //check for 16 or 18 bit data
      if ((Mcp->FrameRx[0] & MCP342x_18_BIT) == MCP342x_18_BIT)
       { 
        //transfer 18 bit data
        Vb[0] = Mcp->FrameRx[1];
        Vb[1] = Mcp->FrameRx[2];
        Vb[2] = Mcp->FrameRx[3];
       }
      else 
       { 
        //transfer 16 bit data
        Vb[0] = Mcp->FrameRx[2];
        Vb[1] = Mcp->FrameRx[3];
        Vb[2] = Vb[3];        
       }
      Mcp->Ch[Mcp->ActCh-1].Points = Valo;
      //calculate nV ((Point * nV/point) / Gain)
      switch (Mcp->FrameRx[0] & MCP342x_18_BIT)
       {
        case MCP342x_12_BIT:
         Valo *= MCP342x_12_BIT_NV; 
         break;
        case MCP342x_14_BIT:
         Valo *= MCP342x_14_BIT_NV; 
         break;
        case MCP342x_16_BIT:
         Valo *= MCP342x_16_BIT_NV; 
         break;
        case MCP342x_18_BIT:
         Valo *= MCP342x_18_BIT_NV; 
         break;
       }

      switch (Mcp->FrameRx[0] & MCP342x_256mVFS)
       {
        case MCP342x_2048mVFS:
         Valo /= 1;
         break;
        case MCP342x_1024mVFS:
         Valo /= 2;
         break;
        case MCP342x_512mVFS:
         Valo /= 4;
         break;
        case MCP342x_256mVFS:
         Valo /= 8;
         break;
       }
      Mcp->Ch[Mcp->ActCh-1].nV = Valo;
      Mcp->InSample = FALSE;
     }
   }

  //Test if not in sampling
  if (Mcp->InSample == FALSE)
   {
    Found = 0;
    //If channel not assigned or wrong channel or channel stopped 
    if ( (Mcp->ActCh == 0) || (Mcp->ActCh > Mcp->NumCh) || (Mcp->Ch[Mcp->ActCh-1].On == FALSE))
     {
      //find first valid channell
      for (Ctn = 1 ; Ctn <= Mcp->NumCh ; Ctn++)
       if (Mcp->Ch[Ctn-1].On == TRUE)
        {
         Found = Ctn;
         break;
        }
     }
    else
     {
      //Find Next valid channell
      Ctn = Mcp->ActCh+1;
      while (Found == 0)
       {
        if (Ctn > Mcp->NumCh)
         Ctn = 1;
        if (Mcp->Ch[Ctn-1].On == TRUE)
         {
          Found = Ctn;
          break;
         }
        if (Ctn == Mcp->ActCh)
         {
          if (Mcp->Ch[Ctn-1].On == TRUE)
           Found = Ctn;
          break;
         }
        Ctn++; 
       }
     }
    //Test if valid channel not found
    if (Found == 0)
     {
      //Stop any sampling and return error
      Mcp->On = FALSE;
      Mcp->ActCh = 0;
      Mcp->MilliSec = millis()+50;
      return (-4);
     }
    else
     {
      //set actual channel
      Mcp->ActCh = Found;
      //Set channel parameters
      switch (Mcp->ActCh)
       {
        case 1:
         ValTx = MCP342x_START | MCP342x_CHANNEL_1 | Mcp->Ch[0].Res | Mcp->Ch[0].Gain;
        break;
        case 2:
         ValTx = MCP342x_START | MCP342x_CHANNEL_2 | Mcp->Ch[1].Res | Mcp->Ch[1].Gain;
        break;
        case 3:
         ValTx = MCP342x_START | MCP342x_CHANNEL_3 | Mcp->Ch[2].Res | Mcp->Ch[2].Gain;
        break;
        case 4:
         ValTx = MCP342x_START | MCP342x_CHANNEL_4 | Mcp->Ch[3].Res | Mcp->Ch[3].Gain;
        break;
       }
      //Start a new sampling
      if (MCP342x_write(Mcp,ValTx) == FALSE)
       {
        Mcp->ActCh = 0;
        Mcp->MilliSec = millis()+70;
        return (-5);
       }

      //Calculate and set the appropriate conversion delay
      switch (Mcp->Ch[Mcp->ActCh-1].Res & MCP342x_18_BIT)
       {
        case MCP342x_12_BIT:
         Mcp->MilliSec = millis()+MCP342x_12_BIT_MS; 
         break;
        case MCP342x_14_BIT:
         Mcp->MilliSec = millis()+MCP342x_14_BIT_MS; 
         break;
        case MCP342x_16_BIT:
         Mcp->MilliSec = millis()+MCP342x_16_BIT_MS; 
         break;
        case MCP342x_18_BIT:
         Mcp->MilliSec = millis()+MCP342x_18_BIT_MS; 
         break;
       }
      //set sampling in progress
      Mcp->InSample = TRUE;
      return(3);
     }
   }
 }



/***********************************************************************************
BOOL MCP342x_getData(MCP342x * Mcp, u8 Ch, u8 Type, int * Valo)

Read data (A data remain valid only for a single lecture, nV and Points have separated buffers)

Mcp = Pointer to a  user declared MCP342x structure
Ch = Channel to stop (1 > 4)
Type = MCP342x_POINTS   points , MCP342x_NV  nV
Valo = Pointer to the read Value

RETURN FALSE on Error or No Data
***********************************************************************************/
BOOL MCP342x_getData(MCP342x * Mcp, u8 Ch, u8 Type, int * Valo)
 {
  if ( (Mcp == NULL) || (Ch > 4)  || (Ch < 1) || (Ch > Mcp->NumCh) )
   return (FALSE);
  *Valo = 0;
  switch (Type)
   {
    case MCP342x_POINTS:
     if (Mcp->Ch[Ch-1].Points == MCP342x_NORE_P)
      return (FALSE);
     *Valo = Mcp->Ch[Ch-1].Points;
     Mcp->Ch[Ch-1].Points = MCP342x_NORE_P;
     return(TRUE);
     
    case MCP342x_NV:
     if (Mcp->Ch[Ch-1].nV == MCP342x_NORE_N)
      return (FALSE);
     *Valo = Mcp->Ch[Ch-1].nV;
     Mcp->Ch[Ch-1].nV = MCP342x_NORE_N;
     return(TRUE);
     
    default:
     return (FALSE);
   }
 }
