#define CS 3		
#define SDI 2		
#define SDO 0		
#define CLKpin 1		
#define RESET 4

#define wd 1	// delay for MRF24J40 clk

// MRF24J40 internal registers
#define RFCTL 0x36
#define EADR0 0x05

void setup()
{
	pinMode(CS,OUTPUT);
	pinMode(SDI,INPUT);
	pinMode(SDO,OUTPUT);
	pinMode(CLKpin,OUTPUT);
	pinMode(RESET,OUTPUT);
	Serial.begin(9600);
}

unsigned char short_read(unsigned char address)
{
	unsigned char i;
	unsigned char resultat=0;
	
	digitalWrite(CS,0);
	address=(address<<1)&0x7E;
	for (i=0;i<8;i++)
		{
		if ((address&0x80)==0x80) digitalWrite(SDO,1);
		else digitalWrite(SDO,0);
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,0);
		delayMicroseconds(wd);
		address=address<<1;
		}
	for (i=0;i<8;i++)
		{
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		if (digitalRead(SDI)) resultat=resultat|1;
		if (i!=7) resultat=resultat<<1;
		digitalWrite(CLKpin,0);
		delayMicroseconds(wd);
		}
digitalWrite(CS,1);
return(resultat);
}

unsigned int long_read(unsigned int address)
{
	unsigned int resultat=0;
	unsigned char i;
	
	digitalWrite(CS,0);
	address=((address<<1)&0x7FE)|0x800;
	for (i=0;i<12;i++)
		{
		if ((address&0x800)!=0) digitalWrite(SDO,1);
		else digitalWrite(SDO,0);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,0);
		address=address<<1;
		}
	for (i=0;i<4;i++)
		{
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,0);
		delayMicroseconds(wd);
		}
	for (i=0;i<8;i++)
		{
		digitalWrite(CLKpin,1);
		if (digitalRead(SDI)==1) resultat=resultat|1;
		if(i!=7) resultat=resultat<<1;
		digitalWrite(CLKpin,0);
		delayMicroseconds(wd);
		}
digitalWrite(CS,1);
return(resultat);
}

void short_write(unsigned char address,unsigned char value)
{
	unsigned char i;
	
	digitalWrite(CS,0);
	address=((address<<1)&0x7F)|1;
	for (i=0;i<8;i++)
		{
		if ((address&0x80)!=0) digitalWrite(SDO,1);
		else digitalWrite(SDO,0);
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,0);
		address=address<<1;
		}
	for (i=0;i<8;i++)
			{
			if ((value&0x80)!=0) digitalWrite(SDO,1);
			else digitalWrite(SDO,0);
			digitalWrite(CLKpin,1);
			delayMicroseconds(wd);
			digitalWrite(CLKpin,0);
			value=value<<1;
			}			
digitalWrite(CS,1);
}

void long_write(unsigned int address, unsigned char value)
{
	unsigned char i;
	
	digitalWrite(CS,0);
	address=((address<<1)&0x7FF)|0x801;
	for (i=0;i<12;i++)
		{
		if ((address&0x800)!=0) digitalWrite(SDO,1);
		else digitalWrite(SDO,0);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,0);
		address=address<<1;
		}	
	for (i=0;i<4;i++)
		{
		digitalWrite(CLKpin,1);
		delayMicroseconds(wd);
		digitalWrite(CLKpin,0);
		delayMicroseconds(wd);
		}
	for (i=0;i<8;i++)
		{
		if ((value&0x80)!=0) digitalWrite(SDO,1);
		else digitalWrite(SDO,0);
		digitalWrite(CLKpin,wd);
		delayMicroseconds(1);
		digitalWrite(CLKpin,wd);
		value=value<<1;
		}			
digitalWrite(CS,1);				
}

void init_zigbee()
{
	digitalWrite(RESET,0);
	digitalWrite(CS,1);
	digitalWrite(CLKpin,0);
	digitalWrite(SDO,0);
	digitalWrite(RESET,1);
	delay(100);
}
		
void loop()
{
	unsigned char reponse;
	unsigned char i;
	
	Serial.print("SPI with MRF24J40MA\r\n");
	init_zigbee();
	
	short_write(EADR0,0x7f);
	reponse=short_read(EADR0);
	Serial.printNumber(reponse,HEX);
	while(1);
}	
