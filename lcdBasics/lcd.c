/*
 * lcd.c
 *
 *  Created on: Jan 17, 2013
 *      Author: esp
 */

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

#define DDR_LCD DDRD
#define PORT_LCD PORTD		// BAJT = - - RS EN DB4 DB5 DB6 DB7
#define RS_LCD PD5 			//RegSel: 1: dane   		0: instrukcje
#define EN_LCD PD4 			//ENABLE: 1: konfiguruj 	0: zaznacz koniec
#define DB4_LCD PD3
#define DB5_LCD PD2
#define DB6_LCD PD1
#define DB7_LCD PD0

//Deklaracje
void Wlacz_LCD(void);
void Wyslij_do_LCD(char);
void Czysc_LCD(void);
void Wyswietl(char*, uint8_t);
char MirrorBajt(char); // tylko dla bezmyślnie podłączonego LCD ;]
void Przesun(char); // (p)rawo ; (l)ewo
void Linia(char);

//Definicje
void Wlacz_LCD(){

	//DDR_LCD na wyjscia DB7..4, ENABLE, RegSel
	DDR_LCD |= (0x0F) | (1<<EN_LCD) | (1<<RS_LCD);
	//stan niski na wyjsciach
	PORT_LCD &= 0;

	//czekamy min 45ms na ustabilizowanie napięcia
	_delay_ms(45);

	//wysyłam sekwencję na DB4 i DB5
	//1.
	//Ustawiam ENABLE
	PORT_LCD |= _BV(EN_LCD);
	//ustawiam DB4 i DB5
	PORT_LCD |= (1<<DB4_LCD) | (1<<DB5_LCD);
	//Koncze i zatwierdzam przesył
	PORT_LCD &= ~(_BV(EN_LCD));
	//czekam 4.1ms
	_delay_ms(4.1);

	//2.
	//Ustawiam ENABLE
	PORT_LCD |= _BV(EN_LCD);
	//Ponieważ DB4 i DB5 są już ustawione, wystraczy zakończyć przesył
	//konczę przesył
	PORT_LCD &= ~(_BV(EN_LCD));
	//czekam 100us
	_delay_us(100);

	//3.
	//Ustawiam ENABLE
	PORT_LCD |= _BV(EN_LCD);
	//Ponieważ DB4 i DB5 są już ustawione, wystraczy zakończyć przesył
	//konczę przesył
	PORT_LCD &= ~(_BV(EN_LCD));
	//czekam 100us
	_delay_us(100);

	//Konfiguracja wyświetlacza
	//Ustawiam ENABLE
	PORT_LCD |= (1<<EN_LCD);
	//ustawiamy interfejs 4-bitowy; ponieważ w PORT_LCD mamy ustawione DB4 i DB5, zerujemy DB4, zgodnie z notą
	PORT_LCD &= ~(_BV(DB4_LCD));
	//konczę przesył
	PORT_LCD &= ~(_BV(EN_LCD));

	//Parametry
	//Upewniam się że mogę wysyłać instrukcje
	PORT_LCD &= ~(_BV(RS_LCD));
	//4 linie transferu, 2 linie wyswietlacz, matryca 5x8
	Wyslij_do_LCD(0b00101000);
	//przełączam się na dane
	PORT_LCD |= _BV(RS_LCD);

	//Tryb pracy (wprowadzanie danych)
	//Upewniam się że mogę wysyłać instrukcje
	PORT_LCD &= ~(_BV(RS_LCD));
	//inkrementacja adresu po zapisie danych, po zapisie przesun kursor
	Wyslij_do_LCD(0b00000110);
	//przełączam się na dane
	PORT_LCD |= _BV(RS_LCD);

	//Włącz funkcje wyświetlacza
	//Upewniam się że mogę wysyłać instrukcje
	PORT_LCD &= ~(_BV(RS_LCD));
	//wyświetlacz włączony, kursor włączony, migania włączone
	Wyslij_do_LCD(0b00001100);
	//przełączam się na dane
	PORT_LCD |= _BV(RS_LCD);

	Czysc_LCD();

}

void Wyslij_do_LCD(char bajt){

	char b = MirrorBajt(bajt);

	//NAJSTARSZY
	//enable na wysoko
	PORT_LCD |= _BV(EN_LCD);
	//wysyłam 4 najstarsze bity zmiennej "bajt" na PORT_LCD
	PORT_LCD = ((b & 0xF0)>>4)|(PORT_LCD & 0xF0);
	//wysyłam dane do LCD; ENABLE na nisko
	PORT_LCD &= ~(_BV(EN_LCD));

	//czekamy cykl
	asm volatile("nop");

	//NAJMŁODSZY
	//ENABLE na wysoko
	PORT_LCD |= _BV(EN_LCD);
	//wysyłam 4 najmlodsze bity zmiennej "bajt" na PORT_LCD
	PORT_LCD = (b & 0x0F)|(PORT_LCD & 0xF0);
	//wysyłam dane do LCD; ENABLE na nisko
	PORT_LCD &= ~(_BV(EN_LCD));
	//czekam przepisowe 40us
	_delay_us(40);

}

void Czysc_LCD(){
	//przepisz PORT_LCD zerując RegSel
	PORT_LCD &= ~(_BV(RS_LCD));
	//Prześli 00000001 do LCD aby go wyczyścić
	Wyslij_do_LCD(1);
	//ustaw RegSel spowrotem na 1
	PORT_LCD |= _BV(RS_LCD);
	//czekam przepisowe 1.64ms
	_delay_ms(1.64);

}

void Wyswietl(char * napis, uint8_t ile){

	uint8_t i;

	for(i=0; i<ile; i++){
		Wyslij_do_LCD(napis[i]);
	}

}

char MirrorBajt(char bajt){
	// zrób lustrzane odbicie bajtu przed wysłaniem np 1011 -> 1101
	// tylko dla odwrotnie podłączonego wyświetlacza

	bajt = ((bajt>>1)&0x55) | ((bajt<<1)&0xAA);
	bajt = ((bajt>>2)&0x33) | ((bajt<<2)&0xCC);

	return bajt;
}

void Przesun(char jak){
	PORT_LCD &= ~(_BV(RS_LCD));
	if(jak == 'p'){
		Wyslij_do_LCD(0b00011100);
	}else{
		Wyslij_do_LCD(0b00011000);
	}
	PORT_LCD |=_BV(RS_LCD);
}

void Linia(char ktora){
//	PORT_LCD &= ~(_BV(RS_LCD));
//	Wyslij_do_LCD(0b00000010);
//	PORT_LCD |=_BV(RS_LCD);
//	_delay_ms(2);

	PORT_LCD &= ~(_BV(RS_LCD));
	if(ktora == 1){
		Wyslij_do_LCD(0b10000000);
	}else{
		Wyslij_do_LCD(0b11000000);
	}
	PORT_LCD |=_BV(RS_LCD);
	_delay_us(100);
}


