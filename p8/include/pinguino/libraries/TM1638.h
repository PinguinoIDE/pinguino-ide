/*
TM1638.h - Library for TM1638.

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

#ifndef TM1638_h
#define TM1638_h

/*#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
*/
//#include "TM16XX.c"
//#include "TM16XX.h"
//#include "TM16XXFonts.h"

#define TM1638_COLOR_NONE   0
#define TM1638_COLOR_RED    1
#define TM1638_COLOR_GREEN  2

//class TM1638 : public TM16XX
//{
//  public:
    /** Instantiate a tm1638 module specifying the display state, the starting intensity (0-7) data, clock and stobe pins. */
    //TM1638(char dataPin, char clockPin, char strobePin, char activateDisplay = true, char intensity = 7);
	void TM1638_init(char dataPin, char clockPin, char strobePin, char activateDisplay, char intensity);

	void TM1638_sendChar(char pos, char data, char dot);
	
	void TM1638_DisplayToDecNumberAt(unsigned long number, char dots, char startingPos, char leadingZeros, char *numberFont);
    /** Set the display to a unsigned hexadecimal number (with or without leading zeros) */
    void TM1638_setDisplayToHexNumber(unsigned long number, char dots, char leadingZeros, char *numberFont);
    /** Set the display to a unsigned decimal number (with or without leading zeros) */
    void TM1638_setDisplayToDecNumber(unsigned long number, char dots, char leadingZeros, char *numberFont);
    /** Set the display to a signed decimal number (with or without leading zeros) */
    void TM1638_setDisplayToSignedDecNumber(signed long number, char dots, char leadingZeros, char *numberFont);
    /** Set the display to a unsigned binary number */
    void TM1638_setDisplayToBinNumber(char number, char dots, char *numberFont);
    
    /** Set the LED at pos to color (TM1638_COLOR_RED, TM1638_COLOR_GREEN or both) */
    void TM1638_setLED(char color, char pos);
    /** Set the LEDs. MSB byte for the green LEDs, LSB for the red LEDs */
    void TM1638_setLEDs(unsigned int led);

    /** Returns the pressed buttons as a bit set (left to right). */
    int TM1638_getButtons();



    
//};
	/*char TM1638_displays;// = 8;
	char TM1638_dataPin;
	char TM1638_clockPin;
	char TM1638_strobePin; 
	char TM1638_activateDisplay;
	char TM1638_intensity;*/
#endif
