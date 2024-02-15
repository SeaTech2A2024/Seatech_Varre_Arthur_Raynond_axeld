#ifndef CB_RX1
#define CB_RX1

void CB_RX1_Add(unsigned char value);
unsigned char CB_RX1_Get(void);
unsigned char CB_RX1_IsDataAvailable(void);
void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void);
int CB_RX1_GetDataSize(void);
int CB_RX1_GetRemainingSize(void);

#endif