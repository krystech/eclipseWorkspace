// obs³uga timerów

// oscylator ustawiony na 8 MHz
// weŸmiemy prescaler 64 dla T0
// co daje tick=8us
// przerwanie co 125 tików = 1 ms

#include <avr\interrupt.h>
#include <avr\signal.h>
#include "projdata.h"

uchar T0_counter;

void InitT0(void)
{
 TCNT0 = -125;
 TCCR0=3;        // preskaler 64
 TIMSK |= _BV(TOIE0);
}

// licznik T1 pos³u¿y do odliczania opóŸnieñ przy taktowaniu SCK
// bêdzie on pracowaæ w trybie synchronicznym ( z systemowego
// oscylatora z preskalerem 1;
// wartoœæ maksymaln¹ ustawimy na 40 (5 us)
// przerwanie pozostaje zablokowane - sprawdzamy programowo flagê

void InitT1(void)
{
 OCR1A=0x80;
 OCR1B=0x80;
 OCR1C=40;
 TCCR1A=0; // praca normalna
 TCCR1B = _BV(CTC1);
}


SIGNAL (SIG_OVERFLOW0)
{
 TCNT0 += -125;
 if (++T0_counter == 100)
    {
     T0_counter=0;
     MS100_FLAG = true;
    }
}


