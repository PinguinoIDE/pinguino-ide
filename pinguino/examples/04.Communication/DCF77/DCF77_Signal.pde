
/*
typedef struct 										// defined in dcf77.h
{
	u8	seconds;
	u8	minutes;
	u8	hours;
	u8	dayofweek;
	u8	dayofmonth;
	u8	month;
	u8	year;
	u8	nosync;
} Time_Date_Format;
*/

#define DCF77Pin 0

char	DayW[8][4] = {"   ","Mon","Tue","Wed","Thu","Fri","Sat","Sun"};
u8	PrevSec = 0;

void setup(void)
{
	Serial.begin(9600);
	Serial.println("\r\n*  Pinguino DCF77 demo  *");
	pinMode(12, OUTPUT);
	DCF77.start(DCF77Pin);
}

void loop(void)
{
	if (digitalRead(DCF77Pin) == 1)
		digitalWrite(12, HIGH);
	else
		digitalWrite(12, LOW);

	if (RTClock.seconds != PrevSec)
	{
		PrevSec = RTClock.seconds;
		Serial.printf("Time: %02d:%02d:%02d    ", RTClock.hours, RTClock.minutes, RTClock.seconds);
		Serial.printf("Date: %s  %02d-%02d-%02d  ", DayW[RTClock.dayofweek], RTClock.dayofmonth, RTClock.month, RTClock.year);
		if (RTClock.nosync > 0)
			Serial.printf("No Sync for %2d min", RTClock.nosync);
		Serial.println(" ");
	}
}
