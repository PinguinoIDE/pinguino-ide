/**
 * UART3 pinguino test
 * For use with PIC32MX795 boards or better!
 * Marcus Fazzi (GPLv2) 2011
*/

#define ENABLE_UART3
#define _UBAUD_ 9600
#define _UPORT_ UART3

void setup(){
	Serial.Configure(_UPORT_, UART_ENABLE, UART_RX_TX_ENABLED, _UBAUD_);
	
	pinMode(REDLED, OUTPUT);
	pinMode(BLUELED, OUTPUT);
}

void loop(){
	char c;
	
	digitalWrite(BLUELED, LOW);
	delay(25);
	
	if(Serial.Available(_UPORT_)){
		
		c = Serial.Read(_UPORT_);
	
		Serial.Printf(_UPORT_,"%c", c);

		digitalWrite(REDLED, HIGH);
		delay(50);
	}
	else{
		digitalWrite(REDLED, LOW);
	}
	
	digitalWrite(BLUELED, HIGH);
	delay(25);
}
