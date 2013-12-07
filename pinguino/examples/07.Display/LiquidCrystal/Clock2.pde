/*
 * LCD routines for use with pinguino board, based on LiquidCrystal lib from Arduino project.
 * By Marcus Fazzi (Anunakin)
 * Pins, Schematics and more info: 
 * http://pinguino.koocotte.org/index.php/LCD_Example
 * http://anunakin.blogspot.com/2010/08/pinguino-clock.html
 */
 
#define LED 13
#define HOUR 14
#define MIN 15

//global vars
u8 counter = 0;
u8 sec  = 0;
u8 min  = 0;
u8 hour = 0;

float num;
 
void setup(){
	//8 bits mode
	//lcd(8, 9, 0, 1, 2, 3, 4, 5, 6, 7); // RS, E, D0 ~ D7
	
	//4 bits mode, dont use last four pins ...
  lcd.pins(0, 1, 2, 3, 4, 5, 0, 0, 0, 0); // RS, E, D4 ~ D8	
	
	//LCD format
	lcd.begin(2, 0);
	
	/* Debug */
	pinMode(LED, OUTPUT);		// LED on pin 13
	
	//Setup buttons
	pinMode(HOUR, INPUT);
	pinMode(MIN, INPUT);
}
 
void loop()
{
	// Output string
	lcd.home();
	lcd.print("Pinguino-LCDlib");	// Output char
		
	if (counter>9){
		counter = 0;
		sec++;
	}
	
	if (sec>59){
		sec = 0;
		min++;
	}
	
	if (min>59){
		min = 0;
		hour++;
	}
	
	if (hour>23){
		hour = 0;
	}
	
	
	lcd.setCursor(0, 1);
	
	if (hour < 10)
		lcd.print("0");	
	lcd.printNumber(hour, 10);
	
	if (min < 10)
		lcd.print(":0");
	else
		lcd.print(":");		
	lcd.printNumber(min, 10);
	
	if (sec < 10)
		lcd.print(":0");
	else
		lcd.print(":");
	lcd.printNumber(sec, 10);	
	
	//set hour and min	
	if(digitalRead(HOUR)){
		hour++;
	}
	if(digitalRead(MIN)){
		min++;
	}
	
	counter++;
	
	delay(51); //just a fine tune to get 100ms with this!
	digitalWrite(LED, LOW);
	delay(51);
	digitalWrite(LED, HIGH);	
}
