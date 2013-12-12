/*	----------------------------------------------------------------------------
	FILE:				interrupt.c
	PROJECT:			pinguino32X
	PURPOSE:			interrupts management
	PROGRAMER:			regis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:		16 nov. 2010
	LAST RELEASE:		23 set. 2011
	----------------------------------------------------------------------------
	CHANGELOG:
    ???             Marcus Fazzi <anunakin@gmail.com> added UART3/4/5/6 for PIC32MX795 support
    05 jul. 2012    Gagabi Added support for GENERIC 32 bits boards in intconfiguresystem()
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

	// INTERRUPT CONFIG MODE
	#define INT_SYSTEM_CONFIG_MULT_VECTOR	1
	#define INT_SYSTEM_CONFIG_SINGLE_VECTOR 2

	// INTERRUPT ENABLE/DISABLE
	#define INT_ENABLED						1
	#define INT_DISABLED					0

	// INTERRUPT VECTOR LOCATION
	#define INT_CORE_TIMER_VECTOR			0
	#define INT_CORE_SOFTWARE0_VECTOR		1
	#define INT_CORE_SOFTWARE1_VECTOR		2
	#define INT_EXTERNAL0_VECTOR			3
	#define INT_TIMER1_VECTOR				4
	#define INT_INPUT_CAPTURE1_VECTOR		5  
	#define INT_OUTPUT_COMPARE1_VECTOR		6   
	#define INT_EXTERNAL1_VECTOR			7          
	#define INT_TIMER2_VECTOR				8   
	#define INT_INPUT_CAPTURE2_VECTOR		9  
	#define INT_OUTPUT_COMPARE2_VECTOR		10   
	#define INT_EXTERNAL2_VECTOR			11          
	#define INT_TIMER3_VECTOR				12   
	#define INT_INPUT_CAPTURE3_VECTOR		13  
	#define INT_OUTPUT_COMPARE3_VECTOR		14   
	#define INT_EXTERNAL3_VECTOR			15          
	#define INT_TIMER4_VECTOR				16   
	#define INT_INPUT_CAPTURE4_VECTOR		17  
	#define INT_OUTPUT_COMPARE4_VECTOR		18   
	#define INT_EXTERNAL4_VECTOR			19          
	#define INT_TIMER5_VECTOR				20   
	#define INT_INPUT_CAPTURE5_VECTOR		21  
	#define INT_OUTPUT_COMPARE5_VECTOR		22   
	#define INT_SPI1_VECTOR					23    
	#define INT_UART1_VECTOR				24    
	#define INT_I2C1_VECTOR					25   
	#define INT_INPUT_CHANGE_VECTOR			26    
	#define INT_ADC1_CONVERT_DONE_VECTOR	27          
	#define INT_PARALLEL_MASTER_PORT_VECTOR	28  
	#define INT_COMPARATOR1_VECTOR			29          
	#define INT_COMPARATOR2_VECTOR			30          
#ifdef ENABLE_UART3
	#define INT_UART3_VECTOR			    31
#else
	#define INT_SPI2_VECTOR					31
#endif
	#define INT_UART2_VECTOR				32
	#define INT_I2C2_VECTOR					33
	#define INT_FSCM_VECTOR					34
	#define INT_RTCC_VECTOR					35
	#define INT_DMA0_VECTOR					36
	#define INT_DMA1_VECTOR					37
	#define INT_DMA2_VECTOR					38
	#define INT_DMA3_VECTOR					39
	#define INT_FCE_VECTOR					44
	#define INT_USB_VECTOR					45
//PIC32MX795
	#define INT_CAN1_VECTOR					46
	#define INT_CAN2_VECTOR					47
	#define INT_ETH_VECTOR					48
#ifdef ENABLE_UART4
	#define INT_UART4_VECTOR			    49
#endif
#ifdef ENABLE_UART6
	#define INT_UART6_VECTOR			    50
#endif
#ifdef ENABLE_UART5
	#define INT_UART5_VECTOR			    51
#endif

	// IRQ NUMBER = INTERRUPT NUMBER (IFSx, IECx and IPSx registers)
	#define INT_CORE_TIMER					0
	#define INT_CORE_SOFTWARE0				1
	#define INT_CORE_SOFTWARE1				2
	#define INT_EXTERNAL0					3
	#define INT_TIMER1						4
	#define INT_INPUT_CAPTURE1				5
	#define INT_OUTPUT_COMPARE1				6
	#define INT_EXTERNAL1					7
	#define INT_TIMER2						8
	#define INT_INPUT_CAPTURE2				9
	#define INT_OUTPUT_COMPARE2				10
	#define INT_EXTERNAL2					11
	#define INT_TIMER3						12
	#define INT_INPUT_CAPTURE3				13
	#define INT_OUTPUT_COMPARE3				14
	#define INT_EXTERNAL3					15
	#define INT_TIMER4						16
	#define INT_INPUT_CAPTURE4				17
	#define INT_OUTPUT_COMPARE4				18
	#define INT_EXTERNAL4					19
	#define INT_TIMER5						20
	#define INT_INPUT_CAPTURE5				21
	#define INT_OUTPUT_COMPARE5				22
	#define INT_SPI1_FAULT					23 
	#define INT_SPI1_TRANSFER_DONE			24 
	#define INT_SPI1_RECEIVE_DONE			25
	#define INT_UART1_ERROR					26 
	#define INT_UART1_RECEIVER				27
	#define INT_UART1_TRANSMITTER			28
	#define INT_I2C1_BUS_COLLISION_EVENT	29
	#define INT_I2C1_SLAVE_EVENT			30
	#define INT_I2C1_MASTER_EVENT			31
	#define INT_INPUT_CHANGE				32
	#define INT_ADC1_CONVERT_DONE			33
	#define INT_PARALLEL_MASTER_PORT		34
	#define INT_COMPARATOR1					35
	#define INT_COMPARATOR2					36
#ifdef ENABLE_UART3
	#define INT_UART3_ERROR					37
	#define INT_UART3_RECEIVER				38
	#define INT_UART3_TRANSMITTER			39
#else
	#define INT_SPI2_FAULT					37 
	#define INT_SPI2_TRANSFER_DONE			38 
	#define INT_SPI2_RECEIVE_DONE			39
#endif
	#define INT_UART2_ERROR					40 
	#define INT_UART2_RECEIVER				41 
	#define INT_UART2_TRANSMITTER			42
	#define INT_I2C2_BUS_COLLISION_EVENT	43 
	#define INT_I2C2_SLAVE_EVENT			44 
	#define INT_I2C2_MASTER_EVENT			45 
	#define INT_FAIL_SAFE_CLOCK_MONITOR		46
	#define INT_REAL_TIME_CLOCK				47
	#define INT_DMA_CHANNEL_0				48 
	#define INT_DMA_CHANNEL_1				49 
	#define INT_DMA_CHANNEL_2				50 
	#define INT_DMA_CHANNEL_3				51 
	#define INT_FLASH_CONTROL_EVENT			56
	#define INT_USB							57
	#define INT_CAN1						58
	#define INT_CAN2						59
	#define INT_ETH							60
#ifdef ENABLE_UART4
	#define INT_UART4_ERROR					67
	#define INT_UART4_RECEIVER				68
	#define INT_UART4_TRANSMITTER			69
#endif
#ifdef ENABLE_UART6
	#define INT_UART6_ERROR					70
	#define INT_UART6_RECEIVER				71
	#define INT_UART6_TRANSMITTER			72
#endif
#ifdef ENABLE_UART5
	#define INT_UART5_ERROR					73
	#define INT_UART5_RECEIVER				74
	#define INT_UART5_TRANSMITTER			75
#endif
/*	----------------------------------------------------------------------------
	Interrupt Natural Priority
	----------------------------------------------------------------------------
	When multiple interrupts are assigned to same group priority and subpriority,
	they are prioritized by their natural priority. The natural priority is a
	fixed priority scheme, where the highest natural priority starts at the lowest
	interrupt vector, meaning that interrupt vector 0 is the highest and interrupt
	vector 63 is the lowest natural priority. See the interrupt vector table in
	the respective device data sheet to learn the natural priority order of each IRQ.
	--------------------------------------------------------------------------*/

	// IPCx: INTERRUPT PRIORITY CONTROL REGISTER
	#define INT_UART1_ALL_PRIORITY		0x0000001F	// disable all UART1 interrupts
	#define INT_UART2_ALL_PRIORITY		0x0000001F	// disable all UART2 interrupts
	#define INT_UART3_ALL_PRIORITY		0x0000001F	// disable all UART3 interrupts
	#define INT_PRIORITY_7				0b111
	#define INT_PRIORITY_6				0b110
	#define INT_PRIORITY_5				0b101
	#define INT_PRIORITY_4				0b100
	#define INT_PRIORITY_3				0b011
	#define INT_PRIORITY_2				0b010
	#define INT_PRIORITY_1				0b001
	#define INT_PRIORITY_DISABLED		0b000
	#define INT_SUBPRIORITY_3			0b11
	#define INT_SUBPRIORITY_2			0b10
	#define INT_SUBPRIORITY_1			0b01
	#define INT_SUBPRIORITY_0			0b00

	// IEC0
	#define I2C1_M_INT_ENABLE			0x80000000
	#define I2C1_S_INT_ENABLE			0x40000000
	#define I2C1_B_INT_ENABLE			0x20000000
