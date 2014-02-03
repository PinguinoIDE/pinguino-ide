#! /usr/bin/python
#-*- coding: utf-8 -*-

import datetime
today = datetime.datetime.now()

Autocompleter = {}
Autocompleter["directive"] = ["define", "include", "ifndef", "endif",  "undef", "if", "elif", "else", "error", "warning"]

Autocompleter["reserved"] = ["TRUE", "FALSE", "HIGH", "LOW", "INPUT", "OUTPUT", "FOSC", "MIPS", "ON", "OFF",
              "LED1", "LED2", "LED3", "LED4", "REDLED", "GREENLED", "BLUELED", "WHITELED", "YELLOWLED",
              "PROGBUTTON", "USERBUTTON", "RUNLED", "USERLED", "A4", 
              "PORTA", "PORTB", "PORTC", "PORTD", "PORTE", "PORTF", "PORTG",
              "if", "switch", "for", "while", "do", "continue", "break", "else", "return", "case", "default",
              "void", "const", "BOOL", "char", "unsigned", "short", "int", "long", "float", "double", "byte", "word",
              "u8", "s8", "u16", "s16", "u32", "s32", "u64", "s64",
              "struct", "union", "typedef", "enum", "register",  "static", "extern", "volatile",
              "loop", "setup", "INT_MILLISEC", "INT_MICROSEC", "INT_FALLING_EDGE", "interrupt"]


Snippet = {}
#Snippet[name {snippet}]=[cursor position,"to insert"]
Snippet["void {snippet}"] = [5, "void (){\n\n\t}"]
Snippet["while( ) {snippet}"] = [7, "while (){\n\n\t}"]
Snippet["do {...} while  {snippet}"] = [8, "do {\n\t\n} while();"]
Snippet["for {snippet}"] = [5, "for (){\n\n\t}"]
Snippet["for (i=0;...{snippet}"] = [7, "for (i= ;i< ;i++){\n\n\t}"]
Snippet["if...else {snippet}"] = [4, "if (){\n\n\t}\nelse {\n\n\t}"]
Snippet["if...else if...else {snippet}"] = [4, "if (){\n\n\t}\nelse if (){\n\n\t}\nelse {\n\n\t}"]
Snippet["switch( ) {snippet}"] = [8, "switch (){\n\tcase:\n\n\t\tbreak;\n\tdefault:\n\n\t}"]

Snippet["struct {snippet}"] = [13, "struct {\n\t\n};"]
Snippet["typedef ... struct {snippet}"] = [21, "typedef struct {\n\t\n}TYPE;"]
Snippet["union {snippet}"] = [12, "union {\n\t\n};"]
Snippet["typedef ... union {snippet}"] = [20, "typedef union {\n\t\n}TYPE;"]

Snippet["Bare minimum {snippet}"] = [35, "void setup() {\n\t// put your setup code here, to run once:\n\n}\n\nvoid loop() {\n\t// put your main code here, to run repeatedly:\n\n}\n\nvoid interrupt() {\n\t// put your interrupt code here\n\t// this one is optional\n\n}"]


Snippet["file {snippet}"] = [65,
"""/*----------------------------------------------------- 
%s:  --<>
%s: %s
%s:

-----------------------------------------------------*/
""" %("Author", "Date", today.strftime("%Y-%m-%d"), "Description")]

for key in Snippet.keys(): Snippet[key][1] = Snippet[key][1].replace("\t", " "*4)
