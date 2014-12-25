/*
 * uart.h
 *
 *  Created on: 08-03-2013
 *      Author: bk
 */

#ifndef UART_H_
#define UART_H_

#define BAUD 9600
#define __UBRR (F_CPU/16/BAUD-1) // U2X=0



void UART_Init(void);

void UART_send(char i);
char UART_readChar(void);
void UART_sendString(char *string);
void UART_sendInt(uint8_t liczba);

void UART_calibrate(void);

#endif /* UART_H_ */
