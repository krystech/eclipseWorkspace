/*
 * adc.c
 *
 *  Created on: 15 wrz 2013
 *      Author: k
 */

#include <avr/io.h>

#include "adc.h"

void adc_init(void) {
#ifdef MEGA168

	ADMUX |= (1<<REFS0) | (1<<REFS1) | (1<<ADLAR); // Internal Vref = 1.1v
	//ADMUX |= (1<<REFS0); // Vref = Vcc
	ADMUX |= (1<<MUX0) | (1<<MUX1); //input channel = ADC3/PC3

	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //enable ADC; prescaler = Fcpu / 128
	ADCSRA |= (1<<ADATE); //free-running mode
#endif

#ifdef MEGA8A
	ADMUX |= (1<<REFS0) | (1<<REFS1) | (1<<ADLAR); // Internal Vref = 2.56v
	ADMUX |= (1<<MUX0) | (1<<MUX1); //input channel = ADC3/PC3
	ADCSRA |= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0); //enable ADC; prescaler = Fcpu / 128
	ADCSRA |= (1<<ADFR); //free-running mode

#endif
}

void adc_start(void) {

	ADCSRA |= (1<<ADSC); // Strat ADC conversion
}

uint8_t adc_value(void) {
	uint8_t conv_done;

	conv_done = (ADCSRA & (1<<ADSC)) >> 6; //read ADSC; if 1 conv in preogress; if 0 conv done
	while(!conv_done);
	return ADCH;

}
