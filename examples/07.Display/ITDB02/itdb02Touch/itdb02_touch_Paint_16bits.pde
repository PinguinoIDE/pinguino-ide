/*
 * Pinguino32X(PIC32) port by Marcus Fazzi
 * blog: http://fazzi.eng.br
 *
 *
 * This program is a demo of how to use the library with the
 * ITDB02-3.2 module.
 *
 * This program requires the ITDB02_Graph16 (16bit mode).
 *
 *
 * This program is a quick demo of how to use the library.
 * Tested with Emperor board
 */

#include <hardware.c>

//Pinguino 32X logo!
//#include "pinguino32X.c"

//Pinguino
#include "pinguino.c"

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
	myGLCD16.setColor(255,255,255);
   myGLCD16.fillRect(0,0, 20,20);
   myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(0,0, 20,20);
	//black
	myGLCD16.setColor(0,0,0);
	myGLCD16.fillRect(20,0,40,20);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(20,0,40,20);
	//blue
	myGLCD16.setColor(0,0,255);
	myGLCD16.fillRect(0,20,20,40);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(0,20,20,40);
	//cian
	myGLCD16.setColor(0,255,255);
	myGLCD16.fillRect(20,20,40,40);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(20,20,40,40);
	//green
	myGLCD16.setColor(0,255,0);
	myGLCD16.fillRect(0,40,20,60);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(0,40,20,60);
	//yellow
	myGLCD16.setColor(255,255,0);
	myGLCD16.fillRect(20,40,40,60);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(20,40,40,60);
	//pink
	myGLCD16.setColor(255,0,255);
	myGLCD16.fillRect(0,60,20,80);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(0,60,20,80);
	//red
	myGLCD16.setColor(255,0,0);
	myGLCD16.fillRect(20,60,40,80);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(20,60,40,80);

	//dark green
	myGLCD16.setColor(48,99,35);
	myGLCD16.fillRect(0,80,20,100);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(0,80,20,100);

   //Orange
	myGLCD16.setColor(255,191,0);
	myGLCD16.fillRect(20,80,40,100);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(20,80,40,100);

   //gray
	myGLCD16.setColor(162,162,162);
	myGLCD16.fillRect(0,100,20,120);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(0,100,20,120);

   //Brown
	myGLCD16.setColor(128,64,0);
	myGLCD16.fillRect(20,100,40,120);
	myGLCD16.setColor(0,0,0);
   myGLCD16.drawRoundRect(20,100,40,120);

	//Tools Font
	myGLCD16.fontSize(FONT_SMALL);
	myGLCD16.setBackColor(255,255,255);

	//Rand Color
   myGLCD16.setColor(255,255,255);
   myGLCD16.fillRect(0,120,40,140);
   myGLCD16.setColor(0,0,0);
   myGLCD16.print("RAND", 5, 124, 0);
	myGLCD16.drawRoundRect(0,120,40,140);

	//Tool type
   myGLCD16.setColor(255,255,255);
   myGLCD16.fillRect(0,140,40,160);
   myGLCD16.setColor(0,0,0);
   myGLCD16.print("CLEAR", 2, 144, 0);
	myGLCD16.drawRoundRect(0,140,40,160);
	
	//Clear
	myGLCD16.setColor(255,255,255);
	myGLCD16.fillRect(0,160,40,180);
	myGLCD16.setColor(0,0,0);
	myGLCD16.print("TOOL", 5, 164, 0);
   myGLCD16.drawRoundRect(0,160,40,180);

   //Selected color and DrawTool
   setCor(0,0,0);
}

// Draw a red frame while a button is touched
void waitForIt(int x1, int y1, int x2, int y2)
{
  myGLCD16.setColor(255,0,0);
  myGLCD16.drawRoundRect(x1, y1, x2, y2);
  while (myTouch.dataAvailable())
    myTouch.read();
  myGLCD16.setColor(0,0,0);
  myGLCD16.drawRoundRect(x1, y1, x2, y2);
}

