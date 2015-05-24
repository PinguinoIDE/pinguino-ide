/*
 * This file contains functions, macros, definitions, variables,
 * datatypes, etc. that are required for usage with the MCHPFSUSB device
 * stack. This file should be included in projects that use the device stack.
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
 
#ifndef USBDESCRIPTORS_C
#define USBDESCRIPTORS_C

#include <typedef.h>
#include <usb/usb_device.h>

/***********************************************************************
 * Device descriptor.
 **********************************************************************/

#if defined(__USBCDC__)
 
const USB_DEVICE_DESCRIPTOR usb_device = {
    //sizeof(usb_device)
    sizeof(USB_DEVICE_DESCRIPTOR),              // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,                      // DEVICE descriptor type
    0x0200,                                     // USB Spec Release Number in BCD format
    CDC_DEVICE,                                 // Class Code CDC_DEVICE
    0x00,                                       // Subclass code, unused at this time
    0x00,                                       // Protocol code, unused at this time
    USB_EP0_BUFF_SIZE,                          // Max packet size for EP0
    0x04D8,                                     // Vendor ID
    0xFEAB,                                     // Product ID: PINGUINO USB CDC
    (BCD(CDC_MAJOR_VER)<<8)|BCD(CDC_MINOR_VER), // Version Number
    1,                                          // Manufacturer string index
    2,                                          // Product string index
    3,                                          // Device serial number string index
    CDC_CONFIG_NUM                              // Number of possible configurations
};

/*
 * Configuration #1 descriptor
 */

// Total length in chars of data returned
#define CONFIGURATION_TOTAL_LENGTH (sizeof(USB_CONFIGURATION_DESCRIPTOR) + \
                                    sizeof(USB_INTERFACE_DESCRIPTOR) + \
                                    sizeof(USB_CDC_HEADER_FN_DSC) + \
                                    sizeof(USB_CDC_ACM_FN_DSC) + \
                                    sizeof(USB_CDC_UNION_FN_DSC) + \
                                    sizeof(USB_CDC_CALL_MGT_FN_DSC) + \
                                    sizeof(USB_ENDPOINT_DESCRIPTOR) + \
                                    sizeof(USB_INTERFACE_DESCRIPTOR) + \
                                    sizeof(USB_ENDPOINT_DESCRIPTOR) + \
                                    sizeof(USB_ENDPOINT_DESCRIPTOR) )

