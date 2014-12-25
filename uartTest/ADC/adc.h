/*
 * adc.h
 *
 *  Created on: 15 wrz 2013
 *      Author: k
 */

#ifndef ADC_H_
#define ADC_H_

//Select MCU
#define MEGA8A
//#define MEGA168A


//Functions
void adc_init(void);
void adc_start(void);
uint8_t adc_value(void);

#endif /* ADC_H_ */
