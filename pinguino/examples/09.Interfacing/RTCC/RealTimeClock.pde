/*	----------------------------------------------------------------------------
	FILE:  			RealTimeClock.pde
	PROJECT: 		Pinguino
	PURPOSE: 		Real Time Clock and Calendar functions Demo
	PROGRAMER:		regis blanchot <rblanchot@gmail.com>
	DOCUMENTATION:	mark harper
	BOARD:			PIC32-PINGUINO			(OLIMEX)
						PIC32-PINGUINO-OTG	(OLIMEX)
						or any other PIC32MX board with external 32.768 kHz clock crystal
	FIRST RELEASE:	11 Apr 2011
	LAST RELEASE:	07 Nov 2011
	--------------------------------------------------------------------------*/

char	Day[7][5]		= {"Sat","Sun","Mon","Tue","Wed","Thu","Fri"};
char	Month[13][5]	= {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

int	LoopDelay		= 500;	// Used to slow down loop so that can see
										// cat /dev/ttyACM0 output more easily
										// 500 corresponds to two lines of output per second
void blink1()
{
	toggle(USERLED);
}

void setup()
{
	u32 Tm  	 = 0x23580000; 	// 23hr, 58 min, 00 sec
	u32 Dt  	 = 0x11123100;		// Saturday (day 0 of the week), 31 Dec 2011
	u32 aTm 	 = 0x01091500;		// 00hr, 09 min, 15 sec
	u32 aDt 	 = 0x12010101;		// Sunday (day 1 of the week), 01 Jan 2012 (Year ignored)
										// Alarm time chosen to illustrate different behaviour of
										// alarmRepeatEveryTenSeconds, alarmRepeatEveryMinute and
										// alarmRepeatEveryTenMinutes, alarmRepeatEveryHour,
										// For the Tm, Dt, Atm & ADt values given above, the first alarm
										// occurs after 5 secs, 15 secs, 1 min 15 secs or 11 min 15 
										// depending on which of the above four alarmRepeats is chosen 
	u16 drift = 200;				// add 200 pulse every minute to adjust time
	
/**	-----------------------------------------------------------------------------
	alarm will toggle the built-in led
	---------------------------------------------------------------------------*/
	pinMode(USERLED, OUTPUT);
	digitalWrite(USERLED, OFF);	// we start with led off
	
/**	-----------------------------------------------------------------------------
	set date, time and calibration
	---------------------------------------------------------------------------*/
	/// 1st option
	RTCC.open(Tm, Dt, drift);
	/// 2nd option
/*
	RTCC.setTimeDate(Tm, Dt);
	RTCC.setCalibration(drift);
*/
	/// 3rd option
/*	
	RTCC.setTime(Tm);
	RTCC.setDate(Dt);
	RTCC.setCalibration(drift);
*/
/**	-----------------------------------------------------------------------------
	set alarm date and time
	---------------------------------------------------------------------------*/
	/// 1st option
	RTCC.setAlarmTimeDate(aTm, aDt);
	
	/// 2nd option
	/*
	RTCC.setAlarmTime(aTm);
	RTCC.setAlarmDate(aDt);
	*/
/**	-----------------------------------------------------------------------------
	set alarm repeat
	---------------------------------------------------------------------------*/
	/// 1st option
/*
	RTCC.chimeEnable();					   // enable indefinite repeats
*/
	/// 2nd option
	RTCC.chimeDisable();					   // disable indefinite repeats
	RTCC.setAlarmRepeat(10);			// repeat alarm 10 times = 10 interrupt events
													// after this number of repeats the RTCC pin and hence LED1
													// stops changing.  The time it takes to reach this condition
													// depends on the alarmRepeat used (see below).
	//// Alternative 1 for 2nd Option
	RTCC.alarmEveryTenSeconds();	// set repeat every 10 secs (only compare the seconds last digit)
													// with this statement enabled the first change in the alarm
													// output will occurr within 10 seconds of the clock starting
													// and then the output will change every 10 seconds later.
	//// Alternative 2 for 2nd Option
//	RTCC.alarmEveryMinute();		// set repeat every minute  (only compare the seconds)
													// with this statement enabled the first change in the alarm
													// output will occurr within 1 minute of the clock starting
													// and then the output will change every minute later.
	//// Alternative 3 for 2nd Option
//	RTCC.alarmEveryTenMinutes();	// set repeat every 10 mins (only compare minutes last digit and seconds)
													// with this statement enabled the first change in the alarm
													// output will occurr within 10 minutess of the clock starting
													// and then the output will change every 10 minutes later.
	//// Alternative 4 for 2nd Option
//	RTCC.alarmEveryHour();			// set repeat every hour (only compare minutes and seconds)
													// with this statement enabled the first change in the alarm
													// output will occurr within 1 hour of the clock starting
													// and then the output will change every hour later.
	
/**	-----------------------------------------------------------------------------
	1st method: set alarm output
	---------------------------------------------------------------------------*/
	RTCC.alarmPulseOutput();			   // select the alarm pulse as the function of the RTCC output pin
// Only available with 32-bit Pinguino
//	RTCC.alarmInitialPulseLow();		   // start Low 
//	RTCC.alarmInitialPulseHigh();			// start High
	RTCC.outputEnable();					   // enable the Output pin of the RTCC
	
/**	-----------------------------------------------------------------------------
	2nd method: alarm interrupt
	---------------------------------------------------------------------------*/
	//RTCC.alarmIntEnable();				// enable alarm interrupt
	
/**	-----------------------------------------------------------------------------
	enable alarm
	---------------------------------------------------------------------------*/
	RTCC.alarmEnable();						// enable alarm
	
/**	-----------------------------------------------------------------------------
	Flash LED to show setup complete
	---------------------------------------------------------------------------*/
	blink1();									// Very short flash

}

void loop()
{
	u8 status;
	rtccTime cT, aT;
	rtccDate cD, aD;
	rtccTime curTime, alTime;
	rtccDate curDate, alDate;

/**	-----------------------------------------------------------------------------
	get time and date
	---------------------------------------------------------------------------*/
	// 1st option
	RTCC.getTimeDate(&cT, &cD);

	// 2nd option
/*
	RTCC.getTime(&cT);
	RTCC.getDate(&cD);
*/
/**	-----------------------------------------------------------------------------
	get alarm time and date
	---------------------------------------------------------------------------*/
	// 1st option
	RTCC.getAlarmTimeDate(&aT, &aD);

 	// 2nd option
/*
	RTCC.getAlarmTime(&aT);
	RTCC.getAlarmDate(&aD);
*/

/**	-----------------------------------------------------------------------------
	convert time and date from bcd to decimal format
	---------------------------------------------------------------------------*/
	//curTime = RTCC.convertTime(&cT);
	//curDate = RTCC.convertDate(&cD);
/**	-----------------------------------------------------------------------------
	convert alarm time and date
	---------------------------------------------------------------------------*/
	//alTime  = RTCC.convertTime(&aT);
	//alDate  = RTCC.convertDate(&aD);
/**	-----------------------------------------------------------------------------
	display time  and date
	see the result with cat /dev/ttyACM0
	---------------------------------------------------------------------------*/
	CDC.printf("RTC now is ");
	CDC.printf("%03s, %02d %03s %04d, ", Day[curDate.dayofweek], curDate.dayofmonth, Month[curDate.month], curDate.year+2000);
	CDC.printf("%02d:%02d:%02d  ", curTime.hours, curTime.minutes, curTime.seconds);
/**	-----------------------------------------------------------------------------
	display alarm time and alarm date
	see the result with cat /dev/ttyACM0 
	---------------------------------------------------------------------------*/
	CDC.printf("Alarm is ");
	CDC.printf("%03s, %02d %03s, ", Day[alDate.dayofweek], alDate.dayofmonth, Month[alDate.month]);
	CDC.printf("%02d:%02d:%02d  ", alTime.hours, alTime.minutes, alTime.seconds);
/**	-----------------------------------------------------------------------------
	alarm output management
	---------------------------------------------------------------------------*/
	status = digitalRead(RTCC);		// RTCC pin
 	digitalWrite(LED1, status);	   // LED1 reproduces RTCC pin state
/**	-----------------------------------------------------------------------------
	include status (=alarm output) on display
	see the result with cat /dev/ttyACM0 
	---------------------------------------------------------------------------*/
	CDC.printf("Status = %01d \r\n", status);
	delay(LoopDelay);                // Slows looping rate
/**	-----------------------------------------------------------------------------
	alarm interrupt management
	---------------------------------------------------------------------------*/
/*
	OnRTCC(blink1);
*/
}
