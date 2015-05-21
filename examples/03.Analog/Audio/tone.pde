/*
    Melody
    Plays a melody on a piezo or 8-ohm speaker
    connected on a PWM pin :
    CCP1 or CCP2 for almost all 8-bit boards except for xxj53 boards
    xxj53 boards : CCP4, 5, 6, 7, 8, 9 or 10
    PWM0 to PWM4 for almost all 32-bit boards
*/

#define LINEOUT CCP1 //PWM4

// Melody structure
typedef struct
{
    int freq;
    int rest;
} Note;

// Note = frequency, duration
// 4 = quarter note, 8 = eighth note, etc.
Note melody[] = {
    {NOTE_C4, 4},
    {NOTE_G3, 8},
    {NOTE_G3, 8},
    {NOTE_A3, 4},
    {NOTE_G3, 4},
    {0,       4},
    {NOTE_B3, 4},
    {NOTE_C4, 4}};

void setup()
{
    pinMode(USERLED, OUTPUT);
    Audio.init(TAPEQUALITY);
    //Audio.staccato();
    //Audio.legato();
}

void loop()
{
    int thisNote;	
    int pauseBetweenNotes;
    int noteDuration;
    
    digitalWrite(USERLED, HIGH);

    // iterate over the notes of the melody:
    for (thisNote = 0; thisNote < 8; thisNote++)
    {
        // to calculate the note duration, take 1/2 a second 
        // divided by the note rest.
        noteDuration = 500 / melody[thisNote].rest;

        Audio.tone(LINEOUT, melody[thisNote].freq, noteDuration);

        // to distinguish the notes, set a minimum time between them.
        // the note's duration + 30% seems to work well:
        pauseBetweenNotes = noteDuration +  noteDuration / 30;
        delay(pauseBetweenNotes);
    }
  
    // stop the tone playing:
    Audio.noTone(LINEOUT);
    digitalWrite(USERLED, LOW);
    delay(500);
}
