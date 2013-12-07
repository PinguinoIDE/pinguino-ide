/*	----------------------------------------------------------------------------
	FILE:			i2c.c
	PROJECT:		pinguino
	PURPOSE:		Include all functions to handle I2C communication for Master and Slave
	PROGRAMER:		Régis Blanchot
	FIRST RELEASE:	03 Apr. 2010
	LAST RELEASE:	06 Nov. 2012
	----------------------------------------------------------------------------
	TODO :
    * Slave Mode Managment
    * PIC18F with more than 1 I2C port
    * Arduino Compatibility

	begin() initializes pinguino as an I2C master
	begin(address) initializes pinguino as a slave at address address. 

	done    void beginTransmission(uint8_t);
	done    void beginTransmission(int);

	done    uint8_t endTransmission(void);

	todo	uint8_t requestFrom(uint8_t, uint8_t);
	todo	uint8_t requestFrom(int, int);

	done    void send(uint8_t);
            void send(uint8_t*, uint8_t);
            void send(int);
            void send(char*);

	todo	uint8_t available(void);
	done  	uint8_t receive(void);
	todo	void onReceive( void (*)(int) );
	todo	void onRequest( void (*)(void) );
	----------------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	--------------------------------------------------------------------------*/

#ifndef __PINGUINO_I2C_C
#define __PINGUINO_I2C_C

#define __I2C__

#include <pic18fregs.h>
#include <typedef.h>
#include <macro.h>
#include <const.h>
//#include <delay.c>

#ifdef WIRE
#include <i2cmaster.c>
#endif

//#include <interrupt.c>
#include <stdio.c>
#include <stdarg.h>

// Modules
#define I2C1            1
#define I2C2            2

// Mode I2C
#define I2C_WRITE		0
#define I2C_READ		1
#define I2C_MASTER_MODE	0
#define I2C_SLAVE_MODE	1
#define I2C_SLEW_OFF	0
#define I2C_SLEW_ON		1

// Speed definitions
#define I2C_100KHZ		100
#define I2C_400KHZ		400
#define I2C_1MHZ		1000

#define I2C_idle() while (((SSPCON2 & 0x1F) > 0) | (SSPSTATbits.R_W))

/*
#if defined(__18f2550) || defined(__18f4550) || \
    defined(__18f26j50) || defined(__18f46j50)
#define SSPCONF1 SSPCON1	
#define SSPCONF2 SSPCON2
#define SSPAD    SSPADD
#define SSPBF	 SSPBUF
#define SSPSTSMP SSPSTATbits.SMP
#define SSPSTR_W SSPSTATbits.R_W
#define SSPACKEN SSPCON2bits.ACKEN
#define SSPACKST SSPCON2bits.ACKSTAT
#define SSPACKDT SSPCON2bits.ACKDT
#define SSPPEN SSPCON2bits.PEN
#define SSPRCEN SSPCON2bits.RCEN
#define SSPRSEN SSPCON2bits.RSEN
#define SSPSEN SSPCON2bits.SEN

#elif defined(__18f25k50) || defined(__18f45k50)
#define SSPCONF1 SSP1CON1	
#define SSPCONF2 SSP1CON2	
#define SSPAD    SSP1ADD
#define SSPBF	 SSP1BUF
#define SSPSTSMP SSP1STATbits.SMP
#define SSPSTR_W SSP1STATbits.R_NOT_W
#define SSPACKEN SSP1CON2bits.ACKEN
#define SSPACKST SSP1CON2bits.ACKSTAT
#define SSPACKDT SSP1CON2bits.ACKDT
#define SSPPEN   SSP1CON2bits.PEN
#define SSPRCEN  SSP1CON2bits.RCEN
#define SSPRSEN  SSP1CON2bits.RSEN
#define SSPSEN   SSP1CON2bits.SEN

#else
    #error "Processor Not Yet Supported. Please, Take Contact with Developpers."
#endif
*/

/// PROTOTYPES

