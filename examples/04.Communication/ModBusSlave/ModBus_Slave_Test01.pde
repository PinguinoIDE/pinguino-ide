/*
 * File			ModBus_Slave_Test01.c
 *
 *         Description		Test program for MODBUS Slave protocol implentation,
 *                      only command 3 (Read Holding Registers)
 *                      and 16 (Write Multiple Registers) are implemented
 *                      Porting from Arduino C to PINGUINO32 C . 
 *                      Added features for support multiple serial port
 *
 * NOTE    At the moment this library is available only for 32bit PIC device
 *
 * Author		Moreno Manzini
 *
 * Version		1.0
 *
 * License	This software is released under the terms of the GPL version 3.0
 * 			   Full details of licensing terms can be found in the "LICENSE" file, distributed with this code
 *
 * 			   Original author bester.j...@gmail.com
 *         Original project http://code.google.com/p/simple-modbus/
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

 
//PIC32-PINGUINO-MICRO GREEN LED
#define ledPinG			    32 // the number of the Green LED pin

//TX Enable
#define TXEN    		    30 //TX ENABLE


//Define a data area of 32 word
typedef struct 
 {
  u16 WriteReg[16];
  u16 ReadReg[16];
 } REGISTER;

REGISTER Register;


//Define here the serial port you want use, one or more can be used concurrently
#define MODBUSS_SER1_OK	0
//#define MODBUSS_SER2_OK	0
//#define MODBUSS_SER3_OK	0

//Define port area, one for each serial port used
MODBUSS_DATA ModBusS_Data_01;
  



void setup()
{               
 System.setCpuFrequency(80000000); // should be 80 MHz
 
 //set pin modes 
 pinMode(ledPinG, OUTPUT); 
 digitalWrite(ledPinG,LOW); 

 //Confgure RS485 serial port 1 at 155200baud using TXEN pin, SlaveID 1, 32 WORD data on struct Register
 ModBusSlave.Configure(&ModBusS_Data_01,MODBUSS_SER1,155200, 1, 
                      TXEN, sizeof(Register)/2,
                      (unsigned int *)&Register);
}


int Delay;
void loop()
{
 int Ctn;
 
 //Update status of MODBUS Slave Serial port
 ModBusSlave.Update(&ModBusS_Data_01);
 Delayus(500); 
 Delay++;
 //Blink green led every 300mS
 if (Delay > (300*2))
  {
   Delay = 0; 
   for (Ctn = 0 ; Ctn < 16 ; Ctn++)
    Register.ReadReg[Ctn] = Register.WriteReg[Ctn]+1;
   //Toggle Green LED
   toggle(ledPinG);
  } 
}
