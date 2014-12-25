// dane projektu
#include "mynames.h"

volatile Flags SysFlags;
volatile Flags UsiFlags;

#define MS100_FLAG SysFlags.Bits.Flag1


#define START_DONE UsiFlags.Bits.Flag1
#define I2C_BUSY UsiFlags.Bits.Flag2

// funkcje globalne

extern void InitT0(void);
extern void InitT1(void);
extern void InitUsi(void);

extern char TestAck(void);


