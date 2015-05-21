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
	CDC.printf("Press any key");
	CDC.getKey();
	CDC.printf("\r\nRunning user test program ...");
	delay(1);
}

void anyUserCode(void)
{
	int i=0;

	while(i<10){
		CDC.printf("\n%02d, %lu [msec]", i++ ,millis());
		delay(1);
		CDC.printf("  ");
		CDC.printNumber(millis(),HEX);
		delay(1);
	}
}

void loop() {
	int i;
	u8 *addrs = PRAM(0);
	u8 adh,adl;

	/***  some user program */
	anyUserCode();
	/*********************/


	CDC.printf("\r\n\r\n --- PIC18F2550 Data Memory Dump ---");
	delay(1);
	while((u32)addrs != PRAM(0x800)){
		adh = ((u32)addrs & 0xfff)>>8;
		adl = (u32)addrs & 0xff;
		CDC.printf("\r\n%01X%02X: ",adh,adl);
		for(i=0; i < 16; i++){
			CDC.printf("%02X ",*addrs++);
			delay(1); // need for the stable behavior in case CDC
		}
	}
	addrs = PRAM(0x0);
	CDC.printf("\r\n Retry: Press any key");
	CDC.getKey();		
}
