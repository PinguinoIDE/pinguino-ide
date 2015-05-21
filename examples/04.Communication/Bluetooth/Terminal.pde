/*	---------------------------------------------------------------------------
	Send AT commands to your bluetooth module and check what it returns.
	2011 Regis Blanchot	<rblanchot@gmail.com>
	http://www.pinguino.cc
	---------------------------------------------------------------------------
	tested with Olimex PIC32-PINGUINO & MOD-BT Bluetooth Transciever Module with BGB203
	output : sudo minicom -o -D /dev/ttyACM0
	---------------------------------------------------------------------------*/

BLUETOOTH res;

void Serial2_GetString(char *buffer)
{
	u8 i = 0;
	char c;
	
	//Serial2.flush();		// reset reception buffer
	do {
		while (!Serial2.available());
		c = Serial2.read();
		CDC.printf("[%c]", c);
		buffer[i++] = c;
	} while (c != '\r');	// AT commands ends with "\r"
	buffer[i] = '\0';		// strings must end with "\0"
	Serial2.flush();		// reset reception buffer
}

void CDC_GetCmd(char *buffer)
{
	u8 i = 0;
	char c;
	
	CDC.printf("ready.\r\n");
	do {
		c = CDC.getKey();
		CDC.printf("%c", c);
		buffer[i++] = c;
	} while (c != '\r');	// until RETURN ("\r") is hit
	CDC.printf("\r\n");
	buffer[i-1] = '\0';		// strings must end with "\0"
}

void setup()
{
	// wait for a key to start
	CDC.printf("Press <RETURN> to start\r\n");
	while (CDC.getKey() != '\r');

	// init BT module connected on UART port 2 at 115200 bauds
	do {
		res = BT.init(UART2, 115200);
		CDC.printf("%s\r\n", res.status);
	} while (res.code != BT_OK);

	// title
	CDC.printf("------------------\r\n");
	CDC.printf("Bluetooth Terminal\r\n");
	CDC.printf("------------------\r\n");
}

void loop()
{
	char cmd[20];

	CDC_GetCmd(cmd);
	CDC.printf("Command = [%s]\r\n", cmd);
	BT.sendCommand(UART2, "%s\r", cmd);
	res = BT.getResponse(UART2);
	CDC.printf("received [%s]\r\n", res.status);
}
