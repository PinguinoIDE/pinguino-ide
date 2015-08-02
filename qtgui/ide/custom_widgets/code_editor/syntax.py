#! /usr/bin/python
#-*- coding: utf-8 -*-

import datetime
today = datetime.datetime.now()

Autocompleter = {}
Autocompleter["directive"] = ["define", "include", "ifndef", "endif", "undef", "if", "elif", "else", "error", "warning"]

#from const.h
const = [ "PI", "HALF_PI", "TWO_PI", "DEG_TO_RAD", "RAD_TO_DEG", "NULL", "ON", "OFF", "FALSE", "TRUE", "True", "False", "false", "true",
          "INPUT", "OUTPUT", "HIGH",
          "LOW", "AND", "OR",
          "BYTE", "BIN", "OCT", "DEC", "HEX", "FLOAT",
          "LED1", "LED2", "LED3", "LED4", "WHITELED", "GREENLED", "USERLED", "YELLOWLED", "REDLED", "PROGBUTTON", "USERBUTTON",
          "RTCC", "PMCS1", "PMRD", "PMWR", "PMA1",


          #Others, not in cons.h
          "FOSC", "MIPS",
          "PORTA", "PORTB", "PORTC", "PORTD", "PORTE", "PORTF", "PORTG",

          "void", "const", "BOOL", "char", "unsigned", "short", "int", "long", "float", "double", "byte", "word",
          "u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64",

          "struct", "union", "typedef", "enum", "register", "static", "extern", "volatile",
          "loop", "setup", "INT_MILLISEC", "INT_MICROSEC", "INT_FALLING_EDGE", "interrupt",


          #C syntax
          "if", "switch", "for", "while", "do", "continue", "break", "else", "return", "case", "default",
]


const += ["P1_{}".format(p) for p in range(1, 17)]
const += ["P2_{}".format(p) for p in range(1, 17)]
const += ["D{}".format(p) for p in range(14)]
const += ["A{}".format(p) for p in range(8)]
const += ["PWM{}".format(p) for p in range(5)]
const += ["PWD{}".format(p) for p in range(8)]

Autocompleter["reserved"] = const

Snippet = {}
#Snippet[name {snippet}]=["to insert, with cursor mark: [!]"] ##[!] is the cursor position

Snippet["void {snippet}"] = "void [!]() {\n\n}"
Snippet["while( ) {snippet}"] = "while ([!]) {\n\n}"
Snippet["do {...} while  {snippet}"] = "do {\n\t[!]\n} while();"
Snippet["for {snippet}"] = "for ([!]) {\n\n}"
Snippet["for (i=0;...{snippet}"] = "for (i=[!]; i<; i++) {\n\n}"
Snippet["if...else {snippet}"] = "if ([!]) {\n\n\t}\nelse {\n\n}"
Snippet["if...else if...else {snippet}"] = "if ([!]) {\n\n}\nelse if () {\n\n}\nelse {\n\n}"
Snippet["switch( ) {snippet}"] = "switch ([!]) {\n\tcase:\n\n\t\tbreak;\n\n\tdefault:\n\n\t\tbreak;}"
Snippet["case {snippet}"] = "case [!]: \n\n\tbreak;"

Snippet["struct {snippet}"] = "struct {\n\t[!]\n};"
Snippet["typedef ... struct {snippet}"] = "typedef struct {\n\t[!]\n}TYPE;"
Snippet["union {snippet}"] = "union [!] {\n\t\n};"
Snippet["typedef ... union {snippet}"] = "typedef union {\n\t[!]\n}TYPE;"

Snippet["Bare minimum {snippet}"] = "void setup() {\n\t// put your setup code here, to run once:\n\n\t[!]\n}\n\nvoid loop() {\n\t// put your main code here, to run repeatedly:\n\n}\n"


Snippet["file {snippet}"] = """/*-----------------------------------------------------
{}: [!] --<>
{}: {}
{}:

-----------------------------------------------------*/
""".format("Author", "Date", today.strftime("%Y-%m-%d"), "Description")

for key in Snippet.keys():
    Snippet[key] = Snippet[key].replace("\t", " "*4)


# Helpers = {
    # "pinMode": "pinMode({{USERLED, OUTPUT}});",
    # "digitalRead": "digitalRead({{PIN}});",
    # "analogRead": "analogRead({{PIN}});",
    # "digitalWrite": "digitalWrite({{USERLED, HIGH}});",
    # "analogWrite": "digitalWrite({{PIN, 256}});",
    # "delay": "delay({{MILLISECONDS}});",

    # "toggle": "toggle({{USERLED}});",
    # "random": "random({{1, 10}});",

    # "Serial.begin": "Serial.begin({{9600}});",
    # "Serial.printNumber": "Serial.printNumber({{256, DEC}});",

    # "lcd.pins": "lcd.pins({{0, 1, 2, 3, 4, 5, 0, 0, 0, 0}}); // RS, E, D4 ~ D8",
    # "lcd.begin": "lcd.begin({{16, 2}});"
# }