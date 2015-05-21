/*
TM1638.cpp - Library implementation for TM1638.

Copyright (C) 2011 Ricardo Batista (rjbatista <at> gmail <dot> com)

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

/*#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
*/

//#include "TM16XX.h"
#include "TM1638.h"
//#include <digitalw.c>
//#include "string.h"
#include "TM16XX.c"

void TM1638_init(char dataPin, char clockPin, char strobePin, char activateDisplay, char intensity) 
{
	TM16XX_displays = 8;
	TM16XX_init(dataPin, clockPin, strobePin, TM16XX_displays, activateDisplay, intensity);
}

void TM1638_setDisplayToHexNumber(unsigned long number, char dots, char leadingZeros, char *numberFont)
{
  int i;
  for (i = 0; i < TM16XX_displays; i++) {
	if (!leadingZeros && number == 0) {
		TM16XX_clearDisplayDigit(TM16XX_displays - i - 1, (dots & (1 << i)) != 0);
	} else {
		TM16XX_setDisplayDigit(number & 0xF, TM16XX_displays - i - 1, (dots & (1 << i)) != 0, numberFont);
		number >>= 4;
    }
  }
}

void TM1638_DisplayToDecNumberAt(unsigned long number, char dots, char startingPos, char leadingZeros, char *numberFont)
{
  TM16XX_setDisplayToDecNumberAt(number, dots, startingPos, leadingZeros, numberFont);
}

void TM1638_setDisplayToDecNumber(unsigned long number, char dots, char leadingZeros, char *numberFont)
{
	TM16XX_setDisplayToDecNumberAt(number, dots, 0, leadingZeros, numberFont);
}

void TM1638_setDisplayToSignedDecNumber(signed long number, char dots, char leadingZeros, char *numberFont)
{
	if (number >= 0) {
		TM16XX_setDisplayToDecNumberAt(number, dots, 0, leadingZeros, numberFont);
	} else {
		if (-number > 9999999L) {
		    TM16XX_setDisplayToError();
		} else {
			TM16XX_setDisplayToDecNumberAt(-number, dots, 1, leadingZeros, numberFont);
			TM1638_sendChar(0, MINUS, (dots & (0x80)) != 0);
			//TM16XX_sendData(pos << 1, data | (dot ? 0b10000000 : 0));
		}
	}
}

void TM1638_setDisplayToBinNumber(char number, char dots, char *numberFont)
{
  int i;
  for (i = 0; i < TM16XX_displays; i++) {
    TM16XX_setDisplayDigit((number & (1 << i)) == 0 ? 0 : 1, TM16XX_displays - i - 1, (dots & (1 << i)) != 0, numberFont);
  }
}

void TM1638_sendChar(char pos, char data, char dot)
{
	TM16XX_sendData(pos << 1, data | (dot ? 0b10000000 : 0));
}


void TM1638_setLED(char color, char pos)
{
    TM16XX_sendData((pos << 1) + 1, color);
}

void TM1638_setLEDs(unsigned int leds)
{
  int i;
  for (i = 0; i < TM16XX_displays; i++) {
    char color = 0;

    if ((leds & (1 << i)) != 0) {
      color |= TM1638_COLOR_RED;
    }

    if ((leds & (1 << (i + 8))) != 0) {
      color |= TM1638_COLOR_GREEN;
    }

    TM1638_setLED(color, i);
  }
}

int TM1638_getButtons(void)
{
  int i;
  int keys = 0;

  digitalwrite(TM16XX_strobePin, LOW);
  TM16XX_send(0x42);
  for (i = 0; i < 4; i++) {
    keys |= TM16XX_receive() << i;
  }
  digitalwrite(TM16XX_strobePin, HIGH);
  if (keys < -64) keys = 128;
  else keys = keys * -1;
  
  return keys;
}


