/*  --------------------------------------------------------------------
    FILE:               isrwrapper.c
    PROJECT:            pinguino 32
    PURPOSE:            weak definition of isr routines
    PROGRAMERS:         Regis Blanchot <rblanchot@gmail.com>
    FIRST RELEASE:      05 Feb. 2015
    LAST RELEASE:       05 Feb. 2015
    --------------------------------------------------------------------
    CHANGELOG:
    --------------------------------------------------------------------
    NOTE: P32MX795 share some vectors : 

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

#ifndef ISRWRAPPER_C
#define ISRWRAPPER_C

#if 0 // p32-gcc doen't emit any weak attribute ... Don't know why ...

    void __DoNothing() { /* do something */; }
    // SERIAL
    void Serial1Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Serial2Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    #if defined(__32MX795F512L__) || \
        defined(__32MX795F512H__)
    void Serial3Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Serial4Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Serial5Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Serial6Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    #endif
    // TIMER
    void Timer1Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Timer2Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Timer3Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Timer4Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void Timer5Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    // SPI
    void SPI1Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void SPI2Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    #if !defined(__32MX795F512L__) && \
        !defined(__32MX795F512H__)
    void SPI3Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void SPI4Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    #endif
    // I2C
    void I2C1Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void I2C2Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    #if !defined(__32MX795F512L__) && \
        !defined(__32MX795F512H__)
    void I2C3Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void I2C4Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void I2C5Interrupt() __attribute__ ((weak, alias ("__DoNothing")));
    #endif
    // OTHER
    void RTCCInterrupt() __attribute__ ((weak, alias ("__DoNothing")));
    void USBInterrupt() __attribute__ ((weak, alias ("__DoNothing")));

#else

    /**************************************************************************/

    #ifndef __SERIAL__

    #if defined(__32MX795F512L__) || \
        defined(__32MX795F512H__)

        // _UART_1_VECTOR = _SPI_3_VECTOR = _I2C_3_VECTOR = 24

        #if !defined(ENABLE_UART1) && (SPIx != 3) && (I2Cx != 3)
        void Serial1Interrupt(void) { Nop(); }
        #endif

        // _UART_2_VECTOR = _SPI_4_VECTOR = _I2C_5_VECTOR = 32

        #if !defined(ENABLE_UART2) && (SPIx != 4) && (I2Cx != 5)
        void Serial2Interrupt(void) { Nop(); }
        #endif

        // _UART_3_VECTOR = _SPI_2_VECTOR = _I2C_4_VECTOR = 31

        #if !defined(ENABLE_UART3) && (SPIx != 2) && (I2Cx != 4)
        void Serial3Interrupt(void) { Nop(); }
        #endif

        #ifndef ENABLE_UART4
        void Serial4Interrupt(void) { Nop(); }
        #endif

        #ifndef ENABLE_UART5
        void Serial5Interrupt(void) { Nop(); }
        #endif

        #ifndef ENABLE_UART6
        void Serial6Interrupt(void) { Nop(); }
        #endif

    #else // all other processors

        void Serial1Interrupt(void) { Nop(); }
        void Serial2Interrupt(void) { Nop(); }

    #endif

    #endif // __SERIAL__

    /**************************************************************************/

    #if !defined(TMR1INT) && !defined(__MILLIS__) && !defined(__DCF77__) // DCF77 TO MOVE
    void Timer1Interrupt(void) { Nop(); }
    #endif

    #if !defined(TMR2INT) && !defined(__SERVOS__) && !defined(__AUDIO__)
    void Timer2Interrupt(void) { Nop(); }
    #endif

    #if !defined(TMR3INT) && !defined(__IRREMOTE__) //&& !defined(__PWM__)
    void Timer3Interrupt(void) { Nop(); }
    #endif

    #if !defined(TMR4INT) && !defined(__STEPPER__)
    void Timer4Interrupt(void) { Nop(); }
    #endif

    #if !defined(TMR5INT) //&& !defined(__DCF77__) TODO
    void Timer5Interrupt(void) { Nop(); }
    #endif

    /**************************************************************************/

    #ifndef __I2C__

    #if defined(__32MX795F512L__) || \
        defined(__32MX795F512H__)

        void I2C1Interrupt(void) { Nop(); }
        void I2C2Interrupt(void) { Nop(); }

        // _UART_1_VECTOR = _SPI_3_VECTOR = _I2C_3_VECTOR = 24

        #if !defined(ENABLE_UART1) && (SPIx != 3) && (I2Cx != 3)
        void I2C3Interrupt(void) { Nop(); }
        #endif

        // _UART_3_VECTOR = _SPI_2_VECTOR = _I2C_4_VECTOR = 31

        #if !defined(ENABLE_UART3) && (SPIx != 2) && (I2Cx != 4)
        void I2C4Interrupt(void) { Nop(); }
        #endif

        // _UART_2_VECTOR = _SPI_4_VECTOR = _I2C_5_VECTOR = 32

        #if !defined(ENABLE_UART2) && (SPIx != 4) && (I2Cx != 5)
        void I2C5Interrupt(void) { Nop(); }
        #endif

    #else // all other processors

        void I2C1Interrupt(void) { Nop(); }
        void I2C2Interrupt(void) { Nop(); }

    #endif

    #endif
    
    /**************************************************************************/

    #ifndef __SPI__

    #if defined(__32MX795F512L__) || \
        defined(__32MX795F512H__)

        void SPI1Interrupt(void) { Nop(); }

        // _UART_3_VECTOR = _SPI_2_VECTOR = _I2C_4_VECTOR = 31

        #if !defined(ENABLE_UART3) && (SPIx != 2) && (I2Cx != 4)
        void SPI2Interrupt(void) { Nop(); }
        #endif

        // _UART_1_VECTOR = _SPI_3_VECTOR = _I2C_3_VECTOR = 24

        #if !defined(ENABLE_UART1) && (SPIx != 3) && (I2Cx != 3)
        void SPI3Interrupt(void) { Nop(); }
        #endif

        // _UART_2_VECTOR = _SPI_4_VECTOR = _I2C_5_VECTOR = 32

        #if !defined(ENABLE_UART2) && (SPIx != 4) && (I2Cx != 5)
        void SPI4Interrupt(void) { Nop(); }
        #endif

    #else // all other processors

        #if !defined(__32MX440F256H__)
        void SPI1Interrupt(void) { Nop(); }
        #endif
        void SPI2Interrupt(void) { Nop(); }

    #endif

    #endif // __SPI__

    /**************************************************************************/

    #ifndef __RTCC__
    void RTCCInterrupt(void) { Nop(); }
    #endif // __RTCC__

    #if !defined(__USBCDCINTERRUPT__) // !defined(__USBCDC__) || 
    void USBInterrupt(void) { Nop(); }
    #endif

#endif

#endif // ISRWRAPPER_C