const u8 usb_config1_descriptor[] = {

    // Configuration descriptor
    sizeof(USB_CONFIGURATION_DESCRIPTOR),       // Length of this descriptor.
    USB_DESCRIPTOR_CONFIGURATION,               // Configuration descriptor type
    CONFIGURATION_TOTAL_LENGTH, 0x00,           // Total length of data for this cfg
    CDC_INT_NUM,                                // Number of interfaces in this cfg
    1,                                          // Value of this configuration
    0,                                          // Configuration string index
    USB_CFG_DSC_SELF_PWR,                       // Attributes  (_DEFAULT | _SELF) // USB_CFG_DSC_REQUIRED
    50,                                         // Max power consumption in 2mA units

        // CDC Communication Interface #1
        // This interface requires one endpoint, the managment element.
        // It optionally can have an additional endpoint, the notification element.
        // The management element uses the default endpoint for all standard
        // and Communications Class-specific requests.
        // The notification element normally uses an interrupt endpoint.
        
        sizeof(USB_INTERFACE_DESCRIPTOR),       // Size of this descriptor in bytes // 0x09
        USB_DESCRIPTOR_INTERFACE,               // Interface descriptor type
        CDC_COMM_INTF_ID,                       // Interface number
        0x00,                                   // Alternate setting number
        0x01,                                   // Number of endpoints in this interface
        CDC_COMM_INTF,                          // Class code
        CDC_ABSTRACT_CONTROL_MODEL,             // Subclass code
        CDC_V25TER,                             // Protocol code
        0,                                      // Interface string index

            // CDC Class Specific
            sizeof(USB_CDC_HEADER_FN_DSC),      // Size of this descriptor in bytes (5)
            CDC_CS_INTERFACE,                   // bDescriptorType
            CDC_DSC_FN_HEADER,                  // bDescriptorSubtype
            0x20, 0x01,                         // bcdCDC 1.20

            // Abstract Control Management Functional Descriptor
            // The RS-232C procedure is included in CDC-ACM
            sizeof(USB_CDC_ACM_FN_DSC),         // Size of this descriptor in bytes (4)
            CDC_CS_INTERFACE,                   // bDescriptorType
            CDC_DSC_FN_ACM,                     // bDescriptorSubtype
            0x00, //(CDC_ACM_CAP_LINE_CODINGS | CDC_ACM_CAP_SEND_BREAK),// bmCapabilities: (see PSTN120.pdf Table 4)

            // Union Functional Descriptor
            sizeof(USB_CDC_UNION_FN_DSC),       // Size of this descriptor in bytes (5)
            CDC_CS_INTERFACE,                   // bDescriptorType
            CDC_DSC_FN_UNION,                   // bDescriptorSubtype
            CDC_COMM_INTF_ID,                   // bControlInterface        // 01 ?
            CDC_DATA_INTF_ID,                   // bSubordinateInterface0   // 02 ?

            // Call Management Functional Descriptor
            sizeof(USB_CDC_CALL_MGT_FN_DSC),    // Size of this descriptor in bytes (5)
            CDC_CS_INTERFACE,                   // bDescriptorType
            CDC_DSC_FN_CALL_MGT,                // bDescriptorSubtype
            CDC_ACM_CAP_COMM_FEATURES,          // bmCapabilities           // 00
            CDC_DATA_INTF_ID,                   // bDataInterface           // 02

            // Endpoint Descriptor
            // CDC uses one interrupt pipe for message
            sizeof(USB_ENDPOINT_DESCRIPTOR),    // 0x07
            USB_DESCRIPTOR_ENDPOINT,            // Endpoint descriptor
            _EP_IN + CDC_COMM_EP,               // Endpoint address         // _EP01_IN
            _INTERRUPT,                         // Attributes
            CDC_COMM_IN_EP_SIZE, 0x00,          // Size
            USB_POLLING_PERIOD,                 // Interval ms

        // CDC Data Interface #2
        // This interface requires a pair (one In and one Out)
        // of the same type, bulk or isochronous transfer pipes.

        sizeof(USB_INTERFACE_DESCRIPTOR),       // 0x09
        USB_DESCRIPTOR_INTERFACE,               // Interface descriptor type
        CDC_DATA_INTF_ID,                       // Interface number         // 0x02
        0x00,                                   // Alternate setting number
        0x02,                                   // Number of endpoints in this interface
        CDC_DATA_INTF,                          // Class code
        0x00,                                   // Subclass code
        CDC_NO_PROTOCOL,                        // Protocol code
        0x00,                                   // Interface string index

            // Endpoint Descriptor
            sizeof(USB_ENDPOINT_DESCRIPTOR),    // 0x07
            USB_DESCRIPTOR_ENDPOINT,            // Endpoint descriptor
            _EP_OUT + CDC_DATA_EP,              // Endpoint address         // _EP02_OUT
            _BULK,                              // Attributes
            CDC_DATA_OUT_EP_SIZE, 0x00,         // Size
            0,//USB_POLLING_PERIOD,                 // Interval ms

            // Endpoint Descriptor
            sizeof(USB_ENDPOINT_DESCRIPTOR),    // 0x07
            USB_DESCRIPTOR_ENDPOINT,            // Endpoint descriptor
            _EP_IN + CDC_DATA_EP,               // Endpoint address         // _EP02_IN
            _BULK,                              // Attributes
            CDC_DATA_IN_EP_SIZE, 0x00,          // Size
            0//USB_POLLING_PERIOD                  // Interval ms
};

#endif /*__USBCDC__*/

/***********************************************************************
 * USB Strings
 **********************************************************************/

#if defined(USB_NUM_STRING_DESCRIPTORS)

static const USB_STRING_INIT(1) string0_descriptor = {
    sizeof(string0_descriptor),
    USB_DESCRIPTOR_STRING,              /* Language code */
    { 0x0409 }
};

static const USB_STRING_INIT(9) string1_descriptor = {
    sizeof(string1_descriptor),
    USB_DESCRIPTOR_STRING,              /* Manufacturer */
    {'S','e','a','I','c','e','L','a','b'}
};

static const USB_STRING_INIT(26) string2_descriptor = {
    sizeof(string2_descriptor),
    USB_DESCRIPTOR_STRING,              /* Product */
    { 'P','i','n','g','u','i','n','o',' ',
      '(','w','w','w','.','p','i','n','g','u','i','n','o','.','c','c',')' }
};

static const USB_STRING_INIT(10) string3_descriptor = {
    sizeof(string3_descriptor),
    USB_DESCRIPTOR_STRING,              /* Serial Number */
    {'R','.','B','l','a','n','c','h','o','t'}
};

// Array of string descriptors
const u8 *const usb_string[USB_NUM_STRING_DESCRIPTORS] = {
    (const u8 *const) &string0_descriptor,
    (const u8 *const) &string1_descriptor,
    (const u8 *const) &string2_descriptor,
    (const u8 *const) &string3_descriptor
};

#endif

// Array of configuration descriptors
const u8 *const usb_config[] = {
    (const u8 *const) &usb_config1_descriptor,
};

#endif	/* USBDESCRIPTORS_C */
