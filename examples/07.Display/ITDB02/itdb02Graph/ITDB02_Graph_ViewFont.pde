// ITDB02_Graph_ViewFont (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://fazzi.eng.br
// This program is a demo of the included fonts.
//
// This program requires the ITDB02_Graph (8bit mode).
//
//
// *** This sketch requires that the large font is enabled for a correct display.

void setup()
{
  myGLCD.InitLCD(LANDSCAPE);
  myGLCD.clrScr();
}

void loop()
{
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 0);

  myGLCD.fontSize(FONT_LARGE);
  myGLCD.print(" !\"#$%&'()*+,-./", CENTER, 0,0);
  myGLCD.print("0123456789:;<=>?", CENTER, 16,0);
  myGLCD.print("@ABCDEFGHIJKLMNO", CENTER, 32,0);
  myGLCD.print("PQRSTUVWXYZ[\\]^_", CENTER, 48,0);
  myGLCD.print("`abcdefghijklmno", CENTER, 64,0);
  myGLCD.print("pqrstuvwxyz{|}~ ", CENTER, 80,0);

  myGLCD.fontSize(FONT_SMALL);
  myGLCD.print(" !\"#$%&'()*+,-./0123456789:;<=>?", CENTER, 150,0);
  myGLCD.print("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", CENTER, 162,0);
  myGLCD.print("`abcdefghijklmnopqrstuvwxyz{|}~ ", CENTER, 174,0);

  delay(500);
  myGLCD.clrScr();
  //while(1) {};
}

