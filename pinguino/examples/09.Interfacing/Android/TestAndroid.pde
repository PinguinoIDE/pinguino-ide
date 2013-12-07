// Android interface with Pinguino
// Jean-Pierre Mandon 2011

void setup()
{
Adb.init();							// Initialise communication with Android Device
Serial.begin(9600);
}

unsigned char ADBrx[100];			// receive buffer
unsigned char ADBopened=0;			// flag 
unsigned int length;
unsigned int i;
unsigned char framereceived=0;		// this flag is used to send a frame every 10 rx frame

void loop()
{
unsigned char connected=Adb.refresh();		// Adb.refresh must be called periodically

if (connected)								// Android device detected
	{
	if (!ADBopened) ADBopened=Adb.open();	// Open tcp connexion with Android device
		if (ADBopened)
			{
			if (Adb.available())			// character received ?
				{
				length=Adb.read(ADBrx);		// Read buffer
				if (length>0) 
					{
					Serial.printf("length=%d\n\r",length);
					for (i=0;i<length;i++) Serial.printf("%c",ADBrx[i]);
					Serial.printf("\n\r");
					framereceived++;
					}
				}
			if (framereceived==10) 
				{
				Adb.send("TOTO\n\r",6);		// send frame to Android device
				framereceived=0;
				}
			}
	}
}