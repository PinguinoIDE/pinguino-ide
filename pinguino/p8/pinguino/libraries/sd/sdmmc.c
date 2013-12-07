/*
** SDMMC.c SD card interface 
** 
** 7/20/06 v1.4 LDJ 
** 7/14/07 v2.0 LDJ 
** 
*/

#ifndef __SDMMC_C__
#define __SDMMC_C__

#include <system.c>
#include <digitalw.c>
#include <pin.h>
#include <sd/sdmmc.h>
#include <sd/fileio.h>

// send one byte of data and receive one back at the same time
unsigned char writeSPI(unsigned char b)
{
	SSPBUF = b;						// write to buffer for TX
	while(!SSPSTATbits.BF);		// wait transfer complete
	return SSPBUF;					// read the received value
}	// writeSPI

void initSD(void)
{
	digitalwrite(SDCS, HIGH);	// initially keep the SD card disabled
	pinmode(SDCS, OUTPUT);		// make Card select an output pin

	// SPI pin names are declared in pins.h
	pinmode(SCK, OUTPUT);		// Master Mode : clock is output
	pinmode(SDI, INPUT);
	pinmode(SDO, OUTPUT);

	// init the SPI module for a slow (safe) clock speed first
	SSPCON1bits.CKP = 0;		// Idle state for clock is a low level
	SSPSTATbits.CKE = 1;		// Transmit occurs on transition from active to Idle clock state
	SSPCON1 = SSPCON1 & 0b11110010; // SSPM<3:0> = 0010 = SPI Master mode, clock = FOSC/64
	SSPCON1bits.SSPEN = 1;	// Enables serial port and configures SCK, SDO, SDI and SS as serial port pins;
}   // initSD

#define readSPI()   writeSPI(0xFF)
#define clockSPI()  writeSPI(0xFF)

void disableSD(void)
{
	digitalwrite(SDCS, HIGH);	// Deselected = SDCS high
	clockSPI();
}

void enableSD(void)
{
	digitalwrite(SDCS, LOW);	// Selected = SDCS low
}

// c    command code
// a    byte address of data block
int sendSDCmd(unsigned char c, unsigned a)
{
	int i, r;

	// enable SD card
	// CS low
	enableSD();

	// send a comand packet (6 bytes)
	writeSPI(c | 0x40);    // send command 
	writeSPI(a>>24);       // msb of the address
	writeSPI(a>>16);       
	writeSPI(a>>8);
	writeSPI(a);           // lsb

	writeSPI(0x95);        // send CMD0 CRC 

	// now wait for a response, allow for up to 8 bytes delay
	for(i=0; i<8; i++) 
	{
		r = readSPI();      
		if (r != 0xFF) 
			break;
	}
	return (r);         

	/* return response
	FF - timeout 
	00 - command accepted
	01 - command received, card in idle state after RESET

	other codes:
	bit 0 = Idle state
	bit 1 = Erase Reset
	bit 2 = Illegal command
	bit 3 = Communication CRC error
	bit 4 = Erase sequence error
	bit 5 = Address error
	bit 6 = Parameter error
	bit 7 = Always 0
	*/
	// NOTE CSCD is still low!
} // sendSDCmd


// returns 0 if successful
//          E_COMMAND_ACK   failed to acknowledge reset command
//          E_INIT_TIMEOUT  failed to initialize
int initMedia(void)
{
	int i, r;

	// 1. with the card NOT selected     
	// Set DI and CS high
	disableSD(); 

	// 2. send 74 or more clock cycles to start up
	// apply 74 or more clock pulses to SCLK.
	// The card will enter its native operating mode and go ready to accept native commands.
	for (i=0; i<10; i++)
		clockSPI();

	// 3. now select the card
	enableSD();

	// 4. send a single RESET command
	// Send a CMD0 with CS low to reset the card.
	r = sendSDCmd(RESET, 0);
	disableSD();
	if (r != 1)                // must return Idle
		return E_COMMAND_ACK;   // comand rejected

	// 5. send repeatedly INIT until Idle terminates
	for (i=0; i<I_TIMEOUT; i++) 
	{
		r = sendSDCmd(INIT, 0);
		disableSD();
		if (!r) 
			break; 
	} 
	if (i == I_TIMEOUT)   
		return E_INIT_TIMEOUT;  // init timed out 

	// 6. increase speed 
	SSPCON1bits.SSPEN = 0;	// Disable SPI
	SSPCON1 = SSPCON1 & 0b11110000; // SSPM<3:0> = 0000 = SPI Master mode, clock = FOSC/4
	SSPCON1bits.SSPEN = 1;	// Enable SPI

	return 0;           
} // init media


// a        LBA of sector requested
// p        pointer to sector buffer
// returns  TRUE if successful
int readSECTOR(LBA a, char *p)
{
	int r, i;

	#ifdef READ_LED    
	digitalwrite(READ_LED, 0);
	#endif

	// 1. send READ command
	r = sendSDCmd(READ_SINGLE, (a << 9));
	if (r == 0)    // check if command was accepted
	{  
	// 2. wait for a response
	for(i=0; i<R_TIMEOUT; i++)
	{
	r = readSPI();     
	if (r == DATA_START) 
	break;
	} 

	// 3. if it did not timeout, read 512 byte of data
	if (i != R_TIMEOUT)
	{
		i = 512;
		do{ 
			*p++ = readSPI();
		} while (--i>0);

		// 4. ignore CRC
		readSPI();
		readSPI();

	} // data arrived

	} // command accepted

	// 5. remember to disable the card
	disableSD();

	#ifdef READLED
	digital(READ_LED, 1);
	#endif

	return (r == DATA_START);    // return TRUE if successful
} // readSECTOR


// a        LBA of sector requested
// p        pointer to sector buffer
// returns  TRUE if successful
int writeSECTOR(LBA a, char *p)
{
	unsigned r, i;

	// 0. check Write Protect
	if (getWP())
		return FAIL;

	// 1. send WRITE command
	r = sendSDCmd(WRITE_SINGLE, (a << 9));
	if (r == 0)    // check if command was accepted
	{  
		// 2. send data
		writeSPI(DATA_START);

		// send 512 bytes of data
		for(i=0; i<512; i++)
		writeSPI(*p++);

		// 3. send dummy CRC
		clockSPI();
		clockSPI();

		// 4. check if data accepted
		r = readSPI(); 
		if ((r & 0xf) == DATA_ACCEPT)
		{   
			#ifdef WRITE_LED    
			digitalwrite(WRITE_LED, 0);
			#endif

			// 5. wait for write completion
			for(i=0; i<W_TIMEOUT; i++)
			{ 
				r = readSPI();
				if (r != 0 )
					break;
			} 
			#ifdef WRITE_LED
			digitalwrite(WRITE_LED, 1);
			#endif
		} // accepted
		else
		{
			r = FAIL;
		}
	} // command accepted

	// 6. disable the card
	disableSD();

	return (r);      // return TRUE if successful
} // writeSECTOR


// SD card connector presence detection switch
// returns  TRUE card present
//          FALSE card not present
int getCD(void) 
{
	return TRUE;
	//return (SDCD);
}

// card Write Protect tab detection switch
// returns  TRUE write protect tab on LOCK
//          FALSE write protection tab OPEN
int getWP(void)
{
	return FALSE;
	//return (SDWP);
}

#endif /* __SDMMC_C__ */
