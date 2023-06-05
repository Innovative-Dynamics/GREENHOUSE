/*
 * dht11.c
 *
 *  Created on: Jun 5, 2023
 *      Author: envy0
 */

#include "dht11.h"

uint32_t dht11_time_prev = 0;
_Bool dht11_is_counting = 0;
_Bool dht11_init = 0;

void dht11_start()
{
	if (!dht11_is_counting)
	{
		GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
		GPIO_InitStructPrivate.Pin = DHT11_Pin;
		GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
		GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructPrivate); // set the pin as output
		HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 0);
		dht11_is_counting = 1;
		dht11_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
	}
	else
	{
		if (__HAL_TIM_GET_COUNTER(&htim2) - dht11_time_prev >= 20000)
		{
			HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 0);

			GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
			GPIO_InitStructPrivate.Pin = DHT11_Pin;
			GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
			GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
			GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructPrivate); // set the pin as input
			dht11_is_counting = 0;
			dht11_init = 1;
		}
	}


}
