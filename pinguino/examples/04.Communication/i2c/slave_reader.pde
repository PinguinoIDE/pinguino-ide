/*----------------------------------------------------- 
Author:  André Gentric
Date: Sat May 18 2013
Description:
This is the slave receiver program
associated to "wire_mt.pde" the master transmitter program
Upload each .hex
Open a tty terminal 9600 bds connected to the slave to show the received message
Start the slave (even if it starts at the end of uploading) before the master
-----------------------------------------------------*/
void receiveEvent(u8);

u8 I2C_address = 0x2C; //slave address

void setup() {
    //run once:
  Serial.begin(9600);
  delay(100);
  Serial.printf("Test i2C master/slave\r\n");
  Wire.begin(I2C_address,0); // the second parameter (0) is not used in the present slave mode    
  Wire.onReceive(receiveEvent);

    }

void loop() {
    //run repeatedly:

    }
void receiveEvent(u8 count)
{
u8 rxbuffer[32]; //length to adapt according your requirement
u8 i;
u8 x;

  i=0;
  Serial.printf("\r\n");
  while(I2C_available())
  {
     rxbuffer[i] = Wire.read();
     Serial.printf("%c", rxbuffer[i]);
     i++;
  }
}