/*	--------------------------------------------------------------------------*/
	#define UART1_TX_INT_ENABLE			0x10000000
	#define UART1_RX_INT_ENABLE			0x08000000
	#define UART1_ERROR_INT_ENABLE		0x04000000
	#define UART1_ALL_INTERRUPT			0x1C000000
/*	--------------------------------------------------------------------------*/
	#define SPI1_RX_INT_ENABLE			0x02000000
	#define SPI1_TX_INT_ENABLE			0x01000000	
	#define SPI1_ERROR_INT_ENABLE		0x00800000
	#define OUTPUT_COMPARE5_INT_ENABLE	0x00400000
	#define IC5_INT_ENABLE				0x00200000
	#define TIMER5_INT_ENABLE			0x00100000
	#define EXTERNAL4_INT_ENABLE		0x00080000
	#define OUTPUT_COMPARE4_INT_ENABLE	0x00040000
	#define IC4_INT_ENABLE				0x00020000
	#define TIMER4_INT_ENABLE			0x00010000
	#define EXTERNAL3_INT_ENABLE		0x00008000
	#define OUTPUT_COMPARE3_INT_ENABLE	0x00004000
	#define IC3_INT_ENABLE				0x00002000
	#define TIMER3_INT_ENABLE			0x00001000
	#define EXTERNAL2_INT_ENABLE		0x00000800
	#define OUTPUT_COMPARE2_INT_ENABLE	0x00000400
	#define IC2_INT_ENABLE				0x00000200
	#define TIMER2_INT_ENABLE			0x00000100
	#define EXTERNAL1_INT_ENABLE		0x00000080
	#define OUTPUT_COMPARE1_INT_ENABLE	0x00000040
	#define IC1_INT_ENABLE				0x00000020
	#define TIMER1_INT_ENABLE			0x00000010
	#define EXTERNAL0_INT_ENABLE		0x00000008
	#define CORE_SYSTEM1_INT_ENABLE		0x00000004
	#define CORE_SYSTEM0_INT_ENABLE		0x00000002
	#define CORE_TIMER_INT_ENABLE		0x00000001

	// IEC1
	#define USB_INT_ENABLE				0x02000000
	#define FCE_INT_ENABLE				0x01000000
	#define DMA3_INT_ENABLE				0x00080000
	#define DMA2_INT_ENABLE				0x00040000
	#define DMA1_INT_ENABLE				0x00020000
	#define DMA0_INT_ENABLE				0x00010000
	#define RTCC_INT_ENABLE				0x00008000
	#define FSCM_INT_ENABLE				0x00004000
	#define I2C2_M_INT_ENABLE			0x00002000
	#define I2C2_S_INT_ENABLE			0x00001000
	#define I2C2_B_INT_ENABLE			0x00000800
