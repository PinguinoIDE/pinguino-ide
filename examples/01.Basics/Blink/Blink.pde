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
    toggle(USERLED);    // alternate ON and OFF
    delay(500);        // wait for 500ms

/*  Or ...
    digitalWrite(USERLED, HIGH);
    delay(500);		     // wait for 500ms
    digitalWrite(USERLED, LOW);
    delay(500);		     // wait for 500ms
*/
}
