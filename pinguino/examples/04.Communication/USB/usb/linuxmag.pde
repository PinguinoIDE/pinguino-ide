// test USB Linux Magazine Pinguino
// Jean-Pierre MANDON 2008

int i;
uchar caractere,caractere1;

void setup() 
{
for (i=0;i<8;i++) 
	{
	pinMode(i,OUTPUT);
	digitalWrite(i,LOW);
	}
}

void loop() 
{
if (USB.available())
	{
	caractere=USB.read();
	if (caractere=='W') if (USB.available())
		{
		caractere1=USB.read()-0x30;
		digitalWrite(caractere1,digitalRead(caractere1)^1);
		}
	}
}
