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

#if !defined(__18f46j53) && !defined(__18f47j53)

    #error " --------------------------------------- "
    #error " No Parallel Port on your Chip !         "
    #error " PMP is only available on Pinguino 47j53 "
    #error " --------------------------------------- "

#endif

#ifndef __PMP_C
#define __PMP_C
//#define INTDISABLE

#include <typedef.h>
#include <macro.h>
#include <delay.c>
//#include <interrupt.h>
//#include <interrupt.c>
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

u8 _pmp_mode     = PMP_MODE_8BIT;       // 8-bit PMP by default
u8 _pmp_polarity = PMP_ACTIVE_LOW;      // Active low by default
u8 _pmp_master   = PMP_MODE_MASTER2;    // PMWR et PMRD on 2 separate pins
u8 _pmp_address  = PMA0 | PMA1 | PMA2;  //

/*	--------------------------------------------------------------------
    ---------- PMP_address(u16 pmaxx)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port address pins
    @param		PMP_MODE_MASTER1 or PMP_MODE_MASTER2
    ------------------------------------------------------------------*/
 
void PMP_address(u16 pmaxx)
{
    _pmp_address = 0;
    _pmp_address = pmaxx;
}

/*	--------------------------------------------------------------------
    ---------- PMP_master(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port mode
    @param		PMP_MODE_MASTER1 or PMP_MODE_MASTER2
    ------------------------------------------------------------------*/
 
void PMP_master(u8 mode)
{
    if ((mode==1) || (mode == PMP_MODE_MASTER1))
    {
        _pmp_master = PMP_MODE_MASTER1;
    }

    else
    {
        _pmp_master = PMP_MODE_MASTER2;
    }

    PMMODEH |= _pmp_master;
}

/*	--------------------------------------------------------------------
    ---------- PMP_slave(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port mode
    @param		PMP_MODE_MASTER1 or PMP_MODE_MASTER2
    ------------------------------------------------------------------*/
 
void PMP_slave(u8 mode)
{
    if ((mode==1) || (mode == PMP_MODE_ESLAVE))
    {
        _pmp_master = PMP_MODE_ESLAVE;
    }

    else
    {
        _pmp_master = PMP_MODE_SLAVE;
    }
}

/*	--------------------------------------------------------------------
    ---------- PMP_mode(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port mode
    @param		mode:	8 = 8-bit mode, 16 = 16-bit mode
    ------------------------------------------------------------------*/
 
void PMP_mode(u8 mode)
{
    if ( (mode==16) || (mode==PMP_MODE_16BIT) )
    {
        _pmp_mode = PMP_MODE_16BIT;
    }

    else
    {
        _pmp_mode = PMP_MODE_8BIT;
    }
}

/*	--------------------------------------------------------------------
    ---------- PMP_polarity(u8 polarity)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port activity
    @param		polarity:	LOW = active-low; HIGH = active-high
    ------------------------------------------------------------------*/
 
void PMP_polarity(u8 polarity)
{
    if ( polarity == PMP_ACTIVE_HIGH )
    {
        _pmp_polarity = PMP_ACTIVE_HIGH;
    }

    else
    {
        _pmp_polarity = PMP_ACTIVE_LOW;
    }
}

/*	--------------------------------------------------------------------
    ---------- PMP_initMaster(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure Parrallel Master Port
    @param		none
    ------------------------------------------------------------------*/
 
