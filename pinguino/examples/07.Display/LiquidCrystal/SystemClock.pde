/*	----------------------------------------------------------------------------
	System Clock Demo
	<rblanchot@gmail.com>
	----------------------------------------------------------------------------
	---------- LCD 2x16
	----------------------------------------------------------------------------
	01 - VSS (GND)
	02 - VDD (+5V)
	03 - Vo (R = 1K Ohm to GND)
	04 - RS (pin 8 in this ex.)
	05 - RW (GND = LOW = write mode)
	06 - EN (pin 9 in this ex.)
	07 a 10 - D0 a D3   (connected to GND in this ex.)
	11 a 16 - D4 to D7  (pin 0 to 3 in this ex.)
	15 - LED+ (R = 470 Ohm to +5V)
	16 - LED- (GND)
	---------------------------------------------------------------------------*/

void setup()
{
	// initialize the library with the numbers of the interface pins
	// lcd.pins(RS, E, D4, D5, D6, D7, 0, 0, 0, 0);		//4bits
	// lcd.pins(RS, E, D0, D1, D2, D3, D4, D5, D6, D7);	//8bits

	// UBW 32 mapping - 4 bits mode
	//lcd.pins(36, 37, 38, 39, 40, 41, 0, 0, 0, 0); // RS, E, D4 ~ D8	

	// PIC32-Pinguino(-OTG) - 4 bits mode
  lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	

	// LCD format
	lcd.begin(16, 2); // cols, lines, [dotsize]

	lcd.home(); // 0, 0
	lcd.printf("Sys.Clock: %02uMHz", System.getCpuFrequency()/1000000);
	lcd.setCursor(0, 1);
	lcd.printf("Per.Clock: %02uMHz", System.getPeripheralFrequency()/1000000);
}
 
void loop()
{
}
