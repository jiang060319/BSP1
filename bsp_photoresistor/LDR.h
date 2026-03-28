#ifndef __LIGHT_CTRL_H
#define __LIGHT_CTRL_H

#include "main.h"

extern TIM_HandleTypeDef htim3;

#define LIGHT_BRIGHT_VOLT 0.5f  
#define LIGHT_DARK_VOLT   3.0f  
/**
 * @brief  
 */
void LightCtrl_Init(void);

/**
 * @brief  
 * @param  
 */
void LightCtrl_AdjustByVoltage(float voltage);

#endif
