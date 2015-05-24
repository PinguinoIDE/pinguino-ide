/*	----------------------------------------------------------------------------
	IR Obstacle Detector
	----------------------------------------------------------------------------
	author:			Régis Blanchot
	first release:	19/12/2010
	last update:	24/12/2010
	pinguino ide:	9.5
	wiring:			tsop1738.gif
	link:				http://www.datelec.fr/detectir/p0.htm
	----------------------------------------------------------------------------
	* Carrier frequency should be center on 38 KHz (with a TSOP1738)
		- Fc = 38 KHz
		- 1 cycle = 1/38 KHz = 26 us 
	* Burst length should be between 10 cycles and 70 cycles
		- 260 us < Burst length < 1820 us
	* After each burst a gap time of at least 14 cycles is neccessary
		- Gaptime > 364 us
	* For each burst which is longer than 1.8ms a corresponding gap time is necessary
		- if Burst length > 1820 us then Gaptime = Burst length
	* Up to 1400 short bursts per second can be received continuously
	--------------------------------------------------------------------------*/

/// This program is only for 8-bit Pinguino
 
#define IR_LEFT_LED		11	// Left IR Led is connected on pin #11
#define IR_RIGHT_LED		12	// Right IR Led is connected on pin #12
#define IR_RECEIVER		0 	// TSOP1738 is connected on pin #0

#define GAPTIME1	0
#define LEFT		1
#define GAPTIME2	2
#define RIGHT		3

u8 phase = 0;

// Turn the IR LEDs on or off.
void IR_emitterOn(u8 pin)
{
	// infrared LEDs have a typical forward voltage drop of 1.5V
	// We take 3.5V / 430Ω to find our peak current of 8.1mA.
	// If we set our PWM duty-cycle to 20%,
	// we then have an average current of 1.6mA supplied to each LED.
	PWM.setPercentDutyCycle(pin, 25);// duty cycle = 25%
}

void IR_emitterOff(u8 pin)
{
	PWM.setPercentDutyCycle(pin, 0);// duty cycle = 25%
}

// Send burst on every timer interrupt.
// sequence : pause, left, pause, right
void IR_transmission()
{
	phase++;
	if (phase > 3) phase = 0;

	switch (phase)
	{
		case LEFT:
			IR_emitterOff(IR_RIGHT_LED);
			IR_emitterOn(IR_LEFT_LED);
			break;
		case RIGHT:
			IR_emitterOff(IR_LEFT_LED);
			IR_emitterOn(IR_RIGHT_LED);
			break;
		default:
			IR_emitterOff(IR_RIGHT_LED);
			IR_emitterOff(IR_LEFT_LED);
			break;
	}
}

void IR_reception()
{
	switch (phase)
	{
		case LEFT:
			Serial.printf("Obstacle on the left\n");
			break;
		case RIGHT:
			Serial.printf("Obstacle on the right\n");
			break;
		default:
			Serial.printf("Get Signal from another IR source\n");
			break;
	}
}

void setup()
{
	Serial.begin(9600);
	Serial.printf("************************\n");
	Serial.printf("* IR Obstacle Detector *\n");
	Serial.printf("************************\n");

	PWM.setFrequency(38000);								// Use Timer2 to make a 38 KHz carrier frequency
	OnTimer0(IR_transmission, INT_MICROSEC, 500);	// Use Timer0 to send burst every 500 us
	OnChangePin0(IR_reception, INT_FALLING_EDGE);	// Goes to 0 if a carrier is received
}

void loop()
{
}
