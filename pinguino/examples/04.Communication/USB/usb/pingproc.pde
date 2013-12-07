// test USB processing Pinguino
// Jean-Pierre MANDON 2009

int i;
u8 caractere,caractere1;

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
		caractere1=USB.read();
		if (caractere1=='C') 
			for (i=0;i<8;i++) digitalWrite(i,LOW);
		else  
			digitalWrite(caractere1,digitalRead(caractere1)^1);
		}
	}
}
