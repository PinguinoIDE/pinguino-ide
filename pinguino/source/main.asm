;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.2.1 #8447 (Jul 12 2013) (Linux)
; This file was generated Fri Jan 17 08:51:28 2014
;--------------------------------------------------------
; PIC16 port for the Microchip 16-bit core micros
;--------------------------------------------------------
	list	p=18f47j53
	radix	dec


;--------------------------------------------------------
; public variables in this module
;--------------------------------------------------------
	global	_hidRxLen
	global	_hidProtocol
	global	_hidIdleRate
	global	_HIDPostProcess
	global	_deviceAddress
	global	___uflags
	global	__entry
	global	__startup
	global	_EnableUSBModule
	global	_ProcessUSBTransactions
	global	_ProcessCDCRequest
	global	_CDCgets
	global	_CDCInitEndpoint
	global	_I2C_master
	global	_I2C_slave
	global	_I2C_init
	global	_I2C_write
	global	_I2C_read
	global	_I2C_wait
	global	_I2C_start
	global	_I2C_stop
	global	_I2C_restart
	global	_I2C_sendAck
	global	_I2C_sendNack
	global	_main
	global	_CONTROL_LINE
	global	_deviceState
	global	_remoteWakeup
	global	_selfPowered
	global	_currentConfiguration
	global	_ctrlTransferStage
	global	_requestHandled
	global	_outPtr
	global	_inPtr
	global	_wCount
	global	_ep_bdt
	global	_SetupPacket
	global	_controlTransferBuffer
	global	_CDCControlBuffer
	global	_CDCRxBuffer
	global	_CDCTxBuffer
	global	_line_config
	global	_zlp
	global	__cdc_buffer
	global	_i2cAddr
	global	_MemoryAddr
	global	_ProcessStandardRequest
	global	_InDataStage
	global	_OutDataStage
	global	_SetupStage
	global	_WaitForSetupStage
	global	_ProcessControlTransfer
	global	_UnSuspend
	global	_StartOfFrame
	global	_Stall
	global	_Suspend
	global	_BusReset
	global	_CDCputs
	global	_Delayms
	global	_Delayus
	global	_pprintf
	global	_psprintf2
	global	_psprintf
	global	_CDC_init
	global	_CDCwrite
	global	_CDCprintf
	global	_CDCprint
	global	_CDCprintln
	global	_CDCgetkey
	global	_CDCgetstring
	global	_CDC_interrupt
	global	_digitalwrite
	global	_digitalread
	global	_pinmode
	global	_toggle
	global	_IO_init
	global	_IO_digital
	global	_IO_remap
	global	_eeprom_byte_write
	global	_eeprom_page_write
	global	_eeprom_byte_read
	global	_eeprom_byte_read_seq
	global	_setup
	global	_loop
	global	_high_priority_isr
	global	_low_priority_isr
	global	_libdevice_descriptor
	global	_libconfiguration_descriptor
	global	_lang
	global	_manu
	global	_prod
	global	_libstring_descriptor
	global	_mask
	global	_port

;--------------------------------------------------------
; extern variables in this module
;--------------------------------------------------------
	extern	__gptrget1
	extern	__gptrput1
	extern	__gptrget3
	extern	__gptrput3
	extern	__gptrget4
	extern	__gptrget2
	extern	_ADCTRIGbits
	extern	_PD0bits
	extern	_PMDIS0bits
	extern	_PD1bits
	extern	_PMDIS1bits
	extern	_PD2bits
	extern	_PMDIS2bits
	extern	_PD3bits
	extern	_PMDIS3bits
	extern	_PPSCONbits
	extern	_CCP10CONbits
	extern	_CCP9CONbits
	extern	_CCP8CONbits
	extern	_CCP7CONbits
	extern	_CCP6CONbits
	extern	_CCP5CONbits
	extern	_CCP4CONbits
	extern	_CCP3CONbits
	extern	_ECCP3DELbits
	extern	_ECCP3ASbits
	extern	_PSTR3CONbits
	extern	_T8CONbits
	extern	_T6CONbits
	extern	_T5GCONbits
	extern	_T5CONbits
	extern	_CM3CONbits
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
	extern	_UIEbits
	extern	_UEIEbits
	extern	_UADDRbits
	extern	_UCFGbits
	extern	_PADCFG1bits
	extern	_REFOCONbits
	extern	_RTCCALbits
	extern	_RTCCFGbits
	extern	_ODCON3bits
	extern	_ODCON2bits
	extern	_ODCON1bits
	extern	_ALRMRPTbits
	extern	_ALRMCFGbits
	extern	_ANCON0bits
	extern	_ANCON1bits
	extern	_DSWAKELbits
	extern	_DSWAKEHbits
	extern	_DSCONLbits
	extern	_DSCONHbits
	extern	_CCPTMRS2bits
	extern	_CCPTMRS1bits
	extern	_CCPTMRS0bits
	extern	_CVRCONbits
	extern	_PMSTATLbits
	extern	_PMSTATHbits
	extern	_PMELbits
	extern	_PMEHbits
	extern	_PMMODELbits
	extern	_PMMODEHbits
	extern	_PMCONLbits
	extern	_PMCONHbits
	extern	_UFRMLbits
	extern	_UFRMHbits
	extern	_UIRbits
	extern	_UEIRbits
	extern	_USTATbits
	extern	_UCONbits
	extern	_PMADDRHbits
	extern	_CMSTATbits
	extern	_CMSTATUSbits
	extern	_SSP2CON2bits
	extern	_SSP2CON1bits
	extern	_SSP2STATbits
	extern	_SSP2MSKbits
	extern	_T4CONbits
	extern	_T3CONbits
	extern	_BAUDCON2bits
	extern	_BAUDCONbits
	extern	_BAUDCON1bits
	extern	_BAUDCTLbits
	extern	_PORTAbits
	extern	_PORTBbits
	extern	_PORTCbits
	extern	_PORTDbits
	extern	_PORTEbits
	extern	_HLVDCONbits
	extern	_DMACON2bits
	extern	_OSCCON2bits
	extern	_DMACON1bits
	extern	_LATAbits
	extern	_LATBbits
	extern	_LATCbits
	extern	_LATDbits
	extern	_LATEbits
	extern	_PIE4bits
	extern	_PIR4bits
	extern	_IPR4bits
	extern	_PIE5bits
	extern	_TRISAbits
	extern	_TRISBbits
	extern	_TRISCbits
	extern	_TRISDbits
	extern	_TRISEbits
	extern	_T3GCONbits
	extern	_PIR5bits
	extern	_IPR5bits
	extern	_T1GCONbits
	extern	_OSCTUNEbits
	extern	_RCSTA2bits
	extern	_PIE1bits
	extern	_PIR1bits
	extern	_IPR1bits
	extern	_PIE2bits
	extern	_PIR2bits
	extern	_IPR2bits
	extern	_PIE3bits
	extern	_PIR3bits
	extern	_IPR3bits
	extern	_EECON1bits
	extern	_TXSTA2bits
	extern	_RCSTAbits
	extern	_RCSTA1bits
	extern	_TXSTAbits
	extern	_TXSTA1bits
	extern	_CTMUICONbits
	extern	_CTMUCONLbits
	extern	_CTMUCONHbits
	extern	_CCP2CONbits
	extern	_ECCP2CONbits
	extern	_ECCP2DELbits
	extern	_PWM2CONbits
	extern	_ECCP2ASbits
	extern	_PSTR2CONbits
	extern	_CCP1CONbits
	extern	_ECCP1CONbits
	extern	_ECCP1DELbits
	extern	_PWM1CONbits
	extern	_ECCP1ASbits
	extern	_PSTR1CONbits
	extern	_WDTCONbits
	extern	_ADCON1bits
	extern	_ADCON0bits
	extern	_SSP1CON2bits
	extern	_SSPCON2bits
	extern	_SSP1CON1bits
	extern	_SSPCON1bits
	extern	_SSP1STATbits
	extern	_SSPSTATbits
	extern	_SSP1MSKbits
	extern	_T2CONbits
	extern	_T1CONbits
	extern	_RCONbits
	extern	_CM2CONbits
	extern	_CM2CON1bits
	extern	_CM1CONbits
	extern	_CM1CON1bits
	extern	_OSCCONbits
	extern	_T0CONbits
	extern	_STATUSbits
	extern	_INTCON3bits
	extern	_INTCON2bits
	extern	_INTCONbits
	extern	_STKPTRbits
	extern	_stack_end
	extern	_ADCTRIG
	extern	_PD0
	extern	_PMDIS0
	extern	_PD1
	extern	_PMDIS1
	extern	_PD2
	extern	_PMDIS2
	extern	_PD3
	extern	_PMDIS3
	extern	_PPSCON
	extern	_RPOR0
	extern	_RPOR1
	extern	_RPOR2
	extern	_RPOR3
	extern	_RPOR4
	extern	_RPOR5
	extern	_RPOR6
	extern	_RPOR7
	extern	_RPOR8
	extern	_RPOR9
	extern	_RPOR10
	extern	_RPOR11
	extern	_RPOR12
	extern	_RPOR13
	extern	_RPOR17
	extern	_RPOR18
	extern	_RPOR19
	extern	_RPOR20
	extern	_RPOR21
	extern	_RPOR22
	extern	_RPOR23
	extern	_RPOR24
	extern	_RPINR1
	extern	_RPINR2
	extern	_RPINR3
	extern	_RPINR4
	extern	_RPINR6
	extern	_RPINR15
	extern	_RPINR7
	extern	_RPINR8
	extern	_RPINR9
	extern	_RPINR12
	extern	_RPINR13
	extern	_RPINR14
	extern	_RPINR16
	extern	_RPINR17
	extern	_RPINR21
	extern	_RPINR22
	extern	_RPINR23
	extern	_RPINR24
	extern	_CCP10CON
	extern	_CCPR10L
	extern	_CCPR10H
	extern	_CCP9CON
	extern	_CCPR9L
	extern	_CCPR9H
	extern	_CCP8CON
	extern	_CCPR8L
	extern	_CCPR8H
	extern	_CCP7CON
	extern	_CCPR7L
	extern	_CCPR7H
	extern	_CCP6CON
	extern	_CCPR6L
	extern	_CCPR6H
	extern	_CCP5CON
	extern	_CCPR5L
	extern	_CCPR5H
	extern	_CCP4CON
	extern	_CCPR4L
	extern	_CCPR4H
	extern	_CCP3CON
	extern	_CCPR3L
	extern	_CCPR3H
	extern	_ECCP3DEL
	extern	_ECCP3AS
	extern	_PSTR3CON
	extern	_T8CON
	extern	_PR8
	extern	_TMR8
	extern	_T6CON
	extern	_PR6
	extern	_TMR6
	extern	_T5GCON
	extern	_T5CON
	extern	_TMR5L
	extern	_TMR5H
	extern	_CM3CON
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
	extern	_UIE
	extern	_UEIE
	extern	_UADDR
	extern	_UCFG
	extern	_RTCVALL
	extern	_RTCVALH
	extern	_PADCFG1
	extern	_REFOCON
	extern	_RTCCAL
	extern	_RTCCFG
	extern	_ODCON3
	extern	_ODCON2
	extern	_ODCON1
	extern	_ALRMVALL
	extern	_ALRMVALH
	extern	_ALRMRPT
	extern	_ALRMCFG
	extern	_ANCON0
	extern	_ANCON1
	extern	_DSWAKEL
	extern	_DSWAKEH
	extern	_DSCONL
	extern	_DSCONH
	extern	_DSGPR0
	extern	_DSGPR1
	extern	_CCPTMRS2
	extern	_CCPTMRS1
	extern	_CCPTMRS0
	extern	_CVRCON
	extern	_PMSTATL
	extern	_PMSTATH
	extern	_PMEL
	extern	_PMEH
	extern	_PMDIN2L
	extern	_PMDIN2H
	extern	_PMDOUT2L
	extern	_PMDOUT2H
	extern	_PMMODEL
	extern	_PMMODEH
	extern	_PMCONL
	extern	_PMCONH
	extern	_UFRM
	extern	_UFRML
	extern	_UFRMH
	extern	_UIR
	extern	_UEIR
	extern	_USTAT
	extern	_UCON
	extern	_DMABCH
	extern	_DMABCL
	extern	_RXADDRH
	extern	_RXADDRL
	extern	_TXADDRH
	extern	_TXADDRL
	extern	_PMDIN1L
	extern	_PMDIN1H
	extern	_PMADDRL
	extern	_PMDOUT1L
	extern	_PMADDRH
	extern	_PMDOUT1H
	extern	_CMSTAT
	extern	_CMSTATUS
	extern	_SSP2CON2
	extern	_SSP2CON1
	extern	_SSP2STAT
	extern	_SSP2ADD
	extern	_SSP2MSK
	extern	_SSP2BUF
	extern	_T4CON
	extern	_PR4
	extern	_TMR4
	extern	_T3CON
	extern	_TMR3
	extern	_TMR3L
	extern	_TMR3H
	extern	_BAUDCON2
	extern	_SPBRGH2
	extern	_BAUDCON
	extern	_BAUDCON1
	extern	_BAUDCTL
	extern	_SPBRGH
	extern	_SPBRGH1
	extern	_PORTA
	extern	_PORTB
	extern	_PORTC
	extern	_PORTD
	extern	_PORTE
	extern	_HLVDCON
	extern	_DMACON2
	extern	_OSCCON2
	extern	_DMACON1
	extern	_LATA
	extern	_LATB
	extern	_LATC
	extern	_LATD
	extern	_LATE
	extern	_PIE4
	extern	_PIR4
	extern	_IPR4
	extern	_PIE5
	extern	_TRISA
	extern	_TRISB
	extern	_TRISC
	extern	_TRISD
	extern	_TRISE
	extern	_T3GCON
	extern	_PIR5
	extern	_IPR5
	extern	_T1GCON
	extern	_OSCTUNE
	extern	_RCSTA2
	extern	_PIE1
	extern	_PIR1
	extern	_IPR1
	extern	_PIE2
	extern	_PIR2
	extern	_IPR2
	extern	_PIE3
	extern	_PIR3
	extern	_IPR3
	extern	_EECON1
	extern	_EECON2
	extern	_TXSTA2
	extern	_TXREG2
	extern	_RCREG2
	extern	_SPBRG2
	extern	_RCSTA
	extern	_RCSTA1
	extern	_TXSTA
	extern	_TXSTA1
	extern	_TXREG
	extern	_TXREG1
	extern	_RCREG
	extern	_RCREG1
	extern	_SPBRG
	extern	_SPBRG1
	extern	_CTMUICON
	extern	_CTMUCONL
	extern	_CTMUCONH
	extern	_CCP2CON
	extern	_ECCP2CON
	extern	_CCPR2
	extern	_CCPR2L
	extern	_CCPR2H
	extern	_ECCP2DEL
	extern	_PWM2CON
	extern	_ECCP2AS
	extern	_PSTR2CON
	extern	_CCP1CON
	extern	_ECCP1CON
	extern	_CCPR1
	extern	_CCPR1L
	extern	_CCPR1H
	extern	_ECCP1DEL
	extern	_PWM1CON
	extern	_ECCP1AS
	extern	_PSTR1CON
	extern	_WDTCON
	extern	_ADCON1
	extern	_ADCON0
	extern	_ADRES
	extern	_ADRESL
	extern	_ADRESH
	extern	_SSP1CON2
	extern	_SSPCON2
	extern	_SSP1CON1
	extern	_SSPCON1
	extern	_SSP1STAT
	extern	_SSPSTAT
	extern	_SSP1ADD
	extern	_SSP1MSK
	extern	_SSPADD
	extern	_SSP1BUF
	extern	_SSPBUF
	extern	_T2CON
	extern	_PR2
	extern	_TMR2
	extern	_T1CON
	extern	_TMR1
	extern	_TMR1L
	extern	_TMR1H
	extern	_RCON
	extern	_CM2CON
	extern	_CM2CON1
	extern	_CM1CON
	extern	_CM1CON1
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
	extern	__modulong
	extern	__divulong
	extern	_cinit

;--------------------------------------------------------
;	Equates to used internal registers
;--------------------------------------------------------
STATUS	equ	0xfd8
PCL	equ	0xff9
PCLATH	equ	0xffa
PCLATU	equ	0xffb
INTCON	equ	0xff2
WREG	equ	0xfe8
BSR	equ	0xfe0
TOSL	equ	0xffd
TOSH	equ	0xffe
TOSU	equ	0xfff
TBLPTRL	equ	0xff6
TBLPTRH	equ	0xff7
TBLPTRU	equ	0xff8
TABLAT	equ	0xff5
FSR0L	equ	0xfe9
FSR0H	equ	0xfea
FSR1L	equ	0xfe1
FSR2L	equ	0xfd9
FSR2H	equ	0xfda
INDF0	equ	0xfef
POSTINC0	equ	0xfee
POSTINC1	equ	0xfe6
POSTDEC1	equ	0xfe5
PREINC1	equ	0xfe4
PLUSW2	equ	0xfdb
PRODL	equ	0xff3
PRODH	equ	0xff4


	idata
___uflags	db	0x00
_CONTROL_LINE	db	0x00
_i2cAddr	db	0x50
_MemoryAddr	db	0x00, 0x00


; Internal registers
.registers	udata_ovr	0x0000
r0x00	res	1
r0x01	res	1
r0x02	res	1
r0x03	res	1
r0x04	res	1
r0x05	res	1
r0x06	res	1
r0x07	res	1
r0x08	res	1
r0x09	res	1
r0x0a	res	1
r0x0b	res	1
r0x0c	res	1
r0x0d	res	1
r0x0e	res	1
r0x0f	res	1
r0x10	res	1
r0x11	res	1
r0x12	res	1
r0x13	res	1
r0x14	res	1
r0x15	res	1
r0x16	res	1
r0x17	res	1
r0x18	res	1
r0x19	res	1
r0x1a	res	1
r0x1b	res	1
r0x1c	res	1
r0x1d	res	1
r0x1e	res	1
r0x1f	res	1
r0x20	res	1


usbram5	udata
_SetupPacket	res	64
_controlTransferBuffer	res	64
_CDCRxBuffer	res	64
_CDCTxBuffer	res	64

udata_main_0	udata
_deviceAddress	res	1

udata_main_1	udata
_HIDPostProcess	res	1

udata_main_2	udata
_hidIdleRate	res	1

udata_main_3	udata
_hidProtocol	res	1

udata_main_4	udata
_hidRxLen	res	1

udata_main_5	udata
_pputchar	res	3

udata_main_6	udata
_requestHandled	res	1

udata_main_7	udata
_outPtr	res	3

udata_main_8	udata
_wCount	res	2

udata_main_9	udata
_inPtr	res	3

udata_main_10	udata
_selfPowered	res	1

udata_main_11	udata
_remoteWakeup	res	1

udata_main_12	udata
_deviceState	res	1

udata_main_13	udata
_currentConfiguration	res	1

udata_main_14	udata
_ctrlTransferStage	res	1

udata_main_15	udata
_line_config	res	7

udata_main_16	udata
_zlp	res	8

udata_main_17	udata
_CDCControlBuffer	res	16

udata_main_18	udata
_pprinti_buffer_1_151	res	12

udata_main_19	udata
_pprintfl_buffer_1_161	res	12

udata_main_20	udata
_pprintfl_tmp_1_161	res	12

udata_main_21	udata
_pprintfl_helper_1_161	res	4

udata_main_22	udata
_pprint_scr_1_175	res	2

udata_main_23	udata
_psprintf2_out_1_196	res	3

udata_main_24	udata
_CDCwrite_c_1_203	res	1

udata_main_25	udata
__cdc_buffer	res	64

udata_main_26	udata
_CDCgetkey_buffer_1_213	res	64

udata_main_27	udata
_CDCgetstring_buffer_1_215	res	80


ustat_main_00	udata	0X0D00
_ep_bdt        	res	128

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------

;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
; ; Starting pCode block
S_main___entry	code	0X000C00
__entry:
	goto __startup
	
; ; Starting pCode block for absolute section
; ;-----------------------------------------
S_main_ivec_0x1_high_priority_isr	code	0X000C08
ivec_0x1_high_priority_isr:
	GOTO	_high_priority_isr

; ; Starting pCode block for absolute section
; ;-----------------------------------------
S_main_ivec_0x2_low_priority_isr	code	0X000C18
ivec_0x2_low_priority_isr:
	GOTO	_low_priority_isr

; I code from now on!
; ; Starting pCode block
S_main__main	code
_main:
;	.line	111; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	if (RCONbits.NOT_POR == 0)
	BTFSC	_RCONbits, 1
	BRA	_01880_DS_
;	.line	113; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	RCON |= 0b10010011;     // set all reset flag
	MOVLW	0x93
	IORWF	_RCON, F
_01880_DS_:
;	.line	126; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	OSCTUNEbits.PLLEN = 1;
	BSF	_OSCTUNEbits, 6
;	.line	127; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	while (pll_startup_counter--);
	MOVLW	0x58
	MOVWF	r0x00
	MOVLW	0x02
	MOVWF	r0x01
_01881_DS_:
	MOVFF	r0x00, r0x02
	MOVFF	r0x01, r0x03
	MOVLW	0xff
	ADDWF	r0x00, F
	ADDWFC	r0x01, F
	MOVF	r0x02, W
	IORWF	r0x03, W
	BNZ	_01881_DS_
;	.line	158; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	INTCONbits.GIEH     = 0;        // Disables all HP interrupts
	BCF	_INTCONbits, 7
;	.line	159; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	INTCONbits.GIEL     = 0;        // Disables all LP interrupts
	BCF	_INTCONbits, 6
;	.line	204; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	IO_init();
	CALL	_IO_init
;	.line	205; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	IO_digital();
	CALL	_IO_digital
;	.line	211; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	IO_remap();
	CALL	_IO_remap
;	.line	224; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	CDC_init();
	CALL	_CDC_init
;	.line	252; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	setup();
	CALL	_setup
_01885_DS_:
;	.line	273; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	loop();
	CALL	_loop
	BRA	_01885_DS_
	RETURN	

; ; Starting pCode block
S_main__low_priority_isr	code
_low_priority_isr:
;	.line	371; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	void low_priority_isr(void) __interrupt 2
	MOVFF	STATUS, POSTDEC1
	MOVFF	BSR, POSTDEC1
	MOVWF	POSTDEC1
	MOVFF	PRODL, POSTDEC1
	MOVFF	PRODH, POSTDEC1
	MOVFF	FSR0L, POSTDEC1
	MOVFF	FSR0H, POSTDEC1
	MOVFF	PCLATH, POSTDEC1
	MOVFF	PCLATU, POSTDEC1
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF TBLPTRL, POSTDEC1
	MOVFF TBLPTRH, POSTDEC1
	MOVFF TBLPTRU, POSTDEC1
	MOVFF TABLAT, POSTDEC1
	
	MOVFF PREINC1, TABLAT
	MOVFF PREINC1, TBLPTRU
	MOVFF PREINC1, TBLPTRH
	MOVFF PREINC1, TBLPTRL
	
	MOVFF	PREINC1, FSR2L
	MOVFF	PREINC1, PCLATU
	MOVFF	PREINC1, PCLATH
	MOVFF	PREINC1, FSR0H
	MOVFF	PREINC1, FSR0L
	MOVFF	PREINC1, PRODH
	MOVFF	PREINC1, PRODL
	MOVF	PREINC1, W
	MOVFF	PREINC1, BSR
	MOVFF	PREINC1, STATUS
	RETFIE	

; ; Starting pCode block
S_main__high_priority_isr	code
_high_priority_isr:
;	.line	294; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	void high_priority_isr(void) __interrupt 1
	MOVFF	STATUS, POSTDEC1
	MOVFF	BSR, POSTDEC1
	MOVWF	POSTDEC1
	MOVFF	PRODL, POSTDEC1
	MOVFF	PRODH, POSTDEC1
	MOVFF	FSR0L, POSTDEC1
	MOVFF	FSR0H, POSTDEC1
	MOVFF	PCLATH, POSTDEC1
	MOVFF	PCLATU, POSTDEC1
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF TBLPTRL, POSTDEC1
	MOVFF TBLPTRH, POSTDEC1
	MOVFF TBLPTRU, POSTDEC1
	MOVFF TABLAT, POSTDEC1
	
;	.line	304; /dvpt/pinguino/git/pinguino-ide/pinguino/source/main.c	CDC_interrupt();
	CALL	_CDC_interrupt
	MOVFF PREINC1, TABLAT
	MOVFF PREINC1, TBLPTRU
	MOVFF PREINC1, TBLPTRH
	MOVFF PREINC1, TBLPTRL
	
	MOVFF	PREINC1, FSR2L
	MOVFF	PREINC1, PCLATU
	MOVFF	PREINC1, PCLATH
	MOVFF	PREINC1, FSR0H
	MOVFF	PREINC1, FSR0L
	MOVFF	PREINC1, PRODH
	MOVFF	PREINC1, PRODL
	MOVF	PREINC1, W
	MOVFF	PREINC1, BSR
	MOVFF	PREINC1, STATUS
	RETFIE	

; ; Starting pCode block
S_main__loop	code
_loop:
;	.line	127; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	void loop()
	MOVFF	r0x00, POSTDEC1
;	.line	130; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	digitalwrite(12, HIGH);                     
	MOVLW	0x01
	MOVWF	POSTDEC1
	MOVLW	0x0c
	MOVWF	POSTDEC1
	CALL	_digitalwrite
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	131; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintf("Reading ...\n\r");
	MOVLW	UPPER(__str_15)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_15)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_15)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x03
	ADDWF	FSR1L, F
;	.line	134; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_start();                                
	CALL	_I2C_start
	BANKSEL	(_MemoryAddr + 1)
;	.line	135; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	value = eeprom_byte_read(high8(MemoryAddr), low8(MemoryAddr));
	MOVF	(_MemoryAddr + 1), W, B
	MOVWF	r0x00
; removed redundant BANKSEL
	MOVF	_MemoryAddr, W, B
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_eeprom_byte_read
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	136; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("%d value red from eeprom", value);
	CLRF	POSTDEC1
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_16)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_16)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_16)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x05
	ADDWF	FSR1L, F
;	.line	137; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_stop();                                 
	CALL	_I2C_stop
;	.line	139; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	digitalwrite(12, LOW);                      
	CLRF	POSTDEC1
	MOVLW	0x0c
	MOVWF	POSTDEC1
	CALL	_digitalwrite
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	140; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	Delayms(1000);                                
	MOVLW	0x03
	MOVWF	POSTDEC1
	MOVLW	0xe8
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__setup	code
_setup:
;	.line	117; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	void setup()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
;	.line	119; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	Delayms(10000);
	MOVLW	0x27
	MOVWF	POSTDEC1
	MOVLW	0x10
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	120; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintf("\n\r\n\r*** 24xx512 i2c Eeprom Demo. ***\n\r\n\r");
	MOVLW	UPPER(__str_13)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_13)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_13)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x03
	ADDWF	FSR1L, F
;	.line	121; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	pinmode(12, OUTPUT);                
	CLRF	POSTDEC1
	MOVLW	0x0c
	MOVWF	POSTDEC1
	CALL	_pinmode
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	122; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_master(I2C_100KHZ);             
	CLRF	POSTDEC1
	MOVLW	0x64
	MOVWF	POSTDEC1
	CALL	_I2C_master
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	124; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintf("Writing ...\n\r");
	MOVLW	UPPER(__str_14)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_14)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_14)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x03
	ADDWF	FSR1L, F
	BANKSEL	(_MemoryAddr + 1)
;	.line	125; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	eeprom_byte_write(high8(MemoryAddr), low8(MemoryAddr), 10);
	MOVF	(_MemoryAddr + 1), W, B
	MOVWF	r0x00
