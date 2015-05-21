/*  --------------------------------------------------------------------
    FILE:				interrupt.c
    PROJECT:			pinguino 32
    PURPOSE:			interrupts management
    PROGRAMER:			regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:		16 Nov. 2010
    LAST RELEASE:		16 Jan. 2015
    --------------------------------------------------------------------
    CHANGELOG:
    ???          : Marcus Fazzi <anunakin@gmail.com> added UART3/4/5/6 for PIC32MX795 support
    05 jul. 2012 : Gagabi Added support for GENERIC 32 bits boards in intconfiguresystem()
    10 May  2014 : Joël fixed wrong interrupt vector number for MX1xx and MX2xx family
    16 May  2014 : Régis replaced BitSet and BitClear macro with register set and clear instructions
    14 Jan  2015 : Régis splited interrupt.c in interrupt.c and interrupt.h
    16 Jan  2015 : Régis updated IntConfigureSystem()
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

#ifndef __INTERRUPT_C
#define __INTERRUPT_C

    #include <p32xxxx.h>
    #include <typedef.h>
    #include <macro.h>
    #include <interrupt.h>
    #include <mips.h>

/*	----------------------------------------------------------------------------
    IntSetPriority
    ----------------------------------------------------------------------------
    Interrupt Natural Priority
    ----------------------------------------------------------------------------
    When multiple interrupts are assigned to same group priority and subpriority,
    they are prioritized by their natural priority. The natural priority is a
    fixed priority scheme, where the highest natural priority starts at the lowest
    interrupt vector, meaning that interrupt vector 0 is the highest and interrupt
    vector 63 is the lowest natural priority. See the interrupt vector table in
    the respective device data sheet to learn the natural priority order of each IRQ.
    ----------------------------------------------------------------------------
    The Interrupt Service Routine (ISR) must clear the associated interrupt flag
    in the IFSx register before lowering the interrupt priority level to avoid
    recursive interrupts.
    IPCx: Interrupt Priority Control Registers
    IFSx: Interrupt Flag Status Registers
    --------------------------------------------------------------------------*/

