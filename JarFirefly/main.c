/*
 * main.c
 *
 *  Created on: 10-02-2013
 *      Author: bk
 */

/*
 *
 * TODO
 * 1. Zapisac do "chan" adres OCR0A/B
 *    Odczytywac i zapisywac OCR0A/B przez pointer
 *
 * 2. PowerDown na Timerze; w razie niepstrykniecia zasilania
 *
 * 3. Zrobić 'inteligentny' random dla kanałów i wietlików
 *
 *
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <stdlib.h>

#define ALL_FLYS_ON (1<<PB2) | (1<<PB3) | (1<<PB4)
#define u8 uint8_t

void fadeInOut(const u8 *channel, const u8 *flyNo);

const u8 chan[20] PROGMEM = {
		1, 0,
		1, 0,
		0, 1,
		1, 0,
		0, 1,
		0, 1,
		1, 0,
		0, 1,
		1, 0,
		0, 1
};
const u8 fly[20] PROGMEM = {
		2,3,3,2,2,3,3,2,2,3,2,3,3,2,2,3,3,2,2,3
};

int main(void)
{
	DDRB =  ALL_FLYS_ON;
	PORTB = ALL_FLYS_ON;

	TCCR0A |= (1<<WGM00) | (1<<WGM01); //Fast PWM
	TCCR0A |= (1<<COM0A1); // Set on compare match
	TCCR0A |= (1<<COM0B1); // Set on compare match
	TCCR0B |= (1<<CS00); // Clk / 1

	/*
	 * Clk = 4800000 Hz
	 * Fpwm = Clk / prescal / 256
	 * presk = 1		Fpwm = 17968,75
	 * presk = 8		Fpwm = 2246 Hz
	 * presk = 64		Fpwm = 280 Hz
	 * presk = 256		Fpwm = 70 Hz
	 * presk = 1024		Fpwm = 17,54 Hz
	 *
	 * Clk = 4800000 / 8 = 575000 Hz (1 MHz, 1.8V: 240 µA)
	 * prescal = 1		Fpwm = 2246 Hz
	 *
	 */




	for(;;)
	{
		fadeInOut(chan, fly);
	}

}


void fadeInOut(const u8 *channel, const u8 *flyNo)
{
	u8 i;
	u8 j;

	for(j=0; j<=20; j++)
	{

		PORTB &= ~(1<<pgm_read_byte(flyNo + j)); //Zapal odpowieni LED

			switch(pgm_read_byte(channel + j)) // Który kanał PWM odpalić
			{
			case 0:
				DDRB |= (1<<PB0);
				for(i=0; i<255; i++)
					{
						OCR0A = i;
						_delay_ms(150);

					}

				_delay_ms(200);

					for(i = 255; i; i--)
					{

						OCR0A = i;
						_delay_ms(150);

					}

				DDRB &= ~(1<<PB0);
				PORTB |= ALL_FLYS_ON;
				_delay_ms(200);
				break;

			case 1:
				DDRB |= (1<<PB1);
				for(i=0; i<255; i++)
					{
						OCR0B = i;
						_delay_ms(150);

					}
				_delay_ms(200);
					for(i = 255; i; i--)
					{

						OCR0B = i;
						_delay_ms(150);

					}

					DDRB &= ~(1<<PB1);
					PORTB |= ALL_FLYS_ON;
					_delay_ms(200);
				break;


			}




	}
}
