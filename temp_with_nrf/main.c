/*
 * main.c
 *
 *  Created on: Jun 20, 2014
 *      Author: esp
 */



#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

#include "ADC/adc.h"
#include "UART/uart.h"
#include "SPI/spi.h"
#include "nRF/nrf.h"
#include "BKTERM/bkterm.h"

uint16_t getTemp(uint16_t adc_val);
uint8_t *retPtr(uint8_t *testData);

int main(void){



		char c;
			UART_Init();
			adc_init(ADC8);
			SPI_Init();
			nrf_init();


			// term_resetAll();
			term_cls();
			term_cursorXY(1,2);
			term_setBGColor(RED);
			term_setFGColor(WHITE);
			UART_sendStringFF(PSTR("   MEGA328P Terminal  "));
			term_setAttr(0,BLACK,WHITE);
			term_cursorXY(1,3);
			UART_sendStringFF(PSTR("1. Pokaż wartość ADC i temperaturę"));
			term_newLine();
			UART_sendStringFF(PSTR("2. Pokąz STATUS nRF"));
			term_newLine();
			UART_sendStringFF(PSTR("3. Pointer test"));
			term_newLine();
			term_setAttr(2,BLACK,WHITE);
			UART_sendStringFF(PSTR("Wprowadz nr: "));

			while(1){
				c = UART_readChar();
				if (c=='1'){

					UART_sendStringFF(PSTR("ADC: "));
					//UART_sendInt(adc_value(),10);
					term_newLine();
					UART_sendStringFF(PSTR("Temp: "));
					UART_sendInt(getTemp(adc_value()),10);
					term_newLine();
				} else if (c=='2') {
					UART_sendStringFF(PSTR("STATUS: "));
					term_newLine();
					UART_sendStringFF(PSTR("CONFIG: "));
					UART_sendInt(nrf_getReg(CONFIG),2);
					term_newLine();
					UART_sendStringFF(PSTR("Ustawiam CONFIG..."));
					term_newLine();
					uint8_t confArray = (1<<PWR_UP) | (1<<MASK_MAX_RT) | (1<<MASK_RX_DR) | (1<<MASK_TX_DS) | (1<<CRCO);
					nrf_write(CONFIG,&confArray, sizeof(confArray));
					term_newLine();
					UART_sendStringFF(PSTR("Nowy CONFIG: "));
					UART_sendInt(nrf_getReg(CONFIG),2);
					term_newLine();

				} else if (c == '3'){
					uint8_t data[] = "Bart";
					uint8_t *printData;
					printData = retPtr(data);
					for (uint8_t i=0;i<=2;i++){
						UART_sendInt(*(printData+i),16);
						term_newLine();
					}

				}


			}
		
	
	}

uint16_t getTemp(uint16_t adc_val){

	uint16_t temp = (adc_val*11)/10/13; //div by circa 13 for internal temp sens calib of degC
	return (uint16_t)temp;
}


uint8_t *retPtr(uint8_t *testData){
	return testData;
}
