/*
Library examples for TM1638.

Copyright (C) 2011 Ricardo Batista <rjbatista at gmail dot com>

This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

void setup() {
 
    // define a module on data pin 5, clock pin 6 and strobe pin 7
    //TM16XX.init(5, 6, 7, 8, 1, 7);
    //TM16XX.setDisplayToError();
    //TM16XX.setDisplayDigit(0, 0, 0,NUMBER_FONT);

    //TM16XX.clearDisplay();
    // define a module on data pin 5, clock pin 6 and strobe pin 7, activate display, intensity
    TM1638.init(5, 6, 7, 1, 2);  
    
    //Examples
    TM16XX.setDisplayToString("Start", 0, 0,	FONT_DEFAULT);
    //TM16XX.setDisplayToDecNumberAt(1265478, 0, 0, 1, NUMBER_FONT);
    //TM1638.DisplayToDecNumberAt(994478, 0, 0, 1, NUMBER_FONT);
    //TM1638.setDisplayToDecNumber(12654078, 0, 1, NUMBER_FONT);          //ERROR
    //TM1638.setDisplayToSignedDecNumber(-1234, 0, 1, NUMBER_FONT);
    //TM1638.setDisplayToBinNumber(654, 0, NUMBER_FONT);
    //TM1638.setDisplayToHexNumber(0x1234ABCD, 0xF0, 1, NUMBER_FONT);
    
  
}

void loop() {
  

  // light the first 4 red LEDs and the last 4 green LEDs as the buttons are pressed
  //TM1638.setLEDs(((keys & 0xF0) << 8) | (keys & 0xF));
  //TM1638.setLED(1,1);
  int i;
  int ledtest = 7;
  for( i=0; i<=7; i++){
    int keys = TM1638.getButtons();
    TM16XX.clearDisplay();
    switch (keys){
        case 1:
                TM16XX.setDisplayToString("Button 1", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(1); 
            break;
        case 2:
                TM16XX.setDisplayToString("Button 2", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(3); 
            break;
        case 4:
                TM16XX.setDisplayToString("Button 3", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(7); 
            break;            
        case 8:
                TM16XX.setDisplayToString("Button 4", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(15); 
            break;
        case 16:
                TM16XX.setDisplayToString("Button 5", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(31); 
            break;           
        case 32:
                TM16XX.setDisplayToString("Button 6", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(63); 
            break;
        case 64:
                TM16XX.setDisplayToString("Button 7", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(127); 
            break;
        case 128:
                TM16XX.setDisplayToString("Button 8", 0, 0,	FONT_DEFAULT);
                TM1638.setLEDs(255); 
            break;
        default:
                TM1638.setLEDs(0);
        }
        
    
    //CDC.println("Keys: %c\r\n", keys);
    CDC.printf("character = %i \r\n", keys);
   /* TM1638.setLED(1,i);
    delay(300);
    TM1638.setLED(0,i);
    delay(300);
   */
  }
}
