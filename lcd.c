#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "lcd.h"

// LCD Shield Pin Configuration:
// RS -> Pin 8 (PB0), EN --> Pin 9 (PB1), Backlight --> Pin 10 (PB2)
// Data D4,D7 -> Pins 4,7 (PD4,PD7)

static void Lcd_PulseEnable(void) {

    PORTB |= (1 << PB1);       // EN hifh
    _delay_us(1);
    PORTB &= ~(1 << PB1);  // EN low
    _delay_us(100);

}

static void Lcd_Write4Bits(uint8_t nibble) {
    // reset PD4,PD7 and assign upper 4bits of nibble to PD4,PD7

    PORTD = (PORTD & 0x0F) | (nibble & 0xF0);

    Lcd_PulseEnable();

}

void Lcd_SendCommand(uint8_t cmd) {

    PORTB &= ~(1 << PB0);       // RS = 0 for command
    Lcd_Write4Bits(cmd);

    Lcd_Write4Bits(cmd << 4);

    if (cmd < 4) {
        _delay_ms(2);            // Commands like Clear/Home take more time

    }

}

void Lcd_SendChar(char data) {

    PORTB |= (1 << PB0);     // RS =1 for data
    Lcd_Write4Bits(data);

    Lcd_Write4Bits(data << 4);

}

void Lcd_SendString(const char *str) {

    while (*str) {

        Lcd_SendChar(*str++);

    }

}

void Lcd_SetCursor(uint8_t row, uint8_t col) {

    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    Lcd_SendCommand(address);

}

void Lcd_PrintNumber(int16_t num) {

    char buf[8];

    itoa(num, buf, 10);
    Lcd_SendString(buf);
}

void Lcd_Clear(void) {

    Lcd_SendCommand(0x01);

}

void Lcd_Init(void) {

       // PB0 (RS), PB1 (EN), PB2 (Backlight) are output pins

    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2);

    // Backlight turned on (Pin 10 high)

    PORTB |= (1 << PB2);
// LCD data lines PD4 - PD7 are output pins
    DDRD |= 0xF0;

    _delay_ms(50);  // delay of 50 milliseconds for LCD power up

    // 4 bit initializ sequence

    PORTB &= ~(1 << PB0);

    Lcd_Write4Bits(0x30);
    _delay_ms(5);
    Lcd_Write4Bits(0x30);

    _delay_us(150);

    Lcd_Write4Bits(0x30);
    Lcd_Write4Bits(0x20);  // enter 4 bit mode
    Lcd_SendCommand(0x28); // 4 bit mode, 2 lines, 5 x 8 font
    Lcd_SendCommand(0x0C); // ON display
    Lcd_SendCommand(0x06); // increment cursor

    Lcd_Clear();
}