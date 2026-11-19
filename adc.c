//The following code block shows the initialization of the ADC channels, along with reading from the particular channel on the microcontroller of type AVR. AVcc (5V) has been taken as the reference voltage, and a prescaler has been defined in order to obtain a 125kHz ADC clock. The functional behavior of the original code has been retained.

#include <avr/io.h>

#include "adc.h"

void Adc_Init(void) {

	// Set the AVcc (5V) as reference voltage (by setting the REFS0 bit)

	ADMUX = (1 << REFS0);

	

	// Enable the ADC and set the prescaler as 128 (16 MHz / 128 = 125 kHz ADC clock)

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);

}

uint16_t Adc_ReadChannel(uint8_t channel) {

	// Maintain the reference voltage settings and select the channel (0-7)

	ADMUX = (ADMUX & 0xF0) | (channel & 0x07);

	

	// Start a conversion

	ADCSRA |= (1 << ADSC);

	

	// Wait for the conversion to finish

	while (ADCSRA & (1 << ADSC));

	

	// Read the result

	return ADC;

}