void IntSetVectorPriority(u8 vector, u8 pri, u8 sub)
{
    //IFSxCLR = ;
    //IPCxCLR = ;		// clear the priority level

    switch (vector)
    {
        case INT_CORE_TIMER_VECTOR:
            break;
            
        case INT_CORE_SOFTWARE0_VECTOR:
            break;
            
        case INT_CORE_SOFTWARE1_VECTOR:
            break;
            
        case INT_EXTERNAL0_VECTOR:
            break;
            
        case INT_TIMER1_VECTOR:
            IFS0bits.T1IF = 0;
            IPC1bits.T1IP = pri;
            IPC1bits.T1IS = sub;
            break;
            
        case INT_INPUT_CAPTURE1_VECTOR:
            break;
            
        case INT_OUTPUT_COMPARE1_VECTOR:
            break;
            
        case INT_EXTERNAL1_VECTOR:
            break;
            
        case INT_TIMER2_VECTOR:
            IFS0bits.T2IF = 0;
            IPC2bits.T2IP = pri;
            IPC2bits.T2IS = sub;
            break;
            
        case INT_INPUT_CAPTURE2_VECTOR:
            break;
            
        case INT_OUTPUT_COMPARE2_VECTOR:
            break;
            
        case INT_EXTERNAL2_VECTOR:
            break;
            
        case INT_TIMER3_VECTOR:
            IFS0bits.T3IF = 0;
            IPC3bits.T3IP = pri;
            IPC3bits.T3IS = sub;
            break;
            
        case INT_INPUT_CAPTURE3_VECTOR:
            break;
            
        case INT_OUTPUT_COMPARE3_VECTOR:
            break;
            
        case INT_EXTERNAL3_VECTOR:
            break;
            
        case INT_TIMER4_VECTOR:
            IFS0bits.T4IF = 0;
            IPC4bits.T4IP = pri;
            IPC4bits.T4IS = sub;
            break;
            
        case INT_INPUT_CAPTURE4_VECTOR:
            break;
            
        case INT_OUTPUT_COMPARE4_VECTOR:
            break;
            
        case INT_EXTERNAL4_VECTOR:
            break;
            
        case INT_TIMER5_VECTOR:
            break;
            
        case INT_INPUT_CAPTURE5_VECTOR:
            break;
            
        case INT_OUTPUT_COMPARE5_VECTOR:
            break;

        #if defined(UBW32_460) || defined(EMPEROR460)
        case INT_SPI1_VECTOR:
            IFS0bits.SPI1EIF  = 0;
            IFS0bits.SPI1TXIF = 0;
            IFS0bits.SPI1RXIF = 0;
            IPC5bits.SPI1IP = pri;
            IPC5bits.SPI1IS = sub;
            break;  
        #endif
        
        case INT_UART1_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            IFS1bits.U1TXIF = 0;
            IFS1bits.U1RXIF = 0;
            IFS1bits.U1EIF  = 0;
            IPC8bits.U1IP = pri;
            IPC8bits.U1IS = sub;
        #else
            IFS0bits.U1TXIF = 0;
            IFS0bits.U1RXIF = 0;
            IFS0bits.U1EIF  = 0;
            IPC6bits.U1IP = pri;
            IPC6bits.U1IS = sub;
        #endif
            break;
             
        case INT_I2C1_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            IFS1bits.I2C1MIF = 0;
            IFS1bits.I2C1SIF = 0;
            IFS1bits.I2C1BIF  = 0;
            IPC8bits.I2C1IP = pri;
            IPC8bits.I2C1IS = sub;
        #else
            IFS0bits.I2C1MIF = 0;
            IFS0bits.I2C1SIF = 0;
            IFS0bits.I2C1BIF = 0;
            IPC6bits.I2C1IP = pri;
            IPC6bits.I2C1IS = sub;
        #endif
            break;

        case INT_INPUT_CHANGE_VECTOR:
            break;

        case INT_ADC1_CONVERT_DONE_VECTOR:
            break;       

        case INT_PARALLEL_MASTER_PORT_VECTOR:
            break;

        case INT_COMPARATOR1_VECTOR:
            break;         

        //case INT_COMPARATOR2_VECTOR:
        //	break;

        #ifdef ENABLE_UART3
        case INT_UART3_VECTOR:
            IFS1bits.U2ATXIF = 0;
            IFS1bits.U2ARXIF = 0;
            IFS1bits.U2AEIF  = 0;
            IPC7bits.U2AIP = pri;
            IPC7bits.U2AIS = sub;
            break;
        #else     
        case INT_SPI2_VECTOR:
            IFS1bits.SPI2EIF = 0;
            IFS1bits.SPI2TXIF = 0;
            IFS1bits.SPI2RXIF  = 0;
            #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            IPC9bits.SPI2IP = pri;
            IPC9bits.SPI2IS = sub;
            #else
            IPC7bits.SPI2IP = pri;
            IPC7bits.SPI2IS = sub;
            #endif
            break;
        #endif

        case INT_UART2_VECTOR:
            IFS1bits.U2TXIF = 0;
            IFS1bits.U2RXIF = 0;
            IFS1bits.U2EIF  = 0;
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            IPC9bits.U2IP = pri;
            IPC9bits.U2IS = sub;
        #else
            IPC8bits.U2IP = pri;
            IPC8bits.U2IS = sub;
        #endif
            break;

        case INT_I2C2_VECTOR:
            break;
            
        case INT_FSCM_VECTOR:
            break;

        case INT_RTCC_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            IFS0bits.RTCCIF = 0;
            IPC6bits.RTCCIP = pri;
            IPC6bits.RTCCIS = sub;
        #else
            IFS1bits.RTCCIF = 0;
            IPC8bits.RTCCIP = pri;
            IPC8bits.RTCCIS = sub;
        #endif
            break;

        case INT_DMA0_VECTOR:
            break;
            
        case INT_DMA1_VECTOR:
            break;
            
        case INT_DMA2_VECTOR:
            break;
            
        case INT_DMA3_VECTOR:
            break;
            
        case INT_FCE_VECTOR:
            break;

        case INT_USB_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            IPC7bits.USBIP = pri;
            IPC7bits.USBIS = sub;
        #else
            IPC11bits.USBIP = pri;
            IPC11bits.USBIS = sub;
        #endif
            break;

        #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
        case INT_CAN1_VECTOR:
            break;
            
        case INT_CAN2_VECTOR:
            break;
            
        case INT_ETH_VECTOR:
            break;
        #endif

        #ifdef ENABLE_UART4
        case INT_UART4_VECTOR:
            IFS2bits.U1BTXIF = 0;
            IFS2bits.U1BRXIF = 0;
            IFS2bits.U1BEIF  = 0;
            IPC12bits.U1BIP = pri;
            IPC12bits.U1BIS = sub;
            break;
        #endif

        #ifdef ENABLE_UART5
        case INT_UART5_VECTOR:
            IFS2bits.U3BTXIF = 0;
            IFS2bits.U3BRXIF = 0;
            IFS2bits.U3BEIF  = 0;
            IPC12bits.U3BIP = pri;
            IPC12bits.U3BIS = sub;
            break;
        #endif

        #ifdef ENABLE_UART6
        case INT_UART6_VECTOR:
            IFS2bits.U2BTXIF = 0;
            IFS2bits.U2BRXIF = 0;
            IFS2bits.U2BEIF  = 0;
            IPC12bits.U2BIP = pri;
            IPC12bits.U2BIS = sub;
            break;
        #endif
    }
}

