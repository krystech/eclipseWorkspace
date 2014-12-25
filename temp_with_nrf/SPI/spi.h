/*
 * spi.h
 *
 *  Created on: 09-03-2013
 *      Author: bk
 */

#ifndef SPI_H_
#define SPI_H_

// For AMEGA328, AMEGA8A
#define MOSI PB3 	// sunduino.pin = 11
#define MISO PB4	// sunduino.pin = 12
#define SCK PB5		// sunduino.pin = 13
#define SS PB2	// sunduino.pin = 10


void SPI_Init(void);
void SPI_send8(uint8_t bajt);
uint8_t SPI_comm(uint8_t bajt);
//void SPI_send16(uint16_t bajt);

void SPI_sendBuf(uint8_t *buf, uint8_t size);
uint8_t SPI_receive(void);


#endif /* SPI_H_ */
