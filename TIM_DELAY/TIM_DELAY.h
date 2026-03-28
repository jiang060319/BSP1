
/**时延方式3：
  *****************************************************************************************************************************************
通用定时器 TIM3 + 查询模式 实现的模块化 us/ms 延时代码*/
//*****************************************************************************************************************************************

#ifndef __TIM_DELAY_H
#define __TIM_DELAY_H

#include <stdint.h>

// 对外导出的函数声明（供main.c、dht_11.o等其他模块调用）
void TIM_Delay_Init(void);                    // 初始化TIM3和打印配置
void TIM_Delay_us(uint16_t us);               // us级延时
void TIM_Delay_ms(uint16_t ms);               // ms级延时
void TIM_Delay_S(uint16_t s);                 // S级延时
void TIM_Delay_S_ms(uint16_t s, uint16_t ms); // S+ms组合延时
void TIM_Delay_test(void);                    // 延时测试（表格打印）

#endif /* __TIM_DELAY_H */
