#include "stm32f446xx.h"
#include <stdint.h>

void SPIConfig (void);
void SPI_Transmit (uint8_t *data, int size);
void SPI_Receive (uint8_t *data, int size);
void SPI_Enable (void);
void SPI_Disable (void);
void CS_Enable (void);
void CS_Disable (void);
