/*
  Row-Column Scanning an 8x8 LED matrix with X-Y input
 
 This example controls an 8x8 LED matrix using two analog inputs
 
 created 27 May 2009
 modified 4 Sep 2010
 by Tom Igoe
 
 This example works for the Lumex  LDM-24488NI Matrix. See 
 http://sigma.octopart.com/140413/datasheet/Lumex-LDM-24488NI.pdf
 for the pin connections
 
 For other LED cathode column matrixes, you should only need to change 
 the pin numbers in the row[] and column[] arrays
 
 rows are the anodes
 cols are the cathodes
 ---------
 
 Pin numbers:
 Matrix:
 * Digital pins 2 through 13,
 * analog pins 2 through 5 used as digital 16 through 19
 Potentiometers:
 * center pins are attached to analog pins 0 and 1, respectively
 * side pins attached to +5V and ground, respectively.
 
 This example code is in the public domain.
 
 http://www.arduino.cc/en/Tutorial/RowColumnScanning
 
 see also http://www.tigoe.net/pcomp/code/category/arduinowiring/514 for more
 */


//
const int row[8] = {
  2,7,19,5,13,18,12,16 };

//
const int col[8] = {
  6,11,10,3,17,4,8,9  };

//
int pixels[8][8];           

//
int x = 5;
int y = 5;

void readSensors() {
//
  pixels[x][y] = HIGH;
//
  x = 7 - map(analogRead(A0), 0, 1023, 0, 7);
  y = map(analogRead(A1), 0, 1023, 0, 7);
//
//
  pixels[x][y] = LOW;

}

void refreshScreen() {
  int thisRow;
  int thisCol;
  int thisPixel;
//
  for (thisRow = 0; thisRow < 8; thisRow++) {
//
    digitalwrite(row[thisRow], HIGH);
//
    for (thisCol = 0; thisCol < 8; thisCol++) {
//
      thisPixel = pixels[thisRow][thisCol];
//
//
      digitalwrite(col[thisCol], thisPixel);
//
      if (thisPixel == LOW) {
        digitalwrite(col[thisCol], HIGH);
      }
    }
//
    digitalwrite(row[thisRow], LOW);
  }
}

void setup() {
  int thisPin;
  int x, y;
  
  serial1init(9600);
//

//
  for (thisPin = 0; thisPin < 8; thisPin++) {
//
    pinmode(col[thisPin], OUTPUT); 
    pinmode(row[thisPin], OUTPUT);  
//
//
    digitalwrite(col[thisPin], HIGH);    
  }

//
  for (x = 0; x < 8; x++) {
    for (y = 0; y < 8; y++) {
      pixels[x][y] = HIGH;
    }
  }
}

void loop() {
//
  readSensors();

//
  refreshScreen();
}

