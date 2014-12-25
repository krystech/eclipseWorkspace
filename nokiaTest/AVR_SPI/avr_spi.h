/*
 * avr_spi.h
 *
 *  Created on: 24 gru 2014
 *      Author: k
 */

#ifndef AVR_SPI_AVR_SPI_H_
#define AVR_SPI_AVR_SPI_H_

#define SPI_DDR DDRB
#define SPI_PORT PORTB

void SPI_Init(void);
void SPI_sendByte(uint8_t byte);

#endif /* AVR_SPI_AVR_SPI_H_ */
