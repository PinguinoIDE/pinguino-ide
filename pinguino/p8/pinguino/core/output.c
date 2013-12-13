/*	--------------------------------------------------------------------
	FILE:			output.c
	PROJECT:		Pinguino
	PURPOSE:		Unified Display Functions
	PROGRAMER:		régis blanchot <rblanchot@gmail.com>
	FIRST RELEASE:	09 Nov. 2013
	LAST RELEASE:	09 Nov. 2013
	--------------------------------------------------------------------
	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	------------------------------------------------------------------*/

#ifndef __OUTPUT_C__
#define __OUTPUT_C__

#define SERIAL      1
#define SERIAL1     1
#define SERIAL2     2

#include <serial.c>
#include <string.h>
//#include <string.c>

// TODO include LCD lib, serial lib for UART2 etc.....


// this function is used to allocate space on the heap
// ( called by malloc )

int *heap_ptr;
extern int _heap;

int *sbrk(int nbbytes)
{
	int *base;
	
	if (!heap_ptr)
		heap_ptr=(int *)&_heap;	// _heap is defined in the linker script
	base=heap_ptr;					// on Pinguino32 heap is 8192 bytes 
	heap_ptr += nbbytes;
	return base;
}

// open
// open a stream output
// success return the number associated with the stream
// else return -1

int open(const char *name, int flags, int mode)
{	
	if (!strcmp(name,"SERIAL1")) 
	{
		SerialConfigure(UART1, UART_ENABLE,	UART_RX_TX_ENABLED,	9600);
		return(1);
	}

	if (!strcmp(name,"SERIAL2")) 
	{
		SerialConfigure(UART2, UART_ENABLE,	UART_RX_TX_ENABLED,	9600);
		return(2);
	}
	
// TODO LCD, SPI etc.....
// TODO define a flag to know if the stream is opened
	
	return -1;
}

// close
// close a stream
// success return 0, failed return -1

int close(int file)
{
	return -1;
	// TODO clear interrupt for some stream
}

// fstat
// called by newlib to know the state of a stream
// return 0 if stream is available
// failed return -1

// NV: Changed the second param to a void* to get rid of a compiler warning.
//     Change it back to struct stat * whenever this gets implemented.
//int fstat(int file, stat *st) 
//int fstat(int file, struct stat *st)
int fstat(int file, void *st)
{
	return 0;
}

// isatty
// isatty is called to know if a stream accepts tty
// success return 1
// failed return 0

int isatty(int file)
{
	return 1;
}

// lseek
// lseek set position in a file
// success output is the position in the file
// failed -1

int lseek(int file, int ptr, int dir)
{
	return 0;
}

// read
// read len char in the file and put them in ptr
// success return the number of char read
// failed return -1

int read(int file, char *ptr, int len)
{
	int caractere=0;
	int nbchar=0;
	
	switch (file)
	{
		case 0:
			while (caractere!=-1)		// stdin
			{
				caractere=SerialRead(UART1);
				if (caractere!=-1)
				{
					*ptr++=caractere;
					nbchar++;
				}
			}
			if (nbchar!=0)
				return(nbchar);
			else
				return(-1);
			break;
	}

	return -1;
}

// write
// write 'len' char on the 'file' stream
// success: return number of characters written
// failed: return -1

int write(char file, char *ptr, int len)
{
	int todo;
	switch (file)
	{
		case 1:
			for (todo = 0; todo < len; todo++)
				SerialUART1WriteChar(*ptr++);	// stdout
			return len;
			break;
		case 2:
			for (todo = 0; todo < len; todo++)
				SerialUART2WriteChar(*ptr++);	// stdout
			return len;
			break;
		default:
			return(-1);
			break;	
	}
	
	return -1;
}

#endif	/* __OUTPUT_C__ */