void PMP_init()
{
    /** 1. If interrupts are used, disable the PMP interrupt **/

    //IntDisable(PMPINT);

    /** 2. Stop and reset the PMP module by clearing the ON control bit **/

    PMCONHbits.PMPEN = 0;//Bit(15);
/*
    PMCONH  = 0; 
    PMCONL  = 0; 
    PMMODEH = 0;
    PMMODEL = 0;
    PMEH    = 0;
    PMEL    = 0;
*/
    /** 3. Configure Mode **/

    // Enable Interrupt Request mode: IRQM<1:0> bits (PMMODE<14:13>).
    PMMODEHbits.IRQM = PMP_MODE_IRQ_OFF;
    // Select auto address increment: INCM<1:0> bits (PMMODE<12:11>).
    PMMODEHbits.INCM = PMP_MODE_AUTO_ADDR_OFF;

    // Select 8- or 16-bit Data mode: MODE16 bit (PMMODE<10>).
    // cf PMP_mode()
    PMMODEHbits.MODE16 = _pmp_mode;
    
    // Select Master mode : MODE<1:0> bits (PMMODE<9:8>).
    // In Master mode 1, read and write strobe are combined into a 
    // single control line, PMRD/PMWR;
    // a second control line, PMENB, determines when a read or write
    // action is to be taken.
    // In Master mode 2, read and write strobes (PMRD and PMWR)
    // are supplied on separate pins.
    PMMODEHbits.MODE = _pmp_master;

    // Select 4 wait cycle for data setup: WAITB<1:0> bits (PMMODE<7:6>) = 00.
    PMMODELbits.WAITB = 0b11;  // number of wait cycles before the PMRD/PMWR strobe
    // Select 3 wait cycles to extend PMRD/PMWR: WAITM<3:0> bits (PMMODE<5:2>) = 0001.
    PMMODELbits.WAITM = 0b0000;// number of wait cycles during the PMRD/PMWR strobe
    // Select 4 wait cycle for data hold: WAITE<1:0> bits (PMMODE<1:0>) = 00.
    PMMODELbits.WAITE = 0b11;  // number of wait cycles after the PMRD/PMWR strobe

    /** 4. Configure Control **/

    //SIDL: Stop in Idle Mode bi
    //1 = Discontinue module operation when device enters Idle mode
    //0 = Continue module operation in Idle mode
    PMCONHbits.PSIDL = 1;    // PMP idle when MCU idle

    // Select multiplexed addressing: ADRMUX<1:0> bits (PMCON<12:11>).
    //PMCONHbits.ADRMUX = PMP_MUX_OFF; // Address and data appear on separate pins
    PMCONHbits.ADRMUX1 = 0; // Address and data appear on separate pins
    PMCONHbits.ADRMUX0 = 0; // Address and data appear on separate pins
    //PMCONbits.ADRMUX = MUX_16X8;  // Partial Multiplexed mode
    //PMCONbits.ADRMUX = MUX_8X16;  // Partial Multiplexed mode
    
    //
    //PMCONbits.PMPTTL = 0;
    
    // To differentiate data bytes, the byte enable control strobe,
    // PMBE, is used to signal when the Most Significant Byte(MSB)
    // of data is being presented on the data lines
    // Enable bit : PTBEEN bit (PMCONH<2>)
    if ( _pmp_mode == PMP_MODE_16BIT)
        PMCONHbits.PTBEEN = 1;		// PMP bit enable
    else
        PMCONHbits.PTBEEN = 0;		// PMP bit disable
    
    // Enable PMWR strobe: PTWREN bit (PMCON<9>)
    PMCONHbits.PTWREN = 1;		// PMP write strobe enable
    
    // Enable PMRD strobe: PTRDEN bit (PMCON<8>)
    PMCONHbits.PTRDEN = 1;		// PMP read strobe enable
    
    // Enable PMCS1 Chip Selects: CSF<1:0> bits (PMCON<7:6>)
    PMCONLbits.CSF = PMP_CS_ON;

    // Address Latch Polarity bit
    PMCONLbits.ALP  = _pmp_polarity;
    
    // Select PMCS2, PMCS1 active-low pin polarity: CS2P bit (PMCON<4>) = 0 and CS1P bit (PMCON<3>) = 0.
    PMCONLbits.CS1P = _pmp_polarity;

    // Byte enable priority bit
    PMCONLbits.BEP = _pmp_polarity;

    // Select PMWR active-low pin polarity: WRSP bit (PMCON<1>) = 0.
    PMCONLbits.WRSP = _pmp_polarity;

    // Select PMRD active-low pin polarity: RDSP bit (PMCON<0>) = 0.
    PMCONLbits.RDSP = _pmp_polarity;

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
    PMEH = high8(_pmp_address);
    PMEL =  low8(_pmp_address);

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
    PMCONHbits.PMPEN = 1;//Bit(15);

    // wait for >30 ms
    Delayms(50);
}

/*
    In 16-bit Data mode (MODE16 bit (PMMODEH<2>)),
    Least Significant Byte (LSB) of data must be presented first
    It is highly recommended that the BUSY bit be checked prior
    to any operation that accesses the PMDIN or PMADDR register

    To perform a write on the parallel port, the user application writes
    to the PMDIN register (same register as a read operation).
    This causes the PMP module to first activate the Chip Select lines
    and the address bus.
    The write data from the PMDIN register is placed onto the PMD data bus
    and the write line PMPWR is strobed in Master mode 2, PMRD/PMWR and
    PMENB lines in Master Mode 1.

*/

void PMP_write(u16 value)
{
    if (_pmp_mode == PMP_MODE_16BIT)
    {
        PMP_wait();         // wait for PMP to be available
        PMDIN1L =  low8(value);
        PMDIN1H = high8(value);
    }
    
    else
    {
        PMP_wait();         // wait for PMP to be available
        PMDIN1L = high8(value);
        PMP_wait();         // wait for PMP to be available
        PMDIN1L =  low8(value);
    }   
}

/*
    Note that the read data obtained from the PMDIN register
    is actually the read value from the	previous read operation.
    Therefore, the first user application read will be a dummy read
    to initiate	the first bus read and fill the read register.
    It is highly recommended that the BUSY bit be checked prior
    to any operation that accesses the PMDIN or PMADDR register
*/

u16 PMP_read()
{
    u16 dummy;
    
    if (_pmp_mode == PMP_MODE_16BIT)
    {
        PMP_wait();         // wait for PMP to be available
        dummy = PMDIN1L;    // init read cycle, dummy read
        PMP_wait();         // wait for PMP to be available
        return (make16(PMDIN1L, PMDIN1H));
    }
    
    else
    {
        PMP_wait();         // wait for PMP to be available
        dummy = PMDIN1L;    // dummy read
        PMP_wait();         // wait for PMP to be available
        return (PMDIN1L);   // real read
    }
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
    PMADDRH |= high8(addr);
    PMADDRL |=  low8(addr);
}

void PMP_clrAddress(u16 addr)
{
    PMP_wait();     // wait for PMP to be available
    PMADDRH &= ( 255 - high8(addr) );
    PMADDRL &= ( 255 -  low8(addr) );
}

#endif /* __PMP_C */
