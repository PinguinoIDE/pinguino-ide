  // Firmware framework for USB I/O on PIC 18F2455 (and siblings)
  // Copyright (C) 2005 Alexander Enzmann
  //
  // Adapted to Pinguino by Jean-Pierre Mandon (C) 2010
  // 
  // This library is free software; you can redistribute it and/or
  // modify it under the terms of the GNU Lesser General Public
  // License as published by the Free Software Foundation; either
  // version 2.1 of the License, or (at your option) any later version.
  //
  // This library is distributed in the hope that it will be useful,
  // but WITHOUT ANY WARRANTY; without even the implied warranty of
  // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  // Lesser General Public License for more details.
  //
  // You should have received a copy of the GNU Lesser General Public
  // License along with this library; if not, write to the Free Software
  // Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
  //

  /**
  TODO:
  **/
#include <pic18fregs.h>
#include <string.h>

#include "picUSB.h"
#include "usb_config.h"

#ifdef USB_USE_CDC
#include "usb_cdc.h"
#endif
#ifdef USB_USE_BULK
#include "usb_bulk.h"
#endif

#define ALLOW_SUSPEND 0
  // It appears that you need at least 6 loops that are replaced by memcpy()
  // before it is an advantage.
#define USE_MEMCPY 0

  // Device and configuration descriptors.  These are used as the
  // host enumerates the device and discovers what class of device
  // it is and what interfaces it supports.
  // TODO: remove below lines and replace with the apropriate device_desc.blength etc.
#define DEVICE_DESCRIPTOR_SIZE 0x12
#define CONFIG_HEADER_SIZE  0x09
#define HID_DESCRIPTOR_SIZE 0x20
#define HID_HEADER_SIZE 0x09

  // Global variables
u8 deviceState;
u8 remoteWakeup;
u8 deviceAddress;
u8 selfPowered;
u8 currentConfiguration;

  // Control Transfer Stages - see USB spec chapter 5
#define SETUP_STAGE    0                // Start of a control transfer (followed by 0 or more data stages)
#define DATA_OUT_STAGE 1                // Data from host to device
#define DATA_IN_STAGE  2                // Data from device to host
#define STATUS_STAGE   3                // Unused - if data I/O went ok, then back to Setup

u8 ctrlTransferStage;                 // Holds the current stage in a control transfer

u8 HIDPostProcess;                    // Set to 1 if HID needs to process after the data stage
u8 requestHandled;                    // Set to 1 if request was understood and processed.

u8 *outPtr;                           // Data to send to the host
u8 *inPtr;                            // Data from the host
word wCount;                            // Number of bytes of data

  // HID Class variables
u8 hidIdleRate;
u8 hidProtocol;                       // [0] Boot Protocol [1] Report Protocol
u8 hidRxLen;                          // # of bytes put into buffer

  /** Buffer descriptors Table (see datasheet page 171)
  RAM Bank 4 (0x400 though 0x4ff) is used spEcifically for endpoint buffer control
  in a structure known as Buffer Descriptor Table (BDTZ).
  TODO: find something smarter to allocate the buffer, like in usbmmap.c
  of the microchip firmware. If not all endpoints are used the space in RAM is wasted.
  **/

//
//  RB 09/09/2012 : added some other PIC support
//

#if   defined(__18f14k50) || defined(__18f14k50)  // Bank 2
    volatile BufferDescriptorTable __at (0x200) ep_bdt[32];
#elif defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)  // Bank 13
    volatile BufferDescriptorTable __at (0xD00) ep_bdt[32];
#else                                             // Bank 4
    volatile BufferDescriptorTable __at (0x400) ep_bdt[32];
#endif

  // Put endpoint 0 buffers into dual port RAM
#pragma udata usbram5 SetupPacket controlTransferBuffer
//volatile BufferDescriptorTable ep_bdt[32];
volatile setupPacketStruct SetupPacket;
volatile u8 controlTransferBuffer[EP0_BUFFER_SIZE];

  //
  // Start of code to process standard requests (USB chapter 9)
  //

  // Process GET_DESCRIPTOR
