/*
 * IRremote.c
 * Version 0.11 August, 2009
 * Copyright 2009 Ken Shirriff
 * For details, see http://arcfn.com/2009/08/multi-protocol-infrared-remote-library.html
 *
 * Modified by Paul Stoffregen <paul@pjrc.com> to support other boards and timers
 * Modified  by Mitra Ardron <mitra@mitra.biz> 
 * Added Sanyo and Mitsubishi controllers
 * Modified Sony to spot the repeat codes that some Sony's send
 *
 * Interrupt code based on NECIRrcv by Joe Knapp
 * http://www.arduino.cc/cgi-bin/yabb2/YaBB.pl?num=1210243556
 * Also influenced by http://zovirl.com/2008/11/12/building-a-universal-remote-with-an-arduino/
 *
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 */

 /*
  * Test version by PinguPlus 2014-02-16
  * 
 */
 
#ifndef IRREMOTE_C
#define IRREMOTE_C

#include <typedef.h>
#include <macro.h>
#include <pin.h>
#include <interrupt.h>
#include <IRremote.h>
//#include <IRremoteInt.h>      // moved content in IRremote.h
#include <pwm.c>
#include <digitalw.c>           // digitalwrite
#include <digitalr.c>           // digitalread
#include <delayms.c>            // Delayms
#include <oscillator.c>

//#define _1us_ { nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop(); }
#define _1us_ { nop(); nop(); nop(); nop(); }

volatile u16 _t3_reload_val;   // Timer3 reload value
volatile irparams_t irparams;
volatile u8 irdata;
//u8 d1; // used by delay50us assembly routine

//volatile decode_results_t results;

// These versions of MATCH, MATCH_MARK, and MATCH_SPACE are only for debugging.
// To use them, set DEBUG in IRremoteInt.h
// Normally macros are used for efficiency
#ifdef DEBUG
u16 MATCH(u16 measured, u16 desired) {
  serial_print("Testing: ");
  serial_print(TICKS_LOW(desired), DEC);
  serial_print(" <= ");
  serial_print(measured, DEC);
  serial_print(" <= ");
  serial_println(TICKS_HIGH(desired), DEC);
  return measured >= TICKS_LOW(desired) && measured <= TICKS_HIGH(desired);
}

u16 MATCH_MARK(u16 measured_ticks, u16 desired_us) {
  Serial_print("Testing mark ");
  Serial_print(measured_ticks * USECPERTICK, DEC);
  Serial_print(" vs ");
  Serial_print(desired_us, DEC);
  Serial_print(": ");
  Serial_print(TICKS_LOW(desired_us + MARK_EXCESS), DEC);
  Serial_print(" <= ");
  Serial_print(measured_ticks, DEC);
  Serial_print(" <= ");
  Serial_println(TICKS_HIGH(desired_us + MARK_EXCESS), DEC);
  return measured_ticks >= TICKS_LOW(desired_us + MARK_EXCESS) && measured_ticks <= TICKS_HIGH(desired_us + MARK_EXCESS);
}

u16 MATCH_SPACE(u16 measured_ticks, u16 desired_us) {
  Serial_print("Testing space ");
  Serial_print(measured_ticks * USECPERTICK, DEC);
  Serial_print(" vs ");
  Serial_print(desired_us, DEC);
  Serial_print(": ");
  Serial_print(TICKS_LOW(desired_us - MARK_EXCESS), DEC);
  Serial_print(" <= ");
  Serial_print(measured_ticks, DEC);
  Serial_print(" <= ");
  Serial_println(TICKS_HIGH(desired_us - MARK_EXCESS), DEC);
  return measured_ticks >= TICKS_LOW(desired_us - MARK_EXCESS) && measured_ticks <= TICKS_HIGH(desired_us - MARK_EXCESS);
}
#endif

#if defined(IRSEND_SENDNEC)
void IRsend_sendNEC(u32 data, u16 nbits)
{
  u16 i;
  
  PWM_setFrequency(38000);
  IRsend_mark(NEC_HDR_MARK);
  IRsend_space(NEC_HDR_SPACE);
  
  for (i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      IRsend_mark(NEC_BIT_MARK);
      IRsend_space(NEC_ONE_SPACE);
    } 
    else {
      IRsend_mark(NEC_BIT_MARK);
      IRsend_space(NEC_ZERO_SPACE);
    }
    data <<= 1;
  }
  IRsend_mark(NEC_BIT_MARK);
  IRsend_space(0);
}
#endif

#if defined(IRSEND_SENDSONY)
void IRsend_sendSony(u32 data, u16 nbits) {
  u16 i; 
  
  PWM_setFrequency(40000);
  IRsend_mark(SONY_HDR_MARK);
  IRsend_space(SONY_HDR_SPACE);
  data = data << (32 - nbits);

  for (i = 0; i < nbits; i++) {
    if (data & TOPBIT) {
      IRsend_mark(SONY_ONE_MARK);
      IRsend_space(SONY_HDR_SPACE);
    } 
    else {
      IRsend_mark(SONY_ZERO_MARK);
      IRsend_space(SONY_HDR_SPACE);
    }
    data <<= 1;
  }
}
#endif

