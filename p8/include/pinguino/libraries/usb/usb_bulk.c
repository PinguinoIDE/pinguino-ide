  /**
  All BULK functions should go here
  **/

#include <string.h>
#include "picUSB.h"
#include "usb_config.h"

#ifdef USB_USE_BULK

// Put USB I/O buffers into dual port RAM.
#pragma udata usbram5 BULKRxBuffer BULKTxBuffer
volatile byte BULKRxBuffer[BULK_BULK_OUT_SIZE];
volatile byte BULKTxBuffer[BULK_BULK_IN_SIZE];

/**
Initialize
**/
void BULKInitEndpoint(void)
{

    // BULK Data EP is IN and OUT EP
    BULK_DATA_EP_UEP = EP_OUT_IN | HSHK_EN;

    // now build EP
    EP_OUT_BD(BULK_DATA_EP_NUM).Cnt = sizeof(BULKRxBuffer);
    EP_OUT_BD(BULK_DATA_EP_NUM).ADDR = PTR16(&BULKRxBuffer);
    // USB owns buffer
    EP_OUT_BD(BULK_DATA_EP_NUM).Stat.uc = BDS_UOWN | BDS_DTSEN;

    EP_IN_BD(BULK_DATA_EP_NUM).ADDR = PTR16(&BULKTxBuffer);
    // CPU owns buffer
    EP_IN_BD(BULK_DATA_EP_NUM).Stat.uc = BDS_DTS ;
}

byte BULKavailable()
{
    u8 received = (!EP_OUT_BD(BULK_DATA_EP_NUM).Stat.UOWN) && (EP_OUT_BD(BULK_DATA_EP_NUM).Cnt > 0);
    return(received );
}
  /**
  Function to read a string from USB
  @param buffer Buffer for reading data
  @param lenght Number of bytes to be read
  @return number of bytes acutally read
  **/
byte BULKgets(char *buffer)
{
    byte i=0;
    byte length=64;

    if (deviceState != CONFIGURED)
        return 0;

    // Only Process if we own the buffer aka not own by SIE
    if (!EP_OUT_BD(BULK_DATA_EP_NUM).Stat.UOWN)
    {
        // check how much bytes came
        if (length > EP_OUT_BD(BULK_DATA_EP_NUM).Cnt)
            length = EP_OUT_BD(BULK_DATA_EP_NUM).Cnt;
        for (i=0; i < EP_OUT_BD(BULK_DATA_EP_NUM).Cnt; i++)
            buffer[i] = BULKRxBuffer[i];

        // clear BDT Stat bits beside DTS and then togle DTS
        EP_OUT_BD(BULK_DATA_EP_NUM).Stat.uc &= 0x40;
        EP_OUT_BD(BULK_DATA_EP_NUM).Stat.DTS = !EP_OUT_BD(BULK_DATA_EP_NUM).Stat.DTS;
        // reset buffer count and handle controll of buffer to USB
        EP_OUT_BD(BULK_DATA_EP_NUM).Cnt = sizeof(BULKRxBuffer);
        EP_OUT_BD(BULK_DATA_EP_NUM).Stat.uc |= BDS_UOWN | BDS_DTSEN;
    }
    // return number of bytes read
    return i;
}


  /**
  Function writes string to USB, atm not more than MAX_SIZE is allowed if more is needed transfer must be split up
  **/
byte BULKputs(char *buffer, byte length)
{
    byte i=0;

    if (deviceState != CONFIGURED) return 0;

    if (!EP_IN_BD(BULK_DATA_EP_NUM).Stat.UOWN)
    {
        if (length > BULK_BULK_IN_SIZE)
            length = BULK_BULK_IN_SIZE;
        for (i=0; i < length; i++)
            BULKTxBuffer[i] = buffer[i];

        // Set counter to num bytes ready for send
        EP_IN_BD(BULK_DATA_EP_NUM).Cnt = i;
        // clear BDT Stat bits beside DTS and then togle DTS
        EP_IN_BD(BULK_DATA_EP_NUM).Stat.uc &= 0x40;
        EP_IN_BD(BULK_DATA_EP_NUM).Stat.DTS = !EP_IN_BD(BULK_DATA_EP_NUM).Stat.DTS;
        // reset Buffer to original state
        EP_IN_BD(BULK_DATA_EP_NUM).Stat.uc |= BDS_UOWN | BDS_DTSEN;
    }
    return i;
}

#endif /* USB_USE_BULK */
