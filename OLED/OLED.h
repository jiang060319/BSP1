#ifndef __OLED_H
#define __OLED_H

#include "stm32f1xx.h"

/* 软件/硬件IIC切换宏 0：软件 1：硬件 */
#define I2C_SELECT 1

#define OLED_ID          0x78
#define OLED_WR_CMD      0x00
#define OLED_WR_DATA     0x40

void OLED_Init(void);                                                                                         //OLED初始化
void OLED_Test(void);                                                                                         //OLED测试函数

void OLED_SetPos(unsigned char x, unsigned char y);                                                           //设置OLED显示的起始坐标
void OLED_Fill(unsigned char fill_data);                                                                      //OLED全屏填充函数
void OLED_CLS(void);                                                                                          //OLED全屏清除函数
void OLED_ON(void);                                                                                           //OLED开启函数
void OLED_OFF(void);                                                                                          //OLED关闭函数
void OLED_ShowStr(unsigned char x, unsigned char y, unsigned char ch[], unsigned char textsize);              //OLED显示字符串函数
void OLED_ShowCN(unsigned char x, unsigned char y, unsigned char n);                                          //OLED显示中文函数
void OLED_DrawBMP(unsigned char x0,unsigned char y0,unsigned char x1,unsigned char y1,unsigned char BMP[]);   //OLED显示图像函数

#endif
