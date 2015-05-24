// ITDB02_Graph_Textrotation_Demo (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://vivaphp.net
//
//
// This program is a demo of the textrotation-functions.
//
// This program requires the ITDB02_Graph (8bit mode).
//
//



void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
  myGLCD.fontSize(FONT_LARGE);
}

void loop()
{
    myGLCD.print("Text rotation", 0, 0, 0);
    myGLCD.setColor(0, 0, 255);
    myGLCD.print("0 degrees", 0, 16, 0);
    myGLCD.print("90 degrees", 319, 0, 90);
    myGLCD.print("180 degrees", 319, 239, 180);
    myGLCD.print("270 degrees", 0, 239, 270);

    myGLCD.fontSize(FONT_SMALL);
    myGLCD.setColor(0, 255, 0);
    myGLCD.print("SMALL FONT - 45 DEGREES", 90, 50, 45);
    
    delay(500);
    myGLCD.clrScr();
    myGLCD.setColor(255, 255, 255);
    myGLCD.fontSize(FONT_LARGE);
}

