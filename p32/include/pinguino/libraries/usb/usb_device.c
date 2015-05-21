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

#ifndef USBDEVICE_C
#define USBDEVICE_C

#include <p32xxxx.h>
#include <typedef.h>
#include <string.h>             // bzero, ...
#include <usb/usb_device.h>

#if (USB_PING_PONG_MODE != USB_PING_PONG__FULL_PING_PONG)
    #error "PIC32 only supports full ping pong mode."
#endif

volatile BDT_ENTRY *pBDTEntryEP0OutCurrent;
volatile BDT_ENTRY *pBDTEntryEP0OutNext;
volatile BDT_ENTRY *pBDTEntryOut[USB_EP_NUM+1];
volatile BDT_ENTRY *pBDTEntryIn[USB_EP_NUM+1];
USBVOLATILE u8 USBCableConnected;      // Current VBUS level 
USBVOLATILE u8 usb_alternate_interface[USB_INT_NUM];
USBVOLATILE u8 short_packet_status;
USBVOLATILE u8 control_transfer_state;
USBVOLATILE u8 ustat_saved;

USBVOLATILE u8 usb_device_state;
USBVOLATILE u8 usb_active_configuration;
USBVOLATILE IN_PIPE usb_in_pipe;
USBVOLATILE OUT_PIPE usb_out_pipe;
USBVOLATILE u8 usb_remote_wakeup;

// Buffer Descriptor Table
volatile BDT_ENTRY usb_buffer[(USB_EP_NUM + 1) * 4] __attribute__ ((aligned (512)));

// EP0 Buffer Space
volatile CTRL_TRF_SETUP usb_setup_pkt;           // 8-byte only

// Buffer for control transfer data
volatile u8 ctrl_trf_data[USB_EP0_BUFF_SIZE];

/***********************************************************************
 * Initializes the device stack in the default state.
 * The USB module will be completely reset including all of the internal
 * variables, registers, and interrupt flags.
 **********************************************************************/

void usb_device_init(void)
{
    u8 i;
    u32 phyaddrusbbuf = ConvertToPhysicalAddress(&usb_buffer);

    #ifdef DEBUG
    //SerialPrint(UART,"\r\nDEVICE INIT\r\n");
    #endif

    // Clear all of the BDT entries
    for (i=0; i<(sizeof(usb_buffer)/sizeof(BDT_ENTRY)); i++)
       usb_buffer[i].Val = 0x00;

    //clear all of the internal pipe information
    usb_in_pipe.info.Val = 0;
    usb_out_pipe.info.Val = 0;
    usb_out_pipe.wCount = 0;

    // Initialize all pBDTEntryIn[] and pBDTEntryOut[] pointers to NULL,
    // so they don't get used inadvertently.
    for(i = 0; i < (USB_EP_NUM + 1); i++)
    {
        pBDTEntryIn[i]  = 0;
        pBDTEntryOut[i] = 0;
    }

    // Get ready for the first packet
    pBDTEntryIn[0] = (volatile BDT_ENTRY*) &usb_buffer[EP0_IN_EVEN];

    // Clear active configuration
    usb_active_configuration = 0;

    // -------------------------------------------------------------------------
    // ---REGISTERS-------------------------------------------------------------
    // -------------------------------------------------------------------------

    // Init. OTG module Off
    U1PWRC = 0;
    U1OTGCON = 0;
    U1CON = 0;

    // Power up the USB module (done in usb_check_cable)
    U1PWRC |= _U1PWRC_USBPWR_MASK;

    // Set the physical address of the BDT
    U1BDTP1 = phyaddrusbbuf >> 8;
    U1BDTP2 = phyaddrusbbuf >> 16;
    U1BDTP3 = phyaddrusbbuf >> 24;
    
    // Reset all endpoints
    //bzero((void*) &U1EP0, USB_EP_NUM);

    // Reset to default address
    U1ADDR = 0x00;

    // Reset configuration
    // Must be preprogrammed prior to enabling the module.
    U1CNFG1 = 0x00;

    // Enable USB interrupts
    // The interrupt flag bits are cleared by writing a 1
    U1IE    = 0x00;
    #ifdef DEBUG
    U1EIE   = 0x00;
    #endif
    U1OTGIE = _U1OTGIE_SESVDIE_MASK;

    U1IR    = 0xFF;
    #ifdef DEBUG
    U1EIR   = 0xFF;
    #endif
    U1OTGIR = _U1OTGIR_SESVDIF_MASK;
    
    // Indicate if USB cable is plugged or not
    USBCableConnected = U1OTGSTATbits.SESVD; 

    // Indicate that we are in the detached state
    usb_device_state = DETACHED_STATE;
}

/***********************************************************************
 * Check if USB cable is plugged or unplugged
 **********************************************************************/
  
void usb_check_cable(void)
{
    u8 VBUSLocal = (U1OTGSTATbits.SESVD != 0); 

    if (VBUSLocal != USBCableConnected)
    { 
        USBCableConnected = VBUSLocal; 

        if (USBCableConnected)
        { 
            #ifdef DEBUG
            SerialPrint(UART,"USB CABLE PLUGGED\r\n");
            #endif
            
            // Enable USB module
            while (!U1CONbits.USBEN)
                U1CON |= _U1CON_USBEN_MASK;

            // Power up the USB module
            U1PWRC |= _U1PWRC_USBPWR_MASK;
             
            // Enable power to the D+/D- pull-up resistors
            U1OTGCON |= _U1OTGCON_DMPULUP_MASK | _U1OTGCON_DPPULUP_MASK;

            // Enable USB reset and idle interrupts
            U1IE = _U1IE_URSTIE_MASK | _U1IE_IDLEIE_MASK;
            U1IR = _U1IR_URSTIF_MASK | _U1IR_IDLEIF_MASK;
            
            // moved to the powered state
            usb_device_state = POWERED_STATE;
        }
        else
        { 
            #ifdef DEBUG
            SerialPrint(UART,"USB CABLE UNPLUGGED\r\n");
            #endif
        
            // Disable USB interrupts
            U1IE  = 0;
            U1EIE = 0;

            // Remove power from the D+/D- pull-up resistor
            U1OTGCONbits.DMPULUP = 0;
            U1OTGCONbits.DPPULUP = 0;

            // Disable USB module
            U1CON = 0;             

            // Shutdown the USB module
            //U1PWRCCLR = _U1PWRC_USBPWR_MASK;

            // Suspend the USB module
            U1PWRC = _U1PWRC_USUSPEND_MASK;

            // Back to the detached state
            usb_device_state = DETACHED_STATE;
        } 
    } 
}

