/*
 * IRremote.h
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.htm http://arcfn.com
 * Edited by Mitra to add new controller SANYO
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */

#ifndef IRREMOTE_H
#define IRREMOTE_H

#ifndef DIGITALWRITE
#define DIGITALWRITE
#endif

#ifndef DIGITALREAD
#define DIGITALREAD
#endif

#include <typedef.h>

// The following are compile-time library options.
// If you change them, recompile the library.
// If DEBUG is defined, a lot of debugging output will be printed during decoding.
// TEST must be defined for the IRtest unittests to work.  It will make some
// methods virtual, which will be slightly slower, which is why it is optional.
// #define DEBUG
// #define TEST

/***********************************************************************
    Some useful constants
***********************************************************************/

// Values for decode_type
#define NEC 1
#define SONY 2
#define RC5 3
#define RC6 4
#define DISH 5
#define SHARP 6
#define PANASONIC 7
#define JVC 8
#define SANYO 9
#define MITSUBISHI 10
#define UNKNOWN -1

// Decoded value for NEC when a repeat code is received
#define REPEAT 0xffffffff

#define USECPERTICK 50  // microseconds per clock interrupt tick
#define RAWBUF 100 // Length of raw duration buffer

// Marks tend to be 100us too long, and spaces 100us too short
// when received due to sensor lag.
#define MARK_EXCESS 100
#define ERR 0
#define DECODED 1

// defines for setting and clearing register bits
/*
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
*/

// Pulse parms are *50-100 for the Mark and *50+100 for the space
// First MARK is the one after the long gap
// pulse parameters in usec
#define NEC_HDR_MARK  9000
#define NEC_HDR_SPACE 4500
#define NEC_BIT_MARK  560
#define NEC_ONE_SPACE 1600
#define NEC_ZERO_SPACE  560
#define NEC_RPT_SPACE 2250

#define SONY_HDR_MARK 2400
#define SONY_HDR_SPACE  600
#define SONY_ONE_MARK 1200
#define SONY_ZERO_MARK  600
#define SONY_RPT_LENGTH 45000
#define SONY_DOUBLE_SPACE_USECS  500  // usually ssee 713 - not using ticks as get number wrapround

// SA 8650B
#define SANYO_HDR_MARK  3500  // seen range 3500
#define SANYO_HDR_SPACE 950 //  seen 950
#define SANYO_ONE_MARK  2400 // seen 2400  
#define SANYO_ZERO_MARK 700 //  seen 700
#define SANYO_DOUBLE_SPACE_USECS  800  // usually ssee 713 - not using ticks as get number wrapround
#define SANYO_RPT_LENGTH 45000

// Mitsubishi RM 75501
// 14200 7 41 7 42 7 42 7 17 7 17 7 18 7 41 7 18 7 17 7 17 7 18 7 41 8 17 7 17 7 18 7 17 7 

// #define MITSUBISHI_HDR_MARK  250  // seen range 3500
#define MITSUBISHI_HDR_SPACE  350 //  7*50+100
#define MITSUBISHI_ONE_MARK 1950 // 41*50-100
#define MITSUBISHI_ZERO_MARK  750 // 17*50-100
// #define MITSUBISHI_DOUBLE_SPACE_USECS  800  // usually ssee 713 - not using ticks as get number wrapround
// #define MITSUBISHI_RPT_LENGTH 45000


#define RC5_T1    889
#define RC5_RPT_LENGTH  46000

#define RC6_HDR_MARK  2666
#define RC6_HDR_SPACE 889
#define RC6_T1    444
#define RC6_RPT_LENGTH  46000

#define SHARP_BIT_MARK 245
#define SHARP_ONE_SPACE 1805
#define SHARP_ZERO_SPACE 795
#define SHARP_GAP 600000
#define SHARP_TOGGLE_MASK 0x3FF
#define SHARP_RPT_SPACE 3000

#define DISH_HDR_MARK 400
#define DISH_HDR_SPACE 6100
#define DISH_BIT_MARK 400
#define DISH_ONE_SPACE 1700
#define DISH_ZERO_SPACE 2800
#define DISH_RPT_SPACE 6200
#define DISH_TOP_BIT 0x8000

#define PANASONIC_HDR_MARK 3502
#define PANASONIC_HDR_SPACE 1750
#define PANASONIC_BIT_MARK 502
#define PANASONIC_ONE_SPACE 1244
#define PANASONIC_ZERO_SPACE 400

#define JVC_HDR_MARK 8000
#define JVC_HDR_SPACE 4000
#define JVC_BIT_MARK 600
#define JVC_ONE_SPACE 1600
#define JVC_ZERO_SPACE 550
#define JVC_RPT_LENGTH 60000

#define SHARP_BITS 15
#define DISH_BITS 16

// receiver states
#define STATE_IDLE     2
#define STATE_MARK     3
#define STATE_SPACE    4
#define STATE_STOP     5

