/*	----------------------------------------------------------------------------
	FILE:			i2cmaster.c
	PROJECT:		pinguino
	PURPOSE:		Include functions to handle I2C communication by Slave
	PROGRAMMER:      A. GENTRIC		
	FIRST RELEASE:	abr. 2013
	LAST RELEASE:	
	----------------------------------------------------------------------------

Adapted from Nicholas Zambetti Todd Krein's programs 
             TWI/I2C library for Wiring & Arduino
	---------------------------------------------------------------------------- */


#include <stdlib.h>
//#include <serial.c>

// i2c buffer length
#ifndef _I2CBUFFERLENGTH_
#define _I2CBUFFERLENGTH_ 32
#endif
// Mode I2C
#define I2C_WRITE		0
#define I2C_READ		1
#define I2C_MASTER_MODE	0
#define I2C_SLAVE_MODE	1
#define I2C_SLEW_OFF	0
#define I2C_SLEW_ON		1

// Speed definitions
#define I2C_100KHZ		100
#define I2C_400KHZ		400
#define I2C_1MHZ		1000

// i2c buffer length
#ifndef _I2CBUFFERLENGTH_
#define _I2CBUFFERLENGTH_ 32
#endif

#define I2C_READY 0
#define I2C_MRX   1
#define I2C_MTX   2
#define I2C_SRX   3
#define I2C_STX   4

void I2C_master(u16);
void I2C_slave(u16);   
void I2C_init(u8, u16);
u8 I2C_write(u8);
u8 I2C_read();
void I2C_idle();
void I2C_start();
void I2C_stop();
void I2C_restart();
void I2C_sendNack();
void I2C_sendAck();

u8 I2C_transmit(const u8*, u8);
u8 I2C_writeTo(u8 ,u8* , u8 , u8, u8 );
u8 I2C_readFrom(u8 ,u8* , u8 , u8 );

static void I2C_onRequest (void(*func)(void));
static void I2C_onReceive (void(*func)( u8));
static void (*_i2c_onRequest_function)(void);
static void (*_i2c_onReceive_function)( u8);

static volatile u8 i2c_state;
static volatile u8 i2c_slarw;
static volatile u8 i2c_sendStop;			// should the transaction end with a stop
static volatile u8 i2c_inRepStart;			// in the middle of a repeated start
static volatile u8 i2c_error;

static volatile u8 _i2c_txBuffer[_I2CBUFFERLENGTH_];              // i2c tx buffer
static volatile u8 _i2c_rxBuffer[_I2CBUFFERLENGTH_];              // i2c rx buffer
static volatile u8 _i2c_rxBufferIndex = 0;
static volatile u8 _i2c_rxBufferLength = 0;

static volatile u8 _i2c_txAddress = 0;
static volatile u8 _i2c_txBufferIndex = 0;
static volatile u8 _i2c_txBufferLength = 0;
static volatile u8 _i2c_transmitting = 0;

static volatile u8 _i2c_masterBuffer[_I2CBUFFERLENGTH_];
static volatile u8 _i2c_masterBufferIndex;
static volatile u8 _i2c_masterBufferLength;

#ifdef I2CINT
  #include <i2cslave.c>
#endif

/* 
 * Function I2C_readFrom
 * Desc     attempts to become I2C bus master and read a
 *          series of bytes from a device on the bus
 * Input    address: 8bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes to read into array
 *          sendStop: indicating whether to send a stop at the end
 * Output   number of bytes read
 */
