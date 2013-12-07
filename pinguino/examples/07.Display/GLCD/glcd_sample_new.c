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
   
//





//
//

//

//


  unsigned long startMillis;
  unsigned int iter = 0;
  
void setup(){
	Init(NON_INVERTED);   // initialise the library
	ClearScreenX();  
	DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	Delayms(3000);
	ClearScreenX();
	SelectFont(System5x7, BLACK);
	
}

void loop(){ // run over and over again
int i;
  startMillis = millis();
  while( millis() - startMillis < 1000){ // loop for one second
    DrawRect(0, 0, 64, 61, BLACK); // rectangle in left side of screen
    DrawRoundRect(68, 0, 58, 61, 5, BLACK);  // rounded rectangle around text area   
    for(i=0; i < 62; i += 4)
      DrawLine(1,1,63,i, BLACK);  // draw lines from upper left down right side of rectangle  
    DrawCircle(32,31,30,BLACK);   // draw circle centered in the left side of screen  
    FillRect(92,40,16,16, WHITE); // clear previous spinner position  
    CursorTo(5,5);               // locate curser for printing text
    PrintNumber(++iter);         // print current iteration at the current cursor position 
  } 
//
  ClearScreenX();               // clear the screen  
  CursorTo(13,2);              // positon cursor  
  Puts("FPS= ");               // print a text string  
  PrintNumber(iter);           // print a number 
  iter=0;
 }

