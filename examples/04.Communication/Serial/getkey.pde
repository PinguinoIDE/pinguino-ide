/*	----------------------------------------------------------------------------
	---------- getkey.pde
	----------------------------------------------------------------------------
	@author	(c)2010 - R. Blanchot <rblanchot@gmail.com>
	@desc		Simple example how to use Serial.getkey()
	--------------------------------------------------------------------------*/

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	u8 c;
	
	Serial.printf("Please, press a key\n");
	c = Serial.getKey();
	Serial.printf("You pressed key [%c]\n",c);
}
