/*	----------------------------------------------------------------------------
	serial32demo - Régis Blanchot
	----------------------------------------------------------------------------
	serial demo for pinguino 32 bits
	----------------------------------------------------------------------------
	TTL/RS232
		=> GND TX RX +5V (Black Green White Red)
	PIC32MX4XXL (Datasheet DS61143C-page 6)
		U1RX = RF2 (pin 52) = Green
		U1TX = RF8 (pin 53) = White
	--------------------------------------------------------------------------*/
#define UARTPORT UART1
#define UARTSPEED 9600

const u8 mainMenu[] =
{
	"\r\n"\
	"Welcome to Pinguino32 Serial Demo!\r\n"\
	"Here are the main menu choices:\r\n"\
	"0. SerialPrintf Test\r\n"\
	"1. View System Clock Frequency\r\n"\
	"2. View Peripheral Clock Frequency\r\n"\
	"3. View Actual BAUD rate\r\n"\
	"4. Increase BAUD rate\r\n"\
	"5. Decrease BAUD rate\r\n"\
	"6. Set Line Control\r\n"\
	"7. ECHO\r\n"\
	"Please Choose a number\r\n"
};

const u8 lineMenu[] =
{
    "Line Control Menu\r\n"\
    "You may need to change your port conficuration on your terminal to see data\r\n"\
    "1. 8-N-1\r\n"\
    "2. 8-E-1\r\n"\
    "3. 8-O-1\r\n"\
    "4. 8-N-2\r\n"\
    "5. 8-E-2\r\n"\
    "6. 8-O-2\r\n"\
    "\r\n\r\nPlease Choose a number\r\n"
};

const u32 lineControl[] =
{
	(UART_8_BITS_NO_PARITY   | UART_STOP_BITS_1),
	(UART_8_BITS_EVEN_PARITY | UART_STOP_BITS_1),
	(UART_8_BITS_ODD_PARITY  | UART_STOP_BITS_1),
	(UART_8_BITS_NO_PARITY   | UART_STOP_BITS_2),
	(UART_8_BITS_EVEN_PARITY | UART_STOP_BITS_2),
	(UART_8_BITS_ODD_PARITY  | UART_STOP_BITS_2)
};

u32 baudrate;

void setup()
{
	baudrate = UARTSPEED;	// 300, 600, 1200, 2400, 4800, 9600, 19200, 38400, 57600, 115200, until Sys. Clock / 4
	SerialP32MX.configure(UARTPORT, UART_ENABLE, UART_RX_TX_ENABLED,	baudrate);

/*	---Use one of these forms------------------------------------------------------------
	SerialP32MX.configure(	UARTPORT, 0x8000, 0x1400, 9600);
	SerialP32MX.configure(	UARTPORT, UART_ENABLE,	UART_RX_TX_ENABLED,	baudrate);
	SerialP32MX.configure(	UARTPORT,
							UART_ENABLE | UART_ENABLE_PINS_TX_RX_ONLY | UART_8_BITS_NO_PARITY | UART_STOP_BITS_1,
							UART_RX_TX_ENABLED | UART_INTERRUPT_ON_RX_FULL,
							baudrate);
/*	---or-----------------------------------------------------------------------
	SerialP32MX.pinConfigure(UARTPORT);
	SerialP32MX.setDataRate(UARTPORT, baudrate);
	SerialP32MX.setLineControl(UARTPORT, UART_ENABLE | UART_ENABLE_PINS_TX_RX_ONLY | UART_8_BITS_NO_PARITY | UART_STOP_BITS_1);
	SerialP32MX.enable(UARTPORT, UART_RX_TX_ENABLED | UART_INTERRUPT_ON_RX_FULL);
	SerialP32MX.intConfigure(UARTPORT, INT_PRIORITY_3, INT_SUBPRIORITY_3);
	--------------------------------------------------------------------------*/

	//SerialP32MX.printf(UARTPORT, "* System Clock is %2uMHz *\n\r", GetSystemClock()/1000000);

/*	---Test---------------------------------------------------------------------
	s16 i=-1000;
	char *string = "Fine !";
	loop
	{
		SerialP32MX.printf(UARTPORT, "Decimal[%5d] = Hexa[0x%4X] %s\n\r", i, i, string);
		i++;
	}
	--------------------------------------------------------------------------*/
}


