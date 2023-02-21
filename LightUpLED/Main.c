#include "stm32f446xx.h"
#include<stdio.h>  

int main(){
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	GPIOA->MODER |= 0x400;
	
	while(1){
		
		GPIOA->PUPDR |= 0x100000; //pull up for port input PA10
		
		uint32_t  val = GPIOA->IDR & (1<<10); //PA10 input val
		
		if(val == 0){
			
			GPIOA->BSRR |= 0x200000; //PA5 off
		
		}
		
		if(val !=0){
			
			GPIOA->BSRR |= 0x20; //PA5 on
		
		}
			
	}
}
