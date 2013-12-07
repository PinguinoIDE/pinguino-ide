// Simple serial test
// Jean-Pierre Mandon 2010

char caractere;
char chaine[5]={'T','E','S','T',0};

void setup()
{
	Serial.begin(9600);
}

void loop()
{
	Serial.printf("TEST\n\r");
	if (Serial.available())
	{
		Serial.printf("caractere \r\n");
		caractere=Serial.read();
		Serial.printf("%s \r\n",chaine);
		Serial.print(caractere,DEC);
        delay(1000);
	}
    delay(100);
}