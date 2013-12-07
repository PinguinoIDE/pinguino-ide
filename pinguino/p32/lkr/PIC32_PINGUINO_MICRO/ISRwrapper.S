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

        .macro  ISR_wrapper     _XX:req,C_ISR_NAME:req

  /*
  ** First, fill in interrupt vector to branch to the ISR wrapper
  */
	.section .vector_\_XX,"ax",%progbits
	j	vector_\_XX\()_ISR_wrapper
	nop

  /*
  ** Second, create an ISR wrapper that calls the C-language routine
  */
	.section .text,"ax",%progbits
        .align  2
        .set    nomips16
	.globl  vector_\_XX\()_ISR_wrapper
	.type	vector_\_XX\()_ISR_wrapper, %function
	.ent	vector_\_XX\()_ISR_wrapper
vector_\_XX\()_ISR_wrapper:
	.frame	$fp,120,$31		# vars= 16, regs= 18/0, args= 16, gp= 0
	.mask	0xc300fffc,-20
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro

	rdpgpr	$sp, $sp
	mfc0	$k0, $13		/* read CAUSE register */
	mfc0	$k1, $14		/* read EPC register */
	addiu	$sp, $sp, -120		/* 30 (!) words space on stack */
	sw	$k1, 116($sp)		/* save EPC on stack */
	mfc0	$k1, $12		/* read STATUS register */
	srl	$k0, $k0, 10		/* align RIPL to bit 0 */
	sw	$k1, 112($sp)		/* save STATUS on stack */
	ins	$k1, $k0, 10, 6		/* insert RIPL to IPL field */
	mflo	$k0
	sw	$k0, 108($sp)
	mfhi	$k0
	sw	$k0, 104($sp)
	ins	$k1, $0, 1, 4
	mtc0	$k1, $12		/* write STATUS register */
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

        /* Finally, call the C-Language ISR */
	jal	\C_ISR_NAME     /* jal stores return address in $31, already saved... */
	nop

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
	di				/* disable interrupts - just in case ? where enabled ? */
	ehb
	lw	$k0, 108($sp)
	mtlo	$k0
	lw	$k0, 104($sp)
	mthi	$k0
	lw	$k0, 116($sp)		/* restore EPC from stack */
	mtc0	$k0, $14
	lw	$k0, 112($sp)
	addiu	$sp, $sp, 120		/* restore stack pointer */
	mtc0	$k0, $12
	eret
	.set	macro
	.set	reorder
	.end	vector_\_XX\()_ISR_wrapper

        .endm

# See p32mx440f256h.h etc. for vector assignments for specific processor.
# Create wrappers for ISRs used in a specific application.

	ISR_wrapper	8,Tmr2Interrupt		# TIMER 2
#if defined(UBW32_460) || defined(EMPEROR460) || defined(UBW32_795) || defined(EMPEROR795)
	#if (SPIx == 1)
	ISR_wrapper	23,SPIxInterrupt	# SPI 1
	#endif
#endif
	#if (SPIx == 2)
	ISR_wrapper	31,SPIxInterrupt	# SPI 2
	#endif
	ISR_wrapper	24,Serial1Interrupt	# UART 1
	ISR_wrapper	32,Serial2Interrupt	# UART 2
	ISR_wrapper	35,RTCCInterrupt	# RTCC

#if defined(UBW32_795) || defined(EMPEROR795)	
	ISR_wrapper 31,Serial3Interrupt # UART 3
	ISR_wrapper 49,Serial4Interrupt # UART 4
	ISR_wrapper 51,Serial5Interrupt # UART 5
	ISR_wrapper 50,Serial6Interrupt # UART 6
#endif
