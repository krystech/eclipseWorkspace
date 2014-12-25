/*
 * uart.c
 *
 *  Created on: 08-03-2013
 *      Author: bk
 */

#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "uart.h"

//Select MCU
#define MEGA8A
//#define MEGA168A

void UART_Init(void)
{
#ifdef MEGA168
	//Ustawinia predkosci
	UBRR0H = (uint8_t)(__UBRR>>8);
	UBRR0L = (uint8_t)__UBRR;
	//włączam nadajnik i odbiornik
	UCSR0B = (1<<RXEN0) | (1<<TXEN0);
	//Format ramki; 8bit, 1 bit stopu
	//UCSRC = (3<<UCSZ0); // Dla Tiny2313
	//UCSRC = (1<<URSEL) | (3<<UCSZ0); // Dla Mega8A
	UCSR0C = (1<<UCSZ00) | (1<<UCSZ01) | (1<<USBS0); // mega168
#endif

#ifdef MEGA8A
	//Ustawinia predkosci
	UBRRH = (uint8_t)(__UBRR>>8);
	UBRRL = (uint8_t)__UBRR;
	//włączam nadajnik i odbiornik
	UCSRB = (1<<RXEN) | (1<<TXEN);
	//Format ramki; 8bit, 1 bit stopu
	UCSRC = (1<<URSEL) | (3<<UCSZ0); // Dla Mega8A

#endif

}

void UART_send(char i)
{
#ifdef MEGA168
	//czekaj az bufor nadawczy bedzie pusty
	while( !(UCSR0A & (1<<UDRE0)) );
	//zaladuj dane do bufra i wyslij
	UDR0 = i;
#endif

#ifdef MEGA8A
	//czekaj az bufor nadawczy bedzie pusty
	while( !(UCSRA & (1<<UDRE)) );
	//zaladuj dane do bufra i wyslij
	UDR = i;
#endif

}

char UART_readChar(void)
{
	char temp = 0;

   //Wait untill a data is available
#ifdef MEGA168
   while(!(UCSR0A & (1<<RXC0)));
   temp = UDR;
#endif

#ifdef MEGA8A
   while(!(UCSRA & (1<<RXC)));
   temp = UDR;
#endif

   return temp;
}

void UART_sendString(char *string)
{
	while(*string){
		UART_send(*string++);
	}

}

void UART_sendInt(uint8_t liczba)
{
	char bufor[3];
	UART_sendString(itoa(liczba, bufor, 10));
}

void UART_calibrate(void)
{

	uint8_t oscyl = OSCCAL;

	while(oscyl)
	{
	oscyl++;
	OSCCAL = oscyl;
	UART_sendString("Wartosc OSCCAL: ");
	UART_sendInt(oscyl);
	UART_sendString("\r");
	_delay_ms(1000);
	}
}
