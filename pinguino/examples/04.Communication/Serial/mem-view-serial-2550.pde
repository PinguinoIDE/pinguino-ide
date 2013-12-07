/*----------------------------------------------------- 
Author:  --<avrin>
Date: Sun May 16  2013

Description: 
This is a simple data RAM viewer for PIC18F2550.
You can check the software stack consumption, SFR values and all other data RAM values
after execution some user programs and this dump program.  

Note: Reading SFR, especially the internal receive buffer of SPI,UART,I2C etc
would occur the empty buffer read error. The software stack consumption displayed 
includes the stack consumption of the dump program itself. :-)
-----------------------------------------------------*/

#define PRAM(m)    (0x800000UL+(m))		/* Define data RAM pointer */

void setup() {
	Serial.begin(9600);
	Serial.printf("Press any key");
	Serial.getKey();
	Serial.printf("\r\nRunning user test program ...");
}

void anyUserCode(void)
{
	int i=0;

	while(i<10){
		Serial.printf("\n%02d, %lu [msec]", i++ ,millis());
		Serial.printf("  ");
		Serial.print(millis(),HEX);
	}
}

void loop() {
	int i;
	u8 *addrs = PRAM(0);
	u8 adh,adl;

	/***  some user program */
	anyUserCode();
	/*********************/


	Serial.printf("\r\n\r\n --- PIC18F2550 Data Memory Dump ---");
	while((u32)addrs != PRAM(0x800)){
		adh = ((u32)addrs & 0xfff)>>8;
		adl = (u32)addrs & 0xff;
		Serial.printf("\r\n%01X%02X: ",adh,adl);
		for(i=0; i < 16; i++){
			Serial.printf("%02X ",*addrs++);
		}
	}
	addrs = PRAM(0x0);
	Serial.printf("\r\n Retry: Press any key");
	Serial.getKey();		
}
