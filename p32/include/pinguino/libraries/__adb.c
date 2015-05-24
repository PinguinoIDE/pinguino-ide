/*	----------------------------------------------------------------------------
	FILE:			__adb.c
	PROJECT:		pinguino
	PURPOSE:		Andoid interface
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
	FIRST RELEASE:	20 agu. 2011
	LAST RELEASE:	20 agu. 2011
	----------------------------------------------------------------------------
	TODO : 
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

// Most of the code used in this library is from IOIO project
// IOIO is licensed to Ytai Ben-Tsvi  <ytaibt@gmail.com>

#ifndef __ADB__
#define __ADB__

#include <adb.h>
#include <interrupt.c>
#include <delay.c>

#ifndef ADBBUFFERLENGTH
#define ADBBUFFERLENGTH 1024          		// rx buffer length
#endif

typedef enum {
  MAIN_STATE_WAIT_CONNECT,
  MAIN_STATE_WAIT_OPEN,
  MAIN_STATE_RECV
} MAIN_STATE;

ADB_CHANNEL_HANDLE h;

u16 adbwpointer,adbrpointer;        		// write and read pointer

char adbrx[ADBBUFFERLENGTH];                // this is the buffer

// Adb.init is directly rerouted to ADBInit() ( IOIO Library )
// Adb.refresh is directly rerouted to ADBTasks() ( IOIO Library )

void ADBreceive(ADB_CHANNEL_HANDLE h, const void* data, unsigned int data_len) 
{	
	unsigned char caractere;
	unsigned char length;
	u16 newwp;
	u16 i;
	
	for (i=0;i<2;i++)								// first store in buffer frame length
		{
			length=(data_len>>(i*8));				// LSB first then MSB of frame length
					
			if (adbwpointer!=ADBBUFFERLENGTH-1)	    // if not last place in buffer
				newwp=adbwpointer+1;			    // place=place+1
			else
				newwp=1;					        // else place=1
			if (adbrpointer!=newwp)					// if read pointer!=write pointer
				adbrx[adbwpointer++]=length;		// store received char
					
			if (adbwpointer==ADBBUFFERLENGTH)		// if write pointer=length buffer
			adbwpointer=1;							// write pointer = 1
		}
	
	for (i=0;i<data_len;i++)						// then store received byte
		{
			caractere=((const unsigned char *) data)[i];
			
			if (adbwpointer!=ADBBUFFERLENGTH-1)	    // if not last place in buffer
				newwp=adbwpointer+1;			    // place=place+1
			else
				newwp=1;					        // else place=1
			if (adbrpointer!=newwp)					// if read pointer!=write pointer
				adbrx[adbwpointer++]=caractere;		// store received char
					
			if (adbwpointer==ADBBUFFERLENGTH)		// if write pointer=length buffer
			adbwpointer=1;							// write pointer = 1
		}			
			
}

unsigned char _ADBInit()
{
	IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
	ADBInit();
}

unsigned char _ADBOpen()
{
	u32 wait=0;
	
	h = ADBOpen("tcp:2000", &ADBreceive);
	while ((wait<1000)&&(!ADBChannelReady(h))) 
		{
			ADBTasks();
			Delayms(1);
			wait++;
		}
	if (wait<1000) return TRUE;
	else return FALSE;
}

unsigned char ADBAvailable()
{
	return(adbwpointer!=adbrpointer);
}

u16 ADBRead(u8 *buffer)
{
	u16 data_len;
	u16 i;
	
	if (ADBAvailable())
		{
			data_len=adbrx[adbrpointer++];
			data_len+=(adbrx[adbrpointer++]>>8);
			for (i=0;i<data_len;i++)
				buffer[i]=adbrx[adbrpointer++];
			return(data_len);
		}
}

void ADBSend(u8 *buffer,u16 data_len)
{
	ADBWrite(h,buffer,data_len);
}

#endif
