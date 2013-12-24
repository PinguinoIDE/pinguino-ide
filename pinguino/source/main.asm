;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.2.1 #8447 (Jul 12 2013) (Linux)
; This file was generated Fri Dec 20 11:30:45 2013
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f2550
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
	extern	_PORTEbits
	extern	_LATAbits
	extern	_LATBbits
	extern	_LATCbits
	extern	_DDRAbits
	extern	_TRISAbits
	extern	_DDRBbits
	extern	_TRISBbits
	extern	_DDRCbits
	extern	_TRISCbits
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
	extern	_PORTE
	extern	_LATA
	extern	_LATB
	extern	_LATC
	extern	_DDRA
	extern	_TRISA
	extern	_DDRB
	extern	_TRISB
	extern	_DDRC
	extern	_TRISC
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
;	.line	112; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	if (RCONbits.NOT_POR == 0)
	BTFSC	_RCONbits, 1
	BRA	_00297_DS_
;	.line	114; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	RCON |= 0b10010011;     // set all reset flag
	MOVLW	0x93
	IORWF	_RCON, F
_00297_DS_:
;	.line	160; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	INTCONbits.GIEH     = 0;        // Disables all HP interrupts
	BCF	_INTCONbits, 7
;	.line	161; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	INTCONbits.GIEL     = 0;        // Disables all LP interrupts
	BCF	_INTCONbits, 6
;	.line	206; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	IO_init();
	CALL	_IO_init
;	.line	207; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	IO_digital();
	CALL	_IO_digital
;	.line	254; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	setup();
	CALL	_setup
_00299_DS_:
;	.line	275; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	loop();
	CALL	_loop
	BRA	_00299_DS_
	RETURN	

; ; Starting pCode block
S_main__epapout_init	code
_epapout_init:
;	.line	84; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	void epapout_init() { return; }
	RETURN	

; ; Starting pCode block
S_main__epapin_init	code
_epapin_init:
;	.line	83; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	void epapin_init()  { return; }
	RETURN	

; ; Starting pCode block
S_main__epap_out	code
_epap_out:
;	.line	82; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	void epap_out()     { return; }
	RETURN	

; ; Starting pCode block
S_main__epap_in	code
_epap_in:
;	.line	81; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/main.c	void epap_in()      { return; }
	RETURN	

; ; Starting pCode block
S_main__loop	code
_loop:
	BANKSEL	___i__
;	.line	17; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/user.c	for(__i__=0;__i__<10;__i__=__i__+1)
	CLRF	___i__, B
; removed redundant BANKSEL
	CLRF	(___i__ + 1), B
_00261_DS_:
;	.line	19; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/user.c	toggle(13);
	MOVLW	0x0d
	MOVWF	POSTDEC1
	CALL	_toggle
	MOVF	POSTINC1, F
;	.line	20; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/user.c	Delayms(100);
	CLRF	POSTDEC1
	MOVLW	0x64
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	BANKSEL	___i__
;	.line	17; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/user.c	for(__i__=0;__i__<10;__i__=__i__+1)
	INCFSZ	___i__, F, B
	BRA	_10296_DS_
; removed redundant BANKSEL
	INCF	(___i__ + 1), F, B
_10296_DS_:
	BANKSEL	(___i__ + 1)
	MOVF	(___i__ + 1), W, B
	ADDLW	0x80
	ADDLW	0x80
	BNZ	_00271_DS_
	MOVLW	0x0a
; removed redundant BANKSEL
	SUBWF	___i__, W, B
_00271_DS_:
	BNC	_00261_DS_
;	.line	22; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/user.c	Delayms(1000);
	MOVLW	0x03
	MOVWF	POSTDEC1
	MOVLW	0xe8
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	RETURN	

; ; Starting pCode block
S_main__setup	code
_setup:
;	.line	13; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/source/user.c	pinmode(13,OUTPUT);
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
;	.line	109; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	ADCON1 = 0x0F;				// AN0 to AN12 Digital I/O
	MOVLW	0x0f
	MOVWF	_ADCON1
;	.line	110; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	CMCON = 0x07;               // Comparators as Digital I/O
	MOVLW	0x07
	MOVWF	_CMCON
	RETURN	

; ; Starting pCode block
S_main__IO_init	code
_IO_init:
;	.line	46; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATA  = 0x00;
	CLRF	_LATA
;	.line	47; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATB  = 0x00;
	CLRF	_LATB
;	.line	64; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATC  = 0x40;	
	MOVLW	0x40
	MOVWF	_LATC
;	.line	75; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISA = 0x00;
	CLRF	_TRISA
;	.line	77; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISB = 0x00;
	CLRF	_TRISB
;	.line	79; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC0 = 0x00;
	BCF	_TRISCbits, 0
;	.line	80; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC1 = 0x00;
	BCF	_TRISCbits, 1
;	.line	81; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC2 = 0x00;
	BCF	_TRISCbits, 2
;	.line	82; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC6 = 0x00;
	BCF	_TRISCbits, 6
;	.line	83; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC7 = 0x00;
	BCF	_TRISCbits, 7
;	.line	92; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTA  = 0x00;
	CLRF	_PORTA
;	.line	93; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTB  = 0x00;
	CLRF	_PORTB
;	.line	94; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTC  = 0x00;
	CLRF	_PORTC
	RETURN	

; ; Starting pCode block
S_main__toggle	code
_toggle:
;	.line	333; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	void toggle(unsigned char pin)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	344; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	state = digitalread(pin);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_digitalread
	MOVWF	r0x01
	MOVF	POSTINC1, F
