//LCD header file 

#ifndef LCD_H
#define LCD_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {

#endif

void Lcd_Init(void);

void Lcd_SendCommand(uint8_t cmd);
void Lcd_SendChar(char data);
void Lcd_SendString(const char *str);

void Lcd_SetCursor(uint8_t row, uint8_t col);
void Lcd_PrintNumber(int16_t num);

void Lcd_Clear(void);

#ifdef __cplusplus

}

#endif

#endif


