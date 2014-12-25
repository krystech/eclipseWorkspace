/*
 * avr_spi.c
 *
 *  Created on: 24 gru 2014
 *      Author: k
 */


#define u8 uint8_t
#define u16 uint16_t

#include <avr/io.h>
//#include <util/delay.h>
//#include <stdlib.h>
//#include <avr/pgmspace.h>
//#include <avr/interrupt.h>

#include "avr_spi.h"

void SPI_Init(uint8_t MOSI, uint8_t CLK, uint8_t SCE) {
	SPI_DDR = (1<<MOSI) | (1<<CLK) | (1<<SCE);
	SPCR = (1<<SPE) | (1<<MSTR);  //Enable, Master, Fosc/16
}


void SPI_sendByte(uint8_t byte) {
	//Zaladuj do rejestru SPI
	SPDR = byte;
	//Poczekaj na zakonczenie wysylania; SPIF ustawione na koniec
	while(!(SPSR & (1<<SPIF)));
}

