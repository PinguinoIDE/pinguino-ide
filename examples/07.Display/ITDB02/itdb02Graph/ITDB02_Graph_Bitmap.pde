// ITDB02_Graph_Bitmap (C)2010 Henning Karlsen
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

#include "info.c"
#include "icon.c"
#include "tux.c"

void setup()
{
  GLCD.init(LANDSCAPE);
}

void loop()
{
  int x, y, s;
  
  GLCD.fillScr(255, 255, 255);
  GLCD.setColor(255, 255, 255);
  GLCD.print(" *** A 10 by 7 grid of a 32x32 icon *** ", CENTER, 228, 0);
  for (x=0; x<10; x++)
    for (y=0; y<7; y++)
      GLCD.drawBitmap (x*32, y*32, 32, 32, info, 1);

  delay(5000);
  
  GLCD.fillScr(255, 255, 255);
  GLCD.setColor(255, 255, 255);
  GLCD.print("   Two different icons in scale 1 to 4  ", CENTER, 228, 0);
  x=0;
  for (s=0; s<4; s++)
  {
    x+=(s*32);
    GLCD.drawBitmap(x, 0, 32, 32, tux, s+1);
  }
  x=0;
  for (s=4; s>0; s--)
  {
    GLCD.drawBitmap(x, 224-(s*32), 32, 32, icon, s);
    x+=(s*32);
  }

  delay(5000);
}

