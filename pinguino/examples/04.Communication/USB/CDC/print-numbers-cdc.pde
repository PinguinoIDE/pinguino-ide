// pinguino print numbers example
// Jean-Pierre Mandon 2010
// pinguino printf numbers example
// Régis Blanchot 2011
// Modified for CDC. by avrin 2013

int number;

void setup()
{
	CDC.getKey();
}

void loop()
{
	number=3200;
	// Arduino or Pinguino
	CDC.printf("Decimal 3200=");
	CDC.print(number,DEC);
	CDC.printf("\r\n");
	// Only Pinguino
	CDC.printf("Decimal 3200=%d\r\n", number);

	number=253;
	// Arduino or Pinguino
	CDC.printf("Hexadecimal 253=");
	CDC.print(number,HEX);
	CDC.printf("\r\n");
	// Only Pinguino
	CDC.printf("Hexadecimal 253=%x\r\n", number);

	number=156;
	// Arduino or Pinguino
	CDC.printf("Binary 156=");
	CDC.print(number,BIN);
	CDC.printf("\r\n");
	// Only Pinguino
	CDC.printf("Binary 156=%b\r\n", number);

	number=236;
	// Arduino or Pinguino
	CDC.printf("Octal 236=");
	CDC.print(number,OCT);
	CDC.printf("\r\n");
	// Only Pinguino
	CDC.printf("Octal 236=%o\r\n", number);

	number=65;
	// Arduino or Pinguino
	CDC.printf("Byte 65=");
	CDC.print(number,BYTE);
	CDC.printf("\r\n");
	// Only Pinguino
	CDC.printf("Byte 65=%d\r\n", number);
 
 CDC.getKey();
 
}