void I2C_master(u16);   
void I2C_slave(u16);   
void I2C_init(u8, u16);
u8 I2C_write(u8);
u8 I2C_read();
void I2C_wait();
//void I2C_idle();
//u8 I2C_waitAck();
void I2C_start();
void I2C_stop();
void I2C_restart();
void I2C_sendNack();
void I2C_sendAck();
/*	----------------------------------------------------------------------------
	---------- Initialisation Functions for Master and Slave
	--------------------------------------------------------------------------*/

void I2C_master(u16 speed)   
{
	I2C_init(I2C_MASTER_MODE, speed);
}

void I2C_slave(u16 DeviceID)   
{
	I2C_init(I2C_SLAVE_MODE, DeviceID);
}

/*	----------------------------------------------------------------------------
	---------- Open the I2C bus
	----------------------------------------------------------------------------
	SSPSTAT.SMP: Slew Rate Control bit
	In Master or Slave mode:
	1 = Slew Mode Off = Standard Speed mode (100 kHz and 1 MHz)
	0 = Slew Mode On = High-Speed mode (400 kHz)
    * u8 mode = I2C_MASTER_MODE or I2C_SLAVE_MODE
    * u16 sora = means s(peed) or a(ddress)
               = speed (100, 400 or 1000 KHz) in master mode
               = address in slave mode
	--------------------------------------------------------------------------*/

void I2C_init(u8 mode, u16 sora)
{
    // In Slave mode, the SCL and SDA pins must be configured as inputs
    #if defined(__18f26j50) || defined(__18f46j50) || \
        defined(__18f27j53) || defined(__18f47j53)
    TRISBbits.TRISB5 = INPUT;			// SDA = INPUT
    TRISBbits.TRISB4 = INPUT;			// SCL = INPUT
    #else
    TRISBbits.TRISB0 = INPUT;			// SDA = INPUT
    TRISBbits.TRISB1 = INPUT;			// SCL = INPUT

    #endif

    switch (mode)
    {
        case I2C_SLAVE_MODE:
            //intUsed[INT_SSP] = INT_USED;
            // Enabling interrupts (peripheral & general)
            //INTCONbits.PEIE=1;
            //INTCONbits.GIE=1;
            if (sora > 0x80)
                SSPCON1= 0b00101111;	// Slave mode, 10-bit address with Start and Stop bit interrupts enabled
            else
                SSPCON1= 0b00101110;	// 00101110Slave mode,  7-bit address with Start and Stop bit interrupts enabled
    /*------------------------------------------------------------------------*/
            SSPADD= sora;				// Slave 7-bit address
            // TODO						// Slave 10-bit address
    /*------------------------------------------------------------------------*/
            break;

        case I2C_MASTER_MODE:
        default:// I2C_MASTER_MODE
            SSPCON1= 0b00101000;		// Master Mode, clock = FOSC/(4 * (SSPADD + 1))
            // datasheet p208
            switch (sora)
            {
                case I2C_1MHZ:
                    // SMP = 1 = Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz)
                    SSPSTATbits.SMP = 1;    // Slew Mode Off
                    SSPADD= 11;            // 1MHz = FOSC/(4 * (SSPADD + 1))
                    break;                  // SSPADD = 48 000 / (4*1000) - 1
                    
                case I2C_400KHZ:
                    // SMP = 0 = Slew rate control enabled for High-Speed mode (400 kHz)
                    SSPSTATbits.SMP = 0;    // Slew Mode On
                    SSPADD= 29;            // 400kHz = FOSC/(4 * (SSPADD + 1))
                    break;           // SSPADD = 48 000 / (4*400) - 1
                case I2C_100KHZ:
                default:
                    // SMP = 1 = Slew rate control disabled for Standard Speed mode (100 kHz and 1 MHz)
                    SSPSTATbits.SMP = 1;    // Slew Mode Off
                    SSPADD= 119;           // 100kHz = FOSC/(4 * (SSPADD + 1))
                    break;                  // SSPADD = 48 000 / (4*100) - 1
            }
            break;
    }
    SSPCON2= 0;
    PIR1bits.SSPIF = 0; // MSSP Interrupt Flag
    PIR2bits.BCLIF = 0; // Bus Collision Interrupt Flag
    //Delayms(1000);
}

