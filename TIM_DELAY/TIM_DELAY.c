/************************************************************
  * @brief   TIM_DELAY.c（TIM3定时器）
  * @note    全寄存器操作，延时期屏蔽中断
  ***********************************************************/

#include "stm32f1xx.h"
#include "TIM_DELAY.h"
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

// 全局变量（清中断使用）
static uint8_t irq_state = 0U; 

// -------------------------- 函数声明 --------------------------
static void Lock_Interrupts(void);
static void Unlock_Interrupts(void);
static uint32_t Calculate_Real_Time(uint16_t start, uint16_t end);
static uint32_t Segment_Delay(uint32_t total_us);

// -------------------------- 初始化函数 --------------------------
void TIM_Delay_Init(void)
{
  // TIM3硬件初始化
  RCC->APB1RSTR |= RCC_APB1RSTR_TIM3RST;
  RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM3RST;
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  
  TIM3->CR1 = 0U;
  TIM3->CR2 = 0U;
  TIM3->SMCR = 0U;
  TIM3->DIER = 0U;
  TIM3->DCR = 0U;
  TIM3->PSC = 35U;         // 36MHz / 36 = 1MHz
  TIM3->ARR = 0xFFFFU;     // 最大计数65535
  TIM3->CNT = 0U;          // 清零计数器
  TIM3->EGR |= TIM_EGR_UG; // 加载配置
  TIM3->CR1 |= TIM_CR1_CEN; // 启动TIM3

  // 美化打印
  printf("\r\n=============================================\r\n");
  printf("【TIM3延时功能初始化完成】\r\n");
  printf("  核心配置：\r\n");
  printf("    - APB1时钟：36MHz\r\n");
  printf("    - TIM3计数频率：1MHz（1us/计数）\r\n");
  printf("=============================================\r\n");

}

// -------------------------- 静态辅助函数 --------------------------
/**
  * @brief  禁用所有中断（硬锁，避免计时干扰）
  */
static void Lock_Interrupts(void)
{
  irq_state = __get_PRIMASK(); // 保存当前中断状态
  __disable_irq();             // 禁用全局中断
}

/**
  * @brief  恢复中断状态
  */
static void Unlock_Interrupts(void)
{
  __set_PRIMASK(irq_state & 0x01);// 恢复中断状态
}

/**
  * @brief  计算真实耗时（处理16位计数器溢出）
  * @param  start：起点计数（uint16_t）
  * @param  end：终点计数（uint16_t）
  * @return 真实耗时（us，uint32_t）
  */
static uint32_t Calculate_Real_Time(uint16_t start, uint16_t end)
{
  uint32_t real_time = 0U;
  if (end >= start)
  {
    real_time = (uint32_t)end - (uint32_t)start;
  }
  else
  {
    // 溢出处理：65536 - start + end（16位计数器溢出一次为65536us）
    real_time = (uint32_t)(0x10000U - start) + (uint32_t)end;
  }
  return real_time;
}

/**
  * @brief  分段计时（避免长时间关中断，解决大时延串口阻塞）
  * @param  total_us：总计时时间（us）
  * @return 实际总耗时（us）
  */
static uint32_t Segment_Delay(uint32_t total_us)
{
  uint32_t elapsed_us = 0U; // 已流逝时间
  uint16_t start_cnt = 0U;
  uint16_t end_cnt = 0U;

  while (elapsed_us < total_us)
  {
    // 每次分段计时≤60ms，避免长时间关中断导致串口阻塞
    uint32_t segment_us = (total_us - elapsed_us) > 60000U ? 60000U : (total_us - elapsed_us);

    Lock_Interrupts();
    TIM3->CNT = 0U;
    start_cnt = TIM3->CNT;
    while (Calculate_Real_Time(start_cnt, TIM3->CNT) < segment_us);
    end_cnt = TIM3->CNT;
    Unlock_Interrupts();

    // 累加耗时
    elapsed_us += Calculate_Real_Time(start_cnt, end_cnt);

    // 小延时，确保串口缓冲区处理
    for (uint32_t i = 0U; i < 1000U; i++);
  }

  return elapsed_us;
}

