 /* Created: 2013-01-23 19:03:35
 *  Author: bk
 */


#include <avr/io.h>
#include <util/delay.h>
//#include <avr/interrupt.h>
#include <avr/pgmspace.h>
//#include <stdlib.h>
//#include <avr/sleep.h>
#include "lcd.h"
#include "1Wire/ds18x20.h"

uint8_t czujniki_cnt;
uint8_t subzero, cel, cel_frac_bits;
void display_temp(void);

int main(void)
{
	DDRB |= (1<<PB0);
	PORTB |= (1<<PB0);

	DDRD &= ~(1<<PD6);
	PORTD &= ~(1<<PD6);

	czujniki_cnt = search_sensors();

	DS18X20_start_meas(DS18X20_POWER_EXTERN, NULL);
	_delay_ms(750);
	if(DS18X20_OK == DS18X20_read_meas_single(0x28, &subzero, &cel, &cel_frac_bits)) display_temp();
	else {
		lcd_puts_P("err");
	}

     lcd_clrscr();   /* clear display home cursor */

 for(;;){


 }


}

void display_temp(void){
	char bufCel[3];
	char bufFrac[3];

	itoa(cel,bufCel,10);
	itoa(cel_frac_bits,bufFrac,10);

	lcd_puts(bufCel);
	lcd_puts_P(".");
	lcd_puts(bufFrac);

}
