#! /usr/bin/python
#-*- coding: utf-8 -*-

import datetime
today = datetime.datetime.now()

Autocompleter = {}
Autocompleter["directive"] = ["define", "include", "ifndef", "endif", "undef", "if", "elif", "else", "error", "warning"]
Autocompleter["reserved"] = ["TRUE", "FALSE", "HIGH", "LOW", "INPUT", "OUTPUT", "FOSC", "MIPS", "ON", "OFF",
              "LED1", "LED2", "LED3", "LED4", "REDLED", "GREENLED", "BLUELED", "WHITELED", "YELLOWLED",
              "PROGBUTTON", "USERBUTTON", "RUNLED", "USERLED", "A4", 
              "PORTA", "PORTB", "PORTC", "PORTD", "PORTE", "PORTF", "PORTG",
              "if", "switch", "for", "while", "do", "continue", "break", "else", "return", "case", "default",
              "void", "const", "BOOL", "char", "unsigned", "short", "int", "long", "float", "double", "byte", "word",
              "u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64",
              "struct", "union", "typedef", "enum", "register", "static", "extern", "volatile",
              "loop", "setup", "INT_MILLISEC", "INT_MICROSEC", "INT_FALLING_EDGE", "interrupt"]


Snippet = {}
#Snippet[name {snippet}]=["to insert, with cursor mark: [!]"] ##[!] is the cursos position

Snippet["void {snippet}"] = "void [!](){\n\n\t}"
Snippet["while( ) {snippet}"] = "while [!](){\n\n\t}"
Snippet["do {...} while  {snippet}"] = "do {\n\t[!]\n} while();"
Snippet["for {snippet}"] = "for ([!]){\n\n\t}"
Snippet["for (i=0;...{snippet}"] = "for (i=[!]; i<; i++){\n\n\t}"
Snippet["if...else {snippet}"] = "if ([!]){\n\n\t}\nelse {\n\n\t}"
Snippet["if...else if...else {snippet}"] = "if ([!]){\n\n\t}\nelse if (){\n\n\t}\nelse {\n\n\t}"
Snippet["switch( ) {snippet}"] = "switch ([!]){\n\tcase:\n\n\t\tbreak;\n\tdefault:\n\n\t}"

Snippet["struct {snippet}"] = "struct {\n\t[!]\n};"
Snippet["typedef ... struct {snippet}"] = "typedef struct {\n\t[!]\n}TYPE;"
Snippet["union {snippet}"] = "union [!]{\n\t\n};"
Snippet["typedef ... union {snippet}"] = "typedef union {\n\t[!]\n}TYPE;"

Snippet["Bare minimum {snippet}"] = "void setup() {\n\t// put your setup code here, to run once:\n\n\t[!]\n}\n\nvoid loop() {\n\t// put your main code here, to run repeatedly:\n\n}\n"


Snippet["file {snippet}"] = """/*----------------------------------------------------- 
%s: [!] --<>
%s: %s
%s:

-----------------------------------------------------*/
""" %("Author", "Date", today.strftime("%Y-%m-%d"), "Description")

for key in Snippet.keys():
    Snippet[key] = Snippet[key].replace("\t", " "*4)
