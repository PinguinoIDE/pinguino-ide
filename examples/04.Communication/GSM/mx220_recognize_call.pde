/*
	
	Pinguino-MX220 + MOD-GSM example
	React to a call from a specific number
	This example works without an additional library
	
	You can find all the schematics for OLIMEX
	Pinguino boards at http://www.olimex.com/dev
	in the DUINO section.		
	
	If you have any questions write to
	support@olimex.com
	
	OLIMEX, April 2012
	http://www.olimex.com/dev
	
	(!!) Replace the "xxx..." in the sketch with your
		  phone's number or it won't work 
	(!!)
	
*/

/* ****************************************** */

	#include <serial.c>	//The Serial Functions for pinguino
	
	//Definitions for PIC32-PINGUINO-MX220
	//For more info, refer to the schematic at 
	//http://olimex.com/dev/pic32-pinguino-mx220.html
	
	#define gsmport UART1      //The UEXT is on UART1
	#define RLED 9
	#define GLED 13
	#define BUTTON1 8
	#define BUTTON1INIT TRISBbits.TRISB7 = 1
	
	//RED LED is A10
	#define RLEDINIT   TRISACLR = 0x400;
	#define RLED1      PORTASET = 0x400;
	#define RLED0      PORTACLR = 0x400;
	#define RLEDSWITCH PORTA   ^= 0x400;
	
	//GREEN LED is B15
	#define GLEDINIT  	TRISBCLR = 0x8000;
	#define GLED1 			PORTBSET = 0x8000;
	#define GLED0 			PORTBCLR = 0x8000;
	#define GLEDSWITCH   PORTB   ^= 0x8000;
	
	#define RED   0
	#define GREEN 1
	#define BOTH  2

	//Buffer for the strings the board receives from the GSM module
	char buffer[80];
	
	// (!!) Write your phone number here (!!)
	// (!!) Write it without the '+', but with
	//		your country's code
	//Note: The lenght of the phone number doesn't matter,
	//      but the format has to be exactly what a phone
	//		  shows if an unknown number is calling (without the '+')
	
	char * Caller_ID = "xxxxxxxxxxxx";

/* ****************************************** */

	//Catches a string and writes to the buffer
	//The module sends strings in the format <CR><LF><string><CR><LF>
	void GSM_get_response( unsigned char uport) __attribute((noinline));
	
	//Blinks an LED five times - mode is RED, GREEN or BOTH
	void blinkled(unsigned char mode) __attribute((noinline));
	
	//Sends a string to the module until an "OK" acknowledgement is received
	void GSM_send(unsigned char uport, char * fmt, ...) __attribute((noinline));
	
	
	// Interrupt routine to check if the button has been pressed
	// Connect the wrapper to vector 4 (Timer1 overflow)
	void ISR_wrapper_vector_4(void) __attribute__ ((section (".vector_4")));

	// The wrapper calls the "Tmr1Interrupt" function
	// each time an interrupt occurs
	void ISR_wrapper_vector_4(void) { Tmr1Interrupt(); }

	//We declare the function as an interrupt routine
	void Tmr1Interrupt(void) __attribute__ ((interrupt));


	//Switch for the interrupt
	volatile unsigned char sw = 1;
	
	// This code will be executed each time an interrupt occurs
	void Tmr1Interrupt(void)
	{
			T1CONCLR = (1<<15);  //Strop Timer 1
			
			//Check if the button has been pressed
			if( !PORTBbits.RB7 )
			{
				RLED1;
				GLED1;
				
				//(!!) Write the number you want to call here (!!)
				//(!!) No need for the country code
				GSM_send(gsmport, "ATDxxxxxxxxx;\r\n");
				GSM_get_response(gsmport);
				//GSM_send(gsmport, "ATH\r\n");
				SerialFlush(gsmport);
				RLED0;
				GLED0;
			}
		
		init_timer1();
	}
	
	// configure timer 1
	void init_timer1(void)
	{
		IntConfigureSystem(INT_SYSTEM_CONFIG_MULT_VECTOR);	// interrupt mode (interrupt.c)
		T1CON=0;			// reset timer 1 configuration
		TMR1=0;				// reset timer 1 counter register
		PR1=0x9896;			// define the period; combined with the prescaler equals ~ 4 interrupts for 1s
		IPC1SET=0x7;		// select interrupt priority and sub-priority
		IFS0CLR=0x10;		// clear interrupt flag
		IEC0SET=0x10;		// enable timer 1 interrupt
		T1CONSET=0x30;	  	// set prescaler to 256
		T1CONSET=0x8000; 	// turn TIMER1 ON
	}
	
	
