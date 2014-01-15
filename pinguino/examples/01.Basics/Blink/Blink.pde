/*
	Blink a LED
*/

void setup()
{
    // initialize the digital pin USERLED as an output.
    pinMode(USERLED, OUTPUT);     

     Serial.begin(9600);
serial_begin(9600);
    
}

void loop()
{
    toggle(USERLED);			// alternate ON and OFF
    delay(150);		        // wait for 50ms
/*	Or ...
    digitalWrite(USERLED, HIGH);
    delay(50);		     // wait for 50ms
    digitalWrite(USERLED, LOW);
    delay(50);		     // wait for 50ms
*/
}
