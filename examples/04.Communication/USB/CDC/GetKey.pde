//	Input/Output on GNU/Linux:	sudo minicom -o -D /dev/ttyACM0

u8 c=0;

void setup()
{
}

void loop()
{
    CDC.printf("Press a Key ...\r\n");
    c = CDC.getKey(); // Wait until a key is pressed
    CDC.printf("You pressed key [%c], code ASCII is \"%d\"\r\n", c, c);
}
