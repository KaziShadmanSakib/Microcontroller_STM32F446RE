#include<stm32f446xx.h>
#include<math.h>

#define INPUTA0 (GPIOA->IDR & (1 << 0)) //PA0 Input
#define INPUTA1 (GPIOB->IDR & (1 << 0)) //PB0 Input
#define INPUTA2 (GPIOC->IDR & (1 << 1)) //PC1 Input
#define INPUTA3 (GPIOC->IDR & (1 << 0)) //PC0 Input

#define INPUTB0 (GPIOA->IDR & (1 << 13)) //PA13 Input
#define INPUTB1 (GPIOA->IDR & (1 << 14))  //PA14 Input
#define INPUTB2 (GPIOA->IDR & (1 << 15))  //PA15 Input
#define INPUTB3 (GPIOB->IDR & (1 << 7))  //PB7 Input 

void outputCheck(int binarySum[]){

	if(binarySum[0] == 0){
		GPIOB->BSRR |= 0x1000000; //PB8 off
	}

	if(binarySum[1] == 0){			
		GPIOB->BSRR |= 0x2000000; //PB9 off
	}
			
	if(binarySum[2] == 0){
		GPIOA->BSRR |= 0x200000; //PA5 off
	}
			
	if(binarySum[3] == 0){
		GPIOA->BSRR |= 0x400000; //PA6 off
	}
			
	if(binarySum[4] == 0){
		GPIOA->BSRR |= 0x800000; //PA7 off
	}
			
	if(binarySum[5] == 0){
		GPIOB->BSRR |= 0x400000; //PB6 off
	}
			
	if(binarySum[6] == 0){		
		GPIOC->BSRR |= 0x800000; //PC7 off
	}
			
	if(binarySum[7] == 0){
		GPIOA->BSRR |= 0x2000000; //PA9 off
	}
	
	if(binarySum[0] == 1){
		GPIOB->BSRR |= 0x100; //PB8 on
	}

	if(binarySum[1] == 1){			
		GPIOB->BSRR |= 0x200; //PB9 on
	}
			
	if(binarySum[2] == 1){
		GPIOA->BSRR |= 0x20; //PA5 on
	}
			
	if(binarySum[3] == 1){
		GPIOA->BSRR |= 0x40; //PA6 on
	}
			
	if(binarySum[4] == 1){
		GPIOA->BSRR |= 0x80; //PA7 on
	}
			
	if(binarySum[5] == 1){
		GPIOB->BSRR |= 0x40; //PB6 on
	}
			
	if(binarySum[6] == 1){		
		GPIOC->BSRR |= 0x80; //PC7 on
	}
			
	if(binarySum[7] == 1){
		GPIOA->BSRR |= 0x200; //PA9 on	
	}
		
}

double binaryToDecimal(int inputVal[]){
	double ans = 0;
	int powerVal = 3;
	for(int i=0;i<4;i++){
		if(inputVal[i] == 1){
			ans = ans + pow(2, powerVal);
		}
		powerVal--;
	}
	return ans;
}

int main(){

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOEEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOHEN;
	
	GPIOB->MODER |= 0x10000; //PB8 OUTPUT
	GPIOB->MODER |= 0x40000; //PB9 OUTPUT
	GPIOA->MODER |= 0x400; //PA5 OUTPUT
	GPIOA->MODER |= 0x1000; //PA6 OUTPUT
	GPIOA->MODER |= 0x4000; //PA7 OUTPUT
	GPIOB->MODER |= 0x1000; //PB6 OUTPUT
	GPIOC->MODER |= 0x4000; //PC7 OUTPUT
	GPIOA->MODER |= 0x40000; //PA9 OUTPUT
	
	
	GPIOA->PUPDR |= 0x1; //pull up on PA0 input1
	GPIOB->PUPDR |= 0x1; //pull up on PB0 input1
	GPIOC->PUPDR |= 0x4; //pull up on PC1 input1
	GPIOC->PUPDR |= 0x1; //pull up on PC0 input1
	

	GPIOA->PUPDR |= 0x4000000; //pull up on PA13 input2
	GPIOA->PUPDR |= 0x10000000; //pull up on PA14 input2
	GPIOA->PUPDR |= 0x40000000; //pull up on PA15 input2
	GPIOB->PUPDR |= 0x4000; //pull up on PB7 input2
	
	while(1){
		
		
		int input1Array[4] = {INPUTA0,INPUTA1,INPUTA2,INPUTA3}; // input1
		int input2Array[4] = {INPUTB0, INPUTB1, INPUTB2, INPUTB3}; // input2
		
		// input binary to decimal value
		double input1ToDecimal = binaryToDecimal(input1Array);
		double input2ToDecimal = binaryToDecimal(input2Array);
		// total sum of the input1 and input2
		int ans = (int) input1ToDecimal + (int) input2ToDecimal;
		
		int binarySum[8];
		
		// turning total sum to binary number finally
		for(int i = 0; ans > 0; i++){
			binarySum[i] = ans % 2;
      ans = ans / 2;
    }
		
		//reverse the ans
		int binarySumFinal[8];
		for(int i=0,j=7;i<8;i++){
			binarySumFinal[j] = binarySum[i];
			j--;
		}
		
		//answer
		outputCheck(binarySumFinal);
		
	}
	
}
