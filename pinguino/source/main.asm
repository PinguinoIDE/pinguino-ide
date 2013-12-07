;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.2.1 #8447 (Jul 12 2013) (Linux)
; This file was generated Sat Dec  7 16:01:14 2013
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f4550
	radix	dec


;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global	___i__
	global	_Delayms
	global	_Delayus
	global	_digitalwrite
	global	_digitalread
	global	_pinmode
	global	_toggle
	global	_IO_init
	global	_IO_digital
	global	_setup
	global	_loop
	global	_epap_in
	global	_epap_out
	global	_epapin_init
	global	_epapout_init
	global	_pinguino_main
	global	_mask
	global	_port

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern	_SPPCFGbits
	extern	_SPPEPSbits
	extern	_SPPCONbits
	extern	_UFRMLbits
	extern	_UFRMHbits
	extern	_UIRbits
	extern	_UIEbits
	extern	_UEIRbits
	extern	_UEIEbits
	extern	_USTATbits
	extern	_UCONbits
	extern	_UADDRbits
	extern	_UCFGbits
	extern	_UEP0bits
	extern	_UEP1bits
	extern	_UEP2bits
	extern	_UEP3bits
	extern	_UEP4bits
	extern	_UEP5bits
	extern	_UEP6bits
	extern	_UEP7bits
	extern	_UEP8bits
	extern	_UEP9bits
	extern	_UEP10bits
	extern	_UEP11bits
	extern	_UEP12bits
	extern	_UEP13bits
	extern	_UEP14bits
	extern	_UEP15bits
	extern	_PORTAbits
	extern	_PORTBbits
	extern	_PORTCbits
	extern	_PORTDbits
	extern	_PORTEbits
	extern	_LATAbits
	extern	_LATBbits
	extern	_LATCbits
	extern	_LATDbits
	extern	_LATEbits
	extern	_DDRAbits
	extern	_TRISAbits
	extern	_DDRBbits
	extern	_TRISBbits
	extern	_DDRCbits
	extern	_TRISCbits
	extern	_DDRDbits
	extern	_TRISDbits
	extern	_DDREbits
	extern	_TRISEbits
	extern	_OSCTUNEbits
	extern	_PIE1bits
	extern	_PIR1bits
	extern	_IPR1bits
	extern	_PIE2bits
	extern	_PIR2bits
	extern	_IPR2bits
	extern	_EECON1bits
	extern	_RCSTAbits
	extern	_TXSTAbits
	extern	_T3CONbits
	extern	_CMCONbits
	extern	_CVRCONbits
	extern	_CCP1ASbits
	extern	_ECCP1ASbits
	extern	_CCP1DELbits
	extern	_ECCP1DELbits
	extern	_BAUDCONbits
	extern	_BAUDCTLbits
	extern	_CCP2CONbits
	extern	_CCP1CONbits
	extern	_ECCP1CONbits
	extern	_ADCON2bits
	extern	_ADCON1bits
	extern	_ADCON0bits
	extern	_SSPCON2bits
	extern	_SSPCON1bits
	extern	_SSPSTATbits
	extern	_T2CONbits
	extern	_T1CONbits
	extern	_RCONbits
	extern	_WDTCONbits
	extern	_HLVDCONbits
	extern	_LVDCONbits
	extern	_OSCCONbits
	extern	_T0CONbits
	extern	_STATUSbits
	extern	_INTCON3bits
	extern	_INTCON2bits
	extern	_INTCONbits
	extern	_STKPTRbits
	extern	_SPPDATA
	extern	_SPPCFG
	extern	_SPPEPS
	extern	_SPPCON
	extern	_UFRM
	extern	_UFRML
	extern	_UFRMH
	extern	_UIR
	extern	_UIE
	extern	_UEIR
	extern	_UEIE
	extern	_USTAT
	extern	_UCON
	extern	_UADDR
	extern	_UCFG
	extern	_UEP0
	extern	_UEP1
	extern	_UEP2
	extern	_UEP3
	extern	_UEP4
	extern	_UEP5
	extern	_UEP6
	extern	_UEP7
	extern	_UEP8
	extern	_UEP9
	extern	_UEP10
	extern	_UEP11
	extern	_UEP12
	extern	_UEP13
	extern	_UEP14
	extern	_UEP15
	extern	_PORTA
	extern	_PORTB
	extern	_PORTC
	extern	_PORTD
	extern	_PORTE
	extern	_LATA
	extern	_LATB
	extern	_LATC
	extern	_LATD
	extern	_LATE
	extern	_DDRA
	extern	_TRISA
	extern	_DDRB
	extern	_TRISB
	extern	_DDRC
	extern	_TRISC
	extern	_DDRD
	extern	_TRISD
	extern	_DDRE
	extern	_TRISE
	extern	_OSCTUNE
	extern	_PIE1
	extern	_PIR1
	extern	_IPR1
	extern	_PIE2
	extern	_PIR2
	extern	_IPR2
	extern	_EECON1
	extern	_EECON2
	extern	_EEDATA
	extern	_EEADR
	extern	_RCSTA
	extern	_TXSTA
	extern	_TXREG
	extern	_RCREG
	extern	_SPBRG
	extern	_SPBRGH
	extern	_T3CON
	extern	_TMR3
	extern	_TMR3L
	extern	_TMR3H
	extern	_CMCON
	extern	_CVRCON
	extern	_CCP1AS
	extern	_ECCP1AS
	extern	_CCP1DEL
	extern	_ECCP1DEL
	extern	_BAUDCON
	extern	_BAUDCTL
	extern	_CCP2CON
	extern	_CCPR2
	extern	_CCPR2L
	extern	_CCPR2H
	extern	_CCP1CON
	extern	_ECCP1CON
	extern	_CCPR1
	extern	_CCPR1L
	extern	_CCPR1H
	extern	_ADCON2
	extern	_ADCON1
	extern	_ADCON0
	extern	_ADRES
	extern	_ADRESL
	extern	_ADRESH
	extern	_SSPCON2
	extern	_SSPCON1
	extern	_SSPSTAT
	extern	_SSPADD
	extern	_SSPBUF
	extern	_T2CON
	extern	_PR2
	extern	_TMR2
	extern	_T1CON
	extern	_TMR1
	extern	_TMR1L
	extern	_TMR1H
	extern	_RCON
	extern	_WDTCON
	extern	_HLVDCON
	extern	_LVDCON
	extern	_OSCCON
	extern	_T0CON
	extern	_TMR0
	extern	_TMR0L
	extern	_TMR0H
	extern	_STATUS
	extern	_FSR2L
	extern	_FSR2H
	extern	_PLUSW2
	extern	_PREINC2
	extern	_POSTDEC2
	extern	_POSTINC2
	extern	_INDF2
	extern	_BSR
	extern	_FSR1L
	extern	_FSR1H
	extern	_PLUSW1
	extern	_PREINC1
	extern	_POSTDEC1
	extern	_POSTINC1
	extern	_INDF1
	extern	_WREG
	extern	_FSR0L
	extern	_FSR0H
	extern	_PLUSW0
	extern	_PREINC0
	extern	_POSTDEC0
	extern	_POSTINC0
	extern	_INDF0
	extern	_INTCON3
	extern	_INTCON2
	extern	_INTCON
	extern	_PROD
	extern	_PRODL
	extern	_PRODH
	extern	_TABLAT
	extern	_TBLPTR
	extern	_TBLPTRL
	extern	_TBLPTRH
	extern	_TBLPTRU
	extern	_PC
	extern	_PCL
	extern	_PCLATH
	extern	_PCLATU
	extern	_STKPTR
	extern	_TOS
	extern	_TOSL
	extern	_TOSH
	extern	_TOSU
	extern	_delay1ktcy

