// Simple serial test
// Press a key and see its ASCII code

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    char c;

    Serial.println("TEST");
    if (Serial.available())
    {
        c = Serial.read();
        
        // with print and printNumber
        Serial.print("You pressed key [");
        Serial.print(&c);
        Serial.print("], Code ASCII = ");
        Serial.printNumber(c, DEC);
        Serial.print("\r\n");

        // with printf
        //Serial.printf("You pressed key [%s], Code ASCII = %d \r\n", &c, c);
        
        delay(1000);
    }
    delay(100);
}
