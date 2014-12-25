/*
 * twi2313.h
 *
 *  Created on: 06-04-2013
 *      Author: k
 */

#ifndef TWI2313_H_
#define TWI2313_H_

#include "../lazy.h"

// Pin Conf
#define SDA_PIN PB5
#define SCL_PIN PB7
// Toggle SCL
#define SCL_TOG USICR |= (1<<USITC)
// Clear start cond flag
#define CLEAR_START_FLAG USISR |= (1<<USISIF);

//Ustawienie dla Slave-a (PCF8583)
#define DEV_WRITE_ADDR 0xA2
#define DEV_READ_ADDR 0xA3

// Pomoce
struct
{
	u8 START_DONE:1;
	u8 ONE_MINUTE:1;
} USIFlags;


// Funckcje

void ToggleSCL(void);
void SendStart(void);
void SendRepStart(void);
void SendStop(void);
u8 SendByte(u8 bajt);
u8 ReadByte(u8 ack);

//u8 dec2bcd(u8 dec);
//u8 bcd2dec(u8 bcd);

void InitUSI(void);
void TwiWriteBuf(u8 adr, u8 len, u8 *buf);
void TwiReadBuf(u8 adr, u8 len, u8 *buf);



#endif /* TWI2313_H_ */
