/*
 * rtc.c
 *
 *  Created on: 08-03-2013
 *      Author: bk
 */
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

#include "rtc.h"

void TWI_start(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTA);
	while( !(TWCR & (1<<TWINT)) );
}

void TWI_stop(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while( !(TWCR & (1<<TWSTO)) );
}

void TWI_write(uint8_t bajt)
{
	TWDR = bajt;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
}

uint8_t TWI_read(uint8_t ack)
{
	TWCR = (1<<TWINT) | (ack<<TWEA) | (1<<TWEN);
	while( !(TWCR & (1<<TWINT)) );
	return TWDR;
}

void TWI_write_buf(uint8_t SLA, uint8_t addr, uint8_t len, uint8_t *buf)
{
	TWI_start();
	TWI_write(SLA);
	TWI_write(addr);
	while(len--) TWI_write(*buf++);
	TWI_stop();
}

void TWI_read_buf(uint8_t SLA, uint8_t addr, uint8_t len, uint8_t *buf)
{
	TWI_start();
	TWI_write(SLA);
	TWI_write(addr);
	TWI_start();
	TWI_write(SLA + 1);
	while(len--) *buf++ = TWI_read(len ? ACK : NACK);
	TWI_stop();

}

uint8_t dec2bcd(uint8_t dec)
{
	return ((dec / 10)<<4) | (dec % 10);

}


void getDate(uint8_t dd, uint8_t mo, uint8_t *buf)
{

	buf[0] = (( ((dd)>>4) & 0x03)*10) + ((dd) & 0x0F); //nr dnia miesiaca
	buf[1] = ((mo)>>5) & 0x07; //nr dnia tygodnia
	buf[2] = ((((mo)>>4) & 0x01)*10) + ((mo) & 0x0F); //nr miesiaca
	buf[3] = ((dd)>>6) & 0x03; // rok <0,3>


}

void getTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t *time)
{
	time[0] = ((hour >>4) & 0x03)*10 + (hour & 0x0F); // miejsce dziesietne godziny;
	//time[1] = hour & 0x0F; //miejsce jednosci godziny
	time[1] = ((min >> 4) & 0x0F)*10 + (min & 0x0F); //miejsce dziesietne minut;
	//time[3] = min & 0x0F; // miejsce jednosci minut;
	time[2] = ((sec >> 4) & 0x0F)*10 + (sec & 0x0F); //miejsce dziesietne sekund;
	//time[5] = sec & 0x0F; // miejsce jednosci sekund;

}