;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
PCL	equ	0xff9
PCLATH	equ	0xffa
PCLATU	equ	0xffb
WREG	equ	0xfe8
TBLPTRL	equ	0xff6
TBLPTRH	equ	0xff7
TBLPTRU	equ	0xff8
TABLAT	equ	0xff5
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
POSTINC1	equ	0xfe6
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW2	equ	0xfdb


	idata
___i__	db	0x00, 0x00


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1
r0x04	res	1

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; I code from now on!
; ; Starting pCode block
S_main__pinguino_main	code
_pinguino_main:
;	.line	111; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	if (RCONbits.NOT_POR == 0)
	BTFSC	_RCONbits, 1
	BRA	_00328_DS_
;	.line	113; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	RCON |= 0b10010011;     // set all reset flag
	MOVLW	0x93
	IORWF	_RCON, F
_00328_DS_:
;	.line	136; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	if (OSCCONbits.SCS > 0x02)
	MOVF	_OSCCONbits, W
	ANDLW	0x03
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	ADDLW	0x80
	ADDLW	0x7d
	BNC	_00333_DS_
_00329_DS_:
;	.line	138; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	while (!OSCCONbits.IOFS);
	BTFSS	_OSCCONbits, 2
	BRA	_00329_DS_
_00333_DS_:
;	.line	157; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	INTCONbits.GIEH     = 0;        // Disables all HP interrupts
	BCF	_INTCONbits, 7
