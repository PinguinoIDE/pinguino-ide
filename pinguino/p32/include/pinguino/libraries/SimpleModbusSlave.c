/*
 * File			SimpleModbusSlave.c
 *
 *         Description		MODBUS Slave protocol implentation, only command 3 (Read Holding Registers)
 *                      and 16 (Write Multiple Registers) are implemented
 *			                  Porting from Arduino C to PINGUINO32 C . 
 *                      Added features for support multiple serial port
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License	This software is released under the terms of the GPL version 3.0
 * 			   Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 * 			   Original author bester.j...@gmail.com
 *                         Original project http://code.google.com/p/simple-modbus/
 *
 *         05/12/2013 	Ver. 1 Rev. 0 First release --- Moreno Manzini ( moreno at mediacom dot it )
*/


/*
 SimpleModbusSlave allows you to communicate
 to any slave using the Modbus RTU protocol.
 
 This implementation DOES NOT fully comply with the Modbus specifications.
 
 Specifically the frame time out have not been implemented according
 to Modbus standards. The code does however combine the check for
 inter character time out and frame time out by incorporating a maximum
 time out allowable when reading from the message stream.
 
 SimpleModbusSlave implements an unsigned int return value on a call to modbus_update().
 This value is the total error count since the slave started. It's useful for fault finding.
 
 This code is for a Modbus slave implementing functions 3 and 16
 function 3: Reads the binary contents of holding registers (4X references)
 function 16: Presets values into a sequence of holding registers (4X references)
 
 All the functions share the same register array.
 
 Note:  
 The Arduino serial ring buffer is 128 bytes or 64 registers.
 Most of the time you will connect the arduino to a master via serial
 using a MAX485 or similar.
 
 In a function 3 request the master will attempt to read from your
 slave and since 5 bytes is already used for ID, FUNCTION, NO OF BYTES
 and two BYTES CRC the master can only request 122 bytes or 61 registers.
 
 In a function 16 request the master will attempt to write to your 
 slave and since a 9 bytes is already used for ID, FUNCTION, ADDRESS, 
 NO OF REGISTERS, NO OF BYTES and two BYTES CRC the master can only write
 118 bytes or 59 registers.
 
 Using a USB to Serial converter the maximum bytes you can send is 
 limited to its internal buffer which differs between manufactures. 
 
 The functions included here have been derived from the 
 Modbus Specifications and Implementation Guides
 
 http://www.modbus.org/docs/Modbus_over_serial_line_V1_02.pdf
 http://www.modbus.org/docs/Modbus_Application_Protocol_V1_1b.pdf
 http://www.modbus.org/docs/PI_MBUS_300.pdf
*/



#ifndef __MODBUSS_C
#define __MODBUSS_C 

#include <macro.h>
#include <typedef.h> 
#include <delay.c> 
#include <digitalw.c> 

#define MODBUSS_BUFFER_SIZE 128

#define MODBUSS_SER1	1
#define MODBUSS_SER2	2
#define MODBUSS_SER3	3


typedef struct 
 {
  unsigned char port;
  // frame[] is used to recieve and transmit packages. 
  unsigned char frame[MODBUSS_BUFFER_SIZE];
  u16 holdingRegsSize; // size of the register array
  u16* regs; // user array address
  unsigned char broadcastFlag;
  unsigned char slaveID;
  unsigned char function;
  unsigned char TxEnablePin;
  u16 errorCount;
  u16 T1_5; // inter character time out
  u16 T3_5; // frame delay
 } MODBUSS_DATA;


#ifdef MODBUSS_SER1_OK
 #include <pinguinoserial1.c> 
#endif
#ifdef MODBUSS_SER2_OK
 #include <pinguinoserial2.c> 
#endif
#ifdef MODBUSS_SER3_OK
 //#include <pinguinoserial3.c> 
#endif




// function definitions
void modbuss_exceptionResponse(MODBUSS_DATA * ModBusS_Data,unsigned char exception);
u16 modbuss_calculateCRC(MODBUSS_DATA * ModBusS_Data,unsigned char bufferSize); 
void modbuss_sendPacket(MODBUSS_DATA * ModBusS_Data,unsigned char bufferSize);



BOOL modbuss_SerialClearRxError(MODBUSS_DATA * ModBusS_Data)
{
 switch (ModBusS_Data->port)
  {
#ifdef MODBUSS_SER1_OK
   case MODBUSS_SER1:
    return(serial1clearrxerror());
    break;
#endif
#ifdef MODBUSS_SER2_OK
   case MODBUSS_SER2:
    return(serial2clearrxerror());
    break;
#endif
#ifdef MODBUSS_SER3_OK
   case MODBUSS_SER3:
    return(serial3clearrxerror());
    break;
#endif
   default:
    return(FALSE);
    break;
  }
}


