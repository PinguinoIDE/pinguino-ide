/*
Ps2 library for host side (pinguino) to communicate
with a device (keyboard, mouse, etc..).
Samuele Paganoni.
*/
#ifndef __PS2_H__
#define __PS2_H__

#include <delay.c>

/*
                    5V    5V
                     |    |
                   10K   10K
     Hardware:       |    |                      Front:
   ------------      |    |       ----------  ------------
   |          |  clk |    |  5V---|4       |  |  6 || 5  |
   |         0|-----------|-------|5  PS2  |  | 4  ||  3 |
   | Pinguino |           |       |   DIN  |  |   2  1   |
   |         1|-------------------|1 CONN. |  ------------
   |          |  data        GND--|3       |
   ------------                   ----------
*/

/*Used pin definition.*/
#define PS2_CLOCK_PIN PORTBbits.RB0
#define PS2_DATA_PIN PORTBbits.RB1

/*Pin direction.*/
#define PS2_CLOCK_DIR TRISBbits.TRISB0
#define PS2_DATA_DIR TRISBbits.TRISB1

/*Wait for the idle state of the bus.*/
#define ps2_wait_idle() while(PS2_DATA_PIN==0|PS2_CLOCK_PIN==0)

/*Inhibits the transmission of the devices.*/
#define ps2_inhibit() PS2_CLOCK_DIR==0;PS2_CLOCK_PIN==0;Delayus(100)

/*Host request to send.*/
#define ps2_rts() PS2_DATA_DIR=0;PS2_DATA_PIN=0;PS2_CLOCK_DIR=1

/*Buffer length in byte.*/
#define PS2_BUFFER_SIZE 30

/*Global variables.*/
volatile unsigned char ps2_rxpointer,ps2_txpointer;
volatile unsigned char ps2_buffer[PS2_BUFFER_SIZE];
const unsigned char vett[]={1,2,4,8,16,32,64,128};

/*Initializes the library.*/
void ps2_init()
{
	PS2_CLOCK_DIR=1;
	PS2_DATA_DIR=1;
	ps2_rxpointer=0;
	ps2_txpointer=0;
	INTCONbits.INT0IE=1;
	INTCON2bits.INTEDG0=0;
	INTCONbits.GIE=1;
}

unsigned char ps2_readfrom()
{
	unsigned char readed=0,i;
	while(PS2_CLOCK_PIN==0);
	for(i=0;i<8;i++)
	{
		while(PS2_CLOCK_PIN==1);
		readed+=(PS2_DATA_PIN*vett[i]);
		while(PS2_CLOCK_PIN==0);
	}
	while(PS2_CLOCK_PIN==1);
	while(PS2_CLOCK_PIN==0);
	while(PS2_CLOCK_PIN==1);
	ps2_wait_idle();
	return readed;
}

/*Write a byte to the device.*/
void ps2_write(unsigned char dta)
{
	unsigned char i,p=1;
	INTCONbits.INT0IE=0;
	ps2_inhibit();
	ps2_rts();
	while(PS2_CLOCK_PIN==1);
	for(i=0;i<8;i++)
	{
		PS2_DATA_PIN=dta;
		p=p^PS2_DATA_PIN;
		dta=dta>>1;
		while(PS2_CLOCK_PIN==0);
		while(PS2_CLOCK_PIN==1);
	}
	PS2_DATA_PIN=p;
	while(PS2_CLOCK_PIN==0);
	while(PS2_CLOCK_PIN==1);
	PS2_DATA_DIR=1;
	while(PS2_DATA_PIN==1);
	while(PS2_CLOCK_PIN==1);
	ps2_wait_idle();
	INTCONbits.INT0IE=1;
}

/*Checks the availability of a byte.*/
unsigned char ps2_available()
{
	return (ps2_rxpointer!=ps2_txpointer);
}

/*Reads a byte from the buffer.*/
unsigned char ps2_read()
{
	unsigned char tmp=ps2_buffer[ps2_txpointer];
	if(ps2_txpointer<PS2_BUFFER_SIZE)
		ps2_txpointer++;
	else
		ps2_txpointer=0;
	return tmp;
}

/*Interrupt routine, called from UserInterrupt.*/
void ps2_isr()
{
	if(INTCONbits.INT0IF)
	{
		if(PS2_CLOCK_PIN==0)
		{
			ps2_buffer[ps2_rxpointer]=ps2_readfrom();
			if(ps2_rxpointer<PS2_BUFFER_SIZE)
				ps2_rxpointer++;
			else
				ps2_rxpointer=0;
			INTCONbits.INT0IF=0;
		}
	}
}

#endif