static void GetDescriptor(void)
{
    #ifdef DEBUG_PRINT
    printf("GetDescriptor\r\n");
    #endif

    if(SetupPacket.bmRequestType == 0x80)
    {
        u8 descriptorType  = SetupPacket.wValue1;
        u8 descriptorIndex = SetupPacket.wValue0;

        if (descriptorType == DEVICE_DESCRIPTOR)
        {
        #ifdef DEBUG_PRINT
        printf("DEVICE_DESCRIPTOR (0x%ux)\r\n",(word)descriptorType);
        #endif
          requestHandled = 1;
          outPtr = (u8 *)&libdevice_descriptor;
          wCount = sizeof(USB_Device_Descriptor);
        }

        else if (descriptorType == CONFIGURATION_DESCRIPTOR)
        {
            #ifdef DEBUG_PRINT
            printf("CONFIGURATION_DESCRIPTOR (0x%ux): %d\r\n", (word)descriptorType, descriptorIndex);
            #endif

            requestHandled = 1;

            #if 0
            outPtr = (u8 *)&libconfiguration_descriptor;
            // wCount = *(outPtr + 2);
            wCount = CFSZ;
            #else
            // SDCC 2.5 makes bad code with the following two lines.  The
            // unusual thing is that it is the first of the two lines that
            // is compiled incorrectly (outPtr gets the contents of
            // libconfiguration_descriptor rather than the address), even though it
            // is the second line that changes from above.
            outPtr = (u8 *)&libconfiguration_descriptor;
            //wCount = libconfiguration_descriptor.configHeader[2]; // Note: SDCC makes bad code with this
            wCount = libconfiguration_descriptor.Header.wTotalLength;
            #endif

            #ifdef DEBUG_PRINT
            printf("Total config size: %d\r\n", wCount);
            #endif
        }

        else if (descriptorType == STRING_DESCRIPTOR)
        {
            #ifdef DEBUG_PRINT
            printf("STRING_DESCRIPTOR: %d\r\n", (word)descriptorIndex);
            #endif

            requestHandled = 1;
            //outPtr = (u8 *)&libstring_descriptor[descriptorIndex];
            outPtr = libstring_descriptor[descriptorIndex];
            wCount = *outPtr;
        }

        else if (descriptorType == DEVICE_QUALIFIER_DESCRIPTOR)
        {
            #ifdef DEBUG_PRINT
            printf("DEVICE_QUALIFIER_DESCRIPTOR\r\n");
            #endif

            requestHandled = 1;
            // TODO: check if this is needed if not requestHandled is not set to 1 the device will
            // stall later when the linux kernel requests this descriptor
            //outPtr = (u8 *)&libconfiguration_descriptor;
            //wCount = sizeof();
        }

        else
        {
            #ifdef DEBUG_PRINT
            printf("Unknown Descriptor: 0x%ux\r\n", (word)descriptorType);
            #endif
        }
    }
}


  // Process GET_STATUS
static void GetStatus(void) {
  // Mask off the Recipient bits
  u8 recipient = SetupPacket.bmRequestType & 0x1F;
#ifdef DEBUG_PRINT
  printf("GetStatus\r\n");
#endif
  controlTransferBuffer[0] = 0;
  controlTransferBuffer[1] = 0;

  // See where the request goes
  if (recipient == 0x00) {
  // Device
    requestHandled = 1;
  // Set bits for self powered device and remote wakeup.
    if (selfPowered)
      controlTransferBuffer[0] |= 0x01;
    if (remoteWakeup)
      controlTransferBuffer[0] |= 0x02;
  }
  else if (recipient == 0x01) {
  // Interface
    requestHandled = 1;
  }
  else if (recipient == 0x02) {
  // Endpoint
    u8 endpointNum = SetupPacket.wIndex0 & 0x0F;
    u8 endpointDir = SetupPacket.wIndex0 & 0x80;
    requestHandled = 1;
  // Endpoint descriptors are 8 bytes long, with each in and out taking 4 bytes
  // within the endpoint. (See PIC datasheet.)
    inPtr = (u8 *)&EP_OUT_BD(0) + (endpointNum * 8);
    if (endpointDir)
      inPtr += 4;
    if(*inPtr & BDS_BSTALL)
      controlTransferBuffer[0] = 0x01;
  }

  if (requestHandled) {
    outPtr = (u8 *)&controlTransferBuffer;
    wCount = 2;
  }
}


  // Process SET_FEATURE and CLEAR_FEATURE