char modbuss_SerialAvailable(MODBUSS_DATA * ModBusS_Data)
{
 switch (ModBusS_Data->port)
  {
#ifdef MODBUSS_SER1_OK
   case MODBUSS_SER1:
    return(serial1available());
    break;
#endif
#ifdef MODBUSS_SER2_OK
   case MODBUSS_SER2:
    return(serial2available());
    break;
#endif
#ifdef MODBUSS_SER3_OK
   case MODBUSS_SER3:
    return(serial3available());
    break;
#endif
   default:
    return(0);
    break;
  }
}


char modbuss_SerialRead(MODBUSS_DATA * ModBusS_Data)
{
 switch (ModBusS_Data->port)
  {
#ifdef MODBUSS_SER1_OK
   case MODBUSS_SER1:
    return(serial1read());
    break;
#endif
#ifdef MODBUSS_SER2_OK
   case MODBUSS_SER2:
    return(serial2read());
    break;
#endif
#ifdef MODBUSS_SER3_OK
   case MODBUSS_SER3:
    return(serial3read());
    break;
#endif
   default:
    return(0);
    break;
  }
}


int modbuss_SerialBegin(MODBUSS_DATA * ModBusS_Data, u32 speed)
{
 switch (ModBusS_Data->port)
  {
#ifdef MODBUSS_SER1_OK
   case MODBUSS_SER1:
    serial1init(speed);
    return (1);
    break;
#endif
#ifdef MODBUSS_SER2_OK
   case MODBUSS_SER2:
    serial2init(speed);
    return (1);
    break;
#endif
#ifdef MODBUSS_SER3_OK
   case MODBUSS_SER3:
    serial3init(speed);
    return (1);
    break;
#endif
   default:
    return (-1);
    break;
  }
}


void modbuss_SerialWrite(MODBUSS_DATA * ModBusS_Data,char c)
{
 switch (ModBusS_Data->port)
  {
#ifdef MODBUSS_SER1_OK
   case MODBUSS_SER1:
    serial1write(c);
    break;
#endif
#ifdef MODBUSS_SER2_OK
   case MODBUSS_SER2:
    serial2write(c);
    break;
#endif
#ifdef MODBUSS_SER3_OK
   case MODBUSS_SER3:
    serial3write(c);
    break;
#endif
   default:
    break;
  }
}


void modbuss_SerialFlush(MODBUSS_DATA * ModBusS_Data)
{
 switch (ModBusS_Data->port)
  {
#ifdef MODBUSS_SER1_OK
   case MODBUSS_SER1:
    serial1flush();
    break;
#endif
#ifdef MODBUSS_SER2_OK
   case MODBUSS_SER2:
    serial2flush();
    break;
#endif
#ifdef MODBUSS_SER3_OK
   case MODBUSS_SER3:
    serial3flush();
    break;
#endif
   default:
    break;
  }
}



