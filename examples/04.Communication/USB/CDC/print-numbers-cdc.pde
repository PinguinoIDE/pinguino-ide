// pinguino print numbers example
// Jean-Pierre Mandon 2010
// pinguino printf numbers example
// Régis Blanchot 2011
// Modified for CDC. by avrin 2013

int number;

void setup()
{
	pinMode(USERLED, OUTPUT);
}

void loop()
{
	number=3200;
	// Arduino or Pinguino
	CDC.print("Decimal 3200=");
	CDC.printNumber(number, DEC);
	CDC.print("\r\n");
	// Only Pinguino
	//CDC.printf("Decimal 3200=%d\r\n", number);

	number=253;
	// Arduino or Pinguino
	CDC.print("Hexadecimal 253=0x");
	CDC.printNumber(number, HEX);
	CDC.print("\r\n");
	// Only Pinguino
	//CDC.printf("Hexadecimal 253=%x\r\n", number);

	number=156;
	// Arduino or Pinguino
	CDC.print("Binary 156=0b");
	CDC.printNumber(number, BIN);
	CDC.print("\r\n");
	// Only Pinguino
	//CDC.printf("Binary 156=%b\r\n", number);

	number=236;
	// Arduino or Pinguino
	CDC.print("Octal 236=");
	CDC.printNumber(number, OCT);
	CDC.print("\r\n");
	// Only Pinguino
	//CDC.printf("Octal 236=%o\r\n", number);

	number=65;
	// Arduino or Pinguino
	CDC.print("Byte 65=");
	CDC.printNumber(number, 2);
	CDC.print("\r\n");
	// Only Pinguino
	//CDC.printf("Byte 65=%d\r\n", number);
 
  toggle(USERLED);
  delay(1000);
 
}