/***********************************************************************
 * Process all USB interrupts.
 *
 * Should be called periodically to receive and transmit packets.
 * Preferably once every 100us during the enumeration process.
 * After the enumeration process time requirements is more relaxed.
 *
 * Alternatively, this routine may be called from the Interrupt Service
 * Routine (ISR) whenever a USB interrupt occurs.
 **********************************************************************/
 
void usb_device_tasks(void)
{
    #ifdef DEBUG
    //SerialPrint(UART, "U1IR=0b");
    //SerialPrintNumber(UART, U1IR, 2);
    //SerialPrint(UART, "\r\n");
    #endif

    // -------------------------------------------------------------------------
    // Check if USB cable is still plugged
    // -------------------------------------------------------------------------

    if (U1OTGIE & _U1OTGIE_SESVDIE_MASK)
    {
        if (U1OTGIR & _U1OTGIR_SESVDIF_MASK)
        {  
            #ifdef DEBUG
            //SerialPrint(UART,"SESSION CHANGE DETECTED\r\n");
            #endif

            // Power up the USB module
            // Enable reset and idle interrupts
            // If the device is connected
            usb_check_cable();

            // Clear SESVD flag
            U1OTGIR |= _U1OTGIR_SESVDIF_MASK;
        }
    }

    // Pointless to continue servicing if the device is detached.
    if (usb_device_state == DETACHED_STATE)
    {
        // At this point the PIC can also go into sleep, idle
        // or switch to a slower clock.
        return;
    }

    // -------------------------------------------------------------------------
    // If the USB became active then wake up from suspend
    // -------------------------------------------------------------------------

    if (U1OTGIE & _U1OTGIE_ACTVIE_MASK)
    {
        if (U1OTGIR & _U1OTGIR_ACTVIF_MASK)
        {  
            #ifdef DEBUG
            //SerialPrint(UART,"ACTIVITY DETECTED\r\n");
            #endif

            // Enable USB reset and idle interrupts
            // because once the USUSPEND is clear
            // the URSTIF bit will be asserted.
            U1IE = _U1IE_URSTIE_MASK | _U1IE_IDLEIE_MASK;
            U1IR = _U1IR_URSTIF_MASK | _U1IR_IDLEIF_MASK;

            // Exit from supsend mode
            U1PWRCbits.USUSPEND = 0;

            U1OTGIR |= _U1OTGIR_ACTVIF_MASK;
        }
    }
    
    // Pointless to continue servicing if the device is in suspend mode.
    if (U1PWRC & _U1PWRC_USUSPEND_MASK)
    {
        #ifdef DEBUG
        //SerialPrint(UART,"DEVICE SUSPENDED\r\n");
        #endif

        return;
    }
    
    /**
     *  Task B: Service USB Bus Reset Interrupt.
     *  When the host wants to start communicating with a device it will
     *  start by applying a 'Reset' condition which sets the device to
     *  its default unconfigured state.
     *  When bus reset is received during suspend, ACTVIF will be set first,
     *  once the UCON_SUSPND is clear, then the URSTIF bit will be asserted.
     *  This is why URSTIF is checked after ACTVIF.
     **/

    // -------------------------------------------------------------------------
    // Host has sent a Bus Reset
    // -------------------------------------------------------------------------

    if (U1IE & _U1IE_URSTIE_MASK)
    {
        if (U1IR & _U1IR_URSTIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint(UART,"DEVICE RESET\r\n");
            #endif

            // Sets the device to its default unconfigured state.
            usb_device_init();
            
            // Initialize EP0 as a Ctrl EP
            U1EP0 = EP_CTRL | USB_HANDSHAKE_ENABLED;
            
            // Configures the buffer descriptor for endpoint 0 so that
            // it's waiting for the status stage of a control transfer.
            usb_buffer[EP0_OUT_EVEN].ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
            usb_buffer[EP0_OUT_EVEN].CNT = USB_EP0_BUFF_SIZE;
            usb_buffer[EP0_OUT_EVEN].STAT.Val &= ~_STAT_MASK;
            usb_buffer[EP0_OUT_EVEN].STAT.Val |= _USIE|_DAT0|_DTSEN|_BSTALL;

            // Flush any pending transactions
            while (U1IR & _U1IR_TRNIF_MASK)
                U1IR |= _U1IR_TRNIF_MASK;

            // Enable packet processing (PKTDIS=0)
            // Stop trying to reset ping pong buffer pointers
            // *** MUST BE CLEARED ***
            U1CONbits.PKTDIS = 0;
            U1CONbits.PPBRST = 0;
            //U1CONbits.RESUME = 0;

            // Enable interrupts
            U1IE = _U1IE_IDLEIE_MASK  | _U1IE_TRNIE_MASK;
                   //_U1IE_STALLIE_MASK;
                   //_U1IE_SOFIE_MASK;
                   //_U1IE_URSTIE_MASK;
                   //_U1IE_RESUMEIE_MASK;

            U1IR = _U1IR_IDLEIF_MASK  | _U1IR_TRNIF_MASK;
                   //_U1IR_STALLIF_MASK;
                   //_U1IR_SOFIF_MASK;
                   //_U1IR_URSTIF_MASK;
                   //_U1IR_RESUMEIF_MASK;

            #ifdef DEBUG
            U1IE |= _U1IE_UERRIE_MASK;
            U1IR |= _U1IR_UERRIF_MASK; 
            U1EIE = 0xFF;
            U1EIR = 0xFF;
            #endif

            // Move to the default state
            usb_device_state = DEFAULT_STATE;

            // Clear RESET interrupt
            U1IR |= _U1IR_URSTIF_MASK;
        }
    }

    // -------------------------------------------------------------------------
    // No bus activity for a while - suspend the firmware
    // -------------------------------------------------------------------------

    if (U1IE & _U1IE_IDLEIE_MASK)
    {
        if (U1IR & _U1IR_IDLEIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint(UART,"DEVICE IDLE\r\n");
            #endif

            // At this point the PIC can also go into sleep, idle
            // or switch to a slower clock.
            U1PWRC |= _U1PWRC_USUSPEND_MASK;

            // Interrupts we want to check to wake up
            U1IE = _U1IE_URSTIE_MASK; // | _U1IE_IDLEIE_MASK;
            U1IR = 0xFF;

            U1OTGIE = _U1OTGIE_ACTVIE_MASK | _U1OTGIE_SESVDIE_MASK;
            U1OTGIR = _U1OTGIR_ACTVIF_MASK | _U1OTGIR_SESVDIF_MASK;

            // Pointless to continue servicing if the device is in suspend mode.
            return;
        }
    }

    // -------------------------------------------------------------------------
    // Start Of Frame
    // -------------------------------------------------------------------------

    #if 0
    if (U1IE & _U1IE_SOFIE_MASK)
    {
        if (U1IR & _U1IR_SOFIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint(UART,"Start Of Frame.\r\n");
            #endif
            
            //usbcb_sof_handler();

            // Clear SOF flag
            U1IR |= _U1IR_SOFIF_MASK;
        }
    }
    #endif
    
    // -------------------------------------------------------------------------
    // Stall
    // -------------------------------------------------------------------------

    #if 0
    if (U1IE & _U1IE_STALLIE_MASK)
    {
        if (U1IR & _U1IR_STALLIF_MASK)
        {
            #ifdef DEBUG
            //SerialPrint(UART,"DEVICE STALL\r\n");
            #endif
            
            if (U1EP0 & _U1EP0_EPSTALL_MASK)
            {
                // UOWN - if 0, owned by CPU, if 1, owned by SIE
                if (pBDTEntryEP0OutCurrent->STAT.Val == _USIE &&
                    pBDTEntryIn[0]->STAT.Val == (_USIE | _BSTALL))
                {
                    // Set ep0Bo to stall also
                    pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
                }
                // Clear stall status
                U1EP0CLR = _U1EP0_EPSTALL_MASK;
            }
            // Clear STALL flag
            U1IR |= _U1IR_STALLIF_MASK;
        }
    }
    #endif
    
    // -------------------------------------------------------------------------
    // Error
    // -------------------------------------------------------------------------

    #ifdef DEBUG
    if (U1IE & _U1IE_UERRIE_MASK)
    {
        if (U1IR & _U1IR_UERRIF_MASK)
        {
            SerialPrint(UART,"ERROR CODE 0b");
            SerialPrintNumber(UART, U1EIR, 2);
            SerialPrint(UART,"\r\n");

            // Clears USB error flag
            U1EIR = 0xFF;
            U1IR |= _U1IR_UERRIF_MASK;
        }
    }
    #endif
    
    // Stop servicing if the host has not sent a bus reset.
    if (usb_device_state < DEFAULT_STATE)
        return;
    
    // -------------------------------------------------------------------------
    // A transaction has finished
    // -------------------------------------------------------------------------

    if (U1IE & _U1IE_TRNIE_MASK)
    {
        if (U1IR & _U1IR_TRNIF_MASK)
        {
            #ifdef DEBUG
            SerialPrint(UART,"TRANSACTION COMPLETE\r\n");
            #endif

             // Checks for three transaction types :
             // 1. EP0 SETUP
             // 2. EP0 OUT
             // 3. EP0 IN
             // It ignores all other types (i.e. EP1, EP2, etc.)

            // U1STAT provides endpoint information
            ustat_saved = U1STAT;

            //usb_ctrl_ep_service();
            // If the last packet was a EP0 OUT packet
            if ((ustat_saved & USTAT_EP0_PP_MASK) == USTAT_EP0_OUT)
            {
                // Point to the EP0 OUT buffer of the buffer that arrived
                pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)
                    &usb_buffer[(ustat_saved & USTAT_EP_MASK) >> 2];

                // Set the next out to the current out packet
                pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;

                // Toggle it to the next ping pong buffer
                *(u8*)&pBDTEntryEP0OutNext ^= USB_NEXT_EP0_OUT_PING_PONG;

                // If the current EP0 OUT buffer has a SETUP token
                if (pBDTEntryEP0OutCurrent->STAT.PID == SETUP_TOKEN)
                {
                    // Handle the control transfer
                    usb_ctrl_trf_setup_handler();
                }
                else
                {
                    // Handle the DATA transfer
                    usb_ctrl_trf_out_handler();
                }
            }
            
            else //if ((ustat_saved & USTAT_EP0_PP_MASK) == USTAT_EP0_IN)
            {
                // Otherwise the transmission was and EP0 IN
                // so take care of the IN transfer
                usb_ctrl_trf_in_handler();
            }
            
            // Clear the TRNIF interrupt
            U1IR |= _U1IR_TRNIF_MASK;
        }
    }

    // Clear USB interrupt
    //IFS1CLR = _IFS1_USBIF_MASK;
}

