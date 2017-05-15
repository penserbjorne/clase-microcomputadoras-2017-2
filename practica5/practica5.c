#include "LiquidCrystal.h"

char cha = 0;

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
    setCursor(6, 1);
    print("FE 3");
    home();
    cursor();
    blink();
    while (1) {
        cha = getc();
         
        if (cha) {
            write(cha);
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
    
    if (c == 0x0F) {// Si es el codigo de liberacion de tecla
        c = 0;
    }
        
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
        default:
            c = 0;
            break;
    }
    return c;
}