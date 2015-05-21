/*

Copyright (c) 2010, Embedded Adventures, www.embeddedadventures.com
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

- Redistributions of source code must retain the above copyright notice,
  this list of conditions and the following disclaimer.

- Redistributions in binary form must reproduce the above copyright
  notice, this list of conditions and the following disclaimer in the
  documentation and/or other materials provided with the distribution.

- Neither the name of Embedded Adventures nor the names of its contributors
  may be used to endorse or promote products derived from this software
  without specific prior written permission.
 
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
THE POSSIBILITY OF SUCH DAMAGE.

Contact us at admin@embeddedadventures.com

*/

/*! \file mrf24j40_defines.h
    \brief Defines for MRF24J40 chip - generated from the datasheet
	
*/

#ifndef __MRF24J40_DEFINES_H
#define __MRF24J40_DEFINES_H

#define RXMCR			0x00

	// RXMCR bits
	// Reserved			7
	// Reserved			6
	#define RXMCR_NOACKRSP			5
	// Reserved			4
	#define RXMCR_PANCOORD			3
	#define RXMCR_COORD			2
	#define RXMCR_ERRPKT			1
	#define RXMCR_PROMI			0

#define PANIDL		0x01
#define PANIDH		0x02
#define SADRL		0x03
#define SADRH		0x04
#define EADR0		0x05
#define EADR1		0x06
#define EADR2		0x07
#define EADR3		0x08
#define EADR4		0x09
#define EADR5		0x0A
#define EADR6		0x0B
#define EADR7		0x0C
#define RXFLUSH			0x0D

	// RXFLUSH bits
	// Reserved			7
	#define RXFLUSH_WAKEPOL			6
	#define RXFLUSH_WAKEPAD			5
	// Reserved			4
	#define RXFLUSH_CMDONLY			3
	#define RXFLUSH_DATAONLY			2
	#define RXFLUSH_BCNONLY			1
	#define RXFLUSH_RXFLUSH			0

// 0x0E - Reserved
// 0x0F - Reserved
#define ORDER			0x10

	// ORDER bits
	#define ORDER_BO3			7
	#define ORDER_BO2			6
	#define ORDER_BO1			5
	#define ORDER_BO0			4
	#define ORDER_SO3			3
	#define ORDER_SO2			2
	#define ORDER_SO1			1
	#define ORDER_SO0			0

#define TXMCR			0x11

	// TXMCR bits
	#define TXMCR_NOCSMA			7
	#define TXMCR_BATLIFEXT			6
	#define TXMCR_SLOTTED			5
	#define TXMCR_MACMINBE1			4
	#define TXMCR_MACMINBE0			3
	#define TXMCR_CSMABF2			2
	#define TXMCR_CSMABF1			1
	#define TXMCR_CSMABF0			0

#define ACKTMOUT			0x12

	// ACKTMOUT bits
	#define ACKTMOUT_DRPACK			7
	#define ACKTMOUT_MAWD6			6
	#define ACKTMOUT_MAWD5			5
	#define ACKTMOUT_MAWD4			4
	#define ACKTMOUT_MAWD3			3
	#define ACKTMOUT_MAWD2			2
	#define ACKTMOUT_MAWD1			1
	#define ACKTMOUT_MAWD0			0

#define ESLOTG1		0x13
#define SYMTICKL			0x14

	// SYMTICKL bits
	#define SYMTICKL_TICKP7			7
	#define SYMTICKL_TICKP6			6
	#define SYMTICKL_TICKP5			5
	#define SYMTICKL_TICKP4			4
	#define SYMTICKL_TICKP3			3
	#define SYMTICKL_TICKP2			2
	#define SYMTICKL_TICKP1			1
	#define SYMTICKL_TICKP0			0

#define SYMTICKH			0x15

	// SYMTICKH bits
	#define SYMTICKH_TXONT6			7
	#define SYMTICKH_TXONT5			6
	#define SYMTICKH_TXONT4			5
	#define SYMTICKH_TXONT3			4
	#define SYMTICKH_TXONT2			3
	#define SYMTICKH_TXONT1			2
	#define SYMTICKH_TXONT0			1
	#define SYMTICKH_TICKP8			0

#define PACON0			0x16

	// PACON0 bits
	#define PACON0_PAONT7			7
	#define PACON0_PAONT6			6
	#define PACON0_PAONT5			5
	#define PACON0_PAONT4			4
	#define PACON0_PAONT3			3
	#define PACON0_PAONT2			2
	#define PACON0_PAONT1			1
	#define PACON0_PAONT0			0

