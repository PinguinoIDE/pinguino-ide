/*
 * USB Device header file
 *
 * This file, with its associated C source file, provides the main substance of
 * the USB device side stack.  These files will receive, transmit, and process
 * various USB commands as well as take action when required for various events
 * that occur on the bus.
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the 'Company') for its PIC® Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN 'AS IS' CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */
#ifndef USBDEVICE_H
#define USBDEVICE_H

#include <typedef.h>
#include <usb/usb_ch9.h>
#include <usb/usb_hal_pic32.h>

#if defined(__USBHID__)

    // TODO

#elif defined(__USBCDC__)

    #ifdef __USBCDCINTERRUPT__
        #define USBVOLATILE volatile
    #else
        #define USBVOLATILE
    #endif

    #include <usb/usb_function_cdc.h>

    #define USB_INT_NUM                     CDC_INT_NUM      // Nb interfaces
    #define USB_EP_NUM                      CDC_EP_NUM       // Nb endpoints
    #define USB_EP0_BUFF_SIZE               8                // CDC_COMM_IN_EP_SIZE
    #define USB_POLLING_PERIOD              1                // ms - must be > 0

#elif defined(__USBUART__)

    // TODO

#elif defined(__USBBULK__)

    // TODO

#else

    #error "No USB class drivers (HID, CDC, UART or BULK) defined"

#endif

#define USB_NUM_STRING_DESCRIPTORS          4   // 4 strings

// PIC32 supports only full ping-pong mode.
#define USB_PING_PONG_MODE USB_PING_PONG__FULL_PING_PONG

/*
 * USB Endpoint Definitions
 * USB Standard EP Address Format: DIR:X:X:X:EP3:EP2:EP1:EP0
 * This is used in the descriptors. See usbcfg.c
 *
 * NOTE: Do not use these values for checking against USTAT.
 * To check against USTAT, use values defined in usbd.h.
 */
#define _EP_IN		0x80
#define _EP_OUT		0x00
#define _EP01_OUT	0x01
#define _EP01_IN	0x81
#define _EP02_OUT	0x02
#define _EP02_IN	0x82
#define _EP03_OUT	0x03
#define _EP03_IN	0x83
#define _EP04_OUT	0x04
#define _EP04_IN	0x84
#define _EP05_OUT	0x05
#define _EP05_IN	0x85
#define _EP06_OUT	0x06
#define _EP06_IN	0x86
#define _EP07_OUT	0x07
#define _EP07_IN	0x87
#define _EP08_OUT	0x08
#define _EP08_IN	0x88
#define _EP09_OUT	0x09
#define _EP09_IN	0x89
#define _EP10_OUT	0x0A
#define _EP10_IN	0x8A
#define _EP11_OUT	0x0B
#define _EP11_IN	0x8B
#define _EP12_OUT	0x0C
#define _EP12_IN	0x8C
#define _EP13_OUT	0x0D
#define _EP13_IN	0x8D
#define _EP14_OUT	0x0E
#define _EP14_IN	0x8E
#define _EP15_OUT	0x0F
#define _EP15_IN	0x8F

/* Configuration Attributes */
#define _DEFAULT        (0x01 << 7)	// Default Value (Bit 7 is set)
#define _SELF           (0x01 << 6)	// Self-powered (Supports if set)
#define _RWU            (0x01 << 5)	// Remote Wakeup (Supports if set)
#define _HNP            (0x01 << 1)	// HNP (Supports if set)
#define _SRP            (0x01)		// SRP (Supports if set)

/* Endpoint Transfer Type */
#define _CTRL           0x00		// Control Transfer
#define _ISO            0x01		// Isochronous Transfer
#define _BULK           0x02		// Bulk Transfer
#define _INTERRUPT      0x03		// Interrupt Transfer

/* Isochronous Endpoint Synchronization Type */
#define _NS             (0x00 << 2)	// No Synchronization
#define _AS             (0x01 << 2)	// Asynchronous
#define _AD             (0x02 << 2)	// Adaptive
#define _SY             (0x03 << 2)	// Synchronous

/* Isochronous Endpoint Usage Type */
#define _DE             (0x00 << 4)	// Data endpoint
#define _FE             (0x01 << 4)	// Feedback endpoint
#define _IE             (0x02 << 4)	// Implicit feedback Data endpoint

#define _ROM            USB_INPIPES_ROM
#define _RAM            USB_INPIPES_RAM

