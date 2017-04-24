;	Autor:	penserbjorne
;	Date:	23/04/2017

processor	16f877A
include		<p16f877A.inc>

nL		equ	H'30'
nH		equ	H'31'
reg1 	equ H'32'
codigo	equ H'69'

    org 0
    goto inicio
    org 5

inicio:
    ;	Configurando el ADC
    bsf     STATUS, RP0	;	Cambio al banco 1
    clrf    TRISB       ;	PORTB como salida
    clrf    ADCON0		;	Limpiamos el ADCON0
	bcf		TRISD, 0	; 	
	bcf		TRISD, 1	;	Para encender un display a la vez.
    bcf     STATUS, RP0	;	Cambio al banco 0
    movlw   H'01'		;	
    movwf   ADCON0		;	Configuracion del ADCON0
	;	Configuracion propia
	movlw	H'FF'		;	W = FF
	movwf	reg1		;	reg1 = W
	movlw	H'FF'		;	Codigo asignado por el profesor, le agregamos un valor mas, era FE
	movwf	codigo		;	codigo = W	
    
	;	Conversion
convertir:
	bsf		ADCON0, 2	;	Pon 1 en el bit 2 de ADCON2 [Ordena conversion]
    ;	Preguntamos si terminó
preguntar:
    btfsc	ADCON0, 2	;	Revisa el bit 2, si es 0 salta
    goto    preguntar	;	Volvemos a preguntar
    movf    ADRESH, W	;	W = ADRESH
conversion:
	;	Verificamos el tope superior
	;	W es igual a ADRESH
	subwf	codigo,W	;	W = W - codigo
 	btfsc 	STATUS,C	;	Verificamos la bandera de carry
    goto	convertir	;	ADRESH >= codigo

	;	Si no hemos llegado al tope superior
	;	Convertimos
	movf	ADRESL,W	;	Carga lo que tenga resL en W
	andlw 	H'0F'		;	Enmascaramiento para el nemles low
	movwf 	nL			;	nL = W [nmles low]
	movf	ADRESH,W		;	Carga lo que tenga resH en W
	andlw	H'F0'		;	Enmascaramiento para el nemles high
	movwf	nH			;	nH = W [nmles high]
	swapf	nH,f		;	Cambio de nibles 30H-->03H y dejalo en f [registro]
	
	;	1er Display
	movf	nL,W		;	W = nL
	call	Tabla		;	Convertimos
	movwf	PORTB		;	PORTB = W
	bcf		PORTD,1		;	Apaga el segundo display
	bsf		PORTD,0		;	Enciende el primer display
	call 	retardo		;	Retardo
	
	;	2do Display
	movf	nH,w		;	W = nH
	call	Tabla		;	Convertimos
	movwf	PORTB		;	PORTB = W
	bcf		PORTD,0		;	Apaga el primer display		
	bsf		PORTD,1		;	Enciende el segundo display
	call 	retardo		;	Retardo
	goto 	convertir

	;	Tabla de conversion a Display 7 seg
Tabla:
	addwf	PCL,f
	retlw	H'A0'
	retlw	H'A1'
	retlw	H'A2'
	retlw	H'A3'
	retlw	H'A4'
	retlw	H'A5'
	retlw	H'A6'
	retlw	H'A7'
	retlw	H'A8'
	retlw	H'A9'
	retlw	H'AA'
	retlw	H'AB'
	retlw	H'AC'
	retlw	H'AD'
	retlw	H'AE'
	retlw	H'AF'

	;	Retardo
retardo:
	movwf	reg1
	nop
loop1:
	decfsz	reg1,1
	goto    loop1
	nop
	return

    end