;	.line	158; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	INTCONbits.GIEL     = 0;        // Disables all LP interrupts
	BCF	_INTCONbits, 6
;	.line	203; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	IO_init();
	CALL	_IO_init
;	.line	204; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	IO_digital();
	CALL	_IO_digital
;	.line	251; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	setup();
	CALL	_setup
_00335_DS_:
;	.line	273; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	loop();
	CALL	_loop
	BRA	_00335_DS_
	RETURN	

; ; Starting pCode block
S_main__epapout_init	code
_epapout_init:
;	.line	84; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	void epapout_init() { return; }
	RETURN	

; ; Starting pCode block
S_main__epapin_init	code
_epapin_init:
;	.line	83; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	void epapin_init()  { return; }
	RETURN	

; ; Starting pCode block
S_main__epap_out	code
_epap_out:
;	.line	82; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	void epap_out()     { return; }
	RETURN	

; ; Starting pCode block
S_main__epap_in	code
_epap_in:
;	.line	81; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/main.c	void epap_in()      { return; }
	RETURN	

; ; Starting pCode block
S_main__loop	code
_loop:
;	.line	20; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/user.c	digitalwrite(13,HIGH);
	MOVLW	0x01
	MOVWF	POSTDEC1
	MOVLW	0x0d
	MOVWF	POSTDEC1
	CALL	_digitalwrite
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	21; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/user.c	Delayms(100);
	CLRF	POSTDEC1
	MOVLW	0x64
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	22; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/user.c	digitalwrite(13,LOW);
	CLRF	POSTDEC1
	MOVLW	0x0d
	MOVWF	POSTDEC1
	CALL	_digitalwrite
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	23; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/user.c	Delayms(100);
	CLRF	POSTDEC1
	MOVLW	0x64
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	RETURN	

; ; Starting pCode block
S_main__setup	code
_setup:
;	.line	13; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/source/user.c	pinmode(13,OUTPUT);
	CLRF	POSTDEC1
	MOVLW	0x0d
	MOVWF	POSTDEC1
	CALL	_pinmode
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	RETURN	

; ; Starting pCode block
S_main__IO_digital	code
_IO_digital:
;	.line	108; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	ADCON1 = 0x0F;				// AN0 to AN12 Digital I/O
	MOVLW	0x0f
	MOVWF	_ADCON1
;	.line	109; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	CMCON = 0x07;               // Comparators as Digital I/O
	MOVLW	0x07
	MOVWF	_CMCON
	RETURN	

; ; Starting pCode block
S_main__IO_init	code
_IO_init:
;	.line	46; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	LATA  = 0x00;
	CLRF	_LATA
;	.line	47; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	LATB  = 0x00;
	CLRF	_LATB
;	.line	61; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	LATC  = 0x40;	
	MOVLW	0x40
	MOVWF	_LATC
;	.line	68; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	LATD  = 0x00; 
	CLRF	_LATD
;	.line	69; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	LATE  = 0x00; 
	CLRF	_LATE
;	.line	72; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISA = 0x00;
	CLRF	_TRISA
;	.line	74; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISB = 0x00;
	CLRF	_TRISB
;	.line	76; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC0 = 0x00;
	BCF	_TRISCbits, 0
;	.line	77; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC1 = 0x00;
	BCF	_TRISCbits, 1
;	.line	78; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC2 = 0x00;
	BCF	_TRISCbits, 2
