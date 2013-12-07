// Output on Linux : sudo cat /dev/ttyACM0
// Outout on Windows : 

float f=0; 

void setup()
{
    pinMode(USERLED, OUTPUT);
}

void loop()
{
    CDC.printf("float=%.1f \r\n", f);  // 1 digit after decimal comma
    f = f + 0.1;
    toggle(USERLED);
    delay(500);
}
