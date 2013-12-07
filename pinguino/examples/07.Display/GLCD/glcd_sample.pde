// include <Corsiva12.h> 		// font definition for 12 point Corsiva font.
// include <Arial14.h>  		// font definition for 14 point Arial font.
#include <ArialBold14.h>	  	// font definition for 14 point Arial Bold font.
// include <Winks.h> 			// Huge 32pt Winks font.
#include <SystemFont5x7.h>   	// system font
// include <ArduinoIcon.h>   	// bitmap
#include <Pinguino.h>     		// bitmap  

//For slow displays uncoment line above... 14FPS with PIC32.
//#define SLOWDISPLAY()

//White LED on UBW32
#define DBG 58

  unsigned long startMillis;
  unsigned int iter = 0;
  
void setup(){
	pinMode(DBG,OUTPUT);

	GLCD.Init(NON_INVERTED);   // initialise the library
	GLCD.ClearScreenX();  
	GLCD.DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	delay(3000);
	GLCD.ClearScreenX();
	GLCD.SelectFont(System5x7, BLACK);       			// select fixed width system font
}

void loop(){ // run over and over again
	unsigned char i;  
	startMillis = millis();
	
	while( millis() - startMillis < 1000){ 				// loop for one second
		GLCD.DrawRect(0, 0, 64, 61, BLACK); 			// rectangle in left side of screen
		GLCD.DrawRoundRect(68, 0, 58, 61, 5, BLACK);  	// rounded rectangle around text area   
		for(i=0; i < 62; i += 4)
			GLCD.DrawLine(1,1,63,i, BLACK);  			// draw lines from upper left down right side of rectangle  
		GLCD.DrawCircle(32,31,30,BLACK);   				// draw circle centered in the left side of screen  
		GLCD.FillRect(92,40,16,16, WHITE); 				// clear previous spinner position  
		GLCD.CursorTo(5,5);               				// locate curser for printing text
		//GLCD.GotoXY(29,40);							// use GotoXY for truetype fonts use 0,8,16,...n*8
		GLCD.PrintNumber(++iter);         				// print current iteration at the current cursor position
	}
	digitalWrite(DBG,LOW);
	
	GLCD.SelectFont(Arial_Bold_14, BLACK); 	// select truetype font
	// display number of iterations in one second
	GLCD.ClearScreen(WHITE);               		// clear the screen  
	//CursorTo(7,2);           				// positon cursor, for fixed fonts, can use GotoXY too
	GLCD.GotoXY(75,16);						// use GotoXY for truetype fonts	
	GLCD.Puts("FPS= ");        				// print a text string  
	GLCD.PrintNumber(iter);       			// print a number  
	GLCD.SelectFont(System5x7, BLACK);
	iter = 0;
	
	digitalWrite(DBG,HIGH);
 }