u16 modbuss_update(MODBUSS_DATA * ModBusS_Data)
{
 char caract;
 unsigned char buffer;
 unsigned char overflow;
 unsigned char id;
 u16 crc;
 u16 startingAddress;
 u16 no_of_registers;
 u16 maxData;
 unsigned char index;
 unsigned char address;
 u16 crc16;
 unsigned char noOfBytes;
 unsigned char responseFrameSize;
 u16 temp;

 if (modbuss_SerialClearRxError(ModBusS_Data) == FALSE)
   return;  

 if (modbuss_SerialAvailable(ModBusS_Data))
  {
   buffer = 0;
   overflow = 0;
	
   while (modbuss_SerialAvailable(ModBusS_Data))
    {
     // The maximum number of bytes is limited to the serial buffer size of 128 bytes
     // If more bytes is received than the MODBUSS_BUFFER_SIZE the overflow flag will be set and the 
     // serial buffer will be red untill all the data is cleared from the receive buffer.
     if (overflow) 
      modbuss_SerialRead(ModBusS_Data);
     else
      {
       caract = modbuss_SerialRead(ModBusS_Data);
       if (buffer == MODBUSS_BUFFER_SIZE)
        overflow = 1;
       else
        {
         ModBusS_Data->frame[buffer] = caract;
         buffer++;
        }
      }
     Delayus(ModBusS_Data->T1_5); // inter character time out
    }
	
   // If an overflow occurred increment the errorCount
   // variable and return to the main sketch without 
   // responding to the request i.e. force a timeout
   if (overflow)
    return ModBusS_Data->errorCount++;
	
   // The minimum request packet is 8 bytes for function 3 & 16
   if (buffer > 7) 
    {
     id = ModBusS_Data->frame[0];
     ModBusS_Data->broadcastFlag = 0;
     if (id == 0)
      ModBusS_Data->broadcastFlag = 1;
		
     if (id == ModBusS_Data->slaveID || ModBusS_Data->broadcastFlag) // if the recieved ID matches the ModBusS_Data->slaveID or broadcasting id (0), continue
      {
       crc = ((ModBusS_Data->frame[buffer - 2] << 8) | ModBusS_Data->frame[buffer - 1]); // combine the crc Low & High bytes
       if (modbuss_calculateCRC(ModBusS_Data,buffer - 2) == crc) // if the calculated crc matches the recieved crc continue
        {
         ModBusS_Data->function = ModBusS_Data->frame[1];
         startingAddress = ((ModBusS_Data->frame[2] << 8) | ModBusS_Data->frame[3]); // combine the starting address bytes
         no_of_registers = ((ModBusS_Data->frame[4] << 8) | ModBusS_Data->frame[5]); // combine the number of register bytes	
         maxData = startingAddress + no_of_registers;
				
         // broadcasting is not supported for function 3 
         if (!ModBusS_Data->broadcastFlag && (ModBusS_Data->function == 3))
          {
           if (startingAddress < ModBusS_Data->holdingRegsSize) // check exception 2 ILLEGAL DATA ADDRESS
            {
             if (maxData <= ModBusS_Data->holdingRegsSize) // check exception 3 ILLEGAL DATA VALUE
              {
               noOfBytes = no_of_registers * 2; 
               // ID, function, noOfBytes, (dataLo + dataHi)*number of registers,
               //  crcLo, crcHi
               responseFrameSize = 5 + noOfBytes; 
               ModBusS_Data->frame[0] = ModBusS_Data->slaveID;
               ModBusS_Data->frame[1] = ModBusS_Data->function;
               ModBusS_Data->frame[2] = noOfBytes;
               address = 3; // PDU starts at the 4th byte
							
               for (index = startingAddress; index < maxData; index++)
                {
                 temp = ModBusS_Data->regs[index];
                 ModBusS_Data->frame[address] = temp >> 8; // split the register into 2 bytes
                 address++;
                 ModBusS_Data->frame[address] = temp & 0xFF;
                 address++;
                }	
							
               crc16 = modbuss_calculateCRC(ModBusS_Data,responseFrameSize - 2);
               ModBusS_Data->frame[responseFrameSize - 2] = crc16 >> 8; // split crc into 2 bytes
               ModBusS_Data->frame[responseFrameSize - 1] = crc16 & 0xFF;
               modbuss_sendPacket(ModBusS_Data,responseFrameSize);
              }
             else	
              modbuss_exceptionResponse(ModBusS_Data,3); // exception 3 ILLEGAL DATA VALUE
            }
           else
            modbuss_exceptionResponse(ModBusS_Data,2); // exception 2 ILLEGAL DATA ADDRESS
          }
         else if (ModBusS_Data->function == 16)
          {
           // Check if the recieved number of bytes matches the calculated bytes 
           // minus the request bytes.
  	   // id + function + (2 * address bytes) + (2 * no of register bytes) + 
           // byte count + (2 * CRC bytes) = 9 bytes
           if (ModBusS_Data->frame[6] == (buffer - 9)) 
            {
             if (startingAddress < ModBusS_Data->holdingRegsSize) // check exception 2 ILLEGAL DATA ADDRESS
              {
               if (maxData <= ModBusS_Data->holdingRegsSize) // check exception 3 ILLEGAL DATA VALUE
                {
                 address = 7; // start at the 8th byte in the ModBusS_Data->frame
                 for (index = startingAddress; index < maxData; index++)
                  {
                   ModBusS_Data->regs[index] = ((ModBusS_Data->frame[address] << 8) | ModBusS_Data->frame[address + 1]);
                   address += 2;
                  }	
                 // only the first 6 bytes are used for CRC calculation
                 crc16 = modbuss_calculateCRC(ModBusS_Data,6); 
                 ModBusS_Data->frame[6] = crc16 >> 8; // split crc into 2 bytes
                 ModBusS_Data->frame[7] = crc16 & 0xFF;
								
                 // a function 16 response is an echo of the first 6 bytes from 
                 // the request + 2 crc bytes
                 if (!ModBusS_Data->broadcastFlag) // don't respond if it's a broadcast message
                 modbuss_sendPacket(ModBusS_Data,8); 
                }
               else	
                modbuss_exceptionResponse(ModBusS_Data,3); // exception 3 ILLEGAL DATA VALUE
              }
             else
              modbuss_exceptionResponse(ModBusS_Data,2); // exception 2 ILLEGAL DATA ADDRESS
            }
           else 
            ModBusS_Data->errorCount++; // corrupted packet
          }					
         else
          modbuss_exceptionResponse(ModBusS_Data,1); // exception 1 ILLEGAL FUNCTION
        }
       else // checksum failed
        ModBusS_Data->errorCount++;
      } // incorrect id
    }
   else
    if (buffer > 0 && buffer < 8)
     ModBusS_Data->errorCount++; // corrupted packet
  }
 return ModBusS_Data->errorCount;
}	


