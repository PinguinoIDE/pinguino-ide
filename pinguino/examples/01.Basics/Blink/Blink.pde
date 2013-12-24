/*
	Blink a LED
*/

void setup()
{
    // initialize the digital pin USERLED as an output.
    pinMode(USERLED, OUTPUT);     
}

void loop()
{
    toggle(USERLED);			// alternate ON and OFF
    delay(50);		        // wait for 50ms
/*	Or ...
    digitalWrite(USERLED, HIGH);
    delay(50);		     // wait for 50ms
    digitalWrite(USERLED, LOW);
    delay(50);		     // wait for 50ms
*/
}
