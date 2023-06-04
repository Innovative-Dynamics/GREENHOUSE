/*
 * ultrasonic.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#include "ultrasonic.h"

//uint32_t pMillis;
uint32_t Value1 = 0;
uint32_t Value2 = 0;
uint16_t Distance  = 0;
uint16_t water_level = 0;

_Bool trig_ready = 0;
_Bool value1_read = 0;
_Bool value2_read = 0;
_Bool water_level_readed = 0;

uint32_t ultrasonic_time_prev;
_Bool ecph_is_counting = 0;

//uint16_t read_water_level ()
//{
//	// Read the water level first to avoid empty runs of the pump
//	if (!trig_ready)
//	{
//		HAL_GPIO_WritePin(ULTRAS__TRIG_GPIO_Port, ULTRAS__TRIG_Pin, GPIO_PIN_SET);  // pull the TRIG pin HIGH
//
//		if (!is_counting)
//		{
//			time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//			is_counting = 1;
//		}
//		else if (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 10)
//		{
//			HAL_GPIO_WritePin(ULTRAS__TRIG_GPIO_Port, ULTRAS__TRIG_Pin, GPIO_PIN_RESET);  // pull the TRIG pin low
//			is_counting = 0;
//			trig_ready = 1;
//		}
//	}
//	else if (!value1_read)
//	{
//		if (!is_counting)
//		{
//			time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//			is_counting = 1;
//		}
//		else if (!(HAL_GPIO_ReadPin(ULTRAS__ECHO_GPIO_Port, ULTRAS__ECHO_Pin)) && (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 10))
//		{
//			Value1 = __HAL_TIM_GET_COUNTER (&htim2);
//			is_counting = 0;
//			value1_read = 1;
//		}
//	}
//	else if (!value2_read)
//	{
//		if (!is_counting)
//		{
//			time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//			is_counting = 1;
//		}
//		else if ((HAL_GPIO_ReadPin(ULTRAS__ECHO_GPIO_Port, ULTRAS__ECHO_Pin)) && (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 50))
//		{
//			Value2 = __HAL_TIM_GET_COUNTER (&htim2);
//			is_counting = 0;
//			value1_read = 1;
//		}
//	}
//	else
//	{
//		Distance = (Value2 - Value1) * 0.034/2;
//
//		water_level = 25 - Distance;
//
//		return water_level;
//	}
//}

void trigger_measurement()
{
    HAL_GPIO_WritePin(ULTRAS__TRIG_GPIO_Port, ULTRAS__TRIG_Pin, GPIO_PIN_SET);
}

void reset_trigger()
{
    HAL_GPIO_WritePin(ULTRAS__TRIG_GPIO_Port, ULTRAS__TRIG_Pin, GPIO_PIN_RESET);
}

void start_counting()
{
	ultrasonic_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
	ecph_is_counting = 1;
}

_Bool check_counting_elapsed(uint32_t elapsed_time)
{
    return (__HAL_TIM_GET_COUNTER(&htim2) - ultrasonic_time_prev >= elapsed_time);
}

void read_value1()
{
    Value1 = __HAL_TIM_GET_COUNTER(&htim2);
    ecph_is_counting = 0;
    value1_read = 1;
}

void read_value2()
{
    Value2 = __HAL_TIM_GET_COUNTER(&htim2);
    ecph_is_counting = 0;
    value2_read = 1;
}

void read_water_level()
{
    if (!trig_ready)
    {
        trigger_measurement();
        if (!ecph_is_counting)
        {
            start_counting();
        }
        else if (check_counting_elapsed(10))
        {
            reset_trigger();
            ecph_is_counting = 0;
            trig_ready = 1;
        }
    }
    else if (!value1_read)
    {
        if (!ecph_is_counting)
        {
            start_counting();
        }
        else if (!HAL_GPIO_ReadPin(ULTRAS__ECHO_GPIO_Port, ULTRAS__ECHO_Pin) && check_counting_elapsed(10))
        {
            read_value1();
        }
    }
    else if (!value2_read)
    {
        if (!ecph_is_counting)
        {
            start_counting();
        }
        else if (HAL_GPIO_ReadPin(ULTRAS__ECHO_GPIO_Port, ULTRAS__ECHO_Pin) && check_counting_elapsed(50))
        {
            read_value2();
        }
    }
    else
    {
        Distance = (Value2 - Value1) * 0.034 / 2;
        water_level = 25 - Distance;
        water_level_readed = 1;
    }
}
