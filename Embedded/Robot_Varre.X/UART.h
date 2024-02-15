#ifndef UART_H
#define UART_H

void InitUART(void);
void SendMessageDirect(unsigned char* message, int length);
//void _U1RXInterrupt(void);
        
#endif /* UART_H */