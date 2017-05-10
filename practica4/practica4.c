/*
 *  Copyright (c) 2017 Emilio Cabrera <emilio1625@gmail.com>
 *
 *  MIT License
 *
 *  Permission is hereby granted, free of charge, to any person obtaining
 *  a copy of this software and associated documentation files (the
 *  "Software"), to deal in the Software without restriction, including
 *  without limitation the rights to use, copy, modify, merge, publish,
 *  distribute, sublicense, and/or sell copies of the Software, and to
 *  permit persons to whom the Software is furnished to do so, subject to
 *  the following conditions:
 *
 *  The above copyright notice and this permission notice shall be
 *  included in all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 *  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 *  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 *  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 *  Controla un teclado tipo PS/2, y muestra el codigo de tecla en el puerto B
 *  Compilador Mikro C
 *
 *  PORTD(0) <- clock
 *  PORTD(1) <- DATA
 *  PORTB(0:7) -> codigo de tecla
 */

unsigned int cont = 0;
unsigned int DATA = 0;

void main() org 5 {
    TRISD = 3; // Bits 0 y 1 del puerto D como entrada
    TRISB = 0; // Puerto B como salida

    while(1) {
        if (cont == 0) { // si estamos en espera de un caracter
            while ((PORTD & 3) == 3); // si PORTD(1:0) = 11, estamos en reposo
            while ((PORTD & 2) == 2); // bit de inicio
        }

        while (PORTD.B0 == 1); // esperamos al flanco de bajada para leer
            if (cont <= 8) { // solo guardamos el primer byte
                DATA = (DATA << 1) | PORTD.B1; // el ultimo bit de DATA = PORTD(1)
            } // e ignoramos paridad y bit de fin
            cont++; // aumentamos el numero de bits que llevamos
        while(PORTD.B0 == 0);

        if (cont == 11) {
            PORTB = DATA; // muestra DATA en el puerto B
            if (PORTB == 0x0F) {// Si es el codigo de liberacion de tecla
                PORTB = 0; // no muestres nada
            }
            cont = 0;
        }


    }//while

}//main