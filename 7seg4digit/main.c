/*
 * main.c
 *
 *  Created on: 25-12-2012
 *      Author: bk
 *
 *
 *
 *       _A_
 *    F |   |B
 *      |_G_|
 *    E |   |C
 *      |___|
 *        D
 *   SEGMENTY: Piny na plytce
 *    A: 11
 *    B: 7
 *    C: 4
 *    D: 2
 *    E: 1
 *    F: 10
 *    G: 5
 *    kropka: 3
 *   CYFRY: od lewej
 *   D1: 12
 *   D2: 9
 *   D3: 8
 *   D4: 6
 *
 *   128 64 32 16 8 4 2 1
 *    8  7  6  5  4 3 2 1
 *    Kr G  F  E  D C B A
 */

#include <avr/io.h>
#include <util/delay.h>
//#define F_CPU 1000000

int main(void) {
	DDRB = 0xFF; //segmenty + kropka
	//DDRD = 0x03; // PD0, PD1
	DDRD = 0x0F; // PD0, PD1, PD2, PD3

	uint8_t cyfry[10] = {0x3F, 0x06, 0x5B, 0x4F, 0x66 ,0x6D, 0x7D, 0x07, 0x7F, 0x6F};
	PORTB = cyfry[0];

	while(1){
		PORTD =(1<<PD0);
		_delay_ms(500);
		PORTD =(1<<PD1);
		_delay_ms(500);
	}
}




