// CDC.printf and sprintf Demo
// Régis Blanchot 2010
// Modified for CDC. by avrin 2013

char *string = "Hello world!";
char c = 65;
int i = 333;
long l = 115200;
float f = 3.14159265;

 

void setup()
{
}

void loop()
{
	u8 *buffer;
	
	CDC.getKey();
 
	CDC.printf("\r\n");
	CDC.printf("**************************\r\n");
	CDC.printf("*** CDC Printf Demo  ***\r\n");
	CDC.printf("**************************\r\n");
	CDC.printf("\r\n");
	CDC.printf("string = %s\r\n", string);
	CDC.printf("character = \"%c\"\r\n", c);
	CDC.printf("signed char = %d / unsigned char = %u\r\n", -c, -c);
	CDC.printf("signed int = %d / unsigned int = %u\r\n", -i, -i);
	CDC.printf("signed long = %ld / unsigned long = %lu\r\n", -l, -l);
	CDC.printf("decimal[%d] = hexa[0x%X] ",i,i);
	CDC.printf("= binary[0b%016b] = octal[%o]\r\n", i,i);
	CDC.printf("float = %f\r\n", f);
	CDC.printf("justif: \"%-10s\"\r\n", "left");
	CDC.printf("justif: \"%10s\"\r\n", "right");
	CDC.printf(" 3: %04d zero padded\r\n", 3);
	CDC.printf(" 3: %-4d left justif.\r\n", 3);
	CDC.printf(" 3: %4d right justif.\r\n", 3);
	CDC.printf("-3: %04d zero padded\r\n", -3);
	CDC.printf("-3: %-4d left justif.\r\n", -3);
	CDC.printf("-3: %4d right justif.\r\n", -3);
	CDC.printf("\r\n");

	CDC.printf("Press Any Key ...\r\n");
	c = CDC.getKey();
	CDC.printf("\rYou pressed Key %c, (ASCII=%d)\r\n", c, c);
	CDC.printf("\r\n");

	CDC.printf("Press Any Key to continue ...\r\n");
	CDC.getKey();
	CDC.printf("Write Any Texte ...\r\n");
	buffer = CDC.getString();
	CDC.printf("You wrote : %s\r\n", buffer);
	CDC.printf("\r\n");
}
