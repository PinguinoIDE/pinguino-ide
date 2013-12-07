//
#define LED 58

unsigned char x,y;

void setup(){
//
	/* Debug */	
//
		
//
//
//
	x = 0;
	y = 0;
	
	pinmode(LED, OUTPUT);
	
//
//
	Init(0);
	
//
//
//
	GotoXY(10,0);
//
//
//
//
	
//
//
	
//
//
//
//
//
//
//
}
 
void loop(){
	
//
//
//
	x+=2;
	DrawCircle(x, 45, 10, BLACK);
	
	
	digitalwrite(LED, LOW);
	Delayms(10);	
	
	
	if (x > 115){
		DrawCircle(x, 20, 10, WHITE);
		x = 10;
	}
	ClearScreen(WHITE);
	
	digitalwrite(LED, HIGH);
	Delayms(10);
}

