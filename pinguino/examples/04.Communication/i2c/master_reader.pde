/*----------------------------------------------------- 
Author:  André Gentric
Date: Sat May 18 2013
Description:
This is the master reader program
associated to "wire_st.pde" the slave transmitter program
available speeds are 100, 400, 1000 Khz coded respectively 1, 4, 10 in Wire.begin
Upload each .hex
Open a tty terminal 9600 bds connected to the master to show the received message
If a terminal is already opened shut it and restart it
Start the slave (even if it starts at the end of uploading) before the master
Tested with a Pinguino 18f4550 connected to another Pinguino 18f4550 (as master)
thru SDA and SCL lines (pins RB0, RB1). Each of these lines is connected to +5V 
thru a 2.2KOhm resistor.
-----------------------------------------------------*/

u8 I2C_address = 0x2C; //slave address

void setup() {
    //run once:
  Wire.begin(0,100); // 0=master mode and 100=100 KHz as bus speed    
  Serial.begin(9600);
  delay(100);
}
void loop() {
u8 c[32]; // length to adapt to your requirement
u8 i;
    //run repeatedly:
  Wire.requestFrom(I2C_address,32); // here 32 characters as maxi are requested
  delay(500);  Serial.printf("\r\n<");
  delay(200);
  while(!Wire.available());
  i=0;
  while(Wire.available())    // slave may send less than requested
  { 
    c[i] = Wire.read(); // receive a byte as character
	Serial.printf("%c",c[i]); // print the character
	i++;
  }

  delay(1000);
}