/*	----------------------------------------------------------------------------
    IntGetVectorPriority
    ----------------------------------------------------------------------------
    Gets the interrupt vector's priority.
    This routine gets the interrupt vector's priority.
    Parameters:
        vector    - Interrupt vector.
    Returns:
        interrupt vector's priority
    --------------------------------------------------------------------------*/

u32 IntGetVectorPriority(u8 vector)
{
    u32 pri = 0;

    switch (vector)
    {
        case INT_CORE_TIMER_VECTOR:
            break;
        case INT_CORE_SOFTWARE0_VECTOR:
            break;
        case INT_CORE_SOFTWARE1_VECTOR:
            break;
        case INT_EXTERNAL0_VECTOR:
            break;
        case INT_TIMER1_VECTOR:
            pri = IPC1bits.T1IP;
            break;
        case INT_INPUT_CAPTURE1_VECTOR:
            break;
        case INT_OUTPUT_COMPARE1_VECTOR:
            break;
        case INT_EXTERNAL1_VECTOR:
            break;    
        case INT_TIMER2_VECTOR:
            break;
        case INT_INPUT_CAPTURE2_VECTOR:
            break;
        case INT_OUTPUT_COMPARE2_VECTOR:
            break;
        case INT_EXTERNAL2_VECTOR:
            break;    
        case INT_TIMER3_VECTOR:
            break;
        case INT_INPUT_CAPTURE3_VECTOR:
            break;
        case INT_OUTPUT_COMPARE3_VECTOR:
            break;
        case INT_EXTERNAL3_VECTOR:
            break;     
        case INT_TIMER4_VECTOR:
            break;
        case INT_INPUT_CAPTURE4_VECTOR:
            break;
        case INT_OUTPUT_COMPARE4_VECTOR:
            break;
        case INT_EXTERNAL4_VECTOR:
            break;
        case INT_TIMER5_VECTOR:
            break;
        case INT_INPUT_CAPTURE5_VECTOR:
            break;
        case INT_OUTPUT_COMPARE5_VECTOR:
            break;
        case INT_SPI1_VECTOR:
            break;  
        case INT_UART1_VECTOR:
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            pri = IPC8bits.U1IP;
#else
            pri = IPC6bits.U1IP;
#endif
            break; 
        case INT_I2C1_VECTOR:
            break;
        case INT_INPUT_CHANGE_VECTOR:
            break;
        case INT_ADC1_CONVERT_DONE_VECTOR:
            break;       
        case INT_PARALLEL_MASTER_PORT_VECTOR:
            break;
        case INT_COMPARATOR1_VECTOR:
            break;         
        case INT_COMPARATOR2_VECTOR:
            break;
#ifdef ENABLE_UART3
        case INT_UART3_VECTOR:
            pri = IPC7bits.U2AIP;
            break;
#else
        case INT_SPI2_VECTOR:
            break;
#endif
        case INT_UART2_VECTOR:
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            pri = IPC9bits.U2IP;
#else
            pri = IPC8bits.U2IP;
#endif
            break;
        case INT_I2C2_VECTOR:
            break;
        case INT_FSCM_VECTOR:
            break;
        case INT_RTCC_VECTOR:
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            pri = IPC6bits.RTCCIP;
#else
            pri = IPC8bits.RTCCIP;
#endif
            break;
        case INT_DMA0_VECTOR:
            break;
        case INT_DMA1_VECTOR:
            break;
        case INT_DMA2_VECTOR:
            break;
        case INT_DMA3_VECTOR:
            break;
        case INT_FCE_VECTOR:
            break;
        case INT_USB_VECTOR:
            break;
   #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
        case INT_CAN1_VECTOR:
            break;
        case INT_CAN2_VECTOR:
            break;
        case INT_ETH_VECTOR:
            break;
    #endif
#ifdef ENABLE_UART4
        case INT_UART4_VECTOR:
            pri = IPC12bits.U1BIP;
            break;
#endif
#ifdef ENABLE_UART5
        case INT_UART5_VECTOR:
            pri = IPC12bits.U3BIP;
            break;
#endif
#ifdef ENABLE_UART6
        case INT_UART6_VECTOR:
            pri = IPC12bits.U2BIP;
            break;
#endif
    }
    return (pri);
}

