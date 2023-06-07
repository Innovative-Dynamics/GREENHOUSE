/*
 * dht11.c
 *
 *  Created on: Jun 5, 2023
 *      Author: envy0
 */

#include "dht11.h"

//uint8_t RHI, RHD, TCI, TCD, SUM;
uint32_t pMillis, cMillis;
//float tCelsius = 0;
//float tFahrenheit = 0;
//float RH = 0;

uint8_t DHT11_Read (void)
{
  uint8_t a,b;
  for (a=0;a<8;a++)
  {
    pMillis = __HAL_TIM_GET_COUNTER(&htim2);
    cMillis = __HAL_TIM_GET_COUNTER(&htim2);
    while (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)) && pMillis + 20000 > cMillis)
    {  // wait for the pin to go high
      cMillis = __HAL_TIM_GET_COUNTER(&htim2);
    }
    delay_us (40);   // wait for 40 us
    if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))   // if the pin is low
      b&= ~(1<<(7-a));
    else
      b|= (1<<(7-a));
    pMillis = __HAL_TIM_GET_COUNTER(&htim2);
    cMillis = __HAL_TIM_GET_COUNTER(&htim2);
    while ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)) && pMillis + 20000 > cMillis)
    {  // wait for the pin to go low
      cMillis = __HAL_TIM_GET_COUNTER(&htim2);
    }
  }
  return b;
}

uint8_t DHT11_Start (void)
{
  uint8_t Response = 0;
  GPIO_InitTypeDef GPIO_InitStructPrivate = {0};
  GPIO_InitStructPrivate.Pin = DHT11_Pin;
  GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructPrivate); // set the pin as output
  HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 0);   // pull the pin low
  HAL_Delay(20);   // wait for 20ms
  HAL_GPIO_WritePin (DHT11_GPIO_Port, DHT11_Pin, 1);   // pull the pin high
  delay_us(30);   // wait for 30us
  GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructPrivate); // set the pin as input
  delay_us (40);
  if (!(HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)))
  {
	  delay_us (80);
    if ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin))) Response = 1;
  }
  pMillis = __HAL_TIM_GET_COUNTER(&htim2);
  cMillis = __HAL_TIM_GET_COUNTER(&htim2);
  while ((HAL_GPIO_ReadPin (DHT11_GPIO_Port, DHT11_Pin)) && pMillis + 20000 > cMillis)
  {
    cMillis = __HAL_TIM_GET_COUNTER(&htim2);
  }
  return Response;
}
