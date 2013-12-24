/*----------------------------------------------------- 
Author:  --<>
Date: Sat Sep 14 09:08:19 2013
Description: usb exchanges between PC and Pinguino bootloader V4 or V2.12
With V4 it uses BULKgets() and BULKputs() functions defined in usb_bulk.c
here used thru alias BULK.read() and BULK.write()
With bootloader V2.12 it uses usbreceive() and usbsend() functions defined in usb.c
You also can send data with BULK.printf. These data will be formatted according your need and statement.
Two methods are proposed to 
-------------------------------------------------------*/
u8 receivedbyte;
char buffer[64];

void setup()
{
    pinMode(USERLED, OUTPUT);
}

void loop()
{
    receivedbyte=0;

//First method to test if data are present
/*
//Here you wait until data are present then you read   
    do {
      receivedbyte = BULK.read(buffer);
    } while (receivedbyte == 0);
*/

//Second method to test if data are present
//Here if data are not present you continue
    if(BULK.available()) 
      receivedbyte = BULK.read(buffer);   
    // C string must be null-terminated
    
    
    buffer[receivedbyte] = 0;

    if (receivedbyte > 0)
    {
      BULK.write(buffer, receivedbyte);
      //BULK.printf("Received : %s\r\n", buffer);
      //BULK.printf("Length = %d bytes\r\n", receivedbyte);
    }
    
    toggle(USERLED);
}
