/*
TM16XX.cpp - Library implementation for TM16XX.

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

#include <TM16XX.h>
#include <digitalw.c>

void TM16XX_init(char dataPin, char clockPin, char strobePin, char displays, char activateDisplay, char intensity)
{
  int i;
  
  TM16XX_dataPin = dataPin;
  TM16XX_clockPin = clockPin;
  TM16XX_strobePin = strobePin;
  TM16XX_displays = displays;

  pinmode(dataPin, OUTPUT);
  pinmode(clockPin, OUTPUT);
  pinmode(strobePin, OUTPUT);
  
  digitalwrite(strobePin, HIGH);
  digitalwrite(clockPin, HIGH);

  TM16XX_sendCommand(0x40);
  TM16XX_sendCommand(0x80 | (activateDisplay ? 8 : 0) | min(7, intensity));

  digitalwrite(strobePin, LOW);
  TM16XX_send(0xC0);
   
  for (i = 0; i < 16; i++) {
    TM16XX_send(0x00);
  }
  digitalwrite(strobePin, HIGH);
}

/*****************************************************************/

void TM16XX_setupDisplay(char active, char intensity)
{
  TM16XX_sendCommand(0x80 | (active ? 8 : 0) | min(7, intensity));

  // necessary for the TM1640
  digitalwrite(TM16XX_strobePin, LOW);
  digitalwrite(TM16XX_clockPin, LOW);
  digitalwrite(TM16XX_clockPin, HIGH);
  digitalwrite(TM16XX_strobePin, HIGH);
}

void TM16XX_setDisplayDigit(char digit, char pos, char dot, const char *numberFont)
{
  TM16XX_sendChar(pos, numberFont[digit & 0xF], dot);
}

void TM16XX_setDisplayToError()
{
    int i;
	TM16XX_setDisplay(ERROR_DATA, 8);
	for (i = 8; i < TM16XX_displays; i++) {
	    TM16XX_clearDisplayDigit(i, 0);
	}
}

void TM16XX_clearDisplayDigit(char pos, char dot)
{
  TM16XX_sendChar(pos, 0, dot);
}

void TM16XX_setDisplay(char *values, unsigned int size)
{
  int i; for (i = 0; i < size; i++) {
    TM16XX_sendChar(i, values[i], 0);
  }
}

void TM16XX_clearDisplay()
{
  int i; 
  for (i = 0; i < TM16XX_displays; i++) {
    TM16XX_sendData(i << 1, 0);
  }
}

void TM16XX_setDisplayToString(const char* string, const word dots, const char pos, const char *font)
{
  int i; 
  for (i = 0; i < TM16XX_displays - pos; i++) {
  	if (string[i] != '\0') {
	  TM16XX_sendChar(i + pos, font[string[i] - 32], (dots & (1 << (TM16XX_displays - i - 1))) != 0);
	} else {
	  break;
	}
  }
}

/*void TM16XX_setDisplayToString(const String string, const word dots, const char pos, const char font[])
{
  int stringLength = string.length();

  int i; for (i = 0; i < TM16XX_displays - pos; i++) {
    if (i < stringLength) {
      TM16XX_sendChar(i + pos, font[string.charAt(i) - 32], (dots & (1 << (TM16XX_displays - i - 1))) != 0);
    } else {
      break;
    }
  }
}*/

void TM16XX_sendCommand(char cmd)
{
  digitalwrite(TM16XX_strobePin, LOW);
  TM16XX_send(cmd);
  digitalwrite(TM16XX_strobePin, HIGH);
}

void TM16XX_sendData(char address, char data)
{
  TM16XX_sendCommand(0x44);
  digitalwrite(TM16XX_strobePin, LOW);
  TM16XX_send(0xC0 | address);
  TM16XX_send(data);
  digitalwrite(TM16XX_strobePin, HIGH);
}

void TM16XX_send(char data)
{
  int i; 
  for (i = 0; i < 8; i++) {
    digitalwrite(TM16XX_clockPin, LOW);
    digitalwrite(TM16XX_dataPin, data & 1 ? HIGH : LOW);
    data >>= 1;
    digitalwrite(TM16XX_clockPin, HIGH);
  }
}

char TM16XX_receive()
{
  char temp = 0;
  int i;
  // Pull-up on
  pinmode(TM16XX_dataPin, INPUT);
  digitalwrite(TM16XX_dataPin, HIGH);

  for (i = 0; i < 8; i++) {
    temp >>= 1;

    digitalwrite(TM16XX_clockPin, LOW);

    if (digitalread(TM16XX_dataPin)) {
      temp |= 0x80;
    }

    digitalwrite(TM16XX_clockPin, HIGH);
  }

  // Pull-up off
  pinmode(TM16XX_dataPin, OUTPUT);
  digitalwrite(TM16XX_dataPin, LOW);

  return temp;
}

//#if !defined(ARDUINO) || ARDUINO < 100
// empty implementation instead of pure virtual for older Arduino IDE
void TM16XX_sendChar(char pos, char data, char dot) {
	TM16XX_sendData(pos << 1, data | (dot ? 0b10000000 : 0));
}
//#endif

void TM16XX_setDisplayToDecNumberAt(unsigned long number, char dots, char startingPos, char leadingZeros, char *numberFont)
{
  int i;
  if (number > 99999999L) {
    TM16XX_setDisplayToError();
  } else {
    for (i = 0; i < TM16XX_displays - startingPos; i++) {
      if (number != 0) {
        TM16XX_setDisplayDigit(number % 10, TM16XX_displays - i - 1, (dots & (1 << i)) != 0, numberFont);
        number = number / 10;
      } else {
		if (leadingZeros) {
		  TM16XX_setDisplayDigit(0, TM16XX_displays - i - 1, (dots & (1 << i)) != 0, numberFont);
		} else {
		  TM16XX_clearDisplayDigit(TM16XX_displays - i - 1, (dots & (1 << i)) != 0);
		}
      }
    }
  }
}

