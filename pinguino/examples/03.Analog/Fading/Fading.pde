/*
 Fading
 
 This example shows how to fade an LED using the analogWrite() function.
 
 The circuit:
 * LED attached from digital pin 11 to ground.
 
 Created 1 Nov 2008
 By David A. Mellis
 Modified 17 June 2009
 By Tom Igoe
 
 http://arduino.cc/en/Tutorial/Fading
 
 This example code is in the public domain.
 
 */


u8 ledPin = 11;    // LED connected to digital pin 11 (CCP)

void setup()
{ 
    // nothing happens in setup 
    pinMode(ledPin, OUTPUT);
} 

void loop()
{ 
    // fade in from min to max in increments of 5 points:
    int fadeValue;

    for (fadeValue = 0 ; fadeValue <= 1023; fadeValue +=5)
    { 
        // sets the value (range from 0 to 1023):
        analogWrite(ledPin, fadeValue);         
        // wait for 30 milliseconds to see the dimming effect    
        delay(30);                            
    }

    // fade out from max to min in increments of 5 points:
    for (fadeValue = 1023 ; fadeValue >= 0; fadeValue -=5)
    { 
        // sets the value (range from 0 to 1023):
        analogWrite(ledPin, fadeValue);         
        // wait for 30 milliseconds to see the dimming effect    
        delay(30);                            
    }
}
