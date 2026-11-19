#include <avr/io.h>

#include <util/delay.h>

#include "adc.h"

#include "lcd.h"

#include "io.h"

// Temperature thresholds (default values) that match the specification picture

int16_t low_limit  = 20;  // LL: 20

int16_t high_limit = 38;  // LH: 38

int16_t current_temp = 0;

void setup() {

    // Initializing the hardware modules

    Adc_Init();

    Io_Init();

    Lcd_Init();

}

void loop() {

    // Step 1. Getting temperature from the LM35 sensor at pin A1

    current_temp = (int16_t)Io_ReadTempCelsius();

    // Step 2. Getting buttons state on the shield pin A0 and modifying the threshold limits

    // Boundary condition: LL can't be more than LH-1 and all limits must be in the range 0..99 degrees Celsius

    Button_t btn = Io_ReadButton();

    if (btn != BTN_NONE) {

        if (btn == BTN_UP && high_limit < 99) {

            high_limit++;

        }

        else if (btn == BTN_DOWN && high_limit > (low_limit + 1)) {

            high_limit--;

        }

        else if (btn == BTN_RIGHT && low_limit < (high_limit - 1)) {

            low_limit++;

        }

        else if (btn == BTN_LEFT && low_limit > 0) {

            low_limit--;

        }

        _delay_ms(180); // Debounce delay for human interaction with the button

    }

    // Step 3. Checking the current temperature against the valid range

    uint8_t is_nok = (current_temp < low_limit || current_temp > high_limit);

    // Activating/deactivating audible/visual alarms (leds at pins 12/13 and buzzer at pin 3)

    Io_SetAlert(is_nok);

    // Step 4. Rendering 16x2 LCD according to the specification:

    // Line 0: "Temp: <value>    OK " / "Temp: <value>   NOK "

    // Line 1: "LL:<value>     LH:<value>"

    Lcd_SetCursor(0, 0);

    Lcd_SendString("Temp: ");

    Lcd_PrintNumber(current_temp);

    Lcd_SendString("   "); // Cleaning up the residual digits

    Lcd_SetCursor(0, 11);

    if (is_nok) {

        Lcd_SendString("NOK");

    } else {

        Lcd_SendString(" OK");

    }

    Lcd_SetCursor(1, 0);

    Lcd_SendString("LL:");

    Lcd_PrintNumber(low_limit);

    Lcd_SendString("     ");

    Lcd_SetCursor(1, 10);

    Lcd_SendString("LH:");

    Lcd_PrintNumber(high_limit);

    Lcd_SendString("  ");

    _delay_ms(100); // Refreshing rate

}