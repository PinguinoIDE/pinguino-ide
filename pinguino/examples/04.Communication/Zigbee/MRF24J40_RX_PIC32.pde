// zigbee receive example
// Jean-Pierre Mandon 2011

#define ZIGchannel 20					// channel must be selected between 11 and 26
#define ZIGNetwork 0xFEAA				// personal area network number ( between 0 and 65535 )
#define ZIGAddress 0x0002				// Address of the module ( between 0 and 65535 )

unsigned char rxdata[128];				// 128 is the max length

void setup()
{
	Serial.begin(9600);
	ZIG.init(ZIGchannel,ZIGNetwork,ZIGAddress);
}

void loop()
{
	unsigned char length;
	unsigned int i;

	length=ZIG.read(rxdata); // received data in rxdata buffer
	if (length>0)
	{
		Serial.printf("Source PAN ID:%04X\n\r",ZIGsrcpan);				// received from Personal Area Network
		Serial.printf("Destination PAN ID:%04X\n\r",ZIGdestpan);		// sent to Personal Area Network
		Serial.printf("Source address:%04X\n\r",ZIGsrcadd);				// received from module address	
		Serial.printf("Destination address:%04X\n\r",ZIGdestadd);		// sent to module address
		Serial.printf(rxdata);											// received data
		Serial.printf("\n\r");
	}
}
