#ifndef ADC_H

#define ADC_H

#include <stdint.h>

#ifdef __cplusplus

extern "C" {

#endif

// Init function for the ADC peripheral

void Adc_Init(void);

// Get a 10-bit digitized reading from an assigned ADC channel (channels 0 through 7)

uint16_t Adc_ReadChannel(uint8_t channel);

#ifdef __cplusplus

}

#endif

#endif