//
//
int i;
int state;
uchar caractere,caractere1,caractere2;

void setup() 
{
for (i=0;i<8;i++) pinmode(i,OUTPUT);}
void loop() 
{
i=1000;
if (usbavailable())
	{
	caractere=usbread();
	if (caractere=='W') if (usbavailable())
		{
		caractere1=usbread();
		caractere2=usbread();
		digitalwrite(caractere1,caractere2);
		if ((caractere1==0) && (caractere2==1))
			usbsendint(i);
		}
	}
}