//These are the directional indicators used for the usb_transfer_one_packet()
//  function.
#define OUT_FROM_HOST   0
#define IN_TO_HOST      1

/*
 * CTRL_TRF_SETUP: Every setup packet has 8 bytes.  This structure
 * allows direct access to the various members of the control
 * transfer.
 */
typedef union __attribute__ ((packed)) _CTRL_TRF_SETUP
{
    /* Standard Device Requests */
    struct __attribute__ ((packed))
    {
        u8 bmRequestType; //from table 9-2 of USB2.0 spec
        u8 bRequest; //from table 9-2 of USB2.0 spec
        u16 wValue; //from table 9-2 of USB2.0 spec
        u16 wIndex; //from table 9-2 of USB2.0 spec
        u16 wLength; //from table 9-2 of USB2.0 spec
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        u16 W_Value; //from table 9-2 of USB2.0 spec, allows byte/bitwise access
        u16 W_Index; //from table 9-2 of USB2.0 spec, allows byte/bitwise access
        u16 W_Length; //from table 9-2 of USB2.0 spec, allows byte/bitwise access
    };
    struct __attribute__ ((packed))
    {
        unsigned Recipient:5;   //Device,Interface,Endpoint,Other
        unsigned RequestType:2; //Standard,Class,Vendor,Reserved
        unsigned DataDir:1;     //Host-to-device,Device-to-host
        unsigned :8;
        u8 bFeature;          //DEVICE_REMOTE_WAKEUP,ENDPOINT_HALT
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        u8 bDscIndex;        //For Configuration and String DSC Only
        u8 bDescriptorType;  //Device,Configuration,String
        u16 wLangID;         //Language ID
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        u8 bDevADR;		//Device Address 0-127
        u8 bDevADRH;         //Must equal zero
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        u8 bConfigurationValue;         //Configuration Value 0-255
        u8 bCfgRSD;           //Must equal zero (Reserved)
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        u8 bAltID;            //Alternate Setting Value 0-255
        u8 bAltID_H;          //Must equal zero
        u8 bIntfID;           //Interface Number Value 0-255
        u8 bIntfID_H;         //Must equal zero
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        u8 bEPID;             //Endpoint ID (Number & Direction)
        u8 bEPID_H;           //Must equal zero
        unsigned :8;
        unsigned :8;
    };
    struct __attribute__ ((packed))
    {
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned :8;
        unsigned EPNum:4;       //Endpoint Number 0-15
        unsigned :3;
        unsigned EPDir:1;       //Endpoint Direction: 0-OUT, 1-IN
        unsigned :8;
        unsigned :8;
        unsigned :8;
    };

    /* End: Standard Device Requests */

} CTRL_TRF_SETUP;

// Defintion of the PIPE structure
// This structure is used to keep track of data that is sent out
// of the stack automatically.
typedef struct __attribute__ ((packed))
{
    union __attribute__ ((packed))
    {
        //Various options of pointers that are available to
        // get the data from
        u8 *bRam;
        const u8 *bRom;
        u16 *wRam;
        const u16 *wRom;
    } pSrc;
    
    union __attribute__ ((packed))
    {
        struct __attribute__ ((packed))
        {
            //is this transfer from RAM or ROM?
            unsigned ctrl_trf_mem          :1;
            unsigned reserved              :5;
            //include a zero length packet after
            //data is done if data_size%ep_size = 0?
            unsigned includeZero           :1;
            //is this PIPE currently in use
            unsigned busy                  :1;
        } bits;
        u8 Val;
    } info;
    
    u16 wCount;
} IN_PIPE;

#define CTRL_TRF_RETURN void
#define CTRL_TRF_PARAMS void

typedef struct __attribute__ ((packed))
{
    union __attribute__ ((packed))
    {
        //Various options of pointers that are available to
        // get the data from
        u8 *bRam;
        u16 *wRam;
    } pDst;
    
    union __attribute__ ((packed))
    {
        struct __attribute__ ((packed))
        {
            unsigned reserved              :7;
            //is this PIPE currently in use
            unsigned busy                  :1;
        } bits;
        u8 Val;
    } info;
    
    u16 wCount;
    CTRL_TRF_RETURN (*pFunc)(CTRL_TRF_PARAMS);
} OUT_PIPE;