#define PACON1			0x17

	// PACON1 bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	#define PACON1_PAONTS3			4
	#define PACON1_PAONTS2			3
	#define PACON1_PAONTS1			2
	#define PACON1_PAONTS0			1
	#define PACON1_PAONT8			0

#define PACON2			0x18

	// PACON2 bits
	#define PACON2_FIFOEN			7
	// Reserved			6
	#define PACON2_TXONTS3			5
	#define PACON2_TXONTS2			4
	#define PACON2_TXONTS1			3
	#define PACON2_TXONTS0			2
	#define PACON2_TXONT8			1
	#define PACON2_TXONT7			0

// 0x19 - Reserved
#define TXBCON0			0x1A

	// TXBCON0 bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	#define TXBCON0_TXBSECEN			1
	#define TXBCON0_TXBTRIG			0

#define TXNCON			0x1B

	// TXNCON bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	#define TXNCON_FPSTAT			4
	#define TXNCON_INDIRECT			3
	#define TXNCON_TXNACKREQ			2
	#define TXNCON_TXNSECEN			1
	#define TXNCON_TXNTRIG			0

#define TXG1CON			0x1C

	// TXG1CON bits
	#define TXG1CON_TXG1RETRY1			7
	#define TXG1CON_TXG1RETRY0			6
	#define TXG1CON_TXG1SLOT2			5
	#define TXG1CON_TXG1SLOT1			4
	#define TXG1CON_TXG1SLOT0			3
	#define TXG1CON_TXG1ACKREQ			2
	#define TXG1CON_TXG1SECEN			1
	#define TXG1CON_TXG1TRIG			0

#define TXG2CON			0x1D

	// TXG2CON bits
	#define TXG2CON_TXG2RETRY1			7
	#define TXG2CON_TXG2RETRY0			6
	#define TXG2CON_TXG2SLOT2			5
	#define TXG2CON_TXG2SLOT1			4
	#define TXG2CON_TXG2SLOT0			3
	#define TXG2CON_TXG2ACKREQ			2
	#define TXG2CON_TXG2SECEN			1
	#define TXG2CON_TXG2TRIG			0

#define ESLOTG23		0x1E
#define ESLOTG45		0x1F
#define ESLOTG67		0x20
#define TXPEND			0x21

	// TXPEND bits
	#define TXPEND_MLIFS5			7
	#define TXPEND_MLIFS4			6
	#define TXPEND_MLIFS3			5
	#define TXPEND_MLIFS2			4
	#define TXPEND_MLIFS1			3
	#define TXPEND_MLIFS0			2
	#define TXPEND_GTSSWITCH			1
	#define TXPEND_FPACK			0

#define WAKECON			0x22

	// WAKECON bits
	#define WAKECON_IMMWAKE			7
	#define WAKECON_REGWAKE			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define FRMOFFSET			0x23

	// FRMOFFSET bits
	#define FRMOFFSET_OFFSET7			7
	#define FRMOFFSET_OFFSET6			6
	#define FRMOFFSET_OFFSET5			5
	#define FRMOFFSET_OFFSET4			4
	#define FRMOFFSET_OFFSET3			3
	#define FRMOFFSET_OFFSET2			2
	#define FRMOFFSET_OFFSET1			1
	#define FRMOFFSET_OFFSET0			0

#define TXSTAT			0x24

	// TXSTAT bits
	#define TXSTAT_TXNRETRY1			7
	#define TXSTAT_TXNRETRY0			6
	#define TXSTAT_CCAFAIL			5
	#define TXSTAT_TXG2FNT			4
	#define TXSTAT_TXG1FNT			3
	#define TXSTAT_TXG2STAT			2
	#define TXSTAT_TXG1STAT			1
	#define TXSTAT_TXNSTAT			0

#define TXBCON1		0x25
#define GATECLK			0x26

	// GATECLK bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	#define GATECLK_GTSON			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define TXTIME			0x27

	// TXTIME bits
	#define TXTIME_TURNTIME3			7
	#define TXTIME_TURNTIME2			6
	#define TXTIME_TURNTIME1			5
	#define TXTIME_TURNTIME0			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define HSYMTMRL			0x28

	// HSYMTMRL bits
	#define HSYMTMRL_HSYMTMR7			7
	#define HSYMTMRL_HSYMTMR6			6
	#define HSYMTMRL_HSYMTMR5			5
	#define HSYMTMRL_HSYMTMR4			4
	#define HSYMTMRL_HSYMTMR3			3
	#define HSYMTMRL_HSYMTMR2			2
	#define HSYMTMRL_HSYMTMR1			1
	#define HSYMTMRL_HSYMTMR0			0

