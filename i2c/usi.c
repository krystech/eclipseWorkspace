// obs³uga i2c z u¿yciem USI

#include <avr\interrupt.h>
#include <avr\signal.h>
#include "projdata.h"

#define CLEAR_START_FLAG USISR|=_BV(USISIF)
#define TOGGLE_SCL USICR |= _BV(USITC)


// do testu mamy pod³¹czony PCF8574A
// linie adresowe A0-A2 do masy :
#define PCF_WRITE_ADDR 0x70
#define PCF_READ_ADDR 0x71

// na liniach D0-3 mamy wyjœcia z ledami
// na liniach D4-7 - wejœcia - dipswitch do masy


// deklaracje funkcji pomocniczych USI
void SckDelay(void);
void ToggleScl(void);
void SendStart(void);
void SendNextStart(void);
void SendStop(void);
char SendByte(char);
char ReadByte(char);


// funkcje pomocnicze

char ReadByte(char Ack)
{
 int i;
 char inbyte;
 
 USIDR = 0xff;     // master utrzymuje SDA zwolnione
 for(i=0;i<8;i++)
   {
    ToggleScl();   // zbocze opadaj¹ce SCL - przesuniêcie rejestru z poprzednim
                   // bitem, slave wystawia nastêpny bit
    ToggleScl();   // zbocze narastaj¹ce - slave utrzymuje bit
   }
 inbyte = USIDR;
 if (Ack) USIDR=0x7f; else USIDR=0xff;
 ToggleScl();      // zbocze opadaj¹ce - wystawienie ack/noack z MSB rejestru
 ToggleScl();      // zbocze narastaj¹ce - ACK/NOACK na linii
 return inbyte;
}


void SckDelay(void)
{
 TCNT1=0;
 TCCR1B |= _BV(CS10);      // start licznika T1
 while ( (TIFR & _BV(TOV1)) ==0 );
 TCCR1B = _BV(CTC1);       // zatrzymanie licznika
 TIFR |= _BV(TOV1);        // skasowanie flagi przepe³nienia
}


void ToggleScl(void)
{
 // prze³¹czamy stan linii SCL i generujemy opóŸnienie ok. 5us
 TOGGLE_SCL;
 SckDelay();
}

void SendStart(void)
{
 // na wejœciu mamy zawsze stany wysokie SCL i SDA
 USIDR=0xff;
 ToggleScl();
 ToggleScl();              // empty clock
 START_DONE=false;
 PORTB &= ~_BV(PB0);       // wymuszenie stanu niskiego SDA rejestrem portu
 while (!START_DONE);      // czekamy na sprzêtowe zarejestrowanie startu
 // i robimy dodatkowe opóŸnienie

 SckDelay();
}

void SendNextStart(void)
{
 // na wejœciu mamy zawsze stan wysoki SCL, natomiast SDA mo¿e byæ dowolny
 USIDR=0xff;
 ToggleScl();              // zbocze opadaj¹ce SCK
 ToggleScl();              // zbocze narastaj¹ce przy wysokim SDA
 START_DONE=false;
 PORTB &= ~_BV(PB0);       // wymuszenie stanu niskiego rejestrem portu
 while (!START_DONE);      // czekamy na sprzêtowe zarejestrowanie startu
 // i robimy dodatkowe opóŸnienie
 SckDelay();
}

void SendStop(void)
{
 // na wejœciu mamy zawsze stan wysoki SCL, natomiast SDA mo¿e byæ dowolny
 USIDR=0xff;
 ToggleScl();              // zbocze opadaj¹ce SCK
 PORTB &= ~_BV(PB0);       // wymuszenie stanu niskiego rejestrem portu
 ToggleScl();              // zbocze narastaj¹ce przy niskim SDA
 PORTB |= _BV(PB0);        // zwolnienie wymuszenia stanu niskiego rejestrem portu
                           // czyli zbocze narastaj¹ce SDA przy wysokim SCL >> STOP
}

char SendByte(char AByte)
{
 int i;

 USIDR=AByte;
 for (i=0;i<8;i++)
     {
      ToggleScl();         // zbocze opadaj¹ce
      ToggleScl();         // zbocze narastaj¹ce
     }
 // kontrola ACK
 // - wys³anie 9. impulsu ze zwolnion¹ lini¹ SDA
 USIDR=0xff;
 ToggleScl();         // zbocze opadaj¹ce
 ToggleScl();         // zbocze narastaj¹ce
 // sprawdzenie co slave wystawi³ na linii SDA
 if ((USIDR & _BV(PB0))==0) return 1; else return 0;
}


// funkcje globalne
void InitUsi(void)
{
 // wybór trybu pracy
 // przerwanie startu w³¹czone
 // tryb i2c bez przetrzymywania SCL przepe³nieniem licznika
 // przesuwanie rejestru opadaj¹cym zboczem SCL
 // taktowanie pinu SCL programowo bitem USITC
 DDRB |= _BV(PB0) | _BV(PB2);
 PORTB |= _BV(PB0) | _BV(PB2);

 USICR= _BV(USISIE) | _BV(USIWM1) |  _BV(USICS1) | _BV(USICLK);
 CLEAR_START_FLAG;
}

char TestAck(void)
{
 char ack;

 SendStart();
 ack=SendByte(PCF_WRITE_ADDR);
 ack=SendByte(0xfd);
 SendNextStart();
 ack=SendByte(PCF_WRITE_ADDR);
 ack=SendByte(0xfe);
 SendNextStart();
 ack=SendByte(PCF_READ_ADDR);
 ack=ReadByte(0);
 SendStop();
 return ack;
}


SIGNAL (SIG_USI_START)
{
 PORTB |= _BV(PB0);    // zwolnienie wymuszenia stanu niskiego SDA rejestrem portu
                       // - przy niskim SCL ¿eby nie by³o traktowane jako stop
 CLEAR_START_FLAG; // zwolnienie SCL
 START_DONE = true;    // informacja dla funkcji
}

// okazuje siê, ¿e SDA jest mocno wra¿liwy na ró¿ne zak³ócenia
// co zreszt¹ jest wspominane w opisie - nie ma ¿adnych uk³adów
// formuj¹cych a tylko 'czyste' io cyfrowe;
// wystarczy manipulacja sond¹ oscyloskopu na SDA ¿eby atttiny odebra³a to
// jako start - wtedy sprzêtowo blokuje SCL i po transmisji ;
// u¿ycie przerwania zapewnia natychmiastowy powrót do stanu podstawowego.

