#include "Gpio.h"


void gpio_usart_init(void) //Enable Alternate Function mode
{
	GPIOA->MODER |= (2<<4); // AF for pin PA2
	GPIOA->MODER |= (2<<6); // AF for pin PA3
			
	GPIOA->OSPEEDR |= (3 << 4);	//Add high speed to pin PA2
	GPIOA->OSPEEDR |= (3 << 6);	 //Add high speed to pin PA3
	
	GPIOA->AFR[0] |= (7 << 8); // Alternate function setting for pin PA2
	GPIOA->AFR[0] |= (7 << 12); // Alternate function setting for pin PA3
	
	
	//pc2 pc3 pc7 for SPI
	GPIOC->MODER |= (2<<4);//ekhane bhul thakte pare
	GPIOC->MODER |= (2<<6);
	GPIOC->MODER |= (2<<14);
	
	GPIOC->OSPEEDR |= (2<<4);//ekhane bhul thakte pare
	GPIOC->OSPEEDR |= (2<<6);
	GPIOC->OSPEEDR |= (2<<14);
	
	GPIOC->AFR[0] |= (5<<8);
	GPIOC->AFR[0] |= (5<<12);
	GPIOC->AFR[0] |= (5<<28);
	//USed for CSB 
	GPIOA->MODER |= GPIO_MODER_MODER5_0;
	GPIOA->OSPEEDR |= (2<<10);
}

// Select Mode as OUTPUT MODE in the GPIOx_MODER
// GPIOA
void gpio_output_init(void)
{
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
//	GPIOA->MODER |= GPIO_MODER_MODER5_0;
//	GPIOA->MODER |= GPIO_MODER_MODER6_0;
//	GPIOA->MODER |= GPIO_MODER_MODER7_0;
//	GPIOA->MODER |= GPIO_MODER_MODER8_0;
//	GPIOA->MODER |= GPIO_MODER_MODER9_0;
//	GPIOA->MODER |= GPIO_MODER_MODER10_0;
//	
//	//GPIOA->MODER |= (1<<14) | (1<<8) | (1<<9) | (1<<10);
//	GPIOB->MODER |= GPIO_MODER_MODER3_0;
//	GPIOB->MODER |= GPIO_MODER_MODER4_0;
//	GPIOB->MODER |= GPIO_MODER_MODER5_0;
//	GPIOB->MODER |= GPIO_MODER_MODER10_0;
//	GPIOB->MODER |= GPIO_MODER_MODER7_0;
//	
//	//GPIOB->MODER |= (1<<3) | (1<<4) | (1<<5) | (1<<10);
//	
//	GPIOC->MODER |= GPIO_MODER_MODER7_0;
}