/***********************************************************************
 * This routine is a task dispatcher and has 3 stages.
 * 1. It initializes the control transfer state machine.
 * 2. It calls on each of the module that may know how to
 *    service the Setup Request from the host.
 *    Module Example: USBD, HID, CDC, MSD, ...
 *    A callback function, usbcb_check_other_req(),
 *    is required to call other module handlers.
 * 3. Once each of the modules has had a chance to check if
 *    it is responsible for servicing the request, stage 3
 *    then checks direction of the transfer to determine how
 *    to prepare EP0 for the control transfer.
 *    Refer to usb_ctrl_ep_service_complete() for more details.
 *
 * PreCondition: usb_setup_pkt buffer is loaded with valid USB Setup Data
 *
 * Microchip USB Firmware has three different states for
 * the control transfer state machine:
 * 1. WAIT_SETUP
 * 2. CTRL_TRF_TX
 * 3. CTRL_TRF_RX
 * Refer to firmware manual to find out how one state
 * is transitioned to another.
 *
 * A Control Transfer is composed of many USB transactions.
 * When transferring data over multiple transactions,
 * it is important to keep track of data source, data
 * destination, and data count. These three parameters are
 * stored in pSrc, pDst, and wCount. A flag is used to
 * note if the data source is from ROM or RAM.
 **********************************************************************/

