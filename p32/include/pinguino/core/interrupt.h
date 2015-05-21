/*	----------------------------------------------------------------------------
    FILE:				interrupt.h
    PROJECT:			pinguino 32
    PURPOSE:			interrupts management
    PROGRAMER:			regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:		16 Nov. 2010
    LAST RELEASE:		14 Jan. 2015
    ----------------------------------------------------------------------------
    CHANGELOG:
    ???          : Marcus Fazzi <anunakin@gmail.com> added UART3/4/5/6 for PIC32MX795 support
    05 jul. 2012 : Gagabi Added support for GENERIC 32 bits boards in intconfiguresystem()
    10 May  2014 : Joël fixed wrong interrupt vector number for MX1xx and MX2xx family
    16 May  2014 : Régis replaced BitSet and BitClear macro with register set and clear instructions
    14 Jan  2015 : Régis splited interrupt.c in interrupt.c and interrupt.h
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

#ifndef __INTERRUPT_H
#define __INTERRUPT_H

    #include <p32xxxx.h>
    #include <typedef.h>
    #include <macro.h>

    #define __ISR(v,ipl) __attribute__((vector(v), interrupt(ipl), nomips16, noinline))

    // INTERRUPT CONFIG MODE
    #define INT_SYSTEM_CONFIG_MULT_VECTOR	1
    #define INT_SYSTEM_CONFIG_SINGLE_VECTOR 2

    // INTERRUPT ENABLE/DISABLE
    #define INT_ENABLED						1
    #define INT_DISABLED					0
    #define INT_ENABLE                      1
    #define INT_DISABLE                     0
    
    // ONEVENT
    #define INT_RISING_EDGE                 1   // Interrupt on rising edge
    #define INT_FALLING_EDGE                0   // Interrupt on falling edge
    #define INT_NOT_USED                    0
    #define INT_USED                        0xFF
    #define INT_MICROSEC                    1
    #define INT_MILLISEC                    2
    #define INT_SEC                         3

    // RB 04-03-2015 : REDUNDANT WITH p32xxxx.h, SHOULD BE REMOVED
    // INTERRUPT VECTOR LOCATION
    #if defined(PIC32_PINGUINO_220) || \
        defined(PINGUINO32MX220)    || \
        defined(PINGUINO32MX250)    || \
        defined(PINGUINO32MX270)

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
        #define INT_ADC1_CONVERT_DONE_VECTOR	23
        #define INT_FSCM_VECTOR					24
        #define INT_RTCC_VECTOR					25
        #define INT_FCE_VECTOR					26
        #define INT_COMPARATOR1_VECTOR			27
        #define INT_COMPARATOR2_VECTOR			28
        #define INT_COMPARATOR3_VECTOR			29
        #define INT_USB_VECTOR					30
        #define INT_SPI1_VECTOR					31
        #define INT_UART1_VECTOR				32
        #define INT_I2C1_VECTOR					33
        #define INT_INPUT_CHANGE_VECTOR			34
        #define INT_PARALLEL_MASTER_PORT_VECTOR	35
        #define INT_SPI2_VECTOR					36
        #define INT_UART2_VECTOR				37
        #define INT_I2C2_VECTOR					38
        #define INT_CTMU_VECTOR					39
        #define INT_DMA0_VECTOR					40
        #define INT_DMA1_VECTOR					41
        #define INT_DMA2_VECTOR					42
        #define INT_DMA3_VECTOR					43
     
    #else
  
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

    #endif

    // IRQ NUMBER = INTERRUPT NUMBER (IFSx, IECx and IPSx registers)
    #if defined(PIC32_PINGUINO_220) || \
        defined(PINGUINO32MX220)    || \
        defined(PINGUINO32MX250)    || \
        defined(PINGUINO32MX270)

        #define INT_CORE_TIMER					0
        #define INT_CORE_SOFTWARE0				1
        #define INT_CORE_SOFTWARE1				2
        #define INT_EXTERNAL0					3
        #define INT_TIMER1						4
        #define INT_INPUT_CAPTURE1_ERROR		5
        #define INT_INPUT_CAPTURE1				6
        #define INT_OUTPUT_COMPARE1				7
        #define INT_EXTERNAL1					8
        #define INT_TIMER2						9
        #define INT_INPUT_CAPTURE2_ERROR		10
        #define INT_INPUT_CAPTURE2              11
        #define INT_OUTPUT_COMPARE2				12
        #define INT_EXTERNAL2					13
        #define INT_TIMER3						14
        #define INT_INPUT_CAPTURE3_ERROR		15
        #define INT_INPUT_CAPTURE3				16
        #define INT_OUTPUT_COMPARE3				17
        #define INT_EXTERNAL3					18
        #define INT_TIMER4						19
        #define INT_INPUT_CAPTURE4_ERROR		20
        #define INT_INPUT_CAPTURE4				21
        #define INT_OUTPUT_COMPARE4				22
        #define INT_EXTERNAL4					23
        #define INT_TIMER5						24
        #define INT_INPUT_CAPTURE5_ERROR		25
        #define INT_INPUT_CAPTURE5				26
        #define INT_OUTPUT_COMPARE5				27
        #define INT_ADC1_CONVERT_DONE			28
        #define INT_FAIL_SAFE_CLOCK_MONITOR		29
        #define INT_REAL_TIME_CLOCK				30
        #define INT_FLASH_CONTROL_EVENT			31
        #define INT_COMPARATOR1					32
        #define INT_COMPARATOR2					33
        #define INT_COMPARATOR3					34
        #define INT_USB							35 
        #define INT_SPI1_FAULT					36 
        #define INT_SPI1_TRANSFER_DONE			38
        #define INT_SPI1_RECEIVE_DONE			37
        #define INT_UART1_ERROR					39 
        #define INT_UART1_RECEIVER				40
        #define INT_UART1_TRANSMITTER			41
        #define INT_I2C1_BUS_COLLISION_EVENT	42
        #define INT_I2C1_SLAVE_EVENT			43
        #define INT_I2C1_MASTER_EVENT			44
        #define INT_INPUTA_CHANGE				45
        #define INT_INPUTB_CHANGE				46
        #define INT_INPUTC_CHANGE				47
        #define INT_PARALLEL_MASTER_PORT		48
        #define INT_PARALLEL_MASTER_PORT_ERROR	49
        #define INT_SPI2_FAULT					50
        #define INT_SPI2_TRANSFER_DONE			52 
        #define INT_SPI2_RECEIVE_DONE			51
        #define INT_UART2_ERROR					53 
        #define INT_UART2_RECEIVER				54 
        #define INT_UART2_TRANSMITTER			55
        #define INT_I2C2_BUS_COLLISION_EVENT	56 
        #define INT_I2C2_SLAVE_EVENT			57
        #define INT_I2C2_MASTER_EVENT			58 
        #define INT_CTMU_EVENT                  59
        #define INT_DMA_CHANNEL_0				60 
        #define INT_DMA_CHANNEL_1				61 
        #define INT_DMA_CHANNEL_2				62 
        #define INT_DMA_CHANNEL_3				63 
        #define INT_NUM                         64

    #else

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
        
        #define INT_NUM                         76

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
    // -----------------------------------------------------------------
    #define UART1_TX_INT_ENABLE			0x10000000
    #define UART1_RX_INT_ENABLE			0x08000000
    #define UART1_ERROR_INT_ENABLE		0x04000000
    #define UART1_ALL_INTERRUPT			0x1C000000
    // -----------------------------------------------------------------
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
    // -----------------------------------------------------------------
    #define UART2_TX_INT_ENABLE			0x00000400
    #define UART2_RX_INT_ENABLE			0x00000200
    #define UART2_ERROR_INT_ENABLE		0x00000100
    #define UART2_ALL_INTERRUPT			0x00000700
    // -----------------------------------------------------------------
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

#endif	/* __INTERRUPT_H */

