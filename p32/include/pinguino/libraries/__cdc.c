// cdc module for Pinguino32X
// Jean-Pierre Mandon 2010
// Based on the Microchip USB stack
// printf, println, print, write, getKey, getString - Régis Blanchot 2011

// 25 Feb. 2012 added support for 32MX220F032 jp.mandon
// 03 Mar. 2012 fixed a bug in WINDOWS CDC jp.mandon
// 18 Jun. 2013 added CDC.USBIsConnected to check if USB cable is connected Moreno manzini
// 13 Mar. 2014 added printNumber, printFloat (r. blanchot)
//              updated print, println, getKey and getString to spare memory on small memory PIC
// 28 Aug. 2014 added #include <string.h> to use strlen

#ifndef __USBCDC
#define __USBCDC

#include <stdarg.h>

#if defined(CDCPRINT) || defined(CDCPRINTLN)
#include <string.h>			// strlen
#endif

#include <typedef.h>
#include <system.c>
#include <interrupt.c>
#include <delay.c>

#if defined(CDCPRINTF)
#include <printf.c>
#endif

// usb_device.c
extern void USBDeviceInit();
extern void USBTask();
extern void USBDeviceAttach();

// usb_function_cdc.c
extern void USBCheckCDCRequest();
extern void CDCInitEP();
extern void putUSBUSART(char*, char);
extern char getsUSBUSART(char*, char);
extern void CDCTxService();

extern unsigned char cdc_trf_state;
extern u8 USBDeviceState;

#define USBCDCisConfigured()    (USBDeviceState == 0x20)
#define mUSBUSARTIsTxTrfReady() (cdc_trf_state==0)
#define USBGetDeviceState()     (USBDeviceState)
/*
typedef enum
{
    DETACHED_STATE     = 0x00,
    ATTACHED_STATE     = 0x01,
    POWERED_STATE      = 0x02,
    DEFAULT_STATE      = 0x04,
    ADR_PENDING_STATE  = 0x08,
    ADDRESS_STATE      = 0x10,
    CONFIGURED_STATE   = 0x20
} USB_DEVICE_STATE;

extern USB_VOLATILE USB_DEVICE_STATE USBDeviceState;
*/

typedef enum
{
    EVENT_NONE = 0,
    EVENT_DEVICE_STACK_BASE = 1,
    EVENT_HOST_STACK_BASE = 100,
    EVENT_HUB_ATTACH,           
    EVENT_STALL,                  
    EVENT_VBUS_SES_REQUEST,     
    EVENT_VBUS_OVERCURRENT,     
    EVENT_VBUS_REQUEST_POWER,   
    EVENT_VBUS_RELEASE_POWER,   
    EVENT_VBUS_POWER_AVAILABLE, 
    EVENT_UNSUPPORTED_DEVICE,   
    EVENT_CANNOT_ENUMERATE,     
    EVENT_CLIENT_INIT_ERROR,    
    EVENT_OUT_OF_MEMORY,        
    EVENT_UNSPECIFIED_ERROR,     
    EVENT_DETACH, 
    EVENT_TRANSFER,
    EVENT_SOF,                  
    EVENT_RESUME,
    EVENT_SUSPEND,
    EVENT_RESET,  
    EVENT_DATA_ISOC_READ,
    EVENT_DATA_ISOC_WRITE,
    EVENT_GENERIC_BASE  = 400,
    EVENT_MSD_BASE      = 500,
    EVENT_HID_BASE      = 600,
    EVENT_PRINTER_BASE  = 700,
    EVENT_CDC_BASE      = 800,
    EVENT_CHARGER_BASE  = 900,
    EVENT_AUDIO_BASE    = 1000,
    EVENT_USER_BASE     = 10000,
    EVENT_BUS_ERROR     = 2^31-1
} USB_EVENT;

typedef enum
{
    EVENT_CONFIGURED = EVENT_DEVICE_STACK_BASE,
    EVENT_SET_DESCRIPTOR,
    EVENT_EP0_REQUEST,
    EVENT_ATTACH,
    EVENT_TRANSFER_TERMINATED
} USB_DEVICE_STACK_EVENTS;

// CDC buffer length
#ifndef _CDCBUFFERLENGTH_
#define _CDCBUFFERLENGTH_ 64
#endif

u8 _cdc_buffer[_CDCBUFFERLENGTH_];  // usb buffer

