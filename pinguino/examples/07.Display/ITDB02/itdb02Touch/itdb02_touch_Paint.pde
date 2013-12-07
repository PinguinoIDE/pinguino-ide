/*
 * ITDB02_Touch_Paint(C)2011 Marcus Fazzi
 * blog: http://fazzi.eng.br
 *
 * This program is a quick demo of how to use the library.
 *
 * This program requires the ITDB02_Graph library (8bit mode)
 * Tested with Emperor board
 * Tested with OLIMEX Pinguino32 board
 */

#include <hardware.c>

//Pinguino 32X logo!
//#include "pinguino32X.c"

//Pinguino
#include "pinguino.c"

//#define USERBT USERBUTTON

//tool color
u8 rR=0, rG=0, rB=0;
u8 bR=255, bG=255, bB=255;
u8 tool = 0;

//setup tool color
void setCor(u8 R, u8 G, u8 B){
	rR=R;
	rG=G;
	rB=B;
}

//16 Colors Aquarela!
void drawAquarela()
{
   //white
	myGLCD.setColor(255,255,255);
   myGLCD.fillRect(0,0, 20,20);
   myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(0,0, 20,20);
	//black
	myGLCD.setColor(0,0,0);
	myGLCD.fillRect(20,0,40,20);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(20,0,40,20);
	//blue
	myGLCD.setColor(0,0,255);
	myGLCD.fillRect(0,20,20,40);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(0,20,20,40);
	//cian
	myGLCD.setColor(0,255,255);
	myGLCD.fillRect(20,20,40,40);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(20,20,40,40);
	//green
	myGLCD.setColor(0,255,0);
	myGLCD.fillRect(0,40,20,60);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(0,40,20,60);
	//yellow
	myGLCD.setColor(255,255,0);
	myGLCD.fillRect(20,40,40,60);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(20,40,40,60);
	//pink
	myGLCD.setColor(255,0,255);
	myGLCD.fillRect(0,60,20,80);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(0,60,20,80);
	//red
	myGLCD.setColor(255,0,0);
	myGLCD.fillRect(20,60,40,80);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(20,60,40,80);

	//dark green
	myGLCD.setColor(48,99,35);
	myGLCD.fillRect(0,80,20,100);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(0,80,20,100);

   //Orange
	myGLCD.setColor(255,191,0);
	myGLCD.fillRect(20,80,40,100);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(20,80,40,100);

   //gray
	myGLCD.setColor(162,162,162);
	myGLCD.fillRect(0,100,20,120);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(0,100,20,120);

   //Brown
	myGLCD.setColor(128,64,0);
	myGLCD.fillRect(20,100,40,120);
	myGLCD.setColor(0,0,0);
   myGLCD.drawRoundRect(20,100,40,120);

	//Tools Font
	myGLCD.fontSize(FONT_SMALL);
	myGLCD.setBackColor(255,255,255);

	//Rand Color
   myGLCD.setColor(255,255,255);
   myGLCD.fillRect(0,120,40,140);
   myGLCD.setColor(0,0,0);
   myGLCD.print("RAND", 5, 124, 0);
	myGLCD.drawRoundRect(0,120,40,140);

	//Tool type
   myGLCD.setColor(255,255,255);
   myGLCD.fillRect(0,140,40,160);
   myGLCD.setColor(0,0,0);
   myGLCD.print("CLEAR", 2, 144, 0);
	myGLCD.drawRoundRect(0,140,40,160);
	
	//Clear
	myGLCD.setColor(255,255,255);
	myGLCD.fillRect(0,160,40,180);
	myGLCD.setColor(0,0,0);
	myGLCD.print("TOOL", 5, 164, 0);
   myGLCD.drawRoundRect(0,160,40,180);

   //Selected color and DrawTool
   setCor(0,0,0);
}

// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD.setColor(255,0,0);
  myGLCD.drawRoundRect(x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD.setColor(0,0,0);
  myGLCD.drawRoundRect(x1, y1, x2, y2);
}

void drawTool(int x, int y, u8 mode){
	
	if(mode)
		myGLCD.setColor(0,0,0);
	else
		myGLCD.setColor(rR,rG,rB);
		
	switch(tool){
		case 0:
			//myGLCD.drawPixel(x,y);  //original, single dot pen
			myGLCD.fillRect(x,y,x+1,y+1); //small pen
			break;
		case 1:
			myGLCD.fillCircle(x,y,3); //circle pen!
			break;
		case 2:
			myGLCD.fillCircle(x,y,5); //big circle pen!
			break;
		case 3:
			myGLCD.fillRect(x,y,x+3,y+3); //square pen
			break;
		case 4:
			myGLCD.drawLine(x, y, x+5, y+5);  //line pen tool
			break;
		case 5:
			myGLCD.setColor(bR,bG,bB);
			myGLCD.fillRect(x,y,x+8,y+8);  //eraser tool
			break;
	}
}