;	.line	79; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC6 = 0x00;
	BCF	_TRISCbits, 6
;	.line	80; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC7 = 0x00;
	BCF	_TRISCbits, 7
;	.line	84; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISD = 0x00; 
	CLRF	_TRISD
;	.line	85; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/io.c	TRISE = 0x00; 
	CLRF	_TRISE
	RETURN	

; ; Starting pCode block
S_main__toggle	code
_toggle:
;	.line	312; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	void toggle(unsigned char pin)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	315; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	state = digitalread(pin);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_digitalread
	MOVWF	r0x01
	MOVF	POSTINC1, F
;	.line	316; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	digitalwrite(pin, state^1);
	BTG	r0x01, 0
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_digitalwrite
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pinmode	code
_pinmode:
;	.line	270; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	void pinmode(unsigned char pin, unsigned char state)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	272; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
	MOVLW	LOW(_port)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_port)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_port)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVLW	0x05
	SUBWF	r0x02, W
	BTFSC	STATUS, 0
	BRA	_00256_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x02, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_00277_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_00277_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_00277_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_00277_DS_:
	GOTO	_00235_DS_
	GOTO	_00239_DS_
	GOTO	_00243_DS_
	GOTO	_00247_DS_
	GOTO	_00251_DS_
_00235_DS_:
;	.line	274; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pA: if (state) TRISA=TRISA | mask[pin];
	MOVF	r0x01, W
	BZ	_00237_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_TRISA, F
	BRA	_00256_DS_
_00237_DS_:
;	.line	275; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else TRISA=TRISA & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_TRISA, F
;	.line	276; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00256_DS_
_00239_DS_:
;	.line	277; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pB: if (state) TRISB=TRISB | mask[pin];
	MOVF	r0x01, W
	BZ	_00241_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_TRISB, F
	BRA	_00256_DS_
_00241_DS_:
;	.line	278; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else TRISB=TRISB & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_TRISB, F
;	.line	279; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00256_DS_
_00243_DS_:
;	.line	280; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pC: if (state) TRISC=TRISC | mask[pin];
	MOVF	r0x01, W
	BZ	_00245_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_TRISC, F
	BRA	_00256_DS_
_00245_DS_:
;	.line	281; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else TRISC=TRISC & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_TRISC, F
;	.line	282; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00256_DS_
_00247_DS_:
;	.line	284; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pD: if (state) TRISD=TRISD | mask[pin];
	MOVF	r0x01, W
	BZ	_00249_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_TRISD, F
	BRA	_00256_DS_
_00249_DS_:
;	.line	285; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else TRISD=TRISD & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_TRISD, F
;	.line	286; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00256_DS_
_00251_DS_:
;	.line	287; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pE: if (state) TRISE=TRISE | mask[pin];
	MOVF	r0x01, W
	BZ	_00253_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_mask)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_mask)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVF	r0x01, W
	IORWF	_TRISE, F
	BRA	_00256_DS_
_00253_DS_:
;	.line	288; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else TRISE=TRISE & (255-mask[pin]);
	CLRF	r0x01
	CLRF	r0x02
	MOVLW	LOW(_mask)
	ADDWF	r0x00, F
	MOVLW	HIGH(_mask)
	ADDWFC	r0x01, F
	MOVLW	UPPER(_mask)
	ADDWFC	r0x02, F
	MOVFF	r0x00, TBLPTRL
	MOVFF	r0x01, TBLPTRH
	MOVFF	r0x02, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x00
	MOVF	r0x00, W
	SUBLW	0xff
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	ANDWF	_TRISE, F
_00256_DS_:
;	.line	291; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	}
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__digitalread	code
_digitalread:
;	.line	229; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	unsigned char digitalread(unsigned char pin)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	231; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
	MOVLW	LOW(_port)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_port)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_port)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVLW	0x05
	SUBWF	r0x01, W
	BTFSC	STATUS, 0
	BRA	_00208_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x01, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_00230_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_00230_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_00230_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_00230_DS_:
	GOTO	_00188_DS_
	GOTO	_00192_DS_
	GOTO	_00196_DS_
	GOTO	_00200_DS_
	GOTO	_00204_DS_