#if defined(IRSEND_SENDRAW)
void IRsend_sendRaw(u16 buf[], u16 len, u16 hz)
{
    u16 i;

    PWM_setFrequency(hz);

    for (i = 0; i < len; i++) {
    if (i & 1) {
      IRsend_space(buf[i]);
    } 
    else {
      IRsend_mark(buf[i]);
    }
    }
    IRsend_space(0); // Just to be sure
}
#endif

#if defined(IRSEND_SENDRC5)
// Note: first bit must be a one (start bit)
void IRsend_sendRC5(u32 data, u16 nbits)
{ 
    u16 i;

    PWM_setFrequency(36000);
    PWM_setPercentDutyCycle(irparams.outpin, 50);
    data = data << (32 - nbits);

    IRsend_mark(RC5_T1); // First start bit
    IRsend_space(RC5_T1); // Second start bit
    IRsend_mark(RC5_T1); // Second start bit

    for (i = 0; i < nbits; i++)
    {
        if (data & TOPBIT)
        {
            IRsend_space(RC5_T1); // 1 is space, then mark
            IRsend_mark(RC5_T1);
        } 
        else
        {
            IRsend_mark(RC5_T1);
            IRsend_space(RC5_T1);
        }
        data <<= 1;
    }

    IRsend_space(0); // Turn off at end
}
#endif

#if defined(IRSEND_SENDRC6)
// Caller needs to take care of flipping the toggle bit
void IRsend_sendRC6(u32 data, u16 nbits)
{
  u16 t;
  u16 i;
  
  PWM_setFrequency(36000);
  data = data << (32 - nbits);
  IRsend_mark(RC6_HDR_MARK);
  IRsend_space(RC6_HDR_SPACE);
  IRsend_mark(RC6_T1); // start bit
  IRsend_space(RC6_T1);
  
  for (i = 0; i < nbits; i++) {
    if (i == 3) {
      // double-wide trailer bit
      t = 2 * RC6_T1;
    } 
    else {
      t = RC6_T1;
    }
    if (data & TOPBIT) {
      IRsend_mark(t);
      IRsend_space(t);
    } 
    else {
      IRsend_space(t);
      IRsend_mark(t);
    }

    data <<= 1;
  }
  IRsend_space(0); // Turn off at end
}
#endif

#if defined(IRSEND_SENDPANASONIC)
void IRsend_sendPanasonic(u16 address, u32 data) {
    u16 i;
	
	PWM_setFrequency(35000);
    IRsend_mark(PANASONIC_HDR_MARK);
    IRsend_space(PANASONIC_HDR_SPACE);
    
    for(i = 0; i<16; i++)
    {
        IRsend_mark(PANASONIC_BIT_MARK);
        if (address & 0x8000) {
            IRsend_space(PANASONIC_ONE_SPACE);
        } else {
            IRsend_space(PANASONIC_ZERO_SPACE);
        }
        address <<= 1;        
    }    
    for (i = 0; i < 32; i++) {
        IRsend_mark(PANASONIC_BIT_MARK);
        if (data & TOPBIT) {
            IRsend_space(PANASONIC_ONE_SPACE);
        } else {
            IRsend_space(PANASONIC_ZERO_SPACE);
        }
        data <<= 1;
    }
    IRsend_mark(PANASONIC_BIT_MARK);
    IRsend_space(0);
}
#endif

#if defined(IRSEND_SENDJVC)
void IRsend_sendJVC(u32 data, u16 nbits, u16 repeat)
{
	u16 i;
	
    PWM_setFrequency(38000);
    data = data << (32 - nbits);
    if (!repeat){
        IRsend_mark(JVC_HDR_MARK);
        IRsend_space(JVC_HDR_SPACE); 
    }
	
    for (i = 0; i < nbits; i++) {
        if (data & TOPBIT) {
            IRsend_mark(JVC_BIT_MARK);
            IRsend_space(JVC_ONE_SPACE); 
        } 
        else {
            IRsend_mark(JVC_BIT_MARK);
            IRsend_space(JVC_ZERO_SPACE); 
        }
        data <<= 1;
    }
    IRsend_mark(JVC_BIT_MARK);
    IRsend_space(0);
}
#endif

void IRsend_mark(u16 time)
{
    u16 i;
    // Sends an IR mark for the specified number of microseconds.
    // The mark output is modulated at the PWM frequency.

    // duty cycle = 50% = Enable PWM output
    PWM_setPercentDutyCycle(irparams.outpin, 50);
    
    // loopcounter i = time - correction factor
    // Without the NOPs the loop has an execution time of ~13 microseconds
    i = time-13;
    
    // while () {} needs much less time than for (;;) {}
    // for loop without NOPs : ~ 1,3 ms (889 iterations on 18f26j50)
    // while loop without NOPs: ~ 712 µs (889 Iterations on 18f26j50)
    while (i) {
      _1us_
      i--;
    }
    
/*
    for (i=0; i<time; i++)
    {
;        _1us_
    }
*/
}

