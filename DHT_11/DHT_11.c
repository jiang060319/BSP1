/*******************************************************************************
  * @file       dht_11.c
  * @brief      DHT11温湿度传感器驱动
  * @note       核心微秒延时用自定义函数，毫秒延时用HAL_Delay，增强稳定性
  ******************************************************************************
  */

#include "main.h"
#include "stdio.h"
#include "stm32f1xx_hal.h"

#include "DHT_11.h"

#include "TIM_DELAY.h"
//#include "./BSP_DELAY/DWT_DELAY.h"
//#include "./BSP_DELAY/SYSTICK_DELAY.h"
//#include "./BSP_DELAY/NOP_DELAY.h"

static DHT11_DATA_TYPEDEF dht11_data = {0};

// DHT11关键时序宏定义
#define DHT11_POWER_STAB_MS   1000U         // 上电稳定延时（HAL_Delay实现）
#define DHT11_START_LOW_MS    20U           // 起始信号低电平（HAL_Delay实现）
#define DHT11_READ_TIMEOUT_US 100U          // 微秒级超时阈值（TIM_Delay_us实现）

/**
 * @brief  DHT11 DATA 引脚 IO 初始化（增加上电稳定延时，用HAL_Delay）
 * @retval 无
 */
void DHT11_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* 使能GPIOB端口时钟 */
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /* 设置引脚为推挽输出 */
    GPIO_InitStruct.Pin = DHT11_DATA_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;                 // 推挽输出
    GPIO_InitStruct.Pull = GPIO_NOPULL;                         // 不上拉不下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                // 低速输出
    HAL_GPIO_Init(DHT11_DATA_GPIO_Port, &GPIO_InitStruct);

    /* 初始化DHT11数据引脚，拉高总线 */
    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET);
    HAL_Delay(DHT11_POWER_STAB_MS);       // 其余延时用HAL_Delay：传感器上电稳定1秒
    printf("DHT11传感器启动正常\r\n");
    fflush(stdout);
}

/**
 * @brief  配置 DHT11 数据引脚的工作模式
 * @param  mode 引脚模式，如输入、输出等，使用 HAL 库中的 GPIO_MODE_XXX 宏
 * @param  pull 上下拉配置，使用 HAL 库中的 GPIO_PULL_XXX 宏
 */
void DHT11_SetGPIOMode(uint32_t mode, uint32_t pull)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin = DHT11_DATA_Pin;                       // 选择DHT11数据引脚
    GPIO_InitStruct.Mode = mode;                                // 设置引脚模式
    GPIO_InitStruct.Pull = pull;                                // 设置上拉或下拉
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;                // 设置引脚速度
    HAL_GPIO_Init(DHT11_DATA_GPIO_Port, &GPIO_InitStruct);      // 初始化GPIO
}

/**
 * @brief  读取DHT11一个字节数据（核心40us延时，确保时序精准）
 * @retval 返回8位数据（1字节）
 */
uint8_t DHT11_ReadByte(void)
{
    uint8_t value = 0;

    for (uint8_t i = 0; i < 8; i++)
    {
        value |= (1 << (7 - i));  // 先置位当前数据位

        // 等待信号线变高，表示开始传输第i位数据
        while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_RESET);

        TIM_Delay_us(40);  // 核心微秒延时

        // 如果40us后线仍然为高，表示该位为1；否则清除该位（为0）
        if (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_RESET)
        {
            value &= ~(1 << (7 - i));
        }
        else
        {
            // 等待信号线变低，准备接收下一位
            while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_SET);
        }
    }
    return value;
}

/**
 * @brief  读取DHT11传感器数据（us级延时用自定义函数，毫秒级用HAL_Delay）
 * @param  data 指向存放温湿度数据的结构体指针
 * @retval HAL_OK 成功，HAL_ERROR 失败
 */
HAL_StatusTypeDef DHT11_ReadData(DHT11_DATA_TYPEDEF *data)
{
    uint8_t retry = 0;

    // 容错判断：结构体指针不为空
    if (data == NULL)
        return HAL_ERROR;

    // 1. 主机拉低总线，发送起始信号
    DHT11_SetGPIOMode(GPIO_MODE_OUTPUT_PP, GPIO_NOPULL);     // 设置为推挽输出
    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_RESET);
    HAL_Delay(DHT11_START_LOW_MS);  // 毫秒延时用HAL_Delay：拉低20ms
    HAL_GPIO_WritePin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin, GPIO_PIN_SET);
    TIM_Delay_us(13);               // 核心微秒延时：用TIM3实现13us

    // 2. 设置引脚为输入，等待DHT11响应信号
    DHT11_SetGPIOMode(GPIO_MODE_INPUT, GPIO_PULLUP);

    // 3. 等待DHT11拉低响应信号
    retry = 0;
    while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_SET)
    {
        if (++retry > DHT11_READ_TIMEOUT_US)
            return HAL_ERROR;    // 超时无响应，读取失败
        TIM_Delay_us(1);
    }

    // 等待DHT11拉高信号
    retry = 0;
    while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_RESET)
    {
        if (++retry > DHT11_READ_TIMEOUT_US)
            return HAL_ERROR;    // 超时，读取失败
        TIM_Delay_us(1);
    }

    // 等待DHT11再次拉低信号
    retry = 0;
    while (HAL_GPIO_ReadPin(DHT11_DATA_GPIO_Port, DHT11_DATA_Pin) == GPIO_PIN_SET)
    {
        if (++retry > DHT11_READ_TIMEOUT_US)
            return HAL_ERROR;    // 超时，读取失败
        TIM_Delay_us(1);
    }

    // 4. 读取5字节数据
    data->humi_int   = DHT11_ReadByte();
    data->humi_deci  = DHT11_ReadByte();
    data->temp_int   = DHT11_ReadByte();
    data->temp_deci  = DHT11_ReadByte();
    data->check_sum  = DHT11_ReadByte();

    // 5. 校验数据是否正确
    uint8_t sum = data->humi_int + data->humi_deci + data->temp_int + data->temp_deci;
    return (sum == data->check_sum) ? HAL_OK : HAL_ERROR;
}

/**
 * @brief  读取DHT11数据并串口打印（优化输出格式，增加容错提示）
 * @retval 无
 */
void Dht11_ReadAndPrint(void)
{
    printf("-----------------------------------------\r\n");
    printf("开始读取DHT11数据...\r\n");
    TIM_Delay_us(100);  // 增加延时，确保内容刷新

    if(DHT11_ReadData(&dht11_data) == HAL_OK)
    {
        printf("DHT11传感器数据正常\r\n");
        TIM_Delay_us(500);
        
        printf("当前湿度： %2d.%1d %%RH\r\n", dht11_data.humi_int, dht11_data.humi_deci);
        TIM_Delay_us(500);
        
        printf("当前温度： %2d.%1d 摄氏度\r\n", dht11_data.temp_int, dht11_data.temp_deci);
    }
    else
    {
        printf("读取DHT11数据错误！\r\n");
    }
    printf("-----------------------------------------\r\n");
    fflush(stdout);
}

