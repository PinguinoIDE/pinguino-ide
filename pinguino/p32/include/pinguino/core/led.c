#ifndef led_c
#define led_c

#include <led.h>
#include <const.h>
#include <typedef.h>
#include <digitalw.c>


Led::Led(u8 pin)
{
	_pin = pin;
	pinmode(_pin, OUTPUT);
}

void Led::on()
{
	digitalwrite(_pin, ON);
}

void Led::off()
{
	digitalwrite(_pin, OFF);
}

void Led::toggle()
{
	digitalwrite(_pin, digitalread(_pin) ^ 1);
}

/*
void Led::dimmer()
{
	// pwm
}
*/

#endif	/* led_c */
