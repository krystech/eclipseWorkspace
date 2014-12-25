// test dla attiny26
// g��wny modu� projektu
#define _MAIN_MOD_ 1
// pliki do��czone ( include ) :
#include "projdata.h"
#include <avr\eeprom.h>
#include <avr\interrupt.h>


#include <stdlib.h>
// dane :

volatile uchar ms_counter;
#define BLINK 10
// led kontrolny z PB6 do masy przez 470R

#define TOGGLE_LED PORTB^=_BV(PB6)
// w ten spos�b pozostawiamy w��czone podci�gni�cie wej��
// funkcje :

//====================
// funkcja main()
int main(void)
{
 // inicjalizacja
 DDRB=_BV(PB6);        // wyj�cie led
 PORTB= ~_BV(PB6);     // wej�cia z podci�gni�ciem ,led niski
 PORTA=0xff;           // wej�cia z podci�gni�ciem
 TOGGLE_LED;           // w��cz led

 OSCCAL=eeprom_read_byte((uchar*)E2END);	 // zapis kalibracji w ostatniej kom�rce
 InitT0();
 InitT1();
 InitUsi();
 sei();
 
 // p�tla g��wna
 forever
  {
   if (MS100_FLAG)
      {
       MS100_FLAG=false;
       ms_counter++;
      }
      
   if (ms_counter == BLINK)
      {
       ms_counter=0;
       if (TestAck()==0x7e) TOGGLE_LED;
      }
  }

}