void usb_ctrl_trf_setup_handler(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_ctrl_trf_setup_handler\r\n");
    #endif

    //if the SIE currently owns the buffer
    if (pBDTEntryIn[0]->STAT.UOWN != 0)
    {
        // give control back to the CPU
        // Compensate for after a STALL
        pBDTEntryIn[0]->STAT.Val = _UCPU;
    }

    // Keep track of if a short packet has been sent yet or not
    short_packet_status = SHORT_PKT_NOT_USED;

    /* Stage 1 */
    control_transfer_state = WAIT_SETUP;

    usb_in_pipe.wCount = 0;
    usb_in_pipe.info.Val = 0;

    /* Stage 2 */
    usb_check_std_request();

    // initialize the required endpoints

    #if defined(__USBHID__)

    // TODO : usb_check_hid_request();

    #elif defined(__USBCDC__)
    
    usb_check_cdc_request();
    
    #elif defined(__USBUART__)

    // TODO : usb_check_uart_request();

    #elif defined(__USBBULK__)

    // TODO : usb_check_bulk_request();

    #else

    #error "No USB class drivers (CDC, HID, BULK) defined"

    #endif

    /* Stage 3 */
    usb_ctrl_ep_service_complete();
}

/***********************************************************************
 * This routine handles an OUT transaction according to which control
 * transfer state is currently active.
 *
 * Note that if the the control transfer was from host to device,
 * the session owner should be notified at the end of each OUT
 * transaction to service the received data.
 **********************************************************************/
 
void usb_ctrl_trf_out_handler(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_ctrl_trf_out_handler\r\n");
    #endif

    if (control_transfer_state == CTRL_TRF_RX)
    {
        usb_ctrl_trf_rx_service();
    }
    else    // CTRL_TRF_TX
    {
        usb_prepare_for_next_setup_trf();
    }
}

/***********************************************************************
 * This routine handles an IN transaction according to which control
 * transfer state is currently active.
 *
 * A Set Address Request must not change the acutal address of the
 * device until the completion of the control transfer.
 * The end of the control transfer for Set Address Request is an IN
 * transaction. Therefore it is necessary to service this unique
 * situation when the condition is right.
 **********************************************************************/
 
void usb_ctrl_trf_in_handler(void)
{
    u8 lastDTS;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_ctrl_trf_in_handler\r\n");
    #endif

    lastDTS = pBDTEntryIn[0]->STAT.DTS;

    //switch to the next ping pong buffer
    *(u8*)&pBDTEntryIn[0] ^= USB_NEXT_EP0_IN_PING_PONG;

    // Must check if in ADR_PENDING_STATE
    if (usb_device_state == ADR_PENDING_STATE)
    {
        U1ADDR = usb_setup_pkt.bDevADR;

        if (U1ADDR > 0)
        {
            usb_device_state = ADDRESS_STATE;
        }
        else
        {
            usb_device_state = DEFAULT_STATE;
        }
    }

    if (control_transfer_state == CTRL_TRF_TX)
    {
        pBDTEntryIn[0]->ADR = ConvertToPhysicalAddress(ctrl_trf_data);
        usb_ctrl_trf_tx_service();

        /* v2b fix */
        if (short_packet_status == SHORT_PKT_SENT)
        {
            // If a short packet has been sent, don't want to send any more,
            // stall next time if host is still trying to read.
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
        else
        {
            if (lastDTS == 0)
            {
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
            }
            else
            {
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT0|_DTSEN;
            }
        }
    }
    else
    {
        // CTRL_TRF_RX
        usb_prepare_for_next_setup_trf();
    }
}

/***********************************************************************
 * The routine forces EP0 OUT to be ready for a new
 * Setup transaction, and forces EP0 IN to be owned by CPU.
 **********************************************************************/

void usb_prepare_for_next_setup_trf(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_prepare_for_next_setup_trf\r\n");
    #endif
    
    u8 setup_cnt;
    BDT_ENTRY* p;

    if ((control_transfer_state == CTRL_TRF_RX) &&
        (U1CON & _U1CON_PKTDIS_MASK) &&
        (pBDTEntryEP0OutCurrent->CNT == sizeof(CTRL_TRF_SETUP)) &&
        (pBDTEntryEP0OutCurrent->STAT.PID == SETUP_TOKEN) &&
        (pBDTEntryEP0OutNext->STAT.UOWN == 0))
    {

        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);

        // The Setup data was written to the ctrl_trf_data buffer, must copy
        // it back to the usb_setup_pkt buffer so that it can be processed correctly
        // by usb_ctrl_trf_setup_handler().
        for(setup_cnt = 0; setup_cnt < sizeof(CTRL_TRF_SETUP); setup_cnt++)
        {
            *(((u8*) &usb_setup_pkt) + setup_cnt) = *(((u8*) &ctrl_trf_data) + setup_cnt);
        }
    }

    else
    {
        control_transfer_state = WAIT_SETUP;
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;  //Added #F1
        pBDTEntryIn[0]->STAT.Val = _UCPU;             // Should be removed

        p = (BDT_ENTRY*)(((u32)pBDTEntryIn[0])^USB_NEXT_EP0_IN_PING_PONG);
        p->STAT.Val = _UCPU;
        //ep0Bi.Stat.Val = _USIE|_BSTALL;   // Should be added #F3
    }

    //if someone is still expecting data from the control transfer
    //  then make sure to terminate that request and let them know that
    //  they are done
    if (usb_out_pipe.info.bits.busy == 1)
    {
        if (usb_out_pipe.pFunc != 0)
        {
            usb_out_pipe.pFunc();
        }
        usb_out_pipe.info.bits.busy = 0;
    }

}//end usb_prepare_for_next_setup_trf

/***********************************************************************
 * This routine checks the setup data packet to see
 * if it knows how to handle it
 **********************************************************************/