static void SetFeature(void) {
  u8 recipient = SetupPacket.bmRequestType & 0x1F;
  u8 feature = SetupPacket.wValue0;
#ifdef DEBUG_PRINT
  // printf("SetFeature\r\n");
#endif

  if (recipient == 0x00) {
  // Device
    if (feature == DEVICE_REMOTE_WAKEUP) {
      requestHandled = 1;
      if (SetupPacket.bRequest == SET_FEATURE)
        remoteWakeup = 1;
      else
        remoteWakeup = 0;
    }
  // TBD: Handle TEST_MODE
  }
  else if (recipient == 0x02) {
  // Endpoint
    u8 endpointNum = SetupPacket.wIndex0 & 0x0F;
    u8 endpointDir = SetupPacket.wIndex0 & 0x80;
    if ((feature == ENDPOINT_HALT) && (endpointNum != 0)) {
  // Halt endpoint (as long as it isn't endpoint 0)
      requestHandled = 1;
  // Endpoint descriptors are 8 bytes long, with each in and out taking 4 bytes
  // within the endpoint. (See PIC datasheet.)
      inPtr = (u8 *)&EP_OUT_BD(0) + (endpointNum * 8);
      if (endpointDir)
        inPtr += 4;

      if(SetupPacket.bRequest == SET_FEATURE)
        *inPtr = 0x84;
      else {
        if(endpointDir == 1)
          *inPtr = 0x00;
        else
          *inPtr = 0x88;
      }
    }
  }
}


void ProcessStandardRequest(void) {
  u8 request = SetupPacket.bRequest;

  if((SetupPacket.bmRequestType & 0x60) != 0x00)
  // Not a standard request - don't process here.  Class or Vendor
  // requests have to be handled seperately.
    return;

  if (request == SET_ADDRESS) {
  // Set the address of the device.  All future requests
  // will come to that address.  Can't actually set UADDR
  // to the new address yet because the rest of the SET_ADDRESS
  // transaction uses address 0.
#ifdef DEBUG_PRINT
    printf("SET_ADDRESS: %uhx\r\n", SetupPacket.wValue0);
#endif
    requestHandled = 1;
    deviceState = ADDRESS;
    deviceAddress = SetupPacket.wValue0;
  }
  else if (request == GET_DESCRIPTOR) {
    GetDescriptor();
  }
  else if (request == SET_CONFIGURATION) {
#ifdef DEBUG_PRINT
    printf("SET_CONFIGURATION\r\n");
#endif
    requestHandled = 1;
    currentConfiguration = SetupPacket.wValue0;
  // TBD: ensure the new configuration value is one that
  // exists in the descriptor.
    if (currentConfiguration == 0)
  // If configuration value is zero, device is put in
  // address state (USB 2.0 - 9.4.7)
      deviceState = ADDRESS;
    else {
  // Set the configuration.
      deviceState = CONFIGURED;

  // Initialize the endpoints for all interfaces
#ifdef USB_USE_HID
      HIDInitEndpoint();
#endif
#ifdef USB_USE_UART
      UARTInitEndpoint();
#endif
#ifdef USB_USE_CDC
      CDCInitEndpoint();
#endif      
#ifdef USB_USE_BULK
      BULKInitEndpoint();
#endif   

  // TBD: Add initialization code here for any additional
  // interfaces beyond the one used for the HID
    }
  }
  else if (request == GET_CONFIGURATION) {
#ifdef DEBUG_PRINT
    printf("GET_CONFIGURATION\r\n");
#endif
    requestHandled = 1;
    outPtr = (u8*)&currentConfiguration;
    wCount = 1;
  }
  else if (request == GET_STATUS) {
    GetStatus();
  }
  else if ((request == CLEAR_FEATURE) ||
  (request == SET_FEATURE)) {
    SetFeature();
  }
  else if (request == GET_INTERFACE) {
  // No support for alternate interfaces.  Send
  // zero back to the host.
#ifdef DEBUG_PRINT
    printf("GET_INTERFACE\r\n");
#endif
    requestHandled = 1;
    controlTransferBuffer[0] = 0;
    outPtr = (u8 *)&controlTransferBuffer;
    wCount = 1;
  }
  else if (request == SET_INTERFACE) {
  // No support for alternate interfaces - just ignore.
#ifdef DEBUG_PRINT
    printf("SET_INTERFACE\r\n");
#endif
    requestHandled = 1;
  }
  else if (request == SET_DESCRIPTOR) {
#ifdef DEBUG_PRINT
    printf("SET_DESCRIPTOR\r\n");
#endif
  }
  else if (request == SYNCH_FRAME) {
#ifdef DEBUG_PRINT
    printf("SYNCH_FRAME\r\n");
#endif
  }
  else {
#ifdef DEBUG_PRINT
    printf("Default Std Request\r\n");
#endif
  }
}


  /**
   Data stage for a Control Transfer that sends data to the host
  **/
