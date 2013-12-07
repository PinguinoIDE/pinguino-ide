/**
   * Fonts for display Text
   * If you get error: multiple sections using address 0x6500 
   * on load more then one truetype font,  try modify font.h file to load at another address, AT YOUR RISC!
   * "The whole range of flash memory 0 to 8000H . Be careful, bootloader is from 0 to 2000H and user program is from 2000H to ??
   *   So for security, i use flash starting at 6000H, with this configuration you can store 8192 bytes."
   *
   *   Fixed Fonts starts at 0x6000
   *   True Type Fonts at 0x6500
   *   Bitmaps at 0x7500
   */
   
//Need include pinguino libraries and ks0108.c before include fonts and bitmaps, or cant use flashmemory.
#include <arduinodelay.c>
#include <millis.c>

#include <ks0108.c>

// include <Corsiva12.h> 		// font definition for 12 point Corsiva font.
// include <Arial14.h>  		// font definition for 14 point Arial font.
//#include <ArialBold14.h>	  	// font definition for 14 point Arial Bold font.
// include <Winks.h> 			// Huge 32pt Winks font.
#include <SystemFont5x7.h>   	// system font
// include <ArduinoIcon.h>   	// bitmap
#include <Pinguino.h>     		// bitmap  

  unsigned long startMillis;
  unsigned int iter = 0;
  
void setup(){
	GLCD.Init(NON_INVERTED);   // initialise the library
	GLCD.ClearScreenX();  
	GLCD.DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	delay(3000);
	GLCD.ClearScreenX();
	GLCD.SelectFont(System5x7, BLACK);
	
}

void loop(){ // run over and over again
int i;
  startMillis = millis();
  while( millis() - startMillis < 1000){ // loop for one second
    GLCD.DrawRect(0, 0, 64, 61, BLACK); // rectangle in left side of screen
    GLCD.DrawRoundRect(68, 0, 58, 61, 5, BLACK);  // rounded rectangle around text area   
    for(i=0; i < 62; i += 4)
      GLCD.DrawLine(1,1,63,i, BLACK);  // draw lines from upper left down right side of rectangle  
    GLCD.DrawCircle(32,31,30,BLACK);   // draw circle centered in the left side of screen  
    GLCD.FillRect(92,40,16,16, WHITE); // clear previous spinner position  
    GLCD.CursorTo(5,5);               // locate curser for printing text
    GLCD.PrintNumber(++iter);         // print current iteration at the current cursor position 
  } 
  // display number of iterations in one second
  GLCD.ClearScreenX();               // clear the screen  
  GLCD.CursorTo(13,2);              // positon cursor  
  GLCD.Puts("FPS= ");               // print a text string  
  GLCD.PrintNumber(iter);           // print a number 
  iter=0;
 }
