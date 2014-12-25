#include <avr/io.h>
#include <util/delay_basic.h>
#include <avr/interrupt.h>

#include "twi2313.h"
#include "../lazy.h"


void InitUSI(void)
{
	DDRB |= (1<<SDA_PIN) | (1<<SCL_PIN);
	PORTB |= (1<<SDA_PIN) | (1<<SCL_PIN);
	// USI mode:
	// przerwanie startu wlaczone
	// tryb i2c brz przytrzymania SCL licznikiem
	// przesuwanie licznika opadajacym zboczem SCL
	// SCL taktowany bitem USITC
	USICR = (1<<USISIE) | (1<<USIWM1) | (1<<USICS1) | (1<<USICLK);
	//CLEAR_START_FLAG;
}

void ToggleSCL(void)
{
	SCL_TOG;
	_delay_loop_1(2); // delay na 6us; zrobic to na timerze
}

void SendStart(void)
{
	// SCL i SDA domyslnie na wysoko
	USIDR=0xff;
	ToggleSCL();
	ToggleSCL();
	PORTB &= ~(1<<SDA_PIN); 		// Sciagam SDA na niski zostawiajc SCL na wysoko
	while(!USIFlags.START_DONE);	// Poczekaj az uC zarejestruje start

	_delay_loop_1(2);				// Dodatkowe opoznienie na 6 us (??)
}

void SendRepStart(void)
{
	// Repeated start
	// SCL wysoki ale SDA dowolny, dlatego:
	USIDR = 0xff;
	ToggleSCL();	// Opadajacy SCL; register USIDR wysuniety na SDA
	ToggleSCL();	// SCL na wysoko

	PORTB &= ~(1<<SDA_PIN); 		// Sciagam SDA na niski zostawiajc SCL na wysoko
	while(!USIFlags.START_DONE);	// Poczekaj az uC zarejestruje start


	_delay_loop_1(2);				// Dodatkowe opoznienie na 6 us (??)
}

void SendStop(void)
{
	// Stop: Przejscie SDA na wysoki przy wysokim SCL
	USIDR = 0xff;
	ToggleSCL();
	PORTB &= ~(1<<SDA_PIN);
	ToggleSCL();
	PORTB |= (1<<SDA_PIN);
}

u8 SendByte(u8 bajt)
{
	u8 i;

	USIDR = bajt; // Laduje dane do rejestru
	for (i=0; i<8; i++) {
		ToggleSCL();	// zbocze padajace; wystawiam bit na SDA; zaczynajac od MSB
		ToggleSCL();	// zbocze rosnace
	}
	// kontrla ACK/NACK
	USIDR = 0xff;		// Zwalniam SDA by przygotowac sie na info od Slave-a
	ToggleSCL();
	ToggleSCL();
	// Sprawdzam co Slave wystawil na linie SDA
	if((USIDR & (1<<0))==0) return 1; /*ACK*/ else return 0; /*NACK*/

}

u8 ReadByte(u8 ack)
{
	u8 i, inbyte;

	USIDR = 0xff; 	// Zwalniam SDA i odbieram bity od Slave-a do USIDR
	for(i=0; i<8; i++) {
		ToggleSCL();
		ToggleSCL();
	}
	inbyte = USIDR;
	// potwierdzenie dla Slave-a; ACK(0) beda kolejne bajty ; NACK(1)koniec trsnsmisji
	if(ack) USIDR = 0x7f /*ACK*/; else USIDR = 0xff; /*NACK*/
	ToggleSCL();
	ToggleSCL();
	return inbyte;
}

void TwiWriteBuf(u8 adr, u8 len, u8 *buf)
{
	SendStart();
	SendByte(DEV_WRITE_ADDR);
	SendByte(adr);

	while(len--) SendByte(*buf++);

	SendStop();
}

void TwiReadBuf(u8 adr, u8 len, u8 *buf)
{
	SendStart();
	SendByte(DEV_WRITE_ADDR);
	SendByte(adr);
	SendRepStart();
	SendByte(DEV_READ_ADDR);

	while(len--) *buf++ = ReadByte(len ? 1 : 0);

	SendStop();
}

// Formatuj dane
//u8 dec2bcd(u8 dec)
//{
//	u8 bcdNum = ((dec / 10) << 4) | (dec % 10);
//	return bcdNum;
//}
//
//
//u8 bcd2dec(u8 bcd)
//{
//	u8 decNum = ((bcd >> 4) & 0x0f) * 10 + (bcd & 0x0f);
//	return decNum;
//}

// Obsluga przerwania po wykryciu startu; przez zaklocenia na SDA
ISR (USI_START_vect)
{
	PORTB |= (1<<SDA_PIN); // wracamy od razu do warunkow pierwotnych; SDA wysokie
	CLEAR_START_FLAG; // czyszcze bit startowy w rejestrze USICR
	USIFlags.START_DONE = 1; // info dla funkcji
}
