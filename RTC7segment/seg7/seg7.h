/*
 * seg7.h
 *
 *  Created on: 29-03-2013
 *      Author: k
 */

#ifndef SEG7_H_
#define SEG7_H_

// Podlaczenie wyswietlacza

// Cyfry (od prawej)
#define DIG1_ON PORTA |= (1<<PA0)
#define DIG1_OFF PORTA &= ~(1<<PA0)
#define DIG2_ON PORTD |= (1<<PD4)
#define DIG2_OFF PORTD &= ~(1<<PD4)
#define DIG3_ON PORTD |= (1<<PD5)
#define DIG3_OFF PORTD &= ~(1<<PD5)
#define DIG4_ON PORTD |= (1<<PD6)
#define DIG4_OFF PORTD &= ~(1<<PD6)
#define NUM_1 \
	SEG_B; \
	SEG_C;


// Segmenty
#define SEG_A PORTB &= ~(1<<PB0)
#define SEG_B PORTB &= ~(1<<PB1)
#define SEG_C PORTB &= ~(1<<PB2)
#define SEG_D PORTB &= ~(1<<PB3)
#define SEG_E PORTB &= ~(1<<PB4)
#define SEG_F PORTB &= ~(1<<PB6)
#define SEG_G PORTD &= ~(1<<PD0)
#define SEG_DP PORTD &= ~(1<<PD1)

void dispInit(void);
void lightsOut(void);
void test(uint8_t cyfra);


#endif /* SEG7_H_ */
