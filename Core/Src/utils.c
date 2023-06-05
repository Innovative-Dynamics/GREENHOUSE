/*
 * utils.c
 *
 *  Created on: Jun 1, 2023
 *      Author: envy0
 */

#include "utils.h"

// Global variables to store DHT11 data
uint8_t dht11_data[5] = {0};

//int counter = 0;
//_Bool timer = 0;
//
//_Bool counting = 0;
//_Bool time_has_passed = 0;
//
//float acquired_value_ADC_volt = 0;
//_Bool water_reading = 0;
//
//_Bool lights_reading = 0;
//
//float water_value = 0;

//void reset_water_reading_done ()
//{
//	water_reading = 0;
//}
//
//_Bool water_reading_done ()
//{
//	return water_reading;
//}
//
//float get_water_value ()
//{
//	return water_value;
//}

//void reset_lights_reading_done ()
//{
//	lights_reading = 0;
//}

//_Bool lights_reading_done ()
//{
//	return lights_reading;
//}

//float get_water_grow_tank_level ()
//{
//	return acquired_value_ADC_volt;
//}

//float get_lights_voltage_value ()
//{
//	return voltage_value;
//}
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == ECHO_Pin)
	{
		echo_callback();
	}
	else if (GPIO_Pin == DHT11_Pin)
	{
		  static uint8_t bits[40];
		  static uint8_t bit_count = 0;
		  static uint8_t byte_count = 0;
		  static uint8_t data_ready = 0;

		  // Read the GPIO pin state
		  uint8_t bit = (HAL_GPIO_ReadPin(DHT11_GPIO_PORT, DHT11_DATA_PIN) == GPIO_PIN_SET) ? 1 : 0;

		  if (bit_count < 8)
		  {
		    // Store the received bit
		    bits[byte_count] <<= 1;
		    bits[byte_count] |= bit;
		    bit_count++;
		  }
		  else
		  {
		    // Move to the next byte
		    byte_count++;
		    bit_count = 0;
		  }

		  if (byte_count == 5)
		  {
		    // Copy received data to the global variable
		    for (int i = 0; i < 5; i++)
		    {
		      dht11_data[i] = bits[i];
		    }

		    // Signal that data is ready
		    data_ready = 1;
		    byte_count = 0;
		    bit_count = 0;
		  }

	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
//	if (hadc == &hadc3)
//	{
//		// ECPH PROCEDURE
//		HAL_ADC_Stop_IT(hadc);
//
//		if (read_ec())
//		{
//			float adEC_val = HAL_ADC_GetValue(hadc);
//			set_adEC(adEC_val);
//		}
//		else if (read_ph())
//		{
//			float adPH_val = HAL_ADC_GetValue(hadc);
//			set_adPH(adPH_val);
//		}
//
//	}
//	else if (hadc == &hadc1)
//	{
//		// WATER LEVEL PROCEDURE
//		HAL_ADC_Stop_IT(hadc);
//		uint32_t acquired_value_ADC_count = 0;
//		acquired_value_ADC_count = HAL_ADC_GetValue(hadc);
//
//		acquired_value_ADC_volt = (float) acquired_value_ADC_count / 4095 * 3.3;
//
//		water_reading = 1;
//
//	}
	HAL_ADC_Stop_IT(hadc);

	if (hadc == &hadc2)
	{
		// LIGHTS PROCEDURE
		// READ FROM THE PHOTORESISTENCE
		HAL_ADC_Stop_IT(hadc);
		voltage_value = HAL_ADC_GetValue(hadc);
	}
	else if (hadc == &hadc1)
	{
		water_level_gt = HAL_ADC_GetValue(hadc);
//		water_reading = 1;
	}
}
