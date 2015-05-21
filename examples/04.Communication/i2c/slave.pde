/*----------------------------------------------------- 
Author:  André Gentric
Date: Sat May 18 2013
Description:
I2C slave example - application number 0x2C version 0x01
tested with a Pinguino 18f4550 connected to another Pinguino 18f4550 (as master)
thru SDA and SCL lines (pins RB0, RB1). Each of these lines is connected to +5V 
thru a 2.2KOhm resistor
Available commands :
0x80 to get these application number and version
0x81 to get a predefined message - to change according your requirement
0x90 + 0x48 or 0x49 ('0'or '1' ASCII) to switch off or on a led on RB6
0x90 to get the led (RB6) status -> 1 (on) 0 (off)
any other (character/digit) code will be increased by 1 and returned to master

Other commands may be added and handled in receiveEvent routine.

See the master application to know how these commands are generated  
-----------------------------------------------------*/
#define SLAVE_ADDRESS 0x2C
#define I2C_BUFFER_SIZE 32
#define TR7 TRISBbits.TRISB7
#define LA7 LATBbits.LATB7
#define TR6 TRISBbits.TRISB6
#define LA6 LATBbits.LATB6

void receiveEvent(u8);
void requestEvent(void);
void fillmessage(u8 *, u8 *);

u8 rxbuffer[2];
u8 cdcount;

void setup()
{
u8 i;
  TR7=0;
  LA7=0;
  TR6=0;
  LA6=0;

  Wire.begin(SLAVE_ADDRESS,0); // join i2c bus with address #xx
                               // the second parameter (0) is not used in the present slave mode 
  for(i=0;i<6;i++)  // to show the slave is started waiting interrupts and commands
    {LA7=!LA7; delay(500);}
  Wire.onReceive(receiveEvent); // register event
// In this example the following call is required because in the requestEvent procedure
// the slave send data to the master as requested by it thru a requestFrom call :
  Wire.onRequest(requestEvent); // register event
 }

void loop()
{
// slave working on interrupts
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
u8 txbuffer[32];
u8 message[] ="hi!?"; // this is a simple example

  if (rxbuffer[0] == 0x80)  // request version 
  {
	  // cmd 0x80 - request version
    txbuffer[0]   = 0x2C;         // application code
    txbuffer[1]   = 0x01;         // version
    Wire.writeS(txbuffer,2);
  }
  else if (rxbuffer[0] == 0x81)   
  {
	  // cmd 0x81 - request message
    fillmessage(message,txbuffer);
    Wire.writeStr(txbuffer);
  }
  else if (rxbuffer[0] == 0x90)           
  {   
    if (cdcount == 1)
         // cmd 0x90 without params: request for led status
      if (LA6)
        Wire.write(0x01);
      else
        Wire.write(0x00);
  }
  else
    Wire.write(rxbuffer[0]+1);

}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(u8 count)
{
u8 i;
  i=0;
  cdcount = count;
  while(I2C_available())
    rxbuffer[i++] = Wire.read(); 

  if (rxbuffer[0] == 0x90)           
    if (count == 2)
         //cmd 0x90 - turn LED off or on
      if (rxbuffer[1] == '1') // master send ASCII
        LA6 = 1;
      else
        LA6 = 0;

   
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
