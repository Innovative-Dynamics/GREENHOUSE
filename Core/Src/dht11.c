/*
 * dht11.c
 *
 *  Created on: Jun 5, 2023
 *      Author: Innovative Dynamics
 */

#include "dht11.h"

uint32_t startTime, endTime;

uint8_t DHT11_Read (void)
{
  uint8_t result = 0;

  // Reading procedure from datasheet
  for (uint8_t bitIndex = 0; bitIndex < 8; bitIndex++)
  {
	startTime = __HAL_TIM_GET_COUNTER(&htim2);
	endTime = __HAL_TIM_GET_COUNTER(&htim2);

	while (!(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) && startTime + 20000 > endTime)
	{
	  // Wait for the pin to go high
	  endTime = __HAL_TIM_GET_COUNTER(&htim2);
	}

	delay_us(40); // Wait for 40 us

	if (!(HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)))
	{
	  // If the pin is low
	  result &= ~(1 << (7 - bitIndex));
	}
	else
	{
	  result |= (1 << (7 - bitIndex));
	}

	startTime = __HAL_TIM_GET_COUNTER(&htim2);
	endTime = __HAL_TIM_GET_COUNTER(&htim2);

	while ((HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin)) && startTime + 20000 > endTime)
	{
	  // Wait for the pin to go low
	  endTime = __HAL_TIM_GET_COUNTER(&htim2);
	}
  }
  return result;
}

uint8_t DHT11_Start (void)
{
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = DHT11_Pin;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructPrivate); // Set the pin as output
  HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 0);   // Pull the pin low
  HAL_Delay(20);   // Wait for 20ms
  HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 1);   // Pull the pin high
  delay_us(30);   // Wait for 30us
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructPrivate); // Set the pin as input
  delay_us (40);
  if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))
  {
	  delay_us (80);
    if ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin))) Response = 1;
  }
  startTime = __HAL_TIM_GET_COUNTER(&htim2);
  endTime = __HAL_TIM_GET_COUNTER(&htim2);
  while ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)) && startTime + 20000 > endTime)
  {
    endTime = __HAL_TIM_GET_COUNTER(&htim2);
  }
  return Response;
}
