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
#include <pin.h>
#include <macro.h>
#include <delay.c>
#include <interrupt.c>
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
    NB : PIC32MX1xx and PIC32MX2xx have only 8-bit mode available
    ------------------------------------------------------------------*/
 
void PMP_setWidth(u8 width)
{
    #if !defined(PINGUINO32MX250) || defined(PINGUINO32MX270) && !defined(PINGUINO32MX220)
    if ( (width==16) || (width==PMP_MODE_16BIT) )
        _pmp_width = PMP_MODE_16BIT;
    else
        _pmp_width = PMP_MODE_8BIT;
    #else
        _pmp_width = PMP_MODE_8BIT;
    #endif
    //PMMODEHbits.MODE16 = _pmp_width;
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

    //IntDisable(INT_PARALLEL_MASTER_PORT);

    /// 2. Stop and reset the PMP module

    //PMCONbits.PMPEN = 0; // disable PMP
    PMCONCLR = 0x8000;// 1<<15 : Bit(15);
    PMMODE = 0;
    PMAEN = 0;
    PMCON = 0; 

    /// 3. Configure Mode

    // Enable Interrupt Request mode: IRQM<1:0> bits (PMMODE<14:13>).
    PMMODEbits.IRQM = PMP_MODE_IRQ_OFF;

    // Select auto address increment: INCM<1:0> bits (PMMODE<12:11>).
    if (_pmp_mode==PMP_MODE_ESLAVE || _pmp_mode==PMP_MODE_SLAVE)
    {
        if (_pmp_inc==1)
            PMMODEbits.INCM = PMP_MODE_INC_BUFFER;
        else
            PMMODEbits.INCM = PMP_MODE_INC_OFF;
    }
    else // master mode 1 or 2
    {
        if (_pmp_inc==1)
            PMMODEbits.INCM = PMP_MODE_INC_ADDR;

        if (_pmp_inc==-1)
            PMMODEbits.INCM = PMP_MODE_DEC_ADDR;
        
        if (_pmp_inc==0)
            PMMODEbits.INCM = PMP_MODE_INC_OFF;
    }
    
    // Select 8- or 16-bit Data mode: MODE16 bit (PMMODE<10>).
    #if !defined(PINGUINO32MX250) || defined(PINGUINO32MX270) && !defined(PINGUINO32MX220)
    PMMODEbits.MODE16 = _pmp_width;
    #endif
    
    // Select Master mode : MODE<1:0> bits (PMMODE<9:8>).
    // In Master mode 1, read and write strobe are combined into a 
    // single control line, PMRD/PMWR;
    // a second control line, PMENB, determines when a read or write
    // action is to be taken.
    // In Master mode 2, read and write strobes (PMRD and PMWR)
    // are supplied on separate pins.
    PMMODEbits.MODE = _pmp_mode;

    // Select 4 wait cycles before the PMRD/PMWR strobe
    PMMODEbits.WAITB = _pmp_waitB;

    // Wait cycles for a Read/Write strobe
    PMMODEbits.WAITM = _pmp_waitM;

    // Wait cycles for data hold
    PMMODEbits.WAITE = _pmp_waitE;

    /// 4. Configure Control

    //SIDL: Stop in Idle Mode bi
    //1 = Discontinue module operation when device enters Idle mode
    //0 = Continue module operation in Idle mode
    PMCONbits.SIDL = 0;    // PMP idle when MCU idle

    // Select multiplexed addressing: ADRMUX<1:0> bits (PMCON<12:11>).
    PMCONbits.ADRMUX = _pmp_mux; // Address and data appear on separate pins
    
    //PMPTTL: PMP Module TTL Input Buffer Select bit
    //1 = PMP module uses TTL input buffers
    //0 = PMP module uses Schmitt Trigger input buffer

    PMCONbits.PMPTTL = 0;   // no TTL level but Schmitt Trigger instead
    
    if ( _pmp_control & PMWR )
        PMCONbits.PTWREN = 1;		// PMWR/PMENB strobe enable
    else
        PMCONbits.PTWREN = 0;		// PMWR/PMENB strobe disable
    
    // Enable PMRD strobe: PTRDEN bit (PMCON<8>)
    if ( _pmp_control & PMRD)
        PMCONbits.PTRDEN = 1;		// PMRD/PMWR strobe enable
    else
        PMCONbits.PTRDEN = 0;		// PMRD/PMWR strobe disable
    
    // Enable PMCS1 Chip Selects: CSF<1:0> bits (PMCON<7:6>)
    #if defined(PINGUINO32MX250) || defined(PINGUINO32MX270) || defined(PINGUINO32MX220)
    if ( _pmp_control & PMCS1 )
        PMCONbits.CSF = PMP_CS1;
    else
        PMCONbits.CSF = PMP_CS_OFF;
    #else
    if ( (_pmp_control & PMCS1) && (_pmp_control & PMCS2) )
        PMCONbits.CSF = PMP_CS1CS2;
    else if ( _pmp_control & PMCS1 )
        PMCONbits.CSF = PMP_CS1CS2;
    else if ( _pmp_control & PMCS2 )
        PMCONbits.CSF = PMP_CS2;
    else
        PMCONbits.CSF = PMP_CS_OFF;
    #endif
    
    // Address Latch Polarity bit
    PMCONbits.ALP  = _pmp_polarity;
    
    // CS pin polarity
    #if !defined(PINGUINO32MX250) || defined(PINGUINO32MX270) && !defined(PINGUINO32MX220)
    PMCONbits.CS2P = _pmp_polarity;
    #endif
    PMCONbits.CS1P = _pmp_polarity;

    // PMWR pin polarity
    PMCONbits.WRSP = _pmp_polarity;

    // PMRD pin polarity
    PMCONbits.RDSP = _pmp_polarity;

    /// 5. Enable/Disable PMA<15:0> Address pins
 
    PMAEN = _pmp_address;

    if ( _pmp_control & PMCS1 )
        PMAENSET = PMCS1;

    #if !defined(PINGUINO32MX250) || defined(PINGUINO32MX270) && !defined(PINGUINO32MX220)
    if ( _pmp_control & PMCS2 )    
        PMAENSET = PMCS2;
    #endif
    
    /// 6. If interrupts are used

    // 6.1. Clear the interrupt flag bit, PMPIF (IFS1<2>) = 0.
    //IntClearFlag(INT_PARALLEL_MASTER_PORT);
    // 6.2. Configure the PMP interrupt priority bits
    //IntSetVectorPriority(INT_PARALLEL_MASTER_PORT_VECTOR, 7, 3);
    // 6.3. Enable the PMP interrupt by setting the interrupt enable bit, PMPIE = 1.
    //IntEnable(INT_PARALLEL_MASTER_PORT);
    
    /// 7. Enable the PMP master port
    //PMCONbits.PMPEN = 1; // enable PMP
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
    PMP_wait();         // wait for PMP to be available
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
    //u16 dummy;
    
    //PMP_wait();         // wait for PMP to be available
    //dummy = PMDIN;      // init read cycle, dummy read
    PMP_wait();         // wait for PMP to be available
    return (PMDIN);
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
    PMADDR = addr;
}

#endif /* __PMP_C */
