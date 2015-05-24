// ITDB02_Graph_Demo_Landscape (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://fazzi.eng.br
//
//
// This program is a demo of how to use the library with the
// ITDB02-3.2 wide module (400x240).
//
// This program requires the ITDB02_Graph16 (16bit mode).
//
//

#include "pinguino32X.c"

void setup(){
  //srand(127);
  //Setup the LCD
  myGLCD16w.InitLCD(LANDSCAPE);
}

int rnd(int num) {
	return (int)(rand()%num);
	//return num;
}

void loop()
{
  int buf[398];
  int x, x2;
  int y, y2;
  int r, i;
  int temp;

  //Show pinguino logo! YEAH!
  myGLCD16w.fillScr(255, 255, 255);
  myGLCD16w.drawBitmap (45, 50, 313, 132, pinguino32X, 1);

  delay(2000);
  
// Clear the screen and draw the frame
  myGLCD16w.clrScr();
  
  myGLCD16w.setColor(255, 0, 0);
  myGLCD16w.fillRect(0, 0, 399, 13);
  myGLCD16w.setColor(64, 64, 64);
  myGLCD16w.fillRect(0, 226, 399, 239);
  myGLCD16w.setColor(255, 255, 255);
  myGLCD16w.setBackColor(255, 0, 0);
  myGLCD16w.print("*** ITDB02 3.2' 400x240 Color Display ***", CENTER, 1, 0);
  //myGLCD16w.setColor(255, 128, 128);
  myGLCD16w.setColor(255,255,0);
  myGLCD16w.setBackColor(64, 64, 64);
  myGLCD16w.print("H.Karlsen/M.Fazzi(Pinguino32X)", LEFT, 227, 0);
  myGLCD16w.print("(C)2011", RIGHT, 227, 0);
  //myGLCD16w.setColor(255,255,0);
  //myGLCD16w.print("Landscape Mode", CENTER, 227, 0);

  myGLCD16w.setColor(0, 0, 255);
  myGLCD16w.drawRect(0, 14, 399, 225);

// Draw crosshairs
  myGLCD16w.setColor(0, 0, 255);
  myGLCD16w.setBackColor(0, 0, 0);
  myGLCD16w.drawLine(159, 15, 159, 224);
  myGLCD16w.drawLine(1, 119, 398, 119);
  for (i=9; i<310; i+=10)
    myGLCD16w.drawLine(i, 117, i, 121);
  for (i=19; i<220; i+=10)
    myGLCD16w.drawLine(157, i, 161, i);

// Draw sin-, cos- and tan-lines  
  myGLCD16w.setColor(0,255,255);
  myGLCD16w.print("Sin", 5, 15, 0);
  for (i=1; i<398; i++){
    temp = (int)(sin(((i*1.13)*3.14)/180)*95);
    myGLCD16w.drawPixel(i,119+temp);
  }
  
  myGLCD16w.setColor(255,0,0);
  myGLCD16w.print("Cos", 5, 27, 0);
  for (i=1; i<398; i++){
    temp = (int)(cos(((i*1.13)*3.14)/180)*95);
    myGLCD16w.drawPixel(i,119+temp);
  }

  myGLCD16w.setColor(255,255,0);
  myGLCD16w.print("Tan", 5, 39, 0);
  for (i=1; i<398; i++){
    temp = (int)(tan(((i*1.13)*3.14)/180));
    myGLCD16w.drawPixel(i,119+temp);
  }

  delay(2000);

//while(1){};

  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);
  myGLCD16w.setColor(0, 0, 255);
  myGLCD16w.setBackColor(0, 0, 0);
  myGLCD16w.drawLine(159, 15, 159, 224);
  myGLCD16w.drawLine(1, 119, 398, 119);

// Draw a moving sinewave
  x=1;
  for (i=1; i<(398*20); i++) 
  {
    x++;
    if (x==399)
      x=1;
    if (i>399){
      if ((x==159)||(buf[x-1]==119))
        myGLCD16w.setColor(0,0,255);
      else
        myGLCD16w.setColor(0,0,0);
      myGLCD16w.drawPixel(x,buf[x-1]);
    }
    myGLCD16w.setColor(0,255,255);
    y=119+(int)(sin(((i*1.1)*3.14)/180)*(90-(i / 100)));
    myGLCD16w.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

// Draw some filled rectangles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD16w.setColor(255,0,255);
        break;
      case 2:
        myGLCD16w.setColor(255,0,0);
        break;
      case 3:
        myGLCD16w.setColor(0,255,0);
        break;
      case 4:
        myGLCD16w.setColor(0,0,255);
        break;
      case 5:
        myGLCD16w.setColor(255,255,0);
        break;
    }
    myGLCD16w.fillRect(110+(i*20), 30+(i*20), 170+(i*20), 90+(i*20));
  }

  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