#define HSYMTMRH			0x29

	// HSYMTMRH bits
	#define HSYMTMRH_HSYMTMR15			7
	#define HSYMTMRH_HSYMTMR14			6
	#define HSYMTMRH_HSYMTMR13			5
	#define HSYMTMRH_HSYMTMR12			4
	#define HSYMTMRH_HSYMTMR11			3
	#define HSYMTMRH_HSYMTMR10			2
	#define HSYMTMRH_HSYMTMR09			1
	#define HSYMTMRH_HSYMTMR08			0

#define SOFTRST			0x2A

	// SOFTRST bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	#define SOFTRST_RSTPWR			2
	#define SOFTRST_RSTBB			1
	#define SOFTRST_RSTMAC			0

// 0x2B - Reserved
#define SECCON0			0x2C

	// SECCON0 bits
	#define SECCON0_SECIGNORE			7
	#define SECCON0_SECSTART			6
	#define SECCON0_RXCIPHER2			5
	#define SECCON0_RXCIPHER1			4
	#define SECCON0_RXCIPHER0			3
	#define SECCON0_TXNCIPHER2			2
	#define SECCON0_TXNCIPHER1			1
	#define SECCON0_TXNCIPHER0			0

#define SECCON1			0x2D

	// SECCON1 bits
	// Reserved			7
	#define SECCON1_TXBCIPHER2			6
	#define SECCON1_TXBCIPHER1			5
	#define SECCON1_TXBCIPHER0			4
	// Reserved			3
	// Reserved			2
	#define SECCON1_DISDEC			1
	#define SECCON1_DISENC			0

#define TXSTBL			0x2E

	// TXSTBL bits
	#define TXSTBL_RFSTBL3			7
	#define TXSTBL_RFSTBL2			6
	#define TXSTBL_RFSTBL1			5
	#define TXSTBL_RFSTBL0			4
	#define TXSTBL_MSIFS3			3
	#define TXSTBL_MSIFS2			2
	#define TXSTBL_MSIFS1			1
	#define TXSTBL_MSIFS0			0

// 0x2F - Reserved
#define RXSR			0x30

	// RXSR bits
	// Reserved			7
	#define RXSR_UPSECERR			6
	#define RXSR_BATIND			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

// INTERRUPT STATUS REGISTER (ADDRESS: 0x31)
#define INTSTAT			0x31

	// INTSTAT bits
	#define INTSTAT_SLPIF			7
	#define INTSTAT_WAKEIF			6
	#define INTSTAT_HSYMTMRIF		5
	#define INTSTAT_SECIF			4
	#define INTSTAT_RXIF				3
	#define INTSTAT_TXG2IF			2
	#define INTSTAT_TXG1IF			1
	#define INTSTAT_TXNIF			0

#define MRF_INTCON			0x32

	// INTCON bits
	#define MRF_INTCON_SLPIE			7
	#define MRF_INTCON_WAKEIE			6
	#define MRF_INTCON_HSYMTMRIE			5
	#define MRF_INTCON_SECIE			4
	#define MRF_INTCON_RXIE			3
	#define MRF_INTCON_TXG2IE			2
	#define MRF_INTCON_TXG1IE			1
	#define MRF_INTCON_TXNIE			0

#define GPIO			0x33

	// GPIO bits
	// Reserved			7
	// Reserved			6
	#define GPIO_GPIO5			5
	#define GPIO_GPIO4			4
	#define GPIO_GPIO3			3
	#define GPIO_GPIO2			2
	#define GPIO_GPIO1			1
	#define GPIO_GPIO0			0

#define TRISGPIO			0x34

	// TRISGPIO bits
	// Reserved			7
	// Reserved			6
	#define TRISGPIO_TRISGP5			5
	#define TRISGPIO_TRISGP4			4
	#define TRISGPIO_TRISGP3			3
	#define TRISGPIO_TRISGP2			2
	#define TRISGPIO_TRISGP1			1
	#define TRISGPIO_TRISGP0			0

#define SLPACK			0x35

	// SLPACK bits
	#define SLPACK_SLPACK			7
	#define SLPACK_WAKECNT6			6
	#define SLPACK_WAKECNT5			5
	#define SLPACK_WAKECNT4			4
	#define SLPACK_WAKECNT3			3
	#define SLPACK_WAKECNT2			2
	#define SLPACK_WAKECNT1			1
	#define SLPACK_WAKECNT0			0