; removed redundant BANKSEL
	MOVF	_MemoryAddr, W, B
	MOVWF	r0x01
	MOVLW	0x0a
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_eeprom_byte_write
	MOVLW	0x03
	ADDWF	FSR1L, F
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__eeprom_byte_read_seq	code
_eeprom_byte_read_seq:
;	.line	97; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	void eeprom_byte_read_seq(u8 Hi_addr, u8 Lo_addr, int num_bytes, u8 *read_data){
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVFF	r0x0a, POSTDEC1
	MOVFF	r0x0b, POSTDEC1
	MOVFF	r0x0c, POSTDEC1
	MOVFF	r0x0d, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
	MOVLW	0x06
	MOVFF	PLUSW2, r0x04
	MOVLW	0x07
	MOVFF	PLUSW2, r0x05
	MOVLW	0x08
	MOVFF	PLUSW2, r0x06
;	.line	99; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_start();
	CALL	_I2C_start
	BANKSEL	_i2cAddr
;	.line	100; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write((i2cAddr << 1) + 1);
	RLNCF	_i2cAddr, W, B
	ANDLW	0xfe
	MOVWF	r0x07
	INCF	r0x07, F
	MOVF	r0x07, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	101; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Hi_addr);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	102; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Lo_addr);
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	103; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_start();
	CALL	_I2C_start
;	.line	104; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	for( i = 0 ; i < num_bytes - 1 ; i++) {
	MOVF	r0x02, W
	ADDLW	0xff
	MOVWF	r0x00
	MOVLW	0xff
	ADDWFC	r0x03, W
	MOVWF	r0x01
	CLRF	r0x07
_01849_DS_:
	MOVFF	r0x07, r0x08
	CLRF	r0x09
	MOVF	r0x09, W
	ADDLW	0x80
	MOVWF	PRODL
	MOVF	r0x01, W
	ADDLW	0x80
	SUBWF	PRODL, W
	BNZ	_01863_DS_
	MOVF	r0x00, W
	SUBWF	r0x08, W
_01863_DS_:
	BC	_01847_DS_
;	.line	105; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	read_data[i] = I2C_read();
	MOVF	r0x07, W
	ADDWF	r0x04, W
	MOVWF	r0x0a
	CLRF	WREG
	ADDWFC	r0x05, W
	MOVWF	r0x0b
	CLRF	WREG
	ADDWFC	r0x06, W
	MOVWF	r0x0c
	CALL	_I2C_read
	MOVWF	r0x0d
	MOVFF	r0x0d, POSTDEC1
	MOVFF	r0x0a, FSR0L
	MOVFF	r0x0b, PRODL
	MOVF	r0x0c, W
	CALL	__gptrput1
;	.line	106; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("%u data_read_read ", read_data[i]);
	CLRF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_11)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_11)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_11)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x05
	ADDWF	FSR1L, F
;	.line	107; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	if (i >= num_bytes)                               
	MOVF	r0x09, W
	ADDLW	0x80
	MOVWF	PRODL
	MOVF	r0x03, W
	ADDLW	0x80
	SUBWF	PRODL, W
	BNZ	_01864_DS_
	MOVF	r0x02, W
	SUBWF	r0x08, W
_01864_DS_:
	BNC	_01845_DS_
;	.line	108; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_sendNack();                     
	CALL	_I2C_sendNack
	BRA	_01850_DS_
_01845_DS_:
;	.line	110; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_sendAck();
	CALL	_I2C_sendAck
_01850_DS_:
;	.line	104; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	for( i = 0 ; i < num_bytes - 1 ; i++) {
	INCF	r0x07, F
	BRA	_01849_DS_
_01847_DS_:
;	.line	112; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	read_data[i+1] = I2C_read();
	INFSNZ	r0x08, F
	INCF	r0x09, F
	MOVF	r0x08, W
	ADDWF	r0x04, F
	MOVF	r0x09, W
	ADDWFC	r0x05, F
	CLRF	WREG
	BTFSC	r0x09, 7
	SETF	WREG
	ADDWFC	r0x06, F
	CALL	_I2C_read
	MOVWF	r0x00
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, PRODL
	MOVF	r0x06, W
	CALL	__gptrput1
;	.line	113; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("%u data_read_read_end ", read_data[i+1]);
	CLRF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_12)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_12)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_12)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x05
	ADDWF	FSR1L, F
;	.line	114; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_stop();
	CALL	_I2C_stop
	MOVFF	PREINC1, r0x0d
	MOVFF	PREINC1, r0x0c
	MOVFF	PREINC1, r0x0b
	MOVFF	PREINC1, r0x0a
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__eeprom_byte_read	code
_eeprom_byte_read:
;	.line	84; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	u8 eeprom_byte_read(u8 Hi_addr, u8 Lo_addr)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	87; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_start();
	CALL	_I2C_start
	BANKSEL	_i2cAddr
;	.line	88; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write((i2cAddr << 1) + 1);
	RLNCF	_i2cAddr, W, B
	ANDLW	0xfe
	MOVWF	r0x02
	INCF	r0x02, F
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	89; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Hi_addr);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	90; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Lo_addr);
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	91; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	temp = I2C_read();                  
	CALL	_I2C_read
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
;	.line	92; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCwrite(temp);
	MOVWF	r0x00
	MOVWF	POSTDEC1
	CALL	_CDCwrite
	MOVF	POSTINC1, F
;	.line	93; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_sendNack();                     
	CALL	_I2C_sendNack
;	.line	94; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_stop();
	CALL	_I2C_stop
;	.line	95; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	return temp;
	MOVF	r0x00, W
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__eeprom_page_write	code
_eeprom_page_write:
;	.line	66; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	void eeprom_page_write(u8 Hi_addr, u8 Lo_addr, u8 *data)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
	MOVLW	0x06
	MOVFF	PLUSW2, r0x04
;	.line	69; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("---------------LAST-------------");
	MOVLW	UPPER(__str_8)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_8)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_8)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x03
	ADDWF	FSR1L, F
;	.line	70; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_start();
	CALL	_I2C_start
	BANKSEL	_i2cAddr
;	.line	71; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write((i2cAddr << 1) & 0xFE);            
	RLNCF	_i2cAddr, W, B
	ANDLW	0xfe
	MOVWF	r0x05
	BCF	r0x05, 0
	MOVF	r0x05, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	72; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("--------------------------------");
	MOVLW	UPPER(__str_9)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_9)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_9)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x03
	ADDWF	FSR1L, F
;	.line	73; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Hi_addr);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	74; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Lo_addr);
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	75; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("--------------------------------");
	MOVLW	UPPER(__str_9)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_9)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_9)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x03
	ADDWF	FSR1L, F
;	.line	76; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	for(i = 0; i<128; i++)
	CLRF	r0x00
_01822_DS_:
;	.line	78; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(data[i]);
	MOVF	r0x00, W
	ADDWF	r0x02, W
	MOVWF	r0x01
	CLRF	WREG
	ADDWFC	r0x03, W
	MOVWF	r0x05
	CLRF	WREG
	ADDWFC	r0x04, W
	MOVWF	r0x06
	MOVFF	r0x01, FSR0L
	MOVFF	r0x05, PRODL
	CALL	__gptrget1
; #	MOVWF	r0x07
; #	MOVF	r0x07, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	79; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCprintln("%u data_write ", data[i]);
	MOVFF	r0x01, FSR0L
	MOVFF	r0x05, PRODL
	MOVF	r0x06, W
	CALL	__gptrget1
	CLRF	POSTDEC1
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_10)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_10)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_10)
	MOVWF	POSTDEC1
	CALL	_CDCprintln
	MOVLW	0x05
	ADDWF	FSR1L, F
;	.line	76; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	for(i = 0; i<128; i++)
	INCF	r0x00, F
	MOVLW	0x80
	SUBWF	r0x00, W
	BNC	_01822_DS_
;	.line	81; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_stop();
	CALL	_I2C_stop
;	.line	82; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	Delayms(10); 
	CLRF	POSTDEC1
	MOVLW	0x0a
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__eeprom_byte_write	code
_eeprom_byte_write:
;	.line	55; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	void eeprom_byte_write(u8 Hi_addr, u8 Lo_addr, u8 data) 
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
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
;	.line	57; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_start();
	CALL	_I2C_start
	BANKSEL	_i2cAddr
;	.line	58; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write((i2cAddr << 1) & 0xFE);            
	RLNCF	_i2cAddr, W, B
	ANDLW	0xfe
	MOVWF	r0x03
	BCF	r0x03, 0
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	59; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Hi_addr);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	60; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(Lo_addr);
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	61; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_write(data);
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	CALL	_I2C_write
	MOVF	POSTINC1, F
;	.line	62; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	I2C_stop();
	CALL	_I2C_stop
;	.line	63; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	CDCwrite(data);
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	CALL	_CDCwrite
	MOVF	POSTINC1, F
;	.line	64; /dvpt/pinguino/git/pinguino-ide/pinguino/source/user.c	Delayms(10);
	CLRF	POSTDEC1
	MOVLW	0x0a
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__IO_remap	code
_IO_remap:
;	.line	217; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	EECON2 = 0x55;
	MOVLW	0x55
	MOVWF	_EECON2
;	.line	218; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	EECON2 = 0xAA;
	MOVLW	0xaa
	MOVWF	_EECON2
	BANKSEL	_PPSCONbits
;	.line	219; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PPSCONbits.IOLOCK = 0;			// Turn on PPS Write Protect
	BCF	_PPSCONbits, 0, B
;	.line	221; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	RPINR21 = 6;                    // RP6 (RB3) <- SDI2
	MOVLW	0x06
	BANKSEL	_RPINR21
	MOVWF	_RPINR21, B
;	.line	222; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	RPOR5 = 11;                     // RP5 (RB2) -> SCK2
	MOVLW	0x0b
	BANKSEL	_RPOR5
	MOVWF	_RPOR5, B
;	.line	223; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	RPOR4 = 10;                     // RP4 (RB1) -> SDO2 (func. num. 10)
	MOVLW	0x0a
	BANKSEL	_RPOR4
	MOVWF	_RPOR4, B
;	.line	225; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	EECON2 = 0x55;
	MOVLW	0x55
	MOVWF	_EECON2
;	.line	226; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	EECON2 = 0xAA;
	MOVLW	0xaa
	MOVWF	_EECON2
	BANKSEL	_PPSCONbits
;	.line	227; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PPSCONbits.IOLOCK = 1;			// Turn on PPS Write Protect
	BSF	_PPSCONbits, 0, B
	RETURN	

; ; Starting pCode block
S_main__IO_digital	code
_IO_digital:
	BANKSEL	_ANCON0
;	.line	137; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	ANCON0 = 0xFF;				// AN0 to AN7  Digital I/O
	SETF	_ANCON0, B
;	.line	138; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	ANCON1 = 0x1F;				// AN8 to AN12 Digital I/O
	MOVLW	0x1f
	BANKSEL	_ANCON1
	MOVWF	_ANCON1, B
	RETURN	

; ; Starting pCode block
S_main__IO_init	code
_IO_init:
;	.line	46; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATA  = 0x00;
	CLRF	_LATA
;	.line	47; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATB  = 0x00;
	CLRF	_LATB
;	.line	54; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATC  = 0x00;	
	CLRF	_LATC
;	.line	71; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATD  = 0x00; 
	CLRF	_LATD
;	.line	72; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	LATE  = 0x00; 
	CLRF	_LATE
;	.line	75; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISA = 0x00;
	CLRF	_TRISA
;	.line	77; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISB = 0x00;
	CLRF	_TRISB
;	.line	79; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC0 = 0x00;
	BCF	_TRISCbits, 0
;	.line	80; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC1 = 0x00;
	BCF	_TRISCbits, 1
;	.line	81; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC2 = 0x00;
	BCF	_TRISCbits, 2
;	.line	82; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC6 = 0x00;
	BCF	_TRISCbits, 6
;	.line	83; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISCbits.TRISC7 = 0x00;
	BCF	_TRISCbits, 7
;	.line	87; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISD = 0x00; 
	CLRF	_TRISD
;	.line	88; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	TRISE = 0x00; 
	CLRF	_TRISE
;	.line	92; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTA  = 0x00;
	CLRF	_PORTA
;	.line	93; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTB  = 0x00;
	CLRF	_PORTB
;	.line	94; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTC  = 0x00;
	CLRF	_PORTC
;	.line	98; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTD  = 0x00; 
	CLRF	_PORTD
;	.line	99; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/io.c	PORTE  = 0x00; 
	CLRF	_PORTE
	RETURN	

; ; Starting pCode block
S_main__I2C_sendNack	code
_I2C_sendNack:
;	.line	450; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_sendNack()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
_01790_DS_:
;	.line	452; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01790_DS_
;	.line	453; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.ACKDT = 1;          // We want a No Ack
	BSF	_SSPCON2bits, 5
;	.line	454; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
	BCF	_PIR1bits, 3
;	.line	455; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.ACKEN = 1;          // Send it now
	BSF	_SSPCON2bits, 4
_01793_DS_:
;	.line	456; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
	BTFSS	_PIR1bits, 3
	BRA	_01793_DS_
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__I2C_sendAck	code
_I2C_sendAck:
;	.line	434; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_sendAck()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
_01779_DS_:
;	.line	436; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01779_DS_
;	.line	437; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.ACKDT = 0;          // We want an Ack
	BCF	_SSPCON2bits, 5
;	.line	438; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
	BCF	_PIR1bits, 3
;	.line	439; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.ACKEN = 1;          // Send it now
	BSF	_SSPCON2bits, 4
_01782_DS_:
;	.line	440; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
	BTFSS	_PIR1bits, 3
	BRA	_01782_DS_
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__I2C_restart	code
_I2C_restart:
;	.line	418; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_restart()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
_01768_DS_:
;	.line	420; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01768_DS_
;	.line	422; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.RSEN = 1;           // Send restart bit
	BSF	_SSPCON2bits, 1
_01771_DS_:
;	.line	423; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (SSPCON2bits.RSEN);       // Wait until RSEN is cleared  
	BTFSC	_SSPCON2bits, 1
	BRA	_01771_DS_
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__I2C_stop	code
_I2C_stop:
;	.line	403; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_stop()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
_01757_DS_:
;	.line	405; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01757_DS_
;	.line	407; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.PEN = 1;            // Send stop bit
	BSF	_SSPCON2bits, 2
_01760_DS_:
;	.line	408; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (SSPCON2bits.PEN);        // Wait until PEN is cleared 
	BTFSC	_SSPCON2bits, 2
	BRA	_01760_DS_
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__I2C_start	code
_I2C_start:
;	.line	386; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_start()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
_01746_DS_:
;	.line	388; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01746_DS_
;	.line	390; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.SEN = 1;            // Send start bit
	BSF	_SSPCON2bits, 0
_01749_DS_:
;	.line	391; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (SSPCON2bits.SEN);        // Wait until SEN is cleared 
	BTFSC	_SSPCON2bits, 0
	BRA	_01749_DS_
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__I2C_wait	code
_I2C_wait:
_01738_DS_:
;	.line	332; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (!PIR1bits.SSPIF);        // Wait the interrupt flag is set
	BTFSS	_PIR1bits, 3
	BRA	_01738_DS_
;	.line	333; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF = 0;             // Clear SSP interrupt flag
	BCF	_PIR1bits, 3
	RETURN	

; ; Starting pCode block
S_main__I2C_read	code
_I2C_read:
;	.line	290; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	u8 I2C_read()
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
_01727_DS_:
;	.line	294; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                 // Wait the MSSP module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01727_DS_
;	.line	296; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2bits.RCEN = 1;       // Initiate reception of byte
	BSF	_SSPCON2bits, 3
;	.line	299; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF = 0;         // Clear SSP interrupt flag
	BCF	_PIR1bits, 3
_01730_DS_:
;	.line	303; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	while (!PIR1bits.SSPIF);    // Wait the interrupt flag is set
	BTFSS	_PIR1bits, 3
	BRA	_01730_DS_
;	.line	304; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF=0; // ROlf clear SSPIF
	BCF	_PIR1bits, 3
;	.line	306; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF=0; // ROlf clear SSPIF
	BCF	_PIR1bits, 3
;	.line	311; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	return SSPBUF;
	MOVF	_SSPBUF, W
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__I2C_write	code
_I2C_write:
;	.line	256; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	u8 I2C_write(u8 value)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
_01716_DS_:
;	.line	258; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait the MSSP module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x01
	CLRF	r0x02
	BTFSC	_SSPSTATbits, 2
	INCF	r0x02, F
	MOVF	r0x02, W
	IORWF	r0x01, F
	MOVF	r0x01, W
	BNZ	_01716_DS_
;	.line	259; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPBUF = value;                 // Write byte to SSPBUF (BF is set to 1)
	MOVFF	r0x00, _SSPBUF
_01719_DS_:
;	.line	260; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_idle();                     // Wait the MSSP module is inactive
	MOVLW	0x1f
	ANDWF	_SSPCON2, W
	MOVWF	r0x00
	CLRF	r0x01
	BTFSC	_SSPSTATbits, 2
	INCF	r0x01, F
	MOVF	r0x01, W
	IORWF	r0x00, F
	MOVF	r0x00, W
	BNZ	_01719_DS_
;	.line	271; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	return (!SSPCON2bits.ACKSTAT);  // 1 if Ack, 0 if NAck
	CLRF	r0x00
	BTFSC	_SSPCON2bits, 6
	INCF	r0x00, F
	MOVF	r0x00, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x00
	RLCF	r0x00, F
	MOVF	r0x00, W
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__I2C_init	code
_I2C_init:
;	.line	167; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_init(u8 mode, u16 sora)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
;	.line	172; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	TRISBbits.TRISB5 = INPUT;			// SDA = INPUT
	BSF	_TRISBbits, 5
;	.line	173; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	TRISBbits.TRISB4 = INPUT;			// SCL = INPUT
	BSF	_TRISBbits, 4
;	.line	180; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	switch (mode)
	MOVF	r0x00, W
	BZ	_01674_DS_
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_01674_DS_
;	.line	187; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	if (sora > 0x80)
	MOVLW	0x00
	SUBWF	r0x02, W
	BNZ	_01705_DS_
	MOVLW	0x81
	SUBWF	r0x01, W
_01705_DS_:
	BNC	_01671_DS_
;	.line	188; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON1= 0b00101111;	// Slave mode, 10-bit address with Start and Stop bit interrupts enabled
	MOVLW	0x2f
	MOVWF	_SSPCON1
	BRA	_01672_DS_
_01671_DS_:
;	.line	190; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON1= 0b00101110;	// 00101110Slave mode,  7-bit address with Start and Stop bit interrupts enabled
	MOVLW	0x2e
	MOVWF	_SSPCON1
_01672_DS_:
;	.line	192; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPADD= sora;				// Slave 7-bit address
	MOVF	r0x01, W
	MOVWF	_SSPADD
;	.line	195; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	break;
	BRA	_01680_DS_
_01674_DS_:
;	.line	199; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON1= 0b00101000;		// Master Mode, clock = FOSC/(4 * (SSPADD + 1))
	MOVLW	0x28
	MOVWF	_SSPCON1
;	.line	201; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	switch (sora)
	MOVF	r0x01, W
	XORLW	0x64
	BNZ	_01707_DS_
	MOVF	r0x02, W
	BZ	_01678_DS_
_01707_DS_:
	MOVF	r0x01, W
	XORLW	0x90
	BNZ	_01709_DS_
	MOVF	r0x02, W
	XORLW	0x01
	BZ	_01676_DS_
_01709_DS_:
	MOVF	r0x01, W
	XORLW	0xe8
	BNZ	_01710_DS_
	MOVF	r0x02, W
	XORLW	0x03
	BZ	_01711_DS_
_01710_DS_:
	BRA	_01678_DS_
_01711_DS_:
;	.line	205; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPSTATbits.SMP = 1;    // Slew Mode Off
	BSF	_SSPSTATbits, 7
;	.line	206; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPADD= 11;            // 1MHz = FOSC/(4 * (SSPADD + 1))
	MOVLW	0x0b
	MOVWF	_SSPADD
;	.line	207; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	break;                  // SSPADD = 48 000 / (4*1000) - 1
	BRA	_01680_DS_
_01676_DS_:
;	.line	211; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPSTATbits.SMP = 0;    // Slew Mode On
	BCF	_SSPSTATbits, 7
;	.line	212; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPADD= 29;            // 400kHz = FOSC/(4 * (SSPADD + 1))
	MOVLW	0x1d
	MOVWF	_SSPADD
;	.line	213; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	break;           // SSPADD = 48 000 / (4*400) - 1
	BRA	_01680_DS_
_01678_DS_:
;	.line	217; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPSTATbits.SMP = 1;    // Slew Mode Off
	BSF	_SSPSTATbits, 7
;	.line	218; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPADD= 119;           // 100kHz = FOSC/(4 * (SSPADD + 1))
	MOVLW	0x77
	MOVWF	_SSPADD
_01680_DS_:
;	.line	223; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	SSPCON2= 0;
	CLRF	_SSPCON2
;	.line	224; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR1bits.SSPIF = 0; // MSSP Interrupt Flag
	BCF	_PIR1bits, 3
;	.line	225; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	PIR2bits.BCLIF = 0; // Bus Collision Interrupt Flag
	BCF	_PIR2bits, 3
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__I2C_slave	code
_I2C_slave:
;	.line	149; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_slave(u16 DeviceID)   
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	151; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_init(I2C_SLAVE_MODE, DeviceID);
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x01
	MOVWF	POSTDEC1
	CALL	_I2C_init
	MOVLW	0x03
	ADDWF	FSR1L, F
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__I2C_master	code
_I2C_master:
;	.line	144; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	void I2C_master(u16 speed)   
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
;	.line	146; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/i2c.c	I2C_init(I2C_MASTER_MODE, speed);
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x00
	CLRF	POSTDEC1
	CALL	_I2C_init
	MOVLW	0x03
	ADDWF	FSR1L, F
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__toggle	code
_toggle:
;	.line	351; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	void toggle(unsigned char pin)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	362; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	state = digitalread(pin);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_digitalread
	MOVWF	r0x01
	MOVF	POSTINC1, F
;	.line	363; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	digitalwrite(pin, state^1);
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
;	.line	307; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	void pinmode(unsigned char pin, unsigned char state)
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
;	.line	309; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
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
	BRA	_01628_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x02, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_01649_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_01649_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_01649_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_01649_DS_:
	GOTO	_01607_DS_
	GOTO	_01611_DS_
	GOTO	_01615_DS_
	GOTO	_01619_DS_
	GOTO	_01623_DS_
_01607_DS_:
;	.line	311; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pA: if (state) TRISA=TRISA | mask[pin];
	MOVF	r0x01, W
	BZ	_01609_DS_
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
	BRA	_01628_DS_
_01609_DS_:
;	.line	312; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISA=TRISA & (255-mask[pin]);
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
;	.line	313; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01628_DS_
_01611_DS_:
;	.line	314; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pB: if (state) TRISB=TRISB | mask[pin];
	MOVF	r0x01, W
	BZ	_01613_DS_
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
	BRA	_01628_DS_
_01613_DS_:
;	.line	315; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISB=TRISB & (255-mask[pin]);
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
;	.line	316; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01628_DS_
_01615_DS_:
;	.line	317; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pC: if (state) TRISC=TRISC | mask[pin];
	MOVF	r0x01, W
	BZ	_01617_DS_
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
	BRA	_01628_DS_
_01617_DS_:
;	.line	318; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISC=TRISC & (255-mask[pin]);
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
;	.line	319; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01628_DS_
_01619_DS_:
;	.line	323; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pD: if (state) TRISD=TRISD | mask[pin];
	MOVF	r0x01, W
	BZ	_01621_DS_
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
	BRA	_01628_DS_
_01621_DS_:
;	.line	324; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISD=TRISD & (255-mask[pin]);
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
;	.line	325; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01628_DS_
_01623_DS_:
;	.line	326; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pE: if (state) TRISE=TRISE | mask[pin];
	MOVF	r0x01, W
	BZ	_01625_DS_
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
	BRA	_01628_DS_
_01625_DS_:
;	.line	327; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else TRISE=TRISE & (255-mask[pin]);
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
_01628_DS_:
;	.line	330; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	}
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
;	.line	264; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	unsigned char digitalread(unsigned char pin)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	266; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
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
	BRA	_01580_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x01, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_01602_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_01602_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_01602_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_01602_DS_:
	GOTO	_01560_DS_
	GOTO	_01564_DS_
	GOTO	_01568_DS_
	GOTO	_01572_DS_
	GOTO	_01576_DS_
_01560_DS_:
;	.line	268; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pA: if ((PORTA & mask[pin])!=0) return (1);
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
	BZ	_01562_DS_
	MOVLW	0x01
	BRA	_01581_DS_
_01562_DS_:
;	.line	269; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_01581_DS_
_01564_DS_:
;	.line	271; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pB: if ((PORTB & mask[pin])!=0) return (1);
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
	BZ	_01566_DS_
	MOVLW	0x01
	BRA	_01581_DS_
_01566_DS_:
;	.line	272; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_01581_DS_
_01568_DS_:
;	.line	274; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pC: if ((PORTC & mask[pin])!=0) return (1);
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
	BZ	_01570_DS_
	MOVLW	0x01
	BRA	_01581_DS_
_01570_DS_:
;	.line	275; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_01581_DS_
_01572_DS_:
;	.line	280; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pD: if ((PORTD & mask[pin])!=0) return (1);
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
	BZ	_01574_DS_
	MOVLW	0x01
	BRA	_01581_DS_
_01574_DS_:
;	.line	281; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_01581_DS_
_01576_DS_:
;	.line	283; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pE: if ((PORTE & mask[pin])!=0) return (1);
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
	BZ	_01578_DS_
	MOVLW	0x01
	BRA	_01581_DS_
_01578_DS_:
;	.line	284; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else return (0);
	CLRF	WREG
	BRA	_01581_DS_
_01580_DS_:
;	.line	288; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	return (0);
	CLRF	WREG
_01581_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__digitalwrite	code
_digitalwrite:
;	.line	222; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	void digitalwrite(unsigned char pin, unsigned char state)
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
;	.line	224; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	switch (port[pin])
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
	BRA	_01534_DS_
	CLRF	PCLATH
	CLRF	PCLATU
	RLCF	r0x02, W
	RLCF	PCLATH, F
	RLCF	WREG, W
	RLCF	PCLATH, F
	ANDLW	0xfc
	ADDLW	LOW(_01555_DS_)
	MOVWF	POSTDEC1
	MOVLW	HIGH(_01555_DS_)
	ADDWFC	PCLATH, F
	MOVLW	UPPER(_01555_DS_)
	ADDWFC	PCLATU, F
	MOVF	PREINC1, W
	MOVWF	PCL
_01555_DS_:
	GOTO	_01513_DS_
	GOTO	_01517_DS_
	GOTO	_01521_DS_
	GOTO	_01525_DS_
	GOTO	_01529_DS_
_01513_DS_:
;	.line	226; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pA: if (state) LATA=LATA | mask[pin];
	MOVF	r0x01, W
	BZ	_01515_DS_
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
	BRA	_01534_DS_
_01515_DS_:
;	.line	227; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATA=LATA & (255-mask[pin]);
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
;	.line	228; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01534_DS_
_01517_DS_:
;	.line	229; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pB: if (state) LATB=LATB | mask[pin]; 
	MOVF	r0x01, W
	BZ	_01519_DS_
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
	BRA	_01534_DS_
_01519_DS_:
;	.line	230; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATB=LATB & (255-mask[pin]);
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
;	.line	231; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01534_DS_
_01521_DS_:
;	.line	232; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pC: if (state) LATC=LATC | mask[pin];
	MOVF	r0x01, W
	BZ	_01523_DS_
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
	BRA	_01534_DS_
_01523_DS_:
;	.line	233; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATC=LATC & (255-mask[pin]);
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
;	.line	234; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01534_DS_
_01525_DS_:
;	.line	238; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pD: if (state) LATD=LATD | mask[pin]; 
	MOVF	r0x01, W
	BZ	_01527_DS_
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
	BRA	_01534_DS_
_01527_DS_:
;	.line	239; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATD=LATD & (255-mask[pin]);
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
;	.line	240; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	break;
	BRA	_01534_DS_
_01529_DS_:
;	.line	241; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	case pE: if (state) LATE=LATE | mask[pin]; 
	MOVF	r0x01, W
	BZ	_01531_DS_
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
	BRA	_01534_DS_
