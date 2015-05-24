/*----------------------------------------------------- 
Author:  --<>
Date: Aug 24 2014
Description:
Example for use of Soft serial 9600 Bds Rx=RB5 Tx=RB4
You can select 4800 Bds Rx=RB1, RB3, RB7 - Tx=RB0, RB2, RB6
                        or any digital pin
This replaces or complements your UART and pins RC6-RC7 
-----------------------------------------------------*/

#define SWBAUDRATE 9600// or 57600, 38400, 19200, 9600, 4800
// other baudrates 115200 2400 1200 were tried but did not work perfectly 
#define _SW_PORT     _PORTB           // Transmit pin port and pin
#define TXDpin      4    // or 0, 2, 6
#define RXDpin      5    // or 1, 3, 7
#define SW_PORT_TXDpin      PORTBbits.RB4    // or 0, 2, 6
#define SW_TRIS_TXDD        TRISBbits.TRISB4 // or 0, 2, 6
#define SW_PORT_RXDpin      PORTBbits.RB5    // or 1, 3, 7
#define SW_TRIS_RXDD        TRISBbits.TRISB5 // or 1, 3, 7
char buf;
void setup() {
    SW_TRIS_TXDD = 0;
    SW_PORT_TXDpin = 1;

    SW_TRIS_RXDD = 1;

    SwSerial.putString("Hello !\n\r");//putString replaceable by printf
    SwSerial.printf("Enter your name or everything you want : ");//putsUART
}

void loop() {
   buf=SwSerial.read();
//    SwSerial.write(buf); //write replaceable by printf :
   SwSerial.printf("%c",buf);

}