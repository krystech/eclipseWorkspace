/*
 * dc4017.h
 *
 *  Created on: 09-03-2013
 *      Author: bk
 */

#ifndef DC4017_H_
#define DC4017_H_

#define DC_DIRPORT DDRB
#define DC_PORT PORTB
#define DC_CLOCK PB1	//4017 clock
#define DC_RST PB0		//4017 master reset

#define DC_CLOCK_HI DC_PORT |= (1<<DC_CLOCK)
#define DC_CLOCK_LO DC_PORT &= ~(1<<DC_CLOCK)
#define DC_RST_HI DC_PORT |= (1<<DC_RST)
#define DC_RST_LO DC_PORT &= ~(1<<DC_RST)
#define DC_INIT DC_DIRPORT |= (1<<DC_CLOCK) | (1<<DC_RST)



#endif /* DC4017_H_ */
