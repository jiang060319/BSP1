#ifndef __LIGHT_SENSOR_H
#define __LIGHT_SENSOR_H

#include "stm32f1xx_hal.h"


extern ADC_HandleTypeDef hadc1;

float LightSensor_GetVoltage(void);

#endif