u8 I2C_readFrom(u8 address, u8* data, u8 length, u8 sendStop)
{
  u8 i;
  u8 resp_sla;
  // ensure data will fit into buffer
  if( length > _I2CBUFFERLENGTH_){
    return 0;
  }

  // wait until twi is ready, become master receiver
  while(i2c_state != I2C_READY){
     continue;
  }
  i2c_state = I2C_MRX;// Master Receiver
  i2c_sendStop = sendStop;
  // reset error state (0xFF.. no error occured)
  i2c_error = 0xFF;

  // initialize buffer iteration vars
  _i2c_masterBufferIndex = 0;
  _i2c_masterBufferLength = length;  // This is not intuitive, read on...
  // On receive, the previously configured ACK/NACK setting is transmitted in
  // response to the received byte before the interrupt is signalled. 
  // Therefor we must actually set NACK when the _next_ to last byte is
  // received, causing that NACK to be sent in response to receiving the last
  // expected byte of data.

  // build sla+w, slave device address + w bit
  i2c_slarw = (address) |I2C_READ;

  if (i2c_inRepStart) {
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the 
    // repeated start that we sent outselves, and that would really confuse things.
    i2c_inRepStart = false;// remember, we're dealing with an ASYNC ISR
    I2C_restart();
	resp_sla = I2C_write(i2c_slarw);
  }
  else {
    // send start condition
    I2C_start();
    resp_sla = I2C_write(i2c_slarw);
  }
       //address sent, ack received
      // put byte into buffer
	if (resp_sla) {
	while (_i2c_masterBufferIndex < _i2c_masterBufferLength)
	  {
       _i2c_masterBuffer[_i2c_masterBufferIndex++] = I2C_read();
       // data received, ack sent
	   // ack if more bytes are expected, otherwise nack
      if(_i2c_masterBufferIndex == _i2c_masterBufferLength){
          I2C_sendNack();
        }else{
          I2C_sendAck();
        }
      }
    }
	

  if (_i2c_masterBufferIndex < length)
    length = _i2c_masterBufferIndex;

  // copy i2c buffer to data
  for(i = 0; i < length; ++i){
    data[i] = _i2c_masterBuffer[i];
  }
  if (i2c_sendStop)
    I2C_stop();
  else {
	i2c_inRepStart = true;
  }
  i2c_state = I2C_READY;
  return length;
}

/* 
 * Function I2C_writeTo
 * Desc     attempts to become twi bus master and write a
 *          series of bytes to a device on the bus
 * Input    address: 7bit i2c device address
 *          data: pointer to byte array
 *          length: number of bytes in array
 *          wait: boolean indicating to wait for write or not
 *          sendStop: boolean indicating whether or not to send a stop at the end
 * Output   0 .. success
 *          1 .. length to long for buffer
 *          2 .. address send, NACK received
 *          3 .. data send, NACK received
 *          4 .. other twi error (lost bus arbitration, bus error, ..)
 */
u8 I2C_writeTo(u8 address, u8* data, u8 length, u8 wait, u8 sendStop)
{
  u8 i;
  u8 resp_sla;
  u8 resp_dat;
  // ensure data will fit into buffer
  if(length > _I2CBUFFERLENGTH_){
    return 1;
  }

  // wait until twi is ready, become master transmitter
  while(i2c_state != I2C_READY){
    continue;
  }
  i2c_state = I2C_MTX;
  i2c_sendStop = sendStop;
  // reset error state (0xFF.. no error occured)
  i2c_error = 0xFF;

  // initialize buffer iteration vars
  _i2c_masterBufferIndex = 0;
  _i2c_masterBufferLength = length;
  
  // copy data to twi buffer
  for(i = 0; i < length; ++i){
    _i2c_masterBuffer[i] = data[i];
  }
  
  // build sla+w, slave device address + w bit
  i2c_slarw = address;  
  // if we're in a repeated start, then we've already sent the START
  // in the ISR. Don't do it again.
  //
  if (i2c_inRepStart  == true) {
    // if we're in the repeated start state, then we've already sent the start,
    // (@@@ we hope), and the TWI statemachine is just waiting for the address byte.
    // We need to remove ourselves from the repeated start state before we enable interrupts,
    // since the ISR is ASYNC, and we could get confused if we hit the ISR before cleaning
    // up. Also, don't enable the START interrupt. There may be one pending from the 
    // repeated start that we sent outselves, and that would really confuse things.
    i2c_inRepStart = false;			// remember, we're dealing with an ASYNC ISR
	resp_sla = I2C_write(i2c_slarw);
  }
  else {
    // send start condition
    I2C_start();
	resp_sla = I2C_write(i2c_slarw);
 }
if(resp_sla) {
//addr ack
	while(_i2c_masterBufferIndex < _i2c_masterBufferLength){
        // copy data to output register and ack
        resp_dat= I2C_write(_i2c_masterBuffer[_i2c_masterBufferIndex++]);
		if (!resp_dat) {
          break;
		}
    }

	
	if (i2c_sendStop)
          I2C_stop();
	else {
	  i2c_inRepStart = true;	// we're gonna send the START
	  // don't enable the interrupt. We'll generate the start, but we 
	  // avoid handling the interrupt until we're in the next transaction,
	  // at the point where we would normally issue the start.
	}
    i2c_state = I2C_READY;
}
  if (i2c_error == 0xFF)
    return 0;	// success
  else
    return 4;	// i2c error

}

