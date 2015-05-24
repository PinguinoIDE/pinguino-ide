/*
** SDMMC.h SD card interface 
** 
** 07/20/07 v1.4 LDJ
** 11/22/07 v1.5 LDJ
**
*/

#ifndef __SDMMC_H__
#define __SDMMC_H__

#define FAIL    FALSE

/*
				PIC32MX4xx
FUNCTION		SPI1 / SPI2
SCK			RD10 / RG6  = D13
SDI			RC4  / RG7  = D12
SDO	 		RD0  / RG8  = D11
CS				RB1  / RG9  = D10
CD				RF0  / NA
WD				RF1  / NA

TO CHECK: When using PIC32MX4xx (PIC32-Pinguino), the SPI2 port cannot be chosen
or the code must be modified to ignore the CD and WD inputs.
*/

// SD card commands
#define RESET           0 // a.k.a. GO_IDLE (CMD0)
#define INIT            1 // a.k.a. SEND_OP_COND (CMD1)
#define READ_SINGLE     17
#define WRITE_SINGLE    24

// additional commands (not used)
#define SEND_CSD        9
#define SEND_CID        10
#define SET_BLEN        16
#define APP_CMD         55
#define SEND_APP_OP     41 // a.k.a. APP_SEND_OP_COND (ACMD41)

// SD card responses
#define DATA_START      0xFE
#define DATA_ACCEPT     0x05

// timeouts
#define I_TIMEOUT       10000   
#define R_TIMEOUT       25000
#define W_TIMEOUT       250000

// Init ERROR code definitions
#define E_COMMAND_ACK       0x80
#define E_INIT_TIMEOUT      0x81


typedef unsigned LBA;  // logic block address, 32 bit wide

void initSD(void);		// initializes I/O pins and SPI
void disableSD(void);	// deselect SD card
void enableSD(void);		// select SD card

int initMedia(void);   // initializes the SD/MMC memory device

int getCD();            // check card presence
int getWP();            // check write protection tab

int readSECTOR(LBA, char *);  // reads a block of data 
int writeSECTOR(LBA, char *);  // writes a block of data

#endif /* __SDMMC_H__ */

