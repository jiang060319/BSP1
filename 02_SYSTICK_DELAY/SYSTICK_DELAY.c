/************************************************************
  * @brief   SYSTICK_DELAY.c
  * @note    SysTick实现延时
  ***********************************************************/

#include "main.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "SYSTICK_DELAY.h" 


// 全局变量（存储 SysTick 重载值，HAL 库默认已初始化）
extern uint32_t uwTick;
static uint32_t uwTickPrio = TICK_INT_PRIORITY;         // SysTick 中断优先级

void SYSTICK_Delay_Init(void)
{
  // 复用 HAL 库 SysTick 配置（72MHz 时钟下，uwTick 每 1ms 自增 1）
  if (HAL_SYSTICK_Config(SystemCoreClock / 1000) != HAL_OK)
  {
    Error_Handler();
  }
  // 设置 SysTick 中断优先级（与 HAL 库一致）
  HAL_NVIC_SetPriority(SysTick_IRQn, uwTickPrio, 0);
  printf("SYSTICK_Delay启动正常\r");
}

void SYSTICK_Delay_us(uint32_t us)
{
  uint32_t ticks;
  uint32_t told, tnow, tcnt = 0;
  uint32_t reload = SysTick->LOAD;            // SysTick 重载值（72000，对应 1ms）
  ticks = us * (SystemCoreClock / 1000000);   // 计算需要的计数 ticks（72MHz 下 1us=72 ticks）
  told = SysTick->VAL;                        // 记录当前计数器值

  while (1)
  {
    tnow = SysTick->VAL;
    if (tnow != told)
    {
      // 计数器向下计数，需处理溢出
      tcnt += (told > tnow) ? (told - tnow) : (reload - tnow + told);
      told = tnow;
      if (tcnt >= ticks)
      {
        break;                              // 延时达标，退出循环
      }
    }
  }
}

// 直接复用 HAL 库 ms 延时函数（无需重新实现）
void SYSTICK_Delay_ms(uint32_t ms)
{
  HAL_Delay(ms);
}


// 计数1000ms，就是1S
void SYSTICK_Delay_S(uint32_t S)
{
    for(uint32_t i = 0; i < S; i++)
    {
        SYSTICK_Delay_ms(1000);
    }
}


/**
  * @brief  获取 us 级时间戳（核心：结合 uwTick 和 SysTick->VAL）
  */
uint64_t SYSTICK_Delay_Get_TimestampUs(void)
{
  uint32_t tick_ms;
  uint32_t val_us;
  uint64_t timestamp;

  // 关闭中断，确保 uwTick 和 SysTick->VAL 读取原子性（避免中途被 SysTick 中断修改）
  __disable_irq();
  tick_ms = uwTick;                                                       // 读取 ms 整数部分
  val_us = (SysTick->LOAD - SysTick->VAL) / (SystemCoreClock / 1000000);  // 计算 ms 内的 us 部分
  __enable_irq();

  // 总时间戳 = ms * 1000 + us
  timestamp = (uint64_t)tick_ms * 1000 + val_us;
  return timestamp;
}


void SYSTICK_Delay_test()
{
  // 打印测试开始提示
  printf("SysTick 延时测试\r\n");
//  printf("系统时钟：%u MHz\r\n", SystemCoreClock / 1000000);

  uint64_t t_start, t_end;
  uint64_t actual_us;

  #define TEST_MS_VAL   10000                 // 测试 us 延时值

  t_start = SYSTICK_Delay_Get_TimestampUs();  // 记录延时前时间戳
  SYSTICK_Delay_us(TEST_MS_VAL);              // 执行待测试延时
  t_end = SYSTICK_Delay_Get_TimestampUs();    // 记录延时后时间戳

  // 计算实际延时和误差（ms 转 us 提高精度）
  actual_us = (float)(t_end - t_start);

  // 打印结果
  printf("设定延时：%d ms\r\n", TEST_MS_VAL);
  printf("延时前时间戳：%llu us\r\n", t_start);
  printf("延时后时间戳：%llu us\r\n", t_end);
  printf("测试值 %u us, 实际耗时：%lu us\r\n\r\n",TEST_MS_VAL,actual_us);
}