/* 
 * Function I2C_transmit
 * Desc     fills slave tx buffer with data
 *          must be called in slave tx event callback
 * Input    data: pointer to byte array
 *          length: number of bytes in array
 * Output   1 length too long for buffer
 *          2 not slave transmitter
 *          0 ok
 */
#ifdef I2CINT
u8 I2C_transmit(const u8* data, u8 length)
{
  u8 i;

  // ensure data will fit into buffer
  if(length > _I2CBUFFERLENGTH_){
    return 1;
  }
  
  // ensure we are currently a slave transmitter
  if(i2c_state != I2C_STX){
    return 2;
  }
  
  // set length and copy data into tx buffer
  _i2c_txBufferLength = length;
  for(i = 0; i < length; ++i){
    _i2c_txBuffer[i] = data[i];
  }
 //serial_printf("\r\n%2X",length); 
  return 0;
}
#endif

void I2C_begin(u8 address, u16 speed)
{
    if (address !=0) {
        //  SSPADD = address; done in I2C_init
        //  I2C_slave((u16)address);
        #ifdef I2CINT
        i2c_hw_slave_init(address);
        #endif
    }
    
    _i2c_rxBufferIndex = 0;
    _i2c_rxBufferLength = 0;

    _i2c_txBufferIndex = 0;
    _i2c_txBufferLength = 0;

    // initialize state
    i2c_state = I2C_READY;
    i2c_sendStop = true;		// default value
    i2c_inRepStart = false;

    if (address ==0) {
        I2C_master(speed);
    }

}

