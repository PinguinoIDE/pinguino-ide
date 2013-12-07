/*
    Project: Using TC74 for temperature measurement
	Author:  regis blanchot
	first release : 20-05-2013
	last update   : 20-05-2013
    NB : SDA and SCL require a pullup resistor to VCC
    (typical 10 kΩ for 100 kHz, 2 kΩ for 400 kHz)
    
    Pinguino    x550    x6j50
    SDA         DO      D5
    SCL         D1      D4
*/
 
/*
    I2C addresses
*/ 
//const u8 TC74A0 = 0b1001000;
const u8 TC74A3 = 0b1001011;
const u8 LCDI2C = 0b0100111;

u8 num;
char temperature[] = " 000  C";

void setup()
{
    pinMode(12, OUTPUT);                // Pinguino 26j50 build-in led is on pin 12
    I2C.master(I2C_100KHZ);             // or I2C.master(100);
    lcdi2c.init(16, 2, LCDI2C);         // display is 2x16
    lcdi2c.backlight();                 // turns backlight on
    lcdi2c.clear();                     // clear screen
    lcdi2c.home();                      // set cursor at (0,0)
    lcdi2c.setCursor(2, 0);             // set cursor at col 1, line 1
    lcdi2c.printf("Testing TC74");
    lcdi2c.setCursor(1, 1);             // set cursor at col 1, line 2
    lcdi2c.printf("Thermal sensor");
    delay(2000);
    lcdi2c.clear();                     // clear screen
}

void loop()
{
    digitalWrite(12, 1);                // Led on
    I2C.start();
    if (I2C.write((TC74A3<<1)&0xFE))
    {
        // Read temperature
        
        I2C.write(0x00);                    // Read Temp
        I2C.restart();                      // Issue start signal
        I2C.write((TC74A3<<1)+1);            // Address + Read bit
        num = I2C.read();

        // Check for negative temperature

        if (num > 127)
        {
            temperature[0] = '-';
            num = ~num +1;
        }
        else
        {
            temperature[0] = '+';
        }
        
        temperature[1] = num/100 + 48;
        temperature[2] = (num/10)%10 + 48;
        temperature[3] = num%10 + 48;
        temperature[5] = 223;

        // Eliminate 0s at beginning
        
        if (temperature[1] == '0')
        {
            temperature[1] = ' ';
            if (temperature[2] == '0')
                temperature[2] = ' ';
        }

        // Display result
        
        lcdi2c.setCursor(6, 0);             // set cursor at col 6, line 0
        lcdi2c.printf("TC74");
        lcdi2c.setCursor(4, 1);             // set cursor at col 4, line 2
        lcdi2c.printf("%s", temperature);
    }
    else
    {
        lcdi2c.setCursor(0, 1);             // set cursor at col 0, line 2
        lcdi2c.printf("Device not found");
    }

    I2C.stop();
    digitalWrite(12, 0);                // Led off
    delay(5000);
}
