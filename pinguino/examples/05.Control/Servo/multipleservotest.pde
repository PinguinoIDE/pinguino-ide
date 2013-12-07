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
servo.attach(8);
servo.attach(9);
servo.attach(10);
servo.attach(11);
servo.attach(12);
servo.attach(13);
servo.attach(14);
servo.attach(15);
servo.attach(16);
servo.attach(17);
}

void loop(void)
{                        
servo.write(0,255-position);
servo.write(1,255-position);
servo.write(2,255-position);
servo.write(8,255-position);
servo.write(9,255-position);
servo.write(10,position);
servo.write(11,position);
servo.write(12,position);
servo.write(16,position);
servo.write(17,position);

if(position>100){
    servo.setMaximumPulse(3);
    servo.setMaximumPulse(4);
    servo.setMaximumPulse(5);
    servo.setMaximumPulse(6);
    servo.setMaximumPulse(7);
    servo.setMaximumPulse(13);
    servo.setMaximumPulse(14);
    servo.setMaximumPulse(15);
} else {
    servo.setMinimumPulse(3); 
    servo.setMinimumPulse(4); 
    servo.setMinimumPulse(5); 
    servo.setMinimumPulse(6); 
    servo.setMinimumPulse(7); 
    servo.setMinimumPulse(13);
    servo.setMinimumPulse(14);
    servo.setMinimumPulse(15);
}
delay(250);
position++;
}