_01531_DS_:
;	.line	242; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	else LATE=LATE & (255-mask[pin]);
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
_01534_DS_:
;	.line	245; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/digitalw.c	}
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDC_interrupt	code
_CDC_interrupt:
;	.line	193; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	if(PIR2bits.USBIF)
	BTFSS	_PIR2bits, 4
	BRA	_01508_DS_
;	.line	195; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	PIR2bits.USBIF = 0;
	BCF	_PIR2bits, 4
;	.line	197; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	ProcessUSBTransactions();
	CALL	_ProcessUSBTransactions
;	.line	198; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UIRbits.SOFIF = 0;
	BCF	_UIRbits, 6
;	.line	199; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UIRbits.URSTIF = 0;
	BCF	_UIRbits, 0
;	.line	201; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UEIR = 0;
	CLRF	_UEIR
_01508_DS_:
	RETURN	

; ; Starting pCode block
S_main__CDCgetstring	code
_CDCgetstring:
;	.line	170; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	u8 * CDCgetstring(void)
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
;	.line	175; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	do {
	CLRF	r0x00
_01488_DS_:
;	.line	176; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	c = CDCgetkey();
	CALL	_CDCgetkey
	MOVWF	r0x01
;	.line	177; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%c", c);
	MOVFF	r0x01, r0x02
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_7)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_7)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_7)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x05
	ADDWF	FSR1L, F
;	.line	178; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	buffer[i++] = c;
	MOVFF	r0x00, r0x02
	INCF	r0x00, F
	CLRF	r0x03
	MOVLW	LOW(_CDCgetstring_buffer_1_215)
	ADDWF	r0x02, F
	MOVLW	HIGH(_CDCgetstring_buffer_1_215)
	ADDWFC	r0x03, F
	MOVFF	r0x02, FSR0L
	MOVFF	r0x03, FSR0H
	MOVFF	r0x01, INDF0
;	.line	179; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	} while (c != '\r');
	MOVF	r0x01, W
	XORLW	0x0d
	BNZ	_01488_DS_
;	.line	180; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	buffer[i] = '\0';
	CLRF	r0x01
	MOVLW	LOW(_CDCgetstring_buffer_1_215)
	ADDWF	r0x00, F
	MOVLW	HIGH(_CDCgetstring_buffer_1_215)
	ADDWFC	r0x01, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, FSR0H
	CLRF	INDF0
;	.line	181; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	return buffer;
	MOVWF	r0x01
	MOVLW	0x80
	MOVWF	r0x02
	MOVFF	r0x02, PRODH
	MOVFF	r0x01, PRODL
	MOVLW	LOW(_CDCgetstring_buffer_1_215)
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__CDCgetkey	code
_CDCgetkey:
;	.line	161; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	u8 CDCgetkey(void)
	MOVFF	r0x00, POSTDEC1
_01480_DS_:
;	.line	165; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	while (!CDCgets(buffer));
	MOVLW	0x80
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVLW	HIGH(_CDCgetkey_buffer_1_213)
	MOVWF	POSTDEC1
	MOVLW	LOW(_CDCgetkey_buffer_1_213)
	MOVWF	POSTDEC1
	CALL	_CDCgets
	MOVWF	r0x00
	MOVLW	0x03
	ADDWF	FSR1L, F
	MOVF	r0x00, W
	BZ	_01480_DS_
	BANKSEL	_CDCgetkey_buffer_1_213
;	.line	166; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	return (buffer[0]);	// return only the first character
	MOVF	_CDCgetkey_buffer_1_213, W, B
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__CDCprintln	code
_CDCprintln:
;	.line	149; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	void CDCprintln(const u8 *fmt, ...)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
;	.line	152; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	va_start(args, fmt);					// initialize the list
	MOVLW	0x02
	ADDWF	FSR2L, W
	MOVWF	r0x00
	MOVLW	0x00
	ADDWFC	FSR2H, W
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
; #	MOVWF	r0x01
; #	MOVWF	r0x01
	MOVWF	r0x01
; #	MOVF	r0x00, W
; #	MOVWF	r0x00
	MOVLW	0x80
	MOVWF	r0x02
	MOVLW	0x03
	ADDWF	r0x00, F
	MOVLW	0x00
	ADDWFC	r0x01, F
	ADDWFC	r0x02, F
;	.line	154; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf(fmt, args);
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x03
	MOVLW	0x03
	MOVFF	PLUSW2, r0x04
	MOVLW	0x04
	MOVFF	PLUSW2, r0x05
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x03
	MOVLW	0x03
	MOVFF	PLUSW2, r0x04
	MOVLW	0x04
	MOVFF	PLUSW2, r0x05
	MOVF	r0x05, W
	MOVWF	POSTDEC1
	MOVF	r0x04, W
	MOVWF	POSTDEC1
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	CALL	_CDCprintf
	MOVLW	0x06
	ADDWF	FSR1L, F
;	.line	155; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("\n\r");
	MOVLW	UPPER(__str_6)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_6)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_6)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x03
	ADDWF	FSR1L, F
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDCprint	code
_CDCprint:
;	.line	112; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	void CDCprint(const u8 *fmt, ...)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
;	.line	116; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	va_start(args, fmt);					// initialize the list
	MOVLW	0x02
	ADDWF	FSR2L, W
	MOVWF	r0x00
	MOVLW	0x00
	ADDWFC	FSR2H, W
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
; #	MOVWF	r0x01
; #	MOVWF	r0x01
	MOVWF	r0x01
; #	MOVF	r0x00, W
; #	MOVWF	r0x00
	MOVLW	0x80
	MOVWF	r0x02
	MOVLW	0x03
	ADDWF	r0x00, F
	MOVLW	0x00
	ADDWFC	r0x01, F
	ADDWFC	r0x02, F
; #	MOVF	r0x02, W
; #	MOVWF	r0x02
; #	MOVF	r0x02, W
; #	MOVF	r0x01, W
; #	MOVF	r0x01, W
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
; #	MOVF	r0x00, W
; #	MOVF	r0x00, W
; #	MOVWF	r0x00
;	.line	117; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	s = (u8) va_arg(args, u32);				// get the first variable arg.
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrget4
	MOVWF	r0x00
;	.line	120; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	switch (s)
	XORLW	0x01
	BNZ	_01460_DS_
	BRA	_01434_DS_
_01460_DS_:
	MOVF	r0x00, W
	XORLW	0x02
	BNZ	_01462_DS_
	BRA	_01436_DS_
_01462_DS_:
	MOVF	r0x00, W
	XORLW	0x08
	BNZ	_01464_DS_
	BRA	_01435_DS_
_01464_DS_:
	MOVF	r0x00, W
	XORLW	0x0a
	BZ	_01432_DS_
	MOVF	r0x00, W
	XORLW	0x10
	BNZ	_01468_DS_
	BRA	_01433_DS_
_01468_DS_:
	MOVF	r0x00, W
	XORLW	0x20
	BZ	_01470_DS_
	BRA	_01437_DS_
_01470_DS_:
;	.line	123; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%f", (u32)fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	r0x07, r0x00
	MOVFF	r0x08, r0x01
	MOVFF	r0x09, r0x02
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_1)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_1)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_1)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x07
	ADDWF	FSR1L, F
;	.line	124; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	break;
	BRA	_01439_DS_
_01432_DS_:
;	.line	126; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%d", (u32)fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	r0x07, r0x00
	MOVFF	r0x08, r0x01
	MOVFF	r0x09, r0x02
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_2)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_2)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_2)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x07
	ADDWF	FSR1L, F
;	.line	127; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	break;
	BRA	_01439_DS_
_01433_DS_:
;	.line	129; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%x", (u32)fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	r0x07, r0x00
	MOVFF	r0x08, r0x01
	MOVFF	r0x09, r0x02
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_3)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_3)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_3)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x07
	ADDWF	FSR1L, F
;	.line	130; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	break;
	BRA	_01439_DS_
_01434_DS_:
;	.line	133; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%d", (u32)fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	r0x07, r0x00
	MOVFF	r0x08, r0x01
	MOVFF	r0x09, r0x02
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_2)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_2)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_2)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x07
	ADDWF	FSR1L, F
;	.line	134; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	break;
	BRA	_01439_DS_
_01435_DS_:
;	.line	136; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%o", (u32)fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	r0x07, r0x00
	MOVFF	r0x08, r0x01
	MOVFF	r0x09, r0x02
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_4)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_4)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_4)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x07
	ADDWF	FSR1L, F
;	.line	137; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	break;
	BRA	_01439_DS_
_01436_DS_:
;	.line	139; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf("%b", (u32)fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	r0x07, r0x00
	MOVFF	r0x08, r0x01
	MOVFF	r0x09, r0x02
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CLRF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	UPPER(__str_5)
	MOVWF	POSTDEC1
	MOVLW	HIGH(__str_5)
	MOVWF	POSTDEC1
	MOVLW	LOW(__str_5)
	MOVWF	POSTDEC1
	CALL	_CDCprintf
	MOVLW	0x07
	ADDWF	FSR1L, F
;	.line	140; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	break;           
	BRA	_01439_DS_
_01437_DS_:
;	.line	142; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCprintf(fmt);
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x07
	MOVLW	0x03
	MOVFF	PLUSW2, r0x08
	MOVLW	0x04
	MOVFF	PLUSW2, r0x09
	MOVF	r0x09, W
	MOVWF	POSTDEC1
	MOVF	r0x08, W
	MOVWF	POSTDEC1
	MOVF	r0x07, W
	MOVWF	POSTDEC1
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	CALL	_CDCprintf
	MOVLW	0x03
	ADDWF	FSR1L, F
_01439_DS_:
;	.line	145; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	va_end(args);
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDCprintf	code
_CDCprintf:
;	.line	81; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	void CDCprintf(const u8 *fmt, ...)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
;	.line	88; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	va_start(args, fmt);
	MOVLW	0x02
	ADDWF	FSR2L, W
	MOVWF	r0x00
	MOVLW	0x00
	ADDWFC	FSR2H, W
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
; #	MOVWF	r0x01
; #	MOVWF	r0x01
	MOVWF	r0x01
; #	MOVF	r0x00, W
; #	MOVWF	r0x00
	MOVLW	0x80
	MOVWF	r0x02
	MOVLW	0x03
	ADDWF	r0x00, F
	MOVLW	0x00
	ADDWFC	r0x01, F
	ADDWFC	r0x02, F
;	.line	89; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	length = psprintf2(_cdc_buffer, fmt, args);
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x04
	MOVFF	PLUSW2, POSTDEC1
	MOVLW	0x03
	MOVFF	PLUSW2, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(__cdc_buffer)
	MOVWF	POSTDEC1
	MOVLW	LOW(__cdc_buffer)
	MOVWF	POSTDEC1
	CALL	_psprintf2
	MOVWF	r0x00
	MOVLW	0x09
	ADDWF	FSR1L, F
;	.line	90; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCputs(_cdc_buffer,length);
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(__cdc_buffer)
	MOVWF	POSTDEC1
	MOVLW	LOW(__cdc_buffer)
	MOVWF	POSTDEC1
	CALL	_CDCputs
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	91; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	va_end(args);
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDCwrite	code
_CDCwrite:
;	.line	75; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	void CDCwrite(u8 c)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVLW	0x02
	MOVFF	PLUSW2, _CDCwrite_c_1_203
;	.line	77; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	CDCputs(&c, 1);
	MOVLW	0x01
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(_CDCwrite_c_1_203)
	MOVWF	POSTDEC1
	MOVLW	LOW(_CDCwrite_c_1_203)
	MOVWF	POSTDEC1
	CALL	_CDCputs
	MOVLW	0x04
	ADDWF	FSR1L, F
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDC_init	code
_CDC_init:
;	.line	30; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	INTCONbits.GIEH = 0;
	BCF	_INTCONbits, 7
;	.line	31; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	INTCONbits.GIEL = 0;
	BCF	_INTCONbits, 6
;	.line	34; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UCON=0;
	CLRF	_UCON
	BANKSEL	_UCFG
;	.line	35; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UCFG=0;
	CLRF	_UCFG, B
	BANKSEL	_UEP0
;	.line	37; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UEP0=0;UEP1=0;UEP2=0;UEP3=0;UEP4=0;UEP5=0;
	CLRF	_UEP0, B
	BANKSEL	_UEP1
	CLRF	_UEP1, B
	BANKSEL	_UEP2
	CLRF	_UEP2, B
	BANKSEL	_UEP3
	CLRF	_UEP3, B
	BANKSEL	_UEP4
	CLRF	_UEP4, B
	BANKSEL	_UEP5
	CLRF	_UEP5, B
	BANKSEL	_UEP6
;	.line	38; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UEP6=0;UEP7=0;UEP8=0;UEP9=0;UEP10=0;UEP11=0;
	CLRF	_UEP6, B
	BANKSEL	_UEP7
	CLRF	_UEP7, B
	BANKSEL	_UEP8
	CLRF	_UEP8, B
	BANKSEL	_UEP9
	CLRF	_UEP9, B
	BANKSEL	_UEP10
	CLRF	_UEP10, B
	BANKSEL	_UEP11
	CLRF	_UEP11, B
	BANKSEL	_UEP12
;	.line	39; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UEP12=0;UEP13=0;UEP14=0;UEP15=0;
	CLRF	_UEP12, B
	BANKSEL	_UEP13
	CLRF	_UEP13, B
	BANKSEL	_UEP14
	CLRF	_UEP14, B
	BANKSEL	_UEP15
	CLRF	_UEP15, B
;	.line	42; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	Delayms(2000);
	MOVLW	0x07
	MOVWF	POSTDEC1
	MOVLW	0xd0
	MOVWF	POSTDEC1
	CALL	_Delayms
	MOVF	POSTINC1, F
	MOVF	POSTINC1, F
;	.line	45; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	UCFG = 0x14; 				// Enable pullup resistors; full speed mode
	MOVLW	0x14
	BANKSEL	_UCFG
	MOVWF	_UCFG, B
	BANKSEL	_deviceState
;	.line	46; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	deviceState = DETACHED;
	CLRF	_deviceState, B
	BANKSEL	_remoteWakeup
;	.line	47; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	remoteWakeup = 0x00;
	CLRF	_remoteWakeup, B
	BANKSEL	_currentConfiguration
;	.line	48; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	currentConfiguration = 0x00;
	CLRF	_currentConfiguration, B
_01404_DS_:
	BANKSEL	_deviceState
;	.line	51; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	while(deviceState != CONFIGURED)
	MOVF	_deviceState, W, B
	XORLW	0x05
	BZ	_01406_DS_
;	.line	53; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	EnableUSBModule();
	CALL	_EnableUSBModule
;	.line	54; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	ProcessUSBTransactions();
	CALL	_ProcessUSBTransactions
	BRA	_01404_DS_
_01406_DS_:
;	.line	63; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	PIR2bits.USBIF = 0;     // clear usb interrupt flag
	BCF	_PIR2bits, 4
;	.line	64; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	PIE2bits.USBIE = 1;     // enable usb interrupt
	BSF	_PIE2bits, 4
;	.line	65; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	IPR2bits.USBIP = 1;     // high priority interrupt
	BSF	_IPR2bits, 4
;	.line	68; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	INTCONbits.GIEH = 1;   // Enable global HP interrupts
	BSF	_INTCONbits, 7
;	.line	69; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/__cdc.c	INTCONbits.GIEL = 1;   // Enable global LP interrupts
	BSF	_INTCONbits, 6
	RETURN	

; ; Starting pCode block
S_main__psprintf	code
_psprintf:
;	.line	474; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 psprintf(u8 *out, const u8 *format, ...)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
;	.line	478; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	va_start(args, format);
	MOVLW	0x05
	ADDWF	FSR2L, W
	MOVWF	r0x00
	MOVLW	0x00
	ADDWFC	FSR2H, W
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
; #	MOVWF	r0x01
; #	MOVWF	r0x01
	MOVWF	r0x01
; #	MOVF	r0x00, W
; #	MOVWF	r0x00
	MOVLW	0x80
	MOVWF	r0x02
	MOVLW	0x03
	ADDWF	r0x00, F
	MOVLW	0x00
	ADDWFC	r0x01, F
	ADDWFC	r0x02, F
;	.line	479; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pprint(&out, format, args);
	MOVLW	0x02
	ADDWF	FSR2L, W
	MOVWF	r0x03
	MOVLW	0x00
	ADDWFC	FSR2H, W
; #	MOVWF	r0x04
; #	MOVF	r0x04, W
; #	MOVWF	r0x04
; #	MOVWF	r0x04
	MOVWF	r0x04
; #	MOVF	r0x03, W
; #	MOVWF	r0x03
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x07
	MOVFF	PLUSW2, POSTDEC1
	MOVLW	0x06
	MOVFF	PLUSW2, POSTDEC1
	MOVLW	0x05
	MOVFF	PLUSW2, POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVF	r0x04, W
	MOVWF	POSTDEC1
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	CALL	_pprint
	MOVWF	r0x00
	MOVLW	0x09
	ADDWF	FSR1L, F
	MOVF	r0x00, W
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__psprintf2	code
_psprintf2:
;	.line	468; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 psprintf2(u8 *out, const u8 *format, va_list args)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, _psprintf2_out_1_196
	MOVLW	0x03
	MOVFF	PLUSW2, (_psprintf2_out_1_196 + 1)
	MOVLW	0x04
	MOVFF	PLUSW2, (_psprintf2_out_1_196 + 2)
	MOVLW	0x05
	MOVFF	PLUSW2, r0x00
	MOVLW	0x06
	MOVFF	PLUSW2, r0x01
	MOVLW	0x07
	MOVFF	PLUSW2, r0x02
	MOVLW	0x08
	MOVFF	PLUSW2, r0x03
	MOVLW	0x09
	MOVFF	PLUSW2, r0x04
	MOVLW	0x0a
	MOVFF	PLUSW2, r0x05
;	.line	470; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pprint(&out, format, args);
	MOVF	r0x05, W
	MOVWF	POSTDEC1
	MOVF	r0x04, W
	MOVWF	POSTDEC1
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(_psprintf2_out_1_196)
	MOVWF	POSTDEC1
	MOVLW	LOW(_psprintf2_out_1_196)
	MOVWF	POSTDEC1
	CALL	_pprint
	MOVWF	r0x00
	MOVLW	0x09
	ADDWF	FSR1L, F
	MOVF	r0x00, W
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pprintf	code
_pprintf:
;	.line	454; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 pprintf(funcout func, const u8 *format, va_list args)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, _pputchar
	MOVLW	0x03
	MOVFF	PLUSW2, (_pputchar + 1)
	MOVLW	0x04
	MOVFF	PLUSW2, (_pputchar + 2)
	MOVLW	0x05
	MOVFF	PLUSW2, r0x00
	MOVLW	0x06
	MOVFF	PLUSW2, r0x01
	MOVLW	0x07
	MOVFF	PLUSW2, r0x02
	MOVLW	0x08
	MOVFF	PLUSW2, r0x03
	MOVLW	0x09
	MOVFF	PLUSW2, r0x04
	MOVLW	0x0a
	MOVFF	PLUSW2, r0x05
;	.line	460; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pprint(0, format, args);
	MOVF	r0x05, W
	MOVWF	POSTDEC1
	MOVF	r0x04, W
	MOVWF	POSTDEC1
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	MOVLW	0x00
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	CALL	_pprint
	MOVWF	r0x00
	MOVLW	0x09
	ADDWF	FSR1L, F
	MOVF	r0x00, W
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pprint	code
_pprint:
;	.line	306; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	static u8 pprint(u8 **out, const u8 *format, va_list args)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVFF	r0x0a, POSTDEC1
	MOVFF	r0x0b, POSTDEC1
	MOVFF	r0x0c, POSTDEC1
	MOVFF	r0x0d, POSTDEC1
	MOVFF	r0x0e, POSTDEC1
	MOVFF	r0x0f, POSTDEC1
	MOVFF	r0x10, POSTDEC1
	MOVFF	r0x11, POSTDEC1
	MOVFF	r0x12, POSTDEC1
	MOVFF	r0x13, POSTDEC1
	MOVFF	r0x14, POSTDEC1
	MOVFF	r0x15, POSTDEC1
	MOVFF	r0x16, POSTDEC1
	MOVFF	r0x17, POSTDEC1
	MOVFF	r0x18, POSTDEC1
	MOVFF	r0x19, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
	MOVLW	0x06
	MOVFF	PLUSW2, r0x04
	MOVLW	0x07
	MOVFF	PLUSW2, r0x05
	MOVLW	0x08
	MOVFF	PLUSW2, r0x06
	MOVLW	0x09
	MOVFF	PLUSW2, r0x07
	MOVLW	0x0a
	MOVFF	PLUSW2, r0x08
;	.line	310; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	register u8 pc = 0;
	CLRF	r0x09
;	.line	311; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 precision = 2; // default value is 2 digits fractional part
	MOVLW	0x02
	MOVWF	r0x0a
_01216_DS_:
;	.line	315; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for (; *format != 0; ++format)
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x0b
	MOVF	r0x0b, W
	BNZ	_01336_DS_
	GOTO	_01204_DS_
_01336_DS_:
;	.line	318; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	islong = 0;                 // default is 16-bit
	CLRF	r0x0c
;	.line	320; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == '%')
	MOVF	r0x0b, W
	XORLW	0x25
	BZ	_01338_DS_
	GOTO	_01199_DS_
_01338_DS_:
;	.line	322; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	width = pad = 0;		// default is left justify, no zero padded
	CLRF	r0x0b
	CLRF	r0x0d
;	.line	323; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++format;				// get the next format identifier
	INCF	r0x03, F
	BNC	_01339_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_01339_DS_:
;	.line	325; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == '\0')	// end of line
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x0e
	MOVF	r0x0e, W
	BTFSC	STATUS, 2
	GOTO	_01204_DS_
;	.line	328; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == '%')		// error
	MOVF	r0x0e, W
	XORLW	0x25
	BNZ	_01341_DS_
	GOTO	_01199_DS_
_01341_DS_:
;	.line	331; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == '-')		// right justify
	MOVF	r0x0e, W
	XORLW	0x2d
	BNZ	_01239_DS_
;	.line	333; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++format;
	INCF	r0x03, F
	BNC	_01344_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_01344_DS_:
;	.line	334; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pad = PAD_RIGHT;
	MOVLW	0x01
	MOVWF	r0x0b
_01239_DS_:
;	.line	337; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	while (*format == '0')	// field is padded with 0's instead of blanks
	MOVFF	r0x03, r0x0e
	MOVFF	r0x04, r0x0f
	MOVFF	r0x05, r0x10
_01169_DS_:
	MOVFF	r0x0e, FSR0L
	MOVFF	r0x0f, PRODL
	MOVF	r0x10, W
	CALL	__gptrget1
; #	MOVWF	r0x11
; #	MOVF	r0x11, W
	XORLW	0x30
	BNZ	_01209_DS_
;	.line	339; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++format;
	INCF	r0x0e, F
	BNC	_01347_DS_
	INFSNZ	r0x0f, F
	INCF	r0x10, F
_01347_DS_:
;	.line	340; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pad |= PAD_ZERO;
	BSF	r0x0b, 1
	BRA	_01169_DS_
_01209_DS_:
;	.line	343; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; *format >= '0' && *format <= '9'; ++format)
	MOVFF	r0x0e, FSR0L
	MOVFF	r0x0f, PRODL
	MOVF	r0x10, W
	CALL	__gptrget1
	MOVWF	r0x11
	MOVLW	0x30
	SUBWF	r0x11, W
	BNC	_01267_DS_
	MOVLW	0x3a
	SUBWF	r0x11, W
	BC	_01267_DS_
; ;multiply lit val:0x0a by variable r0x0d and store in r0x0d
;	.line	345; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	width *= 10;
	MOVF	r0x0d, W
	MULLW	0x0a
	MOVFF	PRODL, r0x0d
;	.line	346; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	width += *format - '0';
	MOVLW	0xd0
	ADDWF	r0x11, F
	MOVF	r0x0d, W
	MOVWF	r0x12
	MOVF	r0x11, W
	ADDWF	r0x12, W
	MOVWF	r0x0d
;	.line	343; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; *format >= '0' && *format <= '9'; ++format)
	INCF	r0x0e, F
	BNC	_01351_DS_
	INFSNZ	r0x0f, F
	INCF	r0x10, F
_01351_DS_:
	BRA	_01209_DS_
_01267_DS_:
	MOVFF	r0x0e, r0x03
	MOVFF	r0x0f, r0x04
	MOVFF	r0x10, r0x05
;	.line	356; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == '.')		// float precision
	MOVFF	r0x0e, FSR0L
	MOVFF	r0x0f, PRODL
	MOVF	r0x10, W
	CALL	__gptrget1
; #	MOVWF	r0x11
; #	MOVF	r0x11, W
	XORLW	0x2e
	BNZ	_01175_DS_
;	.line	358; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++format;
	MOVF	r0x0e, W
	ADDLW	0x01
	MOVWF	r0x03
	MOVLW	0x00
	ADDWFC	r0x0f, W
	MOVWF	r0x04
	MOVLW	0x00
	ADDWFC	r0x10, W
	MOVWF	r0x05
;	.line	359; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	precision = 0;
	CLRF	r0x0a
	MOVFF	r0x03, r0x0e
	MOVFF	r0x04, r0x0f
	MOVFF	r0x05, r0x10
_01213_DS_:
;	.line	361; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; *format >= '0' && *format <= '9'; ++format)
	MOVFF	r0x0e, FSR0L
	MOVFF	r0x0f, PRODL
	MOVF	r0x10, W
	CALL	__gptrget1
	MOVWF	r0x11
	MOVLW	0x30
	SUBWF	r0x11, W
	BNC	_01268_DS_
	MOVLW	0x3a
	SUBWF	r0x11, W
	BC	_01268_DS_
; ;multiply lit val:0x0a by variable r0x0a and store in r0x0a
;	.line	363; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	precision *= 10;
	MOVF	r0x0a, W
	MULLW	0x0a
	MOVFF	PRODL, r0x0a
;	.line	364; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	precision += *format - '0';
	MOVLW	0xd0
	ADDWF	r0x11, F
	MOVF	r0x0a, W
	MOVWF	r0x12
	MOVF	r0x11, W
	ADDWF	r0x12, W
	MOVWF	r0x0a
;	.line	361; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; *format >= '0' && *format <= '9'; ++format)
	INCF	r0x0e, F
	BNC	_01356_DS_
	INFSNZ	r0x0f, F
	INCF	r0x10, F
_01356_DS_:
	BRA	_01213_DS_
_01268_DS_:
	MOVFF	r0x0e, r0x03
	MOVFF	r0x0f, r0x04
	MOVFF	r0x10, r0x05
_01175_DS_:
;	.line	368; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'f') 	// float
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
; #	MOVWF	r0x0e
; #	MOVF	r0x0e, W
	MOVWF	r0x0e
	XORLW	0x66
	BZ	_01358_DS_
	BRA	_01177_DS_
_01358_DS_:
;	.line	370; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprintfl(out, va_arg(args, float), width, pad, separator, precision);
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x0f
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x10
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x11
	MOVFF	r0x0f, r0x06
	MOVFF	r0x10, r0x07
	MOVFF	r0x11, r0x08
	MOVLW	0xfc
	ADDWF	r0x0f, F
	MOVLW	0xff
	ADDWFC	r0x10, F
	ADDWFC	r0x11, F
; #	MOVF	r0x11, W
; #	MOVWF	r0x11
; #	MOVF	r0x11, W
; #	MOVF	r0x10, W
; #	MOVF	r0x10, W
; #	MOVWF	r0x10
; #	MOVF	r0x10, W
; #	MOVF	r0x0f, W
; #	MOVF	r0x0f, W
; #	MOVWF	r0x0f
	MOVFF	r0x0f, FSR0L
	MOVFF	r0x10, PRODL
	MOVF	r0x11, W
	CALL	__gptrget4
	MOVWF	r0x0f
	MOVFF	PRODL, r0x10
	MOVFF	PRODH, r0x11
	MOVFF	FSR0L, r0x12
	MOVF	r0x0a, W
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintfl
	MOVWF	r0x0f
	MOVLW	0x0b
	ADDWF	FSR1L, F
	MOVF	r0x0f, W
	ADDWF	r0x09, F
