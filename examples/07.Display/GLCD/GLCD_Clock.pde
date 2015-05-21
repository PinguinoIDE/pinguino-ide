//Yellow LED
#define DEBUG_LED 58

//UBW32 User and Prog buttons
#define HOUR 68
#define MIN 69

//degrees
#define RSEC 16
#define RMIN 12
#define RHOUR 8
   
//FONTS
#include <VerdanaNumbers.h>	// Huge 28pt Verdana font. only Mumbers, save space!
#include <SystemFont5x7.h>   	// system font.

//Bitmap
#include <Pinguino.h>   		// bitmap  

//global vars
u8 counter = 0;
u8 sec  = 0;
u8 min  = 0;
u8 hour = 0;

void setup(){
	/* Debug */
	pinMode(DEBUG_LED, OUTPUT);		// LED on pin 13
	
	//Setup buttons
	pinMode(HOUR, INPUT);
	pinMode(MIN, INPUT);
	
	//Setup GLCD
	GLCD.Init(NON_INVERTED);   // initialise the library
	GLCD.ClearScreenX();  
	GLCD.DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	delay(5000);
	GLCD.ClearScreenX();	
}

void loop(){ // run over and over again	
	int x, y;
	
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
	
	GLCD.SelectFont(VerdanaNumbers, BLACK);
	GLCD.GotoXY(3,0);
	
	if (hour < 10)
		GLCD.PutChar('0');
	GLCD.PrintNumber(hour);
	
	GLCD.PutChar(':');
	if (min < 10)
		GLCD.PutChar('0');
	GLCD.PrintNumber(min);
	
	GLCD.PutChar(':');
	if (sec < 10)
		GLCD.PutChar('0');
	GLCD.PrintNumber(sec);
	
	//Display ticker centesimos de seconds (1/100) ...
	GLCD.SelectFont(System5x7, BLACK);
	GLCD.PrintNumber(counter);
	
	//set hour and min	
	if(!(digitalRead(HOUR))){
		hour++;
	}
	if(!(digitalRead(MIN))){
		min++;
	}
				
	//Relogio analógico?
	GLCD.DrawCircle(21,43,18,BLACK);
	//Second
	x = (int) RSEC * sinr(sec*0.10472);
	y = (int) RSEC * cosr(sec*0.10472);
	GLCD.DrawLine(21,43,21+x,43-y, BLACK);
	//Minute
	x = (int) RMIN * sinr(min*0.10472);
	y = (int) RMIN * cosr(min*0.10472);
	GLCD.DrawLine(21,43,21+x,43-y, BLACK);
	//Hour
	x = (int) RHOUR * sinr((hour>12?hour-12:hour)*0.5236);
	y = (int) RHOUR * cosr((hour>12?hour-12:hour)*0.5236);
	GLCD.DrawLine(21,43,21+x,43-y, BLACK);
	
	GLCD.DrawRect(45, 30, 82, 10, BLACK); 
	GLCD.GotoXY(48,32);
	GLCD.Puts("Pinguino 32X!");
	
	//DEBUG LED
	delay(40); //just a fine tune to get 100ms with this!
	digitalWrite(DEBUG_LED, LOW);
	delay(40);
	digitalWrite(DEBUG_LED, HIGH);
	
	if(!counter)
		GLCD.ClearScreen(WHITE);         // clear the screen
	
	counter++;
 }