/*	--------------------------------------------------------------------
    FILE:			pmp.c
    PROJECT:		pinguino
    PURPOSE:		PIC32 Parallel Master Port functions
    PROGRAMER:		Regis Blanchot
    FIRST RELEASE:	07 Apr. 2012
    LAST RELEASE:	03 Dec. 2013
    --------------------------------------------------------------------
    CHANGELOG : 

    Apr  07 2012  - initial release

    --------------------------------------------------------------------
    TODO :
    * PMP_autoInc(s8 inc) with inc = 1, -1, or 0 
    --------------------------------------------------------------------
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
    ------------------------------------------------------------------*/

//http://www.ps3devwiki.com/files/flash/Tools/PIC32MX/D32_v1_5/MemInterface.c

#ifndef __PMP_C
#define __PMP_C

#include <typedef.h>
#include <macro.h>
#include <delay.c>
#include <interrupt.c>
#include <pmp.h>

/*
 * The significance of the bits of the PMCON (Parallel Port Control Register)
 * are as follows:
 *
 * bit 31-16 Reserved: Write ?0?; ignore read
 *
 * bit 15 ON: Parallel Master Port Enable bit
 * 1 = PMP enabled
 * 0 = PMP disabled, no off-chip access performed
 * Note: When using 1:1 PBCLK divisor, the user?s software should not read/write
 * the peripheral?s SFRs in the SYSCLK cycle immediately following the instruction
 * that clears the module?s ON control bit.
 *
 * bit 14 FRZ: Freeze in Debug Exception Mode bit
 * 1 = Freeze operation when CPU is in Debug Exception mode
 * 0 = Continue operation even when CPU is in Debug Exception mode
 * Note: FRZ is writable in Debug Exception mode only, it is forced to ?0? in normal mode.
 *
 * bit 13 SIDL: Stop in Idle Mode bit
 * 1 = Discontinue module operation when device enters Idle mode
 * 0 = Continue module operation in Idle mode
 *
 * bit 12-11 ADRMUX<1:0>: Address/Data Multiplexing Selection bits
 * 11 = All 16 bits of address are multiplexed on PMD<15:0> pins
 * 10 = All 16 bits of address are multiplexed on PMD<7:0> pins
 * 01 = Lower 8 bits of address are multiplexed on PMD<7:0> pins,
 *      upper 8 bits are on PMA<15:8>
 * 00 = Address and data appear on separate pins
 *
 * bit 10 PMPTTL: PMP Module TTL Input Buffer Select bit
 * 1 = PMP module uses TTL input buffers
 * 0 = PMP module uses Schmitt Trigger input buffer
 *
 * bit 9 PTWREN: Write Enable Strobe Port Enable bit
 * 1 = PMWR/PMENB port enabled
 * 0 = PMWR/PMENB port disabled
 *
 * bit 8 PTRDEN: Read/Write Strobe Port Enable bit
 * 1 = PMRD/PMWR port enabled
 * 0 = PMRD/PMWR port disabled
 *
 * bit 7-6 CSF<1:0>: Chip Select Function bits(4)
 * 11 = Reserved
 * 10 = PMCS2 and PMCS1 function as Chip Select
 * 01 = PMCS2 functions as Chip Select, PMCS1 functions as address bit 14
 * 00 = PMCS2 and PMCS1 function as address bits 15 and 14
 *
 * bit 5 ALP: Address Latch Polarity bit(4)
 * 1 = Active-high (PMALL and PMALH)
 * 0 = Active-low (PMALL and PMALH)
 *
 * bit 4 CS2P: Chip Select 1 Polarity bit(4)
 * 1 = Active-high (PMCS2)
 * 0 = Active-low (PMCS2)
 *
 * bit 3 CS1P: Chip Select 0 Polarity bit(4)
 * 1 = Active-high (PMCS1)
 * 0 = Active-low (PMCS1)
 *
 * bit 2 Reserved: Write ?0?; ignore read
 *
 * bit 1 WRSP: Write Strobe Polarity bit
 * For Slave Modes and Master mode 2 (PMMODE<9:8> = 00,01,10):
 * 1 = Write strobe active-high (PMWR)
 * 0 = Write strobe active-low (PMWR)
 * For Master mode 1 (PMMODE<9:8> = 11):
 * 1 = Enable strobe active-high (PMENB)
 * 0 = Enable strobe active-low (PMENB)
 *
 * bit 0 RDSP: Read Strobe Polarity bit
 * For Slave modes and Master mode 2 (PMMODE<9:8> = 00,01,10):
 * 1 = Read Strobe active-high (PMRD)
 * 0 = Read Strobe active-low (PMRD)
 * For Master mode 1 (PMMODE<9:8> = 11):
 * 1 = Read/write strobe active-high (PMRD/PMWR)
 * 0 = Read/write strobe active-low (PMRD/PMWR)
 *
 */

