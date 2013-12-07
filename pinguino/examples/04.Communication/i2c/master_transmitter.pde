/*----------------------------------------------------- 
Author:  André Gentric
Date: Sat May 18 2013
Description:
This is the master transmitter program
associated to "wire_sr.pde" the slave receiver program
available speeds are 100, 400, 1000 Khz coded respectively 1, 4, 10 in Wire.begin
Upload each .hex
Open a tty terminal 9600 bds connected to the slave
Start the slave (even if it starts at the end of uploading) before the master
Tested with a Pinguino 18f4550 connected to another Pinguino 18f4550 (as master)
thru SDA and SCL lines (pins RB0, RB1). Each of these lines is connected to +5V 
thru a 2.2KOhm resistor.
-----------------------------------------------------*/
u8 I2C_address = 0x2C; 

void setup() {
    //run once:
  Wire.begin(0,100); // 0=master mode and 100=100 KHz as bus speed   
}

u8 x=0;
u8 message[3]= {'H','i','!'};

void loop() {
u8 convd[3];
    //run repeatedly:
  Wire.beginTransmission(I2C_address);
  Wire.writeS(message,3);
  Sprintf(convd,"%3d",x); // to convert a number to a string
  Wire.writeS(convd,3);
  Wire.endTransmission(1);
  x++;
  delay(5000);
}