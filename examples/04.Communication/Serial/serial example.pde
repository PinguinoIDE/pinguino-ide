// test serial with a modified Fonera
// jean-pierre MANDON 2008

int i;
int caractere;

void setup()
{
for (i=0;i<8;i++) 	{
				pinMode(i,OUTPUT);
				digitalWrite(i,LOW);
				}
Serial.begin(9600);
}

void loop()
{
if (Serial.available())
	{
	digitalWrite(0,HIGH);
	caractere=Serial.read();
	Serial.printf("test");
	if (caractere=='A') digitalWrite(1,digitalRead(1)^1);
	}
}
