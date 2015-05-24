// Servo library test with Pinguino
// Connect servo control lines to pinguino pins.
// Feed servos with +5 V:
//
//   +-----+
//   |servo|--------- PWM Servo control ---> to pinguino pin.
//   |     |--------- +5V
//   |  o  |--------- GND
//   |     | 
//   +-----+

u8 position=1;

void setup(void)
{
delay(500);
servo.attach(0);
servo.attach(1);
servo.attach(2);
servo.attach(3);
servo.attach(4);
servo.attach(5);
servo.attach(6);
servo.attach(7);
}

void loop(void)
{                        
servo.write(0,255-position);
servo.write(1,255-position);
servo.write(2,255-position);
servo.write(3,255-position);
servo.write(4,position);
servo.write(5,position);
servo.write(6,position);
servo.write(7,position);

delay(250);
position++;
}


