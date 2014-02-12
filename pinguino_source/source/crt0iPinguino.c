/*-------------------------------------------------------------------------
   crt0i.c - SDCC pic16 port runtime start code with initialisation

   Copyright (C) 2004, Vangelis Rokas <vrokas at otenet.gr>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2, or (at your option) any
   later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License 
   along with this library; see the file COPYING. If not, write to the
   Free Software Foundation, 51 Franklin Street, Fifth Floor, Boston,
   MA 02110-1301, USA.

   As a special exception, if you link this library with other files,
   some of which are compiled with SDCC, to produce an executable,
   this library does not by itself cause the resulting executable to
   be covered by the GNU General Public License. This exception does
   not however invalidate any other reasons why the executable file
   might be covered by the GNU General Public License.
-------------------------------------------------------------------------*/

/*
 * based on Microchip MPLAB-C18 startup files
 * modified for pinguino by régis blanchot
 */

extern stack_end;
//extern TBLPTRU;
//extern TBLPTRH;
//extern TBLPTRL;
//extern FSR0L;
//extern FSR0H;
//extern TABLAT;
//extern POSTINC0;

#if 1
// global variable for forcing gplink to add _cinit section
char __uflags = 0;
#endif

// Access bank selector
#define _bank_selector_ 0

// external reference to the pinguino's main()
extern void pinguino_main (void);

void _entry (void) __naked __interrupt 0;
void _startup (void) __naked;
void _do_cinit (void) __naked;

// NB: All interrupt vectors are moved respectively at
// ENTRY + 0x08, ENTRY + 0x08 and ENTRY + 0x18
// at compilation time, cf. option --ivt-loc = $(ENTRY) 

// ENTRY + 0x00
void _entry (void) __naked __interrupt 0
{
    __asm
    goto    __startup
    __endasm;
}

void _startup (void) __naked
{
    __asm
    ; Initialize the stack pointer
    lfsr    1, _stack_end
    lfsr    2, _stack_end

    ; 1st silicon does not do this on POR
    clrf    _TBLPTRU, _bank_selector_

    ; Initialize the flash memory access configuration.
    ; This is harmless for non-flash devices, so we do it on all parts.

    ;#if defined(__18f2455)  || defined(__18f4455)  || \
    ;    defined(__18f2550)  || defined(__18f4550)  || \
    ;    defined(__18f25k50) || defined(__18f45k50)
    
    bsf     0xa6, 7, _bank_selector_      ; EECON1.EEPGD = 1, Access Flash program memory, not EEPROM memory
    bcf     0xa6, 6, _bank_selector_      ; EECON1.CFGS  = 0, Access Flash program or data EEPROM memory, not Configuration registers

    ;#elif defined(__18f26j50) || defined(__18f46j50)

    ;bsf     0xa6, 2, _bank_selector_      ; EECON1.WREN = 1; enable write to memory
    ;bsf     0xa6, 1, _bank_selector_      ; EECON1.WR = 1; Initiates a program memory erase cycle or write cycle

    ;#endif

    __endasm;

    // Initialize global and/or static variables.
    _do_cinit();

    // Call the Pinguino main routine.
    pinguino_main();

    __asm
    lockup:
    bra     lockup          ; Returning from main will lock up
    __endasm;
}


// the cinit table will be filled by the linker
extern __code struct
  {
    unsigned short num_init;
    struct
      {
        unsigned long from;
        unsigned long to;
        unsigned long size;
      } entries[1];
  } cinit;


#define TBLRDPOSTINC	tblrd*+

#define prom		0x00		// 0x00 0x01 0x02
#define curr_byte	0x03		// 0x03 0x04
#define curr_entry	0x05		// 0x05 0x06
#define data_ptr	0x07		// 0x07 0x08 0x09

// the variable initialisation routine
void _do_cinit (void) __naked
{
   // access registers 0x00 - 0x09 are not saved in this function
  __asm
    ; TBLPTR = &cinit
    movlw   low(_cinit)
    movwf   _TBLPTRL, _bank_selector_
    movlw   high(_cinit)
    movwf   _TBLPTRH, _bank_selector_
    movlw   upper(_cinit)
    movwf   _TBLPTRU, _bank_selector_

    ; curr_entry = cinit.num_init
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   curr_entry, _bank_selector_

    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   curr_entry + 1, _bank_selector_

    ; while (curr_entry)
    movf    curr_entry, w, _bank_selector_
test:
    bnz     cont1
    movf    curr_entry + 1, w, _bank_selector_
    bz      done

cont1:
    ; Count down so we only have to look up the data in _cinit once.

    ; At this point we know that TBLPTR points to the top of the current
    ; entry in _cinit, so we can just start reading the from, to, and
    ; size values.

    ; read the source address low
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   prom, _bank_selector_

    ; source address high
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   prom + 1, _bank_selector_

    ; source address upper
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   prom + 2, _bank_selector_

    ; skip a byte since it is stored as a 32bit int
    TBLRDPOSTINC

    ; read the destination address directly into FSR0
    ; destination address low
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   _FSR0L, _bank_selector_

    ; destination address high
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   _FSR0H, _bank_selector_

    ; skip two bytes since it is stored as a 32bit int
    TBLRDPOSTINC
    TBLRDPOSTINC

    ; read the size of data to transfer to destination address
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   curr_byte, _bank_selector_

    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   curr_byte + 1, _bank_selector_

    ; skip two bytes since it is stored as a 32bit int
    TBLRDPOSTINC
    TBLRDPOSTINC

    ;  prom = data_ptr->from;
    ;  FSR0 = data_ptr->to;
    ;  curr_byte = (unsigned short) data_ptr->size;

    ; the table pointer now points to the next entry. Save it
    ; off since we will be using the table pointer to do the copying
    ; for the entry

    ; data_ptr = TBLPTR
    movff   _TBLPTRL, data_ptr
    movff   _TBLPTRH, data_ptr + 1
    movff   _TBLPTRU, data_ptr + 2

    ; now assign the source address to the table pointer
    ; TBLPTR = prom
    movff   prom, _TBLPTRL
    movff   prom + 1, _TBLPTRH
    movff   prom + 2, _TBLPTRU

    ; while (curr_byte)
    movf    curr_byte, w, _bank_selector_
copy_loop:
    bnz     copy_one_byte
    movf    curr_byte + 1, w, _bank_selector_
    bz      done_copying

copy_one_byte:
    TBLRDPOSTINC
    movf    _TABLAT, w, _bank_selector_
    movwf   _POSTINC0, _bank_selector_

    ; decrement byte counter
    decf    curr_byte, f, _bank_selector_
    bc      copy_loop
    decf    curr_byte + 1, f, _bank_selector_
    bra     copy_one_byte

done_copying:
    ; restore the table pointer for the next entry
    ; TBLPTR = data_ptr
    movff   data_ptr, _TBLPTRL
    movff   data_ptr + 1, _TBLPTRH
    movff   data_ptr + 2, _TBLPTRU

    ; decrement entry counter
    decf    curr_entry, f, _bank_selector_
    bc      test
    decf    curr_entry + 1, f, _bank_selector_
    bra     cont1

    ; emit done label
done:
    return
  __endasm;
}
