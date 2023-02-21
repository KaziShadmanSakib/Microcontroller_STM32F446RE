#include "SPI2_Config.h"

int spi_debug = 0;
uint8_t spi_reg_address = 0;

void SPIConfig (void)
{
//spi2 is in apb1 ref man page - 
  RCC->APB1ENR |= (1<<14);  // Enable SPI2 CLock
	
  //SPI2->CR1 |= (1<<0)|(1<<1);   // CPOL=1, CPHA=1
	
  SPI2->CR1 |= (1<<2);  // Master Mode
	
  SPI2->CR1 |= (2<<3);  // BR[2:0] = 010: fPCLK/8, PCLK1 = 45MHz, SPI clk = 5.625 MHz
	
  SPI2->CR1 &= ~(unsigned) (1<<7);  // LSBFIRST = 0, MSB first
	
  SPI2->CR1 |= (1u<<8) | (1u<<9);  // SSM=1, SSi=1 -> Software Slave Management
	
  SPI2->CR1 &= ~(unsigned) (1<<10);  // RXONLY = 0, full-duplex
	
  SPI2->CR1 &= ~(unsigned)(1<<11);  // DFF=0, 8 bit data
	
  SPI2->CR2 = 0;
}

void SPI_Transmit (uint8_t *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the TXE bit to set in the Status Register
	2. Write the data to the Data Register
	3. After the data has been transmitted, wait for the BSY bit to reset in Status Register
	4. Clear the Overrun flag by reading DR and SR
	************************************************/		
	int i=0;
	while (i<size)
	{
	   while (!((SPI2->SR)&(1<<1)));  // wait for TXE bit to set -> This will indicate that the buffer is empty
		 SPI2->DR = data[i]; 		// load the data into the Data Register
		 i++;
	}	
	
/*During discontinuous communications, there is a 2 APB clock period delay between the
write operation to the SPI_DR register and BSY bit setting. As a consequence it is
mandatory to wait first until TXE is set and then until BSY is cleared after writing the last
data.
*/
	while (!((SPI2->SR)&(1<<1)));  // wait for TXE bit to set -> This will indicate that the buffer is empty
	while (((SPI2->SR)&(1<<7)));  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication	
	
		
	//  Clear the Overrun flag by reading DR and SR
	uint8_t temp = (uint8_t)SPI2->DR;
					temp = (uint8_t)SPI2->SR;
		
}

void SPI_Receive (uint8_t *data, int size)
{
	/************** STEPS TO FOLLOW *****************
	1. Wait for the BSY bit to reset in Status Register
	2. Send some Dummy data before reading the DATA
	3. Wait for the RXNE bit to Set in the status Register
	4. Read data from Data Register
	************************************************/		

	while (size)
	{
		while (((SPI2->SR)&(1<<7))) {};  // wait for BSY bit to Reset -> This will indicate that SPI is not busy in communication
		SPI2->DR = 0;  // send dummy data
		while (!((SPI2->SR) &(1<<0))){};  // Wait for RXNE to set -> This will indicate that the Rx buffer is not empty
		*data++ = SPI2->DR;		
		size--;
	}	
}


void SPI_Enable (void)
{
	SPI2->CR1 |= (unsigned)(1<<6);   // SPE=1, Peripheral enabled
}

void SPI_Disable (void)
{
	SPI2->CR1 &= ~(unsigned)(1<<6);   // SPE=0, Peripheral Disabled
}

void CS_Enable (void)
{
	GPIOA->BSRR |= (unsigned)(1<<5)<<16;
}

void CS_Disable (void)
{
	GPIOA->BSRR |= (unsigned)(1<<5);
}


