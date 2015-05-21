/*	----------------------------------------------------------------------------
	Pinguino printf demo - Régis Blanchot
	----------------------------------------------------------------------------
 	CDC output
 	--------------------------------------------------------------------------*/

void setup()
{
	CDC.getKey();
}


void loop()
{
	u8		*string = "Hello world!";
	u8		k, c = 65;
	s16		i = -333;
	s32		l = -115200;
	float	f = -123.4567;

	CDC.printf("\r\n");
	CDC.printf("**************************\r\n");
	CDC.printf("*  Pinguino Printf Demo  *\r\n");
	CDC.printf("**************************\r\n");
	CDC.printf("\r\n");
	CDC.printf("string = %s\r\n", string);
	CDC.printf("character = \"%c\"\r\n", c);
	CDC.printf("signed char = %d / unsigned char = %u\r\n", -c, -c);
	CDC.printf("signed int = %d / unsigned int = %u\r\n", -i, -i);
	CDC.printf("signed long = %d / unsigned long = %u\r\n", -l, -l);
	CDC.printf("decimal[%d] = hexa[0x%X] = ", i, i);
	CDC.printf("binary[0b%016b] = octal[%o]\r\n", i, i);
	CDC.printf("float (with 4 digits) = %.4f\r\n", f);
	CDC.printf("justif: \"%-10s\"\r\n", "left");
	CDC.printf("justif: \"%10s\"\r\n", "right");
	CDC.printf(" 3: %04d zero padded\r\n", 3);
	CDC.printf(" 3: %-4d left justif.\r\n", 3);
	CDC.printf(" 3: %4d right justif.\r\n", 3);
	CDC.printf("-3: %04d zero padded\r\n", -3);
	CDC.printf("-3: %-4d left justif.\r\n", -3);
	CDC.printf("-3: %4d right justif.\r\n", -3);
	CDC.printf("\r\n");
	CDC.printf("Press a Key ...\r\n");
	k = CDC.getKey();
	if (k == '\r')
		CDC.printf("You pressed RETURN\r\n");
	else
		CDC.printf("You pressed \"%c\"\r\n", k);
}