// Draw some filled, rounded rectangles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD16w.setColor(255,0,255);
        break;
      case 2:
        myGLCD16w.setColor(255,0,0);
        break;
      case 3:
        myGLCD16w.setColor(0,255,0);
        break;
      case 4:
        myGLCD16w.setColor(0,0,255);
        break;
      case 5:
        myGLCD16w.setColor(255,255,0);
        break;
    }
    myGLCD16w.fillRoundRect(230-(i*20), 30+(i*20), 290-(i*20), 90+(i*20));
  }
  
  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

// Draw some filled circles
  for (i=1; i<6; i++)
  {
    switch (i)
    {
      case 1:
        myGLCD16w.setColor(255,0,255);
        break;
      case 2:
        myGLCD16w.setColor(255,0,0);
        break;
      case 3:
        myGLCD16w.setColor(0,255,0);
        break;
      case 4:
        myGLCD16w.setColor(0,0,255);
        break;
      case 5:
        myGLCD16w.setColor(255,255,0);
        break;
    }
    myGLCD16w.fillCircle(110+(i*30),60+(i*20), 30);
  }
  
//  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

// Draw some lines in a pattern
  myGLCD16w.setColor (255,0,0);
  for (i=15; i<224; i+=5)
  {
    myGLCD16w.drawLine(1, i, (i*1.44)-10, 224);
  }
  myGLCD16w.setColor (255,0,0);
  for (i=224; i>15; i-=5)
  {
    myGLCD16w.drawLine(398, i, (i*1.44)-11, 15);
  }
  myGLCD16w.setColor (0,255,255);
  for (i=224; i>15; i-=5)
  {
    myGLCD16w.drawLine(1, i, 331-(i*1.44), 15);
  }
  myGLCD16w.setColor (0,255,255);
  for (i=15; i<224; i+=5)
  {
    myGLCD16w.drawLine(398, i, 330-(i*1.44), 224);
  }
  
  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,225);

// Draw some rand circles
  for (i=0; i<100; i++)
  {
    myGLCD16w.setColor(rnd(255), rnd(255), rnd(255));
    x=32+rnd(338);
    y=45+rnd(146);
    r=rnd(30);
    myGLCD16w.drawCircle(x, y, r);
  }

  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

// Draw some rand rectangles
  for (i=0; i<100; i++)
  {
    myGLCD16w.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(396);
    y=16+rnd(207);
    x2=2+rnd(396);
    y2=16+rnd(207);
    myGLCD16w.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

// Draw some rand rounded rectangles
  for (i=0; i<100; i++)
  {
    myGLCD16w.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(396);
    y=16+rnd(207);
    x2=2+rnd(396);
    y2=16+rnd(207);
    myGLCD16w.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

  for (i=0; i<100; i++)
  {
    myGLCD16w.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(396);
    y=16+rnd(209);
    x2=2+rnd(396);
    y2=16+rnd(209);
    myGLCD16w.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.fillRect(1,15,398,224);

  for (i=0; i<10000; i++)
  {
    myGLCD16w.setColor(rnd(255), rnd(255), rnd(255));
    myGLCD16w.drawPixel(2+rnd(396), 16+rnd(209));
  }

  delay(2000);

  myGLCD16w.fillScr(0, 0, 255);
  myGLCD16w.setColor(255, 0, 0);
  myGLCD16w.fillRoundRect(120, 70, 279, 169);
  
  myGLCD16w.setColor(255, 255, 255);
  myGLCD16w.setBackColor(255, 0, 0);
  myGLCD16w.print("That's it!", CENTER, 93,0);
  myGLCD16w.print("Restarting in a", CENTER, 119,0);
  myGLCD16w.print("few seconds...", CENTER, 132,0);
  
  myGLCD16w.setColor(0, 255, 0);
  myGLCD16w.setBackColor(0, 0, 255);
  myGLCD16w.print("Runtime: (msecs)", CENTER, 210,0);
  myGLCD16w.printNumI(millis(), CENTER, 225);
  
  delay (10000);
}

