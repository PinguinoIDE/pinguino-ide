/***********************************************************************************

 * File			MCP3422_2CH_18Bit_I2C_ADC.pde
 *
 * Description	 Support for Microchip MCP34x 1/2/4 Channels 12/14/16/18 Bit differential Delta-Sigma ADC with I2C interface
 *                      MCP3422 Test Program
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

/***********************************************************************************
BOOL MCP342x.init(MCP342x * Mcp, u8 I2CAddr, u8 NumCh)

Init a MCP342x device

Mcp = Pointer to a  user declared MCP342x structure
I2CAddr = I2C device address
NumCh = Number of channels (MCP3421 =  1 , MCP3422 =  2 , MCP3423 = 2 , MCP3424 = 4 , MCP3425 =  1 , MCP3426 =  2 , MCP3427 = 2 , MCP3428 = 4)

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP342x.startCh(MCP342x * Mcp, u8 Ch, u8 Gain, u8 Res)

Start sampling on the Ch channel

Mcp = Pointer to a  user declared MCP342x structure
Ch = Channel to stop (1 > 4)
Gain = Gain to use for the channel (MCP342x_2048mVFS , MCP342x_1024mVFS , MCP342x_512mVFS , MCP342x_256mVFS)
Res = Resolution to use for the channel (MCP342x_12_BIT , MCP342x_14_BIT , MCP342x_16_BIT , MCP342x_18_BIT)

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP342x.stopCh(MCP342x * Mcp, u8 Ch)

Stop sampling on the Ch channel

Mcp = Pointer to a  user declared MCP342x structure
Ch = Channel to stop (1 > 4)

RETURN FALSE on Error
***********************************************************************************/

/***********************************************************************************
BOOL MCP342x.loop(MCP342x * Mcp)

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

/***********************************************************************************
BOOL MCP342x.getData(MCP342x * Mcp, u8 Ch, u8 Type, int * Valo)

Read data (A data remain valid only for a single lecture, nV and Points have separated buffers)

Mcp = Pointer to a  user declared MCP342x structure
Ch = Channel to stop (1 > 4)
Type = MCP342x_POINTS   points , MCP342x_NV  nV
Valo = Pointer to the read Value

RETURN FALSE on Error or No Data
***********************************************************************************/



#define ledPinY			    10 // the number of the yellow LED pin 

 
//#define TEST_STOP   1
            
MCP342x Mcp;

 
u64 Time1;                                      
                                                            
void setup()
{  
 pinMode(ledPinY, OUTPUT); 
 digitalWrite(ledPinY,LOW);  
 //Init MCP3422 2CH no hardware address
 MCP342x.init(&Mcp,0,2);

 //Start sampling channel 2 Gain 1 (2048mV FS) 18bit
 MCP342x.startCh(&Mcp,2,MCP342x_2048mVFS,MCP342x_18_BIT);
 //Start sampling channel 1 Gain 1 (2048mV FS) 18bit
 MCP342x.startCh(&Mcp,1,MCP342x_2048mVFS,MCP342x_18_BIT);

 Time1 = millis()+100;
}

int Ttt = 0;

void loop()
{
 char Str[40];
 int Valot;
 
 //ADC Main Loop
 MCP342x.loop(&Mcp);

 //If available read data from channel 1 and clear it (points)
 if (MCP342x.getData(&Mcp,1,MCP342x_POINTS,&Valot) == TRUE)
  {
   sprintf(Str,"CH1 = %d Points - ",Valot);
   CDC.printf(Str);  
  }
 //If available read data from channel 1 and clear it (nV)
 if (MCP342x.getData(&Mcp,1,MCP342x_NV,&Valot) == TRUE)
  {
   sprintf(Str,"%d nV\r\n",Valot);
   CDC.printf(Str);  
  }
 //If available read data from channel 2 and clear it (points)
 if (MCP342x.getData(&Mcp,2,MCP342x_POINTS,&Valot) == TRUE)
  {
   sprintf(Str,"CH2 = %d Points - ",Valot);
   CDC.printf(Str);  
  }
 //If available read data from channel 2 and clear it (nV)
 if (MCP342x.getData(&Mcp,2,MCP342x_NV,&Valot) == TRUE)
  {
   sprintf(Str,"%d nV\r\n",Valot);
   CDC.printf(Str);  
  }

 if (millis() > Time1)
  {
#ifdef TEST_STOP
   Ttt++;
   if (Ttt == 50)
    //Stop sampling on channel 1
    MCP342x.stopCh(&Mcp,1);
   if (Ttt == 150)
    //Stop sampling on channel 1
    MCP342x.stopCh(&Mcp,2);
#endif
   toggle(ledPinY);
   Time1 = millis()+500;
  }
}
