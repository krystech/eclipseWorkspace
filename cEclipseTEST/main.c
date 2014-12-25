/*
 * cTest.c
 *
 * Created: 2012-12-23 19:07:27
 *  Author: bk
 */


#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRD = 0b00000001;
	PORTD = 0b00000000;

	while(1) {
		PORTD = 0x01;
		_delay_ms(250);
		PORTD = 0x00;
		_delay_ms(250);
	}
}

