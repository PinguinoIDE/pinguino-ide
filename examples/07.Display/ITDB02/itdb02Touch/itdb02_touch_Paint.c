//
//
//
//
//
//
//
//
//

//


//


//
//

#define USERBT 68
#define PROGBT 69

//
u8 rR=0, rG=0, rB=0;
u8 bR=255, bG=255, bB=255;
u8 tool = 0;

//
void setCor(u8 R, u8 G, u8 B){
	rR=R;
	rG=G;
	rB=B;
}

//
void drawAquarela()
{
//
	setColor(255,255,255);
   fillRect(0,0, 20,20);
   setColor(0,0,0);
   drawRoundRect(0,0, 20,20);
//
	setColor(0,0,0);
	fillRect(20,0,40,20);
	setColor(0,0,0);
   drawRoundRect(20,0,40,20);
//
	setColor(0,0,255);
	fillRect(0,20,20,40);
	setColor(0,0,0);
   drawRoundRect(0,20,20,40);
//
	setColor(0,255,255);
	fillRect(20,20,40,40);
	setColor(0,0,0);
   drawRoundRect(20,20,40,40);
//
	setColor(0,255,0);
	fillRect(0,40,20,60);
	setColor(0,0,0);
   drawRoundRect(0,40,20,60);
//
	setColor(255,255,0);
	fillRect(20,40,40,60);
	setColor(0,0,0);
   drawRoundRect(20,40,40,60);
//
	setColor(255,0,255);
	fillRect(0,60,20,80);
	setColor(0,0,0);
   drawRoundRect(0,60,20,80);
//
	setColor(255,0,0);
	fillRect(20,60,40,80);
	setColor(0,0,0);
   drawRoundRect(20,60,40,80);

//
	setColor(48,99,35);
	fillRect(0,80,20,100);
	setColor(0,0,0);
   drawRoundRect(0,80,20,100);

//
	setColor(255,191,0);
	fillRect(20,80,40,100);
	setColor(0,0,0);
   drawRoundRect(20,80,40,100);

//
	setColor(162,162,162);
	fillRect(0,100,20,120);
	setColor(0,0,0);
   drawRoundRect(0,100,20,120);

//
	setColor(128,64,0);
	fillRect(20,100,40,120);
	setColor(0,0,0);
   drawRoundRect(20,100,40,120);

//
	fontSize(FONT_SMALL);
	setBackColor(255,255,255);

//
   setColor(255,255,255);
   fillRect(0,120,40,140);
   setColor(0,0,0);
   print("RAND", 5, 124, 0);
	drawRoundRect(0,120,40,140);

//
   setColor(255,255,255);
   fillRect(0,140,40,160);
   setColor(0,0,0);
   print("CLEAR", 2, 144, 0);
	drawRoundRect(0,140,40,160);
	
//
	setColor(255,255,255);
	fillRect(0,160,40,180);
	setColor(0,0,0);
	print("TOOL", 5, 164, 0);
   drawRoundRect(0,160,40,180);

//
   setCor(0,0,0);
}

//
void waitForIt(int x1, int y1, int x2, int y2)
{
  setColor(255,0,0);
  drawRoundRect(x1, y1, x2, y2);
  while (dataAvailable())
    read();
  setColor(0,0,0);
  drawRoundRect(x1, y1, x2, y2);
}

void drawTool(int x, int y, u8 mode){
	
	if(mode)
		setColor(0,0,0);
	else
		setColor(rR,rG,rB);
		
	switch(tool){
		case 0:
//
			fillRect(x,y,x+1,y+1); //small pen
			break;
		case 1:
			fillCircle(x,y,3); //circle pen!
			break;
		case 2:
			fillCircle(x,y,5); //big circle pen!
			break;
		case 3:
			fillRect(x,y,x+3,y+3); //square pen
			break;
		case 4:
			drawLine(x, y, x+5, y+5);  //line pen tool
			break;
		case 5:
			setColor(bR,bG,bB);
			fillRect(x,y,x+8,y+8);  //eraser tool
			break;
	}
}

void setup()
{
  ITDB02_Touch(11,12,13,14,15);
  InitLCD(LANDSCAPE);
  fillScr(255,255,255);

  InitTouch(LANDSCAPE);
  setPrecision(PREC_MEDIUM);

  pinmode(LED1, OUTPUT);  //white button
  pinmode(LED2, OUTPUT);  //green button

//
  fillScr(255, 255, 255);
  drawBitmap (5, 50, 174, 132, pinguino, 1);
  
  fontSize(FONT_LARGE);
  setColor(0,0,0);
  setBackColor(255,255,255);
  print("Pinguino32X ITDB02", 20, 180, 0);
  print("Paint by Fazzi", 40, 200, 0);
  Delayms(5000);

  drawAquarela();
//
  setColor(255,255,255);
  fillRect(0,181,40,210);
  drawTool(18,200,1);
//
  setColor(255,255,255);
  fillRect(41,0,320,240);
}

void loop()
{
	u16 x, y;

	while(1){
	
		if(!digitalread(USERBT)){
			drawBitmap (80, 40, 174, 132, pinguino, 1);
		}
	
		if(dataAvailable())
		{
			read();
			x = getX();
			y = getY();

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
//
						setColor(255,255,255);
						fillRect(0,181,40,210);
						drawTool(18,200,1);
//
						setColor(rR,rG,rB);
						setBackColor(rR,rG,rB);
						bR=rR;
						bG=rG;
						bB=rB;
						fillRect(41,0,320,240);
					}
					
					if ((y>160) && (y<=180)){ //row 10 tool select
						waitForIt(0,160,40,180);
						tool=++tool>5?0:tool;
						
//
						setColor(255,255,255);
						fillRect(0,181,40,210);
						drawTool(18,200,1);
						
					}

				}
				else{
					
					drawTool(x,y,0);
//
					digitalwrite(LED1, LOW);
					digitalwrite(LED2, HIGH);
					
					analogwrite(77,512);
					
				}

//
//
				setColor(rR,rG,rB);
				fillRect(0,218,40,240);				
				setColor(255-rR,255-rG,255-rB);
				setBackColor(rR,rG,rB);
				print("X=   ", 0, 218, 0);
				printNumI(x, 14, 218);
				print("Y=   ", 0, 230, 0);
				printNumI(y, 14, 230);
			}
		}
//
    digitalwrite(LED1, HIGH);
    digitalwrite(LED2, LOW);
    }
}


