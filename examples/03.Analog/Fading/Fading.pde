/*

 Fading
 
 This example shows how to fade an LED using the analogWrite() function.
 The LED must be connected to a valid PWM pin.

*/


u8 ledPin = 10;    // LED connected to digital pin 10 (CCP1)

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
