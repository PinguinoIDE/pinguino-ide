/* 
 * Simple Mandelbrot set renderer for Pinguino32X
 * w/o floating-point math, via www.hackaday.com.
 */

//Pinguino Logo
#include "pinguino.c"

#define LED 			49		 //Emperor blue led is pin 49, see const.h file

#define BITS         12     //Fractional resolution
#define BIT1         11     //BITS - 1
#define BIT4         16384  //4<<BITS
#define PIXELWIDTH   399    //LCD dimensions
#define PIXELHEIGHT  120	 //Height / 2
#define INTERATIONS  255    //Fractal iteration limit or 'dwell'
#define CENTERREAL  -0.6    //Image center point in complex plane
#define CENTERIMAG   0.0
#define RANGEREAL    3.0    //Image coverage in complex plane
#define RANGEIMAG    3.0
#define STARTREAL   -8601   //((centerReal - rangeReal * 0.5)   * (float)(1 << bits))
#define STARTIMAG    6144   //((centerImag + rangeImag * 0.5)   * (float)(1 << bits))
#define INCREAL      31     //((rangeReal / (float)pixelWidth)  * (float)(1 << bits))
#define INCIMAG      51     //((rangeImag / (float)pixelHeight) * (float)(1 << bits))

void setup(){
    pinMode(LED,OUTPUT);   // Pinguino Emperor blue status LED
    pinMode(REDLED,OUTPUT);
    
    digitalWrite(LED,LOW); // LED off

    //Starts ITDB02 display
    myGLCD16w.InitLCD(LANDSCAPE);
    
    myTouch.setTouch(56,57,58,59,60); //clk, cs, din, dout, irq
    myTouch.InitTouch(LANDSCAPE);
    myTouch.setPrecision(PREC_MEDIUM);
  
    //Show pinguino logo! YEAH!
    myGLCD16w.fillScr(255, 255, 255);
    myGLCD16w.drawBitmap (45, 50, 174, 132, pinguino, 1);
    myGLCD16w.fontSize(FONT_LARGE);
    myGLCD16w.setColor(0,0,0);
    myGLCD16w.setBackColor(255,255,255);
    myGLCD16w.print("Pinguino32X ITDB02 wide", 10, 180, 0);
    myGLCD16w.print("MandelBrot fractal", 10, 200, 0);
    myGLCD16w.print("400x240", 10, 220, 0);
    delay(2000);
    
    //Clear
    myGLCD16w.setColor(255,255,255);
    myGLCD16w.fillRect(0,0,400,240);
  
    //Black color
    myGLCD16w.setColor(0,0,0);
}

void loop(){
    int           x,y,n, rr, rg, rb;
    long          a,b,a2,b2,posReal,posImag,startTime,elapsedTime, cx, cy;

    rr = (int)47*rand();
    rg = (int)23*rand();
    rb = (int)67*rand();
    
    myGLCD16w.clrScr();
    
    digitalWrite(LED,HIGH);   // LED on while rendering
    startTime = millis();

    posImag = STARTIMAG;
    for(y = 0; y < PIXELHEIGHT; y++) {
        posReal = STARTREAL;
        for(x = 0; x < PIXELWIDTH; x++) {
            a = posReal;
            b = posImag;				
            for(n = INTERATIONS; n > 0 ; n--) {
                a2 = (a * a)>>BITS;
                b2 = (b * b)>>BITS;
                
                if((a2 + b2) >= BIT4) break; //4<<BITS
					 
					 b  = posImag + ((a * b)>>BIT1); //BIT1 = BITS - 1
                a  = posReal + a2 - b2;
				}

        //myGLCD16w.setColor(n*67, n*47, n*23);
        myGLCD16w.setColor(n*rr, n*rg, n*rb);
	
        myGLCD16w.drawPixel(x,y);
        myGLCD16w.drawPixel(x,239-y);
        posReal += INCREAL;
    }
    posImag -= INCIMAG;
  }

  elapsedTime = millis() - startTime;
  digitalWrite(LED,LOW);    // LED off when done

  // Set color text
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.printNumI(elapsedTime, 5, 5);
  myGLCD16w.print("ms", 70, 5, 0);
  
  toggle(REDLED);
  //Wait for touch and go!
  while(!myTouch.dataAvailable());

  toggle(REDLED);
}
