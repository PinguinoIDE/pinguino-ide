#ifndef led_h
#define led_h

#include <typedef.h>

class Led
{
	private:
		u8 _pin;
	public:
		Led(u8);
		void on();
		void off();
		void toggle();
		//void dimmer();
};

#include <led.c>

#endif	/* led_h */
