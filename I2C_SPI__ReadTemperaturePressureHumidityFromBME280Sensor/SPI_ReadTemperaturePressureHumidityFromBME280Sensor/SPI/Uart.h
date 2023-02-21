
#include "RccConfig.h"
#include <stdint.h>


void Uart2Config (void);
void UART2_SendChar (uint8_t c);
void UART2_SendString (uint8_t *string);
uint8_t UART2_ReceiveChar (void);
uint8_t getChar(void);
void enable_interrupts(void);
