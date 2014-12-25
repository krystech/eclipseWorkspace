/*
 * uart.h
 *
 *  Created on: 08-03-2013
 *      Author: bk
 */

//Select MCU
//#define MEGA8A
//#define MEGA168A
#define MEGA328P

#ifndef UART_H_
#define UART_H_


#define BAUD 9600
#define __UBRR (F_CPU/16/BAUD-1) // U2X=0



void UART_Init(void);

void UART_send(char i);
char UART_readChar(void);
void UART_sendString(char *string);
void UART_sendStringFF(const char *string);
void UART_sendInt(uint16_t liczba, uint8_t format);

void UART_calibrate(void);

#endif /* UART_H_ */
