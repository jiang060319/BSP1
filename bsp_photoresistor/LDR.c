#include "LDR.h"
#include "LED.h"
void LightCtrl_Init(void)
{
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, 0);
}

void LightCtrl_AdjustByVoltage(float voltage)
{
  uint16_t pwm_duty;
  uint16_t pwm_max = htim3.Init.Period;  

  if (voltage < 0)
    return;

  if (voltage <= LIGHT_BRIGHT_VOLT)
  {
    pwm_duty = 0;
  }
  else if (voltage >= LIGHT_DARK_VOLT)
  {
    pwm_duty = pwm_max;
  }
  else
  {
    pwm_duty = (uint16_t)((voltage - LIGHT_BRIGHT_VOLT) 
                         / (LIGHT_DARK_VOLT - LIGHT_BRIGHT_VOLT) 
                         * pwm_max);
  }

  __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, pwm_duty);
}

