/*	-----------------------------------------------------------------------
 Pinguino example to read 2 ds18b20 1wire temperature sensor in blocking and non-blocking mode.
 In asyncronous mode is possible use the acquisition time for other job
	Result is sent on usb-serial bus and can be read with index.php
	author			Moreno Manzini
	first release	        10/06/2013
	last update		10/06/2013
	IDE				Pinguino > b9.5
	-----------------------------------------------------------------------
	DS18B20 wiring
	-----------------------------------------------------------------------
	pin 1: GND
	pin 2: DQ (Data in/out) must be connected to the PIC
	pin 3: VDD (+5V)
	NB : 1-wire bus (DQ line) must have 4K7 pull-up resistor (connected to +5V)
	-----------------------------------------------------------------------
	Data's are sent to /dev/ttyACM0
	Make sure you have persmission on it : sudo chmod 777 /dev/ttyACM0
	Maybe you will have to add your user name to the dialup group
	----------------------------------------------------------------------*/

#define ONEWIREBUS1  20						// DQ line						
#define ONEWIREBUS2  19						// DQ line						

  u32 TimemS0,TimemS1;
  
   
void setup()
{
}


void loop()
{
	TEMPERATURE t;
 
  // Acquire and read operations on 2 sensors in blocking mode, take about 1200mS
  TimemS0 = millis();
	if (DS18B20.read(ONEWIREBUS1, SKIPROM, RES12BIT, &t))
	{
		if (t.sign) CDC.printf("-");
		CDC.printf("CH1 %d.%d°C \r\n", t.integer, t.fraction);
	}
	if (DS18B20.read(ONEWIREBUS2, SKIPROM, RES12BIT, &t))
	{
		if (t.sign) CDC.printf("-");
		CDC.printf("CH2 %d.%d°C \r\n", t.integer, t.fraction);
	}
  TimemS1 = millis() - TimemS0;
	CDC.printf("Time Norm %d mS \r\n", TimemS1);
	CDC.printf("-------------------------------------------\r\n");





  //-------------------------------------------------------------------------------------------------------------------------
  
  
  
 
  // Acquire and read operations on 2 sensors in non-blocking mode, take about 20mS
  TimemS0 = millis();
	DS18B20.startMeasure(ONEWIREBUS1, SKIPROM, RES12BIT);
	DS18B20.startMeasure(ONEWIREBUS2, SKIPROM, RES12BIT);
  TimemS1 = millis() - TimemS0;
	CDC.printf("Time A %d mS \r\n", TimemS1);
 
 
 
 
  //  Do your job here, minimum delay 750mS on a 12bit acquiring temperature, there are no upper limits on the duration of the delay
  // START 
	delay(5000);
  // END
 
     
        
  TimemS0 = millis();
	if (DS18B20.readMeasure(ONEWIREBUS1, SKIPROM, &t))
	{
		if (t.sign) CDC.printf("-");
		CDC.printf("CH1q %d.%d°C \r\n", t.integer, t.fraction);
	}
	if (DS18B20.readMeasure(ONEWIREBUS2, SKIPROM, &t))
	{
		if (t.sign) CDC.printf("-");
		CDC.printf("CH2q %d.%d°C \r\n", t.integer, t.fraction);
	}
  TimemS1 = millis() - TimemS0;
	CDC.printf("Time B %d mS \r\n", TimemS1);
	CDC.printf("-------------------------------------------\r\n");
}
