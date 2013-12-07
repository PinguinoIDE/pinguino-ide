#include <digitalw.c>
#include <millis.c>
#include <keypad.h>






//--------------------
// Consts and Defines
//--------------------
//const char NO_KEY = '\0';
//#define KEY_RELEASED NO_KEY

// this is a macro that converts 2D arrays to pointers:
//#define makeKeymap(x) ((char *)x)

//-------------------
// Library variables 
//-------------------
uchar keypad_rows;
uchar keypad_columns;

char *userKeymap;

unsigned char *rowPins;
unsigned char *columnPins;

KeypadState state;
char currentKey;
unsigned long lastUpdate;
unsigned int debounceTime;
unsigned int holdTime;


//-----------------
// Funtions
//-----------------


KeypadState Keypad_getState(){
	return state;
}

void Keypad_setDebounceTime(unsigned int debounce){
	debounceTime = debounce;
}
void Keypad_setHoldTime(unsigned int hold){
	holdTime = hold;
}

//void Keypad_addEventListener(void (*listener)(char)){
//	keypadEventListener = listener;
//}


//private function
void Keypad_transitionTo(KeypadState newState){
	if (state!=newState){
		state = newState;
//		if (keypadEventListener!=NULL){ 
//			keypadEventListener(currentKey);
//		}
	}
}

//private function
void Keypad_initializePins(){

	unsigned char c,r;

	for (r=0; r<keypad_rows; r++){
        for (c=0; c<keypad_columns; c++){
			pinmode(columnPins[c],OUTPUT);
			digitalwrite(columnPins[c],HIGH);
        }
		//configure row pin modes and states
		pinmode(rowPins[r],INPUT);
		digitalwrite(rowPins[r],HIGH);
    }
}

// Initialization of Allows custom keymap. pin configuration and keypad size
void Keypad_init(char *keypadmap, uchar *rowp, uchar *colp, uchar rows, uchar cols){

        userKeymap = keypadmap;
	rowPins = rowp;
	columnPins = colp;
	keypad_rows = rows;
	keypad_columns = cols;         

	lastUpdate = 0;
	debounceTime = 50;
	holdTime = 1000;
//	keypadEventListener = 0;
	currentKey = NO_KEY;
	state = IDLE;
	
	Keypad_initializePins();
}


// Returns the keykode of the pressed key, or NO_KEY if no key is pressed
char Keypad_getKey(){
    
    char key = NO_KEY; // Assume that no key is pressed, this is the default return for getKey()
    unsigned char c,r;    

    for (c=0; c<keypad_columns; c++){
		digitalwrite(columnPins[c],LOW);	// Activate the current column.
		for (r=0; r<keypad_rows; r++){	// Scan all the rows for a key press.
			//  The user pressed a button for more then debounceTime microseconds.
			if (currentKey == userKeymap[c+(r*keypad_columns)]){
				// Button hold
				if (((Millis()-lastUpdate) >= holdTime) && digitalread(rowPins[r]) == LOW){
					Keypad_transitionTo(HOLD);
				}
				// Button release
				if (((Millis()-lastUpdate) >= debounceTime) && digitalread(rowPins[r]) == HIGH){
					Keypad_transitionTo(RELEASED);
					currentKey = NO_KEY;
				}
			} 
			// Button pressed event.  The user pressed a button.
			else if (((Millis()-lastUpdate) >= debounceTime) && digitalread(rowPins[r]) == LOW){
				digitalwrite(columnPins[c],HIGH);	// De-activate the current column.
				key = userKeymap[c+(r*keypad_columns)];
				lastUpdate = Millis();
				goto EVALUATE_KEY; 			// Save resources and do not attempt to parse two keys at a time
			} 
		}
		digitalwrite(columnPins[c],HIGH);	// De-activate the current column.
	}
	
	EVALUATE_KEY:
	if (key != NO_KEY && key != currentKey){ 
		currentKey = key;
		Keypad_transitionTo(PRESSED);
		return currentKey;
	} 
	else{
		return NO_KEY;
	}
}
