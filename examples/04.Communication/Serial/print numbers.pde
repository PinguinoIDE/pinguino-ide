// pinguino print numbers example
// Jean-Pierre Mandon 2010
// pinguino printf numbers example
// Régis Blanchot 2011

int number;

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	number=3200;
	// Arduino or Pinguino
	Serial.print("Decimal 3200=");
	Serial.printNumber(number,DEC);
	Serial.println("");
	// Only Pinguino
	Serial.printf("Decimal 3200=%d\n\r", number);

	number=253;
	// Arduino or Pinguino
	Serial.print("Hexadecimal 253=");
	Serial.printNumber(number,HEX);
	Serial.println("");
	// Only Pinguino
	Serial.printf("Hexadecimal 253=%x\n\r", number);

	number=156;
	// Arduino or Pinguino
	Serial.print("Binary 156=");
	Serial.printNumber(number,BIN);
	Serial.println("\n\r");
	// Only Pinguino
	Serial.printf("Binary 156=%b\n\r", number);

	number=236;
	// Arduino or Pinguino
	Serial.print("Octal 236=");
	Serial.printNumber(number,OCT);
	Serial.println("\n\r");
	// Only Pinguino
	Serial.printf("Octal 236=%o\n\r", number);

	number=65;
	// Arduino or Pinguino
	Serial.print("Byte 65=");
	Serial.printNumber(number,BYTE);
	Serial.println("\n\r");
	// Only Pinguino
	Serial.printf("Byte 65=%d\n\r", number);
 
 Serial.getKey();
 
}
