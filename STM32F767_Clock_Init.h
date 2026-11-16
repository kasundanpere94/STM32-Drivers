/*=============================================================================
    File Name:  STM32F767_Clock_Init.h
    Author:     KPerera
    Date:       10/11/2026
    Modified:   None

    Description:
        This header file contains the function prototypes required for 
        STM32F767ZI system clock configuration.

        The clock configuration module supports multiple system clock
        frequencies using the HSE oscillator and main PLL.

    Target MCU: STM32F767ZI
    HSE Clock:  8 MHz
=============================================================================*/

#ifndef STM32F767_Clock_Init_H
#define STM32F767_Clock_Init_H

#include "stm32f767xx.h"

void STM32F767_Clock_Init(void);

#endif /*STM32F767_Clock_Init_H*/
