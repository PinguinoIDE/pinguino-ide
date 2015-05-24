// ITDB02_Graph_Demo_Landscape (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://fazzi.eng.br
//
//
// This program is a demo of how to use the library with the
// ITDB02-3.2 module.
//
// This program requires the ITDB02_Graph16 (16bit mode).
//
//

#include "pinguino32X.c"

void setup(){
  srand(127);
  //Setup the LCD
  myGLCD16.InitLCD(LANDSCAPE);
}

int rnd(int num) {
	return (int)(rand()%num);
	//return num;
}

void loop()
{
  int buf[318];
  int x, x2;
  int y, y2;
  int r, i;

  //Show pinguino logo! YEAH!
  myGLCD16.fillScr(255, 255, 255);
  myGLCD16.drawBitmap (5, 50, 313, 132, pinguino32X, 1);

  delay(2000);
  
// Clear the screen and draw the frame
  myGLCD16.clrScr();
  
  myGLCD16.setColor(255, 0, 0);
  myGLCD16.fillRect(0, 0, 319, 13);
  myGLCD16.setColor(64, 64, 64);
  myGLCD16.fillRect(0, 226, 319, 239);
  myGLCD16.setColor(255, 255, 255);
  myGLCD16.setBackColor(255, 0, 0);
  myGLCD16.print("*** ITDB02 - 320x240 Color Display ***", CENTER, 1, 0);
  //myGLCD16.setColor(255, 128, 128);
  myGLCD16.setColor(255,255,0);
  myGLCD16.setBackColor(64, 64, 64);
  myGLCD16.print("H.Karlsen/M.Fazzi(Pinguino32X)", LEFT, 227, 0);
  myGLCD16.print("(C)2011", RIGHT, 227, 0);
  //myGLCD16.setColor(255,255,0);
  //myGLCD16.print("Landscape Mode", CENTER, 227, 0);

  myGLCD16.setColor(0, 0, 255);
  myGLCD16.drawRect(0, 14, 319, 225);

// Draw crosshairs
  myGLCD16.setColor(0, 0, 255);
  myGLCD16.setBackColor(0, 0, 0);
  myGLCD16.drawLine(159, 15, 159, 224);
  myGLCD16.drawLine(1, 119, 318, 119);
  for (i=9; i<310; i+=10)
    myGLCD16.drawLine(i, 117, i, 121);
  for (i=19; i<220; i+=10)
    myGLCD16.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  myGLCD16.setColor(0,255,255);
  myGLCD16.print("Sin", 5, 15, 0);
  for (i=1; i<318; i++)
  {
    myGLCD16.drawPixel(i,119+(sin(((i*1.13)*3.14)/180)*95));
  }
  
  myGLCD16.setColor(255,0,0);
  myGLCD16.print("Cos", 5, 27, 0);
  for (i=1; i<318; i++)
  {
    myGLCD16.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
  }

  myGLCD16.setColor(255,255,0);
  myGLCD16.print("Tan", 5, 39, 0);
  for (i=1; i<318; i++)
  {
    myGLCD16.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
  }

  delay(2000);

//while(1){};

  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);
  myGLCD16.setColor(0, 0, 255);
  myGLCD16.setBackColor(0, 0, 0);
  myGLCD16.drawLine(159, 15, 159, 224);
  myGLCD16.drawLine(1, 119, 318, 119);

// Draw a moving sinewave
  x=1;
  for (i=1; i<(318*20); i++) 
  {
    x++;
    if (x==319)
      x=1;
    if (i>319)
    {
      if ((x==159)||(buf[x-1]==119))
        myGLCD16.setColor(0,0,255);
      else
        myGLCD16.setColor(0,0,0);
      myGLCD16.drawPixel(x,buf[x-1]);
    }
    myGLCD16.setColor(0,255,255);
    y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    myGLCD16.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

// Draw some filled rectangles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD16.setColor(255,0,255);
        break;
      case 2:
        myGLCD16.setColor(255,0,0);
        break;
      case 3:
        myGLCD16.setColor(0,255,0);
        break;
      case 4:
        myGLCD16.setColor(0,0,255);
        break;
      case 5:
        myGLCD16.setColor(255,255,0);
        break;
    }
    myGLCD16.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

// Draw some filled, rounded rectangles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD16.setColor(255,0,255);
        break;
      case 2:
        myGLCD16.setColor(255,0,0);
        break;
      case 3:
        myGLCD16.setColor(0,255,0);
        break;
      case 4:
        myGLCD16.setColor(0,0,255);
        break;
      case 5:
        myGLCD16.setColor(255,255,0);
        break;
    }
    myGLCD16.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
  }
  
  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

// Draw some filled circles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD16.setColor(255,0,255);
        break;
      case 2:
        myGLCD16.setColor(255,0,0);
        break;
      case 3:
        myGLCD16.setColor(0,255,0);
        break;
      case 4:
        myGLCD16.setColor(0,0,255);
        break;
      case 5:
        myGLCD16.setColor(255,255,0);
        break;
    }
    myGLCD16.fillCircle(100+(i*20),60+(i*20), 30);
  }
  
//  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

// Draw some lines in a pattern
  myGLCD16.setColor (255,0,0);
  for (i=15; i<224; i+=5)
  {
    myGLCD16.drawLine(1, i, (i*1.44)-10, 224);
  }
  myGLCD16.setColor (255,0,0);
  for (i=224; i>15; i-=5)
  {
    myGLCD16.drawLine(318, i, (i*1.44)-11, 15);
  }
  myGLCD16.setColor (0,255,255);
  for (i=224; i>15; i-=5)
  {
    myGLCD16.drawLine(1, i, 331-(i*1.44), 15);
  }
  myGLCD16.setColor (0,255,255);
  for (i=15; i<224; i+=5)
  {
    myGLCD16.drawLine(318, i, 330-(i*1.44), 224);
  }
  
  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,225);

// Draw some rand circles
  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=32+rnd(256);
    y=45+rnd(146);
    r=rnd(30);
    myGLCD16.drawCircle(x, y, r);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

// Draw some rand rectangles
  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(316);
    y=16+rnd(207);
    x2=2+rnd(316);
    y2=16+rnd(207);
    myGLCD16.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

// Draw some rand rounded rectangles
  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(316);
    y=16+rnd(207);
    x2=2+rnd(316);
    y2=16+rnd(207);
    myGLCD16.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(316);
    y=16+rnd(209);
    x2=2+rnd(316);
    y2=16+rnd(209);
    myGLCD16.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,318,224);

  for (i=0; i<10000; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    myGLCD16.drawPixel(2+rnd(316), 16+rnd(209));
  }

  delay(2000);

  myGLCD16.fillScr(0, 0, 255);
  myGLCD16.setColor(255, 0, 0);
  myGLCD16.fillRoundRect(80, 70, 239, 169);
  
  myGLCD16.setColor(255, 255, 255);
  myGLCD16.setBackColor(255, 0, 0);
  myGLCD16.print("That's it!", CENTER, 93, 0);
  myGLCD16.print("Restarting in a", CENTER, 119, 0);
  myGLCD16.print("few seconds...", CENTER, 132, 0);
  
  myGLCD16.setColor(0, 255, 0);
  myGLCD16.setBackColor(0, 0, 255);
  myGLCD16.print("Runtime: (msecs)", CENTER, 210, 0);
  myGLCD16.printNumI(millis(), CENTER, 225);
  
  delay (10000);
}