#if !defined(PINGUINO32MX250) && !defined(PINGUINO32MX220)
u8 _pmp_mode =   PMP_MODE_8BIT;   // 8-bit PMP by default
#endif
u8 _pmp_active = PMP_ACTIVE_LOW;  // Active low by default

/*	--------------------------------------------------------------------
    ---------- PMP_mode(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port mode
    @param		mode:	8 = 8-bit mode, 16 = 16-bit mode
    NB : PIC32MX1xx and PIC32MX2xx have only 8-bit mode available
    ------------------------------------------------------------------*/
 
void PMP_mode(u8 mode)
{
    #if !defined(PINGUINO32MX250) && !defined(PINGUINO32MX220)

    PMCONCLR = 1<<15;   // Disable the PMP master port

    if (mode==16)
    {
        _pmp_mode = PMP_MODE_16BIT;
        PMMODEbits.MODE16 = PMP_MODE_16BIT;
    }

    else
    {
        _pmp_mode = PMP_MODE_8BIT;
        PMMODEbits.MODE16 = PMP_MODE_8BIT;
    }

    PMCONSET = 1<<15;   // Enable the PMP master port

    #endif
    
}

/*	--------------------------------------------------------------------
    ---------- PMP_active(u8 active)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port activity
    @param		active:	LOW = active-low; HIGH = active-high
    ------------------------------------------------------------------*/
 
void PMP_active(u8 active)
{
    PMCONCLR = 1<<15;   // Disable the PMP master port

    if (active==HIGH)
    {
        _pmp_active = HIGH;
        // Address Latch Polarity bit
        PMCONbits.ALP  = PMP_ACTIVE_HIGH;
        // Select PMCS2, PMCS1 active-low pin polarity: CS2P bit (PMCON<4>) = 0 and CS1P bit (PMCON<3>) = 0.
        #if !defined(PINGUINO32MX250) && !defined(PINGUINO32MX220)
        PMCONbits.CS2P = PMP_ACTIVE_HIGH;
        #endif
        PMCONbits.CS1P = PMP_ACTIVE_HIGH;
        // Select PMWR active-low pin polarity: WRSP bit (PMCON<1>) = 0.
        PMCONbits.WRSP = PMP_ACTIVE_HIGH;
        // Select PMRD active-low pin polarity: RDSP bit (PMCON<0>) = 0.
        PMCONbits.RDSP = PMP_ACTIVE_HIGH;
    }

    else
    {
        _pmp_active = LOW;
        // Address Latch Polarity bit
        PMCONbits.ALP  = PMP_ACTIVE_LOW;
        // Select PMCS2, PMCS1 active-low pin polarity: CS2P bit (PMCON<4>) = 0 and CS1P bit (PMCON<3>) = 0.
        #if !defined(PINGUINO32MX250) && !defined(PINGUINO32MX220)
        PMCONbits.CS2P = PMP_ACTIVE_LOW;
        #endif
        PMCONbits.CS1P = PMP_ACTIVE_LOW;
        // Select PMWR active-low pin polarity: WRSP bit (PMCON<1>) = 0.
        PMCONbits.WRSP = PMP_ACTIVE_LOW;
        // Select PMRD active-low pin polarity: RDSP bit (PMCON<0>) = 0.
        PMCONbits.RDSP = PMP_ACTIVE_LOW;
    }

    PMCONSET = 1<<15;   // Enable the PMP master port
}

