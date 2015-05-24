/*
  LiquidCrystal Library - Hello World
 
 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the 
 Hitachi HD44780 driver.

 This sketch prints "Hello World!" to the LCD and shows the time.
 
 The circuit:
 * LCD RS pin to digital pin 12
 * LCD E  pin to digital pin 10
 * LCD D0 pin to digital pin 0
 * LCD D1 pin to digital pin 1
 * LCD D2 pin to digital pin 2
 * LCD D3 pin to digital pin 3
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground pin 11
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
 
 */

u16 ValPot;
u32 t;

void setup()
{
    pinMode(11, OUTPUT);        // CHRP3.0 LCD R/W is on pin 11
    digitalWrite(11, LOW);      // CHRP3.0 LCD Write mode
    pinMode(17, OUTPUT);        // CHRP3.0 LCD Backlight is on pin 18 (USERLED)
    digitalWrite(17, LOW);      // CHRP3.0 LCD Backlight On

    lcd.pins(12, 10, 0, 1, 2, 3, 4, 5, 6, 7); // RS, E, DB0 ~ DB7	
    lcd.begin(16, 2);
    //lcd.begin(2, 0);

    //lcd.home();
    lcd.print("Position Sensor");
}

void loop()
{
    t = millis()/1000;
    //lcd.clear;
    //ValPot = analogRead(2);
    lcd.setCursor(0, 1);
    //lcd.printf(" Valeur = %d ", ValPot);
    lcd.printf("Pinguino: %d", t);
    delay(500);
}
