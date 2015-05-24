/*	---------------------------------------------------------------------------
	Connect a Bluetooth Device to your Pinguino
	2011 Regis Blanchot <rblanchot@gmail.com>
	http://www.pinguino.cc
	---------------------------------------------------------------------------
	tested with Olimex PIC32-PINGUINO & MOD-BT Bluetooth Transciever Module with BGB203
	output : sudo minicom -o -D /dev/ttyACM0
	---------------------------------------------------------------------------*/

char * Serial2GetString()
{
	u8 i = 0;
	char c;
	static char buffer[80];
	
	do {
		while (!Serial2.available());
		c = Serial2.read();
		CDC.printf("%c\r\n", c);
		buffer[i++] = c;
	} while (c != '\r');
	buffer[i] = '\0';
	Serial2.flush();

	return (buffer);
}

void setup()
{
	// BT Module is connected to PIC32_PINGUINO UEXT Connector
	// UEXT is connected to the PIC32 UART2
	BT.init(UART2, 115200);
	// Device address you want to be connected to
	BT.connect(UART2, "9463D13C680F");
}

void loop()
{
	// Take a look on your phone if it recognizes your Pinguino Device
	// Now you are in Data Mode
	// Use Serial2 functions to send and/or receive data
	BT.send(UART2, "It works !!!\r\n");
//	Serial2.printf("AT");
	CDC.printf("received [%s]|r|n", Serial2GetString() );
//	Serial2.printf("It works !!!\r\n");
//	CDC.printf("It works !!!\r\n");
}
