/*
 * USB CDC Function Driver File
 *
 * This file contains all of functions, macros, definitions, variables,
 * datatypes, etc. that are required for usage with the CDC function
 * driver. This file should be included in projects that use the CDC
 * \function driver.  This file should also be included into the
 * usb_descriptors.c file and any other user file that requires access to the
 * CDC interface.
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the 'Company') for its PIC® Microcontroller is intended and
 * supplied to you, the Company's customer, for use solely and
 * exclusively on Microchip PIC Microcontroller products. The
 * software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN 'AS IS' CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 */

#ifndef USBFUNCTIONCDC_H
#define USBFUNCTIONCDC_H

#include <typedef.h>

/******************************
 * CDC Specific Configuration *
 ******************************/

// Configurations
#define CDC_CONFIG_NUM                  1

// Interfaces
#define CDC_INT_NUM                     2    // Comm. & Data
#define CDC_COMM_INTF_ID                0
#define CDC_DATA_INTF_ID                1

// Endpoint
#define CDC_EP_NUM                      3    // Comm. IN, Data IN, Data OUT
#define CDC_COMM_IN_EP_SIZE             8    // 8, 16, 32 or 64
#define CDC_DATA_OUT_EP_SIZE            64
#define CDC_DATA_IN_EP_SIZE             64

// EP0
#define CDC_COMM_EP                     1
#define CDC_DATA_EP                     2

/** CDC Functional Descriptors **/

/* Type Values for the bDscType Field */
#define CDC_CS_INTERFACE                0x24
#define CDC_CS_ENDPOINT                 0x25

/* bDscSubType in Functional Descriptors */
#define CDC_DSC_FN_HEADER               0x00
#define CDC_DSC_FN_CALL_MGT             0x01
#define CDC_DSC_FN_ACM                  0x02    // ACM - Abstract Control Management
#define CDC_DSC_FN_DLM                  0x03    // DLM - Direct Line Managment
#define CDC_DSC_FN_TELEPHONE_RINGER     0x04
#define CDC_DSC_FN_RPT_CAPABILITIES     0x05
#define CDC_DSC_FN_UNION                0x06
#define CDC_DSC_FN_COUNTRY_SELECTION    0x07
#define CDC_DSC_FN_TEL_OP_MODES         0x08
#define CDC_DSC_FN_USB_TERMINAL         0x09
/* more.... see Table 25 in USB CDC Specification 1.1 */

/** CDC ACM Descriptors **/

/* CDC ACM Capabilities */
#define CDC_ACM_CAP_COMM_FEATURES       0x1
#define CDC_ACM_CAP_LINE_CODINGS        0x2
#define CDC_ACM_CAP_SEND_BREAK          0x4
#define CDC_ACM_CAP_NETWORK_CONNECTION  0x8

/* CDC Requests */
#define CDC_SEND_ENCAPSULATED_COMMAND   0x00
#define CDC_GET_ENCAPSULATED_RESPONSE   0x01
#define CDC_SET_COMM_FEATURE            0x02
#define CDC_GET_COMM_FEATURE            0x03
#define CDC_CLEAR_COMM_FEATURE          0x04
#define CDC_SET_LINE_CODING             0x20
#define CDC_GET_LINE_CODING             0x21
#define CDC_SET_CONTROL_LINE_STATE      0x22
#define CDC_SEND_BREAK                  0x23

/* Notifications
 * Note: Notifications are polled over Communication Interface
 * (Interrupt Endpoint)
 */
 
#define CDC_NETWORK_CONNECTION          0x00
#define CDC_RESPONSE_AVAILABLE          0x01
#define CDC_SERIAL_STATE                0x20

/* Device Class Code */
#define CDC_DEVICE                      0x02

/* Communication Interface Class Code */
#define CDC_COMM_INTF                   0x02

/* Communication Interface Class SubClass Codes */
#define CDC_ABSTRACT_CONTROL_MODEL      0x02

/* Communication Interface Class Control Protocol Codes */
#define CDC_V25TER                      0x01    // Common AT commands ("Hayes(TM)")

/* Data Interface Class Codes */
#define CDC_DATA_INTF                   0x0A

/* Data Interface Class Protocol Codes */
#define CDC_NO_PROTOCOL                 0x00    // No class specific protocol required

/* Communication Feature Selector Codes */
#define CDC_ABSTRACT_STATE              0x01
#define CDC_COUNTRY_SETTING             0x02

/* CDC Bulk IN transfer states */
#define CDC_TX_READY                    0
#define CDC_TX_BUSY                     1
#define CDC_TX_BUSY_ZLP                 2   // ZLP: Zero Length Packet
#define CDC_TX_COMPLETING               3

#if defined(USB_CDC_SUPPORT_HARDWARE_FLOW_CONTROL)
    #define CONFIGURE_RTS(a) UART_RTS = a;
    #define CONFIGURE_DTR(a) UART_DTR = a;
#else
    #define CONFIGURE_RTS(a)
    #define CONFIGURE_DTR(a)
#endif

#define CDC_ACM_FN_DSC_VAL              0x02

#define CDC_RTS_MASK   0x02
#define CDC_DTR_MASK   0x01

/*
 * This macro is used set the baud rate reported back to the host during
 * a get line coding request. (optional)
 */

