glcdbitmap.pde is a Processing sketch that creates a bitmap definition file that can be included in an Arduino sketch 
and displayed by the  ks0108 GLCD library (requires version 2 or later)

The source bitmap must have a height and width no greater than the height and width of the LCD display and should be a
black and white  image (single bit per pixel)

Change the string variable sourceImage  to the name of the bitmap file you want to convert. Bitmap images should be 
stored in a subdirectory called data.

The output will be a header file with the same base name as the source image with an extension of  .h  - this header 
file should be included in your sketch (see the example). The header file must either be copied to the directory that
contains your Arduino sketch or to the ks0108 library directory. 

The header will contain an array variable with the same basename as your source bmp file. This variable is passed to
the method DrawBitmap 

DrawBitmap takes four parameters, in addition to the array variable, it expects the x and y coordinates of the upper 
left hand position of the bitmap. The last parameter is the color to draw the pixes, this can be BLACK or WHITE.
 

The following line draws an image from a header file that has an array called ArduinoIcon. The left hand side (x axis)
is offset 32 pixels, the y axis starts from the top (first line) of the display. The pixels that were black in the bmp
file are painted to the LCD, pixels that were white are cleared. 
GLCD.DrawBitmap(ArduinoIcon, 32,0, BLACK); 