void usb_check_std_request(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_check_std_request\r\n");
    #endif
    
    if (usb_setup_pkt.RequestType != STANDARD)
        return;

    //#ifdef DEBUG
    //SerialPrint(UART,"> Request type STANDARD = ");
    //#endif

    //#ifdef DEBUG
    //SerialPrint(UART,"> Request = ");
    //SerialPrintNumber(UART,usb_setup_pkt.bRequest,10);
    //SerialPrint(UART,"\r\n");
    //#endif

    if (usb_setup_pkt.bRequest == GET_DSC)
    {
        #ifdef DEBUG
        //SerialPrint(UART,"GET_DSC\r\n");
        #endif

        usb_std_get_dsc_handler();
        return;
    }

    if (usb_setup_pkt.bRequest == SET_ADR)
    {
        #ifdef DEBUG
        //SerialPrint(UART,"SET_ADR\r\n");
        #endif

        // This will generate a zero length packet
        usb_in_pipe.info.bits.busy = 1;
        // Update state only
        usb_device_state = ADR_PENDING_STATE;
        // See usb_ctrl_trf_in_handler() for the next step
        return;
    }

    if (usb_setup_pkt.bRequest == SET_CFG)
    {
        #ifdef DEBUG
        //SerialPrint(UART,"SET_CFG\r\n");
        #endif

        usb_std_set_cfg_handler();
        return;
    }

    #if 0
    if (usb_setup_pkt.bRequest == GET_CFG)
    {
        #ifdef DEBUG
        SerialPrint(UART,"GET_CFG\r\n");
        #endif

        // Set Source
        usb_in_pipe.pSrc.bRam = (u8*)&usb_active_configuration;
        // Set memory type
        usb_in_pipe.info.bits.ctrl_trf_mem = _RAM;
        // Set data count
        usb_in_pipe.wCount |= 0xff;
        usb_in_pipe.info.bits.busy = 1;
        return;
    }
    #endif
    
    #if 0
    if (usb_setup_pkt.bRequest == GET_STATUS)
    {
        #ifdef DEBUG
        SerialPrint(UART,"GET_STATUS\r\n");
        #endif
        usb_std_get_status_handler();
        return;
    }
    #endif
    
    #if 0
    if ( (usb_setup_pkt.bRequest == CLR_FEATURE) || 
         (usb_setup_pkt.bRequest == SET_FEATURE))
    {
        #ifdef DEBUG
        SerialPrint(UART,"SET_FEATURE\r\n");
        #endif

        usb_std_feature_req_handler();
        return;
    }
    #endif
    
    #if 0
    if (usb_setup_pkt.bRequest == GET_INTF)
    {
        #ifdef DEBUG
        SerialPrint(UART,"GET_INTF\r\n");
        #endif

        // Set source
        usb_in_pipe.pSrc.bRam = (u8*)&usb_alternate_interface + usb_setup_pkt.bIntfID;
        // Set memory type
        usb_in_pipe.info.bits.ctrl_trf_mem = _RAM;
        // Set data count
        usb_in_pipe.wCount |= 0xff;
        usb_in_pipe.info.bits.busy = 1;
        return;
    }
    #endif
    
    #if 0
    if (usb_setup_pkt.bRequest == SET_INTF)
    {
        #ifdef DEBUG
        SerialPrint(UART,"SET_INTF\r\n");
        #endif
        
        usb_in_pipe.info.bits.busy = 1;
        usb_alternate_interface[usb_setup_pkt.bIntfID] = usb_setup_pkt.bAltID;
        return;
    }
    #endif
    
    // Note : SET_DESCRIPTOR requests are not used in most applications,
    // and it is optional to support this type of request.
    #if 0
    if (usb_setup_pkt.bRequest == SET_DSC)
    {
        #ifdef DEBUG
        SerialPrint(UART,"SET_DSC\r\n");
        #endif

        //usbcb_std_set_dsc_handler();
        return;
    }
    #endif
    
    #if 0
    if (usb_setup_pkt.bRequest == SYNCH_FRAME)
    {
        #ifdef DEBUG
        SerialPrint(UART,"SYNCH_FRAME\r\n");
        #endif
        return;
    }
    #endif
}

/***********************************************************************
 * This routine handles the standard SET & CLEAR FEATURES requests
 **********************************************************************/

#if 0
void usb_std_feature_req_handler(void)
{
    BDT_ENTRY* p;
    u8* pUEP;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_std_feature_req_handler\r\n");
    #endif
    
    if ((usb_setup_pkt.bFeature == DEVICE_REMOTE_WAKEUP)&&
        (usb_setup_pkt.Recipient == RCPT_DEV))
    {
        usb_in_pipe.info.bits.busy = 1;
        if (usb_setup_pkt.bRequest == SET_FEATURE)
            usb_remote_wakeup = 1;
        else
            usb_remote_wakeup = 0;
    }

    if ((usb_setup_pkt.bFeature == ENDPOINT_HALT)&&
        (usb_setup_pkt.Recipient == RCPT_EP)&&
        (usb_setup_pkt.EPNum != 0))
    {
        usb_in_pipe.info.bits.busy = 1;
        /* Must do address calculation here */

        if (usb_setup_pkt.EPDir == 0) // OUT_FROM_HOST
        {
            p = (BDT_ENTRY*)pBDTEntryOut[usb_setup_pkt.EPNum];
        }
        else
        {
            p = (BDT_ENTRY*)pBDTEntryIn[usb_setup_pkt.EPNum];
        }

        //if it was a SET_FEATURE request
        if (usb_setup_pkt.bRequest == SET_FEATURE)
        {
            // Then STALL the endpoint
            p->STAT.Val = _USIE|_BSTALL;
        }
        else
        {
            // If it was not a SET_FEATURE
            // point to the appropriate UEP register
            pUEP = (u8*) (&U1EP0 + (4 * usb_setup_pkt.EPNum));

            //Clear the STALL bit in the UEP register
            *pUEP &= ~UEP_STALL;

            if (usb_setup_pkt.EPDir == IN_TO_HOST)
            {
                // If the endpoint is an IN endpoint then we need to
                // return it to the CPU and reset the DTS bit so that
                // the next transfer is correct
                
                p->STAT.Val = _UCPU | _DAT0;
                // toggle over the to the next buffer
                *(u8*)&p ^= USB_NEXT_PING_PONG;
                p->STAT.Val = _UCPU | _DAT1;
            }
            else
            {
                // If the endpoint was an OUT endpoint then we need to
                // give control of the endpoint back to the SIE so that
                // the function driver can receive the data as they
                // expected.  Also need to set the DTS bit so the next
                // packet will be correct

                p->STAT.Val = _USIE|_DAT0|_DTSEN;
                //toggle over the to the next buffer
                *(u8*)&p ^= USB_NEXT_PING_PONG;
                p->STAT.Val = _USIE|_DAT1|_DTSEN;
            }
        }
    }
}
#endif

/***********************************************************************
 * This routine handles the standard GET_DESCRIPTOR request.
 **********************************************************************/

