/*
 * main.c
 *
 *  Created on: 09-03-2013
 *      Author: bk
 *
 *      Pierwsza wersja wyswietlacza na pająku
 *      2 x 74HC595 + 4017
 *      74HC595:
 *      D: data pin
 *      S: SHCP; shift register
 *      L: STCP; latch register
 *      4017:
 *      CL: clock pin
 *      R: master reset
 */

#include <avr/io.h>
#include <util/delay.h>
//#include <stdlib.h>
#include <avr/pgmspace.h>
//#include <avr/interrupt.h>
#include "SPI/spi.h"
#include "DC4017/dc4017.h"

//SKRÓTY
#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

//USTAWINIA
#define ROWS 18
#define COLS 8
#define NUM_ELEMENTS(x) (sizeof(x)/ROWS)

// ZNAKI 8x8
#define SPACE {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}
#define sbl {0x70,0x40,0x40,0x40,0x40,0x40,0x70,0x00} //
#define stilde {0x00,0x04,0x08,0x10,0x20,0x40,0x00,0x00} //
#define sbr {0x70,0x10,0x10,0x10,0x10,0x10,0x70,0x00} //
#define sus {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC} //
#define d0 {0x38,0x44,0x44,0x44,0x44,0x44,0x44,0x38}
#define d1 {0x20,0x60,0x20,0x20,0x20,0x20,0x20,0x70}
#define d2 {0x38,0x44,0x04,0x08,0x10,0x20,0x40,0x7C}
#define d3 {0x38,0x44,0x04,0x18,0x04,0x04,0x44,0x38}
#define d4 {0x08,0x18,0x28,0x48,0x7C,0x08,0x08,0x08}
#define d5 {0x7C,0x40,0x40,0x78,0x04,0x04,0x44,0x38}
#define d6 {0x18,0x20,0x40,0x78,0x44,0x44,0x44,0x38}
#define d7 {0x7C,0x04,0x08,0x08,0x10,0x10,0x20,0x20}
#define d8 {0x38,0x44,0x44,0x38,0x44,0x44,0x44,0x38}
#define d9 {0x38,0x44,0x44,0x44,0x3C,0x04,0x08,0x30}
#define A {0x70,0x88,0x88,0x88,0xF8,0x88,0x88,0x00}
#define B {0xF0,0x88,0x88,0xF0,0x88,0x88,0xF0,0x00}
#define C {0x70,0x88,0x80,0x80,0x80,0x88,0x70,0x00}
#define D {0xF0,0x88,0x88,0x88,0x88,0x88,0xF0,0x00}
#define E {0xF8,0x80,0x80,0xF0,0x80,0x80,0xF8,0x00}
#define F {0xF8,0x80,0x80,0xF0,0x80,0x80,0x80,0x00}
#define G {0x70,0x88,0x80,0xB8,0x88,0x88,0x78,0x00}
#define H {0x88,0x88,0x88,0xF8,0x88,0x88,0x88,0x00}
#define I {0x70,0x20,0x20,0x20,0x20,0x20,0x70,0x00}
#define J {0x08,0x08,0x08,0x08,0x88,0x88,0x70,0x00}
#define K {0x88,0x90,0xA0,0xC0,0xA0,0x90,0x88,0x00}
#define L {0x80,0x80,0x80,0x80,0x80,0x80,0xF8,0x00}
#define M {0x88,0xD8,0xA8,0x88,0x88,0x88,0x88,0x00}
#define N {0x88,0xC8,0xA8,0x98,0x88,0x88,0x88,0x00}
#define O {0x70,0x88,0x88,0x88,0x88,0x88,0x70,0x00}
#define P {0xF0,0x88,0x88,0xF0,0x80,0x80,0x80,0x00}
#define Q {0x70,0x88,0x88,0x88,0xA8,0x90,0x68,0x00}
#define R {0xF0,0x88,0x88,0xF0,0x90,0x88,0x88,0x00}
#define S {0x70,0x88,0x80,0x70,0x08,0x88,0x70,0x00}
#define T {0xF8,0x20,0x20,0x20,0x20,0x20,0x20,0x00}
#define U {0x88,0x88,0x88,0x88,0x88,0x88,0x70,0x00}
#define V {0x88,0x88,0x88,0x88,0x88,0x50,0x20,0x00}
#define W {0x88,0x88,0xA8,0xA8,0xA8,0xA8,0x50,0x00}
#define X {0x88,0x88,0x50,0x20,0x50,0x88,0x88,0x00}
#define Y {0x88,0x88,0x88,0x50,0x20,0x20,0x20,0x00}
#define Z {0x78,0x08,0x10,0x20,0x40,0x40,0x78,0x00}
#define a {0x00,0x00,0x70,0x08,0x78,0x88,0x78,0x00}
#define b {0x80,0x80,0xF0,0x88,0x88,0x88,0xF0,0x00}
#define c {0x00,0x00,0x70,0x88,0x80,0x88,0x70,0x00}
#define d {0x08,0x08,0x78,0x88,0x88,0x88,0x78,0x00}
#define e {0x00,0x00,0x70,0x88,0xF0,0x80,0x70,0x00}
#define f {0x18,0x20,0x20,0x78,0x20,0x20,0x20,0x00}
#define g {0x00,0x00,0x78,0x88,0x88,0x78,0x08,0x70}
#define h {0x40,0x40,0x70,0x48,0x48,0x48,0x48,0x00}
#define i {0x20,0x00,0x20,0x20,0x20,0x20,0x30,0x00}
#define j {0x08,0x00,0x18,0x08,0x08,0x08,0x48,0x30}
#define k {0x40,0x40,0x48,0x50,0x60,0x50,0x48,0x00}
#define l {0x20,0x20,0x20,0x20,0x20,0x20,0x30,0x00}
#define m {0x00,0x00,0xD0,0xA8,0xA8,0x88,0x88,0x00}
#define n {0x00,0x00,0x70,0x48,0x48,0x48,0x48,0x00}
#define o {0x00,0x00,0x70,0x88,0x88,0x88,0x70,0x00}
#define p {0x00,0x00,0xF0,0x88,0x88,0x88,0xF0,0x80}
#define q {0x00,0x00,0x78,0x88,0x88,0x88,0x78,0x08}
#define r {0x00,0x00,0xB0,0x48,0x40,0x40,0xE0,0x00}
#define s {0x00,0x00,0x70,0x80,0x70,0x08,0x70,0x00}
#define t {0x00,0x20,0x78,0x20,0x20,0x28,0x10,0x00}
#define u {0x00,0x00,0x48,0x48,0x48,0x58,0x28,0x00}
#define v {0x00,0x00,0x88,0x88,0x88,0x50,0x20,0x00}
#define w {0x00,0x00,0x88,0x88,0xA8,0xF8,0x50,0x00}
#define x {0x00,0x00,0x48,0x48,0x30,0x48,0x48,0x00}
#define y {0x00,0x00,0x48,0x48,0x48,0x38,0x10,0x60}
#define z {0x00,0x00,0x78,0x08,0x30,0x40,0x78,0x00}



