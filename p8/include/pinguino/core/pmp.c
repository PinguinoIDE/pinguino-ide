/*	--------------------------------------------------------------------
    FILE:			pmp.c
    PROJECT:		pinguino
    PURPOSE:		PIC32 Parallel Master Port functions
    PROGRAMER:		Regis Blanchot
    FIRST RELEASE:	07 Apr. 2012
    LAST RELEASE:	20 Mar. 2014
    --------------------------------------------------------------------
    CHANGELOG : 

    Apr  07 2012  - Regis Blanchot - initial release
    Mar  20 2014  - Regis Blanchot - complete rewrite

    --------------------------------------------------------------------
    TODO :
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
#include <pin.h>
#include <macro.h>
#include <delayms.c>
//#include <interrupt.h>
//#include <interrupt.c>
#include <pmp.h>

u8  _pmp_width    = PMP_MODE_8BIT;      // 8-bit PMP by default
u8  _pmp_polarity = PMP_ACTIVE_LOW;     // Active low by default
u8  _pmp_mode     = PMP_MODE_MASTER2;   // PMWR et PMRD on 2 separate pins
u16 _pmp_address  = 0;//PMA1;               // PMA1 used as address 
u16 _pmp_control  = 0;//PMWR | PMRD | PMCS1;//
s8  _pmp_inc      = 0;                  // address won't be auto-incremented
u8  _pmp_mux      = PMP_MUX_OFF;        // data and address on separate lines
u8  _pmp_waitB    = 4;                  //  4 x Tpb
u8  _pmp_waitM    = 16;                 // 16 x Tpb
u8  _pmp_waitE    = 4;                  //  4 x Tpb

/*	--------------------------------------------------------------------
    ---------- PMP_setMode(u8 mode)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port mode
    @param		PMP_MODE_MASTER1, PMP_MODE_MASTER2,
                PMP_MODE_SLAVE or PMP_MODE_ESLAVE
    ------------------------------------------------------------------*/
 
void PMP_setMode(u8 mode)
{
    _pmp_mode = mode;
}

/*	--------------------------------------------------------------------
    ---------- PMP_setWidth(u8 wifth)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port mode
    @param		mode:	8 = 8-bit mode, 16 = 16-bit mode
    ------------------------------------------------------------------*/
 
void PMP_setWidth(u8 width)
{
    if ( (width==16) || (width==PMP_MODE_16BIT) )
        _pmp_width = PMP_MODE_16BIT;
    else
        _pmp_width = PMP_MODE_8BIT;
}

/*	--------------------------------------------------------------------
    ---------- PMP_address(u16 addr)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set address pins
    @param		16-bit address from PMA0 to PMA15
    ------------------------------------------------------------------*/
 
void PMP_setAddress(u16 addr)
{
    _pmp_address = addr;
}

/*	--------------------------------------------------------------------
    ---------- PMP_setControl(u8 control)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port Control pins
    @param		PMWR, PMRD, PMCS or PMBE
    ------------------------------------------------------------------*/
 
void PMP_setControl(u16 control)
{
    _pmp_control = control;
}

/*	--------------------------------------------------------------------
    ---------- PMP_setPolarity(u8 polarity)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Parrallel Master Port activity
    @param		polarity:	LOW = active-low; HIGH = active-high
    ------------------------------------------------------------------*/
 
void PMP_setPolarity(u8 polarity)
{
    _pmp_polarity = polarity;
}

/*	--------------------------------------------------------------------
    ---------- PMP_autoIncrement(u8 inc)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set auto-increment mode
    @param		PMP_MODE_AUTO_ADDR_BUFFER, PMP_MODE_AUTO_ADDR_DEC,
                PMP_MODE_AUTO_ADDR_INC or PMP_MODE_AUTO_ADDR_OFF
    ------------------------------------------------------------------*/
 
void PMP_autoIncrement(s8 inc)
{
    if (inc==1 || inc==PMP_MODE_INC_BUFFER || inc==PMP_MODE_INC_ADDR)
        _pmp_inc = 1;

    else if (inc==-1 || inc==PMP_MODE_DEC_ADDR)
        _pmp_inc = -1;
    
    else //if (inc==0 || inc==PMP_MODE_INC_OFF)
        _pmp_inc = 0;
}

/*	--------------------------------------------------------------------
    ---------- PMP_setMux(u8 mux)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set multiplexed mode
    @param		PMP_MUX_16X16, PMP_MUX_16X8, PMP_MUX_8X16 or PMP_MUX_OFF
    ------------------------------------------------------------------*/
 
