// cdc module for Pinguino
// Jean-Pierre Mandon 2010

#ifndef __USBBULK

/**********************************************************************/
#ifdef boot4
/**********************************************************************/

#define __USBBULK

#define USB_USE_BULK
//#define BULKDEBUG
#define EP1_BUFFER_SIZE 64//added 24-8-13

#include <usb/usb_bulk.h>
#include <usb/usb_config.c>
#include <usb/picUSB.c>
#include <usb/usb_bulk.c>
#include <typedef.h>
#include <delayms.c>
#include <stdio.c>                  // Pinguino printf
#include <stdarg.h>

// BULK buffer length
#ifndef _BULKBUFFERLENGTH_
#define _BULKBUFFERLENGTH_ 64
#endif

u8 _bulk_buffer[_BULKBUFFERLENGTH_];  // usb buffer

#ifdef BULKDEBUG
#include <serial.c>
#endif
void bulk_init()
{
    #ifdef BULKDEBUG
    serial_begin(9600);
    #endif

    // Init
//    INTCON=0;
    INTCONbits.GIEH = 0;
    INTCONbits.GIEL = 0;
//    INTCON2=0xC0;
    UCON=0;
    UCFG=0;
    UEP0=0;UEP1=0;UEP2=0;UEP3=0;UEP4=0;UEP5=0;
    UEP6=0;UEP7=0;UEP8=0;UEP9=0;UEP10=0;UEP11=0;
    UEP12=0;UEP13=0;UEP14=0;UEP15=0;
    // and wait 5 seconds
    Delayms(2000);

    // Initialize USB for CDC
    UCFG = 0x14; 				// Enable pullup resistors; full speed mode
    deviceState = DETACHED;
    remoteWakeup = 0x00;
    currentConfiguration = 0x00;

    // enable USB module
    while(deviceState != CONFIGURED)
    {
      EnableUSBModule();
      ProcessUSBTransactions();
    }

    // Enable Interrupt
    #if defined(__18f25k50) || defined(__18f45k50)
        PIR3bits.USBIF = 0;     // clear usb interrupt flag
        PIE3bits.USBIE = 1;     // enable usb interrupt
        IPR3bits.USBIP = 1;     // high priority interrupt
    #else
        PIR2bits.USBIF = 0;     // clear usb interrupt flag
        PIE2bits.USBIE = 1;     // enable usb interrupt
        IPR2bits.USBIP = 1;     // high priority interrupt
    #endif
    INTCONbits.GIEH = 1;   // Enable global HP interrupts
    INTCONbits.GIEL = 1;   // Enable global LP interrupts
}

void bulk_interrupt(void)
{
    #if defined(__18f25k50) || defined(__18f45k50)
    if(PIR3bits.USBIF)
    {
	    PIR3bits.USBIF = 0;
    #else
    if(PIR2bits.USBIF)
    {
	    PIR2bits.USBIF = 0;
   #endif
        ProcessUSBTransactions();
        UIRbits.SOFIF = 0;
        UIRbits.URSTIF = 0;

        UEIR = 0;
    }
}

// BULK.printf
void BULK_printf(const u8 *fmt, ...)
{
    u8 length;
    va_list	args;

    va_start(args, fmt);
    length = psprintf2(_bulk_buffer, fmt, args);
    BULKputs(_bulk_buffer,length);
    va_end(args);
}

u8 BULK_write(u8 *txpointer, u8 length)
{
    return(BULKputs(txpointer,length));
}

u8 BULK_read(u8 *rxpointer)
{
    return(BULKgets(rxpointer)); // now rxpointer buffer is filled and the buffer length is returned
}

u8 BULK_available(void)
{
    return(BULKavailable());
}

/**********************************************************************/
#elif boot2
/**********************************************************************/

#include <usb.c>

void BULK_write(u8 *txpointer, u8 length)
{
    usbsend(txpointer,length);
}

u8 BULK_read(u8 *rxpointer)
{
    return(usbreceive(rxpointer)); // one only character is returned
}

u8 BULK_available(void)
{
    return(usbavailable());
}

#endif /* boot4 and boot2 */

#endif /* __USBBULK */
