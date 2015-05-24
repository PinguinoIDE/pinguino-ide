#ifndef USB_CDC_H_
#define USB_CDC_H_

#include <typedef.h>

//#define DEBUG_PRINT_CDC 1

/**
This file contains the definitinos for the USB cdc examples
**/

/* conventional codes for class-specific descriptors */
#define USB_DT_CS_DEVICE                0x21
#define USB_DT_CS_CONFIG                0x22
#define USB_DT_CS_STRING                0x23
#define USB_DT_CS_INTERFACE             0x24
#define USB_DT_CS_ENDPOINT              0x25

/*
 * Device and/or Interface Class codes
 * as found in bDeviceClass or bInterfaceClass
 * and defined by www.usb.org documents
 */
#define USB_CLASS_PER_INTERFACE         0       /* for DeviceClass */
#define USB_CLASS_AUDIO                 1
#define USB_CLASS_COMM                  2
#define USB_CLASS_HID                   3
#define USB_CLASS_PHYSICAL              5
#define USB_CLASS_STILL_IMAGE           6
#define USB_CLASS_PRINTER               7
#define USB_CLASS_MASS_STORAGE          8
#define USB_CLASS_HUB                   9
#define USB_CLASS_CDC_DATA              0x0a
#define USB_CLASS_CSCID                 0x0b    /* chip+ smart card */
#define USB_CLASS_CONTENT_SEC           0x0d    /* content security */
#define USB_CLASS_VIDEO                 0x0e
#define USB_CLASS_WIRELESS_CONTROLLER   0xe0
#define USB_CLASS_APP_SPEC              0xfe
#define USB_CLASS_VENDOR_SPEC           0xff

/* Communication Interface Class SubClass Codes */

#define ABSTRACT_CONTROL_MODEL      0x02

/* Communication Interface Class Control Protocol Codes */
#define V25TER                      0x01    // Common AT commands ("Hayes(TM)")

/*
 * Class-Specific descriptors ... there are a couple dozen of them
 */

#define USB_CDC_HEADER_TYPE             0x00            /* header_desc */
#define USB_CDC_CALL_MANAGEMENT_TYPE    0x01            /* call_mgmt_descriptor */
#define USB_CDC_ACM_TYPE                0x02            /* acm_descriptor */
#define USB_CDC_UNION_TYPE              0x06            /* union_desc */
#define USB_CDC_COUNTRY_TYPE            0x07
#define USB_CDC_NETWORK_TERMINAL_TYPE   0x0a            /* network_terminal_desc */
#define USB_CDC_ETHERNET_TYPE           0x0f            /* ether_desc */
#define USB_CDC_WHCM_TYPE               0x11
#define USB_CDC_MDLM_TYPE               0x12            /* mdlm_desc */
#define USB_CDC_MDLM_DETAIL_TYPE        0x13            /* mdlm_detail_desc */
#define USB_CDC_DMM_TYPE                0x14
#define USB_CDC_OBEX_TYPE               0x15

/*
 * Class-Specific Control Requests (6.2)
 *
 * section 3.6.2.1 table 4 has the ACM profile, for modems.
 * section 3.8.2 table 10 has the ethernet profile.
 *
 * Microsoft's RNDIS stack for Ethernet is a vendor-specific CDC ACM variant,
 * heavily dependent on the encapsulated (proprietary) command mechanism.
 */

#define USB_CDC_SEND_ENCAPSULATED_COMMAND       0x00
#define USB_CDC_GET_ENCAPSULATED_RESPONSE       0x01
#define USB_CDC_REQ_SET_LINE_CODING             0x20
#define USB_CDC_REQ_GET_LINE_CODING             0x21
#define USB_CDC_REQ_SET_CONTROL_LINE_STATE      0x22
#define USB_CDC_REQ_SEND_BREAK                  0x23
#define USB_CDC_SET_ETHERNET_MULTICAST_FILTERS  0x40
#define USB_CDC_SET_ETHERNET_PM_PATTERN_FILTER  0x41
#define USB_CDC_GET_ETHERNET_PM_PATTERN_FILTER  0x42
#define USB_CDC_SET_ETHERNET_PACKET_FILTER      0x43
#define USB_CDC_GET_ETHERNET_STATISTIC          0x44


/*
 * USB directions
 *
 * This bit flag is used in endpoint descriptors' bEndpointAddress field.
 * It's also one of three fields in control requests bRequestType.
 */
#define USB_DIR_OUT                     0               /* to device */
#define USB_DIR_IN                      0x80            /* to host */

/*
 * USB types, the second of three bRequestType fields
 */
#define USB_TYPE_MASK                   (0x03 << 5)
#define USB_TYPE_STANDARD               (0x00 << 5)
#define USB_TYPE_CLASS                  (0x01 << 5)
#define USB_TYPE_VENDOR                 (0x02 << 5)
#define USB_TYPE_RESERVED               (0x03 << 5)

/*
 * USB recipients, the third of three bRequestType fields
 */
#define USB_RECIP_MASK                  0x1f
#define USB_RECIP_DEVICE                0x00
#define USB_RECIP_INTERFACE             0x01
#define USB_RECIP_ENDPOINT              0x02
#define USB_RECIP_OTHER                 0x03

/* 
Functional Descriptor Structure - See CDC Specification 1.1 for details 
http://www.usb.org/developers/devclass_docs/usbcdc11.pdf
*/