#define RFCTL			0x36

	// RFCTL bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	#define RFCTL_WAKECNT8			4
	#define RFCTL_WAKECNT7			3
	#define RFCTL_RFRST			2
	// Reserved			1
	// Reserved			0

#define SECCR2			0x37

	// SECCR2 bits
	#define SECCR2_UPDEC			7
	#define SECCR2_UPENC			6
	#define SECCR2_TXG2CIPHER2			5
	#define SECCR2_TXG2CIPHER1			4
	#define SECCR2_TXG2CIPHER0			3
	#define SECCR2_TXG1CIPHER2			2
	#define SECCR2_TXG1CIPHER1			1
	#define SECCR2_TXG1CIPHER0			0

#define BBREG0			0x38

	// BBREG0 bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	#define BBREG0_TURBO			0

#define BBREG1			0x39

	// BBREG1 bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	#define BBREG1_RXDECINV			2
	// Reserved			1
	// Reserved			0

#define BBREG2			0x3A

	// BBREG2 bits
	#define BBREG2_CCAMODE1			7
	#define BBREG2_CCAMODE0			6
	#define BBREG2_CCACSTH3			5
	#define BBREG2_CCACSTH2			4
	#define BBREG2_CCACSTH1			3
	#define BBREG2_CCACSTH0			2
	// Reserved			1
	// Reserved			0

#define BBREG3			0x3B

	// BBREG3 bits
	#define BBREG3_PREVALIDTH3			7
	#define BBREG3_PREVALIDTH2			6
	#define BBREG3_PREVALIDTH1			5
	#define BBREG3_PREVALIDTH0			4
	#define BBREG3_PREDETTH2			3
	#define BBREG3_PREDETTH1			2
	#define BBREG3_PREDETTH0			1
	// Reserved			0

#define BBREG4			0x3C

	// BBREG4 bits
	#define BBREG4_CSTH2			7
	#define BBREG4_CSTH1			6
	#define BBREG4_CSTH0			5
	#define BBREG4_PRECNT2			4
	#define BBREG4_PRECNT1			3
	#define BBREG4_PRECNT0			2
	// Reserved			1
	// Reserved			0

// 0x3D - Reserved
#define BBREG6			0x3E

	// BBREG6 bits
	#define BBREG6_RSSIMODE1			7
	#define BBREG6_RSSIMODE2			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	#define BBREG6_RSSIRDY			0

#define CCAEDTH			0x3F

	// CCAEDTH bits
	#define CCAEDTH_CCAEDTH7			7
	#define CCAEDTH_CCAEDTH6			6
	#define CCAEDTH_CCAEDTH5			5
	#define CCAEDTH_CCAEDTH4			4
	#define CCAEDTH_CCAEDTH3			3
	#define CCAEDTH_CCAEDTH2			2
	#define CCAEDTH_CCAEDTH1			1
	#define CCAEDTH_CCAEDTH0			0

#define RFCON0			0x200

	// RFCON0 bits
	#define RFCON0_CHANNEL3			7
	#define RFCON0_CHANNEL2			6
	#define RFCON0_CHANNEL1			5
	#define RFCON0_CHANNEL0			4
	#define RFCON0_RFOPT3			3
	#define RFCON0_RFOPT2			2
	#define RFCON0_RFOPT1			1
	#define RFCON0_RFOPT0			0

#define RFCON1			0x201

	// RFCON1 bits
	#define RFCON1_VCOOPT7			7
	#define RFCON1_VCOOPT6			6
	#define RFCON1_VCOOPT5			5
	#define RFCON1_VCOOPT4			4
	#define RFCON1_VCOOPT3			3
	#define RFCON1_VCOOPT2			2
	#define RFCON1_VCOOPT1			1
	#define RFCON1_VCOOPT0			0

#define RFCON2			0x202

	// RFCON2 bits
	#define RFCON2_PLLEN			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define RFCON3			0x203

	// RFCON3 bits
	#define RFCON3_TXPWRL1			7
	#define RFCON3_TXPWRL0			6
	#define RFCON3_TXPWRS2			5
	#define RFCON3_TXPWRS1			4
	#define RFCON3_TXPWRS0			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