void usb_std_get_dsc_handler(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_std_get_dsc_handler\r\n");
    #endif

    if (usb_setup_pkt.bmRequestType == 0x80)
    {
        usb_in_pipe.info.Val = USB_INPIPES_ROM | USB_INPIPES_BUSY | USB_INPIPES_INCLUDE_ZERO;

        switch(usb_setup_pkt.bDescriptorType)
        {
            case USB_DESCRIPTOR_DEVICE:
                #ifdef DEBUG
                //SerialPrint(UART,"> USB_DESCRIPTOR_DEVICE\r\n");
                #endif
                
                usb_in_pipe.pSrc.bRom = (const u8*) &usb_device;
                usb_in_pipe.wCount = sizeof(usb_device);
                break;
                
            case USB_DESCRIPTOR_CONFIGURATION:
                #ifdef DEBUG
                //SerialPrint(UART,"> USB_DESCRIPTOR_CONFIGURATION\r\n");
                #endif
                
                //usb_in_pipe.pSrc.bRom = usb_config[usb_setup_pkt.bDscIndex];
                usb_in_pipe.pSrc.bRom = *(usb_config + usb_setup_pkt.bDscIndex);
                usb_in_pipe.wCount = *(usb_in_pipe.pSrc.wRom+1);// Set data count
                break;
                
            case USB_DESCRIPTOR_STRING:
                #ifdef DEBUG
                //SerialPrint(UART,"> USB_DESCRIPTOR_STRING\r\n");
                #endif
                
                #if defined(USB_NUM_STRING_DESCRIPTORS)
                if (usb_setup_pkt.bDscIndex < USB_NUM_STRING_DESCRIPTORS)
                #else
                if (1)
                #endif
                {
                    //Get a pointer to the String descriptor requested
                    //usb_in_pipe.pSrc.bRom = usb_string[usb_setup_pkt.bDscIndex];
                    usb_in_pipe.pSrc.bRom = *(usb_string + usb_setup_pkt.bDscIndex);
                    // Set data count
                    usb_in_pipe.wCount = *usb_in_pipe.pSrc.bRom;
                }
                else
                {
                    usb_in_pipe.info.Val = 0;
                }
                break;

            default:
                usb_in_pipe.info.Val = 0;
                break;
        }
    }
}

/***********************************************************************
 * This routine handles the standard GET_STATUS request
 **********************************************************************/

#if 0
void usb_std_get_status_handler(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_std_get_status_handler\r\n");
    #endif

    ctrl_trf_data[0] = 0;                   // Initialize content
    ctrl_trf_data[1] = 0;

    switch(usb_setup_pkt.Recipient)
    {
        case RCPT_DEV:
            usb_in_pipe.info.bits.busy = 1;
            ctrl_trf_data[0] |= 1;          // self powered

            if (usb_remote_wakeup == 1)
            {
                ctrl_trf_data[0] |= 2;
            }
            break;
            
        case RCPT_INTF:
            usb_in_pipe.info.bits.busy = 1; // No data to update
            break;
            
        case RCPT_EP:
            usb_in_pipe.info.bits.busy = 1;
            BDT_ENTRY *p;

            if (usb_setup_pkt.EPDir == 0)
                p = (BDT_ENTRY*)pBDTEntryOut[usb_setup_pkt.EPNum];
            else
                p = (BDT_ENTRY*)pBDTEntryIn[usb_setup_pkt.EPNum];

            if (p->STAT.Val & _BSTALL)      // Use _BSTALL as a bit mask
                ctrl_trf_data[0] = 1;       // Set bit0

            break;
    }//end switch

    if (usb_in_pipe.info.bits.busy == 1)
    {
        usb_in_pipe.pSrc.bRam = (u8*) &ctrl_trf_data; // Set Source
        usb_in_pipe.info.bits.ctrl_trf_mem = _RAM;    // Set memory type
        usb_in_pipe.wCount &= ~0xff;
        usb_in_pipe.wCount |= 2;            // Set data count
    }
}
#endif

/***********************************************************************
 * This routine wrap up the ramaining tasks in servicing a Setup Request.
 * Its main task is to set the endpoint controls appropriately for a
 * given situation.
 * There are three main scenarios:
 * a) There was no handler for the Request, in this case a STALL should
 *    be sent out.
 * b) The host has requested a read control transfer, endpoints are
 *    required to be setup in a specific way.
 * c) The host has requested a write control transfer, or a control data
 *    stage is not required, endpoints are required to be setup in a specific way.
 *
 * Packet processing is resumed by clearing PKTDIS bit.
 **********************************************************************/
 
void usb_ctrl_ep_service_complete(void)
{
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_ctrl_ep_service_complete\r\n");
    #endif

    // Clear PKTDIS to enable packet processing.
    U1CONbits.PKTDIS = 0;

    // CTRL_TRF_RX ?
    if (usb_in_pipe.info.bits.busy == 0)
    {
        if (usb_out_pipe.info.bits.busy == 1)
        {
            control_transfer_state = CTRL_TRF_RX;

            /// USBCtrlEPAllowStatusStage
            // 1. Prepare IN EP to respond to early termination
            pBDTEntryIn[0]->CNT = 0;
            pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;

            ///USBCtrlEPAllowDataStage
            // 2. Prepare OUT EP to receive data.
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&ctrl_trf_data);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
        }
        else
        {
            // Prepare EP0 to receive the next SETUP transaction.
            pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
            pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
            pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
        }
    }

    // CTRL_TRF_TX
    else
    {
        if (usb_out_pipe.info.bits.busy == 0)
        {
            if (usb_setup_pkt.DataDir == DEV_TO_HOST)
            {
                /// USBCtrlEPAllowDataStage
                if (usb_setup_pkt.wLength < usb_in_pipe.wCount)
                {
                    usb_in_pipe.wCount = usb_setup_pkt.wLength;
                }
                
                usb_ctrl_trf_tx_service();
                control_transfer_state = CTRL_TRF_TX;

                /// USBCtrlEPAllowStatusStage

                // 1. Prepare OUT EP to respond to early termination
                pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
                pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
                pBDTEntryEP0OutNext->STAT.Val = _USIE; // Note: DTSEN is 0!

                pBDTEntryEP0OutCurrent->CNT = USB_EP0_BUFF_SIZE;
                pBDTEntryEP0OutCurrent->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
                #if 0 // RetroBSD
                pBDTEntryEP0OutCurrent->STAT.Val = _USIE;
                #else // Microchip
                pBDTEntryEP0OutCurrent->STAT.Val = _USIE|_BSTALL;
                #endif

                // 2. Prepare IN EP to transfer data
                pBDTEntryIn[0]->ADR = ConvertToPhysicalAddress(&ctrl_trf_data);
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;
            }
            else // if (usb_setup_pkt.DataDir == HOST_TO_DEV)
            {
                control_transfer_state = CTRL_TRF_RX;

                /// USBCtrlEPAllowStatusStage
                // 1. Prepare IN EP to respond to early termination
                pBDTEntryIn[0]->CNT = 0;
                pBDTEntryIn[0]->STAT.Val = _USIE|_DAT1|_DTSEN;

                // 2. Prepare OUT EP to receive data.
                #if 0 // RetroBSD
                pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
                pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&ctrl_trf_data);
                pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
                #else // Microchip
                pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
                pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
                pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;
                #endif
            }
        }
    }
}

