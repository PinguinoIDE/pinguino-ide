//RTC PCF8593 particulars
// I2C frequency 400 kHz
// tested with Pinguino 4550 :
// PCF8593_SDA connected to pin0 and SCL to pin1
// PCF8593 powered  - by CR2032 lithium battery for backup
//         no need for 5V - don't forget to link PINGUINO and PCF GNDs
// With the battery connected, once you have initiated the PCF8593 using your Pinguino you don't need to keep your
// Pinguino connected
// You connect it and you open a RS232 terminal when you want to know time and date

// tested with Pinguino 26j50
// PCF8593_SDA connected to pin5 and SCL to pin4

u8 i2c_tx_buffer[8]={0,0,0,0,0,0,0,0};
u8 i2c_rx_buffer[7]={0,0,0,0,0,0,0};

u8 I2C_address=0xA2; //A2 write A3 read (8bits address-R/W bit included)
u8 adreg = 0x02;						//
u8 carrec;
u8 secondG, minuteG, hourG, dayOfWeekG, dayOfMonthG, monthG, yearG;

u8 I2C_send_receive(u8 address, u8 *wbuffer, u8 wlength, u8 *rbuffer, u8 rlength)
{
    u8 i;
    u8 temp;
// case of writing : wbuffer[0] = first register address to write --- wlength = n (data bytes to send) + 1 (address byte to specify)
//                   rlength = 0 if nothing to receive
// case of reading :  wbuffer[0] = first register address to read --- wlength = 1 (address byte to specify)
//                   rlength = n (data bytes to receive)

    I2C.start();
    if(!I2C.write(address))
    {
        I2C.stop();
        return(0);
    }
    for (i=0; i<wlength; i++)
    {
        if(!I2C.write(wbuffer[i]))
        {
            I2C.stop();
            break;
        }
    }
    if( i < wlength) return(0);
    if( rlength > 0)
    {
        temp = address | 0x01;
        I2C.restart();
        if(!I2C.write(temp))
        {
            I2C.stop();
            return(0);
        }
        for (i=0; i<rlength; i++)           // Sequential read (auto. inc.)
        {
            rbuffer[i] = I2C_read();
            if (i== (rlength-1))            // Last byte is sent ?
                I2C.sendNack();             // Yes, send a No Ack to the slave (no more data to read)
            else
                I2C.sendAck();              // No, send a Ack bit (more data has to be read)

        }
    }
    I2C.stop();
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

void setDate()
{
    u8 syy, smm, sdOfMonth, sdOfWk, shh, smn, sss;
    u8 nb_reg_towrite = 8;
    syy = 12; //year starting in 2012
    smm = 12; //month
    sdOfMonth = 04; // day of month
    sdOfWk=2; // day of week
    shh = 10; // hour 0-23
    smn = 56; // min 0-59
    sss = 0;  // sec
    i2c_tx_buffer[7] = (sdOfWk<<5) | decToBcd(smm);// day of week - month
    i2c_tx_buffer[6]=((syy%4)<<6) | decToBcd(sdOfMonth); // year - day of month
    i2c_tx_buffer[5]=decToBcd(shh);//hour
    i2c_tx_buffer[4]=decToBcd(smn);//min
    i2c_tx_buffer[3]=decToBcd(sss);//sec
//i2c_tx_buffer[0] initialized to 0
//i2c_tx_buffer[1] control/statut - initialized to 0
//i2c_tx_buffer[2] 1/100e s - initialized to 0
    while(!I2C_send_receive(I2C_address,i2c_tx_buffer,nb_reg_towrite,i2c_rx_buffer,0));
/*
Serial.printf("you enter : ");
Serial.print(i2c_tx_buffer[5],HEX);
Serial.printf("h ");
Serial.print(i2c_tx_buffer[4],HEX);
Serial.printf("min ");
Serial.print(i2c_tx_buffer[3],HEX);//smm
Serial.printf("sec ");
Serial.flush();*/
}

void getDate()
{
    u8 dd;
    u8 mo;
    i2c_tx_buffer[0] = adreg; // initialized to 2 - starting with seconds
    while(!I2C_send_receive(I2C_address,i2c_tx_buffer,1,i2c_rx_buffer,5));
    hourG = bcdToDec(i2c_rx_buffer[2]);
    minuteG = bcdToDec(i2c_rx_buffer[1]);
    secondG = bcdToDec(i2c_rx_buffer[0]);
    dd = i2c_rx_buffer[3];
    yearG = dd>>6;
    dayOfMonthG = bcdToDec(dd & 0x3F);
    mo = i2c_rx_buffer[4];
    dayOfWeekG = mo>>5;
    monthG = bcdToDec(mo & 0x1F);
}

void setup()
{
    I2C.master(I2C_400KHZ);
    Serial.begin(9600);
    Serial.printf("\n\rEnter u to initialize or update or c to continue");
    while(!Serial.available());  // a character has been received
    carrec=Serial.read();//
    if(carrec=='u') // time and date to init. or update
        setDate();

    Serial.printf("\n\rEnter t to get time and date");
}

void loop()
{
    while (!Serial.available());  // a character has been received
    carrec=Serial.read();//
    if(carrec=='t') // time and date to display
	  {
        getDate();
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