void modbuss_exceptionResponse(MODBUSS_DATA * ModBusS_Data,unsigned char exception)
{
 u16 crc16;
 // each call to modbuss_exceptionResponse() will increment the errorCount
 ModBusS_Data->errorCount++; 
 if (!ModBusS_Data->broadcastFlag) // don't respond if its a broadcast message
  {
   ModBusS_Data->frame[0] = ModBusS_Data->slaveID;
   ModBusS_Data->frame[1] = (ModBusS_Data->function | 0x80); // set MSB bit high, informs the master of an exception
   ModBusS_Data->frame[2] = exception;
   crc16 = modbuss_calculateCRC(ModBusS_Data,3); // ID, function|0x80, exception code
   ModBusS_Data->frame[3] = crc16 >> 8;
   ModBusS_Data->frame[4] = crc16 & 0xFF;
   // exception response is always 5 bytes 
   // ID, function + 0x80, exception code, 2 bytes crc
   modbuss_sendPacket(ModBusS_Data,5); 
  }
}


void modbuss_configure(MODBUSS_DATA * ModBusS_Data,char port,long baud, unsigned char _slaveID, 
                      unsigned char _TxEnablePin, u16 _holdingRegsSize,
                      u16* _regs)
{
 ModBusS_Data->port = port;
 ModBusS_Data->slaveID = _slaveID;
 modbuss_SerialBegin(ModBusS_Data,baud);
 ModBusS_Data->holdingRegsSize = _holdingRegsSize; 
 ModBusS_Data->regs = _regs;
 ModBusS_Data->TxEnablePin = _TxEnablePin; 
 if (ModBusS_Data->TxEnablePin > 0)
  {
   pinmode(ModBusS_Data->TxEnablePin, OUTPUT);
   digitalwrite(ModBusS_Data->TxEnablePin, LOW);
  }
 ModBusS_Data->errorCount = 0; // initialize errorCount
	
 // Modbus states that a baud rate higher than 19200 must use a fixed 750 us 
 // for inter character time out and 1.75 ms for a ModBusS_Data->frame delay for baud rates
 // below 19200 the timing is more critical and has to be calculated.
 // E.g. 9600 baud in a 10 bit packet is 960 characters per second
 // In milliseconds this will be 960characters per 1000ms. So for 1 character
 // 1000ms/960characters is 1.04167ms per character and finally modbus states
 // an inter-character must be 1.5T or 1.5 times longer than a character. Thus
 // 1.5T = 1.04167ms * 1.5 = 1.5625ms. A ModBusS_Data->frame delay is 3.5T.
	
 if (baud > 19200)
  {
   ModBusS_Data->T1_5 = 750; 
   ModBusS_Data->T3_5 = 1750; 
  }
 else 
  {
   ModBusS_Data->T1_5 = 15000000/baud; // 1T * 1.5 = T1.5
   ModBusS_Data->T3_5 = 35000000/baud; // 1T * 3.5 = T3.5
  }
}   

u16 modbuss_calculateCRC(MODBUSS_DATA * ModBusS_Data,unsigned char bufferSize) 
{
 u16 temp, temp2, flag;
 unsigned char i;
 unsigned char j;
 temp = 0xFFFF;
 for (i = 0; i < bufferSize; i++)
  {
   temp = temp ^ ModBusS_Data->frame[i];
   for (j = 1; j <= 8; j++)
    {
     flag = temp & 0x0001;
     temp >>= 1;
     if (flag)
      temp ^= 0xA001;
    }
  }
 // Reverse byte order. 
 temp2 = temp >> 8;
 temp = (temp << 8) | temp2;
 temp &= 0xFFFF; 
 // the returned value is already swapped
 // crcLo byte is first & crcHi byte is last
 return temp; 
}

void modbuss_sendPacket(MODBUSS_DATA * ModBusS_Data,unsigned char bufferSize)
{
 unsigned char i;
 if (ModBusS_Data->TxEnablePin > 0)
  digitalwrite(ModBusS_Data->TxEnablePin, HIGH);
		
 for (i = 0; i < bufferSize; i++)
 modbuss_SerialWrite(ModBusS_Data,ModBusS_Data->frame[i]);
		
 modbuss_SerialFlush(ModBusS_Data);
	
 // allow a frame delay to indicate end of transmission
 Delayus(ModBusS_Data->T3_5); 
	
 if (ModBusS_Data->TxEnablePin > 0)
  digitalwrite(ModBusS_Data->TxEnablePin, LOW);
}


#endif
