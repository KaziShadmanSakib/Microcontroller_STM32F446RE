#include<stm32f446xx.h>

int main(){
	
	RCC->AHB1ENR |= 0x1; // Clock of GPIOA is enabled
	RCC->AHB1ENR |= 0x4; // Clock of GPIOC is enabled 
	GPIOA->MODER |= 0x400; //PA_05 enabled to output mode
	GPIOA->MODER |= 0x1000; //PA_06 enabled to output mode
	GPIOC->MODER |= 0x400; //PC_05 enabled to output mode
	GPIOC->MODER |= 0x1000; //PC_06 enabled to output mode
	
	while(1){
		
		int input1 = GPIOA->IDR & (1<<13); //input1 is in PA13 
		int input2 = GPIOA->IDR & (1<<15); //input2 is in PA15
		
		if(!(input1) && !(input2)){ // 00
			GPIOA->BSRR |= 0x400000;//PA6 reset
			GPIOC->BSRR |= 0x200000; //PC5 reset
			GPIOC->BSRR |= 0x400000; //PC6 reset
			GPIOA->BSRR |= 0x20; //if input is 00 then (PA5) green light will glow
		}
		
		if(!(input1) && (input2)){ // 01
			GPIOC->BSRR |= 0x200000; // PC5 reset
			GPIOC->BSRR |= 0x400000; // PC6 reset
			GPIOA->BSRR |= 0x200000; //PA5 reset
			GPIOA->BSRR |= 0x40;	// if input is 01 then (PA6) white light will glow
		}
		
		if((input1) && !(input2)){ // 10
			GPIOA->BSRR |= 0x400000;//PA6 reset
			GPIOA->BSRR |= 0x200000; //PA5 reset
			GPIOC->BSRR |= 0x400000; //PC6 reset
			GPIOC->BSRR |= 0x20; //if input is 10 then (PC5) yellow light will glow
		}
		
		if((input1) && (input2)){ // 11
			GPIOA->BSRR |= 0x400000;//PA6 reset
			GPIOA->BSRR |= 0x200000; //PA5 reset
			GPIOC->BSRR |= 0x200000; //PC5 reset
			GPIOC->BSRR |= 0x40; // if input is 11 then red light (PC6) will glow
		}
	}
}
