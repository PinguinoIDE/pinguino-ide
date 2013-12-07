/*	----------------------------------------------------------------------------
	FILE:			memory.c
	PROJECT:		pinguinoX
	PURPOSE:		
	PROGRAMER:		jean-pierre mandon <jp.mandon@gmail.com>
	FIRST RELEASE:	26 jan. 2013
	LAST RELEASE:	27 jan. 2013
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

#ifndef __FLASHMEM__
#define __FLASHMEM__

#include <typedef.h>

// TotalMem return the last address in flash memory

u32 TotalMem()
{
	return(BMXPFMSZ+0x9D000000);
}

// FlashOperation
// execute a standart write operation 

u32 FlashOperation(u16 operation)
{
	// disable interrupt
	asm("di");
	NVMCON=operation;
	
	// wait for LVD to start up ( at least 6 µS )
	{
        unsigned long t0 = _CP0_GET_COUNT();
        while (_CP0_GET_COUNT() - t0 < (80/2)*6);
    }
    
	// unlock system
	NVMKEY=0xAA996655;
	NVMKEY=0x556699AA;
	
	NVMCONSET=NVMCON_WR;
	
    // Wait for WR bit to clear
    while(NVMCON & NVMCON_WR);

	// Disable Flash Write/Erase operations
    NVMCONCLR = NVMCON_WREN;
        
	// enable interrupt
	asm("ei");
		
	return(NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK));	// error if != 0
}

// FlashClearError
// clear error in the NVMCON register    		    		

u32 FlashClearError()
{
	return(FlashOperation(0x4000));
}

u32 PageErase(void* address)
{
	// check for error in the NVMCON register
	if (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK)) FlashClearError();
	// set Address to erase and Non Volatile Memory instruction ERASE PAGE
	NVMADDR = (u32)address & 0x1fffffff ;
	return(FlashOperation(0x4004));
}

// WordWrite
// write a single word in memory

u32 WordWrite(void* address,u32 data)
{

	// check for error in the NVMCON register
	if (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK)) FlashClearError();
	// set Address to erase and Non Volatile Memory instruction ERASE PAGE
	NVMADDR = (u32) address & 0x1fffffff ;
	NVMDATA = (u32) data ;
	return(FlashOperation(0x4001));
}

u32 RowWrite(void* address,u32* data)
{

	// check for error in the NVMCON register
	if (NVMCON & (_NVMCON_WRERR_MASK | _NVMCON_LVDERR_MASK)) FlashClearError();
	// set Address to erase and Non Volatile Memory instruction ERASE PAGE
	NVMADDR = (u32) address & 0x1fffffff;
	NVMDATA = (u32) data & 0x1fffffff;
	return(FlashOperation(0x4003));
}

#endif	/* __NVMMEM__ */