;	.line	371; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	GOTO	_01203_DS_
_01177_DS_:
;	.line	374; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 's')		// string
	MOVF	r0x0e, W
	XORLW	0x73
	BZ	_01360_DS_
	BRA	_01179_DS_
_01360_DS_:
;	.line	376; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 *s = va_arg(args, u8*);
	MOVF	r0x06, W
	ADDLW	0x03
	MOVWF	r0x0f
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x10
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x11
	MOVFF	r0x0f, r0x06
	MOVFF	r0x10, r0x07
	MOVFF	r0x11, r0x08
	MOVLW	0xfd
	ADDWF	r0x0f, F
	MOVLW	0xff
	ADDWFC	r0x10, F
	ADDWFC	r0x11, F
; #	MOVF	r0x11, W
; #	MOVWF	r0x11
; #	MOVF	r0x11, W
; #	MOVF	r0x10, W
; #	MOVF	r0x10, W
; #	MOVWF	r0x10
; #	MOVF	r0x10, W
; #	MOVF	r0x0f, W
; #	MOVF	r0x0f, W
; #	MOVWF	r0x0f
	MOVFF	r0x0f, FSR0L
	MOVFF	r0x10, PRODL
	MOVF	r0x11, W
	CALL	__gptrget3
	MOVWF	r0x0f
	MOVFF	PRODL, r0x10
	MOVFF	PRODH, r0x11
;	.line	377; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprints(out, s?s:"(null)", width, pad);
	IORWF	r0x10, W
; #	IORWF	r0x11, W
; #	BTFSC	STATUS, 2
; #	GOTO	_01219_DS_
; #	GOTO	_01220_DS_
; #	MOVLW	UPPER(__str_0)
	IORWF	r0x11, W
	BNZ	_01220_DS_
	MOVLW	UPPER(__str_0)
	MOVWF	r0x11
	MOVLW	HIGH(__str_0)
	MOVWF	r0x10
	MOVLW	LOW(__str_0)
	MOVWF	r0x0f
_01220_DS_:
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprints
	MOVWF	r0x0f
	MOVLW	0x08
	ADDWF	FSR1L, F
	MOVF	r0x09, W
	MOVWF	r0x10
	MOVF	r0x0f, W
	ADDWF	r0x10, W
	MOVWF	r0x09
;	.line	378; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	GOTO	_01203_DS_
_01179_DS_:
;	.line	381; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'l')		// long support
	MOVF	r0x0e, W
	XORLW	0x6c
	BNZ	_01181_DS_
;	.line	383; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++format;
	INCF	r0x03, F
	BNC	_01363_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_01363_DS_:
;	.line	384; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	islong = 1;
	MOVLW	0x01
	MOVWF	r0x0c
_01181_DS_:
;	.line	387; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'u')		// decimal (10) unsigned (0) integer
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
; #	MOVWF	r0x0e
; #	MOVF	r0x0e, W
	MOVWF	r0x0e
	XORLW	0x75
	BZ	_01365_DS_
	BRA	_01183_DS_
_01365_DS_:
;	.line	389; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
	MOVF	r0x0c, W
	BZ	_01221_DS_
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x0f
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x10
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x11
	MOVFF	r0x0f, r0x06
	MOVFF	r0x10, r0x07
	MOVFF	r0x11, r0x08
	MOVLW	0xfc
	ADDWF	r0x0f, F
	MOVLW	0xff
	ADDWFC	r0x10, F
	ADDWFC	r0x11, F
; #	MOVF	r0x11, W
; #	MOVWF	r0x11
; #	MOVF	r0x11, W
; #	MOVF	r0x10, W
; #	MOVF	r0x10, W
; #	MOVWF	r0x10
; #	MOVF	r0x10, W
; #	MOVF	r0x0f, W
; #	MOVF	r0x0f, W
; #	MOVWF	r0x0f
	MOVFF	r0x0f, FSR0L
	MOVFF	r0x10, PRODL
	MOVF	r0x11, W
	CALL	__gptrget4
	MOVWF	r0x0f
	MOVFF	PRODL, r0x10
	MOVFF	PRODH, r0x11
	MOVFF	FSR0L, r0x12
	BRA	_01222_DS_
_01221_DS_:
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x13
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x14
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x15
	MOVFF	r0x13, r0x06
	MOVFF	r0x14, r0x07
	MOVFF	r0x15, r0x08
	MOVLW	0xfe
	ADDWF	r0x13, F
	MOVLW	0xff
	ADDWFC	r0x14, F
	ADDWFC	r0x15, F
; #	MOVF	r0x15, W
; #	MOVWF	r0x15
; #	MOVF	r0x15, W
; #	MOVF	r0x14, W
; #	MOVF	r0x14, W
; #	MOVWF	r0x14
; #	MOVF	r0x14, W
; #	MOVF	r0x13, W
; #	MOVF	r0x13, W
; #	MOVWF	r0x13
	MOVFF	r0x13, FSR0L
	MOVFF	r0x14, PRODL
	MOVF	r0x15, W
	CALL	__gptrget2
	MOVWF	r0x13
	MOVFF	PRODL, r0x14
	MOVFF	r0x13, r0x0f
	MOVFF	r0x14, r0x10
	CLRF	r0x11
	CLRF	r0x12
_01222_DS_:
;	.line	390; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprinti(out, val, islong, 10, 0, width, pad, separator, 'a');
	MOVLW	0x61
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x0a
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprinti
	MOVWF	r0x13
	MOVLW	0x0e
	ADDWF	FSR1L, F
	MOVF	r0x13, W
	ADDWF	r0x09, F
;	.line	391; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	GOTO	_01203_DS_
_01183_DS_:
;	.line	394; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'd' || *format == 'i') // decimal (10) signed (1) integer
	MOVF	r0x0e, W
	XORLW	0x64
	BZ	_01184_DS_
	MOVF	r0x0e, W
	XORLW	0x69
	BZ	_01184_DS_
	BRA	_01185_DS_
_01184_DS_:
;	.line	396; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
	MOVF	r0x0c, W
	BZ	_01223_DS_
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x13
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x14
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x15
	MOVFF	r0x13, r0x06
	MOVFF	r0x14, r0x07
	MOVFF	r0x15, r0x08
	MOVLW	0xfc
	ADDWF	r0x13, F
	MOVLW	0xff
	ADDWFC	r0x14, F
	ADDWFC	r0x15, F
; #	MOVF	r0x15, W
; #	MOVWF	r0x15
; #	MOVF	r0x15, W
; #	MOVF	r0x14, W
; #	MOVF	r0x14, W
; #	MOVWF	r0x14
; #	MOVF	r0x14, W
; #	MOVF	r0x13, W
; #	MOVF	r0x13, W
; #	MOVWF	r0x13
	MOVFF	r0x13, FSR0L
	MOVFF	r0x14, PRODL
	MOVF	r0x15, W
	CALL	__gptrget4
	MOVWF	r0x13
	MOVFF	PRODL, r0x14
	MOVFF	PRODH, r0x15
	MOVFF	FSR0L, r0x16
	BRA	_01224_DS_
_01223_DS_:
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x17
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x18
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x19
	MOVFF	r0x17, r0x06
	MOVFF	r0x18, r0x07
	MOVFF	r0x19, r0x08
	MOVLW	0xfe
	ADDWF	r0x17, F
	MOVLW	0xff
	ADDWFC	r0x18, F
	ADDWFC	r0x19, F
; #	MOVF	r0x19, W
; #	MOVWF	r0x19
; #	MOVF	r0x19, W
; #	MOVF	r0x18, W
; #	MOVF	r0x18, W
; #	MOVWF	r0x18
; #	MOVF	r0x18, W
; #	MOVF	r0x17, W
; #	MOVF	r0x17, W
; #	MOVWF	r0x17
	MOVFF	r0x17, FSR0L
	MOVFF	r0x18, PRODL
	MOVF	r0x19, W
	CALL	__gptrget2
	MOVWF	r0x17
	MOVFF	PRODL, r0x18
	MOVFF	r0x17, r0x13
	MOVFF	r0x18, r0x14
	CLRF	r0x15
	CLRF	r0x16
_01224_DS_:
	MOVFF	r0x13, r0x0f
	MOVFF	r0x14, r0x10
	MOVFF	r0x15, r0x11
	MOVFF	r0x16, r0x12
;	.line	397; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprinti(out, val, islong, 10, 1, width, pad, separator, 'a');
	MOVLW	0x61
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVLW	0x01
	MOVWF	POSTDEC1
	MOVLW	0x0a
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprinti
	MOVWF	r0x13
	MOVLW	0x0e
	ADDWF	FSR1L, F
	MOVF	r0x13, W
	ADDWF	r0x09, F
;	.line	398; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	GOTO	_01203_DS_
_01185_DS_:
;	.line	401; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'x' || *format == 'p')	// unsigned (0) lower ('a') hexa (16) or pointer
	MOVF	r0x0e, W
	XORLW	0x78
	BZ	_01187_DS_
	MOVF	r0x0e, W
	XORLW	0x70
	BZ	_01187_DS_
	BRA	_01188_DS_
_01187_DS_:
;	.line	403; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
	MOVF	r0x0c, W
	BZ	_01225_DS_
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x13
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x14
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x15
	MOVFF	r0x13, r0x06
	MOVFF	r0x14, r0x07
	MOVFF	r0x15, r0x08
	MOVLW	0xfc
	ADDWF	r0x13, F
	MOVLW	0xff
	ADDWFC	r0x14, F
	ADDWFC	r0x15, F
; #	MOVF	r0x15, W
; #	MOVWF	r0x15
; #	MOVF	r0x15, W
; #	MOVF	r0x14, W
; #	MOVF	r0x14, W
; #	MOVWF	r0x14
; #	MOVF	r0x14, W
; #	MOVF	r0x13, W
; #	MOVF	r0x13, W
; #	MOVWF	r0x13
	MOVFF	r0x13, FSR0L
	MOVFF	r0x14, PRODL
	MOVF	r0x15, W
	CALL	__gptrget4
	MOVWF	r0x13
	MOVFF	PRODL, r0x14
	MOVFF	PRODH, r0x15
	MOVFF	FSR0L, r0x16
	BRA	_01226_DS_
_01225_DS_:
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x17
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x18
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x19
	MOVFF	r0x17, r0x06
	MOVFF	r0x18, r0x07
	MOVFF	r0x19, r0x08
	MOVLW	0xfe
	ADDWF	r0x17, F
	MOVLW	0xff
	ADDWFC	r0x18, F
	ADDWFC	r0x19, F
; #	MOVF	r0x19, W
; #	MOVWF	r0x19
; #	MOVF	r0x19, W
; #	MOVF	r0x18, W
; #	MOVF	r0x18, W
; #	MOVWF	r0x18
; #	MOVF	r0x18, W
; #	MOVF	r0x17, W
; #	MOVF	r0x17, W
; #	MOVWF	r0x17
	MOVFF	r0x17, FSR0L
	MOVFF	r0x18, PRODL
	MOVF	r0x19, W
	CALL	__gptrget2
	MOVWF	r0x17
	MOVFF	PRODL, r0x18
	MOVFF	r0x17, r0x13
	MOVFF	r0x18, r0x14
	CLRF	r0x15
	CLRF	r0x16
_01226_DS_:
	MOVFF	r0x13, r0x0f
	MOVFF	r0x14, r0x10
	MOVFF	r0x15, r0x11
	MOVFF	r0x16, r0x12
;	.line	404; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprinti(out, val, islong, 16, 0, width, pad, separator, 'a');
	MOVLW	0x61
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x10
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprinti
	MOVWF	r0x13
	MOVLW	0x0e
	ADDWF	FSR1L, F
	MOVF	r0x13, W
	ADDWF	r0x09, F
;	.line	405; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	BRA	_01203_DS_
_01188_DS_:
;	.line	408; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'X' || *format == 'P')	// unsigned (0) upper ('A') hexa (16) or pointer
	MOVF	r0x0e, W
	XORLW	0x58
	BZ	_01190_DS_
	MOVF	r0x0e, W
	XORLW	0x50
	BZ	_01190_DS_
	BRA	_01191_DS_
_01190_DS_:
;	.line	410; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
	MOVF	r0x0c, W
	BZ	_01227_DS_
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x13
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x14
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x15
	MOVFF	r0x13, r0x06
	MOVFF	r0x14, r0x07
	MOVFF	r0x15, r0x08
	MOVLW	0xfc
	ADDWF	r0x13, F
	MOVLW	0xff
	ADDWFC	r0x14, F
	ADDWFC	r0x15, F
; #	MOVF	r0x15, W
; #	MOVWF	r0x15
; #	MOVF	r0x15, W
; #	MOVF	r0x14, W
; #	MOVF	r0x14, W
; #	MOVWF	r0x14
; #	MOVF	r0x14, W
; #	MOVF	r0x13, W
; #	MOVF	r0x13, W
; #	MOVWF	r0x13
	MOVFF	r0x13, FSR0L
	MOVFF	r0x14, PRODL
	MOVF	r0x15, W
	CALL	__gptrget4
	MOVWF	r0x13
	MOVFF	PRODL, r0x14
	MOVFF	PRODH, r0x15
	MOVFF	FSR0L, r0x16
	BRA	_01228_DS_
_01227_DS_:
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x17
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x18
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x19
	MOVFF	r0x17, r0x06
	MOVFF	r0x18, r0x07
	MOVFF	r0x19, r0x08
	MOVLW	0xfe
	ADDWF	r0x17, F
	MOVLW	0xff
	ADDWFC	r0x18, F
	ADDWFC	r0x19, F
; #	MOVF	r0x19, W
; #	MOVWF	r0x19
; #	MOVF	r0x19, W
; #	MOVF	r0x18, W
; #	MOVF	r0x18, W
; #	MOVWF	r0x18
; #	MOVF	r0x18, W
; #	MOVF	r0x17, W
; #	MOVF	r0x17, W
; #	MOVWF	r0x17
	MOVFF	r0x17, FSR0L
	MOVFF	r0x18, PRODL
	MOVF	r0x19, W
	CALL	__gptrget2
	MOVWF	r0x17
	MOVFF	PRODL, r0x18
	MOVFF	r0x17, r0x13
	MOVFF	r0x18, r0x14
	CLRF	r0x15
	CLRF	r0x16
_01228_DS_:
	MOVFF	r0x13, r0x0f
	MOVFF	r0x14, r0x10
	MOVFF	r0x15, r0x11
	MOVFF	r0x16, r0x12
;	.line	411; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprinti(out, val, islong, 16, 0, width, pad, separator, 'A');
	MOVLW	0x41
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x10
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprinti
	MOVWF	r0x13
	MOVLW	0x0e
	ADDWF	FSR1L, F
	MOVF	r0x13, W
	ADDWF	r0x09, F
;	.line	412; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	BRA	_01203_DS_
_01191_DS_:
;	.line	415; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'b')		// binary
	MOVF	r0x0e, W
	XORLW	0x62
	BZ	_01379_DS_
	BRA	_01194_DS_
_01379_DS_:
;	.line	417; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
	MOVF	r0x0c, W
	BZ	_01229_DS_
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x13
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x14
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x15
	MOVFF	r0x13, r0x06
	MOVFF	r0x14, r0x07
	MOVFF	r0x15, r0x08
	MOVLW	0xfc
	ADDWF	r0x13, F
	MOVLW	0xff
	ADDWFC	r0x14, F
	ADDWFC	r0x15, F
; #	MOVF	r0x15, W
; #	MOVWF	r0x15
; #	MOVF	r0x15, W
; #	MOVF	r0x14, W
; #	MOVF	r0x14, W
; #	MOVWF	r0x14
; #	MOVF	r0x14, W
; #	MOVF	r0x13, W
; #	MOVF	r0x13, W
; #	MOVWF	r0x13
	MOVFF	r0x13, FSR0L
	MOVFF	r0x14, PRODL
	MOVF	r0x15, W
	CALL	__gptrget4
	MOVWF	r0x13
	MOVFF	PRODL, r0x14
	MOVFF	PRODH, r0x15
	MOVFF	FSR0L, r0x16
	BRA	_01230_DS_
_01229_DS_:
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x17
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x18
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x19
	MOVFF	r0x17, r0x06
	MOVFF	r0x18, r0x07
	MOVFF	r0x19, r0x08
	MOVLW	0xfe
	ADDWF	r0x17, F
	MOVLW	0xff
	ADDWFC	r0x18, F
	ADDWFC	r0x19, F
; #	MOVF	r0x19, W
; #	MOVWF	r0x19
; #	MOVF	r0x19, W
; #	MOVF	r0x18, W
; #	MOVF	r0x18, W
; #	MOVWF	r0x18
; #	MOVF	r0x18, W
; #	MOVF	r0x17, W
; #	MOVF	r0x17, W
; #	MOVWF	r0x17
	MOVFF	r0x17, FSR0L
	MOVFF	r0x18, PRODL
	MOVF	r0x19, W
	CALL	__gptrget2
	MOVWF	r0x17
	MOVFF	PRODL, r0x18
	MOVFF	r0x17, r0x13
	MOVFF	r0x18, r0x14
	CLRF	r0x15
	CLRF	r0x16
_01230_DS_:
	MOVFF	r0x13, r0x0f
	MOVFF	r0x14, r0x10
	MOVFF	r0x15, r0x11
	MOVFF	r0x16, r0x12
;	.line	418; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprinti(out, val, islong, 2, 0, width, pad, separator, 'a');
	MOVLW	0x61
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x02
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprinti
	MOVWF	r0x13
	MOVLW	0x0e
	ADDWF	FSR1L, F
	MOVF	r0x13, W
	ADDWF	r0x09, F
;	.line	419; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	BRA	_01203_DS_
_01194_DS_:
;	.line	422; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'o')		// octal
	MOVF	r0x0e, W
	XORLW	0x6f
	BZ	_01381_DS_
	BRA	_01196_DS_
_01381_DS_:
;	.line	424; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	val = (islong) ? va_arg(args, u32) : va_arg(args, u16);
	MOVF	r0x0c, W
	BZ	_01231_DS_
	MOVF	r0x06, W
	ADDLW	0x04
	MOVWF	r0x13
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x14
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x15
	MOVFF	r0x13, r0x06
	MOVFF	r0x14, r0x07
	MOVFF	r0x15, r0x08
	MOVLW	0xfc
	ADDWF	r0x13, F
	MOVLW	0xff
	ADDWFC	r0x14, F
	ADDWFC	r0x15, F
; #	MOVF	r0x15, W
; #	MOVWF	r0x15
; #	MOVF	r0x15, W
; #	MOVF	r0x14, W
; #	MOVF	r0x14, W
; #	MOVWF	r0x14
; #	MOVF	r0x14, W
; #	MOVF	r0x13, W
; #	MOVF	r0x13, W
; #	MOVWF	r0x13
	MOVFF	r0x13, FSR0L
	MOVFF	r0x14, PRODL
	MOVF	r0x15, W
	CALL	__gptrget4
	MOVWF	r0x13
	MOVFF	PRODL, r0x14
	MOVFF	PRODH, r0x15
	MOVFF	FSR0L, r0x16
	BRA	_01232_DS_
_01231_DS_:
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x17
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x18
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x19
	MOVFF	r0x17, r0x06
	MOVFF	r0x18, r0x07
	MOVFF	r0x19, r0x08
	MOVLW	0xfe
	ADDWF	r0x17, F
	MOVLW	0xff
	ADDWFC	r0x18, F
	ADDWFC	r0x19, F
; #	MOVF	r0x19, W
; #	MOVWF	r0x19
; #	MOVF	r0x19, W
; #	MOVF	r0x18, W
; #	MOVF	r0x18, W
; #	MOVWF	r0x18
; #	MOVF	r0x18, W
; #	MOVF	r0x17, W
; #	MOVF	r0x17, W
; #	MOVWF	r0x17
	MOVFF	r0x17, FSR0L
	MOVFF	r0x18, PRODL
	MOVF	r0x19, W
	CALL	__gptrget2
	MOVWF	r0x17
	MOVFF	PRODL, r0x18
	MOVFF	r0x17, r0x13
	MOVFF	r0x18, r0x14
	CLRF	r0x15
	CLRF	r0x16
_01232_DS_:
	MOVFF	r0x13, r0x0f
	MOVFF	r0x14, r0x10
	MOVFF	r0x15, r0x11
	MOVFF	r0x16, r0x12
;	.line	425; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprinti(out, val, islong, 8, 0, width, pad, separator, 'a');
	MOVLW	0x61
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x08
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprinti
	MOVWF	r0x0c
	MOVLW	0x0e
	ADDWF	FSR1L, F
	MOVF	r0x0c, W
	ADDWF	r0x09, F
;	.line	426; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	BRA	_01203_DS_
_01196_DS_:
;	.line	429; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (*format == 'c') 	// ascii
	MOVF	r0x0e, W
	XORLW	0x63
	BZ	_01383_DS_
	BRA	_01203_DS_
_01383_DS_:
;	.line	431; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	scr[0] = va_arg(args, u16);
	MOVF	r0x06, W
	ADDLW	0x02
	MOVWF	r0x0c
	MOVLW	0x00
	ADDWFC	r0x07, W
	MOVWF	r0x0e
	MOVLW	0x00
	ADDWFC	r0x08, W
	MOVWF	r0x0f
	MOVFF	r0x0c, r0x06
	MOVFF	r0x0e, r0x07
	MOVFF	r0x0f, r0x08
	MOVLW	0xfe
	ADDWF	r0x0c, F
	MOVLW	0xff
	ADDWFC	r0x0e, F
	ADDWFC	r0x0f, F
; #	MOVF	r0x0f, W
; #	MOVWF	r0x0f
; #	MOVF	r0x0f, W
; #	MOVF	r0x0e, W
; #	MOVF	r0x0e, W
; #	MOVWF	r0x0e
; #	MOVF	r0x0e, W
; #	MOVF	r0x0c, W
; #	MOVF	r0x0c, W
; #	MOVWF	r0x0c
	MOVFF	r0x0c, FSR0L
	MOVFF	r0x0e, PRODL
	MOVF	r0x0f, W
	CALL	__gptrget2
	BANKSEL	_pprint_scr_1_175
	MOVWF	_pprint_scr_1_175, B
; removed redundant BANKSEL
;	.line	432; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	scr[1] = '\0';
	CLRF	(_pprint_scr_1_175 + 1), B
;	.line	433; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pc += pprints(out, scr, width, pad);
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(_pprint_scr_1_175)
	MOVWF	POSTDEC1
	MOVLW	LOW(_pprint_scr_1_175)
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprints
	MOVWF	r0x0b
	MOVLW	0x08
	ADDWF	FSR1L, F
	MOVF	r0x09, W
	MOVWF	r0x0c
	MOVF	r0x0b, W
	ADDWF	r0x0c, W
	MOVWF	r0x09
;	.line	434; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	continue;
	BRA	_01203_DS_
_01199_DS_:
;	.line	441; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pprintc(out, *format);
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
; #	MOVWF	r0x0b
; #	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintc
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	442; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++pc;
	INCF	r0x09, F
_01203_DS_:
;	.line	315; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for (; *format != 0; ++format)
	INCF	r0x03, F
	BNC	_01384_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_01384_DS_:
	GOTO	_01216_DS_
_01204_DS_:
;	.line	445; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (out) **out = '\0';
	MOVF	r0x00, W
	IORWF	r0x01, W
	IORWF	r0x02, W
	BZ	_01206_DS_
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrget3
	MOVWF	r0x00
	MOVFF	PRODL, r0x01
	MOVFF	PRODH, r0x02
	CLRF	POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
_01206_DS_:
;	.line	446; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pc;
	MOVF	r0x09, W
	MOVFF	PREINC1, r0x19
	MOVFF	PREINC1, r0x18
	MOVFF	PREINC1, r0x17
	MOVFF	PREINC1, r0x16
	MOVFF	PREINC1, r0x15
	MOVFF	PREINC1, r0x14
	MOVFF	PREINC1, r0x13
	MOVFF	PREINC1, r0x12
	MOVFF	PREINC1, r0x11
	MOVFF	PREINC1, r0x10
	MOVFF	PREINC1, r0x0f
	MOVFF	PREINC1, r0x0e
	MOVFF	PREINC1, r0x0d
	MOVFF	PREINC1, r0x0c
	MOVFF	PREINC1, r0x0b
	MOVFF	PREINC1, r0x0a
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pprintfl	code
_pprintfl:
;	.line	175; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	static u8 pprintfl(u8 **out, float value, u8 width, u8 pad, u8 separator, u8 precision)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVFF	r0x0a, POSTDEC1
	MOVFF	r0x0b, POSTDEC1
	MOVFF	r0x0c, POSTDEC1
	MOVFF	r0x0d, POSTDEC1
	MOVFF	r0x0e, POSTDEC1
	MOVFF	r0x0f, POSTDEC1
	MOVFF	r0x10, POSTDEC1
	MOVFF	r0x11, POSTDEC1
	MOVFF	r0x12, POSTDEC1
	MOVFF	r0x13, POSTDEC1
	MOVFF	r0x14, POSTDEC1
	MOVFF	r0x15, POSTDEC1
	MOVFF	r0x16, POSTDEC1
	MOVFF	r0x17, POSTDEC1
	MOVFF	r0x18, POSTDEC1
	MOVFF	r0x19, POSTDEC1
	MOVFF	r0x1a, POSTDEC1
	MOVFF	r0x1b, POSTDEC1
	MOVFF	r0x1c, POSTDEC1
	MOVFF	r0x1d, POSTDEC1
	MOVFF	r0x1e, POSTDEC1
	MOVFF	r0x1f, POSTDEC1
	MOVFF	r0x20, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
	MOVLW	0x06
	MOVFF	PLUSW2, r0x04
	MOVLW	0x07
	MOVFF	PLUSW2, r0x05
	MOVLW	0x08
	MOVFF	PLUSW2, r0x06
	MOVLW	0x09
	MOVFF	PLUSW2, r0x07
	MOVLW	0x0a
	MOVFF	PLUSW2, r0x08
	MOVLW	0x0c
	MOVFF	PLUSW2, r0x09
;	.line	184; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u32 int_part  = 0;
	CLRF	r0x0a
	CLRF	r0x0b
	CLRF	r0x0c
	CLRF	r0x0d
;	.line	185; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u32 frac_part = 0;
	CLRF	r0x0e
	CLRF	r0x0f
	CLRF	r0x10
	CLRF	r0x11
;	.line	187; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 buffer[PRINT_BUF_LEN], *string = buffer;
	MOVLW	HIGH(_pprintfl_buffer_1_161)
	MOVWF	r0x13
	MOVLW	LOW(_pprintfl_buffer_1_161)
	MOVWF	r0x12
	MOVLW	0x80
	MOVWF	r0x14
;	.line	188; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 tmp[PRINT_BUF_LEN], *s = tmp;
	MOVLW	HIGH(_pprintfl_tmp_1_161)
	MOVWF	r0x16
	MOVLW	LOW(_pprintfl_tmp_1_161)
	MOVWF	r0x15
	MOVLW	0x80
	MOVWF	r0x17
;	.line	189; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 count = 0, m, t;
	CLRF	r0x18
;	.line	190; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 length = PRINT_BUF_LEN - 1;
	MOVLW	0x0b
	MOVWF	r0x19
;	.line	192; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	helper.f = value;
	MOVF	r0x03, W
	BANKSEL	_pprintfl_helper_1_161
	MOVWF	_pprintfl_helper_1_161, B
	MOVF	r0x04, W
; removed redundant BANKSEL
	MOVWF	(_pprintfl_helper_1_161 + 1), B
	MOVF	r0x05, W
; removed redundant BANKSEL
	MOVWF	(_pprintfl_helper_1_161 + 2), B
	MOVF	r0x06, W
; removed redundant BANKSEL
	MOVWF	(_pprintfl_helper_1_161 + 3), B
;	.line	195; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (helper.l < 0)
	BSF	STATUS, 0
