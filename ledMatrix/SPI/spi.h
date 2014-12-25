/*
 * spi.h
 *
 *  Created on: 09-03-2013
 *      Author: bk
 */

#ifndef SPI_H_
#define SPI_H_

#define MOSI PB3
#define SCK PB5
#define SS PB2

void SPI_Init(void);
void SPI_send8(uint8_t bajt);
void SPI_send16(uint16_t bajt);

void SPI_sendBuf(uint8_t *buf, uint8_t size);
uint8_t SPI_receive(void);

void pauseDisp(uint16_t ms);

#endif /* SPI_H_ */
