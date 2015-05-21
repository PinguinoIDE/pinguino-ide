// SwSerial.printf and sprintf Demo
// Régis Blanchot 2010
// Modified by avrin 2013
// Adapted to soft serial by A. Gentric - Aug 2014
#define SWBAUDRATE 9600// or 57600, 38400, 19200, 9600, 4800
// other baudrates 115200 2400 1200 were tried but did not work perfectly 
#define _SW_PORT     _PORTB           // Transmit pin port and pin
#define TXDpin      4    // or 0, 2, 6
#define RXDpin      5    // or 1, 3, 7
#define SW_PORT_TXDpin      PORTBbits.RB4    // or 0, 2, 6
#define SW_TRIS_TXDD        TRISBbits.TRISB4 // or 0, 2, 6
#define SW_PORT_RXDpin      PORTBbits.RB5    // or 1, 3, 7
#define SW_TRIS_RXDD        TRISBbits.TRISB5 // or 1, 3, 7

// For 8bit PICs. long int -> %ld or %lu

char *string = "Hello world!";
char c = 65;
int i = 333;
long l = 115200;
float f = 3.14159265;

void setup()
{
    SW_TRIS_TXDD = 0;
    SW_PORT_TXDpin = 1;

    SW_TRIS_RXDD = 1;

	SwSerial.printf("\r\n");
	SwSerial.printf("**************************\r\n");
	SwSerial.printf("*** Serial Printf Demo ***\r\n");
	SwSerial.printf("**************************\r\n");
	SwSerial.printf("\r\n");
	SwSerial.printf("string = %s\r\n", string);
	SwSerial.printf("character = \"%c\"\r\n", c);
	SwSerial.printf("signed char = %d / unsigned char = %u\r\n", -c, -c);
	SwSerial.printf("signed int = %d / unsigned int = %u\r\n", -i, -i);
	SwSerial.printf("signed long = %ld / unsigned long = %lu\r\n", -l, -l);
	SwSerial.printf("decimal[%d] = hexa[0x%X] = binary[0b%016b] = octal[%o]\r\n", i, i, i, i);
	SwSerial.printf("float = %f\r\n", f);
	SwSerial.printf("justif: \"%-10s\"\r\n", "left");
	SwSerial.printf("justif: \"%10s\"\r\n", "right");
	SwSerial.printf(" 3: %04d zero padded\r\n", 3);
	SwSerial.printf(" 3: %-4d left justif.\r\n", 3);
	SwSerial.printf(" 3: %4d right justif.\r\n", 3);
	SwSerial.printf("-3: %04d zero padded\r\n", -3);
	SwSerial.printf("-3: %-4d left justif.\r\n", -3);
	SwSerial.printf("-3: %4d right justif.\r\n", -3);
	SwSerial.printf("\r\n");

	SwSerial.printf("Press Any Key ...\r\n");
	c = SwSerial.read();
	SwSerial.printf("You pressed Key %c\r\n", c);
	SwSerial.printf("\r\n");

	SwSerial.printf("Press Any Key to continue ...\r\n");
	c = SwSerial.read();
}

void loop()
{
	unsigned char *buffer;
	
	SwSerial.printf("Write Any Texte ...then Enter\r\n");
	buffer = SwSerial.getString();

	SwSerial.printf("You wrote : %s\r\n", buffer);
	SwSerial.printf("\r\n");
}
