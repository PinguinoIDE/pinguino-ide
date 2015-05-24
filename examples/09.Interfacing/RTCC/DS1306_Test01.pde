/*
 * File			DS1306_Test01.pde
 *
 * Description		Basic test program for DS1306 devices. 
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License		This software is released under the terms of the Mozilla Public License (MPL) version 2.0
 * 			Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 * Instructions		Connect USB port and open a terminal emulator.
 *
 *   18/06/2013 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it )
*/

//RTC IO
#define RTC_E          4
#define SCK            5
#define SDO            6
#define SDI            7


void setup()
{
 pinMode(USERLED, OUTPUT); 
 digitalWrite(USERLED,LOW); 
 DS1306.init(0,RTC_E,SCK,SDO,SDI); 
}

char Riga[50];
ds1306time MyTime; 
u8 val;

void loop()
{
 DS1306.getTime(&MyTime);
 Riga[0] = MyTime.seconds;
 Riga[1] = 0;
 DS1306.writeUser(0x40,Riga,1);
 Riga[0] = 0;
 Riga[1] = 0;
 DS1306.readUser(0x40,Riga,1);
 CDC.printf("Time %02d:%02d:%02d - UserWrite %02d\n\r",MyTime.hours,MyTime.minutes,MyTime.seconds,Riga[0]); 
 //Toggle built-in LED
 toggle(USERLED);
 delay(1000);
}