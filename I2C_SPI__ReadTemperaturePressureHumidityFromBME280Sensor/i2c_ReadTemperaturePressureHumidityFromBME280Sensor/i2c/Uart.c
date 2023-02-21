#include "RccConfig.h"
#include "Delay.h"
#include "Uart.h"
#include "string.h"
#include "stm32f4xx.h"                  // Device header
#include "stm32f446xx.h"


#define USART2_CR1_UE (1 << 13)
void Uart2Config (void)
{
	// 1. Enable the UART CLOCK
	RCC->APB1ENR |= (1<<17);  // Enable UART2 CLOCK
	
	// 3. Enable the USART by writing the UE bit in USART_CR1 register to 1.
	USART2->CR1 = 0x00;  // clear all
	USART2->CR1 |= USART2_CR1_UE; //Enable USART2
	
	
	// 4. Program the M bit in USART_CR1 to define the word length.
	USART2->CR1 &= ~(unsigned int)(1<<12);  // M =0; 8 bit word length
	
	
	
	// 5. Select the desired baud rate using the USART_BRR register.
	USART2->BRR = (7<<0) | (24<<4);   // Baud rate of 115200, PCLK1 at 45MHz
	
	// 6. Enable the Transmitter/Receiver by Setting the TE and RE bits in USART_CR1 Register
	//USART2->CR1 |= (1<<2); // RE=1.. Enable the Receiver
	//USART2->CR1 |= (1<<3);  // TE=1.. Enable Transmitter
	USART2->CR1 |= USART_CR1_RE;
	USART2->CR1 |= USART_CR1_TE;
}


void UART2_SendChar (uint8_t c)
{	
	USART2->DR = c; // load the data into DR register
	while (!(USART2->SR & (1<<6)));  // Wait for TC to SET.. This indicates that the data has been transmitted
	
}
	
void UART2_SendString (uint8_t *string)
{
	while (*string)
	{
		 UART2_SendChar (*string++);
	}
}
uint8_t getChar(void)
{
	// When the RXNE bit is set. It indicates data has been received and can be read
	while(!(USART2->SR & (1<<5)));
	uint8_t character = USART2->DR;
	return character;
}
//Method for getting full input string.

void enable_interrupts(void)
{
	USART2->CR1 |= USART_CR1_RXNEIE;
	//USART2->CR1 |= USART_CR1_TCIE;
	
}