/***********************************************************************
 * This routine should be called from only two places.
 * One from usb_ctrl_ep_service_complete() and one from
 * usb_ctrl_trf_in_handler(). It takes care of managing a
 * transfer over multiple USB transactions.
 *
 * This routine works with isochronous endpoint larger than
 * 256 bytes and is shown here as an example of how to deal
 * with BC9 and BC8. In reality, a control endpoint can never
 * be larger than 64 bytes.
 *
 * PreCondition: pSrc, wCount, and usb_stat.ctrl_trf_mem are setup properly.
 **********************************************************************/
 
void usb_ctrl_trf_tx_service(void)
{
    u8 byteToSend;
    u8 *dst;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_ctrl_trf_tx_service\r\n");
    #endif

    // First, have to figure out how many byte of data to send.
    if (usb_in_pipe.wCount < USB_EP0_BUFF_SIZE)
    {
        byteToSend = usb_in_pipe.wCount;

        if (short_packet_status == SHORT_PKT_NOT_USED)
            short_packet_status = SHORT_PKT_PENDING;
        else if (short_packet_status == SHORT_PKT_PENDING)
            short_packet_status = SHORT_PKT_SENT;
    }
    else
    {
        byteToSend = USB_EP0_BUFF_SIZE;
    }

    // Next, load the number of bytes to send to BC9..0 in buffer descriptor
    pBDTEntryIn[0]->CNT = byteToSend;

    // Subtract the number of bytes just about to be sent from the total.
    usb_in_pipe.wCount = usb_in_pipe.wCount - byteToSend;

    // Set destination pointer
    dst = (USBVOLATILE u8*) ctrl_trf_data;

    // Determine type of memory source
    if (usb_in_pipe.info.bits.ctrl_trf_mem == USB_INPIPES_ROM)
    {
        while (byteToSend)
        {
            *dst++ = *usb_in_pipe.pSrc.bRom++;
            byteToSend--;
        }
    }
    else
    {
        // RAM
        while (byteToSend)
        {
            *dst++ = *usb_in_pipe.pSrc.bRam++;
            byteToSend--;
        }
    }
}

/***********************************************************************
 * *** This routine is only partially complete. Check for
 * new version of the firmware.
 *
 * PreCondition: pDst and wCount are setup properly.
 *               pSrc is always &ctrl_trf_data
 *               usb_stat.ctrl_trf_mem is always _RAM.
 *               wCount should be set to 0 at the start of each control transfer.
 **********************************************************************/
 
void usb_ctrl_trf_rx_service(void)
{
    u8 byteToRead, i;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_ctrl_trf_rx_service\r\n");
    #endif

    byteToRead = pBDTEntryEP0OutCurrent->CNT;

    // Accumulate total number of bytes read
    if (byteToRead > usb_out_pipe.wCount)
        byteToRead = usb_out_pipe.wCount;
    else
        usb_out_pipe.wCount -= byteToRead;

    for(i=0;i<byteToRead;i++)
        *usb_out_pipe.pDst.bRam++ = ctrl_trf_data[i];

    //If there is more data to read
    if (usb_out_pipe.wCount > 0)
    {
        /*
         * Don't have to worry about overwriting _KEEP bit
         * because if _KEEP was set, TRNIF would not have been
         * generated in the first place.
         */
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress (&ctrl_trf_data);

        if (pBDTEntryEP0OutCurrent->STAT.DTS == 0)
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT1|_DTSEN;
        else
            pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN;
    }
    else
    {
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_BSTALL;
        if (usb_out_pipe.pFunc != 0)
            usb_out_pipe.pFunc();
        usb_out_pipe.info.bits.busy = 0;
    }

    // reset ep0Bo.Cnt to USB_EP0_BUFF_SIZE

}//end usb_ctrl_trf_rx_service

/***********************************************************************
 * This routine first disables all endpoints by clearing UEP registers.
 * It then configures (initializes) endpoints by calling the callback
 * function usbcb_init_ep().
 **********************************************************************/

void usb_std_set_cfg_handler(void)
{
    u8 i;
    
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_std_set_cfg_handler\r\n");
    #endif

    // This will generate a zero length packet
    usb_in_pipe.info.bits.busy = 1;

    // disable all endpoints except endpoint 0
    bzero((void*) &U1EP1, USB_EP_NUM - 1);

    // Clear all of the BDT entries
    for (i=0; i<(sizeof(usb_buffer)/sizeof(BDT_ENTRY)); i++)
       usb_buffer[i].Val = 0x00;

    // Assert reset request to all of the Ping Pong buffer pointers
    U1CONbits.PPBRST = 1;

    // clear the alternate interface settings
    bzero((void*) &usb_alternate_interface, USB_INT_NUM);

    //Stop trying to reset ping pong buffer pointers
    U1CONbits.PPBRST = 0;

    pBDTEntryIn[0] = (volatile BDT_ENTRY*)&usb_buffer[EP0_IN_EVEN];

    //Set the next out to the current out packet
    pBDTEntryEP0OutCurrent = (volatile BDT_ENTRY*)&usb_buffer[EP0_OUT_EVEN];
    pBDTEntryEP0OutNext = pBDTEntryEP0OutCurrent;

    // set the current configuration
    usb_active_configuration = usb_setup_pkt.bConfigurationValue;

    // if the configuration value == 0
    if (usb_setup_pkt.bConfigurationValue == 0)
    {
        // Go back to the addressed state
        usb_device_state = ADDRESS_STATE;
    }
    else
    {
        // The device has been initialized
        // i.e. the host sends a SET_CONFIGURATION (wValue != 0) request.
        // Initializes the endpoints

        #if defined(__USBHID__)
        
        // TODO : hid_init_ep();
        
        #elif defined(__USBCDC__)
        
        cdc_init_endpoint();
        
        #elif defined(__USBUART__)
        
        // TODO : uart_init_ep();
        
        #elif defined(__USBBULK__)
        
        // TODO : bulk_init_ep();
        
        #else
        
        #error "No USB class drivers (CDC, HID, BULK) defined"
        
        #endif

        // Otherwise go to the configured state
        usb_device_state = CONFIGURED_STATE;
    }
}

