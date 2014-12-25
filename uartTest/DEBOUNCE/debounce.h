/*
 * debounce.h
 *
 *  Created on: 8 wrz 2013
 *      Author: k
 */

#ifndef DEBOUNCE_H_
#define DEBOUNCE_H_

//Select MCU
#define MEGA8A
//#define MEGA168A

#define BUTTONS_MASK (1<<PC3) //piny switchow
#define BUTTONS_PIN PINC
#define BUTTONS_PORT PORTC

extern volatile uint8_t overFlow;

//void timer8_start(void);
//void timer8_stop(void);


uint8_t btnPressed(void);

#endif /* DEBOUNCE_H_ */
