// ITDB02_Touch_Calibration (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program can be used to calibrate the touchscreen
// of the ITDB02 modules.
// This program requires the ITDB02_Graph library (8bit mode)
// or ITDB02_Graph16 (16bit mode).
//
// Instructions will be given on the display.
//

int cx, cy;
int rx[10], ry[10];
float px, py;
int ox, oy;

void setup()
{
  //UBW32 and EMPEROR boards
  //myTouch.setTouch(56,57,58,59,60); //clk, cs, din, dout, irq
  
  //OLIMEX Pinguino32X board
  myTouch.setTouch(15,10,14,9,8); //clk, cs, din, dout, irq
  
  myGLCD.InitLCD(PORTRAIT);
  myGLCD.clrScr();

  myTouch.InitTouch(PORTRAIT);
  myTouch.setPrecision(PREC_LOW);
}

void drawCrossHair(int x, int y)
{
  myGLCD.drawRect(x-10, y-10, x+10, y+10);
  myGLCD.drawLine(x-5, y, x+5, y);
  myGLCD.drawLine(x, y-5, x, y+5);
}

void readCoordinates()
{
  int iter = 2000;
  int cnt = 0;
  unsigned long tx=0;
  unsigned long ty=0;
  u8 OK = 0;
  
  while (OK == 0)
  {
    while (myTouch.dataAvailable() == 0) {}
    while ((myTouch.dataAvailable() == 1) && (cnt<iter))
    {
      myTouch.read();
      tx += TP_X;
      ty += TP_Y;
      cnt++;
    }
    if (cnt>=iter)
    {
      OK = 1;
    }
    else
    {
      tx = 0;
      ty = 0;
      cnt = 0;
    }
  }

  cx = tx / iter;
  cy = ty / iter;

}

void calibrate(int x, int y, int i)
{
  myGLCD.setColor(255, 255, 255);
  drawCrossHair(x,y);
  readCoordinates();
  myGLCD.setColor(80, 80, 80);
  drawCrossHair(x,y);
  
  rx[i]=cx;
  ry[i]=cy;
  while (myTouch.dataAvailable() == 1)
  {
    myTouch.read();
  }
}

void waitForTouch()
{
  while (myTouch.dataAvailable() == 1)
  {
    myTouch.read();
  }
  while (myTouch.dataAvailable() == 0) {}
  while (myTouch.dataAvailable() == 1)
  {
    myTouch.read();
  }
}

void startup()
{
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 239, 13);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, 239, 14);
  myGLCD.print("ITDB02 TOUCH CALIBRATION", CENTER, 1, 0);
  myGLCD.setBackColor(0, 0, 0);
  
  myGLCD.print("INSTRUCTIONS", CENTER, 30, 0);
  myGLCD.print("Use a stylus or something", LEFT, 50, 0);
  myGLCD.print("similar to touch as close to", LEFT, 62, 0);
  myGLCD.print("the center of the highlighted", LEFT, 74, 0);
  myGLCD.print("crosshair as possible. Keep as", LEFT, 86, 0);
  myGLCD.print("still as possible and keep", LEFT, 98, 0);
  myGLCD.print("holding until the highlight is", LEFT, 110, 0);
  myGLCD.print("removed. Repeat for all", LEFT, 122, 0);
  myGLCD.print("crosshairs in sequence.", LEFT, 134, 0);
  myGLCD.print("Further instructions will be", LEFT, 158, 0);
  myGLCD.print("displayed when the calibration", LEFT, 170, 0);
  myGLCD.print("is complete.", LEFT, 182, 0);
  myGLCD.print("Do NOT use your finger as a", LEFT, 206, 0);
  myGLCD.print("calibration stylus or the", LEFT, 218, 0);
  myGLCD.print("result WILL BE very imprecise.", LEFT, 230, 0);
  myGLCD.print("Touch screen to continue", CENTER, 305, 0);

  waitForTouch();
  myGLCD.clrScr();
}