void InDataStage(unsigned char ep) {
#if !USE_MEMCPY
  u8 i;
#endif
  word bufferSize;

  // Determine how many bytes are going to the host
  if(wCount < EP0_BUFFER_SIZE)
    bufferSize = wCount;
  else
    bufferSize = EP0_BUFFER_SIZE;

#ifdef DEBUG_PRINT
  //    printf("USBCtrlTrfTxService: %d\r\n", bufferSize);
#endif

  // Load the high two bits of the u8 count into BC8:BC9
  // Clear BC8 and BC9
  EP_IN_BD(ep).Stat.uc &= ~(BDS_BC8 | BDS_BC9);
  EP_IN_BD(ep).Stat.uc |= (u8)((bufferSize & 0x0300) >> 8);
  EP_IN_BD(ep).Cnt = (u8)(bufferSize & 0xFF);
  EP_IN_BD(ep).ADDR = PTR16(&controlTransferBuffer);

  // Update the number of bytes that still need to be sent.  Getting
  // all the data back to the host can take multiple transactions, so
  // we need to track how far along we are.
  wCount = wCount - bufferSize;

  // Move data to the USB output buffer from wherever it sits now.
  inPtr = (u8 *)&controlTransferBuffer;

#if USE_MEMCPY
  memcpy(inPtr, outPtr, bufferSize);
#else
  for (i=0;i<bufferSize;i++) {
#ifdef DEBUG_PRINT
  //printf("0x%x = 0x%x (0x%uhx) ", PTR16(ramPtr), PTR16(romPtr), *romPtr);
#endif
    *inPtr++ = *outPtr++;
  }
#endif
}


  /**
   Data stage for a Control Transfer that reads data from the host
  **/
void OutDataStage(unsigned char ep) {
  word i, bufferSize;

  bufferSize = ((0x03 & EP_OUT_BD(ep).Stat.uc) << 8) | EP_OUT_BD(ep).Cnt;

#ifdef DEBUG_PRINT
  //    printf("OutDataStage: %d\r\n", bufferSize);
#endif

  // Accumulate total number of bytes read
  wCount = wCount + bufferSize;

  outPtr = (u8*)&controlTransferBuffer;
#if USE_MEMCPY
  memcpy(inPtr, outPtr, bufferSize);
#else
  for (i=0;i<bufferSize;i++) {
#ifdef DEBUG_PRINT
  // printf("0x%x = 0x%x (0x%uhx) ", PTR16(inPtr), PTR16(outPtr), *outPtr);
#endif
    *inPtr++ = *outPtr++;
  }
#ifdef DEBUG_PRINT
  // printf("\r\n");
#endif
#endif
}


  /**
    Process the Setup stage of a control transfer.  This code initializes the
    flags that let the firmware know what to do during subsequent stages of
    the transfer.
    TODO:
    Only Ep0 is handled here.
  **/
