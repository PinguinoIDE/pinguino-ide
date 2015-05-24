/*  ----------------------------------------------------------------------------
    FILE:           reset.c
    PROJECT:        pinguino
    PURPOSE:        pinguino system functions
    PROGRAMER:      regis blanchot <rblanchot@gmail.com>
    FIRST RELEASE:  10 Oct. 2013
    LAST RELEASE:   10 Oct. 2013
    ----------------------------------------------------------------------------
    CHANGELOG:
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

#ifndef __RESET_C
#define __RESET_C

#include <pic18fregs.h>
#include <typedef.h>
#include <const.h>
#include <macro.h>

/**
        An MCLR reset will not affect the TO, POR and BOR bits and can be detected by
        the absence of other reset causes indicated by these bits, if they were set
        appropriately before the reset.

        A Power-On Reset will clear the POR bit and set the TO and PD bits.
        It is recommended that the POR bit be set after a Power-on Reset has been detected,
        so that subsequent Power-on Resets may be detected.

        If a brown-out reset was enabled the BOR bit will be unknown.
        Brown-out Reset is said to have occurred when BOR is ‘0’ and POR is ‘1’
        (assuming that POR was set to ‘1’ by software immediately after a Power-on Reset).

        RCON <7:0> = IPEN | - | CM | RI | TO | PD | POR | BOR
       
        IPEN = 1 : Enable priority levels on interrupts
                     0
        CM   =  1 : A Configuration Mismatch Reset has not occurred
        RI     =  1 : The RESET instruction was not executed
        TO     = 1 : Set by power-up, CLRWDT instruction or SLEEP instruction
        PD     = 0 : 1 = Set by power-up or by the CLRWDT instruction
                           0 = Set by execution of the SLEEP instruction

        POR  = 0 : A Power-on Reset occurred (must be set in software after a Power-on Reset occurs)
        BOR  = 0: A Brown-out Reset occurred (must be set in software after a Brown-out Reset occurs)

**/

/*  ----------------------------------------------------------------------------
    Checks if reset is due to power-on reset
    NOT_POR is not reset to ‘1’ by any hardware event.
    To capture multiple events, the user manually resets
    the bit to ‘1’ in software following any POR.
    --------------------------------------------------------------------------*/

char SystemIsPowerOnReset()
{
    if (!RCONbits.NOT_POR)
    {
        RCONbits.NOT_POR = 1;
        return 1;
    }
    return 0;
}

/*  ----------------------------------------------------------------------------
    Checks if reset is due to config. mismatch
    --------------------------------------------------------------------------*/

#define SystemIsConfigMismatchReset()   !RCONbits.NOT_CM

/*  ----------------------------------------------------------------------------
    Checks if reset is due to sleep instruction
    --------------------------------------------------------------------------*/

#define SystemIsSleepInstructionReset()   !RCONbits.NOT_PD

/*  ----------------------------------------------------------------------------
    Checks if reset is due to brown-out
    Brown-out Reset is said to have occurred when BOR is ‘0’ and POR is ‘1’
    (assuming that POR was set to ‘1’ by software immediately after a Power-on Reset).
    --------------------------------------------------------------------------*/

char SystemIsBrownOutReset()
{
    if (RCONbits.NOT_POR)
    {
        if (!RCONbits.NOT_BOR)
        {
            RCONbits.NOT_BOR = 1;
            return 1;
        }
    }
    return 0;
}

/*  ----------------------------------------------------------------------------
    checks if reset is due to reset instruction
    --------------------------------------------------------------------------*/

#define SystemIsResetInstructionReset()   !RCONbits.NOT_RI

/*  ----------------------------------------------------------------------------
    checks if reset is due to full stack
    --------------------------------------------------------------------------*/

#define SystemIsStackFullReset()   STKPTRbits.STKFUL

/*  ----------------------------------------------------------------------------
    checks if reset is due to underflow stack
    --------------------------------------------------------------------------*/

#define SystemIsStackUnderflowReset()   STKPTRbits.STKUNF

/*  ----------------------------------------------------------------------------
    checks if reset is due to watchdog timeout
    --------------------------------------------------------------------------*/

char SystemIsWatchdogTimeoutReset()
{
    // checks if watchdog is enabled
    if (WDTCONbits.SWDTEN)
    {
        // watchdog timeout reset
        if (!RCONbits.NOT_TO)
            return 1;
    }
    return 0;
}

/*  ----------------------------------------------------------------------------
    checks if reset is due to MCLR pin low
    --------------------------------------------------------------------------*/

char SystemIsMclrReset(void)
{
    // power-on reset ?
    if (SystemIsPowerOnReset())             return 0;
    // config. mismatch reset ?
    if (SystemIsConfigMismatchReset())      return 0;
    // sleep instruction reset ?
    if (SystemIsSleepInstructionReset())    return 0;
    // brown-out reset ?
    if (SystemIsBrownOutReset())            return 0;
    // watchdog timeout reset ?
    if (SystemIsWatchdogTimeoutReset())     return 0;
    // reset instruction ?
    if (SystemIsResetInstructionReset())    return 0;
    // stack full reset ?
    if (SystemIsStackFullReset())           return 0;
    // stack underflow reset ?
    if (SystemIsStackUnderflowReset())      return 0;
    // then it's a MCLR-pin reset
    return 1;
}

#endif /* __RESET_C */
