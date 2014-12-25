// test dla attiny26
// g³ówny modu³ projektu
#define _MAIN_MOD_ 1
// pliki do³¹czone ( include ) :
#include "projdata.h"
#include <avr\eeprom.h>
#include <avr\interrupt.h>


#include <stdlib.h>
// dane :

volatile uchar ms_counter;
#define BLINK 10
// led kontrolny z PB6 do masy przez 470R

#define TOGGLE_LED PORTB^=_BV(PB6)
// w ten sposób pozostawiamy w³¹czone podci¹gniêcie wejœæ
// funkcje :

//====================
// funkcja main()
int main(void)
{
 // inicjalizacja
 DDRB=_BV(PB6);        // wyjœcie led
 PORTB= ~_BV(PB6);     // wejœcia z podci¹gniêciem ,led niski
 PORTA=0xff;           // wejœcia z podci¹gniêciem
 TOGGLE_LED;           // w³¹cz led

 OSCCAL=eeprom_read_byte((uchar*)E2END);	 // zapis kalibracji w ostatniej komórce
 InitT0();
 InitT1();
 InitUsi();
 sei();
 
 // pêtla g³ówna
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



