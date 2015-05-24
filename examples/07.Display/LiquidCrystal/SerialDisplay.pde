/*
  LiquidCrystal Library - Serial Input
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.
 
 This sketch displays text sent over the serial port 
 (e.g. from the Serial Monitor) on an attached LCD.
 
 The circuit:
 * LCD RS pin to digital pin 0
 * LCD Enable pin to digital pin 1
 * LCD D4 pin to digital pin 2
 * LCD D5 pin to digital pin 3
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 5
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 
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

void setup(){
  // initialize the library with the numbers of the interface pins
  //lcd.pins(RS, E, D4, D5, D6, D7, 0, 0, 0, 0); //4bits
  //lcd.pins(RS, E, D0, D1, D2, D3, D4, D5, D6, D7); //8bits
  lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	
    // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // initialize the serial communications:
  Serial.begin(9600);
  
  pinMode(13, OUTPUT);
}

void loop()
{
  // when characters arrive over the serial port...
  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    // clear the screen
    lcd.clear();
    
    // read all the available characters
    while (Serial.available() > 0) {
      // display each character to the LCD
      lcd.write(Serial.read());
    }
  }
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(100);
}
