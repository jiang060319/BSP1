#include "SWITCH.h"
#include "delay.h"
#include "stm32f1xx_hal.h" 


void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};  

    __HAL_RCC_GPIOA_CLK_ENABLE();
		__HAL_RCC_GPIOC_CLK_ENABLE();  

    GPIO_InitStructure.Pin = KEY1_PIN;          
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; 
    GPIO_InitStructure.Pull = GPIO_PULLUP;      
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_LOW;  
    HAL_GPIO_Init(KEY1_PORT, &GPIO_InitStructure);  

    GPIO_InitStructure.Pin = KEY2_PIN;       

    HAL_GPIO_Init(KEY2_PORT, &GPIO_InitStructure);
		 
}


uint8_t KEY_Scan(KEY_TypeDef key)
{
    static uint8_t key_state[2] = {KEY_RELEASED};  
    uint8_t current_state = KEY_RELEASED;


    if(key == KEY1)
    {

        current_state = (HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;
    }
    else if(key == KEY2)
    {
        current_state = (HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;
    }


    if(current_state != key_state[key])
    {
        delay_ms(10);  

 
        if(key == KEY1)
        {
            current_state = (HAL_GPIO_ReadPin(KEY1_PORT, KEY1_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;
        }
        else if(key == KEY2)
        {
            current_state = (HAL_GPIO_ReadPin(KEY2_PORT, KEY2_PIN) == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_RELEASED;
        }

        key_state[key] = current_state;
        return current_state;  
    }

    return KEY_RELEASED; 
}
