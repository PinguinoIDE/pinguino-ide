/* 
 * Simple Mandelbrot set renderer for Pinguino32X
 * w/floating-point math, via www.hackaday.com.
 */
 
//Pinguino Logo
#include "pinguino.c"

#define LED 49

#define PIXELWIDTH   399  // LCD dimensions
#define PIXELHEIGHT  120
#define INTERATIONS  255  // Fractal iteration limit or 'dwell'
#define CENTERREAL  -0.6 // Image center point in complex plane
#define CENTERIMAG   0.0
#define RANGEREAL    3.0 // Image coverage in complex plane
#define RANGEIMAG    3.0
#define STARTREAL   -2.1
#define STARTIMAG    1.5
#define INCREAL      0.0075
#define INCIMAG      0.0125

void setup(){
    pinMode(LED,OUTPUT);   // Pinguino Emperor blue status LED
    digitalWrite(LED,LOW); // LED off

    //Starts ITDB02 display
    myGLCD16w.InitLCD(LANDSCAPE);
    
    myTouch(56,57,58,59,60); //clk, cs, din, dout, irq
    myTouch.InitTouch(LANDSCAPE);
    myTouch.setPrecision(PREC_MEDIUM);
  
    //Show pinguino logo! YEAH!
    myGLCD16w.fillScr(255, 255, 255);
    myGLCD16w.drawBitmap (45, 50, 174, 132, pinguino, 1);
    myGLCD16w.fontSize(FONT_LARGE);
    myGLCD16w.setColor(0,0,0);
    myGLCD16w.setBackColor(255,255,255);
    myGLCD16w.print("Pinguino32X ITDB02 wide", 10, 180, 0);
    myGLCD16w.print("Mandel fractal", 10, 200, 0);
    delay(2000);
    
    //Clear
    myGLCD16w.setColor(255,255,255);
    myGLCD16w.fillRect(0,0,400,240);
  
    //Black color
    myGLCD16w.setColor(0,0,0);
}

void loop(){
    int           x,y,n;
    float         a,b,a2,b2,posReal,posImag;
    long          startTime,elapsedTime;

    myGLCD16w.clrScr();
    delay(100);              // Brief pause, else 1st few pixels are lost

    digitalWrite(LED,HIGH);   // LED on while rendering
    startTime = millis();

    posImag = STARTIMAG;
    for(y = 0; y < PIXELHEIGHT; y++) {
        posReal = STARTREAL;
        for(x = 0; x < PIXELWIDTH; x++) {
            a = posReal;
            b = posImag;
            for(n = INTERATIONS; n > 0 ; n--) {
                a2 = a * a;
                b2 = b * b;
                if((a2 + b2) >= 4.0) break;
                    b  = posImag + a * b * 2.0;
                    a  = posReal + a2 - b2;
            }
      
         //n * 29;     // Pixel color MSB
         //n * 67;     // Pixel color LSB
        
        myGLCD16w.setColor(n*23, n*47, n*67);
        
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
  
  myGLCD16w.setColor(0,0,0);
  myGLCD16w.printNumI(elapsedTime, 5, 5);
  myGLCD16w.print("ms", 110, 5, 0);

  //Wait for touch and go!
  while(!myTouch.dataAvailable());
    //myTouch.read();
  delay(500); // Stall a few seconds, then repeat
}
