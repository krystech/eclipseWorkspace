/*
 * rtc.h
 *
 *  Created on: 08-03-2013
 *      Author: bk
 */

#ifndef RTC_H_
#define RTC_H_

#define ACK 1
#define NACK 0

//void TWI_start(void);
//void TWI_stop(void);
//void TWI_write(uint8_t bajt);
//uint8_t TWI_read(uint8_t ack);

void TWI_write_buf(uint8_t SLA, uint8_t addr, uint8_t len, uint8_t *buf);
void TWI_read_buf(uint8_t SLA, uint8_t addr, uint8_t len, uint8_t *buf);

uint8_t dec2bcd(uint8_t dec);

void getDate(uint8_t dd, uint8_t mo, uint8_t *buf);
void getTime(uint8_t sec, uint8_t min, uint8_t hour, uint8_t *time);

#endif /* RTC_H_ */