/* ****************************************** */
	
	void setup()
	{
		/* Init the Module, LEDs and Button */
		RLEDINIT;
		GLEDINIT;	
		RLED0;
		GLED0;
		BUTTON1INIT;
		
		// Open a serial communication from the board to the module
		SerialConfigure(gsmport, UART_ENABLE, UART_RX_TX_ENABLED, 115200);
		
		//Turn Echo OFF
		SerialPrintf(gsmport, "ATE0\r\n");
		SerialPrintf(gsmport, "ATE0\r\n");
		SerialPrintf(gsmport, "ATE0\r\n");
		delay(1000);
		
		//Indicate the init
		blinkled(GREEN);
		
		//Clear the Srial Buffer
		SerialFlush(gsmport);
		
		//Option that shows extended Caller ID info
		GSM_send(gsmport, "AT+CLIP=1\r\n");
		
		//Indicate the Caller ID Init
		blinkled(GREEN);
		
		init_timer1();
		
	}

	void loop()
	{	
		// Wait until the RING string is received
		   do{		  
				GSM_get_response(gsmport);
			  }while( buffer[0] != 'R');  
											
			RLED1;
			
		// Catch the string with extended Caller ID info
			do{
				GSM_get_response(gsmport);
			  }while( buffer[0] != '+');
			
			
		// Indicate that you've received the string with extended Caller ID	  
			GLED1;
			
		// Check if the right number is calling, by default YES
			boolean flag = true;
			
		// Counter, the phone number in the extended Caller ID string
		// begins from the 10th character (index 9) 
			char i1=9;
			
		// Check if the number calling is the number you've written above
			while(buffer[i1] != '\"')
			{
				if(buffer[i1] != Caller_ID[i1-9])
					{
						flag = false;
						break;
					}
				
				i1++;		
				
			}
			
			
		// Close the connection
			GSM_send(gsmport, "ATH\r\n");
			
		//React if it's the right number calling the board
			if(flag)
			{
				//Write the code for your reaction here
				for(i1=0; i1<5; i1++)
					blinkled(BOTH);						
			}

	}

	
/* ****************************************** */

	void GSM_get_response( unsigned char uport)
	{
		char i = 0;
		char c;
		
	// Skip the <CR><LF> characters at the beginning of the string
		c = SerialGetKey(gsmport);
		c = SerialGetKey(gsmport);	
	
	// The rest of the code is based on the 
	// SerialGetString function in the 
	// <serial.c> library
		do {
				c = SerialGetKey(gsmport);
				
			    // Stop when you meet the <CR> character
				if( c != '\r')
				{	
					buffer[i] = c;
					i++;
				}
						
			}while( c != '\r' );
		buffer[i] = '\0';
		
	 //Catch the <LF> character
		c = SerialGetKey(gsmport);
		
	}

	void blinkled(unsigned char mode)
	{		
				//Turn off all LEDs
				RLED0;
				GLED0;
				
				char i;
				
				// Blink
				switch(mode)
				{
					case RED:
						for(i=0; i<10;i++)
						{
							RLEDSWITCH;
							delay(100);
						}
						break;
					
					case GREEN:
						for(i=0; i<10;i++)
						{
							GLEDSWITCH;
							delay(100);
						}
						break;
					
					case BOTH:
						for(i=0; i<10;i++)
						{
							RLEDSWITCH;
							GLEDSWITCH;
							delay(100);
						}
						break;
				}	
						
					
	}
	
	void GSM_send(unsigned char uport, char * fmt, ...)
	{
		//Sends the string until an "OK" is received 
		do{
			SerialPrintf(uport, fmt);
			GSM_get_response(uport);
		  }while(buffer[0] != 'O');
	}

	