void PMP_setMux(u8 mux)
{
    _pmp_mux = mux;
}

/*	--------------------------------------------------------------------
    ---------- PMP_setWaitStates(u8 b, u8 m, u8 e)
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Set Data Read/Write Strobe Wait States
    @param		WAIT B, WAIT M, WAIT E
    ------------------------------------------------------------------*/
 
void PMP_setWaitStates(u8 beg, u8 mid, u8 end)
{
    if (beg>=4) beg=4;
    _pmp_waitB = beg-1;
    if (mid>=16) mid=16;
    _pmp_waitM = mid-1;
    if (end>=4) end=4;
    _pmp_waitE = end-1;
}

/*	--------------------------------------------------------------------
    ---------- PMP_init()
    --------------------------------------------------------------------
    @author		Regis Blanchot <rblanchot@gmail.com>
    @descr		Configure Parrallel Master Port
    @param		none
    ------------------------------------------------------------------*/
 
void PMP_init()
{
    /// 1. Disable the PMP interrupt

    //IntDisable(PMPINT);

    /// 2. Stop and reset the PMP module

    PMCONHbits.PMPEN = 0;//Bit(15);

    PMCONH  = 0; 
    PMCONL  = 0; 
    PMMODEH = 0;
    PMMODEL = 0;
    PMEH    = 0;
    PMEL    = 0;

    /// 3. Configure Mode

    // Enable Interrupt Request mode: IRQM<1:0> bits (PMMODE<14:13>).
    PMMODEHbits.IRQM = PMP_MODE_IRQ_OFF;
    
    // Select auto address increment: INCM<1:0> bits (PMMODE<12:11>).
    if (_pmp_mode==PMP_MODE_ESLAVE || _pmp_mode==PMP_MODE_SLAVE)
    {
        if (_pmp_inc==1)
            PMMODEHbits.INCM = PMP_MODE_INC_BUFFER;
        else
            PMMODEHbits.INCM = PMP_MODE_INC_OFF;
    }
    else // master mode 1 or 2
    {
        if (_pmp_inc==1)
            PMMODEHbits.INCM = PMP_MODE_INC_ADDR;

        if (_pmp_inc==-1)
            PMMODEHbits.INCM = PMP_MODE_DEC_ADDR;
        
        if (_pmp_inc==0)
            PMMODEHbits.INCM = PMP_MODE_INC_OFF;
    }
    
    // Select 8- or 16-bit Data mode: MODE16 bit (PMMODE<10>).
    PMMODEHbits.MODE16 = _pmp_width;
    
    // Select Master mode : MODE<1:0> bits (PMMODE<9:8>).
    // In Master mode 1, read and write strobe are combined into a 
    // single control line, PMRD/PMWR;
    // a second control line, PMENB, determines when a read or write
    // action is to be taken.
    // In Master mode 2, read and write strobes (PMRD and PMWR)
    // are supplied on separate pins.
    PMMODEH |= _pmp_mode;

    // Select 4 wait cycles before the PMRD/PMWR strobe
    PMMODELbits.WAITB = _pmp_waitB;

    // Wait cycles for a Read/Write strobe
    PMMODELbits.WAITM = _pmp_waitM;

    // Wait cycles for data hold
    PMMODELbits.WAITE = _pmp_waitE;

    /// 4. Configure Control

    //SIDL: Stop in Idle Mode bi
    //1 = Discontinue module operation when device enters Idle mode
    //0 = Continue module operation in Idle mode
    PMCONHbits.PSIDL = 0;    // PMP idle when MCU idle

    // Select multiplexed addressing: ADRMUX<1:0> bits (PMCON<12:11>).
    PMCONHbits.ADRMUX = _pmp_mux; // Address and data appear on separate pins
    
    //PMPTTL: PMP Module TTL Input Buffer Select bit
    //1 = PMP module uses TTL input buffers
    //0 = PMP module uses Schmitt Trigger input buffer

    PADCFG1bits.PMPTTL = 0;
/*    
    /// Master Mode 1 (PMCSx, PMRD/PMWR, PMENB, PMBE, PMA<x:0> and PMD<7:0>)
    if ( _pmp_mode == PMP_MODE_MASTER1 )
    {
        // Enable PMBE strobe: PTWREN bit (PMCON<9>)
        if ( _pmp_control & PMENB )
            PMCONHbits.PTWREN = 1;		// PMP write strobe enable
        else
            PMCONHbits.PTWREN = 0;		// PMP write strobe disable

        // Enable PMRD/PMWR strobe: PTRDEN bit (PMCON<8>)
        if (_pmp_control & PMRD_PMWR)
            PMCONHbits.PTRDEN = 1;		// PMP read strobe enable
        else
            PMCONHbits.PTRDEN = 0;		// PMP read strobe disable

        // To differentiate data bytes, the byte enable control strobe,
        // PMBE, is used to signal when the Most Significant Byte(MSB)
        // of data is being presented on the data lines
        // Enable bit : PTBEEN bit (PMCONH<2>)
        if ( _pmp_control & PMBE )
            PMCONHbits.PTBEEN = 1;		// PMP Enable strobe enable
        else
            PMCONHbits.PTBEEN = 0;		// PMP Enable strobe disable
    }

    /// Master Mode 2 (PMCSx, PMRD, PMWR, PMBE, PMA<x:0> and PMD<7:0>)
    else if ( _pmp_mode == PMP_MODE_MASTER2 )
    {
*/
        // Enable PMWR strobe: PTWREN bit (PMCON<9>)
        if ( _pmp_control & PMWR )
            PMCONHbits.PTWREN = 1;		// PMP write strobe enable
        else
            PMCONHbits.PTWREN = 0;		// PMP write strobe disable
        
        // Enable PMRD strobe: PTRDEN bit (PMCON<8>)
        if (_pmp_control & PMRD)
            PMCONHbits.PTRDEN = 1;		// PMP read strobe enable
        else
            PMCONHbits.PTRDEN = 0;		// PMP read strobe disable

        if ( _pmp_control & PMBE )
            PMCONHbits.PTBEEN = 1;		// PMP Enable strobe enable
        else
            PMCONHbits.PTBEEN = 0;		// PMP Enable strobe disable
//    }
    
    // Enable PMCS1 Chip Selects: CSF<1:0> bits (PMCON<7:6>)
    if ((_pmp_control & PMCS1) || (_pmp_control & PMCS2))
        PMCONLbits.CSF = PMP_CS_ON;
    else
        PMCONLbits.CSF = PMP_CS_OFF;

    // Address Latch Polarity bit
    PMCONLbits.ALP  = _pmp_polarity;
    
    // Select PMCS2, PMCS1 active-low pin polarity: CS2P bit (PMCON<4>) = 0 and CS1P bit (PMCON<3>) = 0.
    PMCONLbits.CS1P = _pmp_polarity;

    // Byte Enable priority bit
    PMCONLbits.BEP  = _pmp_polarity;

    // Select PMWR active-low pin polarity: WRSP bit (PMCON<1>) = 0.
    PMCONLbits.WRSP = _pmp_polarity;

    // Select PMRD active-low pin polarity: RDSP bit (PMCON<0>) = 0.
    PMCONLbits.RDSP = _pmp_polarity;

    /// 5. Enable/Disable PMA<15:0> Address pins
    
    PMEH = high8(_pmp_address);
    PMEL =  low8(_pmp_address);

    if ( _pmp_control & PMCS1 )
    {
        PMEHbits.PTEN14 = 1;	//address 14 or PMCS1
        //PMEH |= PMCS1;
    }
    
    if ( _pmp_control & PMCS2 )
    {
        PMEHbits.PTEN15 = 1;	//address 15 or PMCS2
        //PMEH |= PMCS2;
    }
    
    /** 6. If interrupts are used: **/
/*
    // 6.1. Clear the interrupt flag bit, PMPIF (IFS1<2>) = 0.
    IntClearFlag(INT_PARALLEL_MASTER_PORT);
    // 6.2. Configure the PMP interrupt priority bits
    IntSetVectorPriority(INT_PARALLEL_MASTER_PORT_VECTOR, 7, 3);
    // 6.3. Enable the PMP interrupt by setting the interrupt enable bit, PMPIE = 1.
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
    if (_pmp_width == PMP_MODE_16BIT)
    {
        PMP_wait();         // wait for PMP to be available
        PMDIN1L =  low8(value);
        PMP_wait();         // wait for PMP to be available
        PMDIN1H = high8(value);
    }
    
    else
    {
        PMP_wait();         // wait for PMP to be available
        PMDIN1L = low8(value);
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
    
    if (_pmp_width == PMP_MODE_16BIT)
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

void PMP_sendAddress(u16 addr)
{
    PMP_wait();     // wait for PMP to be available
    PMADDRH = high8(addr);
    PMADDRL =  low8(addr);
}

#endif /* __PMP_C */
