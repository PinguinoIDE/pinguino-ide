/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver.

 This sketch prints "Hello World!" to the LCD and shows the time.
 
 The circuit:
 * LCD RS pin to digital pin 0
 * LCD E  pin to digital pin 1
 * LCD D4 pin to digital pin 2
 * LCD D5 pin to digital pin 3
 * LCD D6 pin to digital pin 4
 * LCD D7 pin to digital pin 5
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 Pinguino port by Marcus Fazzi <marcus@fazzi.eng.br>
 http://fazzi.eng.br
 */

void setup()
{
    pinMode(USERLED, OUTPUT);
    // initialize the library with the numbers of the interface pins
    //lcd.pins(RS, E, D4, D5, D6, D7, 0, 0, 0, 0); //4bits
    //lcd.pins(RS, E, D0, D1, D2, D3, D4, D5, D6, D7); //8bits
    lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	

    // set up the LCD's number of columns and rows: 
    lcd.begin(16, 2);
    // Print a message to the LCD.
    lcd.print("Hello, World!");
}

void loop()
{
    u32 t,r;
    u8 d,h,m,s;
    static u8 temp;

    // Switch build-in led off
    digitalWrite(USERLED, LOW);
    
    t = millis() / 1000;   // time in sec.
    d = t / 86400;         // days
    r = t % 86400; 
    h = r / 3600;          // hours
    r = t % 3600;
    m = r / 60;            // minutes
    s = t % 60;            // seconds

    if (s != temp)
    {
        // save the current value of s
        temp = s;
        // Switch build-in led on
        digitalWrite(USERLED, HIGH);
        delay(100);
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0)
        lcd.setCursor(0, 1);
        // print time since reset
        lcd.printf("Uptime %02d:%02d:%02d", h,m,s);
    }
}
