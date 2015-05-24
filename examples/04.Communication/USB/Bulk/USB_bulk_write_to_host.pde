/*----------------------------------------------------- 
Author:  --<>
Date: Sat Sep 14 09:08:19 2013
Description: usb exchanges between PC and Pinguino bootloader V4 or V2.12
With V4 it uses BULKgets() and BULKputs() functions defined in usb_bulk.c
here used thru alias BULK.read() and BULK.write()
With bootloader V2.12 it uses usbreceive() and usbsend() functions defined in usb.c
-------------------------------------------------------*/

void setup()
{
    pinMode(USERLED, OUTPUT);
}

void loop()
{
    BULK.write("Pinguino\r\n", 10);
    toggle(USERLED);
    delay(1000);
}