/*	----------------------------------------------------------------------------
	---------- Send byte and return ack bit
	----------------------------------------------------------------------------
    WCOL: Write Collision Detect bit, in Master Transmit mode:
    1 = A write to the SSPxBUF register was attempted while the I2C conditions
    were not valid for a transmission to be started (must be cleared in software)
    0 = No collision

    BF: Buffer Full Status bit, in Transmit mode:
    1 = SSPxBUF is full
    0 = SSPxBUF is empty

    A slave sends an Acknowledge when it has recognized its address
    (including a general call), or when the slave has properly received its data.
    If the master receives an Acknowledge, ACKSTAT is cleared;
    if not, the bit is set.
    
    !!! ACK is received BEFORE BF is cleared !!!
    (Datasheet 19.5.10 I2C MASTER MODE TRANSMISSION)
    On the falling edge of the ninth clock :
    * the status of the ACK bit is loaded into the ACKSTAT status bit
    * following the SSPxIF flag is set
    * then the BF flag is cleared

    Datasheet, figure 19-23
    --------------------------------------------------------------------------*/

u8 I2C_write(u8 value)
{
    I2C_idle();                     // Wait the MSSP module is inactive
    SSPBUF = value;                 // Write byte to SSPBUF (BF is set to 1)
    I2C_idle();                     // Wait the MSSP module is inactive
/*
    while (SSPCON1bits.WCOL)        // Send again if write collision occurred 
    {
        LATCbits.LATC2 = 1;
        SSPCON1bits.WCOL = 0;       // Must be cleared by software
        SSPBUF = value;             // Write byte to SSPBUF (BF is set to 1)
    }
*/
//    while (SSPSTATbits.BF);         // Wait until buffer is empty (BF set to 0)
    // ACKSTAT can be returned now because it was loaded before BF was cleared
    return (!SSPCON2bits.ACKSTAT);  // 1 if Ack, 0 if NAck
}

/*	----------------------------------------------------------------------------
	---------- Get a byte from the slave
	----------------------------------------------------------------------------
    RCEN = Receive Enable bit
    The MSSP module must be in an inactive state before the RCEN bit is set
    or the RCEN bit will be disregarded.
    
    In receive operation, the BF bit is set to :
    - 1 when SSPxBUF is full (does not include the ACK and Stop bits)
    - 0 when SSPxBUF is empty (does not include the ACK and Stop bits)

    Datasheet, figure 19-24
    
    Fixed by Rolf Ziegler
	--------------------------------------------------------------------------*/

u8 I2C_read()
{
//rz    u8 r;
    
    I2C_idle();                 // Wait the MSSP module is inactive
    //PIR1bits.SSPIF = 0;         // Clear SSP interrupt flag
    SSPCON2bits.RCEN = 1;       // Initiate reception of byte
    //while (!PIR1bits.SSPIF);    // Wait the interrupt flag is set

    PIR1bits.SSPIF = 0;         // Clear SSP interrupt flag
	
//rz	r = SSPBUF;
	
    while (!PIR1bits.SSPIF);    // Wait the interrupt flag is set
    PIR1bits.SSPIF=0; // ROlf clear SSPIF
//rz    r = SSPBUF;
	PIR1bits.SSPIF=0; // ROlf clear SSPIF
    //while (!SSPSTATbits.BF);    // Wait until buffer is full
    //while (SSPCON2bits.RCEN);    // Wait until RCEN is cleared

//rz    return r;
	return SSPBUF;
}

/*	----------------------------------------------------------------------------
	---------- Wait for the slave to finish its last action
	----------------------------------------------------------------------------
    Application note AN245 page 5, Note 1:
    The master needs to wait for I2C bus idle to indicate that the MSSP
    has finished its last task. The SSPIF interrupt could be used
    instead of the wait for idle.

    The following events will cause the MSSP Interrupt Flag bit to be set:
    - Start condition
    - Stop condition
    - Repeated Start
    - Data transfer byte transmitted/received
    - Acknowledge transmitted
	--------------------------------------------------------------------------*/

void I2C_wait()
{
    while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
    PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
}

