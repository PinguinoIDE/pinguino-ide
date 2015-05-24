/*	----------------------------------------------------------------------------
	Simple Clock demo
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
 
// Global vars

u8 s  = 0;
u8 m  = 0;
u8 h = 0;

// This function is called every sec. by timer 0

void tick()
{
    s++;

    if (s > 59)
    {
        s = 0;
        m++;

        if (m > 59)
        {
            m = 0;
            h++;

            if (h > 23)
            {
                h = 0;
            }
        }
    }     
    lcd.setCursor(4, 1);
    lcd.printf("%02u:%02u:%02u", h, m, s);
}

void setup()
{
	// 4 bits mode, last four pins are not used
  lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	

	// LCD format
	lcd.begin(2, 0); // lines, dotsize

	lcd.home(); // 0, 0
	lcd.printf("Simple Clock Demo");
	
	// Timer0 call function tick() every sec.
	OnTimer0(tick, INT_MILLISEC, 1000);
}
 
void loop()
{
}
