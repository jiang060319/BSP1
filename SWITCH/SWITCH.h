#ifdef __STM32F10X_H
#undef __STM32F10X_H  
#endif


//#define stm32f10x.h "DO_NOT_INCLUDE_STD_LIB"
//#define STM32F10X_H "DO_NOT_INCLUDE_STD_LIB"


//
#ifndef __SWITCH_H
#define __SWITCH_H
#include <stdint.h>
int KEY_GetState(void);


typedef enum {
    KEY1,
    KEY2
} KEY_TypeDef;


#define KEY_RELEASED 0
#define KEY_PRESSED  1


#define KEY1_PORT    GPIOA
#define KEY1_PIN     GPIO_PIN_15
#define KEY1_RCC     RCC_APB2Periph_GPIOA

#define KEY2_PORT    GPIOC
#define KEY2_PIN     GPIO_PIN_5
#define KEY2_RCC     RCC_APB2Periph_GPIOC


void KEY_Init(void);
uint8_t KEY_Scan(KEY_TypeDef key);

#endif