void SetupStage(void) {
  // Note: Microchip says to turn off the UOWN bit on the IN direction as
  // soon as possible after detecting that a SETUP has been received.
  EP_IN_BD(0).Stat.uc &= ~BDS_UOWN;
  EP_OUT_BD(0).Stat.uc &= ~BDS_UOWN;

  // Initialize the transfer process
  ctrlTransferStage = SETUP_STAGE;
  requestHandled = 0;                   // Default is that request hasn't been handled
  HIDPostProcess = 0;                   // Assume standard request until know otherwise
  wCount = 0;                           // No bytes transferred

  // See if this is a standard (as definded in USB chapter 9) request
  ProcessStandardRequest();

  // only Process CDC or HID if recipient is an interface
  // See if the HID class can do something with it.
#ifdef USB_USE_HID
  if ((SetupPacket.bmRequestType & USB_RECIP_MASK) == USB_RECIP_INTERFACE)
    ProcessHIDRequest();
#endif 
  
#ifdef USB_USE_CDC
  if ((SetupPacket.bmRequestType & USB_RECIP_MASK) == USB_RECIP_INTERFACE)  
    ProcessCDCRequest();
#endif
  // TBD: Add handlers for any other classes/interfaces in the device
  if (!requestHandled) {
  // If this service wasn't handled then stall endpoint 0
    EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
    EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
    EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
    EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
  }
  else if (SetupPacket.bmRequestType & 0x80) {
  // Device-to-host
    if(SetupPacket.wLength < wCount)
      wCount = SetupPacket.wLength;
    InDataStage(0);
    ctrlTransferStage = DATA_IN_STAGE;
  // Reset the out buffer descriptor for endpoint 0
    EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
    EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
    EP_OUT_BD(0).Stat.uc = BDS_UOWN;

  // Set the in buffer descriptor on endpoint 0 to send data
    EP_IN_BD(0).ADDR = PTR16(&controlTransferBuffer);
  // Give to SIE, DATA1 packet, enable data toggle checks
    EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
  }
  else {
  // Host-to-device
    ctrlTransferStage = DATA_OUT_STAGE;

  // Clear the input buffer descriptor
    EP_IN_BD(0).Cnt = 0;
    EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;

  // Set the out buffer descriptor on endpoint 0 to receive data
    EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
    EP_OUT_BD(0).ADDR = PTR16(&controlTransferBuffer);
  // Give to SIE, DATA1 packet, enable data toggle checks
    EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
  }

  // Enable SIE token and packet processing
  UCONbits.PKTDIS = 0;
}


  // Configures the buffer descriptor for endpoint 0 so that it is waiting for
  // the status stage of a control transfer.
void WaitForSetupStage(void) {
  ctrlTransferStage = SETUP_STAGE;
  EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
  EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
  // Give to SIE, enable data toggle checks
  EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
  EP_IN_BD(0).Stat.uc = 0x00;           // Give control to CPU
}


  // This is the starting point for processing a Control Transfer.  The code directly
  // follows the sequence of transactions described in the USB spec chapter 5.  The
  // only Control Pipe in this firmware is the Default Control Pipe (endpoint 0).
  // Control messages that have a different destination will be discarded.
