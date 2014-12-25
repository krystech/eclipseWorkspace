/*
 * nokiaLCD.c
 *
 *  Created on: 01-06-2013
 *      Author: k
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
//#include <avr/interrupt.h>

#include "nokiaLCD.h"
#include "AVR_SPI/avr_spi.h"
#include "asciiTable.h"

void nokia_Init(void){
	//Init dla wyswietlacz
	NOKIA_RST_DDR = (1<<NOKIA_RST_PIN);
	NOKIA_LINES_DDR |= (1<<NOKIA_DC_PIN);

		NOKIA_RESET; 	//2. reset low
		NOKIA_SET; 		//3. reset hi

		NOKIA_COMMAND;

		SPI_sendByte(0x21); //2. powerDown =0; Horizontal adress; Extended func set

		SPI_sendByte(0x9C); //3. Vop

		SPI_sendByte(0x04); //4. Set temper. coef

		SPI_sendByte(0x14); //5. Set bias system; 1:48

		SPI_sendByte(0x22); //5. Normal func set

		SPI_sendByte(0x0C); //6. Display Control; Normal Mode

		SPI_sendByte(0b10000000); //X 0<x<83

		SPI_sendByte(0b01000000); //Y 0<y<5

}


void LcdString(char *characters)
{
  while (*characters)
  {
    LcdCharacter(*characters++);
  }
}

void LcdCharacter(char character)
{

  for (int index = 0; index < 5; index++)
  {
    nokia_sendByte(pgm_read_byte(&ASCII[character - 0x20][index]),'d');
  }

}

void nokia_sendByte(uint8_t byte, char c_d){
	(c_d == 'c') ? NOKIA_COMMAND : NOKIA_DATA;
	NOKIA_ENABLE; 	//1. SCE going low
	SPI_sendByte(byte);
	NOKIA_DISABLE; //8. Pull enable hi
}

void nokia_ClrScr(void) {
	for (int i=0; i<502; i++){
		nokia_sendByte(0x00,'d');
	}
}

void LcdStringFont10x13(char *characters, uint8_t x, uint8_t y){


}

void nokia_CalibrateVop(void) {
	//for itoa
	char buf[3];
	nokia_sendByte(0b00100001,'c'); //2. powerDown =0; Horizontal adress; Extended func set
	for(uint8_t a=0; a<128; a++){

		nokia_sendByte(0b00100001,'c'); //2. powerDown =0; Horizontal adress; Extended func set
		nokia_sendByte(128 | a,'c');
		nokia_sendByte(0b00100000,'c'); //2. powerDown =0; Horizontal adress; Extended func set
		nokia_sendByte(0b10000000,'c'); //X 0<x<83
		nokia_sendByte(0b01000000,'c'); //Y 0<y<5

		LcdString(itoa(a,buf,10));

		_delay_ms(500);
		nokia_ClrScr();
	}

}
