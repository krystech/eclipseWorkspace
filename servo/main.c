/*
 * main.c
 *
 *  Created on: 11 mar 2014
 *      Author: k
 */

#define u8 uint8_t
#define u16 uint16_t

#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>

// servo is on PD3 - OC2B




int main(void)
{
	

	// fast PWM, TOP at ICR1, prescaler = 8
	TCCR1A |= (1<<WGM11);
	TCCR1B |= (1<<WGM13) | (1<<WGM12) | (1<<CS11);

	//basefreq at 50Hz (20ms)
	ICR1 = 19999;

	//set port dir; output
	DDRD |= (1<<PD5) | (1<<PD5);
	DDRB |= (1<<PB2);
	// timer B as fast PWM clear pin on compare match
	TCCR1A |= (1<<COM1B1);

	// set the PWM duty cycle (1-2ms)
	//1499 - 0
	//2999 - 1/2
	//4499 - 1

	OCR1B = 1499; //get the dail to the middle


	for(;;)
	{

	}
}
