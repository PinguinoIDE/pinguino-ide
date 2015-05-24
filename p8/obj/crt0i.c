/*-------------------------------------------------------------------------
   crt0i.c - SDCC pic16 port runtime start code with initialisation

   Copyright (C) 2004, Vangelis Rokas <vrokas at otenet.gr>

   This library is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the
   Free Software Foundation; either version 2.1, or (at your option) any
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
 * 2011-11-20 - regis blanchot - modified for pinguino project
 * compilation : sdcc -c -o crt0i18fxxxx.o -mpic16 -p18fxxxx crt0i.c
 * --ivt-loc=0x0800
 */

extern stack_end;
extern TBLPTRU;
extern TBLPTRH;
extern TBLPTRL;
extern FSR0L;
extern FSR0H;
extern TABLAT;
extern POSTINC0;


#if 1
/* global variable for forcing gplink to add _cinit section */
char __uflags = 0;
#endif

/* external reference to the user's main routine */
extern void pinguino_main (void);

void _entry (void) __naked __interrupt 0;
void _startup (void) __naked;
void _do_cinit (void) __naked;

/* Access bank selector. */
#define a 0


/*
 * entry function, placed at interrupt vector 0 (RESET)
 */
void
_entry (void) __naked __interrupt 0
{
  __asm
    goto    __startup
  __endasm;
}

void
_startup (void) __naked
{
  __asm
    ; Initialize the stack pointer
    lfsr    1, _stack_end
    lfsr    2, _stack_end

    ; 1st silicon does not do this on POR
    clrf    _TBLPTRU, a

    ; Initialize the flash memory access configuration.
    ; This is harmless for non-flash devices, so we do it on all parts.
    bsf     0xa6, 7, a      ; EECON1.EEPGD = 1, TBLPTR accesses program memory
    bcf     0xa6, 6, a      ; EECON1.CFGS  = 0, TBLPTR accesses program memory
  __endasm;

  /* Initialize global and/or static variables. */
  _do_cinit ();

  /* Call the main routine. */
  pinguino_main ();

  __asm
lockup:
    ; Returning from main will lock up.
    bra     lockup
  __endasm;
}


/* the cinit table will be filled by the linker */
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

#define prom		0x00		/* 0x00 0x01 0x02*/
#define curr_byte	0x03		/* 0x03 0x04 */
#define curr_entry	0x05		/* 0x05 0x06 */
#define data_ptr	0x07		/* 0x07 0x08 0x09 */

/* the variable initialisation routine */
void
_do_cinit (void) __naked
{
  /*
   * access registers 0x00 - 0x09 are not saved in this function
   */
  __asm
    ; TBLPTR = &cinit
    movlw   low(_cinit)
    movwf   _TBLPTRL, a
    movlw   high(_cinit)
    movwf   _TBLPTRH, a
    movlw   upper(_cinit)
    movwf   _TBLPTRU, a

    ; curr_entry = cinit.num_init
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   curr_entry, a

    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   curr_entry + 1, a

    ; while (curr_entry)
    movf    curr_entry, w, a
test:
    bnz     cont1
    movf    curr_entry + 1, w, a
    bz      done

cont1:
    ; Count down so we only have to look up the data in _cinit once.

    ; At this point we know that TBLPTR points to the top of the current
    ; entry in _cinit, so we can just start reading the from, to, and
    ; size values.

    ; read the source address low
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   prom, a

    ; source address high
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   prom + 1, a

    ; source address upper
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   prom + 2, a

    ; skip a byte since it is stored as a 32bit int
    TBLRDPOSTINC

    ; read the destination address directly into FSR0
    ; destination address low
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   _FSR0L, a

    ; destination address high
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   _FSR0H, a

    ; skip two bytes since it is stored as a 32bit int
    TBLRDPOSTINC
    TBLRDPOSTINC

    ; read the size of data to transfer to destination address
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   curr_byte, a

    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   curr_byte + 1, a

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
    movf    curr_byte, w, a
copy_loop:
    bnz     copy_one_byte
    movf    curr_byte + 1, w, a
    bz      done_copying

copy_one_byte:
    TBLRDPOSTINC
    movf    _TABLAT, w, a
    movwf   _POSTINC0, a

    ; decrement byte counter
    decf    curr_byte, f, a
    bc      copy_loop
    decf    curr_byte + 1, f, a
    bra     copy_one_byte

done_copying:
    ; restore the table pointer for the next entry
    ; TBLPTR = data_ptr
    movff   data_ptr, _TBLPTRL
    movff   data_ptr + 1, _TBLPTRH
    movff   data_ptr + 2, _TBLPTRU

    ; decrement entry counter
    decf    curr_entry, f, a
    bc      test
    decf    curr_entry + 1, f, a
    bra     cont1

    ; emit done label
done:
    return
  __endasm;
}
