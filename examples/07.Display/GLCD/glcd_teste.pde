// Include libraries used on you user lib, before include that, pinguino IDE works well on it.
// #include <delay.h>

// #include <digitalw.c>

//include "arial_bold_14.h"
//include "corsiva_12.h"

/**
   * Fonts for displat Text
   * If you get error: multiple sections using address 0x6500 
   * on load more then one truetype font,  try modify font.h file to load at another address, AT YOUR RISC!
   * "The whole range of flash memory 0 to 8000H . Be careful, bootloader is from 0 to 2000H and user program is from 2000H to ??
   *   So for security, i use flash starting at 6000H, with this configuration you can store 8192 bytes."
   *
   *   Fixed Fonts starts at 0x6000
   *   True Type Fonts at 0x6500
   *   Bitmaps at 0x7600
   */
// #include <Corsiva12.h> 		// font definition for 12 point Corsiva font.
// #include <Arial14.h>  		// font definition for 14 point Arial font.
#include <ArialBold14.h>	  	// font definition for 14 point Arial Bold font.
// #include <Winks.h> 			// Huge 32pt Winks font.
#include <SystemFont5x7.h>   	// system font
// include <ArduinoIcon.h>   	// bitmap
#include <Pinguino.h>     		// bitmap  

  unsigned long startMillis;
  unsigned int iter = 0;
  
void setup(){

	GLCD.Init(NON_INVERTED);   // initialise the library
	GLCD.ClearScreenX();  
	GLCD.DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	delay(5000);
	GLCD.ClearScreenX();
	//GLCD.SelectFont(System5x7, BLACK);			// select fixed width system font 
	//GLCD.SelectFont(Arial_14, BLACK);			// select truetype font
	//GLCD.SelectFont(Arial_Bold_14, BLACK); 	// select truetype font
	//GLCD.SelectFont(Corsiva_12, BLACK); 		// select truetype font
}

void loop(){ // run over and over again
	int i;  
	startMillis = millis();
  
	SelectFont(System5x7, BLACK);       // select fixed width system font
	while( millis() - startMillis < 1000){ 		// loop for one second
    DrawRect(0, 0, 64, 61, BLACK); 				// rectangle in left side of screen
    DrawRoundRect(68, 0, 58, 61, 5, BLACK);  	// rounded rectangle around text area   
    for(i=0; i < 62; i += 4)
      DrawLine(1,1,63,i, BLACK);  				// draw lines from upper left down right side of rectangle  
    DrawCircle(32,31,30,BLACK);   				// draw circle centered in the left side of screen  
    FillRect(92,40,16,16, WHITE); 				// clear previous spinner position  
    CursorTo(3,4);               				// locate curser for printing text
    GotoXY(29,40);								// use GotoXY for truetype fonts
    PrintNumber(++iter);         				// print current iteration at the current cursor position
  }

  SelectFont(Arial_Bold_14, BLACK); 	// select truetype font
  // display number of iterations in one second
  ClearScreenX();               		// clear the screen  
  //CursorTo(7,2);           			// positon cursor, for fixed fonts, can use GotoXY too
  GotoXY(75,24);						// use GotoXY for truetype fonts
  Puts("FPS= ");             			// print a text string  
  PrintNumber(iter);         			// print a number  
  iter = 0;
 }
