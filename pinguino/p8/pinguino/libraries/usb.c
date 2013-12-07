// USB function for Pinguino
// Jean-Pierre MANDON 2008
// modified 08/2010 usbavailable function, USBRX buffer size


// USB

#ifndef __PINGUINOUSB
#define __PINGUINOUSB

//#if defined(boot2)

#include <pic18fregs.h>
#include <typedef.h>
#include <usb.h>

#define __USB__

//
//  RB 12/11/2013 : added some other PIC support
//

#if   defined(__18f14k50) || defined(__18f14k50)  // Bank 2
    volatile BufferDescriptorTable __at (0x200) ep_bdt[32];
#elif defined(__18f26j53) || defined(__18f46j53) || \
      defined(__18f27j53) || defined(__18f47j53)  // Bank 13
    volatile BufferDescriptorTable __at (0xD00) ep_bdt[32];
#else                                             // Bank 4
    volatile BufferDescriptorTable __at (0x400) ep_bdt[32];
#endif

// Size of the power EP application buffer
#define EP1_BUFFER_SIZE 64
#define EP2_BUFFER_SIZE 64

#pragma udata usb_buf TXBuffer
volatile u8 TXBuffer[EP2_BUFFER_SIZE];
#pragma udata usb_buf RXbuffer
volatile u8 RXbuffer[EP1_BUFFER_SIZE];

// pinguino usb buffer
#define USBRXSIZE 64		// size of usb rx buffer
u8 usbrx[USBRXSIZE];		// usb rx buffer pinguino
u8 usbwp,usbrp;		// pointer for USB buffer pinguino

void dispatch_usb_event(void);
void fill_in_buffer(u8 EPnum,
                    u8 **source,
                    u16 buffer_size, 
                    u16 *nb_byte);

static u8 last_send_was_null;
u16 ep2_num_bytes_to_send;
u8 *ep2_source_data;

// RB 05-02-2013
void usb_init(void)
{
    // Enable Interrupt
    #if defined(__18f25k50) || defined(__18f45k50)
        PIR3bits.USBIF = 0;     // clear usb interrupt flag
        PIE3bits.USBIE = 1;     // enable usb interrupt
        IPR3bits.USBIP = 1;     // high priority interrupt
    #else
        PIR2bits.USBIF = 0;     // clear usb interrupt flag
        PIE2bits.USBIE = 1;     // enable usb interrupt
        IPR2bits.USBIP = 1;     // high priority interrupt
    #endif
    INTCONbits.GIEH = 1;   // Enable global HP interrupts
    INTCONbits.GIEL = 1;   // Enable global LP interrupts
}

u8 usbavailable(void)
{
	return (usbwp!=usbrp);
}

// send to host

void epapin_init(void)
{
    EP_IN_BD(2).Stat.uc = BDS_UCPU | BDS_DAT1 | BDS_DTSEN;
    UEP2 = EPHSHK_EN | EPINEN_EN |  EPCONDIS_EN;
}

void prepare_ep2_in(void)
{
    last_send_was_null = (ep2_num_bytes_to_send < EP2_BUFFER_SIZE);
    EP_IN_BD(2).Cnt = ep2_num_bytes_to_send;
    EP_IN_BD(2).ADR = (u8 __data *)&TXBuffer;
    fill_in_buffer(2, &ep2_source_data, EP2_BUFFER_SIZE, &ep2_num_bytes_to_send);
    if(EP_IN_BD(2).Stat.DTS == 0)
    {
        EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
    }
    else
    {
        EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    }
}

void usbsend(u8 *txpointer, u8 length)
{
    ep2_source_data=txpointer;
    ep2_num_bytes_to_send=length;
    prepare_ep2_in();
}

// this function doesn't work on OSX
/*
void usbsendint(int valeur)
{
    ep2_source_data=&valeur;
    ep2_num_bytes_to_send=2;
    prepare_ep2_in();
}
*/

void epap_in(void)
{
    if((ep2_num_bytes_to_send == 0) && last_send_was_null)
	{
        return;
    }
    last_send_was_null = (ep2_num_bytes_to_send < EP2_BUFFER_SIZE);
    EP_IN_BD(2).Cnt = ep2_num_bytes_to_send;
    
    /* import this from bootloader v2.12 ? */ 
    fill_in_buffer(2, &ep2_source_data, EP2_BUFFER_SIZE, &ep2_num_bytes_to_send);
    
    if(EP_IN_BD(2).Stat.DTS == 0)
	{
        EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
    }
    else
    {
        EP_IN_BD(2).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    }
}

// receive from host

u8 usbread()
{
    u8 caractere=0;

    if (usbavailable)
    {
        caractere=usbrx[usbrp++];
        if (usbrp==USBRXSIZE)
            usbrp=1;
    }
    return(caractere);
}

u8 usbreceive(u8 *rxpointer)
{
    u8 rp = 0;
	while (usbavailable())
	{
	    rxpointer[rp++]= usbrx[usbrp++];
        if (usbrp==USBRXSIZE)
            usbrp=1;
	}
	return(rp);
}

void epap_out(void)
{
    u8 cpt;
    u8 newusbwp;    
    u8 caractere;

    if((EP_OUT_BD(1).Cnt >= 1) && ( EP_OUT_BD(1).Cnt<EP1_BUFFER_SIZE))
	{
        for (cpt=0;cpt<EP_OUT_BD(1).Cnt;cpt++)
        {
            caractere=RXbuffer[cpt];
            if (usbwp!=USBRXSIZE-1) newusbwp=usbwp+1;               
                else newusbwp=1;
            if (usbrp!=newusbwp) usbrx[usbwp++]=caractere;
                if (usbwp==USBRXSIZE) usbwp=1;
        }
        EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
        if(EP_OUT_BD(1).Stat.DTS == 0)
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT1 | BDS_DTSEN;
        }
        else
        {
            EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
        }
    }
    else // Raise an error
    {
        EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
        EP_OUT_BD(1).ADR = (u8 __data *)&RXbuffer;
        EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_BSTALL;
    }
}

void epapout_init(void)
{
    EP_OUT_BD(1).Cnt = EP1_BUFFER_SIZE;
    EP_OUT_BD(1).ADR = (u8 __data *)&RXbuffer;
    EP_OUT_BD(1).Stat.uc = BDS_USIE | BDS_DAT0 | BDS_DTSEN;
    UEP1 = EPHSHK_EN | EPOUTEN_EN | EPCONDIS_EN;       	// Init EPAPPLI as an OUT EP
    usbwp=1;						// init pointer for rx buffer pinguino
    usbrp=1;
}

void usb_interrupt(void)
{
    #if defined(__18f25k50) || defined(__18f45k50)
    if(PIR3bits.USBIF)
    #else
    if(PIR2bits.USBIF)
    #endif
    {
        dispatch_usb_event();
        UIRbits.SOFIF = 0;
        UIRbits.URSTIF = 0;
        PIR2bits.USBIF = 0;
        UEIR = 0;
    }
}

void usbputs(u8 *txpointer, u8 length)
{
    usbsend(txpointer, length);
}

u8 usbgets(void)
{
    return(usbread()); // one only character is returned
}

//#endif /* boot2 */

#endif /* __PINGUINOUSB */
