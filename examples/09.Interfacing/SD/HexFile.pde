/*	---------------------------------------------------------------------------
	Read a file on SD card
	2011 Regis Blanchot
	<rblanchot@gmail.com>
	http://www.pinguino.cc

	circuit:
	* SD card attached to SPI bus as follows:
	** MOSI - pin 11
	** MISO - pin 12
	** CLK - pin 13
	** CS - pin 8

	output:
	(GNU/linux) sudo minicom -o -D /dev/ttyACM0
	press <return> to start
	---------------------------------------------------------------------------*/

#define B_SIZE 256	// blocks of arbitrary length,
					// try to change it to improve efficiency 

void setup()
{
}

void loop()
{
	char filename[] = "lorem.txt";
	SD_File file;
	char data[B_SIZE];
	int i, j;
    u16 br;
	SD_Error error;
	
	// wait for RETURN key to start
	CDC.println("Press RETURN to start.");
	while (CDC.getKey() != '\r');
	
	// mount FAT filesystem
	// Card Select output on pin 8
	if (SD.mount(8)) // or SD.init(8) or SD.begin(8)
	{
		// open file
		error = SD.open(&file, filename, FA_OPEN_EXISTING | FA_READ);
		if (!error){
			do {
				error = SD.read(&file, data, B_SIZE, &br);
				
				for(i = 0; i < br; i++)
				{
					CDC.printf("%02X  ", data[i]); 
					if (i%8 == 0)
					{
						for (j=i-8; j<i; j++)
						{
							if ( isAlphaNumeric(data[j]) && !isControl(data[j]) )
								CDC.printf("%c", data[j]);
							else
								CDC.printf(".");
						}
						CDC.printf("\r\n"); 
					}
				}
			} while(br == B_SIZE);
			CDC.printf("\r\n"); 
			SD.close(&file);
			SD.unmount();
		}
		else
		{
			CDC.println("file not found!");
		}
	}
	else
	{
		CDC.println("mount failed!");
	}
}
