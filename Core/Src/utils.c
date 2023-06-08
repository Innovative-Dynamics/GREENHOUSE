/*
 * utils.c
 *
 *  Created on: Jun 1, 2023
 *      Author: envy0
 */

#include "utils.h"

void delay_us (uint32_t us)
{
	uint32_t actual = __HAL_TIM_GET_COUNTER(&htim2);
	while(__HAL_TIM_GET_COUNTER(&htim2) - actual <= us);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ECHO_Pin)
	{
		echo_callback();
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
	HAL_ADC_Stop_IT(hadc);

	if (hadc == &hadc2)
	{
		// LIGHTS PROCEDURE
		// READ FROM THE PHOTORESISTENCE
		voltage_value = HAL_ADC_GetValue(hadc);
	}
	else if (hadc == &hadc1)
	{
		water_level_gt = HAL_ADC_GetValue(hadc);
	}
}