// IR detector output is active low
#define MARK  0
#define SPACE 1

#define TOPBIT 0x80000000

#define NEC_BITS 32
#define SONY_BITS 12
#define SANYO_BITS 12
#define MITSUBISHI_BITS 16
#define MIN_RC5_SAMPLES 11
#define MIN_RC6_SAMPLES 1
#define PANASONIC_BITS 48
#define JVC_BITS 16

#define _GAP 5000 // Minimum map between transmissions
#define GAP_TICKS (_GAP/USECPERTICK)

#define TOLERANCE 25  // percent tolerance in measurements

/***********************************************************************
    Macros
***********************************************************************/

#define LTOL (1.0 - TOLERANCE/100.) 
#define UTOL (1.0 + TOLERANCE/100.) 
#define TICKS_LOW(us) (u16) (((us)*LTOL/USECPERTICK))
#define TICKS_HIGH(us) (u16) (((us)*UTOL/USECPERTICK + 1))

#ifndef DEBUG
u16 MATCH(u16 measured, u16 desired) {return measured >= TICKS_LOW(desired) && measured <= TICKS_HIGH(desired);}
u16 MATCH_MARK(u16 measured_ticks, u16 desired_us) {return MATCH(measured_ticks, (desired_us + MARK_EXCESS));}
u16 MATCH_SPACE(u16 measured_ticks, u16 desired_us) {return MATCH(measured_ticks, (desired_us - MARK_EXCESS));}
// Debugging versions are in IRremote.c
#endif

/***********************************************************************
    Global
***********************************************************************/

// Results returned from the decoder
typedef struct
{
	u16 decode_type; // NEC, SONY, RC5, UNKNOWN
	u16 panasonicAddress; // This is only used for decoding Panasonic data
	u32 value; // Decoded value
	u16 bits; // Number of bits in decoded value
	volatile u16 *rawbuf; // Raw intervals in .5 us ticks
	u16 rawlen; // Number of records in rawbuf.
} decode_results;//_t;

// information for the interrupt handler
typedef struct {
    u8  recvpin;           // pin for IR data from detector
    u8  outpin;            // pin to output IR data
    u8  rcvstate;          // state machine
    u8  blinkflag;         // TRUE to enable blinking of pin 13 on IR processing
    u16 timer;     // state timer, counts 50uS ticks.
    u16 rawbuf[RAWBUF]; // raw data
    u8  rawlen;         // counter of entries in rawbuf
} 
irparams_t;

// Defined in IRremote.c
extern volatile irparams_t irparams;

// Defined in IRremote.c
//extern volatile decode_results_t results;

/***********************************************************************
    Prototypes
***********************************************************************/

// main class for receiving IR
//void IRrecv_IRrecv(u16 recvpin);
//void IRrecv_init(u16 recvpin, u16 outpin);

void IRrecv_blink13(u8 blinkflag);
u16 IRrecv_decode(decode_results *results);
void IRrecv_enableIRIn(u8 recvpin);
void IRrecv_resume();

// These are called by decode
u16 IRrecv_getRClevel(decode_results *results, u16 *offset, u16 *used, u16 t1);
u32 IRrecv_decodeNEC(decode_results *results);
u32 IRrecv_decodeSony(decode_results *results);
u32 IRrecv_decodeSanyo(decode_results *results);
u32 IRrecv_decodeMitsubishi(decode_results *results);
u32 IRrecv_decodeRC5(decode_results *results);
u32 IRrecv_decodeRC6(decode_results *results);
u32 IRrecv_decodePanasonic(decode_results *results);
u32 IRrecv_decodeJVC(decode_results *results);
u32 IRrecv_decodeHash(decode_results *results);
u16 IRrecv_compare(u16 oldval, u16 newval);

// private:
void IRsend_enableIROut(u8 outpin);
void IRsend_mark(u16 usec);
void IRsend_space(u16 usec);

void IRsend_IRsend();
void IRsend_sendNEC(u32 data, u16 nbits);
void IRsend_sendSony(u32 data, u16 nbits);
// Neither Sanyo nor Mitsubishi send is implemented yet
//  void sendSanyo(u32 data, u16 nbits);
//  void sendMitsubishi(u32 data, u16 nbits);
void IRsend_sendRaw(u16 buf[], u16 len, u16 hz);
void IRsend_sendRC5(u32 data, u16 nbits);
void IRsend_sendRC6(u32 data, u16 nbits);
void IRsend_sendDISH(u32 data, u16 nbits);
void IRsend_sendSharp(u32 data, u16 nbits);
void IRsend_sendPanasonic(u16 address, u32 data);
void IRsend_sendJVC(u32 data, u16 nbits, u16 repeat); // *Note instead of sending the REPEAT constant if you want the JVC repeat signal sent, send the original code value and change the repeat argument from 0 to 1. JVC protocol repeats by skipping the header NOT by sending a separate code value like NEC does.

#endif /* IRREMOTE_H */
