#ifndef CB_TX1
#define CB_TX1

void SendMessage(unsigned char* message, int length);
void CB_TX1_Add(unsigned char value);
unsigned char CB_TX1_Get(void);
void __attribute__((interrupt, no_auto_psv)) _U1TXInterrupt(void);
void SendOne();
unsigned char CB_TX1_IsTransmitting(void);
int CB_TX1_GetDataSize(void);
int CB_TX1_GetRemainingSize(void);

#endif