; removed redundant BANKSEL
	BTFSS	(_pprintfl_helper_1_161 + 3), 7, B
	BCF	STATUS, 0
	BNC	_01040_DS_
;	.line	197; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (width && (pad & PAD_ZERO))
	MOVF	r0x07, W
	BZ	_01036_DS_
	BTFSS	r0x08, 1
	BRA	_01036_DS_
;	.line	199; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pprintc(out, '-');
	MOVLW	0x2d
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintc
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	200; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++count;
	MOVLW	0x01
	MOVWF	r0x18
;	.line	201; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	--width;
	DECF	r0x07, F
	BRA	_01040_DS_
_01036_DS_:
;	.line	205; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string++ = '-';
	MOVLW	0x2d
	MOVWF	POSTDEC1
	MOVFF	r0x12, FSR0L
	MOVFF	r0x13, PRODL
	MOVF	r0x14, W
	CALL	__gptrput1
	INCF	r0x12, F
	BNC	_01124_DS_
	INFSNZ	r0x13, F
	INCF	r0x14, F
_01124_DS_:
;	.line	206; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	length--;
	MOVLW	0x0a
	MOVWF	r0x19
_01040_DS_:
	BANKSEL	(_pprintfl_helper_1_161 + 2)
;	.line	212; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	exponent = ((helper.l >> 23) & 0xFF) - 127;	
	RLCF	(_pprintfl_helper_1_161 + 2), W, B
; removed redundant BANKSEL
	RLCF	(_pprintfl_helper_1_161 + 3), W, B
	MOVWF	r0x03
	CLRF	r0x04
	MOVLW	0x81
	ADDWF	r0x03, F
; removed redundant BANKSEL
;	.line	215; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	mantissa = (helper.l & 0x7FFFFF) | 0x800000;
	MOVF	_pprintfl_helper_1_161, W, B
	MOVWF	r0x04
; removed redundant BANKSEL
	MOVF	(_pprintfl_helper_1_161 + 1), W, B
	MOVWF	r0x05
	MOVLW	0x7f
; removed redundant BANKSEL
	ANDWF	(_pprintfl_helper_1_161 + 2), W, B
	MOVWF	r0x06
	CLRF	r0x1a
	BSF	r0x06, 7
;	.line	217; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if ( (exponent >= 31) || (exponent < -23) )
	MOVF	r0x03, W
	ADDLW	0x80
	ADDLW	0x61
	BC	_01047_DS_
	MOVF	r0x03, W
	ADDLW	0x80
	ADDLW	0x97
	BC	_01048_DS_
_01047_DS_:
;	.line	219; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	buffer[0] = 'i';
	MOVLW	0x69
	BANKSEL	_pprintfl_buffer_1_161
	MOVWF	_pprintfl_buffer_1_161, B
;	.line	220; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	buffer[1] = 'n';
	MOVLW	0x6e
; removed redundant BANKSEL
	MOVWF	(_pprintfl_buffer_1_161 + 1), B
;	.line	221; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	buffer[2] = 'f';
	MOVLW	0x66
; removed redundant BANKSEL
	MOVWF	(_pprintfl_buffer_1_161 + 2), B
; removed redundant BANKSEL
;	.line	222; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	buffer[3] = '\0';
	CLRF	(_pprintfl_buffer_1_161 + 3), B
;	.line	223; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pprints(out, buffer, width, pad);
	MOVF	r0x08, W
	MOVWF	POSTDEC1
	MOVF	r0x07, W
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(_pprintfl_buffer_1_161)
	MOVWF	POSTDEC1
	MOVLW	LOW(_pprintfl_buffer_1_161)
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprints
	MOVWF	r0x1b
	MOVLW	0x08
	ADDWF	FSR1L, F
	MOVF	r0x1b, W
	BRA	_01070_DS_
_01048_DS_:
;	.line	225; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	else if (exponent >= 23)
	MOVF	r0x03, W
	ADDLW	0x80
	ADDLW	0x69
	BNC	_01045_DS_
;	.line	227; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	int_part = mantissa << (exponent - 23);
	MOVFF	r0x03, r0x1b
	CLRF	r0x1c
	BTFSC	r0x03, 7
	SETF	r0x1c
	MOVLW	0xe9
	ADDWF	r0x1b, F
	BTFSS	STATUS, 0
	DECF	r0x1c, F
	MOVFF	r0x04, r0x0a
	MOVFF	r0x05, r0x0b
	MOVFF	r0x06, r0x0c
	MOVFF	r0x1a, r0x0d
	MOVF	r0x1b, W
	BZ	_01131_DS_
	BN	_01134_DS_
	NEGF	WREG
	BCF	STATUS, 0
_01132_DS_:
	RLCF	r0x0a, F
	RLCF	r0x0b, F
	RLCF	r0x0c, F
	RLCF	r0x0d, F
	ADDLW	0x01
	BNC	_01132_DS_
	BRA	_01049_DS_
_01134_DS_:
	BCF	STATUS, 0
_01133_DS_:
	RRCF	r0x0d, F
	RRCF	r0x0c, F
	RRCF	r0x0b, F
	RRCF	r0x0a, F
	ADDLW	0x01
	BNC	_01133_DS_
_01131_DS_:
	BRA	_01049_DS_
_01045_DS_:
;	.line	229; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	else if (exponent >= 0) 
	BSF	STATUS, 0
	BTFSS	r0x03, 7
	BCF	STATUS, 0
	BTFSC	STATUS, 0
	BRA	_01042_DS_
;	.line	231; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	int_part = mantissa >> (23 - exponent);
	MOVFF	r0x03, r0x1b
	CLRF	r0x1c
	BTFSC	r0x03, 7
	SETF	r0x1c
	MOVF	r0x1b, W
	SUBLW	0x17
	MOVWF	r0x1d
	MOVLW	0x00
	SUBFWB	r0x1c, W
	MOVFF	r0x04, r0x0a
	MOVFF	r0x05, r0x0b
	MOVFF	r0x06, r0x0c
	MOVFF	r0x1a, r0x0d
	MOVF	r0x1d, W
	BZ	_01135_DS_
	BN	_01138_DS_
	NEGF	WREG
	BCF	STATUS, 0
_01136_DS_:
	RRCF	r0x0d, F
	RRCF	r0x0c, F
	RRCF	r0x0b, F
	RRCF	r0x0a, F
	ADDLW	0x01
	BNC	_01136_DS_
	BRA	_01135_DS_
_01138_DS_:
	BCF	STATUS, 0
_01137_DS_:
	RLCF	r0x0a, F
	RLCF	r0x0b, F
	RLCF	r0x0c, F
	RLCF	r0x0d, F
	ADDLW	0x01
	BNC	_01137_DS_
_01135_DS_:
;	.line	232; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	frac_part = (mantissa << (exponent + 1)) & 0xFFFFFF; // mfh
	INFSNZ	r0x1b, F
	INCF	r0x1c, F
	MOVFF	r0x04, r0x1d
	MOVFF	r0x05, r0x1e
	MOVFF	r0x06, r0x1f
	MOVFF	r0x1a, r0x20
	MOVF	r0x1b, W
	BZ	_01139_DS_
	BN	_01142_DS_
	NEGF	WREG
	BCF	STATUS, 0
_01140_DS_:
	RLCF	r0x1d, F
	RLCF	r0x1e, F
	RLCF	r0x1f, F
	RLCF	r0x20, F
	ADDLW	0x01
	BNC	_01140_DS_
	BRA	_01139_DS_
_01142_DS_:
	BCF	STATUS, 0
_01141_DS_:
	RRCF	r0x20, F
	RRCF	r0x1f, F
	RRCF	r0x1e, F
	RRCF	r0x1d, F
	ADDLW	0x01
	BNC	_01141_DS_
_01139_DS_:
	MOVF	r0x1d, W
	MOVWF	r0x0e
	MOVF	r0x1e, W
	MOVWF	r0x0f
	MOVF	r0x1f, W
	MOVWF	r0x10
	CLRF	r0x11
	BRA	_01049_DS_
_01042_DS_:
;	.line	235; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	frac_part = (mantissa & 0xFFFFFF) >> -(exponent + 1);
	CLRF	r0x1a
	CLRF	r0x1b
	BTFSC	r0x03, 7
	SETF	r0x1b
	INFSNZ	r0x03, F
	INCF	r0x1b, F
	COMF	r0x1b, F
	NEGF	r0x03
	BTFSC	STATUS, 2
	INCF	r0x1b, F
	MOVFF	r0x04, r0x0e
	MOVFF	r0x05, r0x0f
	MOVFF	r0x06, r0x10
	MOVFF	r0x1a, r0x11
	MOVF	r0x03, W
	BZ	_01049_DS_
	BN	_01149_DS_
	NEGF	WREG
	BCF	STATUS, 0
_01147_DS_:
	RRCF	r0x11, F
	RRCF	r0x10, F
	RRCF	r0x0f, F
	RRCF	r0x0e, F
	ADDLW	0x01
	BNC	_01147_DS_
	BRA	_01049_DS_
_01149_DS_:
	BCF	STATUS, 0
_01148_DS_:
	RLCF	r0x0e, F
	RLCF	r0x0f, F
	RLCF	r0x10, F
	RLCF	r0x11, F
	ADDLW	0x01
	BNC	_01148_DS_
_01049_DS_:
;	.line	238; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (int_part == 0)
	MOVF	r0x0a, W
	IORWF	r0x0b, W
	IORWF	r0x0c, W
	IORWF	r0x0d, W
	BNZ	_01080_DS_
;	.line	240; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string++ = '0';
	MOVLW	0x30
	MOVWF	POSTDEC1
	MOVFF	r0x12, FSR0L
	MOVFF	r0x13, PRODL
	MOVF	r0x14, W
	CALL	__gptrput1
	INCF	r0x12, F
	BNC	_01150_DS_
	INFSNZ	r0x13, F
	INCF	r0x14, F
_01150_DS_:
;	.line	241; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	length--;
	DECF	r0x19, F
	BRA	_01059_DS_
_01080_DS_:
;	.line	247; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	while (int_part)
	MOVFF	r0x15, r0x03
	MOVFF	r0x16, r0x04
	MOVFF	r0x17, r0x05
	CLRF	r0x06
	MOVFF	r0x19, r0x15
_01051_DS_:
	MOVF	r0x0a, W
	IORWF	r0x0b, W
	IORWF	r0x0c, W
	IORWF	r0x0d, W
	BTFSC	STATUS, 2
	BRA	_01088_DS_
;	.line	249; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	t = int_part % 10;		// decimal base
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x0a
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0a, W
	MOVWF	POSTDEC1
	CALL	__modulong
	MOVWF	r0x16
	MOVLW	0x08
	ADDWF	FSR1L, F
;	.line	250; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*s++ = t + '0';
	MOVLW	0x30
	ADDWF	r0x16, F
	MOVFF	r0x16, POSTDEC1
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrput1
	INCF	r0x03, F
	BNC	_01151_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_01151_DS_:
;	.line	251; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	int_part /= 10;
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	MOVLW	0x0a
	MOVWF	POSTDEC1
	MOVF	r0x0d, W
	MOVWF	POSTDEC1
	MOVF	r0x0c, W
	MOVWF	POSTDEC1
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0a, W
	MOVWF	POSTDEC1
	CALL	__divulong
	MOVWF	r0x0a
	MOVFF	PRODL, r0x0b
	MOVFF	PRODH, r0x0c
	MOVFF	FSR0L, r0x0d
	MOVLW	0x08
	ADDWF	FSR1L, F
;	.line	252; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	m++;					// string's length counter
	INCF	r0x06, F
;	.line	253; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	length--;
	DECF	r0x15, F
	BRA	_01051_DS_
_01088_DS_:
;	.line	256; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	while (m--)
	MOVFF	r0x15, r0x19
	MOVFF	r0x12, r0x0a
	MOVFF	r0x13, r0x0b
	MOVFF	r0x14, r0x0c
_01054_DS_:
	MOVFF	r0x06, r0x0d
	DECF	r0x06, F
	MOVF	r0x0d, W
	BZ	_01089_DS_
;	.line	258; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string++ = *--s;
	MOVLW	0xff
	ADDWF	r0x03, F
	ADDWFC	r0x04, F
	ADDWFC	r0x05, F
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x0d
	MOVFF	r0x0d, POSTDEC1
	MOVFF	r0x0a, FSR0L
	MOVFF	r0x0b, PRODL
	MOVF	r0x0c, W
	CALL	__gptrput1
	INCF	r0x0a, F
	BNC	_01054_DS_
	INFSNZ	r0x0b, F
	INCF	r0x0c, F
_01152_DS_:
	BRA	_01054_DS_
_01089_DS_:
	MOVFF	r0x0a, r0x12
	MOVFF	r0x0b, r0x13
	MOVFF	r0x0c, r0x14
_01059_DS_:
;	.line	271; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (precision > 6)
	MOVLW	0x07
	SUBWF	r0x09, W
	BNC	_01061_DS_
;	.line	272; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	precision = 6;
	MOVLW	0x06
	MOVWF	r0x09
_01061_DS_:
;	.line	275; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (precision > length)
	MOVF	r0x09, W
; #	SUBWF	r0x19, W
; #	BTFSC	STATUS, 0
; #	GOTO	_01063_DS_
; #	MOVFF	r0x19, r0x09
; #	MOVLW	0x01
;	.line	276; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	precision = length;
	SUBWF	r0x19, W
;	.line	279; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (precision >= 1)
	BTFSS	STATUS, 0
	MOVFF	r0x19, r0x09
	MOVLW	0x01
	SUBWF	r0x09, W
	BTFSS	STATUS, 0
	BRA	_01066_DS_
;	.line	282; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string++ = '.';
	MOVLW	0x2e
	MOVWF	POSTDEC1
	MOVFF	r0x12, FSR0L
	MOVFF	r0x13, PRODL
	MOVF	r0x14, W
	CALL	__gptrput1
	INCF	r0x12, F
	BNC	_01156_DS_
	INFSNZ	r0x13, F
	INCF	r0x14, F
_01156_DS_:
;	.line	285; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for (m = 0; m < precision; m++)
	MOVFF	r0x12, r0x03
	MOVFF	r0x13, r0x04
	MOVFF	r0x14, r0x05
	CLRF	r0x06
_01068_DS_:
	MOVF	r0x09, W
	SUBWF	r0x06, W
	BC	_01090_DS_
;	.line	288; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	frac_part = (frac_part << 3) + (frac_part << 1); 
	MOVF	r0x0e, W
	ADDWF	r0x0e, W
	MOVWF	r0x0a
	RLCF	r0x0f, W
	MOVWF	r0x0b
	RLCF	r0x10, W
	MOVWF	r0x0c
	RLCF	r0x11, W
	MOVWF	r0x0d
	MOVF	r0x0a, W
	ADDWF	r0x0a, F
	RLCF	r0x0b, F
	RLCF	r0x0c, F
	RLCF	r0x0d, F
	MOVF	r0x0a, W
	ADDWF	r0x0a, F
	RLCF	r0x0b, F
	RLCF	r0x0c, F
	RLCF	r0x0d, F
	MOVF	r0x0e, W
	ADDWF	r0x0e, W
	MOVWF	r0x15
	RLCF	r0x0f, W
	MOVWF	r0x16
	RLCF	r0x10, W
	MOVWF	r0x17
	RLCF	r0x11, W
	MOVWF	r0x19
	MOVF	r0x15, W
	ADDWF	r0x0a, W
	MOVWF	r0x0e
	MOVF	r0x16, W
	ADDWFC	r0x0b, W
	MOVWF	r0x0f
	MOVF	r0x17, W
	ADDWFC	r0x0c, W
	MOVWF	r0x10
	MOVF	r0x19, W
	ADDWFC	r0x0d, W
; #	MOVWF	r0x11
; #	MOVF	r0x11, W
;	.line	290; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string++ = (frac_part >> 24) + '0';
	MOVWF	r0x0a
	MOVLW	0x30
	ADDWF	r0x0a, F
	MOVFF	r0x0a, POSTDEC1
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrput1
	INCF	r0x03, F
	BNC	_01158_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_01158_DS_:
;	.line	292; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	frac_part &= 0xFFFFFF;
	CLRF	r0x11
;	.line	285; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for (m = 0; m < precision; m++)
	INCF	r0x06, F
	BRA	_01068_DS_
_01090_DS_:
	MOVFF	r0x03, r0x12
	MOVFF	r0x04, r0x13
	MOVFF	r0x05, r0x14
_01066_DS_:
;	.line	297; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string++ = '\0';
	CLRF	POSTDEC1
	MOVFF	r0x12, FSR0L
	MOVFF	r0x13, PRODL
	MOVF	r0x14, W
	CALL	__gptrput1
;	.line	299; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return count + pprints(out, buffer, width, pad);
	MOVF	r0x08, W
	MOVWF	POSTDEC1
	MOVF	r0x07, W
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(_pprintfl_buffer_1_161)
	MOVWF	POSTDEC1
	MOVLW	LOW(_pprintfl_buffer_1_161)
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprints
	MOVWF	r0x00
	MOVLW	0x08
	ADDWF	FSR1L, F
	MOVF	r0x18, W
	ADDWF	r0x00, F
	MOVF	r0x00, W
_01070_DS_:
	MOVFF	PREINC1, r0x20
	MOVFF	PREINC1, r0x1f
	MOVFF	PREINC1, r0x1e
	MOVFF	PREINC1, r0x1d
	MOVFF	PREINC1, r0x1c
	MOVFF	PREINC1, r0x1b
	MOVFF	PREINC1, r0x1a
	MOVFF	PREINC1, r0x19
	MOVFF	PREINC1, r0x18
	MOVFF	PREINC1, r0x17
	MOVFF	PREINC1, r0x16
	MOVFF	PREINC1, r0x15
	MOVFF	PREINC1, r0x14
	MOVFF	PREINC1, r0x13
	MOVFF	PREINC1, r0x12
	MOVFF	PREINC1, r0x11
	MOVFF	PREINC1, r0x10
	MOVFF	PREINC1, r0x0f
	MOVFF	PREINC1, r0x0e
	MOVFF	PREINC1, r0x0d
	MOVFF	PREINC1, r0x0c
	MOVFF	PREINC1, r0x0b
	MOVFF	PREINC1, r0x0a
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pprinti	code
_pprinti:
;	.line	112; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	static u8 pprinti(u8 **out, u32 i, u8 islong, u8 base, u8 sign, u8 width, u8 pad, u8 separator, u8 letterbase)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVFF	r0x0a, POSTDEC1
	MOVFF	r0x0b, POSTDEC1
	MOVFF	r0x0c, POSTDEC1
	MOVFF	r0x0d, POSTDEC1
	MOVFF	r0x0e, POSTDEC1
	MOVFF	r0x0f, POSTDEC1
	MOVFF	r0x10, POSTDEC1
	MOVFF	r0x11, POSTDEC1
	MOVFF	r0x12, POSTDEC1
	MOVFF	r0x13, POSTDEC1
	MOVFF	r0x14, POSTDEC1
	MOVFF	r0x15, POSTDEC1
	MOVFF	r0x16, POSTDEC1
	MOVFF	r0x17, POSTDEC1
	MOVFF	r0x18, POSTDEC1
	MOVFF	r0x19, POSTDEC1
	MOVFF	r0x1a, POSTDEC1
	MOVFF	r0x1b, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
	MOVLW	0x06
	MOVFF	PLUSW2, r0x04
	MOVLW	0x07
	MOVFF	PLUSW2, r0x05
	MOVLW	0x08
	MOVFF	PLUSW2, r0x06
	MOVLW	0x09
	MOVFF	PLUSW2, r0x07
	MOVLW	0x0a
	MOVFF	PLUSW2, r0x08
	MOVLW	0x0b
	MOVFF	PLUSW2, r0x09
	MOVLW	0x0c
	MOVFF	PLUSW2, r0x0a
	MOVLW	0x0d
	MOVFF	PLUSW2, r0x0b
	MOVLW	0x0f
	MOVFF	PLUSW2, r0x0c
;	.line	116; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 neg = 0, pc = 0;
	CLRF	r0x0d
	CLRF	r0x0e
;	.line	117; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u32 t, uns32 = i;
	MOVFF	r0x03, r0x0f
	MOVFF	r0x04, r0x10
	MOVFF	r0x05, r0x11
	MOVFF	r0x06, r0x12
;	.line	119; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (i == 0)
	MOVF	r0x03, W
	IORWF	r0x04, W
	IORWF	r0x05, W
	IORWF	r0x06, W
	BNZ	_00962_DS_
;	.line	121; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	buffer[0] = '0';
	MOVLW	0x30
	BANKSEL	_pprinti_buffer_1_151
	MOVWF	_pprinti_buffer_1_151, B
; removed redundant BANKSEL
;	.line	122; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	buffer[1] = '\0';
	CLRF	(_pprinti_buffer_1_151 + 1), B
;	.line	123; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pprints(out, buffer, width, pad);
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0a, W
	MOVWF	POSTDEC1
	MOVLW	0x80
	MOVWF	POSTDEC1
	MOVLW	HIGH(_pprinti_buffer_1_151)
	MOVWF	POSTDEC1
	MOVLW	LOW(_pprinti_buffer_1_151)
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprints
	MOVWF	r0x13
	MOVLW	0x08
	ADDWF	FSR1L, F
	MOVF	r0x13, W
	BRA	_00983_DS_
_00962_DS_:
;	.line	127; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if  ( (sign) && (base == 10) )          // decimal signed number ?
	MOVF	r0x09, W
	BZ	_00970_DS_
	MOVF	r0x08, W
	XORLW	0x0a
	BNZ	_00970_DS_
;	.line	129; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if ( (islong) && ((s32)i < 0) )     // negative 32-bit ?
	MOVF	r0x07, W
	BZ	_00964_DS_
	MOVF	r0x03, W
	MOVWF	r0x09
	MOVF	r0x04, W
	MOVWF	r0x13
	MOVF	r0x05, W
	MOVWF	r0x14
	MOVF	r0x06, W
	MOVWF	r0x15
	BSF	STATUS, 0
	BTFSS	r0x15, 7
	BCF	STATUS, 0
	BNC	_00964_DS_
;	.line	131; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	neg = 1;
	MOVLW	0x01
	MOVWF	r0x0d
;	.line	132; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	uns32 = - (s32)i;
	COMF	r0x15, W
	MOVWF	r0x12
	COMF	r0x14, W
	MOVWF	r0x11
	COMF	r0x13, W
	MOVWF	r0x10
	COMF	r0x09, W
	MOVWF	r0x0f
	INCF	r0x0f, F
	BNZ	_00964_DS_
	INCF	r0x10, F
	BNZ	_00964_DS_
	INFSNZ	r0x11, F
	INCF	r0x12, F
_00964_DS_:
;	.line	134; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if ( (!islong) && ((s16)i < 0) )    // negative 16-bit ?
	MOVF	r0x07, W
	BNZ	_00970_DS_
	BSF	STATUS, 0
	BTFSS	r0x04, 7
	BCF	STATUS, 0
	BNC	_00970_DS_
;	.line	136; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	neg = 1;
	MOVLW	0x01
	MOVWF	r0x0d
;	.line	137; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	uns32 = - (s16)i;
	COMF	r0x04, F
	NEGF	r0x03
	BTFSC	STATUS, 2
	INCF	r0x04, F
	MOVFF	r0x03, r0x0f
	MOVFF	r0x04, r0x10
	CLRF	WREG
	BTFSC	r0x04, 7
	MOVLW	0xff
	MOVWF	r0x11
	MOVWF	r0x12
_00970_DS_:
;	.line	142; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	string = buffer + PRINT_BUF_LEN - 1;
	MOVLW	HIGH(_pprinti_buffer_1_151 + 11)
	MOVWF	r0x04
	MOVLW	LOW(_pprinti_buffer_1_151 + 11)
	MOVWF	r0x03
	MOVLW	0x80
	MOVWF	r0x05
;	.line	143; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*string = '\0';
	CLRF	POSTDEC1
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	CALL	__gptrput1
;	.line	145; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	while (uns32)
	MOVFF	r0x03, r0x06
	MOVFF	r0x04, r0x07
	MOVFF	r0x05, r0x09
_00974_DS_:
	MOVF	r0x0f, W
	IORWF	r0x10, W
	IORWF	r0x11, W
	IORWF	r0x12, W
	BTFSC	STATUS, 2
	BRA	_00998_DS_
;	.line	147; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	t = uns32 % base;
	MOVFF	r0x08, r0x13
	CLRF	r0x14
	CLRF	r0x15
	CLRF	r0x16
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	CLRF	POSTDEC1
	MOVF	r0x13, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	CALL	__modulong
	MOVWF	r0x17
	MOVFF	PRODL, r0x18
	MOVFF	PRODH, r0x19
	MOVFF	FSR0L, r0x1a
	MOVLW	0x08
	ADDWF	FSR1L, F
;	.line	148; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if ( t >= 10 )
	MOVLW	0x00
	SUBWF	r0x1a, W
	BNZ	_01029_DS_
	MOVLW	0x00
	SUBWF	r0x19, W
	BNZ	_01029_DS_
	MOVLW	0x00
	SUBWF	r0x18, W
	BNZ	_01029_DS_
	MOVLW	0x0a
	SUBWF	r0x17, W
_01029_DS_:
	BNC	_00973_DS_
;	.line	149; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	t += letterbase - '0' - 10;
	MOVFF	r0x0c, r0x1b
	MOVLW	0xc6
	ADDWF	r0x1b, F
	MOVF	r0x1b, W
	ADDWF	r0x17, F
_00973_DS_:
;	.line	150; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*--string = t + '0';
	MOVLW	0xff
	ADDWF	r0x06, F
	ADDWFC	r0x07, F
	ADDWFC	r0x09, F
	MOVLW	0x30
	ADDWF	r0x17, F
	MOVFF	r0x17, POSTDEC1
	MOVFF	r0x06, FSR0L
	MOVFF	r0x07, PRODL
	MOVF	r0x09, W
	CALL	__gptrput1
;	.line	151; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	uns32 /= base;
	MOVF	r0x16, W
	MOVWF	POSTDEC1
	MOVF	r0x15, W
	MOVWF	POSTDEC1
	MOVF	r0x14, W
	MOVWF	POSTDEC1
	MOVF	r0x13, W
	MOVWF	POSTDEC1
	MOVF	r0x12, W
	MOVWF	POSTDEC1
	MOVF	r0x11, W
	MOVWF	POSTDEC1
	MOVF	r0x10, W
	MOVWF	POSTDEC1
	MOVF	r0x0f, W
	MOVWF	POSTDEC1
	CALL	__divulong
	MOVWF	r0x0f
	MOVFF	PRODL, r0x10
	MOVFF	PRODH, r0x11
	MOVFF	FSR0L, r0x12
	MOVLW	0x08
	ADDWF	FSR1L, F
	BRA	_00974_DS_
_00998_DS_:
	MOVFF	r0x06, r0x03
	MOVFF	r0x07, r0x04
	MOVFF	r0x09, r0x05
;	.line	154; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (neg)
	MOVF	r0x0d, W
	BZ	_00982_DS_
;	.line	156; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (width && (pad & PAD_ZERO))
	MOVF	r0x0a, W
	BZ	_00978_DS_
	BTFSS	r0x0b, 1
	BRA	_00978_DS_
;	.line	158; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pprintc(out, '-');
	MOVLW	0x2d
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintc
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	159; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++pc;
	MOVLW	0x01
	MOVWF	r0x0e
;	.line	160; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	--width;
	DECF	r0x0a, F
	BRA	_00982_DS_
_00978_DS_:
;	.line	164; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	*--string = '-';
	MOVLW	0xff
	ADDWF	r0x03, F
	ADDWFC	r0x04, F
	ADDWFC	r0x05, F
	MOVLW	0x2d
	MOVWF	POSTDEC1
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrput1
_00982_DS_:
;	.line	168; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pc + pprints(out, string, width, pad);
	MOVF	r0x0b, W
	MOVWF	POSTDEC1
	MOVF	r0x0a, W
	MOVWF	POSTDEC1
	MOVF	r0x05, W
	MOVWF	POSTDEC1
	MOVF	r0x04, W
	MOVWF	POSTDEC1
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprints
	MOVWF	r0x00
	MOVLW	0x08
	ADDWF	FSR1L, F
	MOVF	r0x0e, W
	ADDWF	r0x00, F
	MOVF	r0x00, W
