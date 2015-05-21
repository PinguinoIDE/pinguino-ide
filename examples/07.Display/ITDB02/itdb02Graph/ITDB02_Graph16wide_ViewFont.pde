// ITDB02_Graph_ViewFont (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://fazzi.eng.br
//
// This program is a demo of the included fonts.
//
// This program requires the ITDB02_Graph wide 400x320 (16bit mode).
// This sketch requires that the large font is enabled for a correct display.

void setup(){
  myGLCD16w.InitLCD(LANDSCAPE);
  myGLCD16w.clrScr();
  //pinMode(BLUELED, OUTPUT);
}

void loop(){
  
  myGLCD16w.setColor(0, 255, 0);
  myGLCD16w.setBackColor(0, 0, 0);

  myGLCD16w.fontSize(FONT_LARGE);
  myGLCD16w.print(" !\"#$%&'()*+,-./", CENTER, 0,0);
  myGLCD16w.print("0123456789:;<=>?", CENTER, 16,0);
  myGLCD16w.print("@ABCDEFGHIJKLMNO", CENTER, 32,0);
  myGLCD16w.print("PQRSTUVWXYZ[\\]^_", CENTER, 48,0);
  myGLCD16w.print("`abcdefghijklmno", CENTER, 64,0);
  myGLCD16w.print("pqrstuvwxyz{|}~ ", CENTER, 80,0);

  myGLCD16w.fontSize(FONT_SMALL);
  myGLCD16w.print(" !\"#$%&'()*+,-./0123456789:;<=>?", CENTER, 150,0);
  myGLCD16w.print("@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_", CENTER, 162,0);
  myGLCD16w.print("`abcdefghijklmnopqrstuvwxyz{|}~ ", CENTER, 174,0);

  delay(500);
  //toggle(BLUELED);
  myGLCD16w.clrScr();
  //while(1) {};
}

