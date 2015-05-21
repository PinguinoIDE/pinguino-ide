/*****************************************
    Go to Deep Sleep for 1 second
    Toggle built-in led at wake-up
*****************************************/

void setup()
{
    /* ----- Internal oscillator init (optional) ----- */

    //System.setIntOsc(_48MHZ_);
    
    /* ----- First start init ----- */
  
    if ( !System.deepSleepWakeUp() )
    { 
        RTCC.setTime(0x00000000);
        RTCC.setAlarmTime(0x00000000);
    }
  
    /* ----- RTCC Alarm init ----- */
    
    RTCC.alarmEverySecond();            // allow alarm at every second
    RTCC.chimeEnable();                 // allow indefinite alarm repetition
    RTCC.alarmEnable();                 // enable alarm
    RTCC.alarmIntEnable();              // enable alarm interrupt to wake-up from deep sleep mode
    RTCC.enable();                      // enable RTCC module

    /* ----- Toggle built-in led ----- */
    
    pinMode(USERLED, OUTPUT);
    toggle(USERLED);

    /* ----- Deep sleep ----- */

    // Go to deep sleep until RTCC alarm occurs.
    // If RTCC alarm occurs do a Power-on Reset.
    // The program will then restart from the beginning (vector 0x00).
    
    System.deepSleep();
}

void loop()
{
}