void drawTool(int x, int y, u8 mode){
	
	if(mode)
		myGLCD16.setColor(0,0,0);
	else
		myGLCD16.setColor(rR,rG,rB);
		
	switch(tool){
		case 0:
			//myGLCD16.drawPixel(x,y);  //original, single dot pen
			myGLCD16.fillRect(x,y,x+1,y+1); //small pen
			break;
		case 1:
			myGLCD16.fillCircle(x,y,3); //circle pen!
			break;
		case 2:
			myGLCD16.fillCircle(x,y,5); //big circle pen!
			break;
		case 3:
			myGLCD16.fillRect(x,y,x+3,y+3); //square pen
			break;
		case 4:
			myGLCD16.drawLine(x, y, x+5, y+5);  //line pen tool
			break;
		case 5:
			myGLCD16.setColor(bR,bG,bB);
			myGLCD16.fillRect(x,y,x+8,y+8);  //eraser tool
			break;
	}
}

void setup() {
  myTouch.setTouch(56,57,58,59,60); //clk, cs, din, dout, irq
  myTouch.InitTouch(LANDSCAPE);
  myTouch.setPrecision(PREC_MEDIUM);
  
  myGLCD16.InitLCD(LANDSCAPE);

  pinMode(LED1, OUTPUT);  //white button
  pinMode(LED2, OUTPUT);  //green button

  //Show pinguino logo! YEAH!
  myGLCD16.fillScr(255, 255, 255);
  myGLCD16.drawBitmap (5, 50, 174, 132, pinguino, 1);

  myGLCD16.fontSize(FONT_LARGE);
  myGLCD16.setColor(0,0,0);
  myGLCD16.setBackColor(255,255,255);
  myGLCD16.print("Pinguino32X ITDB02", 20, 180, 0);
  myGLCD16.print("Paint by Fazzi", 40, 200, 0);
  delay(5000);

  drawAquarela();
  //Draw tool
  myGLCD16.setColor(255,255,255);
  myGLCD16.fillRect(0,181,40,210);
  drawTool(18,200,1);
  //Clear
  myGLCD16.setColor(255,255,255);
  myGLCD16.fillRect(41,0,319,239);
  
  UBW32LedToggle(REDLED);
}

void loop() {
	u16 x, y;

	while(1){
	
		if(!digitalRead(USERBUTTON)){
			myGLCD16.drawBitmap (80, 40, 174, 132, pinguino, 1);
		}
	
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
						myGLCD16.setColor(255,255,255);
						myGLCD16.fillRect(0,181,40,210);
						drawTool(18,200,1);
						//Clear paint area with selected color					
						myGLCD16.setColor(rR,rG,rB);
						myGLCD16.setBackColor(rR,rG,rB);
						bR=rR;
						bG=rG;
						bB=rB;
						myGLCD16.fillRect(41,0,319,239);
					}
					
					if ((y>160) && (y<=180)){ //row 10 tool select
						waitForIt(0,160,40,180);
						tool=++tool>5?0:tool;
						
						//Draw tool
						myGLCD16.setColor(255,255,255);
						myGLCD16.fillRect(0,181,40,210);
						drawTool(18,200,1);
						
					}

				}
				else{
					
					drawTool(x,y,0);
					//touched LED!
					UBW32LedToggle(BLUELED);
					
					analogWrite(77,512);
					
				}

				//(x,y)
				//DrawTool Select
				myGLCD16.setColor(rR,rG,rB);
				myGLCD16.fillRect(0,218,40,240);				
				myGLCD16.setColor(255-rR,255-rG,255-rB);
				myGLCD16.setBackColor(rR,rG,rB);
				myGLCD16.print("X=   ", 0, 218, 0);
				myGLCD16.printNumI(x, 14, 218);
				myGLCD16.print("Y=   ", 0, 230, 0);
				myGLCD16.printNumI(y, 14, 230);
			}
		}
    //run LED!
	 UBW32LedToggle(REDLED);
    }
}

