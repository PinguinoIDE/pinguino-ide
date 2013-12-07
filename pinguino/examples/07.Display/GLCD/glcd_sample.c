//
//

//

//


//
//

//
#define DBG 58

  unsigned long startMillis;
  unsigned int iter = 0;
  
void setup(){
	pinmode(DBG,OUTPUT);

	Init(NON_INVERTED);   // initialise the library
	ClearScreenX();  
	DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	Delayms(3000);
	ClearScreenX();
	SelectFont(System5x7, BLACK);       			// select fixed width system font
}

void loop(){ // run over and over again
	unsigned char i;  
	startMillis = millis();
	
	while( millis() - startMillis < 1000){ 				// loop for one second
		DrawRect(0, 0, 64, 61, BLACK); 			// rectangle in left side of screen
		DrawRoundRect(68, 0, 58, 61, 5, BLACK);  	// rounded rectangle around text area   
		for(i=0; i < 62; i += 4)
			DrawLine(1,1,63,i, BLACK);  			// draw lines from upper left down right side of rectangle  
		DrawCircle(32,31,30,BLACK);   				// draw circle centered in the left side of screen  
		FillRect(92,40,16,16, WHITE); 				// clear previous spinner position  
		CursorTo(5,5);               				// locate curser for printing text
//
		PrintNumber(++iter);         				// print current iteration at the current cursor position
	}
	digitalwrite(DBG,LOW);
	
	SelectFont(Arial_Bold_14, BLACK); 	// select truetype font
//
	ClearScreen(WHITE);               		// clear the screen  
//
	GotoXY(75,16);						// use GotoXY for truetype fonts	
	Puts("FPS= ");        				// print a text string  
	PrintNumber(iter);       			// print a number  
	SelectFont(System5x7, BLACK);
	iter = 0;
	
	digitalwrite(DBG,HIGH);
 }

