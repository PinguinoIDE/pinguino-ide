/*
	Pinguino example to read ds18b20 1wire temperature sensor
	Result is sent on usb bus and can be read with temp18b20.py
	author		Régis Blanchot
	first release	14/09/2010
	last update	06/10/2010
	IDE			Pinguino b9.2

	DS18B20 Connection
	------------------
	pin 1: GND
	pin 2: DQ (Data in/out) must be connected to the PIC
	pin 3: VDD (+5V)
	NB : 1-wire bus (DQ line) must have 4K7 pull-up resistor (connected to +5V)
*/

#define ONEWIREBUS	0		// 1-wire bus is on pin 0 (RB0), just change it to suit you 

void setup()
{
	//
	//	find all sensors on the bus and get their Rom Code
	//
	
	//DS18B20.find(ONEWIREBUS);
}

void loop()
{
	TEMPERATURE t;
	u8 temp[4];
	// if you want to read temperature from sensor #1 with max. resolution :
	// if (DS18B20.read(ONEWIREBUS, 1, RES12BIT, &t))
	// if you have only one sensor on the bus, just skip rom detection
	if (DS18B20.read(ONEWIREBUS, SKIPROM, RES12BIT, &t))
	{
		// send temperature on usb bus
		temp[0] = t.sign;			// t.sign is unsigned  char (1 byte)
		temp[1] = t.integer;		// t.integer  is unsigned  char (1 byte)
		temp[2] = high8(t.fraction);	// t.fraction is unsigned int (2 bytes)
		temp[3] = low8(t.fraction);
		USB.send(temp, 4);			// send 4-bit temp on usb bus
		toggle(USERLED);		       // blinked led on pin 13 for visual debug
		delay(1000);			// wait for 1 sec. before next reading
	}
}