/*	--------------------------------------------------------------------------*/
	#define UART2_TX_INT_ENABLE			0x00000400
	#define UART2_RX_INT_ENABLE			0x00000200
	#define UART2_ERROR_INT_ENABLE		0x00000100
	#define UART2_ALL_INTERRUPT			0x00000700
/*	--------------------------------------------------------------------------*/
#ifdef ENABLE_UART3
	#define UART3_TX_INT_ENABLE			0x00000080
	#define UART3_RX_INT_ENABLE			0x00000040
	#define UART3_ERROR_INT_ENABLE		0x00000020
	#define UART3_ALL_INTERRUPT			0x000000E0
#else
	#define SPI2RX_INT_ENABLE			0x00000080
	#define SPI2TX_INT_ENABLE			0x00000040
	#define SPI2E_INT_ENABLE			0x00000020
#endif
	#define CMP2_INT_ENABLE				0x00000010
	#define CMP1_INT_ENABLE				0x00000008
	#define PMP_INT_ENABLE				0x00000004
	#define AD1_INT_ENABLE				0x00000002
	#define CN_INT_ENABLE				0x00000001
//PIC32MX795
//if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
	#define CAN1_INT_ENABLE				0x04000000
	#define CAN2_INT_ENABLE				0x08000000
	#define ETH_INT_ENABLE				0x10000000

