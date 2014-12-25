/*
 * nrf.c
 *
 *  Created on: Jun 22, 2014
 *      Author: esp
 */

#include <avr/io.h>
#include <util/delay.h>

#include "nrf.h"
#include "../SPI/spi.h"

void nrf_init(void){

	DDRD |= (1<<CE) | (1<<CSN);
	PORTD |= (1<<CSN);
	PORTD &= ~(1<<CE);

}


uint8_t nrf_getReg(uint8_t reg){
	//_delay_us(10);
	PORTD &= ~(1<<CSN); // start com
	_delay_us(10);
	SPI_send8(R_REGISTER + reg);
	_delay_us(10);
	reg = SPI_comm(NOP);
	//_delay_us(10);
	PORTD |= (1<<CSN); // end com
	return reg;

}

void nrf_write(uint8_t reg, uint8_t *data, uint8_t size){
	PORTD &= ~(1<<CSN); // start com
	_delay_us(10);
	SPI_send8(W_REGISTER + reg);
	_delay_us(10);
	for(uint8_t i=0; i<size; i++){
		SPI_send8(*(data+i));
		_delay_us(10);
	}
	PORTD |= (1<<CSN); // end com
}