_00983_DS_:
	MOVFF	PREINC1, r0x1b
	MOVFF	PREINC1, r0x1a
	MOVFF	PREINC1, r0x19
	MOVFF	PREINC1, r0x18
	MOVFF	PREINC1, r0x17
	MOVFF	PREINC1, r0x16
	MOVFF	PREINC1, r0x15
	MOVFF	PREINC1, r0x14
	MOVFF	PREINC1, r0x13
	MOVFF	PREINC1, r0x12
	MOVFF	PREINC1, r0x11
	MOVFF	PREINC1, r0x10
	MOVFF	PREINC1, r0x0f
	MOVFF	PREINC1, r0x0e
	MOVFF	PREINC1, r0x0d
	MOVFF	PREINC1, r0x0c
	MOVFF	PREINC1, r0x0b
	MOVFF	PREINC1, r0x0a
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pprints	code
_pprints:
;	.line	62; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	static int pprints(u8 **out, const u8 *string, u8 width, u8 pad)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVFF	r0x0a, POSTDEC1
	MOVFF	r0x0b, POSTDEC1
	MOVFF	r0x0c, POSTDEC1
	MOVFF	r0x0d, POSTDEC1
	MOVFF	r0x0e, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
	MOVLW	0x06
	MOVFF	PLUSW2, r0x04
	MOVLW	0x07
	MOVFF	PLUSW2, r0x05
	MOVLW	0x08
	MOVFF	PLUSW2, r0x06
	MOVLW	0x09
	MOVFF	PLUSW2, r0x07
;	.line	64; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	register u8 pc = 0;
	CLRF	r0x08
;	.line	65; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	u8 padchar = ' ';
	MOVLW	0x20
	MOVWF	r0x09
;	.line	69; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (width > 0)
	MOVF	r0x06, W
	BZ	_00889_DS_
;	.line	71; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for (ptr = string; *ptr; ++ptr)
	CLRF	r0x0a
	MOVFF	r0x03, r0x0b
	MOVFF	r0x04, r0x0c
	MOVFF	r0x05, r0x0d
_00896_DS_:
	MOVFF	r0x0b, FSR0L
	MOVFF	r0x0c, PRODL
	MOVF	r0x0d, W
	CALL	__gptrget1
	MOVWF	r0x0e
	MOVF	r0x0e, W
	BZ	_00882_DS_
;	.line	72; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++len;
	INCF	r0x0a, F
;	.line	71; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for (ptr = string; *ptr; ++ptr)
	INCF	r0x0b, F
	BNC	_00896_DS_
	INFSNZ	r0x0c, F
	INCF	r0x0d, F
_00952_DS_:
	BRA	_00896_DS_
_00882_DS_:
;	.line	73; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (len >= width)
	MOVF	r0x06, W
	SUBWF	r0x0a, W
	BNC	_00884_DS_
;	.line	74; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	width = 0;
	CLRF	r0x06
	BRA	_00885_DS_
_00884_DS_:
;	.line	76; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	width -= len;
	MOVF	r0x0a, W
	SUBWF	r0x06, F
_00885_DS_:
;	.line	77; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (pad & PAD_ZERO) padchar = '0';
	BTFSS	r0x07, 1
	BRA	_00889_DS_
	MOVLW	0x30
	MOVWF	r0x09
_00889_DS_:
;	.line	79; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (!(pad & PAD_RIGHT))
	BTFSC	r0x07, 0
	BRA	_00921_DS_
	CLRF	r0x07
	MOVFF	r0x06, r0x0a
_00899_DS_:
;	.line	81; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; width > 0; --width)
	MOVF	r0x0a, W
	BNZ	_00898_DS_
	MOVFF	r0x07, r0x08
	MOVFF	r0x0a, r0x06
	BRA	_00921_DS_
_00898_DS_:
;	.line	83; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pprintc(out, padchar);
	MOVF	r0x09, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintc
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	84; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++pc;
	INCF	r0x07, F
;	.line	81; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; width > 0; --width)
	DECF	r0x0a, F
	BRA	_00899_DS_
_00921_DS_:
	MOVFF	r0x08, r0x07
_00902_DS_:
;	.line	87; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; *string ; ++string)
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrget1
	MOVWF	r0x08
	MOVF	r0x08, W
	BZ	_00923_DS_
;	.line	89; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pprintc(out, *string);
	MOVF	r0x08, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintc
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	90; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++pc;
	INCF	r0x07, F
;	.line	87; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; *string ; ++string)
	INCF	r0x03, F
	BNC	_00902_DS_
	INFSNZ	r0x04, F
	INCF	r0x05, F
_00956_DS_:
	BRA	_00902_DS_
_00923_DS_:
	MOVFF	r0x07, r0x03
	MOVFF	r0x06, r0x04
_00905_DS_:
;	.line	92; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; width > 0; --width)
	MOVF	r0x04, W
	BZ	_00894_DS_
;	.line	94; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pprintc(out, padchar);
	MOVF	r0x09, W
	MOVWF	POSTDEC1
	MOVF	r0x02, W
	MOVWF	POSTDEC1
	MOVF	r0x01, W
	MOVWF	POSTDEC1
	MOVF	r0x00, W
	MOVWF	POSTDEC1
	CALL	_pprintc
	MOVLW	0x04
	ADDWF	FSR1L, F
;	.line	95; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++pc;
	INCF	r0x03, F
;	.line	92; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	for ( ; width > 0; --width)
	DECF	r0x04, F
	BRA	_00905_DS_
; =DF= MOVFF: replaced by CRLF/SETF
_00894_DS_:
;	.line	98; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	return pc;
	CLRF	PRODL
	MOVF	r0x03, W
	MOVFF	PREINC1, r0x0e
	MOVFF	PREINC1, r0x0d
	MOVFF	PREINC1, r0x0c
	MOVFF	PREINC1, r0x0b
	MOVFF	PREINC1, r0x0a
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__pprintc	code
_pprintc:
;	.line	42; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	static void pprintc(u8 **str, u8 c)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
;	.line	44; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	if (str)
	MOVF	r0x00, W
	IORWF	r0x01, W
	IORWF	r0x02, W
	BZ	_00869_DS_
;	.line	46; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	**str = c;
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrget3
	MOVWF	r0x04
	MOVFF	PRODL, r0x05
	MOVFF	PRODH, r0x06
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, PRODL
	MOVF	r0x06, W
	CALL	__gptrput1
;	.line	47; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	++(*str);
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrget3
	MOVWF	r0x04
	MOVFF	PRODL, r0x05
	MOVFF	PRODH, r0x06
	INCF	r0x04, F
	BNC	_00876_DS_
	INFSNZ	r0x05, F
	INCF	r0x06, F
_00876_DS_:
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, PRODH
	MOVFF	r0x06, TBLPTRL
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput3
	BRA	_00871_DS_
_00869_DS_:
;	.line	51; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/stdio.c	pputchar(c);
	MOVF	r0x03, W
	MOVWF	POSTDEC1
	MOVFF	INTCON, POSTDEC1
	BCF	INTCON, 7
	PUSH	
	MOVLW	LOW(_00877_DS_)
	MOVWF	TOSL
	MOVLW	HIGH(_00877_DS_)
	MOVWF	TOSH
	MOVLW	UPPER(_00877_DS_)
	MOVWF	TOSU
	BTFSC	PREINC1, 7
	BSF	INTCON, 7
	MOVFF	(_pputchar + 2), PCLATU
	MOVFF	(_pputchar + 1), PCLATH
	BANKSEL	_pputchar
	MOVF	_pputchar, W, B
	MOVWF	PCL
_00877_DS_:
	MOVF	POSTINC1, F
_00871_DS_:
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
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
;	.line	86; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/delay.c	void Delayus(unsigned int microseconds)
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
;	.line	90; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/delay.c	for (i=0; i<microseconds; i++)
	CLRF	r0x02
	CLRF	r0x03
_00852_DS_:
	MOVF	r0x01, W
	SUBWF	r0x03, W
	BNZ	_00863_DS_
	MOVF	r0x00, W
	SUBWF	r0x02, W
_00863_DS_:
	BC	_00854_DS_
	INFSNZ	r0x02, F
	INCF	r0x03, F
	BRA	_00852_DS_
_00854_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__Delayms	code
_Delayms:
;	.line	78; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/delay.c	void Delayms(unsigned int milliseconds)
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
;	.line	82; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/delay.c	for (i=0; i<milliseconds; i++)
	CLRF	r0x02
	CLRF	r0x03
_00834_DS_:
	MOVF	r0x01, W
	SUBWF	r0x03, W
	BNZ	_00845_DS_
	MOVF	r0x00, W
	SUBWF	r0x02, W
_00845_DS_:
	BC	_00836_DS_
;	.line	83; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/delay.c	delay1ktcy(12);
	MOVLW	0x0c
	CALL	_delay1ktcy
;	.line	82; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/core/delay.c	for (i=0; i<milliseconds; i++)
	INFSNZ	r0x02, F
	INCF	r0x03, F
	BRA	_00834_DS_
_00836_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDCInitEndpoint	code
_CDCInitEndpoint:
	BANKSEL	_line_config
;	.line	195; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	line_config.dwDTERate = USB_CDC_BAUD_RATE;
	CLRF	_line_config, B
	MOVLW	0xc2
; removed redundant BANKSEL
	MOVWF	(_line_config + 1), B
	MOVLW	0x01
; removed redundant BANKSEL
	MOVWF	(_line_config + 2), B
; removed redundant BANKSEL
	CLRF	(_line_config + 3), B
; removed redundant BANKSEL
;	.line	196; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	line_config.bCharFormat = USB_CDC_STOP_BITS;
	CLRF	(_line_config + 4), B
; removed redundant BANKSEL
;	.line	197; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	line_config.bParityType = USB_CDC_PARITY;
	CLRF	(_line_config + 5), B
;	.line	198; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	line_config.bDataBits = USB_CDC_DATA_BITS;
	MOVLW	0x08
; removed redundant BANKSEL
	MOVWF	(_line_config + 6), B
	BANKSEL	_zlp
;	.line	199; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue0=0;
	CLRF	_zlp, B
; removed redundant BANKSEL
;	.line	200; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue1=0;
	CLRF	(_zlp + 1), B
; removed redundant BANKSEL
;	.line	201; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue2=0;
	CLRF	(_zlp + 2), B
; removed redundant BANKSEL
;	.line	202; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue3=0;
	CLRF	(_zlp + 3), B
; removed redundant BANKSEL
;	.line	203; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue4=0;
	CLRF	(_zlp + 4), B
; removed redundant BANKSEL
;	.line	204; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue5=0;
	CLRF	(_zlp + 5), B
; removed redundant BANKSEL
;	.line	205; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue6=0;
	CLRF	(_zlp + 6), B
; removed redundant BANKSEL
;	.line	206; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	zlp.wValue7=0;
	CLRF	(_zlp + 7), B
;	.line	211; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	USB_COMM_EP_UEP = EP_IN | HSHK_EN;
	MOVLW	0x1a
	BANKSEL	_UEP2
	MOVWF	_UEP2, B
;	.line	214; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	CDC_DATA_EP_UEP = EP_OUT_IN | HSHK_EN;
	MOVLW	0x1e
	BANKSEL	_UEP3
	MOVWF	_UEP3, B
;	.line	218; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(USB_COMM_EP_NUM).ADDR = PTR16(&CDCControlBuffer);
	MOVLW	LOW(_CDCControlBuffer)
	BANKSEL	(_ep_bdt + 22)
	MOVWF	(_ep_bdt + 22), B
	MOVLW	HIGH(_CDCControlBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 23), B
; #	MOVLW	0x40
; #	MOVWF	(_ep_bdt + 20), B
; #	MOVLW	0x40
;	.line	219; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(USB_COMM_EP_NUM).Stat.uc = BDS_DAT1 | BDS_COWN;
	MOVLW	0x40
; removed redundant BANKSEL
;	.line	222; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).Cnt = sizeof(CDCRxBuffer);
	MOVWF	(_ep_bdt + 20), B
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 25), B
;	.line	223; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).ADDR = PTR16(&CDCRxBuffer);
	MOVLW	LOW(_CDCRxBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 26), B
	MOVLW	HIGH(_CDCRxBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 27), B
;	.line	224; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).Stat.uc = BDS_UOWN | BDS_DAT0 | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 24), B
;	.line	226; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(CDC_DATA_EP_NUM).ADDR = PTR16(&CDCTxBuffer); // +1 
	MOVLW	LOW(_CDCTxBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 30), B
	MOVLW	HIGH(_CDCTxBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 31), B
;	.line	227; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(CDC_DATA_EP_NUM).Stat.uc = BDS_DAT1 | BDS_COWN; 
	MOVLW	0x40
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 28), B
;	.line	228; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	deviceState=CONFIGURED; 
	MOVLW	0x05
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
	RETURN	

; ; Starting pCode block
S_main__CDCputs	code
_CDCputs:
;	.line	153; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	u8 CDCputs(u8 *buffer, u8 length)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
	MOVLW	0x05
	MOVFF	PLUSW2, r0x03
;	.line	155; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	u8 i=0;
	CLRF	r0x04
	BANKSEL	_deviceState
;	.line	157; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (deviceState != CONFIGURED) return 0;
	MOVF	_deviceState, W, B
	XORLW	0x05
	BZ	_00787_DS_
	CLRF	WREG
	BRA	_00798_DS_
_00787_DS_:
	BANKSEL	_CONTROL_LINE
;	.line	158; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (!CONTROL_LINE) return 0;
	MOVF	_CONTROL_LINE, W, B
	BNZ	_00789_DS_
	CLRF	WREG
	BRA	_00798_DS_
_00789_DS_:
	BANKSEL	(_ep_bdt + 28)
;	.line	159; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (!EP_IN_BD(CDC_DATA_EP_NUM).Stat.UOWN)
	BTFSC	(_ep_bdt + 28), 7, B
	BRA	_00794_DS_
;	.line	161; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (length > CDC_BULK_IN_SIZE) length = CDC_BULK_IN_SIZE;
	MOVLW	0x41
	SUBWF	r0x03, W
	BNC	_00805_DS_
	MOVLW	0x40
	MOVWF	r0x03
_00805_DS_:
;	.line	162; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	for (i=0; i < length; i++)
	CLRF	r0x05
_00796_DS_:
	MOVF	r0x03, W
	SUBWF	r0x05, W
	BC	_00806_DS_
;	.line	164; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	CDCTxBuffer[i] = buffer[i];
	MOVLW	LOW(_CDCTxBuffer)
	ADDWF	r0x05, W
	MOVWF	r0x06
	CLRF	r0x07
	MOVLW	HIGH(_CDCTxBuffer)
	ADDWFC	r0x07, F
	MOVF	r0x05, W
	ADDWF	r0x00, W
	MOVWF	r0x08
	CLRF	WREG
	ADDWFC	r0x01, W
	MOVWF	r0x09
	CLRF	WREG
	ADDWFC	r0x02, W
	MOVFF	r0x08, FSR0L
	MOVFF	r0x09, PRODL
	CALL	__gptrget1
	MOVWF	r0x08
	MOVFF	r0x06, FSR0L
	MOVFF	r0x07, FSR0H
	MOVFF	r0x08, INDF0
;	.line	162; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	for (i=0; i < length; i++)
	INCF	r0x05, F
	BRA	_00796_DS_
_00806_DS_:
	MOVFF	r0x05, r0x04
;	.line	176; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(CDC_DATA_EP_NUM).Cnt = i;
	MOVF	r0x05, W
	BANKSEL	(_ep_bdt + 29)
	MOVWF	(_ep_bdt + 29), B
;	.line	178; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(CDC_DATA_EP_NUM).Stat.uc &= 0x40;
	MOVLW	0x40
; removed redundant BANKSEL
	ANDWF	(_ep_bdt + 28), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	MOVWF	(_ep_bdt + 28), B
;	.line	179; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(CDC_DATA_EP_NUM).Stat.DTS = !EP_IN_BD(CDC_DATA_EP_NUM).Stat.DTS;
	CLRF	r0x00
; removed redundant BANKSEL
	BTFSC	(_ep_bdt + 28), 6, B
	INCF	r0x00, F
	MOVF	r0x00, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x00
	RLCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RRNCF	WREG, W
	RRNCF	WREG, W
	MOVWF	PRODH
; removed redundant BANKSEL
	MOVF	(_ep_bdt + 28), W, B
	ANDLW	0xbf
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 28), B
;	.line	181; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_IN_BD(CDC_DATA_EP_NUM).Stat.uc |= BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	IORWF	(_ep_bdt + 28), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	MOVWF	(_ep_bdt + 28), B
_00794_DS_:
;	.line	183; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	return i;
	MOVF	r0x04, W
_00798_DS_:
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__CDCgets	code
_CDCgets:
;	.line	106; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	u8 CDCgets(u8 *buffer)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
	MOVLW	0x03
	MOVFF	PLUSW2, r0x01
	MOVLW	0x04
	MOVFF	PLUSW2, r0x02
;	.line	108; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	u8 i=0;
	CLRF	r0x03
	BANKSEL	_deviceState
;	.line	111; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (deviceState != CONFIGURED)
	MOVF	_deviceState, W, B
	XORLW	0x05
	BZ	_00746_DS_
;	.line	112; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	return 0;
	CLRF	WREG
	BRA	_00757_DS_
_00746_DS_:
	BANKSEL	_CONTROL_LINE
;	.line	115; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (!CONTROL_LINE)
	MOVF	_CONTROL_LINE, W, B
	BNZ	_00748_DS_
;	.line	116; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	return 0;
	CLRF	WREG
	BRA	_00757_DS_
_00748_DS_:
	BANKSEL	(_ep_bdt + 24)
;	.line	119; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (!EP_OUT_BD(CDC_DATA_EP_NUM).Stat.UOWN)
	BTFSC	(_ep_bdt + 24), 7, B
	BRA	_00753_DS_
;	.line	126; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (length > EP_OUT_BD(CDC_DATA_EP_NUM).Cnt)
	MOVLW	0x40
; #	SUBWF	(_ep_bdt + 25), W, B
; #	BTFSC	STATUS, 0
; #	GOTO	_00764_DS_
; #;;;!!! pic16_aopOp:1117 called for a spillLocation -- assigning WREG instead --- CHECK
; #	MOVFF	(_ep_bdt + 25), WREG
; #	CLRF	r0x04
; removed redundant BANKSEL
;	.line	127; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	length = EP_OUT_BD(CDC_DATA_EP_NUM).Cnt;
	SUBWF	(_ep_bdt + 25), W, B
;	.line	129; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	for (i=0; i < EP_OUT_BD(CDC_DATA_EP_NUM).Cnt; i++)
	BTFSS	STATUS, 0
	MOVFF	(_ep_bdt + 25), WREG
	CLRF	r0x04
_00755_DS_:
	BANKSEL	(_ep_bdt + 25)
	MOVF	(_ep_bdt + 25), W, B
	SUBWF	r0x04, W
	BC	_00765_DS_
;	.line	131; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	buffer[i] = CDCRxBuffer[i];
	MOVF	r0x04, W
	ADDWF	r0x00, W
	MOVWF	r0x05
	CLRF	WREG
	ADDWFC	r0x01, W
	MOVWF	r0x06
	CLRF	WREG
	ADDWFC	r0x02, W
	MOVWF	r0x07
	MOVLW	LOW(_CDCRxBuffer)
	ADDWF	r0x04, W
	MOVWF	r0x08
	CLRF	r0x09
	MOVLW	HIGH(_CDCRxBuffer)
	ADDWFC	r0x09, F
	MOVFF	r0x08, FSR0L
	MOVFF	r0x09, FSR0H
	MOVFF	INDF0, r0x08
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x05, FSR0L
	MOVFF	r0x06, PRODL
	MOVF	r0x07, W
	CALL	__gptrput1
;	.line	129; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	for (i=0; i < EP_OUT_BD(CDC_DATA_EP_NUM).Cnt; i++)
	INCF	r0x04, F
	BRA	_00755_DS_
_00765_DS_:
	MOVFF	r0x04, r0x03
;	.line	143; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).Stat.uc &= 0x40;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 24)
	ANDWF	(_ep_bdt + 24), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	MOVWF	(_ep_bdt + 24), B
;	.line	144; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).Stat.DTS = !EP_OUT_BD(CDC_DATA_EP_NUM).Stat.DTS;
	CLRF	r0x00
; removed redundant BANKSEL
	BTFSC	(_ep_bdt + 24), 6, B
	INCF	r0x00, F
	MOVF	r0x00, W
	BSF	STATUS, 0
	TSTFSZ	WREG
	BCF	STATUS, 0
	CLRF	r0x00
	RLCF	r0x00, F
	MOVF	r0x00, W
	ANDLW	0x01
	RRNCF	WREG, W
	RRNCF	WREG, W
	MOVWF	PRODH
; removed redundant BANKSEL
	MOVF	(_ep_bdt + 24), W, B
	ANDLW	0xbf
	IORWF	PRODH, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 24), B
;	.line	146; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).Cnt = sizeof(CDCRxBuffer);
	MOVLW	0x40
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 25), B
;	.line	147; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	EP_OUT_BD(CDC_DATA_EP_NUM).Stat.uc |= BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	IORWF	(_ep_bdt + 24), W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	MOVWF	(_ep_bdt + 24), B
_00753_DS_:
;	.line	150; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	return i;
	MOVF	r0x03, W
_00757_DS_:
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__ProcessCDCRequest	code
_ProcessCDCRequest:
;	.line	29; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	void ProcessCDCRequest(void)
	MOVFF	r0x00, POSTDEC1
;	.line	40; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if ((SetupPacket.bmRequestType & USB_TYPE_MASK) != USB_TYPE_CLASS) return;
	MOVLW	0x60
	BANKSEL	_SetupPacket
	ANDWF	_SetupPacket, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	XORLW	0x20
	BZ	_00693_DS_
	BRA	_00703_DS_
_00693_DS_:
;	.line	46; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	switch(SetupPacket.bRequest)
	MOVFF	(_SetupPacket + 1), r0x00
	MOVF	r0x00, W
	BNZ	_00730_DS_
	BRA	_00703_DS_
_00730_DS_:
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00732_DS_
	BRA	_00703_DS_
_00732_DS_:
	MOVF	r0x00, W
	XORLW	0x20
	BZ	_00696_DS_
	MOVF	r0x00, W
	XORLW	0x21
	BZ	_00697_DS_
	MOVF	r0x00, W
	XORLW	0x22
	BZ	_00698_DS_
	BRA	_00703_DS_
_00696_DS_:
;	.line	68; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	outPtr = (__data u8 *)&line_config;
	MOVLW	HIGH(_line_config)
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_line_config)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	69; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	wCount = sizeof(USB_CDC_Line_Coding) ;
	MOVLW	0x07
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
;	.line	70; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	requestHandled = 1;				
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	71; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	break;
	BRA	_00703_DS_
_00697_DS_:
;	.line	78; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	outPtr = (__data u8 *)&line_config;
	MOVLW	HIGH(_line_config)
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_line_config)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	79; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	wCount = sizeof(USB_CDC_Line_Coding) ;
	MOVLW	0x07
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
;	.line	80; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	81; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	break;
	BRA	_00703_DS_
_00698_DS_:
	BANKSEL	(_SetupPacket + 2)
;	.line	90; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	if (SetupPacket.wValue0==3) CONTROL_LINE=1;
	MOVF	(_SetupPacket + 2), W, B
	XORLW	0x03
	BNZ	_00700_DS_
	MOVLW	0x01
	BANKSEL	_CONTROL_LINE
	MOVWF	_CONTROL_LINE, B
	BRA	_00701_DS_
_00700_DS_:
	BANKSEL	_CONTROL_LINE
;	.line	91; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	else CONTROL_LINE=0;		
	CLRF	_CONTROL_LINE, B
_00701_DS_:
;	.line	92; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	outPtr = (__data u8 *)&zlp;
	MOVLW	HIGH(_zlp)
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_zlp)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	93; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	wCount = sizeof(Zero_Packet_Length) ;
	MOVLW	0x08
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
;	.line	94; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	requestHandled = 1;						
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
_00703_DS_:
;	.line	96; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/usb_cdc.c	}
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__ProcessUSBTransactions	code
_ProcessUSBTransactions:
;	.line	821; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void ProcessUSBTransactions(void)
	MOVFF	r0x00, POSTDEC1
; #	MOVF	_deviceState, W, B
; #	BTFSS	STATUS, 2
; #	GOTO	_00606_DS_
; #	GOTO	_00632_DS_
; #	BTFSS	_UIRbits, 2
	BANKSEL	_deviceState
;	.line	824; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(deviceState == DETACHED)
	MOVF	_deviceState, W, B
;	.line	825; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	return;
	BTFSC	STATUS, 2
;	.line	828; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(UIRbits.ACTVIF && UIEbits.ACTVIE)
	BRA	_00632_DS_
	BTFSS	_UIRbits, 2
; #	GOTO	_00608_DS_
; #	BTFSS	_UIEbits, 2, B
; #	GOTO	_00608_DS_
; #	CALL	_UnSuspend
; #	CLRF	r0x00
;	.line	829; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UnSuspend();
	BRA	_00608_DS_
	BANKSEL	_UIEbits
;	.line	833; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(UCONbits.SUSPND == 1)
	BTFSC	_UIEbits, 2, B
	CALL	_UnSuspend
_00608_DS_:
	CLRF	r0x00
	BTFSC	_UCONbits, 1
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
;	.line	834; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	return;
	BZ	_00632_DS_
;	.line	837; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (UIRbits.URSTIF && UIEbits.URSTIE)
	BTFSS	_UIRbits, 0
; #	GOTO	_00613_DS_
; #	BTFSS	_UIEbits, 0, B
; #	GOTO	_00613_DS_
; #	CALL	_BusReset
; #	BTFSS	_UIRbits, 4
;	.line	838; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	BusReset();
	BRA	_00613_DS_
	BANKSEL	_UIEbits
;	.line	841; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (UIRbits.IDLEIF && UIEbits.IDLEIE)
	BTFSC	_UIEbits, 0, B
	CALL	_BusReset
_00613_DS_:
	BTFSS	_UIRbits, 4
; #	GOTO	_00616_DS_
; #	BTFSS	_UIEbits, 4, B
; #	GOTO	_00616_DS_
; #	CALL	_Suspend
; #	BTFSS	_UIRbits, 6
;	.line	842; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	Suspend();
	BRA	_00616_DS_
	BANKSEL	_UIEbits
;	.line	845; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (UIRbits.SOFIF && UIEbits.SOFIE)
	BTFSC	_UIEbits, 4, B
	CALL	_Suspend
_00616_DS_:
	BTFSS	_UIRbits, 6
; #	GOTO	_00619_DS_
; #	BTFSS	_UIEbits, 6, B
; #	GOTO	_00619_DS_
; #	CALL	_StartOfFrame
; #	BTFSS	_UIRbits, 5
;	.line	846; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	StartOfFrame();
	BRA	_00619_DS_
	BANKSEL	_UIEbits
;	.line	848; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (UIRbits.STALLIF && UIEbits.STALLIE)
	BTFSC	_UIEbits, 6, B
	CALL	_StartOfFrame
_00619_DS_:
	BTFSS	_UIRbits, 5
; #	GOTO	_00622_DS_
; #	BTFSS	_UIEbits, 5, B
; #	GOTO	_00622_DS_
; #	CALL	_Stall
; #	BTFSS	_UIRbits, 1
;	.line	849; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	Stall();
	BRA	_00622_DS_
	BANKSEL	_UIEbits
;	.line	853; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (UIRbits.UERRIF && UIEbits.UERRIE)
	BTFSC	_UIEbits, 5, B
	CALL	_Stall
_00622_DS_:
	BTFSS	_UIRbits, 1