/*	----------------------------------------------------------------------------
    TODO:IntGetVectorSubPriority
    ----------------------------------------------------------------------------
    Gets the interrupt vector's priority.
    This routine gets the interrupt vector's priority.
    Parameters:
        vector    - Interrupt vector.
    Returns:
        interrupt vector's sub-priority
    --------------------------------------------------------------------------*/

u32 IntGetVectorSubPriority(u8 vector)
{
    u32 sub = 0;

    switch (vector)
    {
        case INT_CORE_TIMER_VECTOR:
            break;
        case INT_CORE_SOFTWARE0_VECTOR:
            break;
        case INT_CORE_SOFTWARE1_VECTOR:
            break;
        case INT_EXTERNAL0_VECTOR:
            break;
        case INT_TIMER1_VECTOR:
            sub = IPC1bits.T1IS;
            break;
        case INT_INPUT_CAPTURE1_VECTOR:
            break;
        case INT_OUTPUT_COMPARE1_VECTOR:
            break;
        case INT_EXTERNAL1_VECTOR:
            break;    
        case INT_TIMER2_VECTOR:
            break;
        case INT_INPUT_CAPTURE2_VECTOR:
            break;
        case INT_OUTPUT_COMPARE2_VECTOR:
            break;
        case INT_EXTERNAL2_VECTOR:
            break;    
        case INT_TIMER3_VECTOR:
            break;
        case INT_INPUT_CAPTURE3_VECTOR:
            break;
        case INT_OUTPUT_COMPARE3_VECTOR:
            break;
        case INT_EXTERNAL3_VECTOR:
            break;     
        case INT_TIMER4_VECTOR:
            break;
        case INT_INPUT_CAPTURE4_VECTOR:
            break;
        case INT_OUTPUT_COMPARE4_VECTOR:
            break;
        case INT_EXTERNAL4_VECTOR:
            break;
        case INT_TIMER5_VECTOR:
            break;
        case INT_INPUT_CAPTURE5_VECTOR:
            break;
        case INT_OUTPUT_COMPARE5_VECTOR:
            break;
        case INT_SPI1_VECTOR:
            break;
              
        case INT_UART1_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            sub = IPC8bits.U1IS;
        #else
            sub = IPC6bits.U1IS;
        #endif
            break;
             
        case INT_I2C1_VECTOR:
            break;
        case INT_INPUT_CHANGE_VECTOR:
            break;
        case INT_ADC1_CONVERT_DONE_VECTOR:
            break;       
        case INT_PARALLEL_MASTER_PORT_VECTOR:
            break;
        case INT_COMPARATOR1_VECTOR:
            break;         
        case INT_COMPARATOR2_VECTOR:
            break;
            
        #ifdef ENABLE_UART3
        case INT_UART3_VECTOR:
            sub = IPC7bits.U2AIS;
            break;
        #else
        case INT_SPI2_VECTOR:
            break;
        #endif

        case INT_UART2_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            sub = IPC9bits.U2IS;
        #else
            sub = IPC8bits.U2IS;
        #endif
            break;
            
        case INT_I2C2_VECTOR:
            break;
        case INT_FSCM_VECTOR:
            break;

        case INT_RTCC_VECTOR:
        #if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250) || defined(PINGUINO32MX270)||defined(PINGUINO32MX220)
            sub = IPC6bits.RTCCIS;
        #else
            sub = IPC8bits.RTCCIS;
        #endif
            break;
            
        case INT_DMA0_VECTOR:
            break;
        case INT_DMA1_VECTOR:
            break;
        case INT_DMA2_VECTOR:
            break;
        case INT_DMA3_VECTOR:
            break;
        case INT_FCE_VECTOR:
            break;
        case INT_USB_VECTOR:
            break;

        #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
        case INT_CAN1_VECTOR:
            break;
        case INT_CAN2_VECTOR:
            break;
        case INT_ETH_VECTOR:
            break;
        #endif
   
        #ifdef ENABLE_UART4
        case INT_UART4_VECTOR:
            sub = IPC12bits.U1BIS;
            break;
        #endif

        #ifdef ENABLE_UART5
        case INT_UART5_VECTOR:
            sub = IPC12bits.U3BIS;
            break;
        #endif

        #ifdef ENABLE_UART6
        case INT_UART6_VECTOR:
            sub = IPC12bits.U2BIS;
            break;
        #endif
    }
    return (sub);
}