//Various options for setting the PIPES
#define USB_INPIPES_ROM            0x00     //Data comes from RAM
#define USB_INPIPES_RAM            0x01     //Data comes from ROM
#define USB_INPIPES_BUSY           0x80     //The PIPE is busy
#define USB_INPIPES_INCLUDE_ZERO   0x40     //include a trailing zero packet
#define USB_INPIPES_NO_DATA        0x00     //no data to send
#define USB_INPIPES_NO_OPTIONS     0x00     //no options set

#define USB_EP0_ROM                USB_INPIPES_ROM
#define USB_EP0_RAM                USB_INPIPES_RAM
#define USB_EP0_BUSY               USB_INPIPES_BUSY
#define USB_EP0_INCLUDE_ZERO       USB_INPIPES_INCLUDE_ZERO
#define USB_EP0_NO_DATA            USB_INPIPES_NO_DATA
#define USB_EP0_NO_OPTIONS         USB_INPIPES_NO_OPTIONS

/*
 * Standard Request Codes
 * USB 2.0 Spec Ref Table 9-4
 */
#define GET_STATUS  0
#define CLR_FEATURE 1
#define SET_FEATURE 3
#define SET_ADR     5
#define GET_DSC     6
#define SET_DSC     7
#define GET_CFG     8
#define SET_CFG     9
#define GET_INTF    10
#define SET_INTF    11
#define SYNCH_FRAME 12

/* Section: Standard Feature Selectors */
#define DEVICE_REMOTE_WAKEUP        0x01
#define ENDPOINT_HALT               0x00

/* Section: USB Device States - To be used with [BYTE usb_device_state] */

/* Detached is the state in which the device is not attached to the bus.  When
 * in the detached state a device should not have any pull-ups attached to either
 * the D+ or D- line.  This defintions is a return value of the
 * function usb_get_device_state()
 */
#define DETACHED_STATE              0x00

/* Attached is the state in which the device is attached ot the bus but the
 * hub/port that it is attached to is not yet configured. This defintions is a
 * return value of the function usb_get_device_state()
 */
#define ATTACHED_STATE              0x01

/* Powered is the state in which the device is attached to the bus and the
 * hub/port that it is attached to is configured. This defintions is a return
 * value of the function usb_get_device_state()
 */
#define POWERED_STATE               0x02

/* Default state is the state after the device receives a RESET command from
 * the host. This defintions is a return value of the function usb_get_device_state()
 */
#define DEFAULT_STATE               0x04

/* Address pending state is not an official state of the USB defined states.
 * This state is internally used to indicate that the device has received a
 * SET_ADDRESS command but has not received the STATUS stage of the transfer yet.
 * The device is should not switch addresses until after the STATUS stage is
 * complete.  This defintions is a return value of the function
 * usb_get_device_state()
 */
#define ADR_PENDING_STATE           0x08

/* Address is the state in which the device has its own specific address on the
 * bus. This defintions is a return value of the function usb_get_device_state().
 */
#define ADDRESS_STATE               0x10

/* Configured is the state where the device has been fully enumerated and is
 * operating on the bus.  The device is now allowed to excute its application
 * specific tasks.  It is also allowed to increase its current consumption to the
 * value specified in the configuration descriptor of the current configuration.
 * This defintions is a return value of the function usb_get_device_state().
 */
#define CONFIGURED_STATE            0x20

/* UCFG Initialization Parameters */
#define SetConfigurationOptions()   {U1CNFG1 = 0;}

/* UEPn Initialization Parameters */
#define EP_CTRL                     0x0C	// Cfg Control pipe for this ep
#define EP_OUT                      0x18	// Cfg OUT only pipe for this ep
#define EP_IN                       0x14	// Cfg IN only pipe for this ep
#define EP_OUT_IN                   0x1C	// Cfg both OUT & IN pipes for this ep
#define HSHK_EN                     0x01	// Enable handshake packet
                                            // Handshake should be disable for isoch

#define USB_HANDSHAKE_ENABLED       0x01
#define USB_HANDSHAKE_DISABLED      0x00

#define USB_OUT_ENABLED             0x08
#define USB_OUT_DISABLED            0x00

#define USB_IN_ENABLED              0x04
#define USB_IN_DISABLED             0x00

#define USB_ALLOW_SETUP             0x00
#define USB_DISALLOW_SETUP          0x10

#define USB_STALL_ENDPOINT          0x02

