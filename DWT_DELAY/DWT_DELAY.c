///************************************************************
//  * @brief   DWT_DELAY.c
//  * @note    DWT实现延时
//  ***********************************************************/

//#include "main.h"
//#include "DWT_DELAY.h" 
//#include "stdio.h"
//#include "string.h"
///**
//  * @brief  初始化DWT计数器
//  * @param  无
//  * @retval 无
//  * @note   使用延时函数前，必须调用本函数
//  */
//void DWT_Delay_Init(void)
//{
//    /* 使能DWT外设 */
//    DEMCR |= (uint32_t)DEMCR_TRCENA;
//    
//    /* DWT CYCCNT寄存器计数清0 */
//    DWT_CYCCNT = (uint32_t)0U;                      //使能CYCCNT寄存器之前，先清0
//    
//    /* 使能Cortex-M DWT CYCCNT寄存器 */
//    DWT_CTRL  |=(uint32_t)DWT_CTRL_CYCCNTENA;
//      printf("DWT_Delay启动正常\r");
//}


///**
//  * @brief  读取当前时间戳
//  * @param  无
//  * @retval 当前时间戳，即DWT_CYCCNT寄存器的值
//  */
//uint32_t DWT_GetTick(void)
//{ 
//    return ((uint32_t)DWT_CYCCNT);
//}

///**
//  * @brief  节拍数转化时间间隔(微妙单位)
//  * @param  tick :需要转换的节拍数
//  * @param  frequency :内核时钟频率
//  * @retval 当前时间戳(微妙单位)
//  */
//uint32_t DWT_TickToMicrosecond(uint32_t tick,uint32_t frequency)
//{ 
//    return (uint32_t)(1000000.0/frequency*tick);
//}


///**
//  * @brief  DWT计数实现精确延时，32位计数器
//  * @param  time : 延迟长度，单位1 us
//  * @retval 无
//  * @note   无
//  */
//void DWT_Delay_us(uint32_t us)
//{
//    /* 将微秒转化为对应的时钟计数值*/
//    uint32_t tick_duration= us * (SystemCoreClock / 1000000) ;
//    uint32_t tick_start = DWT_GetTick();                        /* 刚进入时的计数器值 */
//    
//    while(DWT_GetTick() - tick_start < tick_duration);
//}

///**
//  * @brief  DWT计数实现精确延时，32位计数器
//  * @param  time : 延迟长度，单位1 ms
//  * @retval 无
//  */
//void DWT_Delay_ms(uint32_t ms)
//{
//    for(uint32_t i = 0; i < ms; i++)
//    {
//        DWT_Delay_us(1000);
//    }
//}


///**
//  * @brief  DWT计数实现精确延时，32位计数器
//  * @param  time : 延迟长度，单位1 S
//  * @retval 无
//  */
//void DWT_Delay_S(uint32_t S)
//{
//    for(uint32_t i = 0; i < S; i++)
//    {
//        DWT_Delay_ms(1000);
//    }
//}

//void DWT_Delay_test()
//{
//  // 打印测试开始提示
//  printf("DWT_Delay_test测试\r\n");
//  printf("系统时钟：%u MHz\r\n", SystemCoreClock / 1000000);

//  uint8_t test_number = 10;
//  uint64_t begin_time = 0;
//  uint64_t end_time = 0;
//  uint64_t consum_conter = 0;
//  uint64_t consum_us =0;

//  begin_time = DWT_GetTick();
//  DWT_Delay_ms(test_number);
//  end_time = DWT_GetTick();

//  consum_conter = end_time - begin_time;
//  consum_us  = DWT_TickToMicrosecond(consum_conter,SystemCoreClock);

////  printf("测试值%d ms,实际耗时：%lu us\r\n\r\n",test_number,consum_us);
//}
