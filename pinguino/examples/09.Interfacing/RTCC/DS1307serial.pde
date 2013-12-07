//RTC DS1307 particulars
// case of writing : wbuffer[0] = first register address to write --- wlength = n (data bytes to send) + 1 (address byte to specify)
//                   rlength = 0 if nothing to receive
// case of reading :  wbuffer[0] = first register address to read --- wlength = 1 (address byte to specify)
//                   rlength = n (data bytes to receive)
// tested with Pinguino 4550 :
// DS 1307_SDA connected to pin0 and SCL to pin1
// DS 1307 powered  - by CR2032 lithium battery for backup
//                  - by 5V to communicate with Pinguino
// with the battery connected, once you have initiated the DS1307 using your Pinguino you don't need to keep your
// Pinguino connected
// You connect it and you open a RS232 terminal when you want to know time and date

u8 I2C_address=0xD0;	// i2c 8 bits address of DS1307
u8 reg=0x00;           // first addr. reg. to write or to read
u8 i2c_tx_buffer[8];
u8 i2c_rx_buffer[7]={0,0,0,0,0,0,0};
int secondG, minuteG, hourG, dayOfWeekG, dayOfMonthG, monthG, yearG;

u8 DS1307_send_receive(u8 address, u8 *wbuffer, u8 wlength, u8 *rbuffer, u8 rlength)
{
    u8 i;
    u8 temp;
	I2C.start();
	if(!I2C.write(address))
	{do {I2C.restart;}
	while(!I2C.write(address)); }
    for (i=0; i<wlength; i++)
    {
        while(!I2C.write(wbuffer[i]));
    }
	if( rlength > 0)
	{
    temp = address | 0x01;
		do {I2C.restart();}                 // start again
		while(!I2C.write(temp));             // read operation (bit 0 set to 1)
		for (i=0; i<rlength; i++)           // Sequential read (auto. inc.)
		{
			rbuffer[i] = I2C_read();
			if (i== (rlength-1))            // Last byte is sent ?
				I2C.sendNack();             // Yes, send a No Ack to the slave (no more data to read)
			else
				I2C.sendAck();              // No, send a Ack bit (more data has to be read)
		}
	}
    I2C_stop();
	return(1);
}

u8 decToBcd(u8 val)
{
  return ( (val/10*16) + (val%10) );
}

// Convert binary coded decimal to normal decimal numbers
u8 bcdToDec(u8 val)
{
  return ( (val/16*10) + (val%16) );
}

void setDateDs1307(u8 second,  
                   u8 minute, 
                   u8 hour,
                   u8 dayOfWeek,
                   u8 dayOfMonth,
                   u8 month,
                   u8 year)
{
  u8 j=0;
  u8 nb_reg_towrite = 8;
  i2c_tx_buffer[0]=reg;
  i2c_tx_buffer[1]=decToBcd(second);
  i2c_tx_buffer[2]=decToBcd(minute);
  i2c_tx_buffer[3]=decToBcd(hour);
  i2c_tx_buffer[4]=decToBcd(dayOfWeek);
  i2c_tx_buffer[5]=decToBcd(dayOfMonth);
  i2c_tx_buffer[6]=decToBcd(month);
  i2c_tx_buffer[7]=decToBcd(year);

  j=DS1307_send_receive(I2C_address,i2c_tx_buffer,nb_reg_towrite,i2c_rx_buffer,0);
}  

void getDateDs1307(void)	  
{
  u8 j;
  I2C.master(I2C_100KHZ);
  i2c_tx_buffer[0]=reg;
  
  while(!DS1307_send_receive(I2C_address,i2c_tx_buffer,1,i2c_rx_buffer,7));
  secondG     = bcdToDec(i2c_rx_buffer[0] & 0x7f);
  minuteG     = bcdToDec(i2c_rx_buffer[1] );
  hourG       = bcdToDec(i2c_rx_buffer[2]  & 0x3f);  // Need to change this if 12 hour am/pm
  dayOfWeekG  = bcdToDec(i2c_rx_buffer[3] );
  dayOfMonthG = bcdToDec(i2c_rx_buffer[4] );
  monthG      = bcdToDec(i2c_rx_buffer[5] );
  yearG       = bcdToDec(i2c_rx_buffer[6] );
}

void setup()
{
  u8 second, minute, hour, dayOfWeek, dayOfMonth, month, year;
  char carrec;
  I2C.master(I2C_100KHZ);
  Serial.begin(9600);
// following values to change according your time and date
  second = 0;
  minute = 42;
  hour = 9;
  dayOfWeek = 6;
  dayOfMonth = 1;
  month = 12;
  year = 12;
  Serial.printf("\n\rEnter u to initialize or update or c to continue");
  while(!Serial.available());  // a character has been received
  {
    carrec=Serial.read();//
    if(carrec=='u') // time and date to init. or update
      setDateDs1307(second, minute, hour, dayOfWeek, dayOfMonth, month, year);
  }
  Serial.printf("\n\rEnter t to get time and date");
}

void loop()
{
  char carrec;
  if(Serial.available())  // a character has been received
  {
    carrec=Serial.read();//
    if(carrec=='t') // time and date to display
	  {
	    getDateDs1307();
  	   Serial.printf("\n\rh: %d",hourG);
      delay(200);
	     Serial.printf(" m: %d",minuteG);
      delay(200);
	    Serial.printf(" s: %d",secondG);//....
      delay(200);
	    Serial.printf(" dd: %d",dayOfMonthG);//....
      delay(200);
	    Serial.printf(" mm: %d",monthG);//....
      delay(200);
	    Serial.printf(" yy: %d",yearG);//....
      delay(200);
	    Serial.printf(" wkday %d\n\r",dayOfWeekG);//....
      delay(200);
    }
    Serial.flush();
  }
}