// USB_HANDLE is a pointer to an entry in the BDT.  This pointer can be used
// to read the length of the last transfer, the status of the last transfer,
// and various other information.  Insure to initialize USB_HANDLE objects
// to NULL so that they are in a known state during their first usage.
#define USB_HANDLE volatile BDT_ENTRY*

// Definitions for the BDT
extern volatile BDT_ENTRY usb_buffer[(USB_EP_NUM + 1) * 4];

// Device descriptor (cf. usb_descriptor.c)
extern const USB_DEVICE_DESCRIPTOR usb_device;

// Array of configuration descriptors (cf. usb_descriptor.c)
extern const u8 *const usb_config[];
extern const u8 usb_config1_descriptor[];

// Array of string descriptors (cf. usb_descriptor.c)
extern const u8 *const usb_string[];

// Buffer for control transfers
extern volatile CTRL_TRF_SETUP usb_setup_pkt;           // 8-byte only

extern USBVOLATILE u8 usb_device_state;
extern USBVOLATILE u8 usb_active_configuration;
extern USBVOLATILE IN_PIPE usb_in_pipe;
extern USBVOLATILE OUT_PIPE usb_out_pipe;

/* Control Transfer States */
#define WAIT_SETUP          0
#define CTRL_TRF_TX         1
#define CTRL_TRF_RX         2

/* v2.1 fix - Short Packet States - Used by Control Transfer Read  - CTRL_TRF_TX */
#define SHORT_PKT_NOT_USED  0
#define SHORT_PKT_PENDING   1
#define SHORT_PKT_SENT      2

/* USB PID: Token Types - See chapter 8 in the USB specification */
#define SETUP_TOKEN         0x0D    // 0b00001101
#define OUT_TOKEN           0x01    // 0b00000001
#define IN_TOKEN            0x09    // 0b00001001

/* bmRequestType Definitions */
#define HOST_TO_DEV         0
#define DEV_TO_HOST         1

#define STANDARD            0x00
#define CLASS               0x01
#define VENDOR              0x02

#define RCPT_DEV            0
#define RCPT_INTF           1
#define RCPT_EP             2
#define RCPT_OTH            3

void usb_device_tasks(void);
void usb_device_init(void);
void usb_check_cable(void);
void usb_ctrl_ep_service (void);
void usb_ctrl_trf_setup_handler (void);
void usb_ctrl_trf_in_handler (void);
void usb_check_std_request (void);
void usb_std_get_dsc_handler (void);
void usb_ctrl_ep_service_complete (void);
void usb_ctrl_trf_tx_service (void);
void usb_prepare_for_next_setup_trf (void);
void usb_ctrl_trf_rx_service (void);
void usb_std_set_cfg_handler (void);
void usb_std_get_status_handler (void);
void usb_std_feature_req_handler (void);
void usb_ctrl_trf_out_handler (void);

void usb_wake_from_suspend (void);
void usb_suspend (void);
void usb_stall_handler (void);
void usb_stall_endpoint(u8 ep, u8 dir);
volatile USB_HANDLE usb_transfer_one_packet (u8 ep, u8 dir, u8* data, u8 len);
void usb_enable_endpoint (u8 ep, u8 options);
void usb_configure_endpoint (u8 ep, u8 dir);

#if defined(USB_DYNAMIC_EP_CONFIG)
    void usb_init_ep(u8 const* pConfig);
#else
    #define usb_init_ep(a)
#endif

/* Section: CALLBACKS */
//void usbcb_suspend (void);
//void usbcb_wake_from_suspend (void);
//void usbcb_send_resume (void);

/* Section: MACROS */

#define BCD(x)   ((( x / 10 ) << 4) | ( x % 10 ))
#define DESC_CONFIG_BYTE(a) (a)
#define DESC_CONFIG_WORD(a) (a&0xFF),((a>>8)&0xFF)
#define DESC_CONFIG_DWORD(a) (a&0xFF),((a>>8)&0xFF),((a>>16)&0xFF),((a>>24)&0xFF)
#define usb_handle_busy(handle) (handle != 0 && handle->STAT.UOWN)
#define usb_handle_get_length(handle) (handle->CNT)
#define usb_handle_get_addr(handle) (handle->ADR)
#define usb_ep0_set_source_ram(src) usb_in_pipe.pSrc.bRam = src
#define usb_ep0_set_source_rom(src) usb_in_pipe.pSrc.bRom = src
#define usb_ep0_transmit(options) usb_in_pipe.info.Val = options | USB_INPIPES_BUSY
#define usb_ep0_set_size(size) usb_in_pipe.wCount = size
#define usb_ep0_send_ram_ptr(src,size,options)  {usb_ep0_set_source_ram(src);usb_ep0_set_size(size);usb_ep0_transmit(options | USB_EP0_RAM);}
#define usb_ep0_send_rom_ptr(src,size,options)  {usb_ep0_set_source_rom(src);usb_ep0_set_size(size);usb_ep0_transmit(options | USB_EP0_ROM);}
#define usb_tx_one_packet(ep, data, len)    usb_transfer_one_packet(ep, IN_TO_HOST, data, len)
#define usb_rx_one_packet(ep, data, len)    usb_transfer_one_packet(ep, OUT_FROM_HOST, data, len)