//IEC2
#ifdef ENABLE_UART4
	#define UART4_TX_INT_ENABLE			0x00000020
	#define UART4_RX_INT_ENABLE			0x00000010
	#define UART4_ERROR_INT_ENABLE		0x00000008
	#define UART4_ALL_INTERRUPT			0x00000038
#endif
#ifdef ENABLE_UART5
	#define UART5_TX_INT_ENABLE			0x00000800
	#define UART5_RX_INT_ENABLE			0x00000400
	#define UART5_ERROR_INT_ENABLE		0x00000200
	#define UART5_ALL_INTERRUPT			0x00000E00
#endif
#ifdef ENABLE_UART6
	#define UART6_TX_INT_ENABLE			0x00000100
	#define UART6_RX_INT_ENABLE			0x00000080
	#define UART6_ERROR_INT_ENABLE		0x00000040
	#define UART6_ALL_INTERRUPT			0x000001C0
#endif
//endif
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
		case INT_COMPARATOR2_VECTOR:
			break;
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
			break;
		case INT_CAN1_VECTOR:
			break;
		case INT_CAN2_VECTOR:
			break;
		case INT_ETH_VECTOR:
			break;
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

unsigned int IntGetVectorPriority(u8 vector)
{
	unsigned int pri = 0;

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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
		case INT_CAN1_VECTOR:
			break;
		case INT_CAN2_VECTOR:
			break;
		case INT_ETH_VECTOR:
			break;
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

unsigned int IntGetVectorSubPriority(u8 vector)
{
	unsigned int sub = 0;

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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
#if defined(PIC32_PINGUINO_220)||defined(PINGUINO32MX250)||defined(PINGUINO32MX220)
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
		case INT_CAN1_VECTOR:
			break;
		case INT_CAN2_VECTOR:
			break;
		case INT_ETH_VECTOR:
			break;
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
	--------------------------------------------------------------------------*/

void IntClearFlag(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
	if (numinter > 63)
	{
		numinter -= 64;
		BitClear(IFS2, numinter);
	}
	else if (numinter > 31) 
    #else
	if (numinter > 31)
    #endif
	{
		numinter -= 32;
		BitClear(IFS1, numinter);
	}
	else
		BitClear(IFS0, numinter);
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

unsigned int IntGetFlag(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
	if (numinter > 63)
	{
		numinter -= 64;
		return BitRead(IFS2, numinter);
	}
	else if (numinter > 31)
    #else
	if (numinter > 31)
    #endif
	{		
		numinter -= 32;
		return BitRead(IFS1, numinter);
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
	--------------------------------------------------------------------------*/

void IntEnable(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
	if (numinter > 63)
	{
		numinter -= 64;
		BitSet(IFS2, numinter);
	}
	else if (numinter > 31) 
    #else
	if (numinter > 31)
    #endif
	{
		numinter -= 32;
		BitSet(IEC1, numinter);
	}
	else
	{
		BitSet(IEC0, numinter);
	}
}

/*	----------------------------------------------------------------------------
	IntDisable
	----------------------------------------------------------------------------
	Disables the interrupt.
	IECx: Interrupt Enable Control Registers
	--------------------------------------------------------------------------*/

void IntDisable(u8 numinter)
{
    #if defined(UBW32_795) || defined(EMPEROR795) || defined(PIC32_PINGUINO_T795)
	if (numinter > 63)
	{
		numinter -= 64;
		BitClear(IFS2, numinter);
	}
	else if (numinter > 31) 
    #else
	if (numinter > 31)
    #endif
	{
		numinter -= 32;
		BitClear(IEC1, numinter);
	}
	else
	{
		BitClear(IEC0, numinter);
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

unsigned int IntGetInterruptVectorNumber(void)
{
    return (unsigned int)(INTSTATbits.VEC);
}

/*	----------------------------------------------------------------------------
	IntGetInterruptVectorPriority
	----------------------------------------------------------------------------
	Gets the pending interrupt vector's priority.
	This routine gets the pending interrupt vector's priority.
	Returns:
		The pending interrupt vector's priority.
	--------------------------------------------------------------------------*/

unsigned int IntGetInterruptVectorPriority(void)
{
    return (unsigned int)(INTSTATbits.SRIPL);
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

unsigned int MIPS32 IntDisableInterrupts()
{
	unsigned int intStatus;

	intStatus = _CP0_GET_STATUS(); // Get Status
	asm("di"); // Disable all interrupts
	return intStatus;
}

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

unsigned int MIPS32 IntEnableInterrupts()
{
	unsigned int intStatus;

	intStatus = _CP0_GET_STATUS(); // Get Status
	asm("ei"); // Enable all interrupts
	return intStatus;
}

/*	----------------------------------------------------------------------------
	IntRestoreInterrupts
	----------------------------------------------------------------------------
	Restores the PIC32MX interrupt state.
	This routine restores the core to the previous interrupt handling state.
	Parameters:
		status      - the state of the CP0 register Status.IE
	--------------------------------------------------------------------------*/

void MIPS32 IntRestoreInterrupts(unsigned int intStatus)
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
		* must be 4KB aligned
	NB : EBASE at 0xBD000000 (microchip) or 0xBD005000 (jean-pierre mandon)
	--------------------------------------------------------------------------*/

void MIPS32 IntSetEBASE(unsigned int ebase_address)
{
	_CP0_SET_EBASE(ebase_address);
}

/*	----------------------------------------------------------------------------
	IntConfigureSystem
	----------------------------------------------------------------------------
	Configures the system for  multi-vector or single vectored interrupts.
	This routine configures the core to receive interrupt requests and configures the 
	Interrupt module for Multi-vectored or Single Vectored mode.
	Parameters:
		config      - The interrupt configuration to set.
		IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
		IntConfigureSystem(INT_SYSTEM_CONFIG_SINGLE_VECTOR);
	NB : Place EBASE at 0xBD000000 (microchip) or 0xBD005000 (jean-pierre mandon)
	--------------------------------------------------------------------------*/

void MIPS32 IntConfigureSystem(u8 mode)
{
	unsigned int temp;

	asm("di"); // Disable all interrupts
	temp = _CP0_GET_STATUS(); // Get Status
	temp |= 0x00400000; // Set BEV bit
	_CP0_SET_STATUS(temp); // Update Status
	#if defined(PIC32_PINGUINO_220) || defined(PINGUINO32MX250) || defined(PINGUINO32MX220)
	_CP0_SET_EBASE(0xBD003000); // Set an EBase value of 0xBD003000	
	#else
	_CP0_SET_EBASE(0xBD005000); // Set an EBase value of 0xBD005000	
	#endif
	_CP0_SET_INTCTL(0x00000020); // Set the Vector Spacing to non-zero value
	temp = _CP0_GET_CAUSE(); // Get Cause
	temp |= 0x00800000; // Set IV
	_CP0_SET_CAUSE(temp); // Update Cause
	temp = _CP0_GET_STATUS(); // Get Status
	temp &= 0xFFBFFFFD; // Clear BEV and EXL
	_CP0_SET_STATUS(temp); // Update Status

	switch (mode)
	{
		case INT_SYSTEM_CONFIG_MULT_VECTOR:
			// Set the CP0 registers for multi-vector interrupt
			INTCONSET = 0x1000; // Set MVEC bit
		break;
		case INT_SYSTEM_CONFIG_SINGLE_VECTOR:
			// Set the CP0 registers for single-vector interrupt
			INTCONCLR = 0x1000; // Clear MVEC bit
		break;	
	}

	asm("ei"); // Enable all interrupts
}

#endif	/* __INTERRUPT_C */

