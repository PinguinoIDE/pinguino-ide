/*	----------------------------------------------------------------------------
    mips.h
    Régis Blanchot
    --------------------------------------------------------------------------*/

#ifndef __MIPS_H
#define __MIPS_H

#include <typedef.h>			// Pinguino's types definitions
#include <const.h>              // MIPS32

u32 MIPS32 DisableInterrupt(void)
{
    u32 status;
    asm volatile ("di	%0" : "=r" (status));
    return status;
}

u32 MIPS32 EnableInterrupt(void)
{
    u32 status;
    asm volatile ("ei	%0" : "=r" (status));
    return status;
}

void MIPS32 ResetCoreTimer(void)
{
    asm volatile("mtc0	%0, $9" : : "r" (0));
}

u32 MIPS32 ReadCoreTimer(void)
{
    u32 timer;
    asm volatile("mfc0	%0, $9" : "=r" (timer));
    return timer;
}

// Read C0 coprocessor register.

#define ReadCoreRegister(reg,sel)				    \
    ({ u32 __value;						            \
    asm volatile (					            	\
    "mfc0	%0, $%1, %2"					        \
    : "=r" (__value) : "K" (reg), "K" (sel));		\
    __value;						                \
})

// Write C0 coprocessor register.

#define WriteCoreRegister(reg, sel, value)			        \
do {								                        \
    asm volatile (						                    \
    "mtc0	%z0, $%1, %2 \n	ehb"                            \
    : : "r" ((u32) (value)), "K" (reg), "K" (sel));\
} while (0)

// Restore the hardware interrupt mode using the saved interrupt state.

void MIPS32 RestoreIterruptStatus (u32 x)
{
    /* C0_STATUS */
    WriteCoreRegister(12, 0, x);
}

#endif	/* __MIPS_H */

