;	Autor:	penserbjorne
;	Date:	23/04/2017
;	Update:	24/04/2017

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
	clrf    TRISD       ;	PORTD como salida
    clrf    ADCON0		;	Limpiamos el ADCON0
	bcf		TRISD, 6	; 	
	bcf		TRISD, 7	;	Para encender un display a la vez.
    bcf     STATUS, RP0	;	Cambio al banco 0
    movlw   H'01'		;	
    movwf   ADCON0		;	Configuracion del ADCON0
	;	Configuracion propia
	movlw	H'FF'		;	W = FF
	movwf	reg1		;	reg1 = W
	movlw	H'FE'		;	Codigo asignado por el profesor
	movwf	codigo		;	codigo = W	
    
	;	Conversion
convertir:
	bsf		ADCON0, 2	;	Pon 1 en el bit 2 de ADCON2 [Ordena conversion]
    ;	Preguntamos si terminó
preguntar:
    btfsc	ADCON0, 2	;	Revisa el bit 2, si es 0 salta
    goto    preguntar	;	Volvemos a preguntar
    movf    ADRESH, W	;	W = ADRESH
validacion:
	;	Verificamos el tope superior
	;	W es igual a ADRESH
	subwf	codigo,W	;	W = codigo - W
 	btfss 	STATUS,C	;	Verificamos la bandera de carry
    goto	tope		;	W >= codigo
conversion:
	;	Si no hemos llegado al tope superior
	;	Convertimos
	movf	ADRESL,W	;	Carga lo que tenga ADRESL en W
	andlw 	H'0F'		;	Enmascaramiento para el nemles low
	movwf 	nL			;	nL = W [nmles low]
	movf	ADRESH,W	;	Carga lo que tenga ADRESH en W
	andlw	H'F0'		;	Enmascaramiento para el nemles high
	movwf	nH			;	nH = W [nmles high]
	swapf	nH,f		;	Cambio de nibles 30H-->03H y dejalo en f [registro]

	;	1er Display
	movf	nL,W		;	W = nL
	call	Tabla		;	Convertimos
	movwf	PORTB		;	PORTB = W
	bcf		PORTD,6		;	Apaga el segundo display
	bsf		PORTD,7		;	Enciende el primer display
	call 	retardo		;	Retardo
	
	;	2do Display
	movf	nH,w		;	W = nH
	call	Tabla		;	Convertimos
	movwf	PORTB		;	PORTB = W
	bcf		PORTD,7		;	Apaga el primer display		
	bsf		PORTD,6		;	Enciende el segundo display
	call 	retardo		;	Retardo
	goto 	convertir

tope:
	movfw	codigo		;	W = codigo
	movwf	ADRESH		;	ADRESH = W = codigo
	goto	conversion	;
	;	Tabla de conversion a Display 7 seg
Tabla:
	addwf	PCL,f
	retlw	b'11111100'	;	0
	retlw	b'01100000'	;	1
	retlw	b'11011010'	;	2
	retlw	b'11110010'	;	3
	retlw	b'01100110'	;	4
	retlw	b'10110110'	;	5
	retlw	b'10111110'	;	6
	retlw	b'11100000'	;	7
	retlw	b'11111110'	;	8
	retlw	b'11100110'	;	9
	retlw	b'11101110'	;	A
	retlw	b'00111110'	;	B
	retlw	b'10011100'	;	C
	retlw	b'01111010'	;	D
	retlw	b'10011110'	;	E
	retlw	b'10001110'	;	F

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