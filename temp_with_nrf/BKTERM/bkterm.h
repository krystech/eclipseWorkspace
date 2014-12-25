/*
 * bkterm.h
 *
 *  Created on: Jun 14, 2014
 *      Author: esp
 */

#ifndef BKTERM_H_
#define BKTERM_H_

// COLOR DEFS
#define BLACK 0
#define RED	1
#define GREEN 2
#define YELLOW 3
#define BLUE 4
#define MAGENTA 5
#define CYAN 6
#define WHITE 7

// CURSOR DEFS
#define SHOW 1
#define HIDE 0

void term_cls(void);
void term_resetAll(void);
void term_cursorToggle(uint8_t state);
void term_setBGColor(uint8_t color);
void term_setFGColor(uint8_t color);
void term_setAttr(uint8_t attr, uint8_t bgColor, uint8_t fgColor);
void term_cursorXY(uint8_t x, uint8_t y);
void term_drawHLine(char sign, uint8_t length);
void term_drawVLine(char sign, uint8_t length);
void term_newLine(void);

#endif /* BKTERM_H_ */
