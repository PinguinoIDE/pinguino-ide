/*	----------------------------------------------------------------------------
	FILE:			pmp.h
	PROJECT:		pinguino
	PURPOSE:		PIC32 Parallel Master Port functions
	PROGRAMER:		Regis Blanchot
	FIRST RELEASE:	07 Apr. 2012
	LAST RELEASE:	07 Apr. 2012
	----------------------------------------------------------------------------
	CHANGELOG : 

	Apr  07 2012  - initial release
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
	--------------------------------------------------------------------------*/

//http://www.ps3devwiki.com/files/flash/Tools/PIC32MX/D32_v1_5/MemInterface.c

#ifndef __PMP_H
#define __PMP_H

#include <typedef.h>

/**	--------------------------------------------------------------------
	PMCON Register
    --------------------------------------------------------------------

    The significance of the bits of the PMCON (Parallel Port Control Register)
    are as follows:

    bit 31-16 Reserved: Write ?0?; ignore read

    bit 15 ON: Parallel Master Port Enable bit
    1 = PMP enabled
    0 = PMP disabled, no off-chip access performed
    Note: When using 1:1 PBCLK divisor, the user?s software should not read/write
    the peripheral?s SFRs in the SYSCLK cycle immediately following the instruction
    that clears the module?s ON control bit.

    bit 14 FRZ: Freeze in Debug Exception Mode bit
    1 = Freeze operation when CPU is in Debug Exception mode
    0 = Continue operation even when CPU is in Debug Exception mode
    Note: FRZ is writable in Debug Exception mode only, it is forced to ?0? in normal mode.

    bit 13 SIDL: Stop in Idle Mode bit
    1 = Discontinue module operation when device enters Idle mode
    0 = Continue module operation in Idle mode

    bit 12-11 ADRMUX<1:0>: Address/Data Multiplexing Selection bits
    11 = All 16 bits of address are multiplexed on PMD<15:0> pins
    10 = All 16 bits of address are multiplexed on PMD<7:0> pins
    01 = Lower 8 bits of address are multiplexed on PMD<7:0> pins,
         upper 8 bits are on PMA<15:8>
    00 = Address and data appear on separate pins

    bit 10 PMPTTL: PMP Module TTL Input Buffer Select bit
    1 = PMP module uses TTL input buffers
    0 = PMP module uses Schmitt Trigger input buffer

    bit 9 PTWREN: Write Enable Strobe Port Enable bit
    1 = PMWR/PMENB port enabled
    0 = PMWR/PMENB port disabled

    bit 8 PTRDEN: Read/Write Strobe Port Enable bit
    1 = PMRD/PMWR port enabled
    0 = PMRD/PMWR port disabled

    bit 7-6 CSF<1:0>: Chip Select Function bits(4)
    11 = Reserved
    10 = PMCS2 and PMCS1 function as Chip Select
    01 = PMCS2 functions as Chip Select, PMCS1 functions as address bit 14
    00 = PMCS2 and PMCS1 function as address bits 15 and 14

    bit 5 ALP: Address Latch Polarity bit(4)
    1 = Active-high (PMALL and PMALH)
    0 = Active-low (PMALL and PMALH)

    bit 4 CS2P: Chip Select 1 Polarity bit(4)
    1 = Active-high (PMCS2)
    0 = Active-low (PMCS2)

    bit 3 CS1P: Chip Select 0 Polarity bit(4)
    1 = Active-high (PMCS1)
    0 = Active-low (PMCS1)

    bit 2 Reserved: Write ?0?; ignore read

    bit 1 WRSP: Write Strobe Polarity bit
    For Slave Modes and Master mode 2 (PMMODE<9:8> = 00,01,10):
    1 = Write strobe active-high (PMWR)
    0 = Write strobe active-low (PMWR)
    For Master mode 1 (PMMODE<9:8> = 11):
    1 = Enable strobe active-high (PMENB)
    0 = Enable strobe active-low (PMENB)

    bit 0 RDSP: Read Strobe Polarity bit
    For Slave modes and Master mode 2 (PMMODE<9:8> = 00,01,10):
    1 = Read Strobe active-high (PMRD)
    0 = Read Strobe active-low (PMRD)
    For Master mode 1 (PMMODE<9:8> = 11):
    1 = Read/write strobe active-high (PMRD/PMWR)
    0 = Read/write strobe active-low (PMRD/PMWR)

 	------------------------------------------------------------------*/

// 1 = Port is busy
// 0 = Port is not busy
//#define PMP_busy()				(PMMODE & 0x8000)
#define PMP_wait()			    	while(PMMODEbits.BUSY)

#define PMP_MODE_IRQ_OFF			0b00
#define PMP_MODE_BUF_IRQ			0b10
#define PMP_MODE_CY_IRQ				0b01

#define PMP_MODE_AUTO_ADDR_BUFFER	0b11
#define PMP_MODE_AUTO_ADDR_DEC		0b10
#define PMP_MODE_AUTO_ADDR_INC		0b01
#define PMP_MODE_AUTO_ADDR_OFF		0b00

#define	PMP_MODE_8BIT				0		// PMP<15:8> not controlled by the PMP module
#define	PMP_MODE_16BIT				1

#define PMP_MODE_MASTER1			0b11	// Read & Write on same pin
#define PMP_MODE_MASTER2			0b10	// Read & Write on separate pins
#define PMP_MODE_ESLAVE				0b01	// Enhanced
#define PMP_MODE_SLAVE				0b00	// Legacy

#define PMP_MUX_16X16				0b11    // Fully multiplexed (16 data pins PMD<15:0>)
#define PMP_MUX_16X8				0b10    // Fully multiplexed (lower eight data pins PMD<7:0>)
#define PMP_MUX_8X16				0b01    // Partially multiplexed (lower eight data pins PMD<7:0>)
#define PMP_MUX_OFF					0b00	// Demultiplexed (separate data and address lines)

#define PMP_CS1CS2					0b10    // PMCS1 and PMCS2 function as Chip Select
#define PMP_CS2						0b01    // PMCS2 as CS, PMCS1 as address bit 14
#define PMP_CS_OFF					0b00    // PMCS1 and PMCS2 function as address bit 14 and 15

#define PMP_ACTIVE_LOW				0
#define PMP_ACTIVE_HIGH				1

/**	--------------------------------------------------------------------
	Prototypes
	------------------------------------------------------------------*/

void PMP_mode(u8);
void PMP_active(u8);
void PMP_initMaster();
void PMP_setAddress(u16);
void PMP_clrAddress(u16);
void PMP_write(u16);
u16  PMP_read();

#endif /* __PMP_H */
