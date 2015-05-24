/* ISRwrapper.inc - PIC32MX ISR wrapper code - macro ISR_wrapper
**
** Define ISR wrapper and vector branch code to support "C" ISR.
** For a given vector_XX, create wrapper code to direct vector
** XX to the named C-language ISR.
**
** To use with any Microchip headers (not recommended),
** compile with "-fdollars-in-identifiers" option.
**
** History:
**   20090505 DRNadler: Original coding
**
**
** Copyright (c) 2009 Dave Nadler
**
** The authors hereby grant permission to use, copy, modify, distribute,
** and license this software and its documentation for any purpose, provided
** that existing copyright notices are retained in all copies and that this
** notice is included verbatim in any distributions.  No written agreement,
** license, or royalty fee is required for any of the authorized uses.
** Modifications to this software may be copyrighted by their authors
** and need not follow the licensing terms described here, provided that
** the new terms are clearly indicated on the first page of each file where
** they apply.
*/

#include <p32xxxx.h>

        .macro  ISR_wrapper     _XX:req,C_ISR_NAME:req

  /*
  ** Fill in interrupt vector to branch to the ISR wrapper
  ** cf. Vector Sections in elf32pic32mx.x
  */
  
    .section .vector_\_XX,"ax",%progbits
    j       vector_\_XX\()_ISR_wrapper
    nop

  /*
  ** Create an ISR wrapper that calls the C-language routine
  
    Register name   Register number     Usage
    STATUS          12                  Interrupt mask and enable bits
    CAUSE           13                  Exception type
    EPC             14                  Register containing the following
                                        address of the instruction where
                                        the exception occurred
  */

    .section .text,"ax",%progbits
        .align  2
        .set    nomips16
    .globl  vector_\_XX\()_ISR_wrapper
    .type   vector_\_XX\()_ISR_wrapper, %function
    .ent    vector_\_XX\()_ISR_wrapper
    
vector_\_XX\()_ISR_wrapper:
    .frame  $fp,120,$31		# vars= 16, regs= 18/0, args= 16, gp= 0
    .mask   0xc300fffc,-20
    .fmask  0x00000000,0
    .set    noreorder
    .set    nomacro

    rdpgpr  $sp, $sp
    mfc0    $k0, $13        /* read CAUSE register */
    mfc0    $k1, $14        /* read EPC (Exception Program Counter) register */
    addiu   $sp, $sp, -120  /* 30 (!) words space on stack */
    sw      $k1, 116($sp)   /* save EPC (Exception Program Counter) on stack */
    mfc0    $k1, $12        /* read STATUS register */
    srl     $k0, $k0, 10    /* align RIPL (Requested Priority Level bits) to bit 0 */
    sw      $k1, 112($sp)   /* save STATUS on stack */
    ins     $k1, $k0, 10, 6 /* insert RIPL (Requested Priority Level bits) to IPL field */
    mflo    $k0
    sw      $k0, 108($sp)
    mfhi    $k0
    sw      $k0, 104($sp)
    ins     $k1, $0, 1, 4
    mtc0    $k1, $12        /* write STATUS register */
    
    sw	$31,100($sp)
    sw	$fp,96($sp)
    sw	$25,92($sp)
    sw	$24,88($sp)
    sw	$15,84($sp)
    sw	$14,80($sp)
    sw	$13,76($sp)
    sw	$12,72($sp)
    sw	$11,68($sp)
    sw	$10,64($sp)
    sw	$9,60($sp)
    sw	$8,56($sp)
    sw	$7,52($sp)
    sw	$6,48($sp)
    sw	$5,44($sp)
    sw	$4,40($sp)
    sw	$3,36($sp)
    sw	$2,32($sp)
    move	$fp,$sp

    jal	\C_ISR_NAME         /* Finally, call the C-Language ISR */
    nop                     /* jal stores return address in $31, already saved... */

    move	$sp,$fp
    lw	$31,100($sp)
    lw	$fp,96($sp)
    lw	$25,92($sp)
    lw	$24,88($sp)
    lw	$15,84($sp)
    lw	$14,80($sp)
    lw	$13,76($sp)
    lw	$12,72($sp)
    lw	$11,68($sp)
    lw	$10,64($sp)
    lw	$9,60($sp)
    lw	$8,56($sp)
    lw	$7,52($sp)
    lw	$6,48($sp)
    lw	$5,44($sp)
    lw	$4,40($sp)
    lw	$3,36($sp)
    lw	$2,32($sp)
    
    di                      /* disable interrupts - just in case ? where enabled ? */
    ehb
    lw	$k0, 108($sp)
    mtlo	$k0
    lw	$k0, 104($sp)
    mthi	$k0
    lw	$k0, 116($sp)       /* restore EPC from stack */
    mtc0	$k0, $14
    lw	$k0, 112($sp)
    addiu	$sp, $sp, 120   /* restore stack pointer */
    mtc0	$k0, $12
    eret
    .set	macro
    .set	reorder
    .end	vector_\_XX\()_ISR_wrapper

        .endm

    /*
    ** Create wrappers for ISRs used in specific libraries
    ** 32MX2xx and 32MX4xx do not have UART3,4,5 AND 6
    */

    ISR_wrapper _TIMER_1_VECTOR, Timer1Interrupt
    ISR_wrapper _TIMER_2_VECTOR, Timer2Interrupt
    ISR_wrapper _TIMER_3_VECTOR, Timer3Interrupt
    ISR_wrapper _TIMER_4_VECTOR, Timer4Interrupt
    ISR_wrapper _TIMER_5_VECTOR, Timer5Interrupt

    #if defined(__32MX440F256H__)
    
    ISR_wrapper _SPI_2_VECTOR,   SPI2Interrupt

    #elif defined(__32MX795F512L__) || \
          defined(__32MX795F512H__)

    ISR_wrapper _SPI_1_VECTOR,   SPI1Interrupt
    ISR_wrapper _SPI_2_VECTOR,   SPI2Interrupt
    ISR_wrapper _SPI_3_VECTOR,   SPI3Interrupt
    ISR_wrapper _SPI_4_VECTOR,   SPI4Interrupt

    #else
    
    ISR_wrapper _SPI_1_VECTOR,   SPI1Interrupt
    ISR_wrapper _SPI_2_VECTOR,   SPI2Interrupt

    #endif
    
    ISR_wrapper _UART_1_VECTOR,  Serial1Interrupt
    ISR_wrapper _UART_2_VECTOR,  Serial2Interrupt

    #if defined(__32MX795F512L__) || \
        defined(__32MX795F512H__)

        #ifdef ENABLE_UART3
    ISR_wrapper _UART_3_VECTOR,  Serial3Interrupt
        #else
    ISR_wrapper _SPI_2_VECTOR,   SPI2Interrupt
        #endif
        
    ISR_wrapper _UART_4_VECTOR,  Serial4Interrupt
    ISR_wrapper _UART_5_VECTOR,  Serial5Interrupt
    ISR_wrapper _UART_6_VECTOR,  Serial6Interrupt

    #endif
    
    ISR_wrapper _RTCC_VECTOR,    RTCCInterrupt

    ISR_wrapper _USB_1_VECTOR,   USBInterrupt
