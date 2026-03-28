
 #ifndef __OLED_I2C_H
 #define __OLED_I2C_H

 #include "stm32f1xx.h"
 #include "stdio.h"

 /* I2C引脚宏定义　
    PB6     ------> I2C1_SCL
    PB7     ------> I2C1_SDA
  * 自定义引脚 详见STM32官方数据手册
  */
#define OLED_I2C_NUM                  I2C1
#define OLED_I2C_GPIO_CLK_ENABLE      __HAL_RCC_GPIOB_CLK_ENABLE
#define OLED_I2C_CLK_ENABLE           __HAL_RCC_I2C1_CLK_ENABLE
#define OLED_I2C_GPIO_PORT            GPIOB
#define OLED_I2C_SDA_GPIO_PIN         GPIO_PIN_7
#define OLED_I2C_SCL_GPIO_PIN         GPIO_PIN_6


 /* 定义读写SCL和SDA的宏，已增加代码的可移植性和可阅读性 */

 #if 0
     #define OLED_I2C_SDA_0    HAL_GPIO_WritePin(OLED_I2C_GPIO_PORT, OLED_I2C_SDA_GPIO_PIN, GPIO_PIN_RESET)
     #define OLED_I2C_SDA_1    HAL_GPIO_WritePin(OLED_I2C_GPIO_PORT, OLED_I2C_SDA_GPIO_PIN, GPIO_PIN_SET)
     #define OLED_I2C_SCL_0    HAL_GPIO_WritePin(OLED_I2C_GPIO_PORT, OLED_I2C_SCL_GPIO_PIN, GPIO_PIN_RESET)
     #define OLED_I2C_SCL_1    HAL_GPIO_WritePin(OLED_I2C_GPIO_PORT, OLED_I2C_SCL_GPIO_PIN, GPIO_PIN_SET)
    
     #define OLED_I2C_SDA_READ HAL_GPIO_ReadPin(OLED_I2C_GPIO_PORT, OLED_I2C_SDA_GPIO_PIN)

 /* 这个分支选择直接寄存器操作实现IO读写 
  * 注意：如下写法，在IAR最高级别优化时，会被编译器错误优化
  */
 #else
     #define OLED_I2C_SDA_0 OLED_I2C_GPIO_PORT->BSRR = OLED_I2C_SDA_GPIO_PIN << 16u     /* SDA = 0 */
     #define OLED_I2C_SDA_1 OLED_I2C_GPIO_PORT->BSRR = OLED_I2C_SDA_GPIO_PIN            /* SDA = 1 */
     #define OLED_I2C_SCL_0 OLED_I2C_GPIO_PORT->BSRR = OLED_I2C_SCL_GPIO_PIN << 16u     /* SCL = 0 */
     #define OLED_I2C_SCL_1 OLED_I2C_GPIO_PORT->BSRR = OLED_I2C_SCL_GPIO_PIN            /* SCL = 1 */
    
     #define OLED_I2C_SDA_READ (OLED_I2C_GPIO_PORT->IDR & OLED_I2C_SDA_GPIO_PIN) >> 11
    
 #endif

 void OLED_I2C_GPIO_Config(void);                 //I2C初始化
 void OLED_I2C_Start(void);                       //启动I2C总线
 void OLED_I2C_Stop(void);                        //关闭I2C总线
 void OLED_I2C_ACK(uint8_t ack);                  //发起I2C总线响应
 uint8_t OLED_I2C_Wait_ACK(void);                 //等待I2C总线响应
 void OLED_I2C_SendByte(uint8_t byte);            //通过I2C总线发送数据
 uint8_t OLED_I2C_ReciveByte(void);               //接收I2C总线数据

 #endif