// this function is here to insure compatibility between USB microchip
// stack and Pinguino interrupt code
 
void INTEnableSystemMultiVectoredInt()
{
    IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);
}

// this function is here to insure compatibility between USB microchip
// stack and Pinguino interrupt code

void INTEnableInterrupts()
{
    IntEnable(INT_USB);
}

#if !defined(__32MX220F032D__) && !defined(__32MX220F032B__) && \
    !defined(__32MX250F128B__) && !defined(__32MX270F256B__)

// this is the Set Line coding CallBack function
void mySetLineCodingHandler()
{
/*
    //If the request is not in a valid range
    if(cdc_notice.GetLineCoding.dwDTERate.Val > 115200)
    {
        //NOTE: There are two ways that an unsupported baud rate could be
        //handled.  The first is just to ignore the request and don't change
        //the values.  That is what is currently implemented in this function.
        //The second possible method is to stall the STATUS stage of the request.
        //STALLing the STATUS stage will cause an exception to be thrown in the 
        //requesting application.  Some programs, like HyperTerminal, handle the
        //exception properly and give a pop-up box indicating that the request
        //settings are not valid.  Any application that does not handle the
        //exception correctly will likely crash when this requiest fails.  For
        //the sake of example the code required to STALL the status stage of the
        //request is provided below.  It has been left out so that this demo
        //does not cause applications without the required exception handling
        //to crash.
        //---------------------------------------
        //USBStallEndpoint(0,1);
    }
    else
    {
        u32 dwBaud;

        //Update the baudrate info in the CDC driver
        CDCSetBaudRate(cdc_notice.GetLineCoding.dwDTERate.Val);

        //Update the baudrate of the UART
        U2BRG = ((GetPeripheralClock()+(BRG_DIV2/2*line_coding.dwDTERate.Val))/BRG_DIV2/line_coding.dwDTERate.Val-1);
        //U2MODE = 0;
        U2MODEbits.BRGH = BRGH2;
        //U2STA = 0;
    }
*/
}

// this is the USB Event CallBack function
void USER_USB_CALLBACK_EVENT_HANDLER(USB_EVENT event_usb)
{
    switch(event_usb)
    {
        case EVENT_EP0_REQUEST:
            USBCheckCDCRequest();
            break;
            
        case EVENT_CONFIGURED: 
            CDCInitEP();
            break;
    }
}

#endif /* not 32MX2xx Family */

/***********************************************************************
 * USB CDC init routine
 * called from main32.c
 **********************************************************************/

void CDC_init()
{
    USBDeviceInit();		// Initializes USB module SFRs and firmware

    #if !defined(__32MX220F032D__) && !defined(__32MX220F032B__) && \
        !defined(__32MX250F128B__) && !defined(__32MX270F256B__)

        USBDeviceAttach();

    #endif

    Delayms(1500);
}

/***********************************************************************
 * USB CDC write routine (CDC.write)
 * modified by régis blanchot 18-05-2011 
 * write a string on CDC port
 **********************************************************************/

void CDC_puts(u8 *buffer, u8 length)
{
    u16 i;

    for (i = 1000; i > 0; --i)
    {
        if (mUSBUSARTIsTxTrfReady())
            break;
            
        #if defined(__32MX220F032D__) || defined(__32MX220F032B__) || \
            defined(__32MX250F128B__) || defined(__32MX270F256B__)
            
            USB_Service();

        #else

            CDCTxService();

        #endif
    }
    
    if (i > 0)
    {
        putUSBUSART(buffer, length);
        
        #if defined(__32MX220F032D__) || defined(__32MX220F032B__) || \
            defined(__32MX250F128B__) || defined(__32MX270F256B__)

            USB_Service();

        #else

            CDCTxService();

        #endif
    }
}
    
/***********************************************************************
 * USB CDC read routine (CDC.read)
 * modified by régis blanchot 18-05-2011 
 * get a string from CDC port
 **********************************************************************/

u8 CDC_gets(u8 *buffer)
{
    u8 numBytesRead;
        
    #if defined(__32MX220F032D__) || defined(__32MX220F032B__) || \
        defined(__32MX250F128B__) || defined(__32MX270F256B__)

        USB_Service();
        numBytesRead = USB_Service_CDC_GetString( buffer );

    #else

        CDCTxService();
        numBytesRead = getsUSBUSART(buffer, 64);

    #endif

    return numBytesRead;
/*
    if (mUSBUSARTIsTxTrfReady())
    {
        CDCTxService();
        numBytesRead = getsUSBUSART(buffer, 64);
        CDCTxService();
        return numBytesRead;
    }
*/
}

