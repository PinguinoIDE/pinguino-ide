/*
  For Loop Iteration
 
 Demonstrates the use of a for() loop. 
 Lights multiple LEDs in sequence, then in reverse.
 
 The circuit:
 * LEDs from pins 2 through 7 to ground
 
 created 2006
 by David A. Mellis
 modified 5 Jul 2009
 by Tom Igoe 

This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/ForLoop
 */

int timer = 100;           // The higher the number, the slower the timing.

void setup() {
  int thisPin;
  // use a for loop to initialize each pin as an output:
  for (thisPin = 2; thisPin < 8; thisPin++)  {
    pinMode(thisPin, OUTPUT);      
  }
}

void loop() {
  int thisPin;
  // loop from the lowest pin to the highest:
  for (thisPin = 2; thisPin < 8; thisPin++) { 
    // turn the pin on:
    digitalWrite(thisPin, HIGH);   
    delay(timer);                  
    // turn the pin off:
    digitalWrite(thisPin, LOW);    
  }

  // loop from the highest pin to the lowest:
  for (thisPin = 7; thisPin >= 2; thisPin--) { 
    // turn the pin on:
    digitalWrite(thisPin, HIGH);
    delay(timer);
    // turn the pin off:
    digitalWrite(thisPin, LOW);
  }
}