/*	----------------------------------------------------------------------------
    IntClearFlag
    ----------------------------------------------------------------------------
    IFSx: Interrupt Flag Status Registers
    16-05-2014 : RB replaced BitClear macro with atomic instructions
    --------------------------------------------------------------------------*/

void IntClearFlag(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
    if (numinter > 63)
    {
        //numinter -= 64;
        IFS2CLR = 1 << (numinter-64); // BitClear(IFS2, numinter);
    }
    else if (numinter > 31) 
    #else
    if (numinter > 31)
    #endif
    {
        //numinter -= 32;
        IFS1CLR = 1 << (numinter-32); // BitClear(IFS1, numinter);
    }
    else
    {
        IFS0CLR = 1 << numinter; // BitClear(IFS0, numinter);
    }
}

/*	----------------------------------------------------------------------------
    IntGetFlag
    ----------------------------------------------------------------------------
    Gets the interrupt request flag.
    This routine gets an interrupt request flag.
    Parameters:
        source  - Interrupt source.
    Returns:
        * 0 if the interrupt request flag is clear 
        * 1 if the interrupt request flag is set
    --------------------------------------------------------------------------*/

u32 IntGetFlag(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
    if (numinter > 63)
    {
        //numinter -= 64;
        return BitRead(IFS2, numinter-64);
    }
    else if (numinter > 31)
    #else
    if (numinter > 31)
    #endif
    {
        //numinter -= 32;
        return BitRead(IFS1, numinter-32);
    }
    else
    {
        return BitRead(IFS0, numinter);
    }
}

