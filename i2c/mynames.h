// ulubione oznaczenia

#ifndef _MY_NAMES_H
#define _MY_NAMES_H

#include <stdbool.h>

#define uint unsigned int
#define uchar unsigned char
#define ulong unsigned long

#define forever while(1)
#define EEPROM __attribute__ ((section(".eeprom")))
#define NOINIT __attribute__ ((section(".noinit")))
#define NAKED __attribute__ ((naked))

typedef struct
{
 uchar Flag1:1;
 uchar Flag2:1;
 uchar Flag3:1;
 uchar Flag4:1;
 uchar Flag5:1;
 uchar Flag6:1;
 uchar Flag7:1;
 uchar Flag8:1;
} FlagBits;

typedef union
{
 FlagBits Bits;
 uchar	  Byte;
} Flags;

#endif
