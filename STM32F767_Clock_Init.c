/*>>> STM32F767_Clock_Init: ==============================================================
Author:     KPerera
Date:       10/11/2026
Modified:   None

Desc:       Configures the STM32F767ZI system clock to the selected frequency.
            The function configures the PLL, Flash latency, AHB/APB
            prescalers and required clock control registers.
Input:      None
Returns:    None
=============================================================================*/

#include "STM32F767_Clock_Init.h"

void STM32F767_Clock_Init(void)
{
	SCB_EnableICache();	//Enable instruction cache
	SCB_EnableDCache();	//Enable data cache

	FLASH->ACR = 0x00000303; //3 wait states, enable ART accelerator and prefetch

	RCC->CR |= 0x00010001;	//Turn on HSE and HSI
	while((RCC->CR & 0x00000002) == 0);	//Wait until HSIRDY = 1
	while((RCC->CR & 0x00020000) == 0);	//Wait until HSERDY = 1

	RCC->CFGR = 0x00000000; //SYSCLK = HSI
	while((RCC->CFGR & 0x0000000C) != 0);	//Wait until SYSCLK = HSI

	RCC->CR &= ~0x01000000;	//PLL off, HSE and HSI stay on
	while ((RCC->CR & RCC_CR_PLLRDY) != 0);
	RCC->PLLCFGR = 0x04403008;	//HSE = 8MHz, VCO = 2MHz, PLLN = 192, PLLM = 8, PLLQ = 4, PLLP = 2
								// PLLSRC = 1
								//SYSCLK = HSE * PLLN / (PLLM * PLLP) | 96MHz = 8MHz * 192 / (8 * 2)
								//PLLUSB48CK = HSE * PLLN / (PLLM * PLLQ) | 48MHz = 8MHz * 192 / (8 * 4)

	RCC->CR |= 0x01000000;	//PLL ON
	while((RCC->CR & 0x02000000) == 0);	//Wait until PLLRDY = 1

	/*
	//This part is just for 216MHz clock config keep it as a comment if not using
	//Also needs PLLN = 216, 7 wait states, and a new I2C TIMINGR (APB1 would be 54MHz)
	RCC->APB1ENR |= 0x10000000;	//PWREN=1
	PWR->CR1 |= 0x00010000;	//Over drive enabled
	while((PWR->CSR1 & 0x00010000) == 0); //Wait for ODRDY = 1
	PWR->CR1 |= 0x00020000;	//Over drive switching enabled
	while((PWR->CSR1 & 0x00020000) == 0); //Wait for ODSWRDY = 1
	*/

	//Clock for peripherals
	RCC->CFGR = 0x00409402; //PC9=96MHz(MCO2), PA8=16MHz(MCO1), SYSCLK=PLL, AHB=96MHz, APB1 = 24MHz, APB2 = 48MHz
	//Can test the frequency is set correctly using an oscilloscope
	RCC->DCKCFGR1 |= 0x01000000;	//TIMxCLK = HCLK = 96MHz
	while((RCC->CFGR & 0x0000000C) != 0x00000008);	//SYSCLK=PLL
	RCC->CR |= 0x00080000; //CSSON

	RCC->APB2ENR |= 0x00004000; //SYSCFG is enable for I/O compensation cell
	SYSCFG->CMPCR = 0x00000001; //I/O compensation cell enabled
	while((SYSCFG->CMPCR & 0x00000100) == 0); //Wait until CMPRDY = 1
}
