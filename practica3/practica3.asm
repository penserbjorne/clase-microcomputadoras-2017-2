    ; Copyright (c) 2017 Emilio Cabrera
    ;
    ; MIT License
    ;
    ; Permission is hereby granted, free of charge, to any person obtaining
    ; a copy of this software and associated documentation files (the
    ; "Software"), to deal in the Software without restriction, including
    ; without limitation the rights to use, copy, modify, merge, publish,
    ; distribute, sublicense, and/or sell copies of the Software, and to
    ; permit persons to whom the Software is furnished to do so, subject to
    ; the following conditions:
    ;
    ; The above copyright notice and this permission notice shall be
    ; included in all copies or substantial portions of the Software.
    ;
    ; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    ; EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    ; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
    ; NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
    ; LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
    ; OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    ; WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
    ;

    ; p3.asm
    ; compilado con MPLAB8

    processor 16f877a
    include <p16f877a.inc>

str     EQU 0x20; direccion del registro a mostrar
off     EQU 0x21; indica si debemos apagar los leds
toogle  EQU 0x22; indica si debemos parpadear los leds
temp    EQU 0x7b; temporal, usada en varias partes del codigo
index0  EQU 0x7c; contadores
index1  EQU 0x7d;
index2  EQU 0x7e;
index3  EQU 0x7f;

    org 0
    goto setup
    org 5

setup:
    bcf     STATUS, RP1
    bsf     STATUS, RP0 ; cambiamos al banco 1
    movlw   0x07
    movwf   ADCON1      ; desactivamos el ADC
    movwf   CMCON       ; desactivamos comparadores
    movlw   0xff        ;
    movwf   TRISA       ; PORTA como entrada
    clrf    TRISB       ; PORTB como salida
    movlw   0x3f        ; 0b11000000
    movwf   TRISD       ; bits 6 y siete de PORTD como salida
    bcf     STATUS, RP0 ; regresamos al banco 0
    clrf    PORTA       ;
    clrf    PORTB       ;
    clrf    PORTD       ; limpiamos latches de los puertos
    bsf     PORTD,  6   ; encendemos primero display derecho
    movlw   0x0f
    movwf   index1      ; retraso asinado por el profe (3 segundos)
    movlw   0xfe        ; cadena inicial
    movwf   str         ; la guardamos en el registro
    clrf    off         ; off = false
    clrf    toogle      ; toogle=false

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
    bsf     PORTB,  0   ; encendemos el punto decimal
    call    delay       ; retraso
    movlw   0x0f        ; digito mas significativo de nuestro codigo
    call    put         ; mostramos en display
    bsf     PORTB,  0   ; encendemos el punto decimal
    call    delay       ; retraso
    decfsz  index0, F   ; decrementamos el contador
    goto    dig2        ; si no es cero, volvemos al principio
                        ; si es cero continuamos
    decfsz  index1, F   ; decrementamos el contador
    goto    inicio      ; si no es cero y volvemos al principio
                        ; si no continuamos

    movlw   0x0f        ;
    movwf   index1      ; restauramos el valor del retraso

main:
    movf    PORTA,  W   ; recuperamos el valor de PORTA
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
    movlw   0x12        ; 18 veces
    movwf   index0      ; retraso
loop:
    movf    str,    W   ; cargamos el valor a mostrar
    call    put         ; mostramos en los displays
    call    delay       ; y esperamos antes de mostrar el siguiente caracter
    swapf   str,    W   ; intercambiamos nibles y guardamos en w
    call    put         ; mostramos en los diplays
    call    delay       ; y esperamos
    decfsz  index0      ; retraso antes de leer el siguiente clic
    goto    loop        ; si index0 no es 0 muestra el valor


    btfsc   toogle, 0   ; si no estamos parpadeando ignora la siguiente instrucion
    decfsz  index1      ; si parpadeamos decrementa contador y si no es cero..
    goto    main        ; regresamos a checar el boton
    movlw   0x0f        ; .. si es cero, reinicia el retraso largo
    movwf   index1      ; reinicia el retraso
    xorwf   off,    F   ; switchea la bandera para apagar los displays
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

; obtiene el codigo de 7 segmentos del valor en el acumulador
getseg:
    addwf  PCL,    F   ; w debe tener el valor a mostrar
          ;  abcdefgh
    retlw  b'11111100'  ; el codigo 7 segmentos para el 0
    retlw  b'01100000'  ; el codigo 7 segmentos para el 1
    retlw  b'11011010'  ; el codigo 7 segmentos para el 2
    retlw  b'11110010'  ; el codigo 7 segmentos para el 3
    retlw  b'01100110'  ; el codigo 7 segmentos para el 4
    retlw  b'10110110'  ; el codigo 7 segmentos para el 5
    retlw  b'10111110'  ; el codigo 7 segmentos para el 6
    retlw  b'11100000'  ; el codigo 7 segmentos para el 7
    retlw  b'11111110'  ; el codigo 7 segmentos para el 8
    retlw  b'11100110'  ; el codigo 7 segmentos para el 9
    retlw  b'11101110'  ; el codigo 7 segmentos para el a
    retlw  b'00111110'  ; el codigo 7 segmentos para el b
    retlw  b'10011100'  ; el codigo 7 segmentos para el c
    retlw  b'01111010'  ; el codigo 7 segmentos para el d
    retlw  b'10011110'  ; el codigo 7 segmentos para el e
    retlw  b'10001110'  ; el codigo 7 segmentos para el f

; rutina de retardo
; delay = 0.05 seconds
; clock frequency = 20 MHz
; actual delay = 0.005 seconds = 25000 cycles
; error = 0 %
delay:
        ;24993 cycles
    movlw  0x86
    movwf  index2
    movlw  0x14
    movwf  index3
loopd:
    decfsz  index2, F
    goto  $+2
    decfsz  index3, F
    goto  loopd
        ;3 cycles
    goto  $+1
    nop
        ;4 cycles (including call)
    return

    end
