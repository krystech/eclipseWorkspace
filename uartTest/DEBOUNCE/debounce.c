/*
 * debounce.c
 *
 *  Created on: 8 wrz 2013
 *      Author: k
 */

#include <avr/io.h>
//#include <util/delay.h>
//#include <stdlib.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "debounce.h"

//Dla timera
volatile uint8_t overFlow = 0;

void timer8_start(void){
#ifdef MEGA168
	TCCR0B |= (1<<CS00) | (1<<CS02); 	// Fcpu / 1024
	TIMSK0 |= (1<<TOIE0); 				// TimerOverflow
#endif

#ifdef MEGA8A
	TCCR0 |= (1<<CS00) | (1<<CS02); 	// Fcpu / 1024
	TIMSK |= (1<<TOIE0); 				// TimerOverflow
#endif
}

void timer8_stop(void){
#ifdef MEGA168
	TIMSK0 &= ~(1<<TOIE0); 				// TimerOverflow
#endif
	TIMSK &= ~(1<<TOIE0); 				// TimerOverflow
#ifdef MEGA8A

#endif
}

uint8_t btnPressed(void) {
	uint8_t debounced = 0;
	
	if(~BUTTONS_PIN & BUTTONS_MASK){
		timer8_start(); // jesli wcisniety guzik, odpal timer
		if(overFlow){ 	// jesli overFLow = 1, sprawdzamy stabilonosc przycisku
			if(~BUTTONS_PIN & BUTTONS_MASK){
				debounced = 1;
			}
		} else {
			debounced = 0;
		}

	}
	overFlow = 0;
	timer8_stop();
	return debounced;
}

ISR(TIMER0_OVF_vect) {
	static uint8_t ticks = 0;

	ticks++;
	if (ticks >= 20){
		overFlow = 1;
		ticks = 0;
	}
}
