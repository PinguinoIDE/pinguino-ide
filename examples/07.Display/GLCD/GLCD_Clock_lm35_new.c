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
   *
   */

//
//
//

//
#define PIC18F4550

#define USEDATAD()

//
#define HOUR 2
#define MIN 3

//
#define SENSOR 13

//
#define RSEC 16
#define RMIN 12
#define RHOUR 8

//
#define I2C_address 0x68



//

//

//
uchar counter = 0;
uchar sec  = 0;
uchar min  = 0;
uchar hour = 0;
uchar sensor = 0;

void setup(){
//
	pinmode(HOUR, INPUT);
	pinmode(MIN, INPUT);
	
//
	pinmode(SENSOR, INPUT);
		
//
	Init(NON_INVERTED);   // initialise the library
	ClearScreenX();	
}

void loop(){ // run over and over again	
	char x, y;	
	uchar tempC;
	
	if (counter>9){		
		counter = 0;
		sec++;
	}
	
//
	sensor = analogRead(SENSOR);
	
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
	
	DrawRoundRect(63, 28, 50, 30, 10, BLACK); 
//
//
//
	
	tempC  = (int)(0.48876 * sensor);
//
	
	SelectFont(VerdanaNumbers, BLACK);
	GotoXY(70,32);
	PrintNumber(tempC);
//
	Puts(";");

//
	Delayms(25); //just a fine tune to get 100ms with this!
	
	if(!counter)
		ClearScreenX();         // clear the screen
	
	counter++;
 }

