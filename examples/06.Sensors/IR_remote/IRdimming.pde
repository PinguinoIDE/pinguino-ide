// http://www.bajdi.com
// Dimming LED in 5 steps with IR remote control
 

 
 // pin 1 of IR receiver to Arduino digital pin 11
const int led =  11;
int fadeValue;
int lastCounter = 1;
int counter;
 
int RECV_PIN = 1; // create instance of 'irrecv'
 
decode_results results;
 
void setup()
 
{  
  pinMode(led, OUTPUT);
  IRremote.enableIRIn(RECV_PIN); // Start the receiver 
  IRremote.blink(true);
}
 
void loop()
{
  counter = lastCounter; 
  if (IRremote.decode(&results))
  {
    if (results.value == 0x10 || results.value == 0x810){// insert your decoded button hex number i used the volume +
      counter ++;
      }
       
    if (results.value == 0x11 || results.value == 0x811){ //insert your decoded button hex number i used the volume -
      counter --;
      }
 
 IRremote.resume(); // Receive the next value
  }
   
if (counter > 20){      //maximum for counter = 5
    counter = 20;
  }
   
  if (counter < 2){    //minimum for counter = 1
    counter = 1;
  }
 
  switch (counter){    //depending on the counter the fadevalue is sent to the led
   
  case 1:
  fadeValue = 00;
  break;
   
  case 2:
  fadeValue = 250;
  break;
   
  case 3:
  fadeValue = 500;
  break;
   
  case 4:
  fadeValue = 750;
  break;
   
  case 5:
  fadeValue = 1023;
  break;
   
  }
   
  analogWrite(led, fadeValue);    //set led with PWM value
   
  lastCounter = counter;          //reset counter
}