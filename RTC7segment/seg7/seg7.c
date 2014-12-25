/*
 * seg7.c
 *
 *  Created on: 29-03-2013
 *      Author: k
 */


#include <avr/io.h>
#include <util/delay.h>
#include "seg7.h"

void dispInit(void)
{
	//kierunki
	DDRA |= (1<<PA0);
	DDRB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB6);
	DDRD |= (1<<PD0) | (1<<PD1) | (1<<PD4) | (1<<PD5) | (1<<PD6);
	//stany
	PORTA &= ~(1<<PA0);
	PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB6);
	PORTD |= (1<<PD0) | (1<<PD1);
	PORTD &= ~(1<<PD4) & ~(1<<PD5) & ~(1<<PD6);
}

void test(uint8_t cyfra)
{


	//zapal segmenty
switch(cyfra)
{
case 0:
	SEG_A;
	SEG_B;
	SEG_C;
	SEG_D;
	SEG_E;
	SEG_F;
break;
case 1:
	NUM_1;
break;
case 2:
	SEG_A;
	SEG_B;
	SEG_G;
	SEG_E;
	SEG_D;
break;
case 3:
	SEG_A;
	SEG_B;
	SEG_C;
	SEG_D;
	SEG_G;
break;
case 4:
	SEG_F;
	SEG_G;
	SEG_B;
	SEG_C;
break;
case 5:
	SEG_A;
	SEG_F;
	SEG_G;
	SEG_C;
	SEG_D;
break;
case 6:
	SEG_A;
	SEG_F;
	SEG_G;
	SEG_C;
	SEG_D;
	SEG_E;
break;
case 7:
	SEG_A;
	SEG_B;
	SEG_C;
break;
case 8:
	SEG_A;
	SEG_B;
	SEG_C;
	SEG_D;
	SEG_E;
	SEG_F;
	SEG_G;
break;
case 9:
	SEG_A;
	SEG_B;
	SEG_C;
	SEG_D;
	SEG_F;
	SEG_G;
break;

}

}

void lightsOut(void)
{
	PORTB |= (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB6);
	PORTD |= (1<<PD0) | (1<<PD1);
}

