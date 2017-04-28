    processor 16f877a
    include <p16f877a.inc>

str     EQU 0x20; direccion del registro a mostrar
off     EQU 0x21;
toogle  EQU 0x22;
conta   EQU 0x23;
temp    EQU 0x7b;
index0  EQU 0x7c; contador
index1  EQU 0x7d;
index2  EQU 0x7e;
index3  EQU 0x7f;

    org 0
    goto setup
    org 5

setup:
    bcf     STATUS, RP1
    bsf     STATUS, RP0
    movlw   0x07
    movwf   ADCON1
    movwf   CMCON
    movlw   0xff
    movwf   TRISA       ; porta input
    clrf    TRISB       ; portb output
    movlw   0x3f
    movwf   TRISD       ; pd7, pd6 out
    bcf     STATUS, RP0
    clrf    PORTA
    clrf    PORTB
    clrf    PORTD
    bsf     PORTD, 6    ; encendemos primero display derecho
    movlw   0x0F
    movwf   index1      ; retraso inicial
    movlw   0xfe        ; cadena inicial
    movwf   str
    clrf    off         ; off = false
    clrf    toogle      ; toogle = false
    movlw   0x0f        ; si es cero, reinicia el retraso largo
    movwf   conta       ; reinicia el retraso

inicio:
    movlw   0x09
    movwf   index0
dig1:                   ; mostramos nuestra clave
    movlw   0x0e        ; primer digito de nuestro codigo
    call    put         ; imprime el valor
    call    delay       ; retraso
    movlw   0x0f        ; digito mas significativo de nuestro codigo
    call    put         ; mostramos en display
    call    delay       ; retraso
    decfsz  index0, F   ; decrementamos el contador
    goto    dig1        ; si no es cero y volvemos al principio
                        ; si es cero continuamos
    movlw   0x09
    movwf   index0
dig2:
    movlw   0x0e        ; primer digito de nuestro codigo
    call    put         ; imprime el valor
    bsf     PORTB,  0   ;
    call    delay       ; retraso
    movlw   0x0f        ; digito mas significativo de nuestro codigo
    call    put         ; mostramos en display
    bsf     PORTB,  0   ;
    call    delay       ; retraso
    decfsz  index0, F   ; decrementamos el contador
    goto    dig2        ; si no es cero y volvemos al principio
                        ; si es cero continuamos
    decfsz  index1, F   ; decrementamos el contador
    goto    inicio      ; si no es cero y volvemos al principio
                        ; si no continuamos


main:
    movf    PORTA,  W   ; recuperamos el valor de porta
    movwf   temp        ; y lo guadamos temporalmente
    ; switch
    sublw   0x2e        ;
    btfsc   STATUS, Z   ; case boton1
    goto    boton1      ;
    movf    temp,   W   ;
    sublw   0x2d        ;
    btfsc   STATUS, Z   ; case boton2
    goto    boton2      ;
    movf    temp,   W   ;
    sublw   0x2b        ;
    btfsc   STATUS, Z   ; case boton3
    goto    boton3      ;
    movf    temp,   W   ;
    sublw   0x27        ;
    btfsc   STATUS, Z   ; case boton4
    goto    boton4      ;
    movf    temp,   W   ;
    sublw   0x0f        ;
    btfsc   STATUS, Z   ; case boton5
    goto    boton5      ;
    movf    temp,   W   ;
disp:
    movlw   0x12        ; 31 veces
    movwf   index0      ; retraso
loop:
    movf    str,    W   ; cargamos el valor a mostrar
    call    put         ; mostramos en los displays
    call    delay       ; y esperamos antes de mostrar el siguiente caracter
    swapf   str,    W   ; intercambiamos nibles y guardamos en w
    call    put         ; mostramos en los diplays
    call    delay       ; y esperamos
    decfsz  index0      ; retraso de 300ms aprox antes de leer el siguiente clic
    goto    loop        ; si index0 no es 0 muestra el valor

    movlw   0xff        ;
    btfsc   toogle, 0   ; si no estamos parpadeando
    decfsz  conta       ; si parpadeamos decrementa contador y si no es cero
    goto    main        ; regresamos a checar el boton
    movlw   0x0f        ; si es cero, reinicia el retraso largo
    movwf   conta       ; reinicia el retraso
    xorwf   off,    F   ; toogle off
    goto    main        ; regresamos a checar el boton

boton1:
    swapf   str,    F   ; intecambiamos los nibbles del codigo a mostrar
    goto    disp        ; regresamos a mostrar el valor
boton2:
    movlw   0x00        ; ceros
    movwf   str         ; ponemos a ceros el valor a mostrar
    goto    disp        ; regresamos a mostrar el valor
boton3:
    movlw   0xff        ; mascara
    xorwf   toogle, F   ; parpadear o no parpadear
    btfss   toogle, 7   ; si no parpadeamos
    clrf    off         ; encendemos los leds
    goto    disp        ; regresamos a mostrar el valor
boton4:
    incf    str,    F   ; incrementamos el valor a mostrar
    goto    disp        ; regresamos a mostrar el valor
boton5:
    decf    str,    F   ; decrementamos el valor a mostrar
    goto    disp        ; regresamos a mostrar el valor


; muestra en los display el contenido de w
; debe ser llamado primero con el valor mas
put:
    andlw   0x0f        ; nos deshacemos del nibble mas significativo
    movwf   temp        ; guarda el valor de w temporalmente
    movlw   0xc0        ; 0b11000000
    xorwf   PORTD,  F   ; cambiamos de display
    movf    temp,   W   ; recuperamos el valor de w
    call    getseg      ; obtenemos el codigo 7seg
    btfsc   off,    0   ; checamos si mostramos o apagamos los displays
    clrw                ; limpiamos w si hay que apagar
    movwf   PORTB       ; lo mostramos en portb
    return


getseg:
    addwf	PCL,    F   ; w debe tener el valor a mostrar
            ; abcdefgh
    retlw	b'11111100'	; el codigo 7 segmentos para el 0
    retlw	b'01100000'	; el codigo 7 segmentos para el 1
    retlw	b'11011010'	; el codigo 7 segmentos para el 2
    retlw	b'11110010'	; el codigo 7 segmentos para el 3
    retlw	b'01100110'	; el codigo 7 segmentos para el 4
    retlw	b'10110110'	; el codigo 7 segmentos para el 5
    retlw	b'10111110'	; el codigo 7 segmentos para el 6
    retlw	b'11100000'	; el codigo 7 segmentos para el 7
    retlw	b'11111110'	; el codigo 7 segmentos para el 8
    retlw	b'11100110'	; el codigo 7 segmentos para el 9
    retlw	b'11101110'	; el codigo 7 segmentos para el a
    retlw	b'00111110'	; el codigo 7 segmentos para el b
    retlw	b'10011100'	; el codigo 7 segmentos para el c
    retlw	b'01111010'	; el codigo 7 segmentos para el d
    retlw	b'10011110'	; el codigo 7 segmentos para el e
    retlw	b'10001110'	; el codigo 7 segmentos para el f

; delay = 0.05 seconds
; clock frequency = 20 mhz
; actual delay = 0.005 seconds = 25000 cycles
; error = 0 %
delay:
			;24993 cycles
	movlw	0x86
	movwf	index2
	movlw	0x14
	movwf	index3
loopd:
	decfsz	index2, f
	goto	$+2
	decfsz	index3, f
	goto	loopd
			;3 cycles
	goto	$+1
	nop
			;4 cycles (including call)
	return

    end