/** Header Functional Descriptor 
The class-specific descriptor shall start with a header that is defined in Table 23. The bcdCDC field identifies the release of the USB Class Definitions for Communication Devices Specification (this specification) with which this interface and its descriptors comply. 
**/
typedef struct
{
    u8 bFunctionLength;		/*!< Size of this descriptor in u8s. */
    u8 bDescriptorType;		/*!< CS_INTERFACE descriptor type. */
    u8 bDescriptorSubtype;		/*!< Header functional descriptor subtype as defined in Table 25. */
    u8 bcd1;
    u8 bcd2;
    //unsigned int bcdCDC;		/*!< USB Class Definitions for Communication Devices Specification release number in binary-coded decimal. */
} USB_Header_Functional_Descriptor;

/**
The Call Management functional descriptor describes the processing of calls for the Communication Class interface. It
can only occur within the class-specific portion of an Interface descriptor.
**/
typedef struct
{
  u8 bFunctionLength;			/*!< Size of this functional descriptor, in u8s. */
  u8 bDescriptorType;			/*!< CS_INTERFACE */
  u8 bDescriptorSubtype;		/*!< Call Management functional descriptor subtype, as defined in Table 25. */
  u8 bmCapabilities;			/*!< The capabilities that this configuration supports:<ul><li>D7..D2: RESERVED (Reset to zero)</li><li> D1: 0 - Device sends/receives call management information only over the Communication Class interface.<br>1 - Device can send/receive call management information over a Data Class interface.</li><li>D0: 0 - Device does not handle call management itself. 1 - Device handles call management itself.</li></ul>The previous bits, in combination, identify which call management scenario is used. If bit D0 is reset to 0, then the value of bit D1 is ignored. In this case, bit D1 is reset to zero for future compatibility. */
  u8 bDataInterface;			/*!< Interface number of Data Class interface optionally used for call management. */
} USB_Call_Management_Functional_Descriptor;

/**
 Abstract Control Management Functional Descriptor
The Abstract Control Management functional descriptor describes the commands supported by the Communication
Class interface, as defined in Section 3.6.2, with the SubClass code of Abstract Control Model. It can only occur
within the class-specific portion of an Interface descriptor.
**/
typedef struct
{
  u8 bFunctionLength;			/*!< Size of this functional descriptor, in u8s. */
  u8 bDescriptorType;			/*!< CS_INTERFACE */
  u8 bDescriptorSubtype;		/*!< Abstract Control Management functional descriptor subtype as defined in Table 25.*/
  u8 bmCapabilities;			/*!< The capabilities that this configuration supports. (A bit value of zero means that the request is not supported.)<ul><li>D7..D4: RESERVED (Reset to zero)</li><li>D3: 1 - Device supports the notification Network_Connection.</li><li>D2: 1 - Device supports the request Send_Break</li><li>D1: 1 - Device supports the request combination of Set_Line_Coding, Set_Control_Line_State, Get_Line_Coding, and the notification Serial_State.</li><li>D0: 1 - Device supports the request combination of Set_Comm_Feature, Clear_Comm_Feature, and Get_Comm_Feature.</li></ul>The previous bits, in combination, identify which requests/notifications are supported by a Communication Class interface with the SubClass code of Abstract Control Model. */
} USB_Abstract_Control_Management_Functional_Descriptor;


/**
Union Functional Descriptor
The Union functional descriptor describes the relationship between a group of interfaces that can be considered to form
a functional unit. It can only occur within the class-specific portion of an Interface descriptor. One of the interfaces in
the group is designated as a master or controlling interface for the group, and certain class-specific messages can be
sent to this interface to act upon the group as a whole. Similarly, notifications for the entire group can be sent from this
interface but apply to the entire group of interfaces. Interfaces in this group can include Communication, Data, or any
other valid USB interface class (including, but not limited to, Audio, HID, and Monitor).
TODO: expand to more than 1 slave interfaces
**/
typedef struct 
{
  u8 bFunctionLength;			/*!< Size of this functional descriptor, in u8s. */
  u8 bDescriptorType;			/*!< CS_INTERFACE */
  u8 bMasterInterface;		/*!< The interface number of the Communication or Data Class interface, designated as the master or controlling interface for the union.*/
  u8 bSlaveInterface0;			/*!< Interface number of first slave or associated interface in the union. */
  u8 bSlaveInterface1;
} USB_Union_Functional_Descriptor;



/**
Line Coding
See http://www.usb.org/developers/devclass_docs/usbcdc11.pdf Chap. 6.2.13
**/
typedef struct
{
   u32 dwDTERate;			/*!< Data terminal rate, in bits per second. */
   u8 bCharFormat;			/*!< Stop bits<ul><li>0 - 1 Stop bit</li><li>1 - 1.5 Stop bits</li><li>2 - 2 Stop bits</li></ul> */
   u8 bParityType;			/*!< Parity<ul><li>0 - None</li><li>1 - Odd</li><li>2 - Even</li><li>3 - Mark</li><li>4 - Space</li></ul> */
   u8 bDataBits;			/*!< Data bits (5, 6, 7, 8 or 16). */
} USB_CDC_Line_Coding;

/**
ZLP zero packet length
Used to aknoledge a set_control_line request
**/
typedef struct
{
	u8 wValue0;
	u8 wValue1;
	u8 wValue2;
	u8 wValue3;
	u8 wValue4;
	u8 wValue5;
	u8 wValue6;
	u8 wValue7;
} Zero_Packet_Length;

/*
Functions for CDC classes
*/
#ifdef USB_USE_CDC

#define CDC_IN_EP_SIZE 16
#define CDC_BULK_IN_SIZE 64 
#define CDC_BULK_OUT_SIZE 64


// CDC specific buffers
extern volatile u8 CDCControlBuffer[CDC_IN_EP_SIZE];
extern volatile u8 CDCRxBuffer[CDC_BULK_OUT_SIZE];
extern volatile u8 CDCTxBuffer[CDC_BULK_IN_SIZE];

u8 CONTROL_LINE=0;

void ProcessCDCRequest(void);
void CDCInitEndpoint(void);
#endif

#endif

