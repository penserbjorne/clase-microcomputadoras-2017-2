/*
Keyboard_2.c
Por: Carlos Daniel Hernandez Mena
23-Julio_2011

FUNCIONA MEJOR QUE Keyboard_1.c


Conectando un teclado standard para PC, este
programa imprime en los LEDs del puerto B
el codigo de la tecla pulsada que NO esta en ASCII

PORTD(0) = Reloj
PORTD(1) = Data


*/

//Encabezados
#include <16f877.h>

#fuses HS,NOPROTECT,
#use delay(clock=20000000)
#use rs232(baud=9600, xmit=PIN_C6, rcv=PIN_C7)
#org 0x1F00, 0x1FFF void loader16F877(void) {} //for the 8k 16F876/7

//Definiciones

#byte    PORTB =  0x06
#byte    PORTD =  0x08
#byte    TRISB =  0x86
#byte    TRISD =  0x88


int cont=0;
int DATA11[11], DATA;


void main()
{
   int i=8;


   //Bits 0 y 1 del puerto D como entrada
   bit_set(TRISD,0);
   bit_set(TRISD,1);
   //Puerto B como salida
   set_tris_b(0x00);

   //Condicion Inicial
   //Todos los puertos en 0
   output_b(0x00);

   //Redardo para que todo se estabilice
   delay_ms(1000);

   while(1)
   {

      while(bit_test(PORTD,0)==1);
      DATA11[cont++] = bit_test(PORTD,1);
      while(bit_test(PORTD,0)==0);


      if(cont==11)
      {
         int i;
         cont=0;

         for(i=1;i<=8;i++)
         {
            if(DATA11[i]==1)
               bit_set(DATA,8-i);
            else
               bit_clear(DATA,8-i);
         }

         if(DATA!=0x0F)      //Si es diferente del Codigo de Liberacion de Tecla
            output_b(DATA);  //que SI muestre.

      }


   }//while

}//main


