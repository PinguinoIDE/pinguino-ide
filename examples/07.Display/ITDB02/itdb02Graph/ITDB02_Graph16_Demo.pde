// ITDB02_Graph_Demo (C)2010 Henning Karlsen
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

void setup()
{
  srand(234);
  // Setup the LCD
  myGLCD16.InitLCD(PORTRAIT);
}

int rnd(int num) {
	return (int)(rand()%num);
}

void loop()
{
  int buf[238];
  int x, x2;
  int y, y2;
  int r, i;

// Clear the screen and draw the frame
  myGLCD16.clrScr();

  myGLCD16.setColor(255, 0, 0);
  myGLCD16.fillRect(0, 0, 239, 13);
  myGLCD16.setColor(64, 64, 64);
  myGLCD16.fillRect(0, 306, 239, 319);
  myGLCD16.setColor(255, 255, 255);
  myGLCD16.setBackColor(255, 0, 0);
  myGLCD16.print("ITDB02 240x320 - Pinguino32X", CENTER, 1,0);
  //myGLCD16.setColor(255, 128, 128);
  myGLCD16.setColor(255,255,0);
  myGLCD16.setBackColor(64, 64, 64);
  myGLCD16.print("H.Karlsen/M.Fazzi", LEFT, 307,0);
  myGLCD16.print("(C)2011", RIGHT, 307,0);
  //myGLCD16.setColor(255,255,0);
  //myGLCD16.print("Pinguino", CENTER, 307,0);

  myGLCD16.setColor(0, 0, 255);
  myGLCD16.drawLine(0, 14, 239, 14);
  myGLCD16.drawLine(0, 14, 0, 305);
  myGLCD16.drawLine(239, 14, 239, 305);
  myGLCD16.drawLine(0, 305, 239, 305);

// Draw crosshairs
  myGLCD16.setColor(0, 0, 255);
  myGLCD16.setBackColor(0, 0, 0);
  myGLCD16.drawLine(119, 15, 119, 304);
  myGLCD16.drawLine(1, 159, 238, 159);
  for (i=9; i<240; i+=10)
    myGLCD16.drawLine(i, 157, i, 161);
  for (i=19; i<300; i+=10)
    myGLCD16.drawLine(117, i, 121, i);
  
// Draw sin-, cos- and tan-lines  
  myGLCD16.setColor(0,255,255);
  myGLCD16.print("Sin", 5, 15,0);
  for (i=1; i<238; i++)
  {
    myGLCD16.drawPixel(i,159+(sin(((i*1.51)*3.14)/180)*100));
  }
  
  myGLCD16.setColor(255,0,0);
  myGLCD16.print("Cos", 5, 27,0);
  for (i=1; i<238; i++)
  {
    myGLCD16.drawPixel(i,159+(cos(((i*1.51)*3.14)/180)*100));
  }

  myGLCD16.setColor(255,255,0);
  myGLCD16.print("Tan", 5, 39,0);
  for (i=1; i<238; i++)
  {
    myGLCD16.drawPixel(i,159+(tan(((i*1.51)*3.14)/180)));
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);
  myGLCD16.setColor(0, 0, 255);
  myGLCD16.setBackColor(0, 0, 0);
  myGLCD16.drawLine(119, 15, 119, 304);
  myGLCD16.drawLine(1, 159, 238, 159);

// Draw a moving sinewave
  x=1;
  for (i=1; i<6902; i++)
  {
    x++;
    if (x==239)
      x=1;
    if (i>239)
    {
      if ((x==119)||(buf[x-1]==159))
        myGLCD16.setColor(0,0,255);
      else
        myGLCD16.setColor(0,0,0);
      myGLCD16.drawPixel(x,buf[x-1]);
    }
    myGLCD16.setColor(0,255,255);
    y=159+(sin(((i*1.4)*3.14)/180)*(100-(i / 100)));
    myGLCD16.drawPixel(x,y);
    buf[x-1]=y;
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

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
    myGLCD16.fillRect(30+(i*20), 70+(i*20), 90+(i*20), 130+(i*20));
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

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
    myGLCD16.fillRoundRect(150-(i*20),70+(i*20), 210-(i*20), 130+(i*20));
  }
  
  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

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
    myGLCD16.fillCircle(60+(i*20),100+(i*20), 30);
  }
  
  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

// Draw some lines in a pattern
  myGLCD16.setColor (255,0,0);
  for (i=15; i<304; i+=5)
  {
    myGLCD16.drawLine(1, i, (i/1.22)-10, 304);
  }
  myGLCD16.setColor (255,0,0);
  for (i=304; i>15; i-=5)
  {
    myGLCD16.drawLine(238, i, (i/1.22)-11, 15);
  }
  myGLCD16.setColor (0,255,255);
  for (i=304; i>15; i-=5)
  {
    myGLCD16.drawLine(1, i, 251-(i/1.22), 15);
  }
  myGLCD16.setColor (0,255,255);
  for (i=15; i<304; i+=5)
  {
    myGLCD16.drawLine(238, i, 250-(i/1.22), 304);
  }
  
  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

// Draw some rand circles
  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=32+rnd(175);
    y=45+rnd(212);
    r=rnd(30);
    myGLCD16.drawCircle(x, y, r);
  }

  delay(2000);

  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

// Draw some rand rectangles
  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(236);
    y=16+rnd(287);
    x2=2+rnd(236);
    y2=16+rnd(287);
    myGLCD16.drawRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

// Draw some rand rounded rectangles
  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(236);
    y=16+rnd(287);
    x2=2+rnd(236);
    y2=16+rnd(287);
    myGLCD16.drawRoundRect(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(2,16,238,304);

  for (i=0; i<100; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    x=2+rnd(236);
    y=16+rnd(289);
    x2=2+rnd(236);
    y2=16+rnd(289);
    myGLCD16.drawLine(x, y, x2, y2);
  }

  delay(2000);
  
  myGLCD16.setColor(0,0,0);
  myGLCD16.fillRect(1,15,238,304);

  for (i=0; i<10000; i++)
  {
    myGLCD16.setColor(rnd(255), rnd(255), rnd(255));
    myGLCD16.drawPixel(2+rnd(236), 16+rnd(289));
  }

  delay(2000);

  myGLCD16.fillScr(0, 0, 255);
  myGLCD16.setColor(255, 0, 0);
  myGLCD16.fillRoundRect(30, 100, 209, 219);
  
  myGLCD16.setColor(255, 255, 255);
  myGLCD16.setBackColor(255, 0, 0);
  myGLCD16.print("That's it!", CENTER, 133,0);
  myGLCD16.print("Restarting in a", CENTER, 159,0);
  myGLCD16.print("few seconds...", CENTER, 172,0);
  
  myGLCD16.setColor(0, 0, 0);
  myGLCD16.setBackColor(0, 0, 255);
  myGLCD16.print("Runtime: (msecs)", CENTER, 290,0);
  myGLCD16.printNumI(millis(), CENTER, 305);
  
  delay (10000);
}

