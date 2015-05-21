//
#define DEBUG_LED 58

//
#define HOUR 68
#define MIN 69

//
#define RSEC 16
#define RMIN 12
#define RHOUR 8
   
//



//


//
u8 counter = 0;
u8 sec  = 0;
u8 min  = 0;
u8 hour = 0;

void setup(){
	/* Debug */
	pinmode(DEBUG_LED, OUTPUT);		// LED on pin 13
	
//
	pinmode(HOUR, INPUT);
	pinmode(MIN, INPUT);
	
//
	Init(NON_INVERTED);   // initialise the library
	ClearScreenX();  
	DrawBitmap(Pinguino, 32,0, BLACK); //draw the bitmap at the given x,y position
	Delayms(5000);
	ClearScreenX();	
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
	
	SelectFont(VerdanaNumbers, BLACK);
	GotoXY(3,0);
	
	if (hour < 10)
		PutChar('0');
	PrintNumber(hour);
	
	PutChar(':');
	if (min < 10)
		PutChar('0');
	PrintNumber(min);
	
	PutChar(':');
	if (sec < 10)
		PutChar('0');
	PrintNumber(sec);
	
//
	SelectFont(System5x7, BLACK);
	PrintNumber(counter);
	
//
	if(!(digitalread(HOUR))){
		hour++;
	}
	if(!(digitalread(MIN))){
		min++;
	}
				
//
	DrawCircle(21,43,18,BLACK);
//
	x = (int) RSEC * sinf(sec*0.10472);
	y = (int) RSEC * cosf(sec*0.10472);
	DrawLine(21,43,21+x,43-y, BLACK);
//
	x = (int) RMIN * sinf(min*0.10472);
	y = (int) RMIN * cosf(min*0.10472);
	DrawLine(21,43,21+x,43-y, BLACK);
//
	x = (int) RHOUR * sinf((hour>12?hour-12:hour)*0.5236);
	y = (int) RHOUR * cosf((hour>12?hour-12:hour)*0.5236);
	DrawLine(21,43,21+x,43-y, BLACK);
	
	DrawRect(45, 30, 82, 10, BLACK); 
	GotoXY(48,32);
	Puts("Pinguino 32X!");
	
//
	Delayms(40); //just a fine tune to get 100ms with this!
	digitalwrite(DEBUG_LED, LOW);
	Delayms(40);
	digitalwrite(DEBUG_LED, HIGH);
	
	if(!counter)
		ClearScreen(WHITE);         // clear the screen
	
	counter++;
 }
