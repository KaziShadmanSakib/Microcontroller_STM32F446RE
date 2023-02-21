#include "main.h"
#include<stdio.h>  

uint32_t initialized = 1;
uint32_t uninitializedBss;
const uint32_t readOnlyVariable1 = 100;
const uint8_t readOnlyVariable2 = 100;  

int main(){

	RCC->AHB1ENR |= 0x1;
	GPIOA->MODER |= 0x100; //PA4 for output mode
	
	while(1){

		GPIOA->PUPDR |= 0x4; //pull up for port input PA1
		
		uint32_t  val = GPIOA->IDR & (1<<1); //PA1 input val
		
		if(val == 0){
			
			GPIOA->BSRR |= 0x100000; //PA4 off
		
		}
		
		if(val != 0){
			
			GPIOA->BSRR |= 0x10; //PA4 on
		
		}
			
	}
}
