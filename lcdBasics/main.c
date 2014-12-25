/*
 * main.c
 *
 *  Created on: Jan 3, 2013
 *      Author: esp
 *
 *      LCD BASICS - wersja dla abstynetów ;]
 *
 *
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdlib.h>
//#include <avr/sleep.h>
#include "lcd.h"
#include "1Wire/ds18x20.h"


#define LIGHT (1<<PB0)
#define LIGHT_TOG PORTB ^= LIGHT

//uint8_t Wcisniety(uint8_t ktory);

volatile char sekundy = 0;
char buf[3]; //temp dla itoa


ISR(TIMER0_OVF_vect){
	static uint8_t wejsc=0;

	wejsc++;
	if(wejsc >= 3){
		sekundy++;
		wejsc=0;
	}

	if(sekundy >= 20){
		sekundy = 0;
//		PORTB &= ~((1<<PB0));
//		MCUCR |= (1<<SE);
//		sleep_mode();

	}



}

//ISR(PCINT_vect){
//
//		MCUCR &= ~((1<<SE));
//		PORTB |= (1<<PB0);
//		sekundy = 0;
//
//}

ISR(BADISR_vect){}


uint8_t czujniki_cnt;
uint8_t subzero, cel, cel_frac_bits;
void display_temp(void);

//Jedziemy !
int main(void){
	//PB0: backlight Anoda
	//PB1: swich 1; lewa
	//PB2: swich 2
	//PB3: swich 3
	DDRB = 0x01 ; //PB0 na wyjscie reszta na wejścia
	PORTB = 0 |(1<<PB1) |(1<<PB0); //niski stan na PB1 zeby działało przerwanie; wysokie stany na swich-ach
	//----------- TIMER; usypianie co 20sec
	//tryb overflow: TCCR0A: WGM00=0 WGM01=0 TCCR0B: WGM02:0
	//CTC TCCR0A: WGM01: 1
	TCCR0B |= (1<<CS00) | (1<<CS02); // Prescale = 1024
	// 1000000 / 1024 = 976.5625 Hz
	// 255 / 976.5625 = 0.26112 sec Licznik przekręci się co 1/4 sec
	TIMSK |= (1<<TOIE0); //Włącz przerwanie po przepełnieniu
	//-------------------

	//---- PCINT; Wake Up na PB1
//	GIMSK |= (1<<PCIE);
//	PCMSK |= (1<<PCINT1);
	//--------------------------




	sei();

	//char napis[7]= "1,2,3,4";
	//char napis2[20] PROGMEM ="Min: 10*C, Max: 25*C";

	Wlacz_LCD();
	Linia(1);
	czujniki_cnt = search_sensors();

	DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);
	_delay_ms(750);
	if(DS18X20_OK == DS18X20_read_meas_single(0x28, &subzero, &cel, &cel_frac_bits)) display_temp();
	else {
		Wyswietl("err",2);
	}
	while(1){

	}
}

void display_temp(void){
	char bufCel[3];
	char bufFrac[3];

	itoa(cel,bufCel,10);
	itoa(cel_frac_bits,bufFrac,10);

	Wyswietl(bufCel,2);
	Wyswietl(".",1);
	Wyswietl(bufFrac,2);

}
//uint8_t Wcisniety(uint8_t ktory){
//
//	uint8_t GuzikWcisniety = !(PINB & (1 << ktory));
//
//	if(GuzikWcisniety){
//		_delay_ms(80);
//			if(GuzikWcisniety) return 1;
//	}
// return 0;
//
//}
