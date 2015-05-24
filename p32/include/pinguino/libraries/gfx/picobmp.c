// Sources : http://www.microchip.com/forums/m828547.aspx

/**
* Bitmap file format
* ------------------------------------------------------------------------------
* We are only going to work with 320w x 240h bitmaps.
* Bitmaps start at the lower left corner of the image and sequentially lists
* the BGR colors of each pixel while traveling up from left to right.
* 24bit/pixel must be converted to 16bit/pixel Red/Green/Blue
* Byte 0 = "B"
* Byte 1 = "M"
* Byte 10 is bitmap offset (typically 54)
* Bytes 18-21 = Width (must be 320)
* Bytes 22-25 = Height (must be 240)
* Bytes 26-27 = Planes (must be 1)
* Bytes 28-29 = bits/pixel (must be 24)
* Bytes 30-33 = Compression (must be 0)
* Bytes 34-37 = size of bitmap in bytes (max of 230,400)
* Start of file should be at byte 54 (base 0)
**/

/** 
* Binary file format
* ------------------------------------------------------------------------------
* Data is listed LSB then MSB moving top to bottom, left to right
* Has a 6 byte header (Compression, Color depth, Height, Width see below).
* Has a 16 bit color depth 5-Red, 6-Green, 5-Blue (Blu/Grn byte followed by the Grn/Red byte).
* The bitmap starts at the upper left corner of the image and sequentially lists
* the RGB colors while traveling down from left to right.
**/

// Binary BITMAP_HEADER
// 0x00 Compression (0 none)
// 0x10 color depth (16 bits)
// 0xF000 image height (240 pixels)
// 0x4010 image width (320 pixels)

/*******************************************************************************
 * Write to flash memory a full size BMP file
 * This function will open a *.bmp and write it to the required place in flash.
 * *pStr is a pointer to the bmp filename.
 * "SplashScrn1" position 0
 * "Wallpaper1" position 1
*******************************************************************************/
int mem_wrFullScrBMP(const char *pStr, int pos)
{
    FSFILE *pfd;
    BITMAP_HEADER bmpHdr; //6 byte header
    int i=0, j, k, r;
    int startAddress, size, RowSize, BMSize, numBytes=0;
    unsigned char byteBuff[960]; //process 320 pixels (1 row) at a time
    unsigned char info[54];
    unsigned char Red, Green, Blue;
    WORD RGBbuf;

    //Open the image file if its not found exit.
    pfd = FSfopen ( pStr, "r" );
    if (pfd == NULL)
        return -1; //Read has failed

    //Read the bitmap header
    numBytes = 54;
    r = FSfread((void *)info, 1, numBytes, pfd );
    if(r != 54) //Check for a read error.
    {
        FSfclose(pfd);
        return -1; //Read has failed
    }

    bmpHdr.compression = info[30];
    bmpHdr.colorDepth = info[28];
    bmpHdr.height = info[22];
    bmpHdr.height += (info[23]<<8);
    bmpHdr.width = info[18];
    bmpHdr.width += info[19]<<8;
    BMSize = info[34];
    BMSize += (info[35]<<8);
    BMSize += (info[36]<<16);
    RowSize = BMSize/bmpHdr.height;

    if(!((bmpHdr.compression == 0) && (bmpHdr.colorDepth == 24) && 
         (bmpHdr.height <= 240)    && (bmpHdr.width <= 320)))
    {
        FSfclose(pfd);
        return -1; //Un supported bitmap
    }

    //Set the last row address in Flash and work up to first row address.
    if(pos == 0)
    {
        startAddress = (MEM_SPLASH1+6+((bmpHdr.height-1)*bmpHdr.width*2)); //Start at 1st pix of bottom row
        // size = 57600;
        size = BMSize;
    }
    // else if(pos == 1)
    // {
    // startAddress = MEM_WALLPAPER1;
    // size = 230400;
    // size = BMSize;
    // }
    else
    {
        FSfclose(pfd);
        return -1; //Read has failed
    }

    //Read 1 row of Pixels at a time
    while(size > 0)
    {
        if(size >= RowSize)
        {
            numBytes = RowSize;
        }
        else
        {
            numBytes = size;
        }

        r = FSfread((void *)byteBuff, 1, numBytes, pfd );
        if(r != numBytes) //Check for a read error.
        {
            FSfclose(pfd);
            return -1; //Read has failed
        }

        k=0;
        for(j=0; j<numBytes; j+=3)
        {
            Blue = (byteBuff[j] & 0xF8) >> 3;
            Green = (byteBuff[j+1] & 0xFC);
            Red = (byteBuff[j+2] & 0xF8);
            RGBbuf = ((Red<<8)+(Green<<3)+Blue);
            byteBuff[k] = (RGBbuf & 0x00FF);
            byteBuff[k+1] = ((RGBbuf>>8) & 0x00FF);
            k += 2;
        }
        
        flash_wrBuffer(byteBuff, startAddress, ((numBytes/3)*2));
        startAddress -= ((numBytes/3)*2);
        size -= numBytes;
        i+=numBytes;
    }

    FSfclose(pfd);

    //Write the header to flash
    if(pos == 0)
    {
        startAddress = MEM_SPLASH1;
    }
    // else if(pos == 1)  //Not using this right now
    // {
    // startAddress = MEM_WALLPAPER1;
    // }
    else
    {
        return -1; //Read has failed
    }

    numBytes = 6;
    byteBuff[0] = info[30]; //compression
    byteBuff[1] = 16; //colorDepth
    byteBuff[2] = info[22]; //height low byte
    byteBuff[3] = info[23]; //height high byte
    byteBuff[4] = info[18]; //width low byte
    byteBuff[5] = info[19]; //width high byte

    flash_wrBuffer(byteBuff, startAddress, numBytes);
    return i;
}