/*	----------------------------------------------------------------------------
	---------- requestFrom
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

u8 I2C_requestFrom(u8 address, u8 quantity)
{
    u8 read;
	u8 sendStop;
    
    // clamp to buffer length
    if (quantity > _I2CBUFFERLENGTH_)
        quantity = _I2CBUFFERLENGTH_;

    // perform blocking read into buffer
	sendStop = true;
    read = I2C_readFrom(address, _i2c_rxBuffer, quantity, sendStop);

    // set rx buffer iterator vars
    _i2c_rxBufferIndex = 0;
    _i2c_rxBufferLength = read;

    return read;
}


/*	----------------------------------------------------------------------------
	---------- beginTransmission
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

void I2C_beginTransmission(u8 address)
{
    // indicate that we are transmitting
    _i2c_transmitting = 1;
    // set address of targeted slave
    _i2c_txAddress = address;
    // reset tx buffer iterator vars
    _i2c_txBufferIndex = 0;
    _i2c_txBufferLength = 0;
}


/*	----------------------------------------------------------------------------
	---------- endTransmission
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/

u8 I2C_endTransmission( u8 sendStop )
{
 //   u8 sendStop = true;
    u8 ret;
    // transmit buffer (blocking)
    ret = I2C_writeTo(_i2c_txAddress, _i2c_txBuffer, _i2c_txBufferLength, 1, sendStop);
    // reset tx buffer iterator vars
    _i2c_txBufferIndex = 0;
    _i2c_txBufferLength = 0;
    // indicate that we are done transmitting
    _i2c_transmitting = 0;

    return ret;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
// *****
u8 I2C_inBuffer(u8 data)
{
  if(_i2c_transmitting){
  // in master transmitter mode
    // don't bother if buffer is full
    if(_i2c_txBufferLength >= _I2CBUFFERLENGTH_){
//      setWriteError();
      return 0;
    }
    // put byte in tx buffer
    _i2c_txBuffer[_i2c_txBufferIndex] = data;
    ++_i2c_txBufferIndex;
    // update amount in buffer
//serial_printf("\r\n%2X",data);	
    _i2c_txBufferLength = _i2c_txBufferIndex;
  }
#ifdef I2CINT
  else{
  // in slave send mode
    // reply to master

    I2C_transmit(&data, 1);

  }
#endif
  return 1;
}

// must be called in:
// slave tx event callback
// or after beginTransmission(address)
u8 I2C_inBufferS(u8 *data, u8 quantity)
{
  u8 i;
  if(_i2c_transmitting){
  // in master transmitter mode
    for( i = 0; i < quantity; ++i){
      I2C_inBuffer(data[i]);
    }
  }
#ifdef I2CINT
  else{
  // in slave send mode
    // reply to master

    I2C_transmit(data, quantity);

  }
#endif
  return quantity;
}
u8 I2C_inBufferStr(u8 *data)
{
  u8 i;
  if(_i2c_transmitting){
  // in master transmitter mode
    i = 0;
	while (data[i] != '\0')
	{
      I2C_inBuffer(data[i]);
	  ++i;
    }
  }
#ifdef I2CINT
  else{
  i=0;
  while (data[i] != '\0') i++;
  // in slave send mode
    // reply to master

    I2C_transmit(data,i);

  }
#endif
  return i;
}
/*	----------------------------------------------------------------------------
	---------- available
	----------------------------------------------------------------------------
    must be called in:
    slave rx event callback
    or after requestFrom(address, numBytes)
	--------------------------------------------------------------------------*/

u8 I2C_available()
{
  return _i2c_rxBufferLength - _i2c_rxBufferIndex;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
/*int available(void)
{
  return rxBufferLength - rxBufferIndex;
}
*/
/*	----------------------------------------------------------------------------
	---------- receive
	----------------------------------------------------------------------------
    must be called in:
    slave rx event callback
    or after requestFrom(address, numBytes)
	--------------------------------------------------------------------------*/

u8 I2C_outBuffer()
{
    // default to returning null char
    // for people using with char strings
    u8 value = '\0';
    // get each successive byte on each call
    if (_i2c_rxBufferIndex < _i2c_rxBufferLength)
    {
        value = _i2c_rxBuffer[_i2c_rxBufferIndex];
        ++_i2c_rxBufferIndex;
    }
    return value;
}

// must be called in:
// slave rx event callback
// or after requestFrom(address, numBytes)
int peek(void)
{
  int value = -1;
  
  if(_i2c_rxBufferIndex < _i2c_rxBufferLength){
    value = _i2c_rxBuffer[_i2c_rxBufferIndex];
  }

  return value;
}

/*	----------------------------------------------------------------------------
	---------- OnRequest
	----------------------------------------------------------------------------
	--------------------------------------------------------------------------*/
#ifdef I2CINT

void I2C_onRequest(void (* func) ())
{
    _i2c_onRequest_function = func;
	i2c_state = I2C_STX;
}

void I2C_onReceive(void (*func) (u8)) {
    // alert user program
    _i2c_onReceive_function = func;
	i2c_state = I2C_SRX;
    _i2c_rxBufferIndex = 0;
    I2C_sendAck();
}
#endif  
