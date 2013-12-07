/*	----------------------------------------------------------------------------
	Led Dimmer
	----------------------------------------------------------------------------
	author: 			Régis Blanchot
	description:	This example shows how to fade a LED connected to PWM pin
					      using PWM functions.
 	first release:25/02/2012
	last update:	25/02/2012
	pinguino ide:	> 9.5
	----------------------------------------------------------------------------
	wiring:			pin ----- +LED- ----- 330 Ohm Resistor ----- GND
	----------------------------------------------------------------------------
	NB:			PIC18 PINGUINO x550	 can only use pins	D11, D12
	    		PIC18 PINGUINO 26j50 can only use pins	D10, D11
					PIC32 PINGUINO		   can only use pins	D0, D1, D2
					PIC32 PINGUINO OTG	 can only use pins	D0, D1, D2
					PIC32 PINGUINO 220	 can only use pins	D2, D3, D11, D12, D13
	--------------------------------------------------------------------------*/

#define MYLED 10

u8 i = 0;
s8 dir;

void setup()
{
    // Frequency must be high to avoid blinking effect
    PWM.setFrequency(5000);
}

void loop()
{
    // Duty Cycle from 10% to 90% and back
    if (i > 90) dir = -1;
    if (i < 10) dir =  1;
    i = i + dir;
    // Duty Cycle is a percentage measure of the time that the LED is physically on.
    PWM.setPercentDutyCycle(MYLED, i);
    // Delay of 50 ms
    delay(50);
}
