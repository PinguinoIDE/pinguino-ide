/*
Keyboard pinguino library.
Using ps2 library.
Samuele Paganoni.
*/
#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#define __PS2KEYB__

#include "ps2.h"

/*Renamed definitions.*/
#define keyboard_init() ps2_init()
#define keyboard_interrupt() ps2_isr()
#define keyboard_send(x) ps2_write(x)

/*With this definition the buffer will be empty before reading a char.*/
#define EMPTY_BUFFER

/*With this definition the default char value is null \0 .*/
#define READ_DEFAULT_NULL

/*Maximum number of characters read during a readline.*/
#define READLINE_MAX_CHARS 20

/*Make code definition (not all).*/
#define VK_A 0x1C
#define VK_B 0x32
#define VK_C 0x21
#define VK_D 0x23
#define VK_E 0x24
#define VK_F 0x2B
#define VK_G 0x34
#define VK_H 0x33
#define VK_I 0x43
#define VK_J 0x3B
#define VK_K 0x42
#define VK_L 0x4B
#define VK_M 0x3A
#define VK_N 0x31
#define VK_O 0x44
#define VK_P 0x4D
#define VK_Q 0x15
#define VK_R 0x2D
#define VK_S 0x1B
#define VK_T 0x2C
#define VK_U 0x3C
#define VK_V 0x2A
#define VK_W 0x1D
#define VK_X 0x22
#define VK_Y 0x35
#define VK_Z 0x1A
#define VK_0 0x45
#define VK_1 0x16
#define VK_2 0x1E
#define VK_3 0x26
#define VK_4 0x25
#define VK_5 0x2E
#define VK_6 0x36
#define VK_7 0x3D
#define VK_8 0x3E
#define VK_9 0x46
#define VK_BACKSPACE 0x66
#define VK_SPACE 0x29
#define VK_TAB 0x0D
#define VK_CAPS 0x58
#define VK_SHIFTL 0x12
#define VK_CONTROL 0x14
#define VK_ALT 0x11
#define VK_ENTER 0x5A
#define VK_ESC 0x76
#define VK_F1 0x05
#define VK_F2 0x06
#define VK_F3 0x04
#define VK_F4 0x0C
#define VK_F5 0x03
#define VK_F6 0x0B
#define VK_F7 0x83
#define VK_F8 0x0A
#define VK_F9 0x01
#define VK_F10 0x09
#define VK_F11 0x78
#define VK_F12 0x07
#define VK_PRINTCREEN 0x12
#define VK_SCROLL 0x7E
#define VK_PAUSE 0x77
#define VK_INSERT 0x70
#define VK_HOME 0x6C
#define VK_PAGEUP 0x7D
#define VK_DELETE 0x71
#define VK_END 0x69
#define VK_PAGEDOWN 0x7A
#define VK_ARROWUP 0x75
#define VK_ARROWDOWN 0x72
#define VK_ARROWLEFT 0x6B
#define VK_ARROWRIGHT 0x74
#define VK_NUM 0x77

/*Returns the make code of the key pressed.*/
unsigned char keyboard_readvk()
{
	unsigned char readed='\0',i=2;
#ifdef EMPTY_BUFFER
	while(ps2_available())
		ps2_read();
#endif
	while(!ps2_available());
	readed=ps2_read();
	while(i>0)
	{
		while(!ps2_available());
		ps2_read();
		i--;
	}
	return readed;
}

/*Returns the translation of make code.*/
unsigned char keyboard_translate(unsigned char vk)
{
	switch(vk)
	{
		case VK_A: return 'a';
		case VK_B: return 'b';
		case VK_C: return 'c';
		case VK_D: return 'd';
		case VK_E: return 'e';
		case VK_F: return 'f';
		case VK_G: return 'g';
		case VK_H: return 'h';
		case VK_I: return 'i';
		case VK_J: return 'j';
		case VK_K: return 'k';
		case VK_L: return 'l';
		case VK_M: return 'm';
		case VK_N: return 'n';
		case VK_O: return 'o';
		case VK_P: return 'p';
		case VK_Q: return 'q';
		case VK_R: return 'r';
		case VK_S: return 's';
		case VK_T: return 't';
		case VK_U: return 'u';
		case VK_V: return 'v';
		case VK_W: return 'w';
		case VK_X: return 'x';
		case VK_Y: return 'y';
		case VK_Z: return 'z';
		case VK_0: return '0';
		case VK_1: return '1';
		case VK_2: return '2';
		case VK_3: return '3';
		case VK_4: return '4';
		case VK_5: return '5';
		case VK_6: return '6';
		case VK_7: return '7';
		case VK_8: return '8';
		case VK_9: return '9';
		case VK_ENTER: return '\n';
		case VK_SPACE: return ' ';
#ifdef READ_DEFAULT_NULL
		default: return '\0';
#else
		default: return vk;
#endif
	}
}

/*Returns the char of the pressed key (if is mapped).*/
unsigned char keyboard_read()
{
	unsigned char vk=keyboard_readvk();
	return keyboard_translate(vk);
}

/*Returns a string.*/
unsigned char* keyboard_readline()
{
	unsigned char str[READLINE_MAX_CHARS],i,tmp;
	for(i=0;i<READLINE_MAX_CHARS-1;i++)
	{
		tmp=keyboard_read();
		if((tmp!='\n')&&(tmp!='\r'))
			str[i]=tmp;
		else
			break;
	}
	str[i]='\0';
	return (unsigned char*)str;
}

#endif
