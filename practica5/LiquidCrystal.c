#include "LiquidCrystal.h"

uint8_t i;
uint8_t _displayfunction;
uint8_t _displaycontrol;
uint8_t _displaymode;
uint8_t _initialized;
uint8_t _numlines;
uint8_t _row_offsets[4];

sbit _rs_pin at PORTB.B2;
sbit _rw_pin at PORTB.B1;
sbit _enable_pin at PORTB.B0;


void begin(uint8_t fourbitmode, uint8_t cols, uint8_t lines, uint8_t dotsize) {
    if (fourbitmode) {
        _displayfunction = LCD_4BITMODE | LCD_1LINE | LCD_5x8DOTS;
    } else {
        _displayfunction = LCD_8BITMODE | LCD_1LINE | LCD_5x8DOTS;
    }

    if (lines > 1) {
        _displayfunction |= LCD_2LINE;
    }
    
    _numlines = lines;

    setRowOffsets(0x00, 0x40, 0x00 + cols, 0x40 + cols);

    // for some 1 line displays you can select a 10 pixel high font
    if ((dotsize != LCD_5x8DOTS) && (lines == 1)) {
        _displayfunction |= LCD_5x10DOTS;
    }

    TRISB &= 0xf8; // PORTE salida
    if (fourbitmode) {
        TRISB = TRISB & 0x0f; // PORTB(7:4) salida
    } else {
        TRISB = 0x0; // PORTB salida
    }
    PORTB &= 0x0f;

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way before 4.5V so we'll wait 50
    Delay_ms(50);
    // Now we pull both RS and R/W low to begin commands
    _rw_pin = 0;
    _rs_pin = 0;

    //put the LCD into 4 bit or 8 bit mode
    if (! (_displayfunction & LCD_8BITMODE)) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        write4bits(0x03);
        Delay_ms(5); // wait min 4.1ms

        // second try
        write4bits(0x03);
        Delay_ms(5); // wait min 4.1ms

        // third go!
        write4bits(0x03);
        Delay_us(150);

        // finally, set to 4-bit interface
        write4bits(0x02);
    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        command(LCD_FUNCTIONSET | _displayfunction);
        Delay_ms(5);  // wait more than 4.1ms

        // second try
        command(LCD_FUNCTIONSET | _displayfunction);
        Delay_us(150);

        // third go
        command(LCD_FUNCTIONSET | _displayfunction);
    }

    // finally, set lines, font size, etc.
    command(LCD_FUNCTIONSET | _displayfunction);

    // turn the display on with no cursor or blinking default
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    display();

    // clear it off
    clear();

    // Initialize to default text direction (for romance languages)
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
    // set the entry mode
    command(LCD_ENTRYMODESET | _displaymode);
}

void setRowOffsets(uint8_t row0, uint8_t row1, uint8_t row2, uint8_t row3) {
    _row_offsets[0] = row0;
    _row_offsets[1] = row1;
    _row_offsets[2] = row2;
    _row_offsets[3] = row3;
}

/********** high level commands, for the user! */
void clear() {
    command(LCD_CLEARDISPLAY);    // clear display, set cursor position to zero
    Delay_ms(2);    // this command takes a long time!
}

void home() {
    command(LCD_RETURNHOME);    // set cursor position to zero
    Delay_ms(2);    // this command takes a long time!
}

void setCursor(uint8_t col, uint8_t row) {
    const uint8_t max_lines = 4;
    if ( row >= max_lines ) {
        row = max_lines - 1;        // we count rows starting w/0
    }
    if ( row >= _numlines ) {
        row = _numlines - 1;        // we count rows starting w/0
    }
    command(LCD_SETDDRAMADDR | (col + _row_offsets[row]));
}

// Turn the display on/off (quickly)
void noDisplay() {
    _displaycontrol &= ~LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void display() {
    _displaycontrol |= LCD_DISPLAYON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turns the underline cursor on/off
void noCursor() {
    _displaycontrol &= ~LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void cursor() {
    _displaycontrol |= LCD_CURSORON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// Turn on and off the blinking cursor
void noBlink() {
    _displaycontrol &= ~LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

void blink() {
    _displaycontrol |= LCD_BLINKON;
    command(LCD_DISPLAYCONTROL | _displaycontrol);
}

// These commands scroll the display without changing the RAM
void scrollDisplayLeft(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void scrollDisplayRight(void) {
    command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

// This is for text that flows Left to Right
void leftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This is for text that flows Right to Left
void rightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'right justify' text from the cursor
void autoscroll(void) {
    _displaymode |= LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// This will 'left justify' text from the cursor
void noAutoscroll(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINCREMENT;
    command(LCD_ENTRYMODESET | _displaymode);
}

// Allows us to fill the first 8 CGRAM locations
// with custom characters
void createChar(uint8_t location, uint8_t charmap[]) {
    location &= 0x7; // we only have 8 locations 0-7
    command(LCD_SETCGRAMADDR | (location << 3));
    for (i = 0; i < 8; i++) {
        write(charmap[i]);
    }
}

/*********** mid level commands, for sending data/cmds */

void command(uint8_t value) {
    _rs_pin = 0;
    // if there is a RW set it low to Write
    _rw_pin = 0;
    if (_displayfunction & LCD_8BITMODE) {
        write8bits(value);
    } else {
        write4bits(value >> 4);
        write4bits(value);
    }
}

void write(uint8_t value) {
    _rs_pin = 1;
    // if there is a RW set it low to Write
    _rw_pin = 0;
    if (_displayfunction & LCD_8BITMODE) {
        write8bits(value);
    } else {
        write4bits(value >> 4);
        write4bits(value);
    }
}

/************ low level data pushing commands **********/


void write4bits(uint8_t value) {
    value = value << 4;
    PORTB &= 0x0f;
    PORTB |= value;
    _enable_pin = 0;
    Delay_us(1);
    _enable_pin = 1;
    Delay_us(1);        // enable pulse must be >450ns
    _enable_pin = 0;
    Delay_us(100);     // commands need > 37us to settle
}

void write8bits(uint8_t value) {
    PORTB = value;
    _enable_pin = 0;
    Delay_us(1);
    _enable_pin = 1;
    Delay_us(1);        // enable pulse must be >450ns
    _enable_pin = 0;
    Delay_us(100);     // commands need > 37us to settle
}

void print(char str[]) {
     for (i = 0; str[i] != '\0'; i++) {
         write((uint8_t)str[i]);
     }
}