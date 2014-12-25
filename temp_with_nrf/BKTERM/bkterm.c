/*
 * bkterm.c
 *
 *  Created on: Jun 14, 2014
 *      Author: esp
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "bkterm.h"
#include "../UART/uart.h"

const char RST_ATTR[] PROGMEM = {"\x1b""[0m"};
const char NEW_LINE[] PROGMEM = {"\r\n"};
const char CUR_HOME[] PROGMEM = {"\x1b""[H"};
const char CLR_SCRN[] PROGMEM = {"\x1b""[2J"};
const char RST_ALL[] PROGMEM = {"\x1b""c"};
const char CUR_HIDE[] PROGMEM = {"\x1b""[25l"};
const char CUR_SHOW[] PROGMEM = {"\x1b""[25h"};
const char CUR_BACK[] PROGMEM = {"\x1b""[D"};
const char CUR_DOWN[] PROGMEM = {"\x1b""[B"};

void term_newLine(void){
	UART_sendStringFF(NEW_LINE);
}

void term_cls(void){
	UART_sendStringFF(RST_ATTR); 	//reset all atributes
	UART_sendStringFF(CUR_HOME); 	//Home cursor
	UART_sendStringFF(CLR_SCRN);	//clear screen
}

void term_resetAll(void){
	UART_sendStringFF(RST_ALL);
}

void term_cursorToggle(uint8_t state){
	if (state==1){
		UART_sendStringFF(CUR_SHOW);
	} else {
		UART_sendStringFF(CUR_HIDE);
	}
}

void term_setBGColor(uint8_t color){
	UART_sendString("\x1b""[4");
	UART_sendInt(color,10);
	UART_send('m');
}

void term_setFGColor(uint8_t color){
	UART_sendString("\x1b""[3");
	UART_sendInt(color,10);
	UART_send('m');
}

void term_setAttr(uint8_t attr, uint8_t bgColor, uint8_t fgColor){
	UART_sendString("\x1b""[");
	UART_sendInt(attr,10);
	UART_send(';');
	UART_sendInt(fgColor,10);
	UART_send(';');
	UART_sendInt(bgColor,10);
	UART_send('m');
}

void term_cursorXY(uint8_t x, uint8_t y){
	UART_send(0x1b);
	UART_send('[');
	UART_sendInt(y,10);
	UART_send(';');
	UART_sendInt(x,10);
	UART_send('H');
}
void term_drawHLine(char sign, uint8_t length){
	for (uint8_t i=1; i<= length; i++){
		UART_send(sign);
	}
}

void term_drawVLine(char sign, uint8_t length){
	for (uint8_t i=1; i<= length; i++){
		UART_send(sign);
		UART_sendStringFF(CUR_BACK);	//cursor backwards (by 1 line)
		UART_sendStringFF(CUR_DOWN);	//cursor down (by 1 line)
	}
}