;	.line	345; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	digitalwrite(pin, state^1);
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
;	.line	289; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	void pinmode(unsigned char pin, unsigned char state)
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
;	.line	291; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
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
	MOVLW	0x03
	SUBWF	r0x02, W
	BTFSC	STATUS, 0
	BRA	_00220_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x02, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_00235_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_00235_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_00235_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_00235_DS_:
	GOTO	_00207_DS_
	GOTO	_00211_DS_
	GOTO	_00215_DS_
_00207_DS_:
;	.line	293; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pA: if (state) TRISA=TRISA | mask[pin];
	MOVF	r0x01, W
	BZ	_00209_DS_
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
	BRA	_00220_DS_
_00209_DS_:
;	.line	294; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISA=TRISA & (255-mask[pin]);
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
;	.line	295; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00220_DS_
_00211_DS_:
;	.line	296; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pB: if (state) TRISB=TRISB | mask[pin];
	MOVF	r0x01, W
	BZ	_00213_DS_
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
	BRA	_00220_DS_
_00213_DS_:
;	.line	297; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISB=TRISB & (255-mask[pin]);
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
;	.line	298; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00220_DS_
_00215_DS_:
;	.line	299; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pC: if (state) TRISC=TRISC | mask[pin];
	MOVF	r0x01, W
	BZ	_00217_DS_
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
	IORWF	_TRISC, F
	BRA	_00220_DS_
_00217_DS_:
;	.line	300; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISC=TRISC & (255-mask[pin]);
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
	ANDWF	_TRISC, F
_00220_DS_:
;	.line	312; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	}
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
;	.line	246; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	unsigned char digitalread(unsigned char pin)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	248; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
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
	MOVLW	0x03
	SUBWF	r0x01, W
	BTFSC	STATUS, 0
	BRA	_00186_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x01, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_00202_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_00202_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_00202_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_00202_DS_:
	GOTO	_00174_DS_
	GOTO	_00178_DS_
	GOTO	_00182_DS_
_00174_DS_:
;	.line	250; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pA: if ((PORTA & mask[pin])!=0) return (1);
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
	BZ	_00176_DS_
	MOVLW	0x01
	BRA	_00187_DS_
_00176_DS_:
;	.line	251; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00187_DS_
_00178_DS_:
;	.line	253; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pB: if ((PORTB & mask[pin])!=0) return (1);
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
	BZ	_00180_DS_
	MOVLW	0x01
	BRA	_00187_DS_
_00180_DS_:
;	.line	254; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00187_DS_
_00182_DS_:
;	.line	256; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pC: if ((PORTC & mask[pin])!=0) return (1);
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
	MOVF	_PORTC, W
	ANDWF	r0x00, F
	MOVF	r0x00, W
	BZ	_00184_DS_
	MOVLW	0x01
	BRA	_00187_DS_
_00184_DS_:
;	.line	257; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_00187_DS_
_00186_DS_:
;	.line	270; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	return (0);
	CLRF	WREG
_00187_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__digitalwrite	code
_digitalwrite:
;	.line	204; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	void digitalwrite(unsigned char pin, unsigned char state)
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
;	.line	206; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
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
	MOVLW	0x03
	SUBWF	r0x02, W
	BTFSC	STATUS, 0
	BRA	_00154_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x02, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_00169_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_00169_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_00169_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_00169_DS_:
	GOTO	_00141_DS_
	GOTO	_00145_DS_
	GOTO	_00149_DS_
_00141_DS_:
;	.line	208; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pA: if (state) LATA=LATA | mask[pin];
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
	BRA	_00154_DS_
_00143_DS_:
;	.line	209; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATA=LATA & (255-mask[pin]);
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
;	.line	210; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00154_DS_
_00145_DS_:
;	.line	211; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pB: if (state) LATB=LATB | mask[pin]; 
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
	BRA	_00154_DS_
_00147_DS_:
;	.line	212; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATB=LATB & (255-mask[pin]);
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
;	.line	213; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_00154_DS_
_00149_DS_:
;	.line	214; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pC: if (state) LATC=LATC | mask[pin];
	MOVF	r0x01, W
	BZ	_00151_DS_
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
	IORWF	_LATC, F
	BRA	_00154_DS_
_00151_DS_:
;	.line	215; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATC=LATC & (255-mask[pin]);
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
	ANDWF	_LATC, F
_00154_DS_:
;	.line	227; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	}
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
;	.line	86; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/delay.c	void Delayus(unsigned int microseconds)
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
;	.line	90; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/delay.c	for (i=0; i<microseconds; i++)
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
;	.line	78; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/delay.c	void Delayms(unsigned int milliseconds)
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
;	.line	82; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/delay.c	for (i=0; i<milliseconds; i++)
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
;	.line	83; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/delay.c	delay1ktcy(12);
	MOVLW	0x0c
	CALL	_delay1ktcy
;	.line	82; /home/yeison/Documentos/Pinguino/pinguino-ide/pinguino/p8/pinguino/core/delay.c	for (i=0; i<milliseconds; i++)
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
	DB	0x04, 0x01, 0x02, 0x04, 0x08, 0x20, 0x10
; ; Starting pCode block for Ival
_port:
	DB	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02
	DB	0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00


; Statistics:
; code size:	 1540 (0x0604) bytes ( 1.17%)
;           	  770 (0x0302) words
; udata size:	    0 (0x0000) bytes ( 0.00%)
; access size:	    5 (0x0005) bytes


	end