/* Leave pin off for time (given in microseconds) */
void IRsend_space(u16 time)
{
    u16 i;
    // Sends an IR space for the specified number of microseconds.
    // A space is no output, so the PWM output is disabled.

    // duty cycle = 0% = Disable PWM output
    PWM_setPercentDutyCycle(irparams.outpin, 0);

    // loopcounter i = time - correction factor
    // without the NOPs the loop has an execution time of ~70 microseconds
  
    i = time - 70; // correction factor: 13 microseconds
    while (i) {
      _1us_
      i--;
    }
/*
    for (i=0; i<time; i++)
    {
        _1us_
    }
*/
}

//#define TIMER_DISABLE_INTR; //Timer2 Overflow Interrupt

#if defined(IRSEND_ENABLEIROUT)
void IRsend_enableIROut(u8 outpin)
{
    irparams.blinkflag = 0;
    irparams.outpin  = outpin;
}
#endif

#if defined(__IRREMOTE__)
void IRrecv_enableIRIn(u8 recvpin)
{
    irparams.recvpin = recvpin;
    irparams.blinkflag = 0;

    // Configure Timer3 to overload every 50us

    INTCONbits.GIEH    = 0;   // Disable global HP interrupts
    INTCONbits.GIEL    = 0;   // Disable global LP interrupts

    // Timer3 is on 16 bits, prescaler 1, based on external crystal Frequency
    #if defined(__18f25k50) || defined(__18f45k50)
    T3CON = T3_OFF | T3_16BIT | T3_SYNC_EXT_OFF | T3_SOSC_OFF | T3_PS_1_1 | T3_RUN_FROM_OSC;
    #else
    T3CON = T3_OFF | T3_16BIT | T3_SYNC_EXT_OFF | T3_OSC_OFF | T3_PS_1_1 | T3_RUN_FROM_OSC;
    #endif

    // nb cycles for 50us
    _t3_reload_val = ( System_getPeripheralFrequency() / 1000 / 1000 ); // 1us = 12 cycles @ 48MHz
    _t3_reload_val *= 50; // 50us = 50 * 12 cy @ 48MHz
    _t3_reload_val = 0xFFFF - _t3_reload_val;

    // load Timer3
    TMR3H = high8(_t3_reload_val);
    TMR3L =  low8(_t3_reload_val);

    IPR2bits.TMR3IP  = INT_HIGH_PRIORITY;   // Timer3 interrupt has a High-priority
    PIR2bits.TMR3IF  = 0;                   // Clear the Timer3 interrupt flag
    PIE2bits.TMR3IE  = INT_ENABLE;          // Enable Peripheral interrupts
    T3CONbits.TMR3ON = 1;                   // Start Timer3
    INTCONbits.GIEH  = 1;                   // Enable global HP interrupts
    INTCONbits.GIEL  = 1;                   // Enable global LP interrupts

    // initialize state machine variables
    irparams.rcvstate = STATE_IDLE;
    irparams.rawlen = 0;

    // set pin modes
    pinmode(irparams.recvpin, INPUT);
}
#endif

#if defined(IRRECV_BLINK13)
// enable/disable blinking of USERLED on IR processing
void IRrecv_blink13(u8 blinkflag)
{
  irparams.blinkflag = blinkflag;
  if (blinkflag)
    pinmode(USERLED, OUTPUT);
}
#endif

// TIMER3 interrupt code to collect raw data.
// Widths of alternating SPACE, MARK are recorded in rawbuf.
// Recorded in ticks of 50 microseconds.
// rawlen counts the number of entries recorded so far.
// First entry is the SPACE between transmissions.
// As soon as a SPACE gets long, ready is set, state switches to IDLE, timing of SPACE continues.
// As soon as first MARK arrives, gap width is recorded, ready is cleared, and new logging starts

