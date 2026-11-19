#include <avr/io.h>
#include "io.h"
#include "adc.h"

// pins mapping:
// Buzzer : Pin 3 (PD3)
// LEd anode (+) : Pin 12 (PB4)
// LEd ground (-):Pin 13 (PB5)
// buttons A0 "ADC0"
// temp sensor: A1 "ADC1"

void Io_Init(void) {
    // Output PD3 pin for buzzer and initializing it to low state
    DDRD |= (1 << PD3);
    PORTD &= ~(1 << PD3);

    // output pins for LED (PB4 and PB5) initialization
    DDRB |= (1 << PB4) | (1 << PB5);
    PORTB &= ~(1 << PB4); // LED off state
    PORTB &= ~(1 << PB5); // PB5 is kept LOW inorder to use it as led ground

}

void Io_SetAlert(uint8_t active) {
    if (active) {
        PORTD |= (1 << PD3); // Buzzer ON (PD3 HIGH)
        PORTB |= (1 << PB4); // LED on (PB4 HIGH, PB5 is used as ground)
    } else {
        PORTD &= ~(1 << PD3); // Buzzer off
        PORTB &= ~(1 << PB4); // LED OFF
    }
}

Button_t Io_ReadButton(void) {
    uint16_t adc_val = Adc_ReadChannel(0); // Buttons array is mapped on A0

    // The following thresholds corresponds to the standard DFRobot LCD Keypad Shield
    if (adc_val > 900) return BTN_NONE;
    if (adc_val < 60) return BTN_RIGHT;
    if (adc_val < 200) return BTN_UP;
    if (adc_val < 400) return BTN_DOWN;
    if (adc_val < 600) return BTN_LEFT;
    if (adc_val < 850) return BTN_SELECT;

    return BTN_NONE;
}

uint16_t Io_ReadTempCelsius(void) {
    // Averaging ADC values over four reading from A1 for LCD stability
    uint32_t sum = 0;
    for (uint8_t i = 0; i < 4; i++) {
        sum += Adc_ReadChannel(1);
    }
    uint16_t avg_adc = sum / 4;

    // From LM35 sensor, 10mV/degree Celsius are generated
    // T[C°] = (ADC_value * 500)/1024
    return (uint16_t)(((uint32_t)avg_adc * 500UL) / 1024UL);
}