#include "delay.h"


void delay_us(uint32_t us) {
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 0;
    uint32_t reload = SysTick->LOAD;  
    ticks = us * (SystemCoreClock / 1000000);  
    told = SysTick->VAL;  
    
    while (tcnt < ticks) {
        tnow = SysTick->VAL;
        if (tnow != told) {
           
            if (tnow < told) {
                tcnt += told - tnow;
            } else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
        }
    }
}


void delay_ms(uint32_t ms) {
    HAL_Delay(ms);
}