#define cdc_set_baud_rate(baudRate) { cdc_line_coding.dwDTERate.Val = baudRate; }

/*
 * This macro is used manually set the character format reported back to
 * the host during a get line coding request. (optional)
 */

#define cdc_set_character_format(charFormat) { cdc_line_coding.bCharFormat = charFormat; }
#define CDC_NUM_STOP_BITS_1     0   // 1 stop bit
#define CDC_NUM_STOP_BITS_1_5   1   // 1.5 stop bit
#define CDC_NUM_STOP_BITS_2     2   // 2 stop bit

/*
 * This function is used manually set the parity format reported back to
 * the host during a get line coding request. (optional)
 */

#define cdc_set_parity(parityType) { cdc_line_coding.bParityType = parityType; }
#define CDC_PARITY_NONE     0   // no parity
#define CDC_PARITY_ODD      1   // odd parity
#define CDC_PARITY_EVEN     2   // even parity
#define CDC_PARITY_MARK     3   // mark parity
#define CDC_PARITY_SPACE    4   // space parity

/*
 * This function is used manually set the number of data bits reported back
 * to the host during a get line coding request. (optional)
 */

#define cdc_set_data_size(dataBits) { cdc_line_coding.bDataBits = dataBits; }

/*
 * This function is used to manually set the data reported back
 * to the host during a get line coding request. (optional)
 */

#define cdc_set_line_coding(baud, format, parity, dataSize) {\
            cdc_set_baud_rate(baud);\
            cdc_set_character_format(format);\
            cdc_set_parity(parity);\
            cdc_set_data_size(dataSize);\
        }

/* Default Line Coding */
#define CDC_DEFAULT_BPS         115200              // bauds
#define CDC_DEFAULT_FORMAT      CDC_NUM_STOP_BITS_1 // 1 stop bit
#define CDC_DEFAULT_PARITY      CDC_PARITY_NONE     // No parity
#define CDC_DEFAULT_NUM_BITS    8                   // 8-bits per word

/*
 * This macro is used to check if the CDC class is ready to send more data.
 */
 
#define cdc_is_tx_ready()   (cdc_trf_state == CDC_TX_READY)

/*
 * S T R U C T U R E S
 */

/* Line Coding Structure */
#define CDC_LINE_CODING_LENGTH          0x07

typedef union _LINE_CODING
{
    struct
    {
        u8 _byte[CDC_LINE_CODING_LENGTH];
    };
    struct
    {
        u32 dwDTERate;          // Complex data structure
        u8 bCharFormat;
        u8 bParityType;
        u8 bDataBits;
    };
} LINE_CODING;

typedef union _CONTROL_SIGNAL_BITMAP
{
    u8 _byte;
    struct
    {
        unsigned DTE_PRESENT:1;       // [0] Not Present  [1] Present
        unsigned CARRIER_CONTROL:1;   // [0] Deactivate   [1] Activate
    };
} CONTROL_SIGNAL_BITMAP;


/* Functional Descriptor Structure - See CDC Specification 1.1 for details */

/* Header Functional Descriptor */
typedef struct __attribute__((packed)) _USB_CDC_HEADER_FN_DSC
{
    u8 bFNLength;
    u8 bDscType;
    u8 bDscSubType;
    u16 bcdCDC;
} USB_CDC_HEADER_FN_DSC;

/* Abstract Control Management Functional Descriptor */
typedef struct __attribute__((packed)) _USB_CDC_ACM_FN_DSC
{
    u8 bFNLength;
    u8 bDscType;
    u8 bDscSubType;
    u8 bmCapabilities;
} USB_CDC_ACM_FN_DSC;

/* Union Functional Descriptor */
typedef struct __attribute__((packed)) _USB_CDC_UNION_FN_DSC
{
    u8 bFNLength;
    u8 bDscType;
    u8 bDscSubType;
    u8 bMasterIntf;
    u8 bSaveIntf0;
} USB_CDC_UNION_FN_DSC;

/* Call Management Functional Descriptor */
typedef struct __attribute__((packed)) _USB_CDC_CALL_MGT_FN_DSC
{
    u8 bFNLength;
    u8 bDscType;
    u8 bDscSubType;
    u8 bmCapabilities;
    u8 bDataInterface;
} USB_CDC_CALL_MGT_FN_DSC;

typedef union __attribute__((packed)) _CDC_NOTICE
{
    LINE_CODING GetLineCoding;
    LINE_CODING SetLineCoding;
    u8 packet[CDC_COMM_IN_EP_SIZE];
} CDC_NOTICE, *PCDC_NOTICE;

/*
 * E X T E R N S
 */

extern u8 cdc_rx_len;
extern u8 cdc_tx_len;
extern u8 cdc_trf_state;

extern LINE_CODING cdc_line_coding;

/*
 * Public Prototypes
 */
void usb_check_cdc_request(void);
void cdc_init_endpoint(void);
//u8 cdc_consume(void (*func) (u32));
char cdc_getc();
u8 cdc_gets(char *buffer);
void cdc_putc(char c);
void cdc_puts(const char *buffer, u8 length);
void cdc_tx_service(void);

#endif //USBFUNCTIONCDC_H