// -------------------------- 延时函数 --------------------------
/**
  * @brief  精准us级延时（对外导出，供其他模块调用）
  * @param  us：延时时间（0~65535us）
  */
void TIM_Delay_us(uint16_t us)
{
    if (us == 0U || us > 0xFFFFU) return;

    // 36MHz系统时钟下，1us ≈ 12个指令周期（__NOP()为1时钟周期）
    uint32_t cycles = (uint32_t)us * 12; 
    __disable_irq(); // 临时关闭中断，避免中断打断延时（可选）
    while (cycles--)
    {
        __NOP();
    }
    __enable_irq(); // 恢复中断
}


/**
  * @brief  精准ms级延时（对外导出，供其他模块调用）
  * @param  ms：延时时间（≥0ms）
  */
void TIM_Delay_ms(uint16_t ms)
{
  if (ms == 0U) return;

  uint32_t total_us = (uint32_t)ms * 1000U;
  Segment_Delay(total_us);
}

/**
  * @brief  精准S级延时（对外导出，供其他模块调用）
  * @param  s：延时时间（≥0S）
  */
void TIM_Delay_S(uint16_t s)
{
  if (s == 0U) return;

  uint32_t total_us = (uint32_t)s * 1000000U;
  Segment_Delay(total_us);
}

/**
  * @brief  增强版延时（s+ms组合，对外导出）
  * @param  s：秒数，ms：毫秒数（0~999）
  */
void TIM_Delay_S_ms(uint16_t s, uint16_t ms)
{
  if (s > 0U) TIM_Delay_S(s);
  if (ms > 0U && ms <= 999U) TIM_Delay_ms(ms);
}

// -------------------------- 测试函数 --------------------------
/**
  * @brief  TIM3延时测试（表格美化打印，所有项正常输出）
  */
