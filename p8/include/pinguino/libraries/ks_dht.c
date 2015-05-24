#ifndef __KS_DHT_C
#define __KS_DHT_C

#include <digitalw.c>
/*	-----------------------------------------------------------------------
Pinguino library; DHT22 1wire humidity/temperature sensor
Code base: » Forum Post by mirasu 
http://forum.pinguino.cc/showthread.php?tid=3911
--------------------------------------------------------------------------*/
#define wt	5000	 //Timeout

typedef struct
{
u8 sign;	 // sign (1=negative)
double temp;	// temperature
double hum;	// humidity
u8 dht_error; //global error
} KS_DHT_Data;

/// PROTOTYPES
u8 KS_DHTRead(u8,KS_DHT_Data *); 

/*	-----------------------------------------------------------------------
---------- KS_DHTRead()
-----------------------------------------------------------------------
* Description:	reads the dht22 device via 1-wire bus 
* Arguments:	
dhpin = pin number where one wire bus is connected.
dh = data record
return errorcode or 0
--------------------------------------------------------------------*/
u8 KS_DHTRead(u8 dhpin,KS_DHT_Data * dh)
{
u16 timeout;
double h,t;
u8 DHTDAT[5]={0};
u8 DHTCHECKSUM;
u8 i,j;
u8 dhtbyte=0;

h=0;
t=0;
if (digitalread(dhpin)==LOW) {return 1;}	 // Bus not free
pinmode(dhpin,OUTPUT); 
digitalwrite(dhpin,LOW);	 // MCU start signal (>=500us) 
Delayms(1);
//Request Data
pinmode(dhpin,INPUT);
timeout = wt; 
while(digitalread(dhpin)) {timeout--;if (timeout==0) {return 2;}}	 // Wait for DHT’s response (20-40us)
timeout = wt;
while(!digitalread(dhpin)) {timeout--;if (timeout==0) {return 3;}}	 // Response signal (80us)
timeout = wt;	
while(digitalread(dhpin)) {timeout--;if (timeout==0) {return 4;}}	 // Preparation for sending data (80us)
//Read Data
for(i=0;i<5;i++)
{
for(j=1;j<=8;j++)
{
timeout = wt;
while(digitalread(dhpin)==LOW) {timeout--;if (timeout==0) {return 5;}}	 // Start to transmit 1-Bit (50 us)
Delayus(30);
dhtbyte <<= 1;
if (digitalread(dhpin))	 // Hi > 30us (70 us) -> Bit=1	
{
dhtbyte |= 1;
timeout = wt;
while(digitalread(dhpin)) {timeout--;if (timeout==0) {return 6;}}	
}	 // Hi < 30us (26-28 us) -> Bit=0	
}
DHTDAT[i] = dhtbyte;
}

DHTCHECKSUM = DHTDAT[0]+DHTDAT[1]+DHTDAT[2]+DHTDAT[3];
if (DHTCHECKSUM != DHTDAT[4]) // Checksum	
{
return 7;	
}	
dh->sign = 0; 

if (DHTDAT[2] & 0b11111000)	// test if sign is set, i.e. negative
{	
dh->sign = 1;
DHTDAT[2] = (DHTDAT[2] ^ 0xFFFF) + 1;	// 2's complement conversion
}

h=(DHTDAT[0]<<8)+DHTDAT[1];
t=(DHTDAT[2]<<8)+DHTDAT[3];
dh->hum=h/10;
dh->temp=t/10;
return 0;
}

#endif