/*	----------------------------------------------------------------------------
    IntEnable
    ----------------------------------------------------------------------------
    Enables the interrupt.
    IECx: Interrupt Enable Control Registers
    16-05-2014 : RB replaced BitSet macro with atomic instructions
    --------------------------------------------------------------------------*/

void IntEnable(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
    if (numinter > 63)
    {
        //numinter -= 64;
        //IFS2SET = 1 << (numinter-64); // BitSet(IFS2, numinter);
        IEC2SET = 1 << (numinter-64); // BitSet(IEC2, numinter);
    }
    else if (numinter > 31) 
    #else
    if (numinter > 31)
    #endif
    {
        //numinter -= 32;
        IEC1SET = 1 << (numinter-32); // BitSet(IEC1, numinter);
    }
    else
    {
        IEC0SET = 1 << numinter; // BitSet(IEC0, numinter);
    }
}

/*	----------------------------------------------------------------------------
    IntDisable
    ----------------------------------------------------------------------------
    Disables the interrupt.
    IECx: Interrupt Enable Control Registers
    16-05-2014 : RB replaced BitClear macro with atomic instructions
    --------------------------------------------------------------------------*/

void IntDisable(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
    if (numinter > 63)
    {
        //numinter -= 64;
        //IFS2CLR = 1 << (numinter-64); // BitClear(IFS2, numinter);
        IEC2CLR = 1 << (numinter-64); // BitClear(IEC2, numinter);
    }
    else if (numinter > 31) 
    #else
    if (numinter > 31)
    #endif
    {
        //numinter -= 32;
        IEC1CLR = 1 << (numinter-32); // BitClear(IEC1, numinter);
    }
    else
    {
        IEC0CLR = 1 << numinter; // BitClear(IEC0, numinter);
    }
}

/*	----------------------------------------------------------------------------
    IntGetInterruptVectorNumber
    ----------------------------------------------------------------------------
    Gets the pending interrupt vector.
    This routine gets the pending interrupt vector number.
    Returns:
        The pending interrupt vector number.  
    --------------------------------------------------------------------------*/

u32 IntGetInterruptVectorNumber(void)
{
    return (u32)(INTSTATbits.VEC);
}

/*	----------------------------------------------------------------------------
    IntGetInterruptVectorPriority
    ----------------------------------------------------------------------------
    Gets the pending interrupt vector's priority.
    This routine gets the pending interrupt vector's priority.
    Returns:
        The pending interrupt vector's priority.
    --------------------------------------------------------------------------*/

u32 IntGetInterruptVectorPriority(void)
{
    return (u32)(INTSTATbits.SRIPL);
}

/*	----------------------------------------------------------------------------
    IntDisableInterrupts
    ----------------------------------------------------------------------------
    Disables the PIC32MX from handling interrupts.
    This routine disables the core from handling any pending interrupt requests.
    Returns:
    The previous state of the CP0 register Status.IE bit.  The INTRestoreInterrupts 
    function can be used in other routines to restore the system interrupt state.
    --------------------------------------------------------------------------*/
/*
u32 MIPS32 IntDisableInterrupts()
{
    u32 intStatus;

    intStatus = _CP0_GET_STATUS(); // Get Status
    asm("di"); // Disable all interrupts
    return intStatus;
}
*/
/*	----------------------------------------------------------------------------
    IntEnableInterrupts
    ----------------------------------------------------------------------------
    Enables the PIC32MX to handle interrupts.
    This routine enables the core to handle any pending interrupt requests.
    Need to configure system using INTConfigureSystem
    Returns: The previous state of the CP0 register Status.IE bit.
    The IntRestoreInterrupts function can be used in other routines to restore
    the system interrupt state.
    --------------------------------------------------------------------------*/