// -------------------------- 测试函数（表格对齐优化） --------------------------
void TIM_Delay_test(void)
{
  uint16_t start_cnt = 0U;
  uint16_t end_cnt = 0U;
  uint32_t real_us = 0U;
  int32_t error = 0;

  printf("\r\n=============================================\r\n");
  printf("【TIM3延时功能开始测试】\r\n");
  printf("  核心配置：\r\n");
  printf("    - APB1时钟：36MHz\r\n");
  printf("    - TIM3计数频率：1MHz（1us/计数）\r\n");
  printf("    - 测试覆盖：1000us、2ms、10ms、70ms、2S\r\n");
  printf("=============================================\r\n");


  // 表头（固定列宽度，确保对齐）
  printf("+-------------+------------+------------+-------------+-------------+---------+--------+\r\n");
  printf("|  测试项     | 起点(start)| 终点(end)  | 真实耗时    | 设定值      | 误差    | 结果   |\r\n");
  printf("+-------------+------------+------------+-------------+-------------+---------+--------+\r\n");
  fflush(stdout);

  // -------------------------- 1000us测试 --------------------------
  const uint16_t TEST_US = 1000U;
  Lock_Interrupts();
  TIM3->CNT = 0U;
  start_cnt = TIM3->CNT;
  while (TIM3->CNT < TEST_US);
  end_cnt = TIM3->CNT;
  Unlock_Interrupts();
  real_us = Calculate_Real_Time(start_cnt, end_cnt);
  error = (int32_t)real_us - (int32_t)TEST_US;
  // 固定每列宽度：测试项(11)、起点(10)、终点(10)、耗时(11)、设定值(11)、误差(7)、结果(6)
  printf("| %-11s | %-10" PRIu16 " | %-10" PRIu16 " | %-9" PRIu32 "us | %-9" PRIu16 "us | %-5" PRId32 "us | %-6s |\r\n",
         "1000us测试", start_cnt, end_cnt, real_us, TEST_US, error,
         (abs(error) <= 5) ? "达标" : "不达标");
  fflush(stdout);

  // -------------------------- 2ms测试 --------------------------
  const uint16_t TEST_MS = 2U;
  const uint32_t TEST_MS_US = (uint32_t)TEST_MS * 1000U;
  Lock_Interrupts();
  TIM3->CNT = 0U;
  start_cnt = TIM3->CNT;
  while (Calculate_Real_Time(start_cnt, TIM3->CNT) < TEST_MS_US);
  end_cnt = TIM3->CNT;
  Unlock_Interrupts();
  real_us = Calculate_Real_Time(start_cnt, end_cnt);
  error = (int32_t)real_us - (int32_t)TEST_MS_US;
  printf("| %-11s | %-10" PRIu16 " | %-10" PRIu16 " | %-9" PRIu32 "us | %-9" PRIu32 "us | %-5" PRId32 "us | %-6s |\r\n",
         "2ms测试", start_cnt, end_cnt, real_us, TEST_MS_US, error,
         (abs(error) <= 10) ? "达标" : "不达标");
  fflush(stdout);

  // -------------------------- 10ms测试 --------------------------
  const uint16_t TEST_10MS = 10U;
  const uint32_t TEST_10MS_US = (uint32_t)TEST_10MS * 1000U;
  Lock_Interrupts();
  TIM3->CNT = 0U;
  start_cnt = TIM3->CNT;
  while (Calculate_Real_Time(start_cnt, TIM3->CNT) < TEST_10MS_US);
  end_cnt = TIM3->CNT;
  Unlock_Interrupts();
  real_us = Calculate_Real_Time(start_cnt, end_cnt);
  error = (int32_t)real_us - (int32_t)TEST_10MS_US;
  printf("| %-11s | %-10" PRIu16 " | %-10" PRIu16 " | %-9" PRIu32 "us | %-9" PRIu32 "us | %-5" PRId32 "us | %-6s |\r\n",
         "10ms测试", start_cnt, end_cnt, real_us, TEST_10MS_US, error,
         (abs(error) <= 10) ? "达标" : "不达标");
  fflush(stdout);

  // -------------------------- 70ms测试 --------------------------
  const uint16_t TEST_70MS = 70U;
  const uint32_t TEST_70MS_US = (uint32_t)TEST_70MS * 1000U;
  Lock_Interrupts();
  start_cnt = TIM3->CNT;
  Unlock_Interrupts();
  real_us = Segment_Delay(TEST_70MS_US);
  Lock_Interrupts();
  end_cnt = TIM3->CNT;
  Unlock_Interrupts();
  error = (int32_t)real_us - (int32_t)TEST_70MS_US;
  printf("| %-11s | %-10" PRIu16 " | %-10" PRIu16 " | %-9" PRIu32 "us | %-9" PRIu32 "us | %-5" PRId32 "us | %-6s |\r\n",
         "70ms测试", start_cnt, end_cnt, real_us, TEST_70MS_US, error,
         (abs(error) <= 30) ? "达标" : "不达标");
  fflush(stdout);

  // -------------------------- 2S测试 --------------------------
  const uint16_t TEST_2S = 2U;
  const uint32_t TEST_2S_US = (uint32_t)TEST_2S * 1000000U;
  Lock_Interrupts();
  start_cnt = TIM3->CNT;
  Unlock_Interrupts();
  real_us = Segment_Delay(TEST_2S_US);
  Lock_Interrupts();
  end_cnt = TIM3->CNT;
  Unlock_Interrupts();
  error = (int32_t)real_us - (int32_t)TEST_2S_US;
  printf("| %-11s | %-10" PRIu16 " | %-10" PRIu16 " | %-9" PRIu32 "us | %-9" PRIu32 "us | %-5" PRId32 "us | %-6s |\r\n",
         "2S测试", start_cnt, end_cnt, real_us, TEST_2S_US, error,
         (abs(error) <= 200) ? "达标" : "不达标");
  fflush(stdout);

  // 表尾
  printf("+-------------+------------+------------+-------------+-------------+---------+--------+\r\n");
  printf("【测试完成】\r\n");
  fflush(stdout);
}
