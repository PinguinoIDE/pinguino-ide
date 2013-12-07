/**
  All UART functions should go here
**/

#include <string.h>
#include <stdio.h>

#include "usb_uart.h"
#include "picUSB.h"
#include "usb_config.h"

  // Put USB I/O buffers into dual port RAM.
#pragma udata usbram5 UARTRxBuffer UARTTxBuffer
  // UART specific buffers
#ifdef USB_USE_CDC
  // for later use
volatile byte UARTControlBuffer[UART_IN_EP_SIZE];
USB_UART_Line_Coding line_config;       // for later use
#endif

#ifdef USB_USE_UART
volatile byte UARTRxBuffer[UART_BULK_OUT_SIZE];
volatile byte UARTTxBuffer[UART_BULK_IN_SIZE];
#endif

#ifdef USB_USE_CDC
  /**
  Special Setup function for UART Class to handle Setup requests.
  See Chap. 3.6.2.1 of USB UART 1.1 Doc
  **/
void ProcessCDCRequest(void) {
  /*
  If its not a Class request return
  TODO: Check if the folowing check can be done in picUSB.c -> SetupStage like the check for the Interface
  */
  if ((SetupPacket.bmRequestType & USB_TYPE_MASK) != USB_TYPE_CLASS) return;
#ifdef DEBUG_UART
  printf("ProcessUARTRequest\r\n");
#endif
}
#endif

#ifdef USB_USE_UART
  /**
  Initialize
  **/
void UARTInitEndpoint(void) {
#ifdef DEBUG_UART
  printf("UARTInitEndpoint\r\n");
#endif

  // UART Data EP is IN and OUT EP
  UART_DATA_EP_UEP = EP_OUT_IN | HSHK_EN;

  // now build EP
  EP_OUT_BD(UART_DATA_EP_NUM).Cnt = sizeof(UARTRxBuffer);
  EP_OUT_BD(UART_DATA_EP_NUM).ADDR = PTR16(&UARTRxBuffer);
  // USB owns buffer
  EP_OUT_BD(UART_DATA_EP_NUM).Stat.uc = BDS_UOWN | BDS_DTSEN;

  EP_IN_BD(UART_DATA_EP_NUM).ADDR = PTR16(&UARTTxBuffer);
  // CPU owns buffer
  EP_IN_BD(UART_DATA_EP_NUM).Stat.uc = BDS_DTS ;
}


  /**
  Function to read a string from USB
  @param buffer Buffer for reading data
  @param lenght Number of bytes to be read
  @return number of bytes acutally read
  **/
byte UARTgets(char *buffer, byte length) {
  byte i=0;
  if (deviceState != CONFIGURED) return 0;
  // Only Process if we own the buffer aka not own by SIE
  if (!EP_OUT_BD(UART_DATA_EP_NUM).Stat.UOWN) {
#ifdef DEBUG_PRINT
    printf("Rx on EP %d, Size %d\r\n", UART_DATA_EP_NUM, EP_OUT_BD(UART_DATA_EP_NUM).Cnt);
#endif
  // check how much bytes came
    if (length > EP_OUT_BD(UART_DATA_EP_NUM).Cnt) length = EP_OUT_BD(UART_DATA_EP_NUM).Cnt;
    for (i=0; i < EP_OUT_BD(UART_DATA_EP_NUM).Cnt; i++) {
      buffer[i] = UARTRxBuffer[i];
#ifdef DEBUG_PRINT
      printf("%c",UARTRxBuffer[i]);
#endif
    }
#ifdef DEBUG_PRINT
    printf("->");
#endif
  // clear BDT Stat bits beside DTS and then togle DTS
    EP_OUT_BD(UART_DATA_EP_NUM).Stat.uc &= 0x40;
    EP_OUT_BD(UART_DATA_EP_NUM).Stat.DTS = !EP_OUT_BD(UART_DATA_EP_NUM).Stat.DTS;
  // reset buffer count and handle controll of buffer to USB
    EP_OUT_BD(UART_DATA_EP_NUM).Cnt = sizeof(UARTRxBuffer);
    EP_OUT_BD(UART_DATA_EP_NUM).Stat.uc |= BDS_UOWN | BDS_DTSEN;
  }
  // return number of bytes read
  return i;
}


  /**
  Function writes string to USB, atm not more than MAX_SIZE is allowed if more is needed tranfser must be split up
  **/
byte UARTputs(char *buffer, byte length) {
  byte i=0;
  if (deviceState != CONFIGURED) return 0;
  if (!EP_IN_BD(UART_DATA_EP_NUM).Stat.UOWN) {
  //printf("IN: 0x%x ", &EP_IN_BD(0));
  //printf("OUT: 0x%x \r\n", &EP_OUT_BD(0));
  //printf("IN: 0x%x ", &EP_IN_BD(1));
  //printf("OUT: 0x%x \r\n", &EP_OUT_BD(1));
  //printf("IN: 0x%x ", &EP_IN_BD(2));
  //printf("OUT: 0x%x \r\n", &EP_OUT_BD(2));
    if (length > UART_BULK_IN_SIZE) length = UART_BULK_IN_SIZE;
    for (i=0; i < length; i++) {
      UARTTxBuffer[i] = buffer[i];
#ifdef DEBUG_PRINT
      printf("%c",UARTTxBuffer[i]);
#endif
    }
#ifdef DEBUG_PRINT
    printf("->");
#endif
//    printf("Stat 0x%x -> ", EP_IN_BD(UART_DATA_EP_NUM).Stat.uc);
  // Set counter to num bytes ready for send
    EP_IN_BD(UART_DATA_EP_NUM).Cnt = i;
  // clear BDT Stat bits beside DTS and then togle DTS
    EP_IN_BD(UART_DATA_EP_NUM).Stat.uc &= 0x40;
    EP_IN_BD(UART_DATA_EP_NUM).Stat.DTS = !EP_IN_BD(UART_DATA_EP_NUM).Stat.DTS;
  // reset Buffer to original state
  //BDS_UOWN | BDS_DTSEN | BDS_DTS;
    EP_IN_BD(UART_DATA_EP_NUM).Stat.uc |= BDS_UOWN | BDS_DTSEN;
//    printf("Stat 0x%x\r\n", EP_IN_BD(UART_DATA_EP_NUM).Stat.uc);
  }
  return i;
}
#endif
