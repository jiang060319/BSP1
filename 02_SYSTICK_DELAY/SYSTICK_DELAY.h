#ifndef __SYSTICK_DELAY_H__
#define __SYSTICK_DELAY_H__

#include "main.h"


/**时延方式2：
  *****************************************************************************************************************************************
  Cortex-M3 内核自带的 SysTick 定时器（24 位向下计数器），
  HAL 库已经用SysTick封装了 HAL_Delay()（ms 级），通过修改 SysTick 中断或查询模式扩展到 us 级延时，无需占用通用定时器
*/
//*****************************************************************************************************************************************

/**
  * @brief  延时模块初始化（复用 HAL 库 SysTick 配置，无需额外操作）
  */
void SYSTICK_Delay_Init(void);

/**
  * @brief  us 级延时（查询模式，精度±2us）
  * @param  us: 延时时间（0~16777215us，24 位计数器最大范围）
  */
void SYSTICK_Delay_us(uint32_t us);

/**
  * @brief  ms 级延时（复用 HAL_Delay，精度±1ms）
  * @param  ms: 延时时间（0~4294967295ms）
  */
void SYSTICK_Delay_ms(uint32_t ms);

/**
  * @brief  S 级延时
  * @param  S: 延时时间
  */
void SYSTICK_Delay_S(uint32_t S);



/**
  * @brief  获取SysTick时间戳的模块
  * @param  获取us级的时间戳
  */
uint64_t SYSTICK_Delay_Get_TimestampUs(void);



/**
  * @brief  测试时延功能
  * @param  测试时延功能
  */
void SYSTICK_Delay_test(void);

#endif

