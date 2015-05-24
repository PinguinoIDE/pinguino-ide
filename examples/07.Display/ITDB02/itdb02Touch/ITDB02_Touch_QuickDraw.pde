// ITDB02_Touch_QuickDraw (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://vivaphp.net
//
// This program is a quick demo of how to use the library.
//
// This program requires the ITDB02_Graph library (8bit mode)
// or ITDB02_Graph16 (16bit mode).
//
// It is assumed that the ITDB02 module is connected to a
// ITDB02 Shield, a ITDB02 Mega Shield or that you know how 
// to change the pin numbers in the setup.
//


void setup()
{
  myTouch(11,12,13,14,15);
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();

  myTouch.InitTouch(PORTRAIT);
  myTouch.setPrecision(PREC_MEDIUM);
  
  pinMode(63, OUTPUT);
  pinMode(64, OUTPUT);
  
  srand(millis());
}

void loop()
{
	u16 x, y;

	myGLCD.setColor(rand()%255,rand()%255,rand()%255);
    while (myTouch.dataAvailable() == 1)
    {
      myTouch.read();
      x = myTouch.getX();
      y = myTouch.getY();
      
      //myGLCD.drawPixel(x,y);
      myGLCD.fillCircle(x,y, 3);
      //myGLCD.drawLine(x, y, x+5, y+5);
      
      digitalWrite(63, LOW);
      digitalWrite(64, HIGH);
    }
    digitalWrite(63, HIGH);
    digitalWrite(64, LOW);
}

