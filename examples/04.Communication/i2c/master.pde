/*----------------------------------------------------- 
Author:  André Gentric
Date: Sat May 18 2013
Description:
I2C master example -
Tested with a Pinguino 18f4550 connected to another Pinguino 18f4550 (as master)
thru SDA and SCL lines (pins RB0, RB1). Each of these lines is connected to +5V 
thru a 2.2KOhm resistor.
Start the slave before the master
Open a tty terminal 9600 bds
Available commands :
v key validated by enter to get the slave program version (here 2C 1)
m key validated by enter to get a predefined message (here "hi!?")
l1 keys validated by enter to switch on a led connected to pin7 /slave
l1 keys validated by enter to switch off this same led
s key validated by enter to know the status (0 or 1) of this led
e key followed by any letter or digit you want to be dealt by the slave
A response is printed in case of v m s e

---------------------------------------------------------------------------------------*/
#define TR7 TRISBbits.TRISB7
#define LA7 LATBbits.LATB7
#define TR6 TRISBbits.TRISB6
#define LA6 LATBbits.LATB6

u8 I2C_address=0x2C; //8bits address-R/W bit included

u8 Line[128];  /* Serial Console input buffer */
static void get_line (u8 *buff, u8 len)
{
	u8 c, i;

	i = 0;
	for (;;) {
		c = Serial.getKey();
		if (c == '\r') break;
		if ((c == '\b') && i) i--;
		if ((c >= ' ') && (i < len - 1))
				{
         buff[i++] = c;
         Serial.printf("%c",c);
       }
	}
	buff[i] = 0;
	Serial.printf("\r\n");
}

void setup()
{
  Wire.begin(0,100);// first parameter = 0 means master mode; second bus speed = 100 KHz
  Serial.begin(9600);
}
void loop()
{
  u8 *ptr;
  u8 c;
// entering commands
  Serial.printf("\r\n>");
  get_line(Line, sizeof(Line));
  ptr = Line;
  delay(100);
  switch (*ptr++)
  {
    case 'v' :

//version request
  Wire.beginTransmission(I2C_address);
	Wire.write(0x80);
	Wire.endTransmission(1);
   delay(100);
 	Wire.requestFrom(I2C_address,2); //
  delay(100);
  Serial.printf("<");
	while(Wire.available())    // slave may send less than requested
    { 
      c = Wire.read(); // receive a byte as character
      Serial.printf("%2X",c);         // print the character
    }
  break;
  
    case 'l' : // led on request
    {
       c = *ptr;
       Wire.beginTransmission(I2C_address);
	     Wire.write(0x90);
       Wire.write(c);
      	Wire.endTransmission(1);
      break;
    }
    case 's' :
// led on status

	Wire.beginTransmission(I2C_address);
	Wire.write(0x90);
 	Wire.endTransmission(1);
  delay(100);
	Wire.requestFrom(I2C_address,1);
	while(Wire.available())    // slave may send less than requested
    { 
      c = Wire.read(); // receive a byte as character
      Serial.printf("<%2X",c);         // print the character
    }
    break;
    case 'e' : // echo request
       c=*ptr;
	Wire.beginTransmission(I2C_address);
	Wire.write(c);
 	Wire.endTransmission(1);
  delay(100);
	Wire.requestFrom(I2C_address,1);
 delay(100);
 	while(Wire.available())    // slave may send less than requested
    { 
      delay(100);
      c = Wire.read(); // receive a byte as character
      Serial.printf("<%c",c);         // print the character
    }
    break;
 
 
    case 'm' : // echo request
	Wire.beginTransmission(I2C_address);
	Wire.write(0x81);
 	Wire.endTransmission(1);
  delay(100);
	Wire.requestFrom(I2C_address,4);
 delay(100);
  Serial.printf("<");
  while(Wire.available())    // slave may send less than requested
    { 
      delay(100);
      c = Wire.read(); // receive a byte as character
      Serial.printf("%c",c);         // print the character
    }
    break;

 } 

}
