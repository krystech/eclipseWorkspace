/*
 * adc.h
 *
 *  Created on: 15 wrz 2013
 *      Author: k
 */

#ifndef ADC_H_
#define ADC_H_

//Select MCU
//#define MEGA8A
//#define MEGA168A
#define MEGA328P

//Select input channel
#define ADC0 0x00
#define ADC1 0x01
#define ADC2 0x02
#define ADC3 0x03
#define ADC4 0x04
#define ADC5 0x05
#define ADC6 0x06
#define ADC7 0x07
#define ADC8 0x08


//Functions
void adc_init(uint8_t channel);
void adc_start(void);
uint16_t adc_value(void);

#endif /* ADC_H_ */
