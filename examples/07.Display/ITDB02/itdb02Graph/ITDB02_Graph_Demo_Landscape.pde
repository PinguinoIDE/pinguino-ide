// ITDB02_Graph_Demo_Landscape (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://fazzi.eng.br
//
//
// This program is a demo of how to use the library with the
// ITDB02-2.4 module.
//
// This program requires the ITDB02_Graph (8bit mode).
//
//
#include "pinguino32X.c"

void setup(){
  srand(127);
  //Setup the LCD
  myGLCD.InitLCD(LANDSCAPE);
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
  myGLCD.fillScr(255, 255, 255);
  myGLCD.drawBitmap (5, 50, 313, 132, pinguino32X, 1);

  delay(3000);
  
// Clear the screen and draw the frame
  myGLCD.clrScr();
  
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 319, 13);
  myGLCD.setColor(64, 64, 64);
  myGLCD.fillRect(0, 226, 319, 239);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("*** ITDB02 - 240x320 Color Display ***", CENTER, 1, 0);
  //myGLCD.setColor(255, 128, 128);
  myGLCD.setColor(255,255,0);
  myGLCD.setBackColor(64, 64, 64);
  myGLCD.print("H.Karlsen/M.Fazzi(Pinguino32X)", LEFT, 227, 0);
  myGLCD.print("(C)2011", RIGHT, 227, 0);
  //myGLCD.setColor(255,255,0);
  //myGLCD.print("Landscape Mode", CENTER, 227, 0);

  myGLCD.setColor(0, 0, 255);
  myGLCD.drawRect(0, 14, 319, 225);

// Draw crosshairs
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);
  for (i=9; i<310; i+=10)
    myGLCD.drawLine(i, 117, i, 121);
  for (i=19; i<220; i+=10)
    myGLCD.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  myGLCD.setColor(0,255,255);
  myGLCD.print("Sin", 5, 15, 0);
  for (i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(int)(sin(((i*1.13)*3.14)/180)*95));
  }
  
  myGLCD.setColor(255,0,0);
  myGLCD.print("Cos", 5, 27, 0);
  for (i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(cos(((i*1.13)*3.14)/180)*95));
  }

  myGLCD.setColor(255,255,0);
  myGLCD.print("Tan", 5, 39, 0);
  for (i=1; i<318; i++)
  {
    myGLCD.drawPixel(i,119+(tan(((i*1.13)*3.14)/180)));
  }

  delay(2000);

//while(1){};

  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);
  myGLCD.setColor(0, 0, 255);
  myGLCD.setBackColor(0, 0, 0);
  myGLCD.drawLine(159, 15, 159, 224);
  myGLCD.drawLine(1, 119, 318, 119);

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
        myGLCD.setColor(0,0,255);
      else
        myGLCD.setColor(0,0,0);
      myGLCD.drawPixel(x,buf[x-1]);
    }
    myGLCD.setColor(0,255,255);
    y=119+(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    myGLCD.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled rectangles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRect(70+(i*20), 30+(i*20), 130+(i*20), 90+(i*20));
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled, rounded rectangles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillRoundRect(190-(i*20), 30+(i*20), 250-(i*20), 90+(i*20));
  }
  
  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some filled circles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD.setColor(255,0,255);
        break;
      case 2:
        myGLCD.setColor(255,0,0);
        break;
      case 3:
        myGLCD.setColor(0,255,0);
        break;
      case 4:
        myGLCD.setColor(0,0,255);
        break;
      case 5:
        myGLCD.setColor(255,255,0);
        break;
    }
    myGLCD.fillCircle(100+(i*20),60+(i*20), 30);
  }
  
//  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some lines in a pattern
  myGLCD.setColor (255,0,0);
  for (i=15; i<224; i+=5)
  {
    myGLCD.drawLine(1, i, i*2-10, 224);
  }
  myGLCD.setColor (255,0,0);
  for (i=224; i>15; i-=5)
  {
    myGLCD.drawLine(318, i, i*2-11, 15);
  }
  myGLCD.setColor (0,255,255);
  for (i=224; i>15; i-=5)
  {
    myGLCD.drawLine(1, i, 331-i*2, 15);
  }
  myGLCD.setColor (0,255,255);
  for (i=15; i<224; i+=5)
  {
    myGLCD.drawLine(318, i, 330-i*2, 224);
  }
  
  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,225);

// Draw some rand circles
  for (i=0; i<100; i++)
  {
    myGLCD.setColor(rnd(255), rnd(255), rnd(255));
    x=32+rnd(256);
    y=45+rnd(146);
    r=rnd(30);
    myGLCD.drawCircle(x, y, r);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some rand rectangles
  for (i=0; i<100; i++)
  {
    myGLCD.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(316);
    y=16+rnd(207);
    x2=2+rnd(316);
    y2=16+rnd(207);
    myGLCD.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

// Draw some rand rounded rectangles
  for (i=0; i<100; i++)
  {
    myGLCD.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(316);
    y=16+rnd(207);
    x2=2+rnd(316);
    y2=16+rnd(207);
    myGLCD.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

  for (i=0; i<100; i++)
  {
    myGLCD.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(316);
    y=16+rnd(209);
    x2=2+rnd(316);
    y2=16+rnd(209);
    myGLCD.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD.setColor(0,0,0);
  myGLCD.fillRect(1,15,318,224);

  for (i=0; i<10000; i++)
  {
    myGLCD.setColor(rnd(255), rnd(255), rnd(255));
    myGLCD.drawPixel(2+rnd(316), 16+rnd(209));
  }

  delay(2000);

  myGLCD.fillScr(0, 0, 255);
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRoundRect(80, 70, 239, 169);
  
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.print("That's it!", CENTER, 93, 0);
  myGLCD.print("Restarting in a", CENTER, 119, 0);
  myGLCD.print("few seconds...", CENTER, 132, 0);
  
  myGLCD.setColor(0, 255, 0);
  myGLCD.setBackColor(0, 0, 255);
  myGLCD.print("Runtime: (msecs)", CENTER, 210, 0);
  myGLCD.printNumI(millis(), CENTER, 225);
  
  delay (10000);
}