/***********************************************************************
 * USB CDC print routine (CDC.print)
 * added by regis blanchot 04/03/2014
 * write a string on CDC port
 **********************************************************************/

#if defined(CDCPRINT) || defined(CDCPRINTLN)
void CDC_print(char *string)
{
    CDC_puts(string, strlen(string));
}
#endif

/*
void CDC_print(const u8 *fmt, ...)
{
    u8 s;
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list
    s = (u8) va_arg(args, u32);				// get the first variable arg.
    
    //switch (*args)
    switch (s)
    {
        case FLOAT:
            CDC_printf("%f", (u32)fmt);
            break;
        case DEC:
            CDC_printf("%d", (u32)fmt);
            break;
        case HEX:
            CDC_printf("%x", (u32)fmt);
            break;
        case BYTE:
            //CDC_printf("%d", (u8)fmt);
            CDC_printf("%d", (u32)fmt);
            break;
        case OCT:
            CDC_printf("%o", (u32)fmt);
            break;
        case BIN:
            CDC_printf("%b", (u32)fmt);
            break;           
        default:
            CDC_printf(fmt);
            break;
    }
    va_end(args);
}
*/

/***********************************************************************
 * USB CDC print routine (CDC.println)
 * added by regis blanchot 04/03/2014
 * write a string followed by a carriage return character (ASCII 13, or '\r')
 * and a newline character (ASCII 10, or '\n') on CDC port
 **********************************************************************/

#if defined(CDCPRINTLN)
void CDC_println(char *string)
{
    CDC_print(string);
    CDC_print("\n\r");
}
#endif

/*
void CDC_println(const u8 *fmt, ...)
{
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list

    CDC_printf(fmt, args);
    CDC_printf("\n\r");
    //CDC_printf(fmt);
    //CDC_printf("\n\r");
}
*/

/***********************************************************************
 * USB CDC printNumber routine (CDC.printNumber)
 * added by regis blanchot 14/06/2011
 * write a number on CDC port
 * base : see const.h (DEC, BIN, HEXA, OCTO, ...)
 **********************************************************************/

#if defined(CDCPRINTNUMBER) || defined(CDCPRINTFLOAT)
void CDC_printNumber(long value, u8 base)
{  
    u8 sign;
    u8 length;

    long i;
    unsigned long v;    // absolute value

    u8 tmp[12];
    u8 *tp = tmp;       // pointer on tmp

    u8 string[12];
    u8 *sp = string;    // pointer on string

    if (value==0)
    {
        CDC_puts("0", 1);
        return;
    }
    
    sign = ( (base == 10) && (value < 0) );

    if (sign)
        v = -value;
    else
        v = (unsigned long)value;

    //while (v || tp == tmp)
    while (v)
    {
        i = v % base;
        v = v / base;
        
        if (i < 10)
            *tp++ = i + '0';
        else
            *tp++ = i + 'A' - 10;
    }

    // start of string
    if (sign)
        *sp++ = '-';

    length = sign + tp - tmp + 1;

    // backwards writing 
    while (tp > tmp)
        *sp++ = *--tp;

    // end of string
    *sp = 0;

    CDC_puts(string, length);
}
#endif

/***********************************************************************
 * USB CDC printFloat routine (CDC.printFloat)
 * added by regis blanchot 14/06/2011
 * write a float number on CDC port
 * base : see const.h (DEC, BIN, HEXA, OCTO, ...)
 **********************************************************************/