_00188_DS_:
;	.line	233; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pA: if ((PORTA & mask[pin])!=0) return (1);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_mask)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_mask)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVF	_PORTA, W
	ANDWF	r0x01, F
	MOVF	r0x01, W
	BZ	_00190_DS_
	MOVLW	0x01
	BRA	_00209_DS_
_00190_DS_:
;	.line	234; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00209_DS_
_00192_DS_:
;	.line	236; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pB: if ((PORTB & mask[pin])!=0) return (1);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_mask)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_mask)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVF	_PORTB, W
	ANDWF	r0x01, F
	MOVF	r0x01, W
	BZ	_00194_DS_
	MOVLW	0x01
	BRA	_00209_DS_
_00194_DS_:
;	.line	237; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00209_DS_
_00196_DS_:
;	.line	239; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pC: if ((PORTC & mask[pin])!=0) return (1);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_mask)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_mask)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVF	_PORTC, W
	ANDWF	r0x01, F
	MOVF	r0x01, W
	BZ	_00198_DS_
	MOVLW	0x01
	BRA	_00209_DS_
_00198_DS_:
;	.line	240; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00209_DS_
_00200_DS_:
;	.line	243; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pD: if ((PORTD & mask[pin])!=0) return (1);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_mask)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_mask)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVF	_PORTD, W
	ANDWF	r0x01, F
	MOVF	r0x01, W
	BZ	_00202_DS_
	MOVLW	0x01
	BRA	_00209_DS_
_00202_DS_:
;	.line	244; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00209_DS_
_00204_DS_:
;	.line	246; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pE: if ((PORTE & mask[pin])!=0) return (1);
	CLRF	r0x01
	CLRF	r0x02
	MOVLW	LOW(_mask)
	ADDWF	r0x00, F
	MOVLW	HIGH(_mask)
	ADDWFC	r0x01, F
	MOVLW	UPPER(_mask)
	ADDWFC	r0x02, F
	MOVFF	r0x00, TBLPTRL
	MOVFF	r0x01, TBLPTRH
	MOVFF	r0x02, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x00
	MOVF	_PORTE, W
	ANDWF	r0x00, F
	MOVF	r0x00, W
	BZ	_00206_DS_
	MOVLW	0x01
	BRA	_00209_DS_
_00206_DS_:
;	.line	247; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00209_DS_
_00208_DS_:
;	.line	251; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	return (0);
	CLRF	WREG
_00209_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__digitalwrite	code
_digitalwrite:
;	.line	189; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	void digitalwrite(unsigned char pin, unsigned char state)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	191; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
	MOVLW	LOW(_port)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_port)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_port)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVLW	0x05
	SUBWF	r0x02, W
	BTFSC	STATUS, 0
	BRA	_00162_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x02, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_00183_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_00183_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_00183_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_00183_DS_:
	GOTO	_00141_DS_
	GOTO	_00145_DS_
	GOTO	_00149_DS_
	GOTO	_00153_DS_
	GOTO	_00157_DS_
_00141_DS_:
;	.line	193; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pA: if (state) LATA=LATA | mask[pin];
	MOVF	r0x01, W
	BZ	_00143_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_LATA, F
	BRA	_00162_DS_
_00143_DS_:
;	.line	194; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else LATA=LATA & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_LATA, F
;	.line	195; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00162_DS_
_00145_DS_:
;	.line	196; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pB: if (state) LATB=LATB | mask[pin]; 
	MOVF	r0x01, W
	BZ	_00147_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_LATB, F
	BRA	_00162_DS_
_00147_DS_:
;	.line	197; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else LATB=LATB & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_LATB, F
;	.line	198; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00162_DS_
_00149_DS_:
;	.line	199; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pC: if (state) LATC=LATC | mask[pin];
	MOVF	r0x01, W
	BZ	_00151_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_LATC, F
	BRA	_00162_DS_
_00151_DS_:
;	.line	200; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else LATC=LATC & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_LATC, F
;	.line	201; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00162_DS_
_00153_DS_:
;	.line	203; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pD: if (state) LATD=LATD | mask[pin]; 
	MOVF	r0x01, W
	BZ	_00155_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	IORWF	_LATD, F
	BRA	_00162_DS_
