/*	----------------------------------------------------------------------------
	interrupt library demo
	----------------------------------------------------------------------------
	author:			Régis Blanchot
	first release:	19/12/2010
	last update:	24/12/2010
	pinguino ide:	9.5
 	boards:			8-bit ONLY
	wiring:			leds on pin 0 to 4 (RB0 to RB4)
					ex.: RB0 --- 470 Ohms ---<|--- GND
	--------------------------------------------------------------------------*/

#define myLED0	0
#define myLED1	1
#define myLED2	2
#define myLED3	3
//PINGUINO 26J50 Only
//#define myLED4	4

void blink0() {	toggle(myLED0); }
void blink1() {	toggle(myLED1); }
void blink2() {	toggle(myLED2); }
void blink3() {	toggle(myLED3); }
//PINGUINO 26J50 Only
//void blink4() {	toggle(myLED4); }

void setup()
{
	// Clear RB4 to RB0
	LATB  &= 0b11100000;
	PORTB &= 0b11100000;
	// RB4 to RB0 are OUTPUT
	TRISB &= 0b11100000;

	OnTimer0(blink0, INT_MILLISEC, 500);	// Use Timer0 to toggle pin 0 every 500 ms
	OnTimer1(blink1, INT_MILLISEC, 500);	// Use Timer1 to toggle pin 0 every 500 ms
	OnTimer2(blink2, INT_MILLISEC, 500);	// Use Timer2 to toggle pin 0 every 500 ms
	OnTimer3(blink3, INT_MILLISEC, 500);	// Use Timer3 to toggle pin 0 every 500 ms
  //PINGUINO 26J50 Only
 	//OnTimer4(blink4, INT_MILLISEC, 500);	// Use Timer4 to toggle pin 0 every 500 ms
}

void loop()
{
}
