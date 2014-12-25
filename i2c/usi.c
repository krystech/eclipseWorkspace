// obs�uga i2c z u�yciem USI

#include <avr\interrupt.h>
#include <avr\signal.h>
#include "projdata.h"

#define CLEAR_START_FLAG USISR|=_BV(USISIF)
#define TOGGLE_SCL USICR |= _BV(USITC)


// do testu mamy pod��czony PCF8574A
// linie adresowe A0-A2 do masy :
#define PCF_WRITE_ADDR 0x70
#define PCF_READ_ADDR 0x71

// na liniach D0-3 mamy wyj�cia z ledami
// na liniach D4-7 - wej�cia - dipswitch do masy


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
    ToggleScl();   // zbocze opadaj�ce SCL - przesuni�cie rejestru z poprzednim
                   // bitem, slave wystawia nast�pny bit
    ToggleScl();   // zbocze narastaj�ce - slave utrzymuje bit
   }
 inbyte = USIDR;
 if (Ack) USIDR=0x7f; else USIDR=0xff;
 ToggleScl();      // zbocze opadaj�ce - wystawienie ack/noack z MSB rejestru
 ToggleScl();      // zbocze narastaj�ce - ACK/NOACK na linii
 return inbyte;
}


void SckDelay(void)
{
 TCNT1=0;
 TCCR1B |= _BV(CS10);      // start licznika T1
 while ( (TIFR & _BV(TOV1)) ==0 );
 TCCR1B = _BV(CTC1);       // zatrzymanie licznika
 TIFR |= _BV(TOV1);        // skasowanie flagi przepe�nienia
}


void ToggleScl(void)
{
 // prze��czamy stan linii SCL i generujemy op�nienie ok. 5us
 TOGGLE_SCL;
 SckDelay();
}

void SendStart(void)
{
 // na wej�ciu mamy zawsze stany wysokie SCL i SDA
 USIDR=0xff;
 ToggleScl();
 ToggleScl();              // empty clock
 START_DONE=false;
 PORTB &= ~_BV(PB0);       // wymuszenie stanu niskiego SDA rejestrem portu
 while (!START_DONE);      // czekamy na sprz�towe zarejestrowanie startu
 // i robimy dodatkowe op�nienie

 SckDelay();
}

void SendNextStart(void)
{
 // na wej�ciu mamy zawsze stan wysoki SCL, natomiast SDA mo�e by� dowolny
 USIDR=0xff;
 ToggleScl();              // zbocze opadaj�ce SCK
 ToggleScl();              // zbocze narastaj�ce przy wysokim SDA
 START_DONE=false;
 PORTB &= ~_BV(PB0);       // wymuszenie stanu niskiego rejestrem portu
 while (!START_DONE);      // czekamy na sprz�towe zarejestrowanie startu
 // i robimy dodatkowe op�nienie
 SckDelay();
}

void SendStop(void)
{
 // na wej�ciu mamy zawsze stan wysoki SCL, natomiast SDA mo�e by� dowolny
 USIDR=0xff;
 ToggleScl();              // zbocze opadaj�ce SCK
 PORTB &= ~_BV(PB0);       // wymuszenie stanu niskiego rejestrem portu
 ToggleScl();              // zbocze narastaj�ce przy niskim SDA
 PORTB |= _BV(PB0);        // zwolnienie wymuszenia stanu niskiego rejestrem portu
                           // czyli zbocze narastaj�ce SDA przy wysokim SCL >> STOP
}

char SendByte(char AByte)
{
 int i;

 USIDR=AByte;
 for (i=0;i<8;i++)
     {
      ToggleScl();         // zbocze opadaj�ce
      ToggleScl();         // zbocze narastaj�ce
     }
 // kontrola ACK
 // - wys�anie 9. impulsu ze zwolnion� lini� SDA
 USIDR=0xff;
 ToggleScl();         // zbocze opadaj�ce
 ToggleScl();         // zbocze narastaj�ce
 // sprawdzenie co slave wystawi� na linii SDA
 if ((USIDR & _BV(PB0))==0) return 1; else return 0;
}


// funkcje globalne
void InitUsi(void)
{
 // wyb�r trybu pracy
 // przerwanie startu w��czone
 // tryb i2c bez przetrzymywania SCL przepe�nieniem licznika
 // przesuwanie rejestru opadaj�cym zboczem SCL
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
                       // - przy niskim SCL �eby nie by�o traktowane jako stop
 CLEAR_START_FLAG; // zwolnienie SCL
 START_DONE = true;    // informacja dla funkcji
}

// okazuje si�, �e SDA jest mocno wra�liwy na r�ne zak��cenia
// co zreszt� jest wspominane w opisie - nie ma �adnych uk�ad�w
// formuj�cych a tylko 'czyste' io cyfrowe;
// wystarczy manipulacja sond� oscyloskopu na SDA �eby atttiny odebra�a to
// jako start - wtedy sprz�towo blokuje SCL i po transmisji ;
// u�ycie przerwania zapewnia natychmiastowy powr�t do stanu podstawowego.