// 0x204 - Reserved
#define RFCON5			0x205

	// RFCON5 bits
	#define RFCON5_BATTH3			7
	#define RFCON5_BATTH2			6
	#define RFCON5_BATTH1			5
	#define RFCON5_BATTH0			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define RFCON6			0x206

	// RFCON6 bits
	#define RFCON6_TXFIL			7
	// Reserved			6
	// Reserved			5
	#define RFCON6_20MRECVR			4
	#define RFCON6_BATEN			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define RFCON7			0x207

	// RFCON7 bits
	#define RFCON7_SLPCLKSEL1			7
	#define RFCON7_SLPCLKSEL0			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	#define RFCON7_CLKOUTMODE1			1
	#define RFCON7_CLKOUTMODE0			0

#define RFCON8			0x208

	// RFCON8 bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	#define RFCON8_RFVCO			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define SLPCAL0			0x209

	// SLPCAL0 bits
	#define SLPCAL0_SLPCAL7			7
	#define SLPCAL0_SLPCAL6			6
	#define SLPCAL0_SLPCAL5			5
	#define SLPCAL0_SLPCAL4			4
	#define SLPCAL0_SLPCAL3			3
	#define SLPCAL0_SLPCAL2			2
	#define SLPCAL0_SLPCAL1			1
	#define SLPCAL0_SLPCAL0			0

#define SLPCAL1			0x20A

	// SLPCAL1 bits
	#define SLPCAL1_SLPCAL15			7
	#define SLPCAL1_SLPCAL14			6
	#define SLPCAL1_SLPCAL13			5
	#define SLPCAL1_SLPCAL12			4
	#define SLPCAL1_SLPCAL11			3
	#define SLPCAL1_SLPCAL10			2
	#define SLPCAL1_SLPCAL9			1
	#define SLPCAL1_SLPCAL8			0

#define SLPCAL2			0x20B

	// SLPCAL2 bits
	#define SLPCAL2_SLPCALRDY			7
	// Reserved			6
	// Reserved			5
	#define SLPCAL2_SLPCALEN			4
	#define SLPCAL2_SLPCAL19			3
	#define SLPCAL2_SLPCAL18			2
	#define SLPCAL2_SLPCAL17			1
	#define SLPCAL2_SLPCAL16			0

// 0x20C - Reserved
// 0x20D - Reserved
// 0x20E - Reserved
#define RFSTATE			0x20F

	// RFSTATE bits
	#define RFSTATE_RFSTATE2			7
	#define RFSTATE_RFSTATE1			6
	#define RFSTATE_RFSTATE0			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	// Reserved			1
	// Reserved			0

#define RSSI			0x210

	// RSSI bits
	#define RSSI_RSSI7			7
	#define RSSI_RSSI6			6
	#define RSSI_RSSI5			5
	#define RSSI_RSSI4			4
	#define RSSI_RSSI3			3
	#define RSSI_RSSI2			2
	#define RSSI_RSSI1			1
	#define RSSI_RSSI0			0

#define SLPCON0			0x211

	// SLPCON0 bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	#define SLPCON0_INTEDGE			1
	#define SLPCON0_SLPCLKEN			0


// 0x212 - Reserved
// 0x213 - Reserved
// 0x214 - Reserved
// 0x215 - Reserved
// 0x216 - Reserved
// 0x217 - Reserved
// 0x218 - Reserved
// 0x219 - Reserved
// 0x21A - Reserved
// 0x21B - Reserved
// 0x21C - Reserved
// 0x21D - Reserved
// 0x21E - Reserved
// 0x21F - Reserved
#define SLPCON1			0x220

	// SLPCON1 bits
	// Reserved			7
	// Reserved			6
	#define SLPCON1_CLKOUTEN			5
	#define SLPCON1_SLPCLKDIV4			4
	#define SLPCON1_SLPCLKDIV3			3
	#define SLPCON1_SLPCLKDIV2			2
	#define SLPCON1_SLPCLKDIV1			1
	#define SLPCON1_SLPCLKDIV0			0

// 0x221 - Reserved
#define WAKETIMEL			0x222

	// WAKETIMEL bits
	#define WAKETIMEL_WAKETIME7			7
	#define WAKETIMEL_WAKETIME6			6
	#define WAKETIMEL_WAKETIME5			5
	#define WAKETIMEL_WAKETIME4			4
	#define WAKETIMEL_WAKETIME3			3
	#define WAKETIMEL_WAKETIME2			2
	#define WAKETIMEL_WAKETIME1			1
	#define WAKETIMEL_WAKETIME0			0