/***********************************************************************
 * This function will configure the specified endpoint.
 *
 * Input: u8 ep  - the endpoint to be configured
 *        u8 dir - the direction to be configured
 **********************************************************************/
 
void usb_configure_endpoint(u8 ep, u8 dir)
{
    volatile BDT_ENTRY* handle;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_configure_endpoint\r\n");
    #endif

    handle = (volatile BDT_ENTRY*) &usb_buffer[EP0_OUT_EVEN];
    //handle += BD(ep, dir, 0) / sizeof(BDT_ENTRY);
    handle += EP(ep, dir, 0);     //Add in offset to the BDT of interest

    handle->STAT.UOWN = 0;

    if (dir == OUT_FROM_HOST)
    {
        pBDTEntryOut[ep] = handle;
    }
    else
    {
        pBDTEntryIn[ep] = handle;
    }

    // USB_PING_PONG__FULL_PING_PONG

    handle->STAT.DTS = 0;
    (handle+1)->STAT.DTS = 1;
}

/***********************************************************************
 * Enable the specified endpoint with the specified options.
 *
 * Input:
 *   u8 ep -       the endpoint to be configured
 *   u8 options -  optional settings for the endpoint. The options should
 *                 be ORed together to form a single options string. The
 *                 available optional settings for the endpoint. The
 *                 options should be ORed together to form a single options
 *                 string. The available options are the following\:
 *                 * USB_HANDSHAKE_ENABLED enables USB handshaking (ACK, NAK)
 *                 * USB_HANDSHAKE_DISABLED disables USB handshaking (ACK, NAK)
 *                 * USB_OUT_ENABLED enables the out direction
 *                 * USB_OUT_DISABLED disables the out direction
 *                 * USB_IN_ENABLED enables the in direction
 *                 * USB_IN_DISABLED disables the in direction
 *                 * USB_ALLOW_SETUP enables control transfers
 *                 * USB_DISALLOW_SETUP disables control transfers
 *                 * USB_STALL_ENDPOINT STALLs this endpoint
 **********************************************************************/
 
void usb_enable_endpoint(u8 ep, u8 options)
{
    // Set the options to the appropriate endpoint control register
    u8* p;
    
    #ifdef DEBUG
    //SerialPrint(UART,"> usb_enable_endpoint\r\n");
    #endif

    if (options & USB_OUT_ENABLED)
    {
        usb_configure_endpoint(ep, OUT_FROM_HOST);
    }
    
    if (options & USB_IN_ENABLED)
    {
        usb_configure_endpoint(ep, IN_TO_HOST);
    }

    p = (u8*) (&U1EP0 + (4 * ep));
    *p = options;
}

/***********************************************************************
 * STALLs the specified endpoint
 *
 * Input:
 *   u8 ep - the endpoint the data will be transmitted on
 *   u8 dir - the direction of the transfer
 **********************************************************************/

#if 0
void usb_stall_endpoint(u8 ep, u8 dir)
{
    BDT_ENTRY *p;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_stall_endpoint\r\n");
    #endif
    
    if (ep == 0)
    {
        /*
         * If no one knows how to service this request then stall.
         * Must also prepare EP0 to receive the next SETUP transaction.
         */
        pBDTEntryEP0OutNext->CNT = USB_EP0_BUFF_SIZE;
        pBDTEntryEP0OutNext->ADR = ConvertToPhysicalAddress(&usb_setup_pkt);
        pBDTEntryEP0OutNext->STAT.Val = _USIE|_DAT0|_DTSEN|_BSTALL;
        pBDTEntryIn[0]->STAT.Val = _USIE|_BSTALL;
    }
    else
    {
        p = (BDT_ENTRY*) &usb_buffer[EP(ep, dir, 0)];
        p->STAT.Val |= _BSTALL | _USIE;

        //If the device is in FULL or ALL_BUT_EP0 ping pong modes
        //then stall that entry as well

        p = (BDT_ENTRY*) &usb_buffer[EP(ep, dir, 1)];
        p->STAT.Val |= _BSTALL | _USIE;
    }
}
#endif

/***********************************************************************
 * Transfers one packet over the USB.
 *
 * Input:
 *   u32 ep - the endpoint the data will be transmitted on
 *   u32 dir - the direction of the transfer
 *                  This value is either OUT_FROM_HOST or IN_TO_HOST
 *   u8* data - pointer to the data to be sent
 *   u32 len - length of the data needing to be sent
 **********************************************************************/
 
USB_HANDLE usb_transfer_one_packet (u8 ep, u8 dir, u8* data, u8 len)
{
    USB_HANDLE handle;

    #ifdef DEBUG
    //SerialPrint(UART,"> usb_transfer_one_packet\r\n");
    #endif
    
    // If the direction is IN
    if (dir == IN_TO_HOST)
    {
        // point to the IN BDT of the specified endpoint
        handle = pBDTEntryIn[ep];
    }
    else
    {
        // else point to the OUT BDT of the specified endpoint
        handle = pBDTEntryOut[ep];
    }

    //Error checking code.
    //Make sure the handle (pBDTEntryIn[ep] or pBDTEntryOut[ep])
    //is initialized before using it.
    if (handle == 0)
    {
        return 0;
    }

    //Set the data pointer, data length, and enable the endpoint
    handle->ADR = ConvertToPhysicalAddress(data);
    handle->CNT = len;
    handle->STAT.Val &= _DTSMASK;
    handle->STAT.Val |= _USIE | _DTSEN;

    // Point to the next buffer for ping pong purposes.
    if (dir == IN_TO_HOST)
    {
        // toggle over the to the next buffer for an IN endpoint
        *(u8*)&pBDTEntryIn[ep] ^= USB_NEXT_PING_PONG;
    }
    else
    {
        // toggle over the to the next buffer for an OUT endpoint
        *(u8*)&pBDTEntryOut[ep] ^= USB_NEXT_PING_PONG;
    }
    
    return handle;
}

#endif /* USBDEVICE_C */