const uint8_t test[ROWS][COLS] PROGMEM ={s,o,b,o,t,a,SPACE,d1,d9,stilde,d1,d0,stilde,d2,d0,d1,d3,SPACE};
void shiftLeft(const u8 array[][COLS], u8 size);



int main(void)
{
	
	//uruchom SPI
	SPI_Init();
	//Init dla 4017;
	DC_INIT;
	DC_RST_HI; //RESET
	DC_RST_LO; //RESET


	for(;;)
	{
		//wyslij i czekaj
		shiftLeft(test,NUM_ELEMENTS(test));


	}
}

void shiftLeft(const u8 array[][COLS], u8 size){

	u8 rows;
	u8 cols;
	u8 shift_loops;
	u8 shift;

for(rows=0; rows<ROWS-1; rows++){ // rzędy przekazanej macierzy
	for(shift=0; shift<7; shift++){ // przesuniecie w lewo
		for(shift_loops=0;shift_loops<=20;shift_loops++){ // opóźnienie przuswania
			for(cols=0;cols<size;cols++){ // bajty w kolumnie danego rzędu
					SPI_send8((pgm_read_byte(&(array[rows][cols]))<<shift) | (pgm_read_byte(&(array[rows+1][cols]))>>(7-shift)));
			}
			DC_RST_HI;
			DC_RST_LO;

		}

		}

}


}