#if defined(__IRREMOTE__)
void irremote_interrupt() // called from main.c
{
    if (PIR2bits.TMR3IF) // Timer3 interrupt ?
    {
        irdata = digitalread(irparams.recvpin);

        irparams.timer++; // One more 50us tick
        
        // Buffer overflow ?
        if (irparams.rawlen >= RAWBUF)
            irparams.rcvstate = STATE_STOP;

        switch(irparams.rcvstate)
        {
            // In the middle of a gap
            case STATE_IDLE:
                if (irdata == MARK)
                {
                    // Not big enough to be a gap.
                    if (irparams.timer < GAP_TICKS)
                        irparams.timer = 0;

                    // gap just ended, record duration and start recording transmission
                    else
                    {
                        irparams.rawlen = 0;
                        irparams.rawbuf[irparams.rawlen++] = irparams.timer;
                        irparams.timer = 0;
                        irparams.rcvstate = STATE_MARK;
                    }
                }
                break;

            // timing MARK
            case STATE_MARK:

                // MARK ended, record time
                if (irdata == SPACE)
                {
                  irparams.rawbuf[irparams.rawlen++] = irparams.timer;
                  irparams.timer = 0;
                  irparams.rcvstate = STATE_SPACE;
                }
                break;

            // timing SPACE
            case STATE_SPACE:

                // SPACE just ended, record it
                if (irdata == MARK)
                {
                    irparams.rawbuf[irparams.rawlen++] = irparams.timer;
                    irparams.timer = 0;
                    irparams.rcvstate = STATE_MARK;
                } 

                // SPACE
                else
                {
                    // big SPACE, indicates gap between codes
                    // Mark current code as ready for processing
                    // Switch to STOP
                    // Don't reset timer; keep counting space width
                    if (irparams.timer > GAP_TICKS)
                        irparams.rcvstate = STATE_STOP;
                }
                break;

            // waiting, measuring gap
            case STATE_STOP:

                // reset gap timer
                if (irdata == MARK)
                    irparams.timer = 0;
                break;
        }

        if (irparams.blinkflag)
        {
            if (irdata == MARK)
                digitalwrite(USERLED, 1);  // turn USERLED on
            else
                digitalwrite(USERLED, 0);  // turn USERLED off
        }

        // Timer3 reset
        TMR3H = high8(_t3_reload_val);
        TMR3L =  low8(_t3_reload_val);
        
        // Timer3 flag reset
        PIR2bits.TMR3IF = 0;
    }
}
#endif

#if defined(IRRECV_DECODE)
void IRrecv_resume()
{
  irparams.rcvstate = STATE_IDLE;
  irparams.rawlen = 0;
}
#endif

