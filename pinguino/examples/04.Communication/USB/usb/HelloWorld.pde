/*
	Pinguino example to send bytes on USB bus
	Result can be read with HelloWorld.py
	author		Régis Blanchot
	first release	14/09/2010
	last update	17/07/2013
*/

void setup()
{
}

void loop()
{
  	USB.send("Hello World !\r\n", 15);			// send 15 bytes on usb bus
		toggle(USERLED);		                    // blinked user led for visual debug
		delay(1000);			                      // wait for 1 sec. before next sending
}
