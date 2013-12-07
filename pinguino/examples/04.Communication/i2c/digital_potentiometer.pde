// I2C Digital Potentiometer
// by Nicholas Zambetti <http://www.zambetti.com>
// and Shawn Bonkowski <http://people.interaction-ivrea.it/s.bonkowski/>

// Demonstrates use of the Wire library
// Controls AD5171 digital potentiometer via I2C/TWI

// Created 31 March 2006

// This example code is in the public domain.

// This example code is in the public domain.


void setup()
{
  Wire.master(100); // join i2c bus, speed is 100KHz
}

byte val = 0;

void loop()
{
  Wire.beginTransmission();
  Wire.write((44 << 1) & 0xFE);// transmit to device #44 (0x2c)
                              // device address is specified in datasheet
  Wire.write(0x00);           // sends instruction byte  
  Wire.write(val);            // sends potentiometer value byte  
  Wire.endTransmission();     // stop transmitting

  val++;        // increment value
  if(val == 64) // if reached 64th position (max)
  {
    val = 0;    // start over from lowest value
  }
  delay(500);
}

