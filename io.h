#ifndef IO_H

#define IO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {

#endif

   // Buttons definitions for LCD Keypad Shield connected to A0 pin

typedef enum {
    BTN_NONE = 0,
    BTN_RIGHT,
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_SELECT
} Button_t;

void Io_Init(void);

void Io_SetAlert(uint8_t active);
Button_t Io_ReadButton(void);
uint16_t Io_ReadTempCelsius(void);

#ifdef __cplusplus
}

#endif

#endif