/**
 * glcdBitmap. 
 * 
 * Creates a bitmap definition file that can be included in an Arduino sketch
 * to display a bitmap image on a graphics LCD using the Arduino GLCD library.
 *
 * Created  6 Nov  2008  Copyright Michael Margolis 2008
 */
String sourceImage =  "Pinguino.bmp"; // change this to the name of your bitmap file
PImage a;
int[] aPixels;

void setup() 
{

  noFill();
  stroke(255);
  frameRate(30);
  // load the image
  a = loadImage(sourceImage);
  if( a != null){
    size(a.width, a.height);
    aPixels = new int[width*height];
    for(int i=0; i<width*height; i++) {
      aPixels[i] = a.pixels[i];
    }
    // display the image
    loadPixels();
    for (int i=0; i<width*height; i++) 
        pixels[i] = aPixels[i];  
    updatePixels();
    writeFile(sourceImage);
    println("created header file for " + sourceImage);  
  }  
}

void writeFile(String inFileName){
  String[] f = split(inFileName, '.');
  String baseName = f[0];

  String outFileName = baseName + ".h"; 
  PrintWriter output;
  output = createWriter(outFileName);

  output.println("/* " + outFileName + " bitmap file for GLCD library */");
  output.println("/* Bitmap created from " + inFileName + "        */");
  String[] monthName = {"","Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
  output.println("/* Date: " +  day() + " " + monthName[month()] +  " " +  year() + "                             */" ); 
  output.println();
  
  /* NOT Need it for Pinguino version
  output.println("#include <inttypes.h>");
  output.println("#include <avr/pgmspace.h>");
  */
  output.println();
  output.println("#ifndef " + baseName + "_H");
  output.println("#define " + baseName + "_H");
  
  output.println();  
  //const unsigned char Pinguino[] = { //Write font to PIC32 flash memory
  output.print("const unsigned char ");
  output.print(baseName);
  output.println("[] = {");
 
  output.print("  ");
  output.print(a.width);   // note width and height are bytes so 256 will be 0
  output.println(", // width"); 
  output.print("  ");
  output.print(a.height);
  output.println(", // height"); 
  for(int page=0; page < (a.height + 7)/8; page++) {
     output.println("\n  /* page " + page + " (lines "  + page*8 + "-" + (page*8+7) + ") */");
     output.print("  "); 
     for(int x=0; x < a.width; x++){
          output.print( "0x" + Integer.toHexString(getValue(x,page))) ;   
          if( (x == (a.width-1)) && (page == (((a.height +7)/8)-1))  )
              println("\n"); // this is the last element so new line instead of comma
           else   
              output.print(",");   // comma on all but last entry
          if( x % 16 == 15)
             output.print("\n  "); 
     }
  }  
  output.print("\n};\n");
  output.println("#endif");
  
  output.flush(); // Write the remaining data
  output.close(); // Finish the file

}
int getValue( int x, int page){
  int val = 0;
  for( byte b=0; b < 8; b++){
      int y = page * 8 + b;
      int pos = y * a.width + x;
      if( aPixels[pos] != 0xffffffff ){     
         val |=  (1 << b);   // here if color is not black
      }
  }   
  return val;  
}

void draw() 
{
  
}