void loop()
{
	u8 choice;
	u8 mainItem;
	u8 lineItem;

	char *string = "Hello world!";
	char c = 65;
	int i = 333;
	long l = 115200;
	//float f = 3.14159265; // floating point support is not yet implemented in printf

	SerialP32MX.printf(UARTPORT, "%s\r\n", mainMenu);
	mainItem = SerialP32MX.getKey(UARTPORT);
	mainItem -= '0';

	switch (mainItem)
	{
		case 0:
			SerialP32MX.printf(UARTPORT, "\r\n");
			SerialP32MX.printf(UARTPORT, "**************************\r\n");
			SerialP32MX.printf(UARTPORT, "*      Serial32 Demo     *\r\n");
			SerialP32MX.printf(UARTPORT, "**************************\r\n");
			SerialP32MX.printf(UARTPORT, "\r\n");
			SerialP32MX.printf(UARTPORT, "string = %s\r\n", string);
			SerialP32MX.printf(UARTPORT, "character = \"%c\"\r\n", c);
			SerialP32MX.printf(UARTPORT, "signed char = %d / unsigned char = %u\r\n", -c, -c);
			SerialP32MX.printf(UARTPORT, "signed int = %d / unsigned int = %u\r\n", -i, -i);
			SerialP32MX.printf(UARTPORT, "signed long = %d / unsigned long = %u\r\n", -l, -l);
			SerialP32MX.printf(UARTPORT, "decimal[%d] = hexa[0x%X] = binary[0b%016b] = octal[%o]\r\n", i, i, i, i);
			//Serial.printf(UARTPORT, "float = %f\r\n", f);
			SerialP32MX.printf(UARTPORT, "justif: \"%-10s\"\r\n", "left");
			SerialP32MX.printf(UARTPORT, "justif: \"%10s\"\r\n", "right");
			SerialP32MX.printf(UARTPORT, " 3: %04d zero padded\r\n", 3);
			SerialP32MX.printf(UARTPORT, " 3: %-4d left justif.\r\n", 3);
			SerialP32MX.printf(UARTPORT, " 3: %4d right justif.\r\n", 3);
			SerialP32MX.printf(UARTPORT, "-3: %04d zero padded\r\n", -3);
			SerialP32MX.printf(UARTPORT, "-3: %-4d left justif.\r\n", -3);
			SerialP32MX.printf(UARTPORT, "-3: %4d right justif.\r\n", -3);
			SerialP32MX.printf(UARTPORT, "\r\n");
			SerialP32MX.printf(UARTPORT, "Press Any Key to continue ...\r\n");
			SerialP32MX.getKey(UARTPORT);
			break;
		case 1:
			SerialP32MX.printf(UARTPORT, "**************************\r\n");
			SerialP32MX.printf(UARTPORT, "* System Clock is %2uMHz *\r\n", GetSystemClock()/1000000);
			SerialP32MX.printf(UARTPORT, "**************************\r\n");
			SerialP32MX.printf(UARTPORT, "Press Any Key to continue ...\r\n");
			SerialP32MX.getKey(UARTPORT);
			break;
		case 2:
			SerialP32MX.printf(UARTPORT, "******************************\r\n");
			SerialP32MX.printf(UARTPORT, "* Peripheral Clock is %2uMHz *\r\n", GetPeripheralClock()/1000000);
			SerialP32MX.printf(UARTPORT, "******************************\r\n");
			SerialP32MX.printf(UARTPORT, "Press Any Key to continue ...\r\n");
			SerialP32MX.getKey(UARTPORT);
			break;
		case 3:
			SerialP32MX.printf(UARTPORT, "**************************************\r\n");
			SerialP32MX.printf(UARTPORT, "* Actual BAUD rate is %8u bauds *\r\n", SerialGetDataRate(UARTPORT));
			SerialP32MX.printf(UARTPORT, "**************************************\r\n");
			SerialP32MX.printf(UARTPORT, "Press Any Key to continue ...\r\n");
			SerialP32MX.getKey(UARTPORT);
			break;
		case 4:
			baudrate = baudrate << 1;
			SerialP32MX.printf(UARTPORT, "BAUD rate was %u and will be %u bauds\r\n", SerialGetDataRate(UARTPORT), baudrate);
			SerialP32MX.printf(UARTPORT, "Press Any Character after re-configuring you terminal\r\n");
			SerialP32MX.getKey(UARTPORT);
			SerialP32MX.setDataRate(UARTPORT, baudrate);
			SerialP32MX.printf(UARTPORT, "New BAUD rate is %u bauds\r\n", SerialGetDataRate(UARTPORT));
			SerialP32MX.printf(UARTPORT, "Press Any Key to continue ...\r\n");
			SerialP32MX.getKey(UARTPORT);
			break;
		case 5:
			baudrate = baudrate >> 1;
			SerialP32MX.printf(UARTPORT, "BAUD rate was %u and will be %u bauds\r\n", SerialGetDataRate(UARTPORT), baudrate);
			SerialP32MX.printf(UARTPORT, "Press Any Character after re-configuring you terminal\r\n");
			SerialP32MX.getKey(UARTPORT);
			SerialP32MX.setDataRate(UARTPORT, baudrate);
			SerialP32MX.printf(UARTPORT, "New BAUD rate is %u bauds\r\n", SerialGetDataRate(UARTPORT));
			SerialP32MX.printf(UARTPORT, "Press Any Key to continue ...\r\n");
			SerialP32MX.getKey(UARTPORT);
			break;
		case 6:
			SerialP32MX.printf(UARTPORT, "%s", lineMenu);
			lineItem = SerialP32MX.getKey(UARTPORT);
			lineItem = lineItem - '0';
			SerialP32MX.printf(UARTPORT, "Press Any Character after re-configuring you terminal\r\n");
			SerialP32MX.getKey(UARTPORT);
			SerialP32MX.setLineControl(UARTPORT, lineControl[lineItem]);
			break;
		case 7:
			SerialP32MX.printf(UARTPORT, "Type a message (less than 100 characters) and press return\r\n");
			string = SerialP32MX.getString(UARTPORT);
			SerialP32MX.printf(UARTPORT, "You Typed:\r\n\r\n%s\r\n\r\n", string);
			SerialP32MX.printf(UARTPORT, "\r\n\r\nPress any key to continue");
			SerialP32MX.getKey(UARTPORT);
			break;
	}
}
