  /*
  LiquidCrystal Library - TextDirection
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch demonstrates how to use leftToRight() and rightToLeft()
 to move the cursor.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe 
 modified 22 Nov 2010
 by Tom Igoe
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 Pinguino port by Marcus Fazzi <marcus@fazzi.eng.br>
 http://fazzi.eng.br
 */

char thisChar = 'a';

void setup() {
  // initialize the library with the numbers of the interface pins
  //lcd.pins(RS, E, D4, D5, D6, D7, 0, 0, 0, 0); //4bits
  //lcd.pins(RS, E, D0, D1, D2, D3, D4, D5, D6, D7); //8bits
  lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // turn on the cursor:
  lcd.cursor();
}

void loop() {
  // reverse directions at 'm':
  if (thisChar == 'm') {
    // go right for the next letter
    lcd.rightToLeft(); 
  }
  // reverse again at 's':
  if (thisChar == 's') {
    // go left for the next letter
    lcd.leftToRight(); 
  }
  // reset at 'z':
  if (thisChar > 'z') {
    // go to (0,0):
    lcd.home(); 
    // start again at 0
    thisChar = 'a';
  }
  // print the character
  lcd.printf("%c",thisChar);
  // wait a second:
  delay(500);
  // increment the letter:
  thisChar++;
}