void setup()
{
  //UBW32 and EMPEROR boards
  //myTouch.setTouch(56,57,58,59,60); //clk, cs, din, dout, irq
  
  //OLIMEX Pinguino32X board
  myTouch.setTouch(15,10,14,9,8); //clk, cs, din, dout, irq
  
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);
  
  myGLCD.InitLCD(LANDSCAPE);

  //pinMode(LED1, OUTPUT);  //white button
  //pinMode(LED2, OUTPUT);  //green button

  //Show pinguino logo! YEAH!
  myGLCD.fillScr(255, 255, 255);
  myGLCD.drawBitmap (5, 50, 174, 132, pinguino, 1);

  myGLCD.fontSize(FONT_LARGE);
  myGLCD.setColor(0,0,0);
  myGLCD.setBackColor(255,255,255);
  myGLCD.print("Pinguino32X ITDB02", 20, 180, 0);
  myGLCD.print("Paint by Fazzi", 40, 200, 0);
  delay(5000);

  drawAquarela();
  //Draw tool
  myGLCD.setColor(255,255,255);
  myGLCD.fillRect(0,181,40,210);
  drawTool(18,200,1);
  //Clear
  myGLCD.setColor(255,255,255);
  myGLCD.fillRect(41,0,320,240);
}

void loop()
{
	u16 x, y;

	while(1){
	
		/*if(!digitalRead(USERBT)){
			myGLCD.drawBitmap (80, 40, 174, 132, pinguino, 1);
		}*/
	
		if(myTouch.dataAvailable())
		{
			myTouch.read();
			x = myTouch.getX();
			y = myTouch.getY();

         if(x<320 && y<240){ //no illegal points
				if(x<45 && y<240){ //tool bar
					if ((y>=0) && (y<=20)){ //row 1
						if ((x>=0) && (x<=20)) { //white
							waitForIt(0, 0, 20, 20);
							setCor(255,255,255);
						}
						if ((x>=21) && (x<=40)) { //black
							waitForIt(20,0,40,20);
							setCor(0,0,0);
						}
					}

					if ((y>20) && (y<=40)){ //row 2
						if ((x>=0) && (x<=20)) { //blue
							waitForIt(0,20,20,40);
							setCor(0,0,255);
						}
						if ((x>=21) && (x<=40)) { //cian
							waitForIt(20,20,40,40);
							setCor(0,255,255);
						}
					}

					if ((y>40) && (y<=60)){ //row 3
						if ((x>=0) && (x<=20)) { //green
							waitForIt(0,40,20,60);
							setCor(0,255,0);
						}
						if ((x>=21) && (x<=40)) { //yellow
							waitForIt(20,40,40,60);
							setCor(255,255,0);
						}
					}

					if ((y>60) && (y<=80)){ //row 4
						if ((x>=0) && (x<=20)) { //pink
							waitForIt(0,60,20,80);
							setCor(255,0,255);
						}
						if ((x>=21) && (x<=40)) { //red
							waitForIt(20,60,40,80);
							setCor(255,0,0);
						}
					}

					if ((y>80) && (y<=100)){ //row 5
						if ((x>=0) && (x<=20)) { //dark green
							waitForIt(0,80,20,100);
							setCor(48,99,35);
						}
						if ((x>=21) && (x<=40)) { //Orange
							waitForIt(20,80,40,100);
							setCor(255,191,0);
						}
					}

					if ((y>100) && (y<=120)){ //row 6
						if ((x>=0) && (x<=20)) { //gray
							waitForIt(0,100,20,120);
							setCor(162,162,162);
						}
						if ((x>=21) && (x<=40)) { //Brown
							waitForIt(20,100,40,120);
							setCor(128,64,0);
						}
					}

					if ((y>120) && (y<=140)){ //row 8 random color
						waitForIt(0,120,40,140);
						srand(millis());
						setCor(rand()%255,rand()%255,rand()%255);
					}

					if ((y>140) && (y<=160)){ //row 9 clear
						waitForIt(0,140,40,160);
						//Draw tool
						myGLCD.setColor(255,255,255);
						myGLCD.fillRect(0,181,40,210);
						drawTool(18,200,1);
						//Clear paint area with selected color					
						myGLCD.setColor(rR,rG,rB);
						myGLCD.setBackColor(rR,rG,rB);
						bR=rR;
						bG=rG;
						bB=rB;
						myGLCD.fillRect(41,0,320,240);
					}
					
					if ((y>160) && (y<=180)){ //row 10 tool select
						waitForIt(0,160,40,180);
						tool=++tool>5?0:tool;
						
						//Draw tool
						myGLCD.setColor(255,255,255);
						myGLCD.fillRect(0,181,40,210);
						drawTool(18,200,1);
						
					}

				}
				else{
					
					drawTool(x,y,0);
					//touched LED!
					//digitalWrite(LED1, LOW);
					//UBW32LedToggle(LED2);
					
					analogWrite(77,512);
					
				}

				//(x,y)
				//DrawTool Select
				myGLCD.setColor(rR,rG,rB);
				myGLCD.fillRect(0,218,40,240);				
				myGLCD.setColor(255-rR,255-rG,255-rB);
				myGLCD.setBackColor(rR,rG,rB);
				myGLCD.print("X=   ", 0, 218, 0);
				myGLCD.printNumI(x, 14, 218);
				myGLCD.print("Y=   ", 0, 230, 0);
				myGLCD.printNumI(y, 14, 230);
			}
		}
    //run LED!
    //UBW32LedToggle(LED1);
    //delay(100);
    //UBW32LedToggle(LED1);
    //digitalWrite(LED2, LOW);
    }
}

