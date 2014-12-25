/*
 * nokiaLCD.h
 *
 *  Created on: 01-06-2013
 *      Author: k
 */

#ifndef NOKIALCD_H_
#define NOKIALCD_H_

#define NOKIA_RST_DDR DDRD
#define NOKIA_RST_PORT PORTD
#define NOKIA_LINES_DDR DDRB
#define NOKIA_LINES_PORT PORTB

#define NOKIA_SCE_PIN PB0 	// arduino D8
#define NOKIA_DC_PIN PB2 	// arduino D10
#define NOKIA_SDIN_PIN PB3	// arduino D11
#define NOKIA_CLK_PIN PB5	// arduino D13
#define NOKIA_RST_PIN PD7	// arduino D7

#define NOKIA_ENABLE SPI_PORT &= ~(1<<NOKIA_SCE_PIN)
#define NOKIA_DISABLE SPI_PORT |= (1<<NOKIA_SCE_PIN)
#define NOKIA_DATA SPI_PORT |= (1<<NOKIA_DC_PIN)
#define NOKIA_COMMAND SPI_PORT &= ~(1<<NOKIA_DC_PIN)
#define NOKIA_SET NOKIA_RST_PORT |= (1<<NOKIA_RST_PIN)
#define NOKIA_RESET NOKIA_RST_PORT &= ~(1<<NOKIA_RST_PIN)

void nokia_Init(void);

void nokia_sendByte(uint8_t byte, char c_d);
void LcdString(char *characters);
void LcdStringFont10x13(char *characters,uint8_t x, uint8_t y);
void LcdCharacter(char character);
void nokia_ClrScr(void);

void nokia_CalibrateVop(void);

#endif /* NOKIALCD_H_ */
