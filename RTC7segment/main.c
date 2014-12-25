/*
 * main.c
 *
 *  Created on: 29-03-2013
 *      Author: k
 */

#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
//#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "seg7/seg7.h"
#include "twi2313/twi2313.h"
#include "lazy.h"
//#include "I2C_SOFT/i2c_soft.h"

//ustawienia
//	struct
//	{
//		u8 mmTens:4;
//		u8 mmUnit:4;
//		u8 hhTens:2;
//		u8 hhUnit:4;
//	} Time;

volatile u8 sec = 0;
int main(void)
{


	//DDRD &= ~(1<<PD2);
	//PORTD |= (1<<PD2);
	// Ustawienia dla przerwania na INT0 z zegara; co 1s
	// Reakcja na opadajace zbocze

	EIFR |= (1<<INTF0);
	PCMSK |= (1<<PD2);
	MCUCR |= (1<<ISC00);
	GIMSK |= (1<<INT0);

	//Timer 16bit
	//TCCR1B |= (1<<CS11) | (1<<CS10);
	//TIMSK |= (1<<TOIE1);

	// Flaga uplywajacej minuty, po ktorej odswiezam wyswietlacz
	USIFlags.ONE_MINUTE = 1;

	dispInit();
	InitUSI();
	sei();

	u8 clkConf[1];
	clkConf[0] = 0b10000011;
	TwiWriteBuf(0x0D,1,clkConf);
	u8 bufor[2];
	u8 buf[2];
	buf[0] = (4 << 4) | 0;
	buf[1] = (1 << 4 ) |  3;
	TwiWriteBuf(0x03,2,buf);

//	TwiReadBuf(0x03,2,bufor);
//	u8 mmT = (bufor[0] & 0xf0) >> 4;
//	u8 mmU = bufor[0] & 0x0f;
//	u8 hhT = (bufor[1] & 0x30) >> 4;
//	u8 hhU = bufor[1] & 0x0f;
	u8 mmT = 0;
	u8 mmU = 0;
	u8 hhT = 0;
	u8 hhU = 0;




	for(;;)
	{

		if(USIFlags.ONE_MINUTE)
		{
			TwiReadBuf(0x03,2,bufor);
			 mmT = (bufor[0] & 0x70) >> 4;
			 mmU = bufor[0] & 0x0f;
			 hhT = (bufor[1] & 0x30) >> 4;
			 hhU = bufor[1] & 0x0f;

			 USIFlags.ONE_MINUTE = 0;
		}

		test(hhT);
		DIG1_ON;
		_delay_ms(5);
		DIG1_OFF;
		lightsOut();
		//zapal pierwsza liczbe

		test(hhU);
		//zapal pierwsza liczbe
		DIG2_ON;
		_delay_ms(5);
		DIG2_OFF;
		lightsOut();


		test(mmT);
		//zapal pierwsza liczbe
		DIG3_ON;
		_delay_ms(5);
		DIG3_OFF;
		lightsOut();

		test(mmU);
		//zapal pierwsza liczbe
		DIG4_ON;
		_delay_ms(5);
		DIG4_OFF;
		lightsOut();


	}
}

ISR (INT0_vect)
{
	sec++;
	if(sec >= 60) {
		USIFlags.ONE_MINUTE = 1;
		sec = 0;
	}

}
