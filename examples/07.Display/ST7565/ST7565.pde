/*
    ST7565 library for Pinguino/Arduino, example code with some explanations.
    By Samuel Ureta.
    You can freely distribute, copy, or modify this code.
*/

// This library allows to control a LCD display with ST7565 interface. 
// We need 5 control pins and maybe some more for controlling LED backlight.
// Coordinates are started to count from up-left corner to down-right. 
// Coordinates larger than screen size (defined on ST7565.h) do continue on the opposite side.


// We can define a bitmap with some picture here for putting somewhere. 
// First of all, we have to define bitmap's height and width.
//#define BITMAP_HEIGHT 16 
//#define BITMAP_WIDTH  16 
// Now we can define it with binary numbers: 1 = black pixel, 0 = white pixel. 
// Each byte (8 bits) is a vertical line of 8x1 pixels, being drawed from botton to top.
// It's drawed from up-left to right. If BITMAP_WIDTH is reached, it moves to a new line,
// and it continues to the right. 
// In summary, a 16x16 bitmap it's drawed as a picture of 16x2 vertical lines with 8 pixels each. 
// You can write the numbers in binary form, like this: B00000000
// Or maybe on another base, like decimal: 0
// Or maybe hexadecimal: 0x00

// Actually defining the bitmap:
// static const unsigned char picture[]=
// {
// B00000000, B11111111, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, B11111111, 
// B11111111, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
// };

// Control pins for the ST7565 LCD:
//
// pin 9 - Serial data out (SID)
// pin 8 - Serial clock out (SCLK)
// pin 7 - Data/Command select (RS or A0)
// pin 6 - LCD reset (RST)
// pin 5 - LCD chip select (CS)
int8_t SID = 9, SCLK = 8, RS = 7, RST = 6, CS = 5;

void setup()
{
    // Init. the display
    ST7565.init(SID, SCLK, RS, RST, CS); 
    ST7565.clear(); // LCD buffer starts being filled with Adafruit logo. We use this function to clear the screen.
    ST7565.refresh(); // We call this function every time we make a change on the buffer, 
                   // to actually reflect that change on the screen.

//  To draw a single pixel, we use: ST7565setpixel( x, y, color); The color can be BLACK o WHITE.
//  ST7565.setPixel(10, 10, BLACK);
//  ST7565.refresh();

  
//  To draw a circle, we use: ST7565fillcircle( x, y, radio, color); 
//  Coordinates are not the center, but the top-left pixel of a square containing the circle.
//  ST7565.fillCircle(32, 32, 10, BLACK);
//  ST7565.refresh();


//  To write a single letter, we use: ST7565drawchar(x, line, letter between ' and ')
//  We have VERTICALSIZEOFSCREEN/8 horizontal lines to write, starting from 0.
//  It starts to write on the x pixel of the chosen line, x>=0.
//  ST7565.drawChar(0, 1, 'a');
//  ST7565.refresh();


//  To write a sentence, we use ST7565drawstring(x, line, sentence between " and ").
//  We have VERTICALSIZEOFSCREEN/8 horizontal lines to write, starting from 0.
//  It starts to write on the x pixel of the chosen line, x>=0.
//  ST7565.drawString(0, 0, "Hello world.");
//  ST7565.refresh();


//  To draw a straight line from (x0,y0) to (x1,y1) we use: ST7565drawline(x0, y0, x1, y1, color);
//  It can be used to draw a line from right to left or from botton to top, no problems there.
//  ST7565.drawLine(0, 0, 127, 63, BLACK);
//  ST7565.refresh();


//  To draw a rectangle with no fill which top-left corner were (x0,y0) and which has w of width and h of height,
//  we use ST7565drawrect(x0, y0, w, h, color).
//  ST7565.drawRect(0, 0, 127, 63, BLACK);
//  ST7565.refresh();


//  To draw a filled rectangle which top-left corner were (x0,y0) and which has w of width and h of height,
//  we use ST7565fillrect(x0, y0, w, h, BLACK);
//  ST7565.fillRect(0, 0, 127, 63, BLACK);
//  ST7565.refresh();


// To draw a bitmap defined on the top of the code starting from point (x,y) to bottom-right, 
// we use the function ST7565drawbitmap(x, y, picture, pictureheight, picturewidth, color);
//  ST7565.drawBitmap(0, 0, dibujo, ALTURABITMAP, ANCHURABITMAP, BLACK);
//  ST7565.refresh();
}

void loop()
{}
