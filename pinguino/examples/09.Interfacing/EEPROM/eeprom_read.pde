/*
 * EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it 
 * to the computer.
 * This example code is in the public domain.
 *
 * NB : 32-bit PICs have no EEPROM
 */

// start reading from the first byte (address 0) of the EEPROM
u16 address = 0;
u8 value;

void setup()
{
  Serial.begin(9600);
}

void loop()
{
	// read a byte from the current address of the EEPROM
	value = EEPROM.read(address);

/*	Arduino Syntax
	Serial.print(address);
	Serial.print("\t");
	Serial.print(value, DEC);
	Serial.println();
*/

	// Pinguino Syntax
	Serial.printf("%d\t%d \n", address, value);

	// advance to the next address of the EEPROM
	address = address + 1;

	// there are only 256 bytes of EEPROM, from 0 to 255, so if we're
	// on address 256, wrap around to address 0
	if (address == 256)
	address = 0;

	delay(500);
}
