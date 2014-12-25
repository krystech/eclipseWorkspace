/*
 * spi.c
 *
 *  Created on: 09-03-2013
 *      Author: bk
 */

#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"


void SPI_Init(void)
{
	DDRB |= (1<<MOSI) | (1<<SCK); // MOIS -> out, SCK -> out, rest -> in
	PORTB |= (1<<SS);
	// uruchom SPI, tryb MASTER, SCKspeed = Fosc / 16
	SPCR |= (1<<SPE) | (1<<MSTR) | (1<<SPR0);


}

void SPI_send8(uint8_t bajt)
{

	SPDR = bajt;
	//czekaj na ostatni bit
	while( !(SPSR & (1<<SPIF)));

}

uint8_t SPI_comm(uint8_t bajt){

	SPDR = bajt;
	//czekaj na ostatni bit
	while( !(SPSR & (1<<SPIF)));
	return SPDR;
}

//void SPI_send16(uint16_t bajt)
//{
//
//
//	SPDR = bajt & 0x00FF;
//	//czekaj na ostatni bit
//	while( !(SPSR & (1<<SPIF)));
//	PORTB |= (1<<SS);
//	//Odłączam wyjscia na 595
//	PORTB &= ~(1<<SS);
//
//	SPDR = (bajt & 0xFF00)>>8;
//	//czekaj na ostatni bit
//	while( !(SPSR & (1<<SPIF)));
//	PORTB |= (1<<SS);
//	//Odłączam wyjscia na 595
//	PORTB &= ~(1<<SS);
//
//}

void SPI_sendBuf(uint8_t *buf, uint8_t size)
{
	uint8_t i=0;
	while(i<=size){
		SPI_send8(buf[i++]);

}

}

uint8_t SPI_receive(void)
{
	while( !(SPSR & (1<<SPIF)));
	return SPDR;
}