_00155_DS_:
;	.line	204; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else LATD=LATD & (255-mask[pin]);
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x02
	CLRF	r0x03
	MOVLW	HIGH(_mask)
	ADDWFC	r0x03, F
	CLRF	r0x04
	MOVLW	UPPER(_mask)
	ADDWFC	r0x04, F
	MOVFF	r0x02, TBLPTRL
	MOVFF	r0x03, TBLPTRH
	MOVFF	r0x04, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x02
	MOVF	r0x02, W
	SUBLW	0xff
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	ANDWF	_LATD, F
;	.line	205; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00162_DS_
_00157_DS_:
;	.line	206; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	case pE: if (state) LATE=LATE | mask[pin]; 
	MOVF	r0x01, W
	BZ	_00159_DS_
	MOVLW	LOW(_mask)
	ADDWF	r0x00, W
	MOVWF	r0x01
	CLRF	r0x02
	MOVLW	HIGH(_mask)
	ADDWFC	r0x02, F
	CLRF	r0x03
	MOVLW	UPPER(_mask)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x01
	MOVF	r0x01, W
	IORWF	_LATE, F
	BRA	_00162_DS_
_00159_DS_:
;	.line	207; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	else LATE=LATE & (255-mask[pin]);
	CLRF	r0x01
	CLRF	r0x02
	MOVLW	LOW(_mask)
	ADDWF	r0x00, F
	MOVLW	HIGH(_mask)
	ADDWFC	r0x01, F
	MOVLW	UPPER(_mask)
	ADDWFC	r0x02, F
	MOVFF	r0x00, TBLPTRL
	MOVFF	r0x01, TBLPTRH
	MOVFF	r0x02, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, r0x00
	MOVF	r0x00, W
	SUBLW	0xff
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	ANDWF	_LATE, F
_00162_DS_:
;	.line	210; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/digitalw.c	}
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__Delayus	code
_Delayus:
;	.line	86; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/delay.c	void Delayus(unsigned int microseconds)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	90; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/delay.c	for (i=0; i<microseconds; i++)
	CLRF	r0x02
	CLRF	r0x03
_00125_DS_:
	MOVF	r0x01, W
	SUBWF	r0x03, W
	BNZ	_00136_DS_
	MOVF	r0x00, W
	SUBWF	r0x02, W
_00136_DS_:
	BC	_00127_DS_
	INFSNZ	r0x02, F
	INCF	r0x03, F
	BRA	_00125_DS_
_00127_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__Delayms	code
_Delayms:
;	.line	78; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/delay.c	void Delayms(unsigned int milliseconds)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	82; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/delay.c	for (i=0; i<milliseconds; i++)
	CLRF	r0x02
	CLRF	r0x03
_00107_DS_:
	MOVF	r0x01, W
	SUBWF	r0x03, W
	BNZ	_00118_DS_
	MOVF	r0x00, W
	SUBWF	r0x02, W
_00118_DS_:
	BC	_00109_DS_
;	.line	83; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/delay.c	delay1ktcy(12);
	MOVLW	0x0c
	CALL	_delay1ktcy
;	.line	82; /home/yeison/Documentos/python/dev/developing/Pinguino11/pinguino/p8/pinguino/core/delay.c	for (i=0; i<milliseconds; i++)
	INFSNZ	r0x02, F
	INCF	r0x03, F
	BRA	_00107_DS_
_00109_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block for Ival
	code
_mask:
	DB	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x40, 0x80, 0x01, 0x02
	DB	0x04, 0x01, 0x02, 0x04, 0x08, 0x20, 0x01, 0x02, 0x04, 0x01, 0x02, 0x04
	DB	0x08, 0x10, 0x20, 0x40, 0x80, 0x10
; ; Starting pCode block for Ival
_port:
	DB	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02
	DB	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x04, 0x04, 0x03, 0x03, 0x03
	DB	0x03, 0x03, 0x03, 0x03, 0x03, 0x00


; Statistics:
; code size:	 2036 (0x07f4) bytes ( 1.55%)
;           	 1018 (0x03fa) words
; udata size:	    0 (0x0000) bytes ( 0.00%)
; access size:	    5 (0x0005) bytes


	end
