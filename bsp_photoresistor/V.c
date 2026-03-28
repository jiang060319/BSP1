#include "V.h"

/**
 * @brief 
 * @retval 
 */
float LightSensor_GetVoltage(void)
{
    uint16_t adc_val; 
    float voltage;

    if (HAL_ADC_Start(&hadc1) != HAL_OK)
    {
        return -1.0f;
    }

    if (HAL_ADC_PollForConversion(&hadc1, 10) != HAL_OK)
    {
        HAL_ADC_Stop(&hadc1);
        return -1.0f;
    }

    adc_val = HAL_ADC_GetValue(&hadc1);
    voltage = (adc_val * 3.3f) / 4095.0f;

    HAL_ADC_Stop(&hadc1);

    return voltage;
}