void ProcessControlTransfer(void) {
#ifdef DEBUG_PRINT
    printf("Pct\n\r");
#endif
  if (USTATbits.DIR == OUT) {
  // Endpoint 0:out
  // Pull PID from middle of BD0STAT
    u8 PID = (EP_OUT_BD(0).Stat.uc & 0x3C) >> 2;
    if (PID == 0x0D)
  // SETUP PID - a transaction is starting
      SetupStage();
    else if (ctrlTransferStage == DATA_OUT_STAGE) {
  // Complete the data stage so that all information has
  // passed from host to device before servicing it.
      OutDataStage(0);

#ifdef USB_USE_HID
      if (HIDPostProcess) {
  // Determine which report is being set.
        u8 reportID = SetupPacket.wValue0;

  // Find out if an Output or Feature report has arrived on the control pipe.
  // Get the report type from the Setup packet.
        if (SetupPacket.wValue1 == 0x02) {
  // Output report
          SetOutputReport(reportID);
        }
        else if (SetupPacket.wValue1 == 0x03) {
  // Feature report
          SetFeatureReport(reportID);
        }
        else {
  // Unknown report type
        }
      }
#endif

  // Turn control over to the SIE and toggle the data bit
      if(EP_OUT_BD(0).Stat.DTS)
        EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
      else
        EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
    }
    else {
  // Prepare for the Setup stage of a control transfer
      WaitForSetupStage();
    }
  }
  else if(USTATbits.DIR == IN) {
  // Endpoint 0:in
    if ((UADDR == 0) && (deviceState == ADDRESS)) {
  // TBD: ensure that the new address matches the value of
  // "deviceAddress" (which came in through a SET_ADDRESS).
      UADDR = SetupPacket.wValue0;
#ifdef DEBUG_PRINT
      printf("UADDR = 0x%x\r\n", (word)UADDR);
#endif
      if(UADDR == 0)
  // If we get a reset after a SET_ADDRESS, then we need
  // to drop back to the Default state.
        deviceState = DEFAULT_STATUS;
    }

    if (ctrlTransferStage == DATA_IN_STAGE) {
  // Start (or continue) transmitting data
      InDataStage(0);

  // Turn control over to the SIE and toggle the data bit
      if(EP_IN_BD(0).Stat.DTS)
        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
      else
        EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
    }
    else {
  // Prepare for the Setup stage of a control transfer
      WaitForSetupStage();
    }
  }
  else {
#ifdef DEBUG_PRINT
    printf("IN on EP %d\r\n", (USTAT >> 3) & 0x0f);
    printf("USTAT = 0x%uhx\r\n", USTAT);
#endif
  }
}



void EnableUSBModule(void)
{
    // TBD: Check for voltage coming from the USB cable and use that
    // as an indication we are attached.
    if(UCONbits.USBEN == 0)
    {
        #ifdef DEBUG_PRINT
        printf("Enable the module\r\n");
        #endif
        UCON = 0;
        UIE = 0;
        UCONbits.USBEN = 1;
        deviceState = ATTACHED;
    }

    // If we are attached and no single-ended zero is detected, then
    // we can move to the Powered state.
    if ((deviceState == ATTACHED) && !UCONbits.SE0)
    {
        UIR = 0;
        UIE = 0;
        UIEbits.URSTIE = 1;
        UIEbits.IDLEIE = 1;
        deviceState = POWERED;
        #ifdef DEBUG_PRINT
        printf("Device powered\r\n");
        #endif
    }
}


  // Unsuspend the device
void UnSuspend(void)
{
    #ifdef DEBUG_PRINT
    printf("UnSuspend\r\n");
    #endif

    UCONbits.SUSPND = 0;
    UIEbits.ACTVIE = 0;
    UIRbits.ACTVIF = 0;
}


  // Full speed devices get a Start Of Frame (SOF) packet every 1 millisecond.
  // Nothing is currently done with this interrupt (it is simply masked out).
void StartOfFrame(void)
{
      // TBD: Add a callback routine to do something
      UIRbits.SOFIF = 0;
}

  // This routine is called in response to the code stalling an endpoint.
void Stall(void)
{
    #ifdef DEBUG_PRINT
    printf("Stall\r\n");
    #endif
    if(UEP0bits.EPSTALL == 1)
    {
        // Prepare for the Setup stage of a control transfer
        WaitForSetupStage();
        UEP0bits.EPSTALL = 0;
    }
    UIRbits.STALLIF = 0;
}


  // Suspend all processing until we detect activity on the USB bus
