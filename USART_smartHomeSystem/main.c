#include "stm32f446xx.h"
#include "RccConfig.h"
#include "Delay.h"
#include "Usart2tConfig.h"
#include "string.h"
#include "stdbool.h"

struct Config
{
	char temp[3];
	char humid[3];
	char water[3];
	char rooms[5];

};
struct Config config;


void setConfig(char string[])
{
	for(int i=0;i < strlen(string)-1;i++)
	{
		if(string[i]=='t' && string[i+1]=='e')
		{
			while(string[i]!=' ')i++;
			i++;
			config.temp[0]=string[i];
			config.temp[1]=string[i+1];
			config.temp[2]=0;
		}
		if(string[i]=='w' && string[i+1]=='a')
		{
			while(string[i]!=' ')i++;
			i++;
			config.water[0]=string[i];
			config.water[1]=string[i+1];
			config.water[2]=0;
		}
		if(string[i]=='h' && string[i+1]=='u')
		{
			while(string[i]!=' ')i++;
			i++;
			config.humid[0]=string[i];
			config.humid[1]=string[i+1];
			config.humid[2]=0;
		}
		if(string[i]=='l' && string[i+1]=='i' )
		{
			while(string[i]!=' ')i++;
			i++;
			config.rooms[0]=string[i];
			config.rooms[1]=string[i+1];
			config.rooms[2]=string[i+2];
			config.rooms[3]=string[i+3];
			config.rooms[4]=0;
		}
	
	}

}


void controlTemp(){	
	
	int input1= (GPIOC->IDR & (1<<0)); 
	int input2= (GPIOC->IDR & (1<<1));
	// if PC0 is 1
	if(input1){
		// if PC1 is 1
		if(input2){					
			// Set Pin PA5
			GPIOA->BSRR |= (1<<5);		
			Delay_ms(30000);					
			// Reset Pin PA5
			GPIOA->BSRR |= (1<<(5+16));		
		}
		// else if PC1 is 0
		else {											
			// set Pin PA6
			GPIOA->BSRR |= (1<<6);			
			Delay_ms(30000);						
			// Reset Pin PA6
			GPIOA->BSRR |= (1<<(6+16));			
		}
	}
	// else if PC0 is 0
	else{
		// if PC1 is 0
		if(!(GPIOC->IDR & (1<<1))){					
			// do nothing
		}
	}
	
}
void controllight()
{
	int night=(GPIOC->IDR & (1<<10));
	int living=(GPIOA->IDR & (1<<0));
	int dining=(GPIOA->IDR & (1<<1));
	int bed=(GPIOA->IDR & (1<<4));
	int office=(GPIOB->IDR & (1<<0));
	if(night)
	{
		if(living && config.rooms[0]=='1') GPIOA->BSRR=(1<<7);
		else GPIOA->BSRR=(1<<(7+16));
		
		if(dining && config.rooms[1]=='1') GPIOB->BSRR=(1<<6);
		else GPIOB->BSRR=(1<<(6+16));
		
		if(bed && config.rooms[2]=='1') GPIOC->BSRR=(1<<7);
		else GPIOC->BSRR=(1<<(7+16));
		
		if(office && config.rooms[3]=='1') GPIOA->BSRR=(1<<9);
		else GPIOA->BSRR=(1<<(9+16));
	}
	else
	{
		GPIOA->BSRR=(1<<(7+16));
		GPIOB->BSRR=(1<<(6+16));
		GPIOC->BSRR=(1<<(7+16));
		GPIOA->BSRR=(1<<(9+16));
	}
	
}
void controlPump()
{
	int waterlevel=10*(config.water[0]-'0')+(config.water[1]-'0');
	if(waterlevel<45)
	{
		GPIOA->BSRR=(1<<5);
		Delay_ms(10000);
		config.water[0]='5';
		config.water[1]='5';
		GPIOA->BSRR=(1<<(5+16));
	}
}
void respond(char string[])
{
	if(string[0]=='C')
		setConfig(string);
	else if(string[0]=='R')
	{
		bool flag=true;
		UART2_SendString("\n");
		for(int i=0;i<strlen(string)-1;i++)
		{
			if(string[i]=='t' && string[i+1]=='e')
			{
				UART2_SendString("temp ");
				UART2_SendString(config.temp);
				UART2_SendString("\n");
				flag=false;
				break;
			}
			if(string[i]=='w' && string[i+1]=='a')
			{
				UART2_SendString("water ");
				UART2_SendString(config.water);
				UART2_SendString("\n");
				flag=false;
				break;
			}
			if(string[i]=='h' && string[i+1]=='u')
			{
				UART2_SendString("humid ");
				UART2_SendString(config.humid);
				UART2_SendString("\n");
				flag=false;
				break;
			}
			if(string[i]=='l' && string[i+1]=='i')
			{
				UART2_SendString("light ");
				UART2_SendString(config.rooms);
				UART2_SendString("\n");
				flag=false;
				break;
			}
		}
		if(flag)
		{
			UART2_SendString("temp ");
			UART2_SendString(config.temp);
			UART2_SendString(" water ");
			UART2_SendString(config.water);
			UART2_SendString(" humid ");
			UART2_SendString(config.humid);
			UART2_SendString(" light ");
			UART2_SendString(config.rooms);
			UART2_SendString("\n");
		
		}
	}

}


char buff[100];
int idx=0;
// Interrupt Service Routine
void USART2_IRQHandler(void)
{
	
	unsigned int statusRegister;
	statusRegister = USART2->SR;
	if(statusRegister & USART_SR_RXNE)
	{			
			
		uint8_t byteData = USART2->DR;	
		USART2->SR &= ~(USART_SR_RXNE);
		if(byteData != '.')
		{					
			buff[idx++] = byteData;
		}
		else 
		{
			buff[idx]=0;
			idx=0;
			respond(buff);
		}
	}
	
}
void setTempGPIO()
{
	//output
	GPIOA->MODER |= (1<<10) | (1<<12);
	//input pull up / down
	GPIOC->PUPDR |=(1<<0)|(1<<2);



}
void setLightGPIO()
{
	//all output moders set PA7 PB6 PC7 PA9
	GPIOA->MODER |=(1<<14) | (1<<18);	
	GPIOB->MODER |=(1<<12);
	GPIOC->MODER |=(1<<14);
	
	
	//inputs PA0 PA1 PA4 PB0
	GPIOB->MODER &= ~((1<<0)|(1<<1));
	GPIOA->PUPDR |=(1<<0)|(1<<2)|(1<<8);
	GPIOB->PUPDR |=(1<<0);
	GPIOC->PUPDR |=(1<<20);

}
void setWaterGPIO()
{
	GPIOA->MODER |= (1<<10);

}

int main (void)
{
	SysClockConfig ();
	
	TIM6Config ();
	
	Uart2Config();
	
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;
	
	
	
	uint8_t string[100];
	getString(string);
	
	setConfig(string);
	NVIC_EnableIRQ(USART2_IRQn);
	
	
	//setTempGPIO();
	setLightGPIO();
	//setWaterGPIO();
	
	while (1)
	{
		//controlTemp();//using IO and is same as humidifier
		controllight();//using interrupt handler and IO
		//controlPump();//using interrupt handler
		
	}
	
	
	
}
