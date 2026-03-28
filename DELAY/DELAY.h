
#ifdef __STM32F10X_H
#undef __STM32F10X_H 
#endif

//#define stm32f10x.h "DO_NOT_INCLUDE_STD_LIB"
//#define STM32F10X_H "DO_NOT_INCLUDE_STD_LIB"

#include "stm32f1xx_hal.h"
#ifndef __DELAY_H   
#define __DELAY_H





void delay_init(uint8_t sysclk);  
void delay_us(uint32_t us);       
void delay_ms(uint32_t ms);      

#endif

