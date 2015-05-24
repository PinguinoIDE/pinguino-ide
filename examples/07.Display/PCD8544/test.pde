
#define PIN0    0
#define PIN1    1
#define PIN2    2
#define PIN3    3
#define PIN4    4
#define PIN5    5
#define PIN6    6                             
#define PIN7    7

void setup()
{
    output(USERLED);
    
    output(PIN0);
    output(PIN1);
    output(PIN2);
    output(PIN3);
    output(PIN4);
    output(PIN5);
    output(PIN6);
    output(PIN7);

    high(PIN0);
    high(PIN1);
    high(PIN2);
    high(PIN3);
    high(PIN4);
    high(PIN5);
    high(PIN6);
    high(PIN7);
}

void loop()
{
    // Show that Pinguino is still alive
    toggle(USERLED);
    delay(500);
}
