// ITDB02_Graph_Rotate_Bitmap (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://vivaphp.net
//
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the ITDB02_Graph (8bit mode).
//
//

#include "tux.c"

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.fillScr(255, 255, 255);
  myGLCD.setColor(0, 0, 0);
}

void loop()
{
	int i;
	
    for (i=0; i<360; i+=5)
    {
      myGLCD.drawBitmapR(100, 100, 32, 32, tux, i, 16, 16);
    }
}

