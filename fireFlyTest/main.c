/*
 * main.c
 *
 *  Created on: 23-02-2013
 *      Author: bk
 */


#include <avr/io.h>
#include <util/delay.h>
//KATODY
#define CH1_ON (1<<PB0)
#define CH2_ON (1<<PB1)
#define CH1_OFF ~(1<<PB0)
#define CH2_OFF ~(1<<PB1)
//ANODY
#define FF1_OFF (1<<PB2)
#define FF2_OFF (1<<PB3)
#define FF3_OFF (1<<PB4)
#define FF4_OFF (1<<PB5)
#define FF1_ON ~(1<<PB2)
#define FF2_ON ~(1<<PB3)
#define FF3_ON ~(1<<PB4)
#define FF4_ON ~(1<<PB5)

int main(void)
{

	DDRB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4)  | (1<<PB5);
	//PORTB = (1<<PB1) | (1<<PB0); // Katody
	//PORTB = ~((1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5)); // Anody

	for(;;)
	{
		PORTB |= CH1_ON;
		//PORTB |= FF2_OFF | FF3_OFF | FF4_OFF;
		PORTB &= FF1_ON | FF2_ON | FF3_ON | FF4_ON;

	}

}