; #	GOTO	_00625_DS_
; #	BTFSS	_UIEbits, 1, B
; #	GOTO	_00625_DS_
; #	BCF	_UIRbits, 1
; #	MOVLW	0x03
;	.line	854; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.UERRIF = 0;
	BRA	_00625_DS_
	BANKSEL	_UIEbits
;	.line	857; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (deviceState < DEFAULT_STATUS)
	BTFSC	_UIEbits, 1, B
	BCF	_UIRbits, 1
_00625_DS_:
	MOVLW	0x03
; #	SUBWF	_deviceState, W, B
; #	BTFSC	STATUS, 0
; #	GOTO	_00628_DS_
; #	GOTO	_00632_DS_
; #	BTFSS	_UIRbits, 3
	BANKSEL	_deviceState
;	.line	858; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	return;
	SUBWF	_deviceState, W, B
;	.line	861; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(UIRbits.TRNIF && UIEbits.TRNIE)
	BNC	_00632_DS_
	BTFSS	_UIRbits, 3
	BRA	_00632_DS_
	BANKSEL	_UIEbits
	BTFSS	_UIEbits, 3, B
	BRA	_00632_DS_
;	.line	863; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ProcessControlTransfer();
	CALL	_ProcessControlTransfer
;	.line	865; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.TRNIF = 0;
	BCF	_UIRbits, 3
