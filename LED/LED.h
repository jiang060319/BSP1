

#ifndef __led_H__
#define __led_H__

#define led0_ON     do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);}while(0)
#define led0_OFF    do{HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);}while(0)
#define led0_Toggle do{HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_8);}while(0)

#define led1_ON     do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);}while(0)
#define led1_OFF    do{HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);}while(0)
#define led1_Toggle do{HAL_GPIO_TogglePin(GPIOD, GPIO_PIN_2);}while(0)

#endif