/*	----------------------------------------------------------------------------
	---------- Wait until module is no longuer active
	----------------------------------------------------------------------------
    This function waits until 
    * all the five low bits of SSPCON2 (SEN, RSEN, PEN, RCEN or ACKEN) are 0
    AND
    * bit 2 of SSPSTAT (R_W) is 0
    
    If either 1 bit of SSPCON2 <4:0> is set or R_W bit is set then
    operation is in progress.

    Source = Datasheet : ORing R_W bit with SEN, RSEN, PEN, RCEN or ACKEN
    will indicate if the MSSP is in Active mode
	--------------------------------------------------------------------------*/
/*
void I2C_idle()
{
    while (((SSPCON2& 0x1F) > 0) | (SSPSTATbits.R_W));
}
*/
/*	----------------------------------------------------------------------------
	---------- Wait for Acknowledge (Ack) from the slave
	----------------------------------------------------------------------------
    In Transmit mode, the ACKSTAT bit (SSPxCON2<6>)
    is cleared when the slave has sent an Acknowledge
    (ACK = 0) and is set when the slave does not Acknowl-
    edge (ACK = 1). A slave sends an Acknowledge when
    it has recognized its address (including a general call),
    or when the slave has properly received its data.
	--------------------------------------------------------------------------*/
/*
u8 I2C_waitAck()
{
    u8 i=0;

    while (!SSPCON2bits.ACKSTAT)
    {
        i++;
        if (i==0) return 0;
    }
    return 1;
}
*/
/*	----------------------------------------------------------------------------
	---------- Send start bit
	----------------------------------------------------------------------------
    Start condition is issued to indicate the beginning of a serial transfer.
    If the I2C module is active, this bit may not be set.
	--------------------------------------------------------------------------*/

void I2C_start()
{
    I2C_idle();                     // Wait module is inactive
    //PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
    SSPCON2bits.SEN = 1;            // Send start bit
    while (SSPCON2bits.SEN);        // Wait until SEN is cleared 
    //while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
}

/*	----------------------------------------------------------------------------
	---------- Send stop bit
	----------------------------------------------------------------------------
    Stop condition is issued to indicate the end of a serial transfer.
    If the I2C module is active, this bit may not be set.
    When the PEN bit is cleared, the SSPIF bit is set. 
	--------------------------------------------------------------------------*/

void I2C_stop()
{
    I2C_idle();                     // Wait module is inactive
    //PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
    SSPCON2bits.PEN = 1;            // Send stop bit
    while (SSPCON2bits.PEN);        // Wait until PEN is cleared 
    //while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
}

/*	----------------------------------------------------------------------------
	---------- Send restart bit
	----------------------------------------------------------------------------
    If the I2C module is active, this bit may not be set.
	--------------------------------------------------------------------------*/

void I2C_restart()
{
    I2C_idle();                     // Wait module is inactive
    //PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
    SSPCON2bits.RSEN = 1;           // Send restart bit
    while (SSPCON2bits.RSEN);       // Wait until RSEN is cleared  
    //while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
}

/*	----------------------------------------------------------------------------
	---------- Send an Acknowledge (Ack) to the slave
	----------------------------------------------------------------------------
    If the I2C module is active, this bit may not be set.
    NB: the ACKEN bit is automatically cleared
	--------------------------------------------------------------------------*/

void I2C_sendAck()
{
    I2C_idle();                     // Wait module is inactive
    SSPCON2bits.ACKDT = 0;          // We want an Ack
    PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
    SSPCON2bits.ACKEN = 1;          // Send it now
    while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
}

/*	----------------------------------------------------------------------------
	---------- Send a Not Acknowledge (NAck) to the slave
	----------------------------------------------------------------------------
    If the I2C module is active, this bit may not be set.
    NB: the ACKEN bit is automatically cleared
	--------------------------------------------------------------------------*/

void I2C_sendNack()
{
    I2C_idle();                     // Wait module is inactive
    SSPCON2bits.ACKDT = 1;          // We want a No Ack
    PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
    SSPCON2bits.ACKEN = 1;          // Send it now
    while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
}

#endif
