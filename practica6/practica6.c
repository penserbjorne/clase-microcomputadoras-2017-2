#include <string.h>
#include "LiquidCrystal.h"

#define MAX_SIZE_COMMAND 15

#define CMD1 "LENGBIELORRUSIA\0"
#define CMD2 "RUTA5\0"
#define CMD3 "LENGESP2\0"
#define CMD4 "NOMBRESHO\0"
#define CMD5 "NOMBRESMO\0"
#define CMD6 "POSGRADOARTES\0"
#define CMD7 "LENGCONGO\0"
#define CMD8 "ORGANOS\0"
#define CMD9 "HELP\0"

char cha = 0;
char cmd[MAX_SIZE_COMMAND + 1] = "";
int index = 0;

char getc();
char conv(uint8_t code);

void main() org 5 {
    // teclado
    TRISD = 3; // Bits 0 y 1 del puerto D como entrada
    // serial setup
    BRGH_bit = 0; // low speed
    SPBRG = 31; // 96,000
    SYNC_bit = 0;
    TXEN_bit = 1;
    CREN_bit = 1;
    SPEN_bit = 1;
    //lcd
    begin(1, 16, 2, LCD_5x8DOTS);
    // Print a message to the LCD.
    home();
    clear();
    print("   LIDSOL-term  ");    //    Mensaje de bienvenida
    Delay_ms(1000);           //     Esperamos un segundo
    clear();
    home();
    cursor();
    blink();
    while (1) {
        cha = getc();

        if (cha) {
           if(cha != '\n'){
                 write(cha);
                 if(index < MAX_SIZE_COMMAND){
                    cmd[index] = cha;
                    index++;
                 }
           }else{
                  // Limpiamos para mostrar resultados
                 clear();
                 home();
                 // completamos comando
                 cmd[index] = '\0';
                 index++;
                 
                 // analizamos comando
                 if(strcmp(cmd, CMD1) == 0){
                     clear();home();print("1.- Ruso");
                     setCursor(0,1);print("2.- Bielorruso");Delay_ms(1000);
                     clear();home();print("3.- Ucraniano");Delay_ms(1000);
                     setCursor(0,0);print("4.- Polaco");Delay_ms(1000);
                     clear();home();print("5.- Yiddish");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD2) == 0){
                     clear();home();print("1.- Odontologia");
                     setCursor(0,1);print("2.- Economia");Delay_ms(1000);
                     clear();home();print("3.- FilosLetras");
                     setCursor(0,1);print("4.- Psicologia");Delay_ms(1000);
                     clear();home();print("5.- Medicina");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD3) == 0){
                     clear();home();print("5.- Asturiano");
                     setCursor(0,1);
                     print("      y leonés");
                     setCursor(0,0);
                     Delay_ms(1000);
                     clear();home();print("6.- Aragonés");
                     setCursor(0,1);print("7.- Aranés");Delay_ms(1000);
                     clear();home();print("8.- Extremeño");
                     setCursor(0,1);print("9.- Fala");Delay_ms(1000);
                     clear();home();print("10.- Silbo");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD4) == 0){
                     clear();home();print("1.- Omar");
                     setCursor(0,1);print("2.- Olaf");Delay_ms(1000);
                     clear();home();print("3.- Oscar");
                     setCursor(0,1);print("4.- Oberon");Delay_ms(1000);
                     clear();home();print("5.- Octavio");
                     setCursor(0,1);print("6.- Og");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD5) == 0){
                     clear();home();print("1.- Ofelia");
                     setCursor(0,1);print("2.- Odette");Delay_ms(1000);
                     clear();home();print("3.- Olga");
                     setCursor(0,1);print("4.- Olivia");Delay_ms(1000);
                     clear();home();print("5.- Olimpia");
                     setCursor(0,1);print("6.- Olaya");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD6) == 0){
                     clear();home();print("1.- Maestria en ");
                     setCursor(0,1);print("Artes Visuales");Delay_ms(1000);
                     clear();home();print("2.- Maestria en ");
                     setCursor(0,1);print("Docencia en Artes");Delay_ms(1000);
                     clear();home();print("y Diseño");Delay_ms(1000);
                     clear();home();print("3.- Maestria en ");
                     setCursor(0,1);print("Diseño y Comuni-");Delay_ms(1000);
                     clear();home();print("cacion Visual");Delay_ms(1000);
                     clear();home();print("4.- Maestria en ");
                     setCursor(0,1);print("Cine Documental");Delay_ms(1000);
                     clear();home();print("5.- Doctorado en ");
                     setCursor(0,1);print("Artes y Diseño");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD7) == 0){
                     clear();home();print("1.- Frances");
                     setCursor(0,1);print("2.- Kikongo");Delay_ms(1000);
                     clear();home();print("3.- Lingala");
                     setCursor(0,1);print("4.- Suajili");Delay_ms(1000);
                     clear();home();print("5.- Tshiluba");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD8) == 0){
                     clear();home();print("1.- Cerebro");
                     setCursor(0,1);print("2.- Corazon");Delay_ms(1000);
                     clear();home();print("3.- Pulmones");
                     setCursor(0,1);print("4.- Riñones");Delay_ms(1000);
                 }else if(strcmp(cmd, CMD9) == 0){
                     clear();home();print("LENGBIELORRUSIA");
                     setCursor(0,1);print("RUTA5");Delay_ms(1000);
                     clear();home();print("LENGESP2");
                     setCursor(0,1);print("NOMBRESHO");Delay_ms(1000);
                     clear();home();print("NOMBRESMO");
                     setCursor(0,1);print("POSGRADOARTES");Delay_ms(1000);
                     clear();home();print("LENGCONGO");
                     setCursor(0,1);print("ORGANOS");Delay_ms(1000);
                     clear();home();print("HELP");Delay_ms(1000);
                 }else{
                       clear();home();print("Error 777 :c");Delay_ms(1000);
                 }

                 // Volemos a limpiar
                 clear();
                 home();
                 index = 0;
                 cmd[0] = '\0';
           }
        }
    }
}

