// test USB on Pinguino
// Jean-Pierre MANDON 2008
int i;
int state;
uchar caractere,caractere1,caractere2;

void setup() 
{
for (i=0;i<8;i++) pinMode(i,OUTPUT);}
void loop() 
{
i=1000;
if (USB.available())
	{
	caractere=USB.read();
	if (caractere=='W') if (USB.available())
		{
		caractere1=USB.read();
		caractere2=USB.read();
		digitalWrite(caractere1,caractere2);
		if ((caractere1==0) && (caractere2==1))
			USB.send(i,2);
		}
	}
}
