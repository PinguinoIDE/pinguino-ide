/*
   LCD Library (lcdlib.c) - setCursor
 
   Demonstrates the use of a 16x2 LCD display.  The LiquidCrystal
   library works with all LCD displays that are compatible with the 
   Hitachi HD44780 driver. There are many of them out there, and you
   can usually tell them by the 16-pin interface.
 
   This program prints to all the positions of the LCD using the
   setCursor(0 method:
   Although written to run on a 16x2 LCD it will run on a larger
   LCD (e.g. 20x4) without modification all be it that it will
   only use a limited part of the LCD display.
 
   The circuit:
   * LCD RS pin to digital pin 12
   * LCD Enable pin to digital pin 11
   * LCD D4 pin to digital pin 5
   * LCD D5 pin to digital pin 4
   * LCD D6 pin to digital pin 3
   * LCD D7 pin to digital pin 2
   * LCD R/W pin to ground
   * 10K potentiometer:
      * ends to +5V and ground
      * wiper to LCD VO pin (pin 3)
 
   Library originally added 18 Apr 2008 by David A. Mellis
   Library modified 5 Jul 2009 by Limor Fried (http://www.ladyada.net)
   Example added 9 Jul 2009 by Tom Igoe 
   Example modified 22 Nov 2010 by Tom Igoe
 
   The original example code is in the public domain.

   http://www.arduino.cc/en/Tutorial/LiquidCrystal
 
   Original Pinguino port by Marcus Fazzi <marcus@fazzi.eng.br>
   http://fazzi.eng.br
   Updated 01 Jun 2012  MFH updated program to use correct syntax,
                        put arguments for lcd.setCursor() in correct 
                        order and updated some of the comments. 
*/

// These constants won't change while the program is running but you
// can change their initial values to the size of your LCD display

   const u8 numCols = 16;
   const u8 numRows = 2;

void setup() 
{
// initialize the library with the numbers of the interface pins
// lcd.pins(RS, E, D0, D1, D2, D3, D4, D5, D6, D7); //8bits
// lcd.pins(RS, E, D4, D5, D6, D7,  0,  0,  0,  0); //4bits
  lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	

   // set up the LCD's number of rows & dotsize: 
   lcd.begin(numRows,0);
}

void loop() 
{
   u8 thisLetter, thisCol, thisRow;
   // loop from ASCII 'a' to ASCII 'z':
   for (thisLetter = 'a'; thisLetter <= 'z'; thisLetter++) {
      // loop over the columns:
      for (thisRow = 0; thisRow < numRows; thisRow++) {
         // loop over the rows:
         for (thisCol = 0; thisCol < numCols; thisCol++) {
            // set the cursor position:
            lcd.setCursor(thisCol,thisRow);
            // print the letter:
            lcd.write(thisLetter);
            delay(200);
         }
      }
   }
}