char getc() {
    uint8_t DATA = 0;
    uint8_t cont = 0;
    uint8_t c = 0;
    while ((PORTD & 3) == 3); // si PORTD(1:0) = 11, estamos en reposo
    while ((PORTD & 2) == 2); // bit de inicio

    while (cont < 31) {
        while (PORTD.B0 == 1); // esperamos al flanco de bajada para leer
            if (cont <= 30) { // solo guardamos el primer byte
                DATA = (DATA << 1) | PORTD.B1; // el ultimo bit de DATA = PORTD(1)
            } // e ignoramos paridad y bit de fin
            cont++; // aumentamos el numero de bits que llevamos
        while(PORTD.B0 == 0);
    }
    while ((PORTD & 3) == 3);

    c = DATA;
    c = conv(c);
    TXREG = c;
    while (!TRMT_bit) {}

    return c;
}

char conv(uint8_t chcode) {
    char c;
    switch (chcode) {
        case 0x38:
            c = 'A';
            break;
        case 0x4C:
            c = 'B';
            break;
        case 0x84:
            c = 'C';
            break;
        case 0xC4:
            c = 'D';
            break;
        case 0x24:
            c = 'E';
            break;
        case 0xD4:
            c = 'F';
            break;
        case 0x2C:
            c = 'G';
            break;
        case 0xCC:
            c = 'H';
            break;
        case 0xC2:
            c = 'I';
            break;
        case 0xDC:
            c = 'J';
            break;
        case 0x42:
            c = 'K';
            break;
        case 0xD2:
            c = 'L';
            break;
        case 0x5C:
            c = 'M';
            break;
        case 0x8C:
            c = 'N';
            break;
        case 0x22:
            c = 'O';
            break;
        case 0xB2:
            c = 'P';
            break;
        case 0xA8:
            c = 'Q';
            break;
        case 0xB4:
            c = 'R';
            break;
        case 0xD8:
            c = 'S';
            break;
        case 0x34:
            c = 'T';
            break;
        case 0X3C:
            c = 'U';
            break;
        case 0x54:
            c = 'V';
            break;
        case 0xB8:
            c = 'W';
            break;
        case 0x44:
            c = 'X';
            break;
        case 0xAC:
            c = 'Y';
            break;
        case 0x58:
            c = 'Z';
            break;
        case 0x0E:
            c = '0';
            break;
        case 0x96:
            c = '1';
            break;
        case 0x4E:
            c = '2';
            break;
        case 0x5E:
            c = '3';
            break;
        case 0xD6:
            c = '4';
            break;
        case 0xCE:
            c = '5';
            break;
        case 0x2E:
            c = '6';
            break;
        case 0x36:
            c = '7';
            break;
        case 0xAE:
            c = '8';
            break;
        case 0xBE:
            c = '9';
            break;
        case 0x8E:
            c = '.';
            break;
        case 0x9E:
            c = '+';
            break;
        case 0xDE:
            c = '-';
            break;
        case 0x3E:
            c = '*';
            break;
        case 0x68:
            c = '1';
            break;
        case 0x78:
            c = '2';
            break;
        case 0x64:
            c = '3';
            break;
        case 0xA4:
            c = '4';
            break;
        case 0x74:
            c = '5';
            break;
        case 0x6C:
            c = '6';
            break;
        case 0xBC:
            c = '7';
            break;
        case 0x8C:
            c = '8';
            break;
        case 0x62:
            c = '9';
            break;
        case 0xA2:
            c = '0';
            break;
        case 0x72:
            c = '?';
            break;
        case 0xAA:
            c = '¿';
            break;
        case 0x94:
            c = ' ';
            break;
        case 0x5A:  //    Enter
            c = '\n';
            break;
        case 0x66:  //    Backspace
            c = '\b';
            break;
        default:
            c = 0;
            break;
    }
    return c;
}