#if defined(CDCPRINTFLOAT)
void CDC_printFloat(float number, u8 digits)
{ 
    u8 i, toPrint;
    u16 int_part;
    float rounding, remainder;

    // Handle negative numbers
    if (number < 0.0)
    {
        CDC_puts('-', 1);
        number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"  
    rounding = 0.5;
    for (i=0; i<digits; ++i)
        rounding /= 10.0;

    number += rounding;

    // Extract the integer part of the number and print it  
    int_part = (u16)number;
    remainder = number - (float)int_part;
    CDC_printNumber(int_part, 10);

    // Print the decimal point, but only if there are digits beyond
    if (digits > 0)
        CDC_puts('.', 1); 

    // Extract digits from the remainder one at a time
    while (digits-- > 0)
    {
        remainder *= 10.0;
        toPrint = (unsigned int)remainder; //Integer part without use of math.h lib, I think better! (Fazzi)
        CDC_printNumber(toPrint, 10);
        remainder -= toPrint; 
    }
}
#endif

/***********************************************************************
 * USB CDC printf routine (CDC.printf)
 * added by regis blanchot 14/06/2011
 * write a formated string on CDC port
 **********************************************************************/

/*
#if defined(CDCPRINTF)
void CDC_printf(const u8 *fmt, ...)
{
    u8 length;
    va_list	args;

    va_start(args, fmt);
    length = psprintf2(_cdc_buffer, fmt, args);
    CDC_puts(_cdc_buffer, length);
    va_end(args);
}
#endif
*/

#if defined(CDCPRINTF)
void CDC_printf(const u8 *fmt, ...)
{
    u8 buffer[_CDCBUFFERLENGTH_];
    u8 length;
    va_list	args;

    va_start(args, fmt);
    length = psprintf2(buffer, fmt, args);
    CDC_puts(buffer,length);
    va_end(args);
}
#endif

/***********************************************************************
 * USB CDC getKey routine (CDC.getKey)
 * added by regis blanchot 14/06/2011
 * wait and return a char from CDC port
 **********************************************************************/

#if defined(CDCGETKEY) || defined(CDCGETSTRING)
u8 CDC_getkey(void)
{
    u8 buffer[_CDCBUFFERLENGTH_];		// always get a full packet

    while (!CDC_gets(buffer));
    return (buffer[0]);	// return only the first character
}
#endif

/*
char CDC_getkey()
{
    u8 buffer[64];		// always get a full packet

    while (!CDC_gets(buffer));
    return (buffer[0]);	// return only the first character
}
*/

/***********************************************************************
 * USB CDC getString routine (CDC.getString)
 * added by regis blanchot 14/06/2011
 * wait and return a string from CDC port
 **********************************************************************/

#if defined(CDCGETSTRING)
u8 * CDC_getstring(void)
{
    u8 c, i = 0;
    static u8 buffer[_CDCBUFFERLENGTH_];	// Needs static buffer at least.
    
    do {
        c = CDC_getkey();
        buffer[i++] = c;
        CDC_puts(&buffer[i-1], 1);
    } while (c != '\r');
    buffer[i] = '\0';
    return buffer;
}
#endif

/*
char * CDC_getstring(void)
{
    u8 c, i = 0;
    static u8 buffer[80];
    
    do {
        c = CDC_getkey();
        CDC_printf("%c", c);
        buffer[i++] = c;
    } while (c != '\r');
    buffer[i] = '\0';
    return buffer;
}
*/

/***********************************************************************
 * USB CDC IsConnected (CDC.USBIsConnected)
 * added by Moreno Manzini 18 Jun. 2013 
 * check if USB cable is connected
 **********************************************************************/

BOOL CDCUSBNotConnected = false;

BOOL CDC_USBIsConnected(void)
{
    if ( (U1OTGSTATbits.VBUSVD != 0) && (U1OTGSTATbits.SESVD != 0) )
    {
        if (CDCUSBNotConnected == true)
        {
            CDC_init();
            CDCUSBNotConnected = false;
        }
        return(true);
    }
    else
    {
        CDCUSBNotConnected = true;
        return (false);
    }
}

// from MLA
/*
typedef union _CONTROL_SIGNAL_BITMAP
{
    BYTE _byte;
    struct
    {
        unsigned DTE_PRESENT:1;
        unsigned CARRIER_CONTROL:1;
    };
} CONTROL_SIGNAL_BITMAP;
CONTROL_SIGNAL_BITMAP control_signal_bitmap;
*/

extern u8 control_signal_bitmap;    // in libusb.a

#define CDCClearDTR() (control_signal_bitmap &= 0xfe)

BOOL CDCDTRIsReady(void)
{
    if (CDC_USBIsConnected() == true)
    {
        //if(control_signal_bitmap.DTE_PRESENT == 1)
        if((control_signal_bitmap & 0x01) == 0x01)
            return (true);
        else
            return (false);
    }
    else
    {
        //control_signal_bitmap &= 0xfe;       // !0x01
        CDCClearDTR();
        return (false);
    }
}

#endif	/* __USBCDC */
