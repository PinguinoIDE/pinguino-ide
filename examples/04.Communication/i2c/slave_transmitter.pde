/*----------------------------------------------------- 
Author:  André Gentric
Date: Sat May 18 2013
Description:
This slave transmitter program is associated to "wire_mr.pde" - master reader program -
tested with 2 18f4550 pinguinos 
Upload each .hex
Open a tty terminal 9600 bds connected to the master to show the received message
Start the slave (even if it starts at the end of uploading) before the master
-----------------------------------------------------*/
#define TR7 TRISBbits.TRISB3 // led used briefly when the program starts
                             // as soon as an first interrupt happens this led turns on
							 // when an error happens relating interrupt the led blinks rapidly and indefinitely
#define LA7 LATBbits.LATB3
#define TR6 TRISBbits.TRISB2 // led blinks when requestEvent(...) is performed
#define LA6 LATBbits.LATB2

void receiveEvent(u8);
void requestEvent(void);
void fillmessage(u8 *, u8 *);

u8 I2C_address = 0x2C; //slave address

void setup() {
    //run once:
u8 i;
  TR7=0;
  LA7=0;
  TR6=0;
  LA6=0;
  Wire.begin(I2C_address,0); // the second parameter (0) is not used in the present slave mode    
  for(i=0;i<6;i++)  // to show the slave is started waiting interrupts and commands
    {LA7=!LA7; delay(500);}
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent); // register event
  delay(100);
    }

void loop() {
    //run repeatedly:

}
void receiveEvent(u8 count)
{
u8 rxbuffer[2];
u8 i;
  i=0;
  while(I2C_available())
    rxbuffer[i++] = Wire.read(); 
}
void requestEvent()
{
u8 txbuffer[32];
u8 message[] ="hi!?"; // this is a simple example
  fillmessage(message,txbuffer);
  LA6=!LA6; // led blinking to show requestEvent is executed
  Wire.writeStr(txbuffer);
}

void fillmessage(u8 *tosend, u8 *buff)
{
//This function might be more complicated to send text + numbers (rcv codified)
u8 i = 0;
  while (tosend[i] != '\0')
  {
    buff[i] = tosend[i];
    ++i;
  }
  buff[i] = tosend[i];
}
