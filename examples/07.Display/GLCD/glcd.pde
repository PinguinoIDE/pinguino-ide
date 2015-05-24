//DEBUG LED pin
#define LED 58

unsigned char x,y;

void setup(){
	//int i;
	/* Debug */	
	//pinMode(RUNLED, OUTPUT);
		
	//for (i=0; i<8; i++){
	//	pinMode(9, OUTPUT);
	//}
	x = 0;
	y = 0;
	
	pinMode(LED, OUTPUT);
	
	//delay(1000);
	// Initialize the LCD
	GLCD.Init(0);
	
	// Select a font
	//SelectFont(Arial_Bold_14, ks0108ReadFontData, BLACK);
	// Set a position
	GLCD.GotoXY(10,0);
	// Print some text
	//Puts("KS0108-Treiber");
	// a nice little round rect
	//DrawRoundRect(5, 5, 117, 20, 8, BLACK);
	
	//GotoXY(15,50);
	//DrawRoundRect(5, 30, 117, 20, 8, BLACK);
	
	// Once again :)
	// Select a font
	//SelectFont(Corsiva_12, ReadFontData, BLACK);
	// Set a position
	//GotoXY(5,30);
	// Print some text
	//Puts("http://www.apetech.de\nmailto:me@apetech.de");
}
 
void loop(){
	
	//GotoXY(15,10);
	//DrawRoundRect(5, 5, 117, 20, 8, BLACK);
	//GLCD.DrawCircle(x, 45, 10, WHITE);	
	x+=2;
	GLCD.DrawCircle(x, 45, 10, BLACK);
	
	
	digitalWrite(LED, LOW);
	delay(10);	
	
	
	if (x > 115){
		GLCD.DrawCircle(x, 20, 10, WHITE);
		x = 10;
	}
	GLCD.ClearScreen(WHITE);
	
	digitalWrite(LED, HIGH);
	delay(10);
}
