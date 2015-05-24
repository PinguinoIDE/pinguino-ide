/*
	Pinguino MX220 and MOD-BT example
	Receive and recognise characters
	
	To be used with Pinguino Rev.305 and above

*/


/********** Libraries ***************/

	/*
		If you want to save additional memory,
		comment out the inclusion of the bluetooth_mx220.c library
		and use Serial.printf to send commands instead (You can
		find the correct strings in the bluetooth_mx220.c library).
		To ensure that a commaand has been recognised you should
		check for the "OK" that the MOD-BT module retuns.
		Refer to the BT_send() and BT_ok() functions in the 
		bluetooth_mx220.c library for more information
	*/
	#include <bt/bluetooth_mx220.c>


/********** Macros & Constants ******/

	//Definitions for PIC32-PINGUINO-MX220
	//For more info, refer to the schematic at 
	//http://olimex.com/dev/pic32-pinguino-mx220.html
	
	#define btp UART1      //The UEXT is on UART1
	#define btbaud 115200
	#define RLED 9
	#define GLED 13
	#define BUTTON1 8
	
	//RED LED is A10
	#define RLEDINIT   TRISACLR = 0x400;
	#define RLED1      PORTASET = 0x400;
	#define RLED0      PORTACLR = 0x400;
	#define RLEDSWITCH PORTA   ^= 0x400;
	
	//GREEN LED is B15
	#define GLEDINIT  	TRISBCLR = 0x8000;
	#define GLED1 		PORTBSET = 0x8000;
	#define GLED0 		PORTBCLR = 0x8000;
	#define GLEDSWITCH  PORTB   ^= 0x8000;
	
	//A macro that waits for a key via CDC
	//Works with all pinguinos that support CDC
	//Syntax:
	//   WAIT;   //-> pauses the program and waits
				 //   until a key is pressed 
		
	/* WAIT */
	#define WAIT wait_123()
	char ch123;
	
	void wait_123() __attribute ((noinline));
	
	void wait_123()
	{
	  ch123 = CDC.getKey();
	}
	/* END Wait */



/********** Function Prototypes *********/
	
	//sends a message via CDC
	//the ((noinline)) makes the compiled
	//code smaller (see the README file)
	void message(char * m) __attribute((noinline));
	
	void message(char * m)
	   { CDC.println(m);}

	void local_init_bt(unsigned char uart_port, uint32_t baud_rate);


/********** Main Functions  *********/
	void setup()
	{
		
		RLEDINIT;
		GLEDINIT;
		RLED0;
		GLED0;
		
		WAIT;
		message("Press a key to init MOD-BT:");
		WAIT;

		local_init_bt(btp, btbaud);
		
		message("Device is in data mode.");
		
		//Clear the buffer of the Serial Communication
		SerialFlush(btp);
		
		}
		
		
	void loop()
	{
		//reads every received character from the BT module
		//Also, the characters '1' and '2' toggle the two LEDS on the board
		
		char comm;
		
		//waits until a character is received and stores it
		comm = SerialGetKey(btp);
		
		//Uncomment the next row if you want the board to echo characters on the terminal
		CDC.printf("%c", comm);
		message("");
		
		// A simple switch that performs different commands
		// You can make this into a function for more
		// sophisticated examples
		
		switch(comm)
		{
			case '1': 
				RLEDSWITCH;
				break;
			case '2':
				GLEDSWITCH;
				break;
			default:
				message("\r\n Unknown command");
				
		}	

	}
		
/********** Function bodies *********/

	void local_init_bt(unsigned char uart_port, uint32_t baud_rate)
	{
		//configure the board to open a serial connection to the MOD-BT module (via UEXT)
		SerialConfigure(uart_port, UART_ENABLE, UART_RX_TX_ENABLED,	baud_rate);
		
		//Allow other devices to auto-connect to MOD-BT
		BT_setAutoConnection(uart_port);
		
		//Start the server
		BT_start(uart_port);
	
	}