#if defined(IRRECV_DECODE)
// Decodes the received IR message
// Returns 0 if no data ready, 1 if data ready.
// Results of decoding are stored in results
u16 IRrecv_decode(decode_results *results)
{
    results->rawbuf = irparams.rawbuf;
    results->rawlen = irparams.rawlen;
    if (irparams.rcvstate != STATE_STOP) {
        return ERR;
    }
    #ifdef DEBUG
    Serial_println("Attempting NEC decode");
    #endif
    if (IRrecv_decodeNEC(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting Sony decode");
    #endif
    if (IRrecv_decodeSony(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting Sanyo decode");
    #endif
    if (IRrecv_decodeSanyo(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting Mitsubishi decode");
    #endif
    if (IRrecv_decodeMitsubishi(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting RC5 decode");
    #endif  
    if (IRrecv_decodeRC5(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting RC6 decode");
    #endif 
    if (IRrecv_decodeRC6(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting Panasonic decode");
    #endif 
    if (IRrecv_decodePanasonic(results)) {
        return DECODED;
    }
    #ifdef DEBUG
    Serial_println("Attempting JVC decode");
    #endif 
    if (IRrecv_decodeJVC(results)) {
        return DECODED;
    }
    // decodeHash returns a hash on any input.
    // Thus, it needs to be last in the list.
    // If you add any decodes, add them before this.
    if (IRrecv_decodeHash(results)) {
        return DECODED;
    }
    // Throw away and start over
    IRrecv_resume();
    return ERR;
}
#endif

#if defined(IRRECV_DECODENEC) || defined(IRRECV_DECODE)
// NECs have a repeat only 4 items long
u32 IRrecv_decodeNEC(decode_results *results)
{
    u32 data = 0;
    u16 offset = 1; // Skip first space
    u16 i;
  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], NEC_HDR_MARK)) {
    return ERR;
  }
  offset++;
  // Check for repeat
  if (irparams.rawlen == 4 &&
    MATCH_SPACE(results->rawbuf[offset], NEC_RPT_SPACE) &&
    MATCH_MARK(results->rawbuf[offset+1], NEC_BIT_MARK)) {
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = NEC;
    return DECODED;
  }
  if (irparams.rawlen < 2 * NEC_BITS + 4) {
    return ERR;
  }
  // Initial space  
  if (!MATCH_SPACE(results->rawbuf[offset], NEC_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  
  for (i = 0; i < NEC_BITS; i++) {
    if (!MATCH_MARK(results->rawbuf[offset], NEC_BIT_MARK)) {
      return ERR;
    }
    offset++;
    if (MATCH_SPACE(results->rawbuf[offset], NEC_ONE_SPACE)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_SPACE(results->rawbuf[offset], NEC_ZERO_SPACE)) {
      data <<= 1;
    } 
    else {
      return ERR;
    }
    offset++;
  }
  // Success
  results->bits = NEC_BITS;
  results->value = data;
  results->decode_type = NEC;
  return DECODED;
}
#endif

#if defined(IRRECV_DECODESONY) || defined(IRRECV_DECODE)
u32 IRrecv_decodeSony(decode_results *results)
{
    u32 data = 0;
    u16 offset = 0; // Dont skip first space, check its size

    if (irparams.rawlen < 2 * SONY_BITS + 2) return ERR;

    // Some Sony's deliver repeats fast after first
    // unfortunately can't spot difference from of repeat from two fast clicks
    if (results->rawbuf[offset] < SONY_DOUBLE_SPACE_USECS)
    {
        // Serial_print("IR Gap found: ");
        results->bits = 0;
        results->value = REPEAT;
        results->decode_type = SANYO;
        return DECODED;
    }
    offset++;

    // Initial mark
    if (!MATCH_MARK(results->rawbuf[offset], SONY_HDR_MARK)) return ERR;
    offset++;

    while (offset + 1 < irparams.rawlen)
    {
        if (!MATCH_SPACE(results->rawbuf[offset], SONY_HDR_SPACE))
            break;

        offset++;

        if (MATCH_MARK(results->rawbuf[offset], SONY_ONE_MARK))
            data = (data << 1) | 1;
        else if (MATCH_MARK(results->rawbuf[offset], SONY_ZERO_MARK))
            data <<= 1;
        else
            return ERR;

        offset++;
    }

    // Success
    results->bits = (offset - 1) / 2;
    if (results->bits < 12)
    {
        results->bits = 0;
        return ERR;
    }
    results->value = data;
    results->decode_type = SONY;
    return DECODED;
}
#endif

#if defined(IRRECV_DECODESANYO) || defined(IRRECV_DECODE)
// I think this is a Sanyo decoder - serial = SA 8650B
// Looks like Sony except for timings, 48 chars of data and time/space different
u32 IRrecv_decodeSanyo(decode_results *results)
{
  u32 data = 0;
  u16 offset = 0; // Skip first space

  if (irparams.rawlen < 2 * SANYO_BITS + 2) {
    return ERR;
  }
  // Initial space  
  /* Put this back in for debugging - note can't use #DEBUG as if Debug on we don't see the repeat cos of the delay
  Serial_print("IR Gap: ");
  Serial_println( results->rawbuf[offset]);
  Serial_println( "test against:");
  Serial_println(results->rawbuf[offset]);
  */
  if (results->rawbuf[offset] < SANYO_DOUBLE_SPACE_USECS) {
    // Serial_print("IR Gap found: ");
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = SANYO;
    return DECODED;
  }
  offset++;

  // Initial mark
  if (!MATCH_MARK(results->rawbuf[offset], SANYO_HDR_MARK)) {
    return ERR;
  }
  offset++;

  // Skip Second Mark
  if (!MATCH_MARK(results->rawbuf[offset], SANYO_HDR_MARK)) {
    return ERR;
  }
  offset++;

  while (offset + 1 < irparams.rawlen) {
    if (!MATCH_SPACE(results->rawbuf[offset], SANYO_HDR_SPACE)) {
      break;
    }
    offset++;
    if (MATCH_MARK(results->rawbuf[offset], SANYO_ONE_MARK)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_MARK(results->rawbuf[offset], SANYO_ZERO_MARK)) {
      data <<= 1;
    } 
    else {
      return ERR;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < 12) {
    results->bits = 0;
    return ERR;
  }
  results->value = data;
  results->decode_type = SANYO;
  return DECODED;
}
#endif

#if defined(IRRECV_DECODEMITSUBISHI) || defined(IRRECV_DECODE)
// Looks like Sony except for timings, 48 chars of data and time/space different
u32 IRrecv_decodeMitsubishi(decode_results *results)
{
  // Serial_print("?!? decoding Mitsubishi:");Serial.print(irparams.rawlen); Serial.print(" want "); Serial.println( 2 * MITSUBISHI_BITS + 2);
  u32 data = 0;
  u16 offset = 0; // Skip first space

  if (irparams.rawlen < 2 * MITSUBISHI_BITS + 2) {
    return ERR;
  }
  // Initial space  
  /* Put this back in for debugging - note can't use #DEBUG as if Debug on we don't see the repeat cos of the delay
  Serial_print("IR Gap: ");
  Serial_println( results->rawbuf[offset]);
  Serial_println( "test against:");
  Serial_println(results->rawbuf[offset]);
  */
  /* Not seeing double keys from Mitsubishi
  if (results->rawbuf[offset] < MITSUBISHI_DOUBLE_SPACE_USECS) {
    // Serial_print("IR Gap found: ");
    results->bits = 0;
    results->value = REPEAT;
    results->decode_type = MITSUBISHI;
    return DECODED;
  }
  */
  offset++;

  // Typical
  // 14200 7 41 7 42 7 42 7 17 7 17 7 18 7 41 7 18 7 17 7 17 7 18 7 41 8 17 7 17 7 18 7 17 7 

  // Initial Space
  if (!MATCH_MARK(results->rawbuf[offset], MITSUBISHI_HDR_SPACE)) {
    return ERR;
  }
  offset++;
  while (offset + 1 < irparams.rawlen) {
    if (MATCH_MARK(results->rawbuf[offset], MITSUBISHI_ONE_MARK)) {
      data = (data << 1) | 1;
    } 
    else if (MATCH_MARK(results->rawbuf[offset], MITSUBISHI_ZERO_MARK)) {
      data <<= 1;
    } 
    else {
      // Serial_println("A"); Serial.println(offset); Serial_println(results->rawbuf[offset]);
      return ERR;
    }
    offset++;
    if (!MATCH_SPACE(results->rawbuf[offset], MITSUBISHI_HDR_SPACE)) {
      // Serial_println("B"); Serial.println(offset); Serial_println(results->rawbuf[offset]);
      break;
    }
    offset++;
  }

  // Success
  results->bits = (offset - 1) / 2;
  if (results->bits < MITSUBISHI_BITS) {
    results->bits = 0;
    return ERR;
  }
  results->value = data;
  results->decode_type = MITSUBISHI;
  return DECODED;
}
#endif

#if defined(IRRECV_DECODERC5) || defined(IRRECV_DECODERC6) || defined(IRRECV_DECODE)

// Gets one undecoded level at a time from the raw buffer.
// The RC5/6 decoding is easier if the data is broken into time intervals.
// E.g. if the buffer has MARK for 2 time intervals and SPACE for 1,
// successive calls to getRClevel will return MARK, MARK, SPACE.
// offset and used are updated to keep track of the current position.
// t1 is the time interval for a single bit in microseconds.
// Returns -1 for error (measured time interval is not a multiple of t1).
u16 IRrecv_getRClevel(decode_results *results, u16 *offset, u16 *used, u16 t1)
{
  u16 width, val, correction, avail;

  if (*offset >= results->rawlen) {
    // After end of recorded buffer, assume SPACE.
    return SPACE;
  }
  width = results->rawbuf[*offset];
  val = ((*offset) % 2) ? MARK : SPACE;
  correction = (val == MARK) ? MARK_EXCESS : - MARK_EXCESS;

  if (MATCH(width, t1 + correction)) {
    avail = 1;
  } 
  else if (MATCH(width, 2*t1 + correction)) {
    avail = 2;
  } 
  else if (MATCH(width, 3*t1 + correction)) {
    avail = 3;
  } 
  else {
    return -1;
  }

  (*used)++;
  if (*used >= avail) {
    *used = 0;
    (*offset)++;
  }
#ifdef DEBUG
  if (val == MARK) {
    Serial_println("MARK");
  } 
  else {
    Serial_println("SPACE");
  }
#endif
  return val;   
}
#endif

#if defined(IRRECV_DECODERC5) || defined(IRRECV_DECODE)
u32 IRrecv_decodeRC5(decode_results *results)
{
    u16 offset = 1; // Skip gap space
    u32 data = 0;
    u16 used = 0;
    u16 nbits;
    u16 levelA, levelB;

    if (irparams.rawlen < MIN_RC5_SAMPLES + 2) return ERR;

    // Get start bits
    if (IRrecv_getRClevel(results, &offset, &used, RC5_T1) != MARK) return ERR;
    if (IRrecv_getRClevel(results, &offset, &used, RC5_T1) != SPACE) return ERR;
    if (IRrecv_getRClevel(results, &offset, &used, RC5_T1) != MARK) return ERR;

    for (nbits = 0; offset < irparams.rawlen; nbits++)
    {
        levelA = IRrecv_getRClevel(results, &offset, &used, RC5_T1); 
        levelB = IRrecv_getRClevel(results, &offset, &used, RC5_T1);

        if (levelA == SPACE && levelB == MARK)
        {
          // 1 bit
          data = (data << 1) | 1;
        } 

        else if (levelA == MARK && levelB == SPACE)
        {
          // zero bit
          data <<= 1;
        } 

        else
        {
          return ERR;
        } 
    }

    // Success
    results->bits = nbits;
    results->value = data;
    results->decode_type = RC5;
    return DECODED;
}
#endif

#if defined(IRRECV_DECODERC6) || defined(IRRECV_DECODE)
u32 IRrecv_decodeRC6(decode_results *results)
{
    u16 offset = 1; // Skip first space
    u32 data = 0;
    u16 used = 0;
    u16 nbits;
    u16 levelA, levelB; // Next two levels

    if (results->rawlen < MIN_RC6_SAMPLES) return ERR;
    // Initial mark
    if (!MATCH_MARK(results->rawbuf[offset], RC6_HDR_MARK)) return ERR;
    offset++;
    if (!MATCH_SPACE(results->rawbuf[offset], RC6_HDR_SPACE)) return ERR;
    offset++;
    // Get start bit (1)
    if (IRrecv_getRClevel(results, &offset, &used, RC6_T1) != MARK) return ERR;
    if (IRrecv_getRClevel(results, &offset, &used, RC6_T1) != SPACE) return ERR;

    for (nbits = 0; offset < results->rawlen; nbits++)
    {
        levelA = IRrecv_getRClevel(results, &offset, &used, RC6_T1); 
        if (nbits == 3)
        {
            // T bit is double wide; make sure second half matches
            if (levelA != IRrecv_getRClevel(results, &offset, &used, RC6_T1)) return ERR;
        } 
        levelB = IRrecv_getRClevel(results, &offset, &used, RC6_T1);
        if (nbits == 3)
        {
            // T bit is double wide; make sure second half matches
            if (levelB != IRrecv_getRClevel(results, &offset, &used, RC6_T1)) return ERR;
        } 
        if (levelA == MARK && levelB == SPACE)
        {
            // reversed compared to RC5
            // 1 bit
            data = (data << 1) | 1;
        } 
        else if (levelA == SPACE && levelB == MARK)
        {
            // zero bit
            data <<= 1;
        } 
        else
        {
            return ERR; // Error
        } 
    }
    // Success
    results->bits = nbits;
    results->value = data;
    results->decode_type = RC6;
    return DECODED;
}
#endif

#if defined(IRRECV_DECODEPANASONIC) || defined(IRRECV_DECODE)
u32 IRrecv_decodePanasonic(decode_results *results)
{
    //u32 long data = 0; // PIC18F don't have 64-bit type
    u32 data = 0;
    u16 offset = 1;
    u16 i;
	
    if (!MATCH_MARK(results->rawbuf[offset], PANASONIC_HDR_MARK)) {
        return ERR;
    }
    offset++;
    if (!MATCH_MARK(results->rawbuf[offset], PANASONIC_HDR_SPACE)) {
        return ERR;
    }
    offset++;
    
    // decode address
    for (i = 0; i < PANASONIC_BITS; i++) {
        if (!MATCH_MARK(results->rawbuf[offset++], PANASONIC_BIT_MARK)) {
            return ERR;
        }
        if (MATCH_SPACE(results->rawbuf[offset],PANASONIC_ONE_SPACE)) {
            data = (data << 1) | 1;
        } else if (MATCH_SPACE(results->rawbuf[offset],PANASONIC_ZERO_SPACE)) {
            data <<= 1;
        } else {
            return ERR;
        }
        offset++;
    }
    results->value = (u32)data;
    results->panasonicAddress = (u16)(data >> 32);
    results->decode_type = PANASONIC;
    results->bits = PANASONIC_BITS;
    return DECODED;
}
#endif

#if defined(IRRECV_DECODEJVC) || defined(IRRECV_DECODE)
u32 IRrecv_decodeJVC(decode_results *results)
{
    u32 data = 0;
    u16 offset = 1; // Skip first space
    u16 i;
	// Check for repeat
    if (irparams.rawlen - 1 == 33 &&
        MATCH_MARK(results->rawbuf[offset], JVC_BIT_MARK) &&
        MATCH_MARK(results->rawbuf[irparams.rawlen-1], JVC_BIT_MARK)) {
        results->bits = 0;
        results->value = REPEAT;
        results->decode_type = JVC;
        return DECODED;
    } 
    // Initial mark
    if (!MATCH_MARK(results->rawbuf[offset], JVC_HDR_MARK)) {
        return ERR;
    }
    offset++; 
    if (irparams.rawlen < 2 * JVC_BITS + 1 ) {
        return ERR;
    }
    // Initial space 
    if (!MATCH_SPACE(results->rawbuf[offset], JVC_HDR_SPACE)) {
        return ERR;
    }
    offset++;
    for (i = 0; i < JVC_BITS; i++) {
        if (!MATCH_MARK(results->rawbuf[offset], JVC_BIT_MARK)) {
            return ERR;
        }
        offset++;
        if (MATCH_SPACE(results->rawbuf[offset], JVC_ONE_SPACE)) {
            data = (data << 1) | 1;
        } 
        else if (MATCH_SPACE(results->rawbuf[offset], JVC_ZERO_SPACE)) {
            data <<= 1;
        } 
        else {
            return ERR;
        }
        offset++;
    }
    //Stop bit
    if (!MATCH_MARK(results->rawbuf[offset], JVC_BIT_MARK)){
        return ERR;
    }
    // Success
    results->bits = JVC_BITS;
    results->value = data;
    results->decode_type = JVC;
    return DECODED;
}
#endif

#if defined(IRRECV_COMPARE) || defined(IRRECV_DECODEHASH) || defined(IRRECV_DECODE)
/* -----------------------------------------------------------------------
 * hashdecode - decode an arbitrary IR code.
 * Instead of decoding using a standard encoding scheme
 * (e.g. Sony, NEC, RC5), the code is hashed to a 32-bit value.
 *
 * The algorithm: look at the sequence of MARK signals, and see if each one
 * is shorter (0), the same length (1), or longer (2) than the previous.
 * Do the same with the SPACE signals.  Hszh the resulting sequence of 0's,
 * 1's, and 2's to a 32-bit value.  This will give a unique value for each
 * different code (probably), for most code systems.
 *
 * http://arcfn.com/2010/01/using-arbitrary-remotes-with-arduino.html
 */

// Compare two tick values, returning 0 if newval is shorter,
// 1 if newval is equal, and 2 if newval is longer
// Use a tolerance of 20%
u16 IRrecv_compare(u16 oldval, u16 newval)
{
  if (newval < oldval * .8) {
    return 0;
  } 
  else if (oldval < newval * .8) {
    return 2;
  } 
  else {
    return 1;
  }
}
#endif

#if defined(IRRECV_DECODEHASH) || defined(IRRECV_DECODE)
// Use FNV hash algorithm: http://isthe.com/chongo/tech/comp/fnv/#FNV-param
#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261

/* Converts the raw code values into a 32-bit hash code.
 * Hopefully this code is unique for each button.
 * This isn't a "real" decoding, just an arbitrary value.
 */
u32 IRrecv_decodeHash(decode_results *results)
{
  u16 i;
  u16 value;
  u32 hash;
  
  // Require at least 6 samples to prevent triggering on noise
  if (results->rawlen < 6) {
    return ERR;
  }
  hash = FNV_BASIS_32;
  
  for (i = 1; i+2 < results->rawlen; i++) {
    value =  IRrecv_compare(results->rawbuf[i], results->rawbuf[i+2]);
    // Add value into the hash
    hash = (hash * FNV_PRIME_32) ^ value;
  }
  results->value = hash;
  results->bits = 32;
  results->decode_type = UNKNOWN;
  return DECODED;
}
#endif

/* Sharp and DISH support by Todd Treece ( http://unionbridge.org/design/ircommand )

The Dish send function needs to be repeated 4 times, and the Sharp function
has the necessary repeat built in because of the need to invert the signal.

Sharp protocol documentation:
http://www.sbprojects.com/knowledge/ir/sharp.htm

Here are the LIRC files that I found that seem to match the remote codes
from the oscilloscope:

Sharp LCD TV:
http://lirc.sourceforge.net/remotes/sharp/GA538WJSA

DISH NETWORK (echostar 301):
http://lirc.sourceforge.net/remotes/echostar/301_501_3100_5100_58xx_59xx

For the DISH codes, only send the last for characters of the hex.
i.e. use 0x1C10 instead of 0x0000000000001C10 which is listed in the
linked LIRC file.
*/

#if defined(IRSEND_SENDSHARP)
void IRsend_sendSharp(u32 data, u16 nbits)
{
  u32 invertdata = data ^ SHARP_TOGGLE_MASK;
  u16 i;
  PWM_setFrequency(38000);
  
  for (i = 0; i < nbits; i++) {
    if (data & 0x4000) {
      IRsend_mark(SHARP_BIT_MARK);
      IRsend_space(SHARP_ONE_SPACE);
    }
    else {
      IRsend_mark(SHARP_BIT_MARK);
      IRsend_space(SHARP_ZERO_SPACE);
    }
    data <<= 1;
  }
  
  IRsend_mark(SHARP_BIT_MARK);
  IRsend_space(SHARP_ZERO_SPACE);
  Delayms(46);
  
  for (i = 0; i < nbits; i++) {
    if (invertdata & 0x4000) {
      IRsend_mark(SHARP_BIT_MARK);
      IRsend_space(SHARP_ONE_SPACE);
    }
    else {
      IRsend_mark(SHARP_BIT_MARK);
      IRsend_space(SHARP_ZERO_SPACE);
    }
    invertdata <<= 1;
  }
  IRsend_mark(SHARP_BIT_MARK);
  IRsend_space(SHARP_ZERO_SPACE);
  Delayms(46);
}
#endif

#if defined(IRSEND_SENDDISH)
void IRsend_sendDISH(u32 data, u16 nbits)
{
  u16 i;
  PWM_setFrequency(56000);
  IRsend_mark(DISH_HDR_MARK);
  IRsend_space(DISH_HDR_SPACE);
  
  for (i = 0; i < nbits; i++) {
    if (data & DISH_TOP_BIT) {
      IRsend_mark(DISH_BIT_MARK);
      IRsend_space(DISH_ONE_SPACE);
    }
    else {
      IRsend_mark(DISH_BIT_MARK);
      IRsend_space(DISH_ZERO_SPACE);
    }
    data <<= 1;
  }
}
#endif

/*
void delay50us()
{
    __asm

        ; Delay = 600 instruction cycles
        ; Clock frequency = 48 MHz

        ; Actual delay = 5e-005 seconds = 600 cycles
        ; Error = 0 %

                ;598 cycles
        movlw   0xC7
        movwf   _d1
    Delay_0:
        decfsz	_d1, f
        goto    Delay_0

                ;2 cycles
        goto    $+1

     __endasm;
}
*/

#endif /* IRREMOTE_C */