/*
u32 MIPS32 IntEnableInterrupts()
{
    u32 intStatus;

    intStatus = _CP0_GET_STATUS(); // Get Status
    asm("ei"); // Enable all interrupts
    return intStatus;
}
*/
/*	----------------------------------------------------------------------------
    IntRestoreInterrupts
    ----------------------------------------------------------------------------
    Restores the PIC32MX interrupt state.
    This routine restores the core to the previous interrupt handling state.
    Parameters:
        status      - the state of the CP0 register Status.IE
    --------------------------------------------------------------------------*/

void MIPS32 IntRestoreInterrupts(u32 intStatus)
{
    _CP0_SET_STATUS(intStatus); // Update Status
}

/*	----------------------------------------------------------------------------
    IntSetEBASE
    ----------------------------------------------------------------------------
    Sets the PIC32MX exception base.
    This routine sets the exception base of the core.
    Parameters:
        ebase_address   - The address of the EBASE.
        * must be be located in KSEG0 or KSEG1
        * must be 4KB aligned (1KB aligned for PIC32MX1 or 2 family)
    --------------------------------------------------------------------------*/

void MIPS32 IntSetEBASE(u32 ebase_address)
{
    _CP0_SET_EBASE(ebase_address);
}

/*	----------------------------------------------------------------------------
    IntConfigureSystem
    ----------------------------------------------------------------------------
    This routine configures  
    * 1 - the core to receive interrupt requests
    * 2 - the interrupt module for Multi-vectored or Single Vectored mode 

    Parameters:
        config      - The interrupt configuration to set.
    ex :
        IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
        IntConfigureSystem(INT_SYSTEM_CONFIG_SINGLE_VECTOR);
    
    All values must be the same as defined in the file "your_proc".ld
    --------------------------------------------------------------------------*/

void MIPS32 IntConfigureSystem(u8 mode)
{
    u32 temp;

    /// Disable all interrupts
    //asm volatile ("di");
    DisableInterrupt();

    /// Set BEV bit
    temp = _CP0_GET_STATUS();   // Get Status
    temp |= 0x00400000;         // Set BEV bit <22>
    _CP0_SET_STATUS(temp);      // Update Status
    
    /// Set EASE value (see lkr/your_proc.ld)
    #if defined(PINGUINO32MX270)

        _CP0_SET_EBASE(0xBD002000);

    #elif defined(PIC32_PINGUINO_220) || \
          defined(PINGUINO32MX220)    || \
          defined(PINGUINO32MX250)
        
        _CP0_SET_EBASE(0xBD003000);

    #else
    
        _CP0_SET_EBASE(0xBD005000);

    #endif

    /// Set the Vector Spacing to non-zero value
    _CP0_SET_INTCTL(0x00000020);

    /// Set CAUSE IV (Interrupt Vector bit)
    /// to use the special interrupt vector
    temp = _CP0_GET_CAUSE();    // Get Cause
    temp |= 0x00800000;         // Set IV <23>
    _CP0_SET_CAUSE(temp);       // Update Cause

    /// Clear BEV bit
    /// to place the special interrupt vector at the base
    /// of the vectored interrupt table.
    temp = _CP0_GET_STATUS();   // Get Status
    temp &= 0xFFBFFFFD;         // Clear BEV and EXL
    _CP0_SET_STATUS(temp);      // Update Status

    switch (mode)
    {
        case INT_SYSTEM_CONFIG_MULT_VECTOR:
            // Set the CP0 registers for multi-vector interrupt
            INTCONSET = 0x1000; // Set MVEC bit (bit 12 : 1<<12=0x1000)
            break;
            
        case INT_SYSTEM_CONFIG_SINGLE_VECTOR:
            // Set the CP0 registers for single-vector interrupt
            INTCONCLR = 0x1000; // Clear MVEC bit (bit 12 : 1<<12=0x1000)
            break;
    }

    /// Enable all interrupts
    //asm volatile ("ei"); // Enable all interrupts
    EnableInterrupt();
}

#endif	/* __INTERRUPT_C */

