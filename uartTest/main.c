/*
 * main.c
 *
 *  Created on: 03-03-2013
 *      Author: bk
 */

#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#include "UART/uart.h"
#include "RTC_TWI/rtc.h"
#include "DEBOUNCE/debounce.h"
#include "ADC/adc.h"

enum {ss, mm, hh, dd, mo};
uint8_t bufor[5];

enum {h,m,s};
uint8_t time[3];

enum {day, dayName, month, year};
uint8_t data[4];



//ustawienia
uint8_t setup[7];

int main(void)
{

	//DDRC = (1<<PC3);  // PC3 input
	//PORTC = (1<<PC3); // PC3 pullup; active low

	setup[0] = (1<<3); //maska dla roku i nr dnia tygodnia
	setup[1] = 0; //setne sec
	setup[2] = 0; //sekundy
	setup[3] = dec2bcd(2); //minuty
	setup[4] = dec2bcd(22); //godziny
	setup[5] = dec2bcd(8); // dzien
	setup[6] = dec2bcd(3); // miesiac


	//Inicjalizacja UARTu
	UART_Init();
	UART_calibrate();

	// ustaw zegar
	//TWI_write_buf(0xA2, 0x00, 6 ,setup);

	// Init ADC
	adc_init();
	adc_start();

	sei();

	for(;;)
	{
		//pobranie czasu i daty
		//TWI_read_buf(0xA2, 0x02, 5, bufor);

		// przetworzenie czsu i daty na cos zrozumialego ;]
		//getTime(bufor[ss], bufor[mm], bufor[hh], time);
		//getDate(bufor[dd], bufor[mo], data);

		//UART_sendInt(time[h]);

		//UART_sendString(" : ");
		//UART_sendInt(time[m]);

		//UART_sendString(" : ");
		//UART_sendInt(time[s]);

		//UART_sendString(" - ");
		//UART_sendInt(data[day]);

		//UART_sendString(" / ");
		//UART_sendInt(data[month]);

		//UART_sendString("\r");
		//_delay_ms(1000);

		UART_sendInt(adc_value());
		UART_sendString("\r");
		_delay_ms(1000);


	}
	// Interupts



}