void Suspend(void)
{
#ifdef ALLOW_SUSPEND
#ifdef DEBUG_PRINT
  printf("Suspend\r\n");
#endif
  UIEbits.ACTVIE = 1;
  UIRbits.IDLEIF = 0;
  UCONbits.SUSPND = 1;

  #if defined(__18f25k50) || defined(__18f45k50)
  PIR3bits.USBIF = 0;
  #else
  PIR2bits.USBIF = 0;
  #endif

  #if defined(__18f25k50) || defined(__18f45k50)
  INTCONbits.IOCIF = 0;
  #else
  INTCONbits.RBIF = 0;
  #endif

  #if defined(__18f25k50) || defined(__18f45k50)
  PIE3bits.USBIE = 1;
  #else
  PIE2bits.USBIE = 1;
  #endif

  #if defined(__18f25k50) || defined(__18f45k50)
  INTCONbits.IOCIE = 1;
  #else
  INTCONbits.RBIE = 1;
  #endif

  // disable the USART
#ifdef DEBUG_PRINT
  #if defined(__18f25k50) || defined(__18f45k50)
  RCSTA1bits.CREN = 0;
  TXSTA1bits.TXEN = 0;
  #else
  RCSTAbits.CREN = 0;
  TXSTAbits.TXEN = 0;
  #endif
#endif

  Sleep();

  // enable the USART
#ifdef DEBUG_PRINT
  #if defined(__18f25k50) || defined(__18f45k50)
  RCSTA1bits.CREN = 1;
  TXSTA1bits.TXEN = 1;
  #else
  RCSTAbits.CREN = 1;
  TXSTAbits.TXEN = 1;
  #endif
#endif

  #if defined(__18f25k50) || defined(__18f45k50)
  PIE3bits.USBIE = 0;
  #else
  PIE2bits.USBIE = 0;
  #endif

  #if defined(__18f25k50) || defined(__18f45k50)
  INTCONbits.IOCIE = 0;
  #else
  INTCONbits.RBIE = 0;
  #endif
#endif
}


void BusReset() {
  UEIR  = 0x00;
  UIR   = 0x00;
  UEIE  = 0x9f;
  UIE   = 0x7b;
  UADDR = 0x00;

  // Set endpoint 0 as a control pipe
  UEP0 = EP_CTRL | HSHK_EN;

  // Flush any pending transactions
  while (UIRbits.TRNIF == 1)
    UIRbits.TRNIF = 0;

  // Enable packet processing
  UCONbits.PKTDIS = 0;

  // Prepare for the Setup stage of a control transfer
  WaitForSetupStage();

  remoteWakeup = 0;                     // Remote wakeup is off by default
  selfPowered = 0;                      // Self powered is off by default
  currentConfiguration = 0;             // Clear active configuration
  deviceState = DEFAULT_STATUS;
}


  // Main entry point for USB tasks.  Checks interrupts, then checks for transactions.
void ProcessUSBTransactions(void)
{
  // See if the device is connected yet.
  if(deviceState == DETACHED)
    return;

  // If the USB became active then wake up from suspend
  if(UIRbits.ACTVIF && UIEbits.ACTVIE)
    UnSuspend();

  // If we are supposed to be suspended, then don't try performing any
  // processing.
  if(UCONbits.SUSPND == 1)
    return;

  // Process a bus reset
  if (UIRbits.URSTIF && UIEbits.URSTIE)
    BusReset();

  // No bus activity for a while - suspend the firmware
  if (UIRbits.IDLEIF && UIEbits.IDLEIE)
    Suspend();

  
  if (UIRbits.SOFIF && UIEbits.SOFIE)
    StartOfFrame();
  
  if (UIRbits.STALLIF && UIEbits.STALLIE)
    Stall();

  // TBD: See where the error came from.
  // Clear errors
  if (UIRbits.UERRIF && UIEbits.UERRIE)
    UIRbits.UERRIF = 0;

  // Unless we have been reset by the host, no need to keep processing
  if (deviceState < DEFAULT_STATUS)
    return;

  // A transaction has finished.  Try default processing on endpoint 0.
  if(UIRbits.TRNIF && UIEbits.TRNIE)
  {
    ProcessControlTransfer();
    // Turn off interrupt
    UIRbits.TRNIF = 0;
  }
}


#if 0
  // Test - put something into EEPROM
code at 0xF00000 word dataEEPROM[] = {
  0, 1, 2, 3, 4, 5, 6, 7,
  '0', '1', '2', '3', '4', '5', '6', '7',
  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};
#endif
