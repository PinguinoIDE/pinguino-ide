/*
  Switch statement  with serial input
 
 Demonstrates the use of a switch statement.  The switch
 statement allows you to choose from among a set of discrete values
 of a variable.  It's like a series of if statements.
 
 To see this sketch in action, open the Serial monitor and send any character.
 The characters a, b, c, d, and e, will turn on LEDs.  Any other character will turn
 the LEDs off.
 
 The circuit:
 * 5 LEDs attached to digital pins 2 through 6 through 220-ohm resistors
 
 created 1 Jul 2009
 by Tom Igoe 
 
This example code is in the public domain.
   
 http://www.arduino.cc/en/Tutorial/SwitchCase2
 */

void setup() {
  int thisPin;
//
  serial1init(9600); 
//
      for (thisPin = 2; thisPin < 7; thisPin++) {
        pinmode(thisPin, OUTPUT);
      } 
}

void loop() {
  int thisPin;
//
  if (serial1available() > 0) {
    int inByte = serial1read();
//
//
//
//
//

    switch (inByte) {
    case 'a':    
      digitalwrite(2, HIGH);
      break;
    case 'b':    
      digitalwrite(3, HIGH);
      break;
    case 'c':    
      digitalwrite(4, HIGH);
      break;
    case 'd':    
      digitalwrite(5, HIGH);
      break;
    case 'e':    
      digitalwrite(6, HIGH);
      break;
    default:
//
      for (thisPin = 2; thisPin < 7; thisPin++) {
        digitalwrite(thisPin, LOW);
      }
    } 
  }
}