void done()
{
  myGLCD.clrScr();
  myGLCD.setColor(255, 0, 0);
  myGLCD.fillRect(0, 0, 239, 13);
  myGLCD.setColor(255, 255, 255);
  myGLCD.setBackColor(255, 0, 0);
  myGLCD.drawLine(0, 14, 239, 14);
  myGLCD.print("ITDB02 TOUCH CALIBRATION", CENTER, 1, 0);
  myGLCD.setBackColor(0, 0, 0);
  
  myGLCD.print("CALIBRATION COMPLETE", CENTER, 30, 0);
  myGLCD.print("To use the new calibration", LEFT, 50, 0);
  myGLCD.print("settings you must edit the", LEFT, 62, 0);
  myGLCD.setColor(160, 160, 255);
  myGLCD.print("itdb02_touch.c", LEFT, 74, 0);
  myGLCD.setColor(255, 255, 255);
  myGLCD.print("file and", 136, 74, 0);
  myGLCD.print("change the following values.", LEFT, 86, 0);
  myGLCD.print("The values are located right", LEFT, 98, 0);
  myGLCD.print("below the opening comment in", LEFT, 110, 0);
  myGLCD.print("the file.", LEFT, 122, 0);
  myGLCD.print("PixSizeX", LEFT, 158, 0);
  myGLCD.print("PixOffsX", LEFT, 170, 0);
  myGLCD.print("PixSizeY", LEFT, 182, 0);
  myGLCD.print("PixOffsY", LEFT, 194, 0);
  myGLCD.print("Connected module:", LEFT, 250, 0);
  myGLCD.drawLine(0, 155, 239, 155);
  myGLCD.drawLine(0, 209, 239, 209);

  myGLCD.printNumI((int)px, 100, 158);
  myGLCD.printNumI((int)ox, 100, 170);
  myGLCD.printNumI((int)py, 100, 182);
  myGLCD.printNumI((int)oy, 100, 194);
  if (px>=0)
    myGLCD.print("2.4\"", 144, 250, 0);
  else
  {
    if (py>=0)
    {
      myGLCD.print("3.2\"", 144, 250, 0);
      myGLCD.print("Negative numbers for PixSizeX", LEFT, 270, 0);
      myGLCD.print("is expected :)", LEFT, 282, 0);
    }
    else
    {
      myGLCD.print("3.2\" Wide", 144, 250, 0);
      myGLCD.print("Negative numbers for PixSizeX", LEFT, 270, 0);
      myGLCD.print("and PixSizeY are expected :)", LEFT, 282, 0);
    }
  }
}

void loop()
{
  startup();
  
  myGLCD.setColor(80, 80, 80);
  drawCrossHair(10,10);
  drawCrossHair(10,160);
  drawCrossHair(10,309);
  drawCrossHair(120,10);
  drawCrossHair(120,160);
  drawCrossHair(120,309);
  drawCrossHair(229,10);
  drawCrossHair(229,160);
  drawCrossHair(229,309);
  
  calibrate(10, 10, 0);
  calibrate(10, 160, 1);
  calibrate(10, 309, 2);
  calibrate(120, 10, 3);
  calibrate(120, 160, 4);
  calibrate(120, 309, 5);
  calibrate(229, 10, 6);
  calibrate(229, 160, 7);
  calibrate(229, 309, 8);
  
  px = (((float)(rx[0]+rx[1]+rx[2])/3)-((float)(rx[6]+rx[7]+rx[8])/3))/220;
  if (px>=0)
    ox = (((rx[6]+rx[7]+rx[8])/3))-(px*10);
  else
    ox = (((rx[0]+rx[1]+rx[2])/3))+(px*10);

  py = (((float)(ry[0]+ry[3]+ry[6])/3)-((float)(ry[2]+ry[5]+ry[8])/3))/300;
  if (py>=0)
    oy = (((ry[2]+ry[5]+ry[8])/3))-(py*10);
  else
    oy = (((ry[0]+ry[3]+ry[6])/3))+(py*10);
    

  done();
  while(1) {}
}
