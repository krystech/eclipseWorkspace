/*
 * lcd.h
 *
 *  Created on: Jan 17, 2013
 *      Author: esp
 */

#ifndef LCD_H_
#define LCD_H_

//Deklaracje
void Wlacz_LCD(void);
void Wyslij_do_LCD(char);
void Czysc_LCD(void);
void Wyswietl(char*, uint8_t);
char MirrorBajt(char); // tylko dla bezmyślnie podłączonego LCD ;]
void Przesun(char); // (p)rawo ; (l)ewo
void Linia(char);

#endif /* LCD_H_ */
