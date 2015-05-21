/*
 * EEPROM Clear
 *
 * Sets all of the bytes of the EEPROM to 0.
 * This example code is in the public domain.
 *
 * NB : 32-bit PICs have no EEPROM
 */

void setup()
{
	u16 i;
	// write a 0 to all 256 bytes of the EEPROM
	for (i = 0; i < 256; i++)
		EEPROM.write(i, 0);
    
	// turn the LED on when we're done
	digitalWrite(13, HIGH);
}

void loop()
{
}
