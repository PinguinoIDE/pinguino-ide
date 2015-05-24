//--	watchdog-test.pde

void setup()
{
	Watchdog.Init();	// disabled by default in init()
	Watchdog.Clear();

	pinMode(GREENLED, OUTPUT);
	pinMode(YELLOWLED, OUTPUT);

	// turn off all led
	digitalWrite(GREENLED, LOW);
	digitalWrite(YELLOWLED, LOW);

	// led indicates pinguino reset
	// led indicates pinguino reset
	if (Watchdog.Event())	// by watchdog
		digitalWrite(YELLOWLED, HIGH);
	else			// normal
		digitalWrite(GREENLED, HIGH);
	delay(1000);

	digitalWrite(GREENLED, LOW);
	digitalWrite(YELLOWLED, LOW);

	// enable watchdog, start full wdt period
	Watchdog.Enable();
	Watchdog.Clear();
}

void loop()
{
	// blink led every second
	digitalWrite(GREENLED, HIGH);
	digitalWrite(YELLOWLED, HIGH);
	delay(500);
	digitalWrite(GREENLED, LOW);
	digitalWrite(YELLOWLED, LOW);
	delay(500);

	// watchdog should reset pinguino due to lack of clear
}
