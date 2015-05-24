// test serial with a modified Fonera
// pinguino is connected to the serial port of the Fonera
// jean-pierre MANDON 2008
// modified beta 9 2010

int i;
int caractere,caractere1,caractere2;

void setup()
{
// led's are on output 0..7
for (i=0;i<8;i++) 	{
				pinMode(i,OUTPUT);
				digitalWrite(i,LOW);
				}
// Serial is 9600 bauds
Serial.begin(9600);
}

void loop()
{
// if something was received
if (Serial.available())
	{
	// read received character
	caractere=Serial.read();
	if (caractere=='W')
		{
		// wait for the next character
		delay(3);
		// and try to read if something was received
		if (Serial.available()) caractere1=Serial.read();
		else 	{
				// else reset receive buffer
				Serial.flush();
				//and exit from loop
				goto end;
				}
		delay(3);
		if (Serial.available()) caractere2=Serial.read();
		else  	{
				Serial.flush();
				goto end;
				}
		// second and third characters are ASCII so sub 48 decimal
		if ((caractere1>=0x30)&&(caractere2>=0x30))
			{
			caractere1-=0x30; 
			caractere2-=0x30;		
			}
		else goto end;
		// and use those characters to light a led
		digitalWrite(caractere1,caractere2);
		// say OK to the transmitter
		Serial.printf("bonjour\n\r");
		end:;		
		}
	}
}