/*	--------------------------------------------------------------------
    ---------- PMP_initMaster(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure Parrallel Master Port
    @param		none
    ------------------------------------------------------------------*/
 
void PMP_initMaster()
{
    // 1. If interrupts are used, disable the PMP interrupt by clearing
    // the interrupt enable bit, PMPIE(IEC1<2>) = 0.
    IntDisable(INT_PARALLEL_MASTER_PORT);

    // 2. Stop and reset the PMP module by clearing the ON control bit (PMCON<15)
    PMMODE = 0;
    PMAEN = 0;
    PMCON = 0; 

    /** 3. Configure Mode **/

    // Enable Interrupt Request mode: IRQM<1:0> bits (PMMODE<14:13>).
    PMMODEbits.IRQM = PMP_MODE_IRQ_OFF;
    // Select auto address increment: INCM<1:0> bits (PMMODE<12:11>).
    PMMODEbits.INCM = PMP_MODE_AUTO_ADDR_OFF;

    // Select 8- or 16-bit Data mode: MODE16 bit (PMMODE<10>).
    // cf PMP_mode()
    #if !defined(PINGUINO32MX250) && !defined(PINGUINO32MX220)
    PMMODEbits.MODE16 = _pmp_mode;
    #endif
    
    // Select Master mode : MODE<1:0> bits (PMMODE<9:8>).
    // In Master mode 1, read and write strobe are combined into a 
    // single control line, PMRD/PMWR;
    // a second control line, PMENB, determines when a read or write
    // action is to be taken.
    // In Master mode 2, read and write strobes (PMRD and PMWR)
    // are supplied on separate pins.
    PMMODEbits.MODE = PMP_MODE_MASTER2;

    // Select 4 wait cycle for data setup: WAITB<1:0> bits (PMMODE<7:6>) = 00.
    PMMODEbits.WAITB = 0b11;  // number of wait cycles before the PMRD/PMWR strobe
    // Select 3 wait cycles to extend PMRD/PMWR: WAITM<3:0> bits (PMMODE<5:2>) = 0001.
    PMMODEbits.WAITM = 0b1111;// number of wait cycles during the PMRD/PMWR strobe
    // Select 4 wait cycle for data hold: WAITE<1:0> bits (PMMODE<1:0>) = 00.
    PMMODEbits.WAITE = 0b11;  // number of wait cycles after the PMRD/PMWR strobe

    /** 4. Configure Control **/

    //SIDL: Stop in Idle Mode bi
    //1 = Discontinue module operation when device enters Idle mode
    //0 = Continue module operation in Idle mode
    //PMCONbits.SIDL = 0;

    // Select multiplexed addressing: ADRMUX<1:0> bits (PMCON<12:11>).
    PMCONbits.ADRMUX = PMP_MUX_OFF; // Address and data appear on separate pins
    //PMCONbits.ADRMUX = MUX_16X8;  // Partial Multiplexed mode
    //PMCONbits.ADRMUX = MUX_8X16;  // Partial Multiplexed mode
    
    //
    //PMCONbits.PMPTTL = 0;
    
    // Enable PMWR strobe: PTWREN bit (PMCON<9>)
    PMCONbits.PTWREN = 1;		// PMP write enable
    // Enable PMRD strobe: PTRDEN bit (PMCON<8>)
    PMCONbits.PTRDEN = 1;		// PMP read enable
    // Enable PMCS2 and PMCS1 Chip Selects: CSF<1:0> bits (PMCON<7:6>)
    PMCONbits.CSF = PMP_CS1CS2; //CS1 and CS2 as Chip Select;

    // Address Latch Polarity bit
    PMCONbits.ALP  = _pmp_active;
    
    // Select PMCS2, PMCS1 active-low pin polarity: CS2P bit (PMCON<4>) = 0 and CS1P bit (PMCON<3>) = 0.
    #if !defined(PINGUINO32MX250) && !defined(PINGUINO32MX220)
    PMCONbits.CS2P = _pmp_active;
    #endif
    PMCONbits.CS1P = _pmp_active;

    // Select PMWR active-low pin polarity: WRSP bit (PMCON<1>) = 0.
    PMCONbits.WRSP = _pmp_active;

    // Select PMRD active-low pin polarity: RDSP bit (PMCON<0>) = 0.
    PMCONbits.RDSP = _pmp_active;

    /** 5. Enable/Disable PMA<15:0> Address pins **/
    /*
    bit 15-14 PTEN<15:14>: PMCSx Strobe Enable bits
    1 = PMA15 and PMA14 function as either PMA<15:14> or PMCS2 and PMCS1(1)
    0 = PMA15 and PMA14 function as port I/O
    bit 13-2 PTEN<13:2>: PMP Address Port Enable bits
    1 = PMA<13:2> function as PMP address lines
    0 = PMA<13:2> function as port I/O
    bit 1-0
    PTEN<1:0>: PMALH/PMALL Strobe Enable bits
    1 = PMA1 and PMA0 function as either PMA<1:0> or PMALH and PMALL(2)
    0 = PMA1 and PMA0 pads function as port I/O
    */
    PMAEN = 0;
    PMAEN = (1<<14) | (1<<1); // PMA14 functions as PMCS1
                              // PMA1  functions as PMA1
    
    /** 6. If interrupts are used: **/
/*
    // 4.1. Clear the interrupt flag bit, PMPIF (IFS1<2>) = 0.
    IntClearFlag(INT_PARALLEL_MASTER_PORT);
    // 4.2. Configure the PMP interrupt priority bits
    IntSetVectorPriority(INT_PARALLEL_MASTER_PORT_VECTOR, 7, 3);
    // 4.3. Enable the PMP interrupt by setting the interrupt enable bit, PMPIE = 1.
    IntEnable(INT_PARALLEL_MASTER_PORT);
*/

    /** 7. Enable the PMP master port **/
    PMCONSET = 0x8000;//Bit(15);

    // wait for >30 ms
    Delayms(50);
}

