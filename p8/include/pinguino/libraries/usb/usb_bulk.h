#ifndef USB_BULK_H_
#define USB_BULK_H_

#include <typedef.h>

/**
This file contains the definitinos for the USB BULK examples
**/

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
 * Functions for BULK classes
 */
#ifdef USB_USE_BULK

    #define BULK_IN_EP_SIZE             8
    #define BULK_BULK_IN_SIZE           64
    #define BULK_BULK_OUT_SIZE          64

    // BULK specific buffers
    extern volatile byte BULKRxBuffer[BULK_BULK_OUT_SIZE];
    extern volatile byte BULKTxBuffer[BULK_BULK_IN_SIZE];

    //void ProcessBULKrequest(void);
    void BULKInitEndpoint(void);

#endif

#endif /* USB_BULK_H_ */

