/*
** SDMMC.h SD card interface
**
** 07/20/07 v1.4 LDJ
** 11/22/07 v1.5 LDJ
**
** [29-03-12][hgmvanbeek@gmail.com][defined PIC32-PINGUINO-MICRO pinning]
** [30-03-12][hgmvanbeek@gmail.com][Yellow Led indicates SD activity ]
** 07 May 2012	As part of providing support for PIC32 Pinguino Micro, and
					potentially other boards, remmed out board specific definitions
					of SDCS, READ_LED and WRITE_LED.  SDCS is now set via the
					SD.mount() function.  For the PIC32 Pinguino, the PIC32 Pinguino
					OTG and the PIC32 Pinguino Micro, LED1, the green LED is
					connected to the SCK2 line so the green LED flashes, all be it
					very briefly every time a card is accessed.
					Previous lines have been left in for reference
					and also as a prototype if boards with Card Detect or Write 
					Protect are added in future.
** 26 May 2012 Minor bug fix - added two missing function prototypes
*/

#ifndef __SDMMC_H__
#define __SDMMC_H__

#include <typedef.h>

// SD card commands
#define RESET           0           // a.k.a. GO_IDLE (CMD0)
#define INIT            1           // a.k.a. SEND_OP_COND (CMD1)
#define READ_SINGLE     17
#define WRITE_SINGLE    24

// additional commands (not used)
#define SEND_CSD        9
#define SEND_CID        10
#define SET_BLEN        16
#define APP_CMD         55
#define SEND_APP_OP     41          // a.k.a. APP_SEND_OP_COND (ACMD41)

// SD card responses
#define DATA_START      0xFE
#define DATA_ACCEPT     0x05

// timeouts
#define I_TIMEOUT       20000
#define R_TIMEOUT       25000
#define W_TIMEOUT       250000

// Init ERROR code definitions
#define E_COMMAND_ACK       0x80
#define E_INIT_TIMEOUT      0x81

typedef unsigned LBA;               // logic block address, 32 bit wide

// Function prototypes
//void initSD(u8);                    // initializes I/O pins and SPI
void disableSD(u8);                 // deselect SD card
void enableSD(u8);                  // select SD card
int sendSDCmd(u8, u8, unsigned);    // send command to SD card
//int initMedia(u8);                  // initializes the SD/MMC memory device
int readSECTOR(u8, LBA, char *);    // reads a block of data
int writeSECTOR(u8, LBA, char *);   // writes a block of data
int getCD(u8);                      // check card presence
int getWP(u8);                      // check write protection tab

#endif /* __SDMMC_H__ */