/*
    In 16-bit Data mode (MODE16 bit (PMMODE<10>) = 1),
    the write to the PMDIN register causes
    PMDIN<15:0> to appear on the data bus, (PMD<15:0>).
    In 8-bit Data mode (MODE16 bit (PMMODE<10>) = 0),
    the write to the PMDIN register causes
    PMDIN<7:0> to appear on the data bus, PMD<7:0>.
    The upper 8 bits, PMD<15:8>, are ignored.
*/

void PMP_write(u16 value)
{
    PMDIN = value;
}

/*
    Note that the read data obtained from the PMDIN register
    is actually the read value from the	previous read operation.
    Therefore, the first user application read will be a dummy read
    to initiate	the first bus read and fill the read register.
*/

u16 PMP_read()
{
    u16 dummy;
    
    dummy = PMDIN;  // init read cycle, dummy read
    PMP_wait();     // wait for PMP to be available
    return (PMDIN);
}

/*
    16-bit value to be written to external device
    Note: Use in MASTER mode 1 or 2, MODE[1:0] = 10, 11
    1 = active
    0 = inactive
*/

void PMP_setAddress(u16 addr)
{
    PMP_wait();     // wait for PMP to be available
    PMADDRSET = addr;
}

void PMP_clrAddress(u16 addr)
{
    PMP_wait();     // wait for PMP to be available
    PMADDRCLR = addr;
}

#endif /* __PMP_C */