#define WAKETIMEH			0x223

	// WAKETIMEH bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	#define WAKETIMEH_WAKETIME10			2
	#define WAKETIMEH_WAKETIME9			1
	#define WAKETIMEH_WAKETIME8			0

#define REMCNTL			0x224

	// REMCNTL bits
	#define REMCNTL_REMCNT7			7
	#define REMCNTL_REMCNT6			6
	#define REMCNTL_REMCNT5			5
	#define REMCNTL_REMCNT4			4
	#define REMCNTL_REMCNT3			3
	#define REMCNTL_REMCNT2			2
	#define REMCNTL_REMCNT1			1
	#define REMCNTL_REMCNT0			0

#define REMCNTH			0x225

	// REMCNTH bits
	#define REMCNTH_REMCNT15			7
	#define REMCNTH_REMCNT14			6
	#define REMCNTH_REMCNT13			5
	#define REMCNTH_REMCNT12			4
	#define REMCNTH_REMCNT11			3
	#define REMCNTH_REMCNT10			2
	#define REMCNTH_REMCNT9			1
	#define REMCNTH_REMCNT8			0

#define MAINCNT0			0x226

	// MAINCNT0 bits
	#define MAINCNT0_MAINCNT7			7
	#define MAINCNT0_MAINCNT6			6
	#define MAINCNT0_MAINCNT5			5
	#define MAINCNT0_MAINCNT4			4
	#define MAINCNT0_MAINCNT3			3
	#define MAINCNT0_MAINCNT2			2
	#define MAINCNT0_MAINCNT1			1
	#define MAINCNT0_MAINCNT0			0

#define MAINCNT1			0x227

	// MAINCNT1 bits
	#define MAINCNT1_MAINCNT15			7
	#define MAINCNT1_MAINCNT14			6
	#define MAINCNT1_MAINCNT13			5
	#define MAINCNT1_MAINCNT12			4
	#define MAINCNT1_MAINCNT11			3
	#define MAINCNT1_MAINCNT10			2
	#define MAINCNT1_MAINCNT9			1
	#define MAINCNT1_MAINCNT8			0

#define MAINCNT2			0x228

	// MAINCNT2 bits
	#define MAINCNT2_MAINCNT23			7
	#define MAINCNT2_MAINCNT22			6
	#define MAINCNT2_MAINCNT21			5
	#define MAINCNT2_MAINCNT20			4
	#define MAINCNT2_MAINCNT19			3
	#define MAINCNT2_MAINCNT18			2
	#define MAINCNT2_MAINCNT17			1
	#define MAINCNT2_MAINCNT16			0

#define MAINCNT3			0x229

	// MAINCNT3 bits
	#define MAINCNT3_STARTCNT			7
	// Reserved			6
	// Reserved			5
	// Reserved			4
	// Reserved			3
	// Reserved			2
	#define MAINCNT3_MAINCNT25			1
	#define MAINCNT3_MAINCNT24			0

// 0x22A - Reserved
// 0x22B - Reserved
// 0x22C - Reserved
// 0x22D - Reserved
// 0x22E - Reserved
#define TESTMODE			0x22F

	// TESTMODE bits
	// Reserved			7
	// Reserved			6
	// Reserved			5
	#define TESTMODE_RSSIWAIT1			4
	#define TESTMODE_RSSIWAIT0			3
	#define TESTMODE_TESTMODE2			2
	#define TESTMODE_TESTMODE1			1
	#define TESTMODE_TESTMODE0			0

#define ASSOEADR0		0x230
#define ASSOEADR1		0x231
#define ASSOEADR2		0x232
#define ASSOEADR3		0x233
#define ASSOEADR4		0x234
#define ASSOEADR5		0x235
#define ASSOEADR6		0x236
#define ASSOEADR7		0x237
#define ASSOSADR0		0x238
#define ASSOSADR1		0x239
// 0x23A - Reserved
// 0x23B - Reserved
// 0x23C - Unimplemented
// 0x23D - Unimplemented
// 0x23E - Unimplemented
// 0x23F - Unimplemented
#define UPNONCE0		0x240
#define UPNONCE1		0x241
#define UPNONCE2		0x242
#define UPNONCE3		0x243
#define UPNONCE4		0x244
#define UPNONCE5		0x245
#define UPNONCE6		0x246
#define UPNONCE7		0x247
#define UPNONCE8		0x248
#define UPNONCE9		0x249
#define UPNONCE10		0x24A
#define UPNONCE11		0x24B
#define UPNONCE12		0x24C

#endif
