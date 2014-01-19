// cdc module for Pinguino32X
// Jean-Pierre Mandon 2010
// Based on the Microchip USB stack
// printf, println, print, write, getKey, getString - Régis Blanchot 2011

// 25 Feb. 2012 added support for 32MX220F032 jp.mandon
// 03 Mar. 2012 fixed a bug in WINDOWS CDC jp.mandon
// 18 Jun. 2013 Added CDC.USBIsConnected to check if USB cable is connected Moreno manzini


#ifndef __USBCDC
#define __USBCDC

#include <stdarg.h>

#include <typedef.h>
#include <system.c>
#include <interrupt.c>
#include <delay.c>
#include <printf.c>

extern void USBCheckCDCRequest();
extern void CDCInitEP();
extern void USBTask();
extern void USBDeviceInit();
extern void USBDeviceAttach();
extern void putUSBUSART(char*, char);
extern char getsUSBUSART(char*, char);
extern void CDCTxService();
extern unsigned char cdc_trf_state;

#define mUSBUSARTIsTxTrfReady() (cdc_trf_state==0)

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

#if defined(__32MX220F032D__)||defined(__32MX250F128B__)||defined(__32MX220F032B__)
// nothing to do
#else

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

#endif /* not __32MX220F032D__ */

// this function is called by the main32.c file
// CDC.init
void CDC_init()
{
    USBDeviceInit();		// Initializes USB module SFRs and firmware
    #if defined(__32MX220F032D__)||defined(__32MX250F128B__)||defined(__32MX220F032B__)
        // nothing to do
    #else
        USBDeviceAttach();
    #endif
    Delayms(1500);
}

// CDC.puts
// 18-05-2011 modified by régis blanchot
void CDCputs(u8 *buffer, u8 length)
{
    u16 i;
    for (i = 1000; i > 0; --i)
    {
        if (mUSBUSARTIsTxTrfReady())
            break;
        #if defined(__32MX220F032D__)||defined(__32MX250F128B__)||defined(__32MX220F032B__)
            USB_Service();
        #else
            CDCTxService();
        #endif
    }
    if (i > 0)
    {
        putUSBUSART(buffer,length);
        #if defined(__32MX220F032D__)||defined(__32MX250F128B__)||defined(__32MX220F032B__)
            USB_Service();
        #else
            CDCTxService();
        #endif
    }
}
    
// CDC.read
u8 CDCgets(u8 *buffer)
{
    u8 numBytesRead;
        
    #if defined(__32MX220F032D__)||defined(__32MX250F128B__)||defined(__32MX220F032B__)
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

// added by regis blanchot 29/05/2011
// ---------------------------------------------------------------------

// CDC.write
void CDCwrite(u8 c)
{
//	CDCputs(c, 1);
    CDCputs(&c, 1);
}

// CDC.printf
void CDCprintf(const u8 *fmt, ...)
{
    u8 buffer[80];
    //char *buffer;
    u8 length;
    va_list	args;

    va_start(args, fmt);
    //length = strlen(fmt);
    //buffer = (char *) malloc(1 + length * sizeof(char));	
    length = psprintf2(buffer, fmt, args);
    CDCputs(buffer,length);
    va_end(args);
}

// CDC.print
// last is a string (char *) or an integer

void CDCprint(const u8 *fmt, ...)
{
    u8 s;
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list
    s = (u8) va_arg(args, u32);				// get the first variable arg.
    
    //switch (*args)
    switch (s)
    {
        case FLOAT:
            CDCprintf("%f", (u32)fmt);
            break;
        case DEC:
            CDCprintf("%d", (u32)fmt);
            break;
        case HEX:
            CDCprintf("%x", (u32)fmt);
            break;
        case BYTE:
            //CDCprintf("%d", (u8)fmt);
            CDCprintf("%d", (u32)fmt);
            break;
        case OCT:
            CDCprintf("%o", (u32)fmt);
            break;
        case BIN:
            CDCprintf("%b", (u32)fmt);
            break;           
        default:
            CDCprintf(fmt);
            break;
    }
    va_end(args);
}

// CDC.println
void CDCprintln(const u8 *fmt, ...)
{
    va_list args;							// a list of arguments
    va_start(args, fmt);					// initialize the list

    CDCprintf(fmt, args);
    CDCprintf("\n\r");
    //CDCprintf(fmt);
    //CDCprintf("\n\r");
}

// CDC.getKey
char CDCgetkey()
{
    u8 buffer[64];		// always get a full packet

    while (!CDCgets(buffer));
    return (buffer[0]);	// return only the first character
}

// CDC.getString
char * CDCgetstring(void)
{
    u8 c, i = 0;
    static u8 buffer[80];
    
    do {
        c = CDCgetkey();
        CDCprintf("%c", c);
        buffer[i++] = c;
    } while (c != '\r');
    buffer[i] = '\0';
    return buffer;
}

BOOL CDCUSBNotConnected = false;

BOOL CDCUSBIsConnected(void)
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

#endif	/* __USBCDC */
