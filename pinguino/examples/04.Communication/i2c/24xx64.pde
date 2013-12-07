/*
    I2C communication with a 24C64 eeprom memory
    
    A control byte is the first byte received following the
    start condition from the master device.
    The control byte consists of a four bit control code;
    for the 24xx64 this is set as 1010 binary for read and write
    operations.
    The next three bits of the control byte are
    the chip select bits (A2, A1, A0). The chip select bits
    allow the use of up to eight 24xx64 devices on the
    same bus and are used to select which device is
    accessed. The chip select bits in the control byte must
    correspond to the logic levels on the corresponding A2,
    A1, and A0 pins for the device to respond. These bits
    are in effect the three most significant bits of the word
    address.

    Address = 1 0 1 0 A2 A1 A0

                               __
    (VSS or left floating) A0-|° |-VCC (+1.8 to 5.5V)
    (VSS or left floating) A1-|  |-WP  (VSS or left floating if not write protected)
    (VSS or left floating) A2-|  |-SCL (to Pinguino SCL)
                 (Ground) VSS-|__|-SDA (to Pinguino SDA)

    In that case, address = 1 0 1 0 0 0 0 = 0x50

    NB : SDA and SCL require a pullup resistor to VCC
    (typical 10 kΩ for 100 kHz, 2 kΩ for 400 kHz)
    
    Pinguino    x550    x6j50
    SDA         DO      D5
    SCL         D1      D4
    
    The last bit of the control byte defines the operation to
    be performed. When set to a one a read operation is
    selected, and when set to a zero a write operation is
    selected. The next two bytes received define the
    address of the first data byte (Figure 5-2). Because
    only A2...A0 are used, the upper three address bits
    are don’t care bits. The upper address bits are trans-
    ferred first, followed by the less significant bits.

    Output (linux):
    - sudo minicom -o -D /dev/ttyACM0
    - sudo cat /dev/ttyACM0
*/

u8  i2cAddr=0x50;       // 0b1010000, i2c address of eeprom (see above)
u16 MemoryAddr=0x0000;  // Memory address to write or read
    
// Bytes to write in the device's memory
u8 wBuffer[8]={'P','I','N','G','U','I','N','O'};

void setup()
{
    u8 i;
    
    delay(1000);
    CDC.printf("\n\r\n\r*** 24xx64 i2c Eeprom Demo. ***\n\r\n\r");
    pinMode(12, OUTPUT);                // Pinguino 26j50 build-in led is on pin 12
    // Define Pinguino as Master and Clock speed on i2c bus
    I2C.master(I2C_100KHZ);             // or I2C.master(100);
    //I2C.master(I2C_400KHZ);             // or I2C.master(400);

    ///
    ///     Write
    ///

    CDC.printf("Writing ...\n\r");
    I2C.start();                                // All I2C commands must begin with a Start condition
    I2C.write((i2cAddr << 1) & 0xFE);            // write operation (bit 0 set to 0)
    I2C.write(highByte(MemoryAddr));             // MSB first
    I2C.write(lowByte(MemoryAddr));              // LSB second
    for (i=0; i<8; i++)
    {
        I2C.send(wBuffer[i]);                   // Page write (32 bytes max, auto. inc.)
        CDC.write(wBuffer[i]);
    }
    CDC.printf("\n\r");
    I2C.stop();                                 // Terminate the write sequence
}
 
void loop()
{
    u8 i, c;

    
    ///
    ///     Read
    ///
    
    digitalWrite(12, HIGH);                     // Switch build-in led on
    CDC.printf("Reading ...\n\r");

    // Send Memory Address

    I2C.start();                                // All I2C commands must begin with a Start condition
    I2C.write((i2cAddr << 1) & 0xFE);            // write operation (bit 0 set to 0)
    I2C.write(highByte(MemoryAddr));             // MSB first
    I2C.write(lowByte(MemoryAddr));              // LSB second
    
    // Starts reading
    
    I2C.start();                                // start again
    I2C.write((i2cAddr << 1) + 1);               // read operation (bit 0 set to 1)
    for (i=0; i<8; i++)                         // Sequential read (auto. inc.)
    {
        c = I2C.read();                         // read a byte from the slave
        CDC.write(c);
        if (i>=7)                               // Last byte is sent ?
            I2C.sendNack();                     // Yes, send a No Ack to the slave (no more data to read)
        else
            I2C.sendAck();                      // No, send a Ack bit (more data has to be read)
    }
    CDC.printf("\n\r");
    I2C.stop();                                 // Terminate the read sequence

    digitalWrite(12, LOW);                      // Switch build-in led off
    delay(1000);                                // Wait for 1 sec. (1000 ms)
}
