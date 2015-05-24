// ITDB02_Graph_Rotate_Bitmap (C)2010 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// Pinguino32X(PIC32) port by Marcus Fazzi
// blog: http://vivaphp.net
//
//
// This program is a demo of the drawBitmap()-function.
//
// This program requires the ITDB02_Graph wide 400x240 (16bit data bus mode).
//
//

#define COM UART2

#include "tux.c"

void setup() {
  myGLCD16w.InitLCD(LANDSCAPE);
  myGLCD16w.fillScr(255, 255, 255);
  myGLCD16w.setColor(0, 0, 0);
  
  Serial.Configure(COM, UART_ENABLE, UART_RX_TX_ENABLED, 9600);
}

void loop(){
	int i;
	
    for (i=0; i<360; i+=5){
			myGLCD16w.drawBitmapR(100, 100, 32, 32, tux, i, 16, 16);
    }
}

