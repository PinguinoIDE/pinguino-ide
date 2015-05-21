/*  --------------------------------------------------------------------
    FILE:               main32.c
    PROJECT:            pinguino 32
    PURPOSE:            application main function
    PROGRAMERS:         Regis Blanchot <rblanchot@gmail.com>
                        Jean-Pierre Mandon <jp.mandon@gmail.com>
    FIRST RELEASE:      16 Nov. 2010
    LAST RELEASE:       20 Mar. 2015
    --------------------------------------------------------------------
    CHANGELOG:

    22 Sep. 2011    M.Fazzi added UART3,4,5,6 support
    14 Jan. 2015    R.Blanchot added OnTimerX support
    03 Mar. 2015    R.Blanchot moved interrupt weak definitions in isrwrapper.c
    20 Mar. 2015    R.Blanchot removed SystemConfig() (defined in the bootloader)
    14 Apr. 2015    R.Blanchot added SPI init.
    --------------------------------------------------------------------
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
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
----------------------------------------------------------------------*/

#include <p32xxxx.h>    // Always in first place to avoid conflict with const.h ON
#include <typedef.h>    // Pinguino's types definitions
#include <const.h>      // Pinguino's constants definitions
#include <pin.h>        // Pinguino's pin definitions
#include <macro.h>      // Pinguino's macros definitions
#include "define.h"     // Pinguino Sketch Constants
#include <isrwrapper.c> // PIC32 interrupt weak definitions
//#include <system.c>     // PIC32 System Core Functions
#include <io.c>         // Pinguino Boards Peripheral Remappage and IOs configurations

/*
#if !defined(__32MX220F032D__) && \
    !defined(__32MX220F032B__) && \
    !defined(__32MX250F128B__) && \
    !defined(__32MX270F256B__)
    #include <newlib.c>
#endif
*/

#ifdef __USBCDC
    #include <cdc.h>
#endif

/**********************************************************************/
#include "user.c"               // Pinguino User's Sketch
/**********************************************************************/

int main()
{
    // Configure pins
    IOsetDigital();
    IOsetSpecial();
    #if defined(__SERIAL__) || defined(__SPI__) || \
        defined(__PWM__)    || defined(__AUDIO__)
    IOsetRemap();
    #endif

    // Different init.
    #ifdef __ANALOG__
    analog_init();
    #endif

    #ifdef __MILLIS__
    millis_init();
    #endif

    #ifdef __PWM__
    PWM_init();
    #endif    

    #ifdef __USBCDC
    CDC_init();
    #endif    

    //#ifdef __RTCC__
    //RTCC_init();
    //#endif    

    #ifdef __SPI__
    SPI_init();
    #endif    
    
    #ifdef __SERVOS__
    servos_init();
    #endif    

/** USER'S SKETCH *****************************************************/

    setup();

    while (1)
    {
        #ifdef __USBCDC
            #if defined(__32MX220F032D__) || \
                defined(__32MX220F032B__) || \
                defined(__32MX250F128B__) || \
                defined(__32MX270F256B__)
                USB_Service();
            #else
                CDCTxService();
            #endif
        #endif
 
        loop();
    }
/**********************************************************************/

    return(0);
    
} // end of main
