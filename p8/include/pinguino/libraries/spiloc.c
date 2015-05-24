/*
 * File			SPILoc.h
 *
 * Description		SPI Emulation using standard IO pin
 *			Is possible define CPOL, CPHA and clock delay
 *
 * Author		Moreno Manzini 
 *
 * Version		1.0
 *
 * License		This software is released under the terms of the Mozilla Public License (MPL) version 2.0
 * 			Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 *
 *   18/06/2013 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it )
*/
  
 
#include <delayus.c> 
#include <digitalw.c> 
#include <const.h>     // NULL
#include <typedef.h>   // BOOL
  
typedef struct 
 {
  BOOL CPOL;    // Clock Polarity
  BOOL CPHA;    // Sample position
  u16 DelayuS;  // Delay Clock uS
  u8 LoSCK;	    // SCK Clock pin
  u8 LoSDO;     // SDO Data out pin
  u8 LoSDI;     // SDI Data in pin
 } SPI_Loc_Conf;

BOOL SPILoc_Init(SPI_Loc_Conf * Conf)
 {
  if (Conf == NULL) return;
  pinmode(Conf->LoSCK, OUTPUT); 
  if (Conf->CPOL == FALSE)
   digitalwrite(Conf->LoSCK,LOW); 
  else 
   digitalwrite(Conf->LoSCK,HIGH); 
  pinmode(Conf->LoSDO, OUTPUT); 
  digitalwrite(Conf->LoSDO,LOW); 
  pinmode(Conf->LoSDI, INPUT); 
 }     

void SPILoc_Write(SPI_Loc_Conf * Conf, volatile byte data) 
{
 byte i;
 
 if (Conf->DelayuS == 0)
  {
   for ( i = 0; i < 8; i++)
    {
     if (Conf->CPHA)
      {
       digitalwrite(Conf->LoSCK,(!Conf->CPOL)); 
       digitalwrite(Conf->LoSDO,(data & 0x80)); 
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       data = data << 1;
      }
     else 
      {
       digitalwrite(Conf->LoSDO,(data & 0x80)); 
       digitalwrite(Conf->LoSCK,(!Conf->CPOL)); 
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       data = data << 1;
      }
    }
  } 
 else 
  {
   for ( i = 0; i < 8; i++)
    {
     if (Conf->CPHA)
      {
       digitalwrite(Conf->LoSCK,(!Conf->CPOL));
       digitalwrite(Conf->LoSDO,(data & 0x80)); 
       Delayus(Conf->DelayuS); 
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       Delayus(Conf->DelayuS); 
       data = data << 1;
      }
     else 
      {
       digitalwrite(Conf->LoSDO,(data & 0x80)); 
       Delayus(Conf->DelayuS); 
       digitalwrite(Conf->LoSCK,(!Conf->CPOL)); 
       Delayus(Conf->DelayuS); 
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       data = data << 1;
      }
    }
  } 
 return;    
}            

u8 SPILoc_Read(SPI_Loc_Conf * Conf) 
{
 byte i;
 byte data = 0;
 
 if (Conf->DelayuS == 0)
  {
   for ( i = 0; i < 8; i++)
    {
     if (Conf->CPHA)
      {
       digitalwrite(Conf->LoSCK,(!Conf->CPOL)); 
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       data = data << 1;
       if (digitalread(Conf->LoSDI))
        data = data | 0x01;
      }
     else 
      {
       digitalwrite(Conf->LoSCK,(!Conf->CPOL)); 
       data = data << 1;
       if (digitalread(Conf->LoSDI))
        data = data | 0x01;
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
      }
    }
  } 
 else 
  {
   for ( i = 0; i < 8; i++)
    {
     if (Conf->CPHA)
      {
       digitalwrite(Conf->LoSCK,(!Conf->CPOL));
       Delayus(Conf->DelayuS); 
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       data = data << 1;
       if (digitalread(Conf->LoSDI))
        data = data | 0x01;
       Delayus(Conf->DelayuS); 
      }
     else 
      {
       digitalwrite(Conf->LoSCK,(!Conf->CPOL)); 
       Delayus(Conf->DelayuS); 
       data = data << 1;
       if (digitalread(Conf->LoSDI))
        data = data | 0x01;
       digitalwrite(Conf->LoSCK,(Conf->CPOL)); 
       Delayus(Conf->DelayuS); 
      }
    }
  } 
 return(data);    
}            
