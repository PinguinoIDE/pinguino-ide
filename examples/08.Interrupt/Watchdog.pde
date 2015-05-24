/*	----------------------------------------------------------------------------
    blink led with help from interrupt library
    ----------------------------------------------------------------------------
    author:	Régis Blanchot
    --------------------------------------------------------------------------*/

void setup()
{
    // Switch to Internal Oscillator
    System.intOsc(_31KHZ_);
    // We want to use the Built-in led
    pinMode(USERLED, OUTPUT);
    // Enable Watchdog Timer
    // Watchdog is driven by the Internal Oscillator (8MHz)
    // Nominal period of Watchdog is 4ms
    // Watchdog postscaler is set to 
    // 1:32768 by config. bits (bootloader version <= 4.8)
    // 1:256 (bootloader version > 4.8)
    // Watchdog timer will overload after 32768*4ms = 135sec = 2.25min
    // or 256*4ms = 1024 ms = 1 sec. 
    System.watchdog();
}

void loop()
{
    System.clearWatchdog(); // clear watchdog timer
    // Enter Sleep Mode
    System.sleep();         // wait for watchdog timer overload
    // Back to Run Mode
    toggle(USERLED);          // toggle the led
}