_00632_DS_:
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__BusReset	code
_BusReset:
;	.line	793; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void BusReset() {
	MOVFF	r0x00, POSTDEC1
;	.line	794; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UEIR  = 0x00;
	CLRF	_UEIR
;	.line	795; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIR   = 0x00;
	CLRF	_UIR
;	.line	796; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UEIE  = 0x9f;
	MOVLW	0x9f
	BANKSEL	_UEIE
	MOVWF	_UEIE, B
;	.line	797; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIE   = 0x7b;
	MOVLW	0x7b
	BANKSEL	_UIE
	MOVWF	_UIE, B
	BANKSEL	_UADDR
;	.line	798; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UADDR = 0x00;
	CLRF	_UADDR, B
;	.line	801; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UEP0 = EP_CTRL | HSHK_EN;
	MOVLW	0x16
	BANKSEL	_UEP0
	MOVWF	_UEP0, B
_00588_DS_:
;	.line	804; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	while (UIRbits.TRNIF == 1)
	CLRF	r0x00
	BTFSC	_UIRbits, 3
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00590_DS_
;	.line	805; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.TRNIF = 0;
	BCF	_UIRbits, 3
	BRA	_00588_DS_
_00590_DS_:
;	.line	808; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UCONbits.PKTDIS = 0;
	BCF	_UCONbits, 4
;	.line	811; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BANKSEL	_remoteWakeup
;	.line	813; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	remoteWakeup = 0;                     // Remote wakeup is off by default
	CLRF	_remoteWakeup, B
	BANKSEL	_selfPowered
;	.line	814; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	selfPowered = 0;                      // Self powered is off by default
	CLRF	_selfPowered, B
	BANKSEL	_currentConfiguration
;	.line	815; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	currentConfiguration = 0;             // Clear active configuration
	CLRF	_currentConfiguration, B
;	.line	816; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = DEFAULT_STATUS;
	MOVLW	0x03
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__Suspend	code
_Suspend:
;	.line	720; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void Suspend(void)
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	BANKSEL	_UIEbits
;	.line	726; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIEbits.ACTVIE = 1;
	BSF	_UIEbits, 2, B
;	.line	727; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.IDLEIF = 0;
	BCF	_UIRbits, 4
;	.line	728; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UCONbits.SUSPND = 1;
	BSF	_UCONbits, 1
;	.line	733; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	PIR2bits.USBIF = 0;
	BCF	_PIR2bits, 4
;	.line	739; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	INTCONbits.RBIF = 0;
	BCF	_INTCONbits, 0
;	.line	745; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	PIE2bits.USBIE = 1;
	BSF	_PIE2bits, 4
;	.line	751; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	INTCONbits.RBIE = 1;
	BSF	_INTCONbits, 3
	sleep 
;	.line	781; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	PIE2bits.USBIE = 0;
	BCF	_PIE2bits, 4
;	.line	787; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	INTCONbits.RBIE = 0;
	BCF	_INTCONbits, 3
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__Stall	code
_Stall:
;	.line	704; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void Stall(void)
	MOVFF	r0x00, POSTDEC1
;	.line	709; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(UEP0bits.EPSTALL == 1)
	CLRF	r0x00
	BANKSEL	_UEP0bits
	BTFSC	_UEP0bits, 0, B
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00571_DS_
;	.line	712; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BANKSEL	_UEP0bits
;	.line	713; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UEP0bits.EPSTALL = 0;
	BCF	_UEP0bits, 0, B
_00571_DS_:
;	.line	715; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.STALLIF = 0;
	BCF	_UIRbits, 5
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__StartOfFrame	code
_StartOfFrame:
;	.line	700; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.SOFIF = 0;
	BCF	_UIRbits, 6
	RETURN	

; ; Starting pCode block
S_main__UnSuspend	code
_UnSuspend:
;	.line	689; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UCONbits.SUSPND = 0;
	BCF	_UCONbits, 1
	BANKSEL	_UIEbits
;	.line	690; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIEbits.ACTVIE = 0;
	BCF	_UIEbits, 2, B
;	.line	691; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIRbits.ACTVIF = 0;
	BCF	_UIRbits, 2
	RETURN	

; ; Starting pCode block
S_main__EnableUSBModule	code
_EnableUSBModule:
;	.line	655; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(UCONbits.USBEN == 0)
	BTFSC	_UCONbits, 3
	BRA	_00539_DS_
;	.line	660; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UCON = 0;
	CLRF	_UCON
	BANKSEL	_UIE
;	.line	661; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIE = 0;
	CLRF	_UIE, B
;	.line	662; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UCONbits.USBEN = 1;
	BSF	_UCONbits, 3
;	.line	663; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = ATTACHED;
	MOVLW	0x01
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00539_DS_:
	BANKSEL	_deviceState
;	.line	668; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if ((deviceState == ATTACHED) && !UCONbits.SE0)
	MOVF	_deviceState, W, B
	XORLW	0x01
	BNZ	_00543_DS_
	BTFSC	_UCONbits, 5
	BRA	_00543_DS_
;	.line	670; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIR = 0;
	CLRF	_UIR
	BANKSEL	_UIE
;	.line	671; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIE = 0;
	CLRF	_UIE, B
	BANKSEL	_UIEbits
;	.line	672; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIEbits.URSTIE = 1;
	BSF	_UIEbits, 0, B
;	.line	673; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UIEbits.IDLEIE = 1;
	BSF	_UIEbits, 4, B
;	.line	674; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = POWERED;
	MOVLW	0x02
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00543_DS_:
	RETURN	

; ; Starting pCode block
S_main__ProcessControlTransfer	code
_ProcessControlTransfer:
;	.line	563; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void ProcessControlTransfer(void) {
	MOVFF	r0x00, POSTDEC1
;	.line	567; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (USTATbits.DIR == OUT) {
	BTFSC	_USTATbits, 2
	BRA	_00489_DS_
;	.line	570; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 PID = (EP_OUT_BD(0).Stat.uc & 0x3C) >> 2;
	MOVLW	0x3c
	BANKSEL	_ep_bdt
	ANDWF	_ep_bdt, W, B
	MOVWF	r0x00
	RRNCF	r0x00, W
	RRNCF	WREG, W
	ANDLW	0x3f
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
;	.line	571; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (PID == 0x0D)
	XORLW	0x0d
	BNZ	_00473_DS_
;	.line	573; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	SetupStage();
	CALL	_SetupStage
	BRA	_00491_DS_
_00473_DS_:
	BANKSEL	_ctrlTransferStage
;	.line	574; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (ctrlTransferStage == DATA_OUT_STAGE) {
	MOVF	_ctrlTransferStage, W, B
	XORLW	0x01
	BNZ	_00470_DS_
;	.line	577; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	OutDataStage(0);
	MOVLW	0x00
	CLRF	POSTDEC1
	CALL	_OutDataStage
	MOVF	POSTINC1, F
	BANKSEL	_ep_bdt
;	.line	601; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(EP_OUT_BD(0).Stat.DTS)
	BTFSS	_ep_bdt, 6, B
	BRA	_00467_DS_
;	.line	602; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
	BRA	_00491_DS_
_00467_DS_:
;	.line	604; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
	BANKSEL	_ep_bdt
	MOVWF	_ep_bdt, B
	BRA	_00491_DS_
_00470_DS_:
;	.line	608; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
	BRA	_00491_DS_
_00489_DS_:
;	.line	611; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if(USTATbits.DIR == IN) {
	CLRF	r0x00
	BTFSC	_USTATbits, 2
	INCF	r0x00, F
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00491_DS_
_00529_DS_:
	BANKSEL	_UADDR
;	.line	613; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if ((UADDR == 0) && (deviceState == ADDRESS)) {
	MOVF	_UADDR, W, B
	BNZ	_00478_DS_
	BANKSEL	_deviceState
	MOVF	_deviceState, W, B
	XORLW	0x04
	BNZ	_00478_DS_
;	.line	616; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UADDR = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), _UADDR
	BANKSEL	_UADDR
;	.line	620; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(UADDR == 0)
	MOVF	_UADDR, W, B
	BNZ	_00478_DS_
;	.line	623; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = DEFAULT_STATUS;
	MOVLW	0x03
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
_00478_DS_:
	BANKSEL	_ctrlTransferStage
;	.line	626; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (ctrlTransferStage == DATA_IN_STAGE) {
	MOVF	_ctrlTransferStage, W, B
	XORLW	0x02
	BNZ	_00484_DS_
;	.line	628; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	InDataStage(0);
	MOVLW	0x00
	CLRF	POSTDEC1
	CALL	_InDataStage
	MOVF	POSTINC1, F
	BANKSEL	(_ep_bdt + 4)
;	.line	631; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(EP_IN_BD(0).Stat.DTS)
	BTFSS	(_ep_bdt + 4), 6, B
	BRA	_00481_DS_
;	.line	632; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
	BRA	_00491_DS_
_00481_DS_:
;	.line	634; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
	BANKSEL	(_ep_bdt + 4)
	MOVWF	(_ep_bdt + 4), B
	BRA	_00491_DS_
_00484_DS_:
;	.line	638; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	WaitForSetupStage();
	CALL	_WaitForSetupStage
_00491_DS_:
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__WaitForSetupStage	code
_WaitForSetupStage:
	BANKSEL	_ctrlTransferStage
;	.line	550; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ctrlTransferStage = SETUP_STAGE;
	CLRF	_ctrlTransferStage, B
;	.line	551; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 1)
	MOVWF	(_ep_bdt + 1), B
;	.line	552; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
	MOVLW	LOW(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
;	.line	554; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTSEN;
	MOVLW	0x88
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
; removed redundant BANKSEL
;	.line	555; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc = 0x00;           // Give control to CPU
	CLRF	(_ep_bdt + 4), B
	RETURN	

; ; Starting pCode block
S_main__SetupStage	code
_SetupStage:
;	.line	477; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void SetupStage(void) {
	MOVFF	r0x00, POSTDEC1
	BANKSEL	(_ep_bdt + 4)
;	.line	480; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc &= ~BDS_UOWN;
	MOVF	(_ep_bdt + 4), W, B
	MOVWF	r0x00
	BCF	r0x00, 7
	MOVF	r0x00, W
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
; removed redundant BANKSEL
;	.line	481; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc &= ~BDS_UOWN;
	MOVF	_ep_bdt, W, B
	MOVWF	r0x00
	BCF	r0x00, 7
	MOVF	r0x00, W
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
	BANKSEL	_ctrlTransferStage
;	.line	484; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ctrlTransferStage = SETUP_STAGE;
	CLRF	_ctrlTransferStage, B
	BANKSEL	_requestHandled
;	.line	485; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 0;                   // Default is that request hasn't been handled
	CLRF	_requestHandled, B
	BANKSEL	_HIDPostProcess
;	.line	486; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	HIDPostProcess = 0;                   // Assume standard request until know otherwise
	CLRF	_HIDPostProcess, B
	BANKSEL	_wCount
;	.line	487; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = 0;                           // No bytes transferred
	CLRF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
;	.line	490; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ProcessStandardRequest();
	CALL	_ProcessStandardRequest
;	.line	500; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if ((SetupPacket.bmRequestType & USB_RECIP_MASK) == USB_RECIP_INTERFACE)  
	MOVLW	0x1f
	BANKSEL	_SetupPacket
	ANDWF	_SetupPacket, W, B
; #	MOVWF	r0x00
; #	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00426_DS_
;	.line	501; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ProcessCDCRequest();
	CALL	_ProcessCDCRequest
_00426_DS_:
	BANKSEL	_requestHandled
;	.line	504; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (!requestHandled) {
	MOVF	_requestHandled, W, B
	BNZ	_00433_DS_
;	.line	506; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 1)
	MOVWF	(_ep_bdt + 1), B
;	.line	507; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
	MOVLW	LOW(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
; #	MOVLW	0x84
; #	MOVWF	_ep_bdt, B
; #	MOVLW	0x84
;	.line	508; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
	MOVLW	0x84
; removed redundant BANKSEL
;	.line	509; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_BSTALL;
	MOVWF	_ep_bdt, B
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
	BRA	_00434_DS_
_00433_DS_:
	BANKSEL	_SetupPacket
;	.line	511; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (SetupPacket.bmRequestType & 0x80) {
	BTFSS	_SetupPacket, 7, B
	BRA	_00430_DS_
	BANKSEL	(_wCount + 1)
;	.line	513; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(SetupPacket.wLength < wCount)
	MOVF	(_wCount + 1), W, B
	BANKSEL	(_SetupPacket + 7)
	SUBWF	(_SetupPacket + 7), W, B
	BNZ	_00456_DS_
	BANKSEL	_wCount
	MOVF	_wCount, W, B
	BANKSEL	(_SetupPacket + 6)
	SUBWF	(_SetupPacket + 6), W, B
_00456_DS_:
	BC	_00428_DS_
;	.line	514; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = SetupPacket.wLength;
	MOVFF	(_SetupPacket + 6), _wCount
	MOVFF	(_SetupPacket + 7), (_wCount + 1)
_00428_DS_:
;	.line	515; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	InDataStage(0);
	MOVLW	0x00
	CLRF	POSTDEC1
	CALL	_InDataStage
	MOVF	POSTINC1, F
;	.line	516; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ctrlTransferStage = DATA_IN_STAGE;
	MOVLW	0x02
	BANKSEL	_ctrlTransferStage
	MOVWF	_ctrlTransferStage, B
;	.line	518; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
	BANKSEL	(_ep_bdt + 1)
	MOVWF	(_ep_bdt + 1), B
;	.line	519; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).ADDR = PTR16(&SetupPacket);
	MOVLW	LOW(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_SetupPacket)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
;	.line	520; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN;
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
;	.line	523; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).ADDR = PTR16(&controlTransferBuffer);
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 6), B
	MOVLW	HIGH(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 7), B
;	.line	525; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
	BRA	_00434_DS_
_00430_DS_:
;	.line	529; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	ctrlTransferStage = DATA_OUT_STAGE;
	MOVLW	0x01
	BANKSEL	_ctrlTransferStage
	MOVWF	_ctrlTransferStage, B
	BANKSEL	(_ep_bdt + 5)
;	.line	532; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Cnt = 0;
	CLRF	(_ep_bdt + 5), B
;	.line	533; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 4), B
;	.line	536; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Cnt = EP0_BUFFER_SIZE;
	MOVLW	0x40
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 1), B
;	.line	537; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).ADDR = PTR16(&controlTransferBuffer);
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 2), B
	MOVLW	HIGH(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	(_ep_bdt + 3), B
;	.line	539; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_OUT_BD(0).Stat.uc = BDS_UOWN | BDS_DTS | BDS_DTSEN;
	MOVLW	0xc8
; removed redundant BANKSEL
	MOVWF	_ep_bdt, B
_00434_DS_:
;	.line	543; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	UCONbits.PKTDIS = 0;
	BCF	_UCONbits, 4
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__OutDataStage	code
_OutDataStage:
;	.line	441; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void OutDataStage(unsigned char ep) {
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVFF	r0x09, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	444; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	bufferSize = ((0x03 & EP_OUT_BD(ep).Stat.uc) << 8) | EP_OUT_BD(ep).Cnt;
	RLNCF	r0x00, W
	ANDLW	0xfe
; #	MOVWF	r0x01
; #;;multiply lit val:0x04 by variable r0x01 and store in r0x00
; #	MOVF	r0x01, W
	MULLW	0x04
	MOVF	PRODH, W
	MOVWF	r0x02
	MOVFF	PRODL, r0x00
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x01
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, W
	MOVWF	r0x03
	MOVFF	r0x01, FSR0L
	MOVFF	r0x03, FSR0H
	MOVFF	INDF0, r0x01
	MOVLW	0x03
	ANDWF	r0x01, F
	MOVF	r0x01, W
	MOVWF	r0x05
	CLRF	r0x04
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, F
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x02, F
	INFSNZ	r0x00, F
	INCF	r0x02, F
	MOVFF	r0x00, FSR0L
	MOVFF	r0x02, FSR0H
	MOVFF	INDF0, r0x00
	CLRF	r0x01
	MOVF	r0x04, W
	IORWF	r0x00, F
	MOVF	r0x05, W
	IORWF	r0x01, F
;	.line	451; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = wCount + bufferSize;
	MOVF	r0x00, W
	BANKSEL	_wCount
	ADDWF	_wCount, F, B
	MOVF	r0x01, W
; removed redundant BANKSEL
	ADDWFC	(_wCount + 1), F, B
;	.line	453; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = (u8*)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	457; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	for (i=0;i<bufferSize;i++) {
	CLRF	r0x02
	CLRF	r0x03
_00405_DS_:
	MOVF	r0x01, W
	SUBWF	r0x03, W
	BNZ	_00418_DS_
	MOVF	r0x00, W
	SUBWF	r0x02, W
_00418_DS_:
	BC	_00407_DS_
;	.line	461; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	*inPtr++ = *outPtr++;
	MOVFF	_inPtr, r0x04
	MOVFF	(_inPtr + 1), r0x05
	MOVFF	(_inPtr + 2), r0x06
	MOVFF	_outPtr, r0x07
	MOVFF	(_outPtr + 1), r0x08
	MOVFF	(_outPtr + 2), r0x09
	MOVFF	r0x07, FSR0L
	MOVFF	r0x08, PRODL
	MOVF	r0x09, W
	CALL	__gptrget1
	MOVWF	r0x07
	BANKSEL	_outPtr
	INCF	_outPtr, F, B
	BNC	_00419_DS_
; removed redundant BANKSEL
	INCFSZ	(_outPtr + 1), F, B
	BRA	_11897_DS_
; removed redundant BANKSEL
	INCF	(_outPtr + 2), F, B
_11897_DS_:
_00419_DS_:
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, PRODL
	MOVF	r0x06, W
	CALL	__gptrput1
	BANKSEL	_inPtr
	INCF	_inPtr, F, B
	BNC	_00420_DS_
; removed redundant BANKSEL
	INCFSZ	(_inPtr + 1), F, B
	BRA	_21898_DS_
; removed redundant BANKSEL
	INCF	(_inPtr + 2), F, B
_21898_DS_:
_00420_DS_:
;	.line	457; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	for (i=0;i<bufferSize;i++) {
	INFSNZ	r0x02, F
	INCF	r0x03, F
	BRA	_00405_DS_
_00407_DS_:
	MOVFF	PREINC1, r0x09
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__InDataStage	code
_InDataStage:
;	.line	394; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void InDataStage(unsigned char ep) {
	MOVFF	FSR2L, POSTDEC1
	MOVFF	FSR1L, FSR2L
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x07, POSTDEC1
	MOVFF	r0x08, POSTDEC1
	MOVLW	0x02
	MOVFF	PLUSW2, r0x00
;	.line	401; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(wCount < EP0_BUFFER_SIZE)
	MOVLW	0x00
	BANKSEL	(_wCount + 1)
	SUBWF	(_wCount + 1), W, B
	BNZ	_00390_DS_
	MOVLW	0x40
; removed redundant BANKSEL
	SUBWF	_wCount, W, B
_00390_DS_:
	BC	_00372_DS_
;	.line	402; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	bufferSize = wCount;
	MOVFF	_wCount, r0x01
	MOVFF	(_wCount + 1), r0x02
	BRA	_00373_DS_
_00372_DS_:
;	.line	404; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	bufferSize = EP0_BUFFER_SIZE;
	MOVLW	0x40
	MOVWF	r0x01
	CLRF	r0x02
_00373_DS_:
;	.line	412; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(ep).Stat.uc &= ~(BDS_BC8 | BDS_BC9);
	RLNCF	r0x00, W
	ANDLW	0xfe
	MOVWF	r0x03
	INCF	r0x03, W
; #	MOVWF	r0x00
; #;;multiply lit val:0x04 by variable r0x00 and store in r0x00
; #	MOVF	r0x00, W
	MULLW	0x04
	MOVF	PRODH, W
	MOVWF	r0x03
	MOVFF	PRODL, r0x00
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x04
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x03, W
	MOVWF	r0x05
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x06
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x03, W
	MOVWF	r0x07
	MOVFF	r0x06, FSR0L
	MOVFF	r0x07, FSR0H
	MOVFF	INDF0, r0x06
	MOVLW	0xfc
	ANDWF	r0x06, F
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, FSR0H
	MOVFF	r0x06, INDF0
;	.line	413; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(ep).Stat.uc |= (u8)((bufferSize & 0x0300) >> 8);
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x04
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x03, W
	MOVWF	r0x05
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x06
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x03, W
	MOVWF	r0x07
	MOVFF	r0x06, FSR0L
	MOVFF	r0x07, FSR0H
	MOVFF	INDF0, r0x06
	MOVLW	0x03
	ANDWF	r0x02, W
; #	MOVWF	r0x08
; #	MOVF	r0x08, W
	IORWF	r0x06, F
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, FSR0H
	MOVFF	r0x06, INDF0
;	.line	414; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(ep).Cnt = (u8)(bufferSize & 0xFF);
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, W
	MOVWF	r0x04
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x03, W
	MOVWF	r0x05
	INFSNZ	r0x04, F
	INCF	r0x05, F
	MOVF	r0x01, W
	MOVWF	r0x06
	MOVFF	r0x04, FSR0L
	MOVFF	r0x05, FSR0H
	MOVFF	r0x06, INDF0
;	.line	415; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	EP_IN_BD(ep).ADDR = PTR16(&controlTransferBuffer);
	MOVLW	LOW(_ep_bdt)
	ADDWF	r0x00, F
	MOVLW	HIGH(_ep_bdt)
	ADDWFC	r0x03, F
	MOVLW	0x02
	ADDWF	r0x00, F
	BTFSC	STATUS, 0
	INCF	r0x03, F
	MOVLW	LOW(_controlTransferBuffer)
	MOVWF	r0x04
	MOVLW	HIGH(_controlTransferBuffer)
	MOVWF	r0x05
	MOVFF	r0x00, FSR0L
	MOVFF	r0x03, FSR0H
	MOVFF	r0x04, POSTINC0
	MOVFF	r0x05, INDF0
;	.line	420; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = wCount - bufferSize;
	MOVF	r0x01, W
	BANKSEL	_wCount
	SUBWF	_wCount, F, B
	MOVF	r0x02, W
; removed redundant BANKSEL
	SUBWFB	(_wCount + 1), F, B
;	.line	423; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	inPtr = (u8 *)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_inPtr + 1)
	MOVWF	(_inPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_inPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_inPtr + 2), B
;	.line	428; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	for (i=0;i<bufferSize;i++) {
	CLRF	r0x00
_00376_DS_:
	MOVFF	r0x00, r0x03
	CLRF	r0x04
	MOVF	r0x02, W
	SUBWF	r0x04, W
	BNZ	_00396_DS_
	MOVF	r0x01, W
	SUBWF	r0x03, W
_00396_DS_:
	BC	_00378_DS_
;	.line	432; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	*inPtr++ = *outPtr++;
	MOVFF	_inPtr, r0x03
	MOVFF	(_inPtr + 1), r0x04
	MOVFF	(_inPtr + 2), r0x05
	MOVFF	_outPtr, r0x06
	MOVFF	(_outPtr + 1), r0x07
	MOVFF	(_outPtr + 2), r0x08
	MOVFF	r0x06, FSR0L
	MOVFF	r0x07, PRODL
	MOVF	r0x08, W
	CALL	__gptrget1
	MOVWF	r0x06
	BANKSEL	_outPtr
	INCF	_outPtr, F, B
	BNC	_00397_DS_
; removed redundant BANKSEL
	INCFSZ	(_outPtr + 1), F, B
	BRA	_31899_DS_
; removed redundant BANKSEL
	INCF	(_outPtr + 2), F, B
_31899_DS_:
_00397_DS_:
	MOVFF	r0x06, POSTDEC1
	MOVFF	r0x03, FSR0L
	MOVFF	r0x04, PRODL
	MOVF	r0x05, W
	CALL	__gptrput1
	BANKSEL	_inPtr
	INCF	_inPtr, F, B
	BNC	_00398_DS_
; removed redundant BANKSEL
	INCFSZ	(_inPtr + 1), F, B
	BRA	_41900_DS_
; removed redundant BANKSEL
	INCF	(_inPtr + 2), F, B
_41900_DS_:
_00398_DS_:
;	.line	428; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	for (i=0;i<bufferSize;i++) {
	INCF	r0x00, F
	BRA	_00376_DS_
_00378_DS_:
	MOVFF	PREINC1, r0x08
	MOVFF	PREINC1, r0x07
	MOVFF	PREINC1, r0x06
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	MOVFF	PREINC1, FSR2L
	RETURN	

; ; Starting pCode block
S_main__ProcessStandardRequest	code
_ProcessStandardRequest:
;	.line	283; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	void ProcessStandardRequest(void) {
	MOVFF	r0x00, POSTDEC1
;	.line	284; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 request = SetupPacket.bRequest;
	MOVFF	(_SetupPacket + 1), r0x00
	BANKSEL	_SetupPacket
;	.line	286; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if((SetupPacket.bmRequestType & 0x60) != 0x00)
	MOVF	_SetupPacket, W, B
; #	ANDLW	0x60
; #	BTFSC	STATUS, 2
; #	GOTO	_00285_DS_
; #	GOTO	_00317_DS_
; #	MOVF	r0x00, W
;	.line	289; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	return;
	ANDLW	0x60
;	.line	291; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (request == SET_ADDRESS) {
	BTFSS	STATUS, 2
	BRA	_00317_DS_
	MOVF	r0x00, W
	XORLW	0x05
	BNZ	_00315_DS_
;	.line	299; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	300; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = ADDRESS;
	MOVLW	0x04
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
;	.line	301; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceAddress = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), _deviceAddress
	BRA	_00317_DS_
_00315_DS_:
;	.line	303; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == GET_DESCRIPTOR) {
	MOVF	r0x00, W
	XORLW	0x06
	BNZ	_00312_DS_
;	.line	304; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	GetDescriptor();
	CALL	_GetDescriptor
	BRA	_00317_DS_
_00312_DS_:
;	.line	306; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == SET_CONFIGURATION) {
	MOVF	r0x00, W
	XORLW	0x09
	BNZ	_00309_DS_
;	.line	310; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	311; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	currentConfiguration = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), _currentConfiguration
	BANKSEL	_currentConfiguration
;	.line	314; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (currentConfiguration == 0)
	MOVF	_currentConfiguration, W, B
	BNZ	_00287_DS_
;	.line	317; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = ADDRESS;
	MOVLW	0x04
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
	BRA	_00317_DS_
_00287_DS_:
;	.line	320; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	deviceState = CONFIGURED;
	MOVLW	0x05
	BANKSEL	_deviceState
	MOVWF	_deviceState, B
;	.line	330; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	CDCInitEndpoint();
	CALL	_CDCInitEndpoint
	BRA	_00317_DS_
_00309_DS_:
;	.line	340; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == GET_CONFIGURATION) {
	MOVF	r0x00, W
	XORLW	0x08
	BNZ	_00306_DS_
;	.line	344; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	345; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = (u8*)&currentConfiguration;
	MOVLW	HIGH(_currentConfiguration)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_currentConfiguration)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	346; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = 1;
	MOVLW	0x01
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BRA	_00317_DS_
_00306_DS_:
;	.line	348; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == GET_STATUS) {
	MOVF	r0x00, W
	BNZ	_00303_DS_
;	.line	349; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	GetStatus();
	CALL	_GetStatus
	BRA	_00317_DS_
_00303_DS_:
;	.line	351; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if ((request == CLEAR_FEATURE) ||
	MOVF	r0x00, W
	XORLW	0x01
	BZ	_00298_DS_
;	.line	352; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	(request == SET_FEATURE)) {
	MOVF	r0x00, W
	XORLW	0x03
	BNZ	_00299_DS_
_00298_DS_:
;	.line	353; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	SetFeature();
	CALL	_SetFeature
	BRA	_00317_DS_
_00299_DS_:
;	.line	355; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == GET_INTERFACE) {
	MOVF	r0x00, W
	XORLW	0x0a
	BNZ	_00296_DS_
;	.line	361; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
	BANKSEL	_controlTransferBuffer
;	.line	362; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	controlTransferBuffer[0] = 0;
	CLRF	_controlTransferBuffer, B
;	.line	363; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = (u8 *)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	364; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = 1;
	MOVLW	0x01
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BRA	_00317_DS_
_00296_DS_:
;	.line	366; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == SET_INTERFACE) {
	MOVF	r0x00, W
	XORLW	0x0b
	BNZ	_00317_DS_
;	.line	371; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
_00317_DS_:
;	.line	378; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (request == SYNCH_FRAME) {
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__SetFeature	code
_SetFeature:
;	.line	239; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	static void SetFeature(void) {
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
;	.line	240; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 recipient = SetupPacket.bmRequestType & 0x1F;
	MOVLW	0x1f
	BANKSEL	_SetupPacket
	ANDWF	_SetupPacket, W, B
	MOVWF	r0x00
;	.line	241; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 feature = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), r0x01
;	.line	246; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (recipient == 0x00) {
	MOVF	r0x00, W
	BNZ	_00236_DS_
;	.line	248; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (feature == DEVICE_REMOTE_WAKEUP) {
	MOVF	r0x01, W
	XORLW	0x01
	BZ	_00269_DS_
	BRA	_00238_DS_
_00269_DS_:
;	.line	249; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
	BANKSEL	(_SetupPacket + 1)
;	.line	250; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (SetupPacket.bRequest == SET_FEATURE)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x03
	BNZ	_00218_DS_
;	.line	251; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	remoteWakeup = 1;
	MOVLW	0x01
	BANKSEL	_remoteWakeup
	MOVWF	_remoteWakeup, B
	BRA	_00238_DS_
_00218_DS_:
	BANKSEL	_remoteWakeup
;	.line	253; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	remoteWakeup = 0;
	CLRF	_remoteWakeup, B
	BRA	_00238_DS_
_00236_DS_:
;	.line	257; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (recipient == 0x02) {
	MOVF	r0x00, W
	XORLW	0x02
	BZ	_00273_DS_
	BRA	_00238_DS_
_00273_DS_:
;	.line	259; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 endpointNum = SetupPacket.wIndex0 & 0x0F;
	MOVLW	0x0f
	BANKSEL	(_SetupPacket + 4)
	ANDWF	(_SetupPacket + 4), W, B
	MOVWF	r0x00
;	.line	260; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 endpointDir = SetupPacket.wIndex0 & 0x80;
	MOVLW	0x80
; removed redundant BANKSEL
	ANDWF	(_SetupPacket + 4), W, B
	MOVWF	r0x02
;	.line	261; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if ((feature == ENDPOINT_HALT) && (endpointNum != 0)) {
	MOVF	r0x01, W
	BTFSS	STATUS, 2
	BRA	_00238_DS_
	MOVF	r0x00, W
	BTFSC	STATUS, 2
	BRA	_00238_DS_
;	.line	263; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	266; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	inPtr = (u8 *)&EP_OUT_BD(0) + (endpointNum * 8);
	MOVLW	HIGH(_ep_bdt)
	MOVWF	r0x03
	MOVLW	LOW(_ep_bdt)
	MOVWF	r0x01
	MOVLW	0x80
	MOVWF	r0x04
; ;multiply lit val:0x08 by variable r0x00 and store in r0x00
	MOVF	r0x00, W
	MULLW	0x08
	MOVF	PRODH, W
	MOVWF	r0x05
	MOVFF	PRODL, r0x00
	MOVF	r0x00, W
	ADDWF	r0x01, W
	BANKSEL	_inPtr
	MOVWF	_inPtr, B
	MOVF	r0x05, W
	ADDWFC	r0x03, W
; removed redundant BANKSEL
	MOVWF	(_inPtr + 1), B
	CLRF	WREG
	BTFSC	r0x05, 7
	SETF	WREG
	ADDWFC	r0x04, W
; removed redundant BANKSEL
	MOVWF	(_inPtr + 2), B
;	.line	267; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (endpointDir)
	MOVF	r0x02, W
	BZ	_00223_DS_
;	.line	268; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	inPtr += 4;
	MOVLW	0x04
; removed redundant BANKSEL
	ADDWF	_inPtr, F, B
	MOVLW	0x00
; removed redundant BANKSEL
	ADDWFC	(_inPtr + 1), F, B
; removed redundant BANKSEL
	ADDWFC	(_inPtr + 2), F, B
_00223_DS_:
	BANKSEL	(_SetupPacket + 1)
;	.line	270; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(SetupPacket.bRequest == SET_FEATURE)
	MOVF	(_SetupPacket + 1), W, B
	XORLW	0x03
	BNZ	_00228_DS_
;	.line	271; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	*inPtr = 0x84;
	MOVFF	_inPtr, r0x00
	MOVFF	(_inPtr + 1), r0x01
	MOVFF	(_inPtr + 2), r0x03
	MOVLW	0x84
	MOVWF	POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x03, W
	CALL	__gptrput1
	BRA	_00238_DS_
_00228_DS_:
;	.line	273; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(endpointDir == 1)
	MOVF	r0x02, W
	XORLW	0x01
	BNZ	_00225_DS_
;	.line	274; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	*inPtr = 0x00;
	MOVFF	_inPtr, r0x00
	MOVFF	(_inPtr + 1), r0x01
	MOVFF	(_inPtr + 2), r0x02
	CLRF	POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
	BRA	_00238_DS_
_00225_DS_:
;	.line	276; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	*inPtr = 0x88;
	MOVFF	_inPtr, r0x00
	MOVFF	(_inPtr + 1), r0x01
	MOVFF	(_inPtr + 2), r0x02
	MOVLW	0x88
	MOVWF	POSTDEC1
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrput1
_00238_DS_:
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__GetStatus	code
_GetStatus:
;	.line	194; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	static void GetStatus(void) {
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	MOVFF	r0x04, POSTDEC1
	MOVFF	r0x05, POSTDEC1
;	.line	196; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 recipient = SetupPacket.bmRequestType & 0x1F;
	MOVLW	0x1f
	BANKSEL	_SetupPacket
	ANDWF	_SetupPacket, W, B
	MOVWF	r0x00
	BANKSEL	_controlTransferBuffer
;	.line	200; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	controlTransferBuffer[0] = 0;
	CLRF	_controlTransferBuffer, B
; removed redundant BANKSEL
;	.line	201; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	controlTransferBuffer[1] = 0;
	CLRF	(_controlTransferBuffer + 1), B
;	.line	204; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (recipient == 0x00) {
	MOVF	r0x00, W
	BNZ	_00173_DS_
;	.line	206; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
	BANKSEL	_selfPowered
;	.line	208; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (selfPowered)
	MOVF	_selfPowered, W, B
	BZ	_00160_DS_
;	.line	209; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	controlTransferBuffer[0] |= 0x01;
	MOVLW	0x01
	BANKSEL	_controlTransferBuffer
	IORWF	_controlTransferBuffer, W, B
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
	MOVWF	_controlTransferBuffer, B
_00160_DS_:
	BANKSEL	_remoteWakeup
;	.line	210; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (remoteWakeup)
	MOVF	_remoteWakeup, W, B
	BTFSC	STATUS, 2
	BRA	_00174_DS_
;	.line	211; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	controlTransferBuffer[0] |= 0x02;
	MOVLW	0x02
	BANKSEL	_controlTransferBuffer
	IORWF	_controlTransferBuffer, W, B
; #	MOVWF	r0x01
; #	MOVF	r0x01, W
	MOVWF	_controlTransferBuffer, B
	BRA	_00174_DS_
_00173_DS_:
;	.line	213; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (recipient == 0x01) {
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00170_DS_
;	.line	215; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
	BRA	_00174_DS_
_00170_DS_:
;	.line	217; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (recipient == 0x02) {
	MOVF	r0x00, W
	XORLW	0x02
	BZ	_00209_DS_
	BRA	_00174_DS_
_00209_DS_:
;	.line	219; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 endpointNum = SetupPacket.wIndex0 & 0x0F;
	MOVLW	0x0f
	BANKSEL	(_SetupPacket + 4)
	ANDWF	(_SetupPacket + 4), W, B
	MOVWF	r0x00
;	.line	220; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 endpointDir = SetupPacket.wIndex0 & 0x80;
	MOVLW	0x80
; removed redundant BANKSEL
	ANDWF	(_SetupPacket + 4), W, B
	MOVWF	r0x01
;	.line	221; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	224; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	inPtr = (u8 *)&EP_OUT_BD(0) + (endpointNum * 8);
	MOVLW	HIGH(_ep_bdt)
	MOVWF	r0x03
	MOVLW	LOW(_ep_bdt)
	MOVWF	r0x02
	MOVLW	0x80
	MOVWF	r0x04
; ;multiply lit val:0x08 by variable r0x00 and store in r0x00
	MOVF	r0x00, W
	MULLW	0x08
	MOVF	PRODH, W
	MOVWF	r0x05
	MOVFF	PRODL, r0x00
	MOVF	r0x00, W
	ADDWF	r0x02, W
	BANKSEL	_inPtr
	MOVWF	_inPtr, B
	MOVF	r0x05, W
	ADDWFC	r0x03, W
; removed redundant BANKSEL
	MOVWF	(_inPtr + 1), B
	CLRF	WREG
	BTFSC	r0x05, 7
	SETF	WREG
	ADDWFC	r0x04, W
; removed redundant BANKSEL
	MOVWF	(_inPtr + 2), B
;	.line	225; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (endpointDir)
	MOVF	r0x01, W
	BZ	_00164_DS_
;	.line	226; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	inPtr += 4;
	MOVLW	0x04
; removed redundant BANKSEL
	ADDWF	_inPtr, F, B
	MOVLW	0x00
; removed redundant BANKSEL
	ADDWFC	(_inPtr + 1), F, B
; removed redundant BANKSEL
	ADDWFC	(_inPtr + 2), F, B
_00164_DS_:
;	.line	227; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(*inPtr & BDS_BSTALL)
	MOVFF	_inPtr, r0x00
	MOVFF	(_inPtr + 1), r0x01
	MOVFF	(_inPtr + 2), r0x02
	MOVFF	r0x00, FSR0L
	MOVFF	r0x01, PRODL
	MOVF	r0x02, W
	CALL	__gptrget1
	MOVWF	r0x00
	BTFSS	r0x00, 2
	BRA	_00174_DS_
;	.line	228; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	controlTransferBuffer[0] = 0x01;
	MOVLW	0x01
	BANKSEL	_controlTransferBuffer
	MOVWF	_controlTransferBuffer, B
_00174_DS_:
	BANKSEL	_requestHandled
;	.line	231; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (requestHandled) {
	MOVF	_requestHandled, W, B
	BZ	_00177_DS_
;	.line	232; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = (u8 *)&controlTransferBuffer;
	MOVLW	HIGH(_controlTransferBuffer)
	BANKSEL	(_outPtr + 1)
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_controlTransferBuffer)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
	MOVLW	0x80
; removed redundant BANKSEL
	MOVWF	(_outPtr + 2), B
;	.line	233; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = 2;
	MOVLW	0x02
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
_00177_DS_:
	MOVFF	PREINC1, r0x05
	MOVFF	PREINC1, r0x04
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main__GetDescriptor	code
_GetDescriptor:
;	.line	109; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	static void GetDescriptor(void)
	MOVFF	r0x00, POSTDEC1
	MOVFF	r0x01, POSTDEC1
	MOVFF	r0x02, POSTDEC1
	MOVFF	r0x03, POSTDEC1
	BANKSEL	_SetupPacket
;	.line	115; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if(SetupPacket.bmRequestType == 0x80)
	MOVF	_SetupPacket, W, B
	XORLW	0x80
	BZ	_00146_DS_
	BRA	_00128_DS_
_00146_DS_:
;	.line	117; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 descriptorType  = SetupPacket.wValue1;
	MOVFF	(_SetupPacket + 3), r0x00
;	.line	118; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	u8 descriptorIndex = SetupPacket.wValue0;
	MOVFF	(_SetupPacket + 2), r0x01
;	.line	120; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	if (descriptorType == DEVICE_DESCRIPTOR)
	MOVF	r0x00, W
	XORLW	0x01
	BNZ	_00124_DS_
;	.line	125; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	126; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = (u8 *)&libdevice_descriptor;
	MOVLW	UPPER(_libdevice_descriptor)
	BANKSEL	(_outPtr + 2)
	MOVWF	(_outPtr + 2), B
	MOVLW	HIGH(_libdevice_descriptor)
; removed redundant BANKSEL
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_libdevice_descriptor)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
;	.line	127; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = sizeof(USB_Device_Descriptor);
	MOVLW	0x12
	BANKSEL	_wCount
	MOVWF	_wCount, B
; removed redundant BANKSEL
	CLRF	(_wCount + 1), B
	BRA	_00128_DS_
_00124_DS_:
;	.line	130; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (descriptorType == CONFIGURATION_DESCRIPTOR)
	MOVF	r0x00, W
	XORLW	0x02
	BNZ	_00121_DS_
;	.line	136; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
;	.line	148; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = (u8 *)&libconfiguration_descriptor;
	MOVLW	UPPER(_libconfiguration_descriptor)
	BANKSEL	(_outPtr + 2)
	MOVWF	(_outPtr + 2), B
	MOVLW	HIGH(_libconfiguration_descriptor)
; removed redundant BANKSEL
	MOVWF	(_outPtr + 1), B
	MOVLW	LOW(_libconfiguration_descriptor)
; removed redundant BANKSEL
	MOVWF	_outPtr, B
;	.line	150; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = libconfiguration_descriptor.Header.wTotalLength;
	MOVLW	LOW(_libconfiguration_descriptor + 2)
	MOVWF	TBLPTRL
	MOVLW	HIGH(_libconfiguration_descriptor + 2)
	MOVWF	TBLPTRH
	MOVLW	UPPER(_libconfiguration_descriptor + 2)
	MOVWF	TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, _wCount
	TBLRD*+	
	MOVFF	TABLAT, (_wCount + 1)
	BRA	_00128_DS_
_00121_DS_:
;	.line	158; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (descriptorType == STRING_DESCRIPTOR)
	MOVF	r0x00, W
	XORLW	0x03
	BNZ	_00118_DS_
;	.line	164; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
; ;multiply lit val:0x03 by variable r0x01 and store in r0x01
;	.line	166; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	outPtr = libstring_descriptor[descriptorIndex];
	MOVF	r0x01, W
	MULLW	0x03
	MOVF	PRODH, W
	MOVWF	r0x02
	MOVFF	PRODL, r0x01
	CLRF	r0x03
	MOVLW	LOW(_libstring_descriptor)
	ADDWF	r0x01, F
	MOVLW	HIGH(_libstring_descriptor)
	ADDWFC	r0x02, F
	MOVLW	UPPER(_libstring_descriptor)
	ADDWFC	r0x03, F
	MOVFF	r0x01, TBLPTRL
	MOVFF	r0x02, TBLPTRH
	MOVFF	r0x03, TBLPTRU
	TBLRD*+	
	MOVFF	TABLAT, _outPtr
	TBLRD*+	
	MOVFF	TABLAT, (_outPtr + 1)
	TBLRD*+	
	MOVFF	TABLAT, (_outPtr + 2)
;	.line	167; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	wCount = *outPtr;
	MOVFF	_outPtr, r0x01
	MOVFF	(_outPtr + 1), r0x02
	MOVFF	(_outPtr + 2), r0x03
	MOVFF	r0x01, FSR0L
	MOVFF	r0x02, PRODL
	MOVF	r0x03, W
	CALL	__gptrget1
	MOVWF	r0x01
	MOVFF	r0x01, _wCount
	BANKSEL	(_wCount + 1)
	CLRF	(_wCount + 1), B
	BRA	_00128_DS_
_00118_DS_:
;	.line	170; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	else if (descriptorType == DEVICE_QUALIFIER_DESCRIPTOR)
	MOVF	r0x00, W
	XORLW	0x06
	BNZ	_00128_DS_
;	.line	176; /dvpt/pinguino/git/pinguino-ide/pinguino/p8/pinguino/libraries/usb/picUSB.c	requestHandled = 1;
	MOVLW	0x01
	BANKSEL	_requestHandled
	MOVWF	_requestHandled, B
_00128_DS_:
	MOVFF	PREINC1, r0x03
	MOVFF	PREINC1, r0x02
	MOVFF	PREINC1, r0x01
	MOVFF	PREINC1, r0x00
	RETURN	

; ; Starting pCode block
S_main___startup	code
__startup:
	; Initialize the stack pointer
	lfsr 1, _stack_end
	lfsr 2, _stack_end
	; 1st silicon does not do this on POR
	clrf _TBLPTRU, 0
	; Initialize the flash memory access configuration.
	; This is harmless for non-flash devices, so we do it on all parts.
	bsf 0xa6, 7, 0 ; EECON1.EEPGD = 1, TBLPTR accesses program memory
	bcf 0xa6, 6, 0 ; EECON1.CFGS = 0, TBLPTR accesses program memory
	; Load FSR0 with top of RAM.
	movlw 0xbf
	movwf _FSR0L, 0
	movlw 0x0e
	movwf _FSR0H, 0
	; Place 0xff at address 0x00 as a sentinel.
	setf 0x00, 0
clear_loop:
	clrf _POSTDEC0, 0
	movf 0x00, w, 0
	bnz clear_loop
	; TBLPTR = &cinit
	movlw low(_cinit)
	movwf _TBLPTRL, 0
	movlw high(_cinit)
	movwf _TBLPTRH, 0
	movlw upper(_cinit)
	movwf _TBLPTRU, 0
	; 0x05 = cinit.num_init
	tblrd*+
	movff _TABLAT, 0x05
	tblrd*+
	movff _TABLAT, (0x05 + 1)
	; while (0x05)
	bra entry_loop_dec
entry_loop:
	; Count down so we only have to look up the data in _cinit once.
	; At this point we know that TBLPTR points to the top of the current
	; entry in _cinit, so we can just start reading the from, to, and
	; size values.
	; Read the source address low.
	; 0x00 = 0x07 ->from;
	tblrd*+
	movff _TABLAT, 0x00
	; source address high
	tblrd*+
	movff _TABLAT, (0x00 + 1)
	; source address upper
	tblrd*+
	movff _TABLAT, (0x00 + 2)
	; Skip a byte since it is stored as a 32bit int.
	tblrd*+
	; Read the destination address directly into FSR0
	; destination address low.
	; FSR0 = (unsigned short)0x07 ->to;
	tblrd*+
	movff _TABLAT, _FSR0L
	; destination address high
	tblrd*+
	movff _TABLAT, _FSR0H
	; Skip two bytes since it is stored as a 32bit int.
	tblrd*+
	tblrd*+
	; Read the size of data to transfer to destination address.
	; 0x03 = (unsigned short)0x07 ->size;
	tblrd*+
	movff _TABLAT, 0x03
	tblrd*+
	movff _TABLAT, (0x03 + 1)
	; Skip two bytes since it is stored as a 32bit int.
	tblrd*+
	tblrd*+
	; 0x00 = 0x07 ->from;
	; FSR0 = (unsigned short)0x07 ->to;
	; 0x03 = (unsigned short)0x07 ->size;
	; The table pointer now points to the next entry. Save it
	; off since we will be using the table pointer to do the copying
	; for the entry.
	; 0x07 = TBLPTR
	movff _TBLPTRL, 0x07
	movff _TBLPTRH, (0x07 + 1)
	movff _TBLPTRU, (0x07 + 2)
	; Now assign the source address to the table pointer.
	; TBLPTR = 0x00
	movff 0x00, _TBLPTRL
	movff (0x00 + 1), _TBLPTRH
	movff (0x00 + 2), _TBLPTRU
	bra copy_loop_dec
copy_loop:
	tblrd*+
	movff _TABLAT, _POSTINC0
copy_loop_dec:
	; while (--0x03);
	; Decrement and test the byte counter.
	; The cycle ends when the value of counter reaches the -1.
	decf 0x03, f, 0
	bc copy_loop
	decf (0x03 + 1), f, 0
	bc copy_loop
	; Restore the table pointer for the next entry.
	; TBLPTR = 0x07
	movff 0x07, _TBLPTRL
	movff (0x07 + 1), _TBLPTRH
	movff (0x07 + 2), _TBLPTRU
entry_loop_dec:
	; while (--0x05);
	; Decrement and test the entry counter.
	; The cycle ends when the value of counter reaches the -1.
	decf 0x05, f, 0
	bc entry_loop
	decf (0x05 + 1), f, 0
	bc entry_loop
	
;	.line	259; /dvpt/pinguino/git/pinguino-ide/pinguino/source/crt0iz.c	main ();
	CALL	_main
lockup:
	; Returning from main will lock up.
	bra lockup
	
; ; Starting pCode block for Ival
	code
_libdevice_descriptor:
	DB	0x12, 0x01, 0x00, 0x02, 0x02, 0x00, 0x00, 0x40, 0xd8, 0x04, 0xab, 0xfe
	DB	0x00, 0x00, 0x01, 0x02, 0x00, 0x01
; ; Starting pCode block for Ival
_libconfiguration_descriptor:
	DB	0x09, 0x02, 0x43, 0x00, 0x02, 0x01, 0x00, 0xc0, 0x14, 0x09, 0x04, 0x00
	DB	0x00, 0x01, 0x02, 0x02, 0x01, 0x00, 0x05, 0x24, 0x00, 0x10, 0x01, 0x04
	DB	0x24, 0x02, 0x00, 0x05, 0x24, 0x06, 0x00, 0x01, 0x05, 0x24, 0x01, 0x00
	DB	0x01, 0x07, 0x05, 0x82, 0x03, 0x10, 0x00, 0x02, 0x09, 0x04, 0x01, 0x00
	DB	0x02, 0x0a, 0x00, 0x00, 0x00, 0x07, 0x05, 0x03, 0x02, 0x40, 0x00, 0x00
	DB	0x07, 0x05, 0x83, 0x02, 0x40, 0x00, 0x00
; ; Starting pCode block for Ival
_lang:
	DB	0x04, 0x03, 0x09, 0x04
; ; Starting pCode block for Ival
_manu:
	DB	0x16, 0x03, 0x4a, 0x00, 0x50, 0x00, 0x20, 0x00, 0x4d, 0x00, 0x41, 0x00
	DB	0x4e, 0x00, 0x44, 0x00, 0x4f, 0x00, 0x4e, 0x00, 0x20, 0x00
; ; Starting pCode block for Ival
_prod:
	DB	0x12, 0x03, 0x50, 0x00, 0x69, 0x00, 0x6e, 0x00, 0x67, 0x00, 0x75, 0x00
	DB	0x69, 0x00, 0x6e, 0x00, 0x6f, 0x00
; ; Starting pCode block for Ival
_libstring_descriptor:
	DB	LOW(_lang), HIGH(_lang), UPPER(_lang), LOW(_manu), HIGH(_manu), UPPER(_manu), LOW(_prod), HIGH(_prod), UPPER(_prod)
; ; Starting pCode block
__str_0:
	DB	0x28, 0x6e, 0x75, 0x6c, 0x6c, 0x29, 0x00
; ; Starting pCode block
__str_1:
	DB	0x25, 0x66, 0x00
; ; Starting pCode block
__str_2:
	DB	0x25, 0x64, 0x00
; ; Starting pCode block
__str_3:
	DB	0x25, 0x78, 0x00
; ; Starting pCode block
__str_4:
	DB	0x25, 0x6f, 0x00
; ; Starting pCode block
__str_5:
	DB	0x25, 0x62, 0x00
; ; Starting pCode block
__str_6:
	DB	0x0a, 0x0d, 0x00
; ; Starting pCode block
__str_7:
	DB	0x25, 0x63, 0x00
; ; Starting pCode block for Ival
_mask:
	DB	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x01, 0x02, 0x04, 0x08
	DB	0x10, 0x01, 0x02, 0x04, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
	DB	0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80
; ; Starting pCode block for Ival
_port:
	DB	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00
	DB	0x00, 0x04, 0x04, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02
	DB	0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03
; ; Starting pCode block
__str_8:
	DB	0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
	DB	0x2d, 0x2d, 0x2d, 0x4c, 0x41, 0x53, 0x54, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
	DB	0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x00
; ; Starting pCode block
__str_9:
	DB	0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
	DB	0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d
	DB	0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x2d, 0x00
; ; Starting pCode block
__str_10:
	DB	0x25, 0x75, 0x20, 0x64, 0x61, 0x74, 0x61, 0x5f, 0x77, 0x72, 0x69, 0x74
	DB	0x65, 0x20, 0x00
; ; Starting pCode block
__str_11:
	DB	0x25, 0x75, 0x20, 0x64, 0x61, 0x74, 0x61, 0x5f, 0x72, 0x65, 0x61, 0x64
	DB	0x5f, 0x72, 0x65, 0x61, 0x64, 0x20, 0x00
; ; Starting pCode block
__str_12:
	DB	0x25, 0x75, 0x20, 0x64, 0x61, 0x74, 0x61, 0x5f, 0x72, 0x65, 0x61, 0x64
	DB	0x5f, 0x72, 0x65, 0x61, 0x64, 0x5f, 0x65, 0x6e, 0x64, 0x20, 0x00
; ; Starting pCode block
__str_13:
	DB	0x0a, 0x0d, 0x0a, 0x0d, 0x2a, 0x2a, 0x2a, 0x20, 0x32, 0x34, 0x78, 0x78
	DB	0x35, 0x31, 0x32, 0x20, 0x69, 0x32, 0x63, 0x20, 0x45, 0x65, 0x70, 0x72
	DB	0x6f, 0x6d, 0x20, 0x44, 0x65, 0x6d, 0x6f, 0x2e, 0x20, 0x2a, 0x2a, 0x2a
	DB	0x0a, 0x0d, 0x0a, 0x0d, 0x00
; ; Starting pCode block
__str_14:
	DB	0x57, 0x72, 0x69, 0x74, 0x69, 0x6e, 0x67, 0x20, 0x2e, 0x2e, 0x2e, 0x0a
	DB	0x0d, 0x00
; ; Starting pCode block
__str_15:
	DB	0x52, 0x65, 0x61, 0x64, 0x69, 0x6e, 0x67, 0x20, 0x2e, 0x2e, 0x2e, 0x0a
	DB	0x0d, 0x00
; ; Starting pCode block
__str_16:
	DB	0x25, 0x64, 0x20, 0x76, 0x61, 0x6c, 0x75, 0x65, 0x20, 0x72, 0x65, 0x64
	DB	0x20, 0x66, 0x72, 0x6f, 0x6d, 0x20, 0x65, 0x65, 0x70, 0x72, 0x6f, 0x6d
	DB	0x00


; Statistics:
; code size:	15472 (0x3c70) bytes (11.80%)
;           	 7736 (0x1e38) words
; udata size:	  691 (0x02b3) bytes (19.63%)
; access size:	   33 (0x0021) bytes


	end