#define usb_get_remote_wakeup_status() usb_remote_wakeup
#define usb_get_device_state() usb_device_state
//#define usb_is_device_suspended() (U1PWRC & _U1PWRC_USUSPEND_MASK)

#if (USB_PING_PONG_MODE == USB_PING_PONG__FULL_PING_PONG)
    #define USB_NEXT_EP0_OUT_PING_PONG 0x0008
    #define USB_NEXT_EP0_IN_PING_PONG 0x0008
    #define USB_NEXT_PING_PONG 0x0008

    #define EP0_OUT_EVEN    0
    #define EP0_OUT_ODD     1
    #define EP0_IN_EVEN     2
    #define EP0_IN_ODD      3
    #define EP1_OUT_EVEN    4
    #define EP1_OUT_ODD     5
    #define EP1_IN_EVEN     6
    #define EP1_IN_ODD      7
    #define EP2_OUT_EVEN    8
    #define EP2_OUT_ODD     9
    #define EP2_IN_EVEN     10
    #define EP2_IN_ODD      11
    #define EP3_OUT_EVEN    12
    #define EP3_OUT_ODD     13
    #define EP3_IN_EVEN     14
    #define EP3_IN_ODD      15
    #define EP4_OUT_EVEN    16
    #define EP4_OUT_ODD     17
    #define EP4_IN_EVEN     18
    #define EP4_IN_ODD      19
    #define EP5_OUT_EVEN    20
    #define EP5_OUT_ODD     21
    #define EP5_IN_EVEN     22
    #define EP5_IN_ODD      23
    #define EP6_OUT_EVEN    24
    #define EP6_OUT_ODD     25
    #define EP6_IN_EVEN     26
    #define EP6_IN_ODD      27
    #define EP7_OUT_EVEN    28
    #define EP7_OUT_ODD     29
    #define EP7_IN_EVEN     30
    #define EP7_IN_ODD      31
    #define EP8_OUT_EVEN    32
    #define EP8_OUT_ODD     33
    #define EP8_IN_EVEN     34
    #define EP8_IN_ODD      35
    #define EP9_OUT_EVEN    36
    #define EP9_OUT_ODD     37
    #define EP9_IN_EVEN     38
    #define EP9_IN_ODD      39
    #define EP10_OUT_EVEN   40
    #define EP10_OUT_ODD    41
    #define EP10_IN_EVEN    42
    #define EP10_IN_ODD     43
    #define EP11_OUT_EVEN   44
    #define EP11_OUT_ODD    45
    #define EP11_IN_EVEN    46
    #define EP11_IN_ODD     47
    #define EP12_OUT_EVEN   48
    #define EP12_OUT_ODD    49
    #define EP12_IN_EVEN    50
    #define EP12_IN_ODD     51
    #define EP13_OUT_EVEN   52
    #define EP13_OUT_ODD    53
    #define EP13_IN_EVEN    54
    #define EP13_IN_ODD     55
    #define EP14_OUT_EVEN   56
    #define EP14_OUT_ODD    57
    #define EP14_IN_EVEN    58
    #define EP14_IN_ODD     59
    #define EP15_OUT_EVEN   60
    #define EP15_OUT_ODD    61
    #define EP15_IN_EVEN    62
    #define EP15_IN_ODD     63

    #define EP(ep,dir,pp) (4*ep+2*dir+pp)

    #define BD(ep,dir,pp) (8*(4*ep+2*dir+pp))

#else
    #error "No ping pong mode defined."
#endif

//extern int usb_remote_wakeup;

#endif //USBDEVICE_H
