/*
 * virtual_main.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#include "virtual_main.h"

_Bool enough_water = 0;
_Bool water_ready = 0;

_Bool ecph_initialization_called = 0;
_Bool ecph_ready = 0;

_Bool is_default_check = 1; // TRUE == 1; FALSE == 0

_Bool mixing = 0;

_Bool loading = 0;
_Bool unloading = 0;

_Bool reading_value = 0;
_Bool is_reading_adc = 0;

_Bool is_counting = 0;

_Bool dht11_initialization = 0;

long long int time_prev = 0;
long long int water_time_prev = 0;

float temp = 0;
float humidity = 0;

char data[8];


// LIGHTS PROCEDURE WORKING DONT TOUCH
machine state = ECPH_PROCEDURE;
uint16_t water_tank_level;


// IMPLEMENTARE UN TIMER GLOBALE!


void virtual_main()
{
	switch(state)
	{
//	case(WATER_STATUS_CHECK):
//
//		// CHECK THE WATER LEVEL OF THE GROW TANK
//		if (!is_reading_adc)
//		{
//			HAL_ADC_Start_IT(&hadc1);
//			is_reading_adc = 1;
//		}
//		else if (water_reading_done())
//		{
//			is_reading_adc = 0;
//			reset_water_reading_done();
//
//
//
//			if (get_water_grow_tank_level() < ROOT_LEVEL)
//			{
//				if (!is_counting)
//				{
//					water_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//					is_counting = 1;
//				}
//				else
//				{
//					// 30 MINUTES COUNTER
//					if (__HAL_TIM_GET_COUNTER(&htim2) - water_time_prev >= 1.8e+9)
//					{
//						// WATER LEVEL CHECK TO AVOID EMPTY PUMP RUNS
//						if (!get_water_read())
//						{
//							water_tank_level = read_water_level();
//						}
//						else if ((water_tank_level >= MIN_WATER_LVL) & (water_tank_level <= MAX_WATER_LVL))
//						{
//							// WATER LOAD
//							if (!water_ready)
//							{
//								// NUETRIENTS CHECK
//								state = ECPH_PROCEDURE;
//								break;
//							}
//							else
//							{
//								// IF WATER READY START THE LOADING PROCEDURE
//								state = WATER_LOAD_PROCEDURE;
//								is_counting = 0;
//								break;
//							}
//						}
//						else
//						{
//							// ALARM NOT ENOUGH WATER
//						}
//					}
//				}
//			}
//			else
//			{
//				// WATER UNLOAD
//				if (!is_counting)
//				{
//					water_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//					is_counting = 1;
//				}
//				else
//				{
//					if (__HAL_TIM_GET_COUNTER(&htim2) - water_time_prev >= 3e+8)
//					{
//						state = WATER_UNLOAD_PROCEDURE;
//						is_counting = 0;
//						break;
//					}
//
//				}
//			}
//		}
//
//	case(WATER_LOAD_PROCEDURE):
//
//			if (!loading)
//			{
//				HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_SET);
//				loading = 1;
//			}
//			else
//			{
//				if (!is_reading_adc)
//				{
//					HAL_ADC_Start_IT(&hadc1);
//					is_reading_adc = 1;
//				}
//				else if (water_reading_done())
//				{
//					// WAIT FOR THE ADC TO FINISH READING THE VALUE
//					is_reading_adc = 0;
//					reset_water_reading_done();
//
//					if (get_water_grow_tank_level() >= ROOT_LEVEL)
//					{
//						// STOP THE PUMP
//						HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_RESET);
//						loading = 0;
//
//						// AFTER LOAD COMPLETED THEN THE WATER ISNT READY ANYMORE
//						water_ready = 0;
//						state = WATER_STATUS_CHECK;
//						break;
//					}
//				}
//			}
//
//	case(WATER_UNLOAD_PROCEDURE):
//
//			if (!unloading)
//			{
//				// OPEN THE ELECTROVALVE TO UNLOAD WATER
//				HAL_GPIO_WritePin(ELECTROVALVE_GPIO_Port, ELECTROVALVE_Pin, GPIO_PIN_SET);
//				unloading = 1;
//			}
//			else
//			{
//				if (!is_reading_adc)
//				{
//					HAL_ADC_Start_IT(&hadc1);
//					is_reading_adc = 1;
//				}
//				else if (water_reading_done())
//				{
//					is_reading_adc = 0;
//					reset_water_reading_done();
//
//					if (get_water_grow_tank_level() < ROOT_LEVEL)
//					{
//						HAL_GPIO_WritePin(ELECTROVALVE_GPIO_Port, ELECTROVALVE_Pin, GPIO_PIN_RESET);
//						unloading = 0;
//						break;
//					}
//				}
//			}
//
	case(ECPH_PROCEDURE):

			if (!is_ec_initialized())
			{
				ec_init();
			}
			// CHECK FOR 5 SECONDS
			else
			{
				if (!is_ec_value_readed())
				{
					ec_read(&hadc3);
				}
				else
				{
					if (!is_counting)
					{
						is_counting = 1;
						time_prev = __HAL_TIM_GET_COUNTER(&htim2);
					}
					else
					{
						if (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 2000000)
						{
							char data2[32];
							sprintf(data2, "EC: %f \n\r", get_EC());

							HAL_UART_Transmit(&huart2, (uint8_t*)data2, strlen(data2), HAL_MAX_DELAY);

							if (!is_ph_value_readed())
							{
								ph_read(&hadc3);
							}
							else
							{
								char data1[32];
								sprintf(data1, "PH: %f \n\r", get_PH());

								HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

								reset_ec_initialized();

								reset_is_ec_value_readed();
								reset_is_ph_value_readed();
								is_counting = 0;
							}

						}
					}
				}
			}
			break;
//
//				else if (get_EC() < EC_SETPOINT)
//				{
//					// DA FARE CONTROLLO SUL PH
//					state = MIX_PROCEDURE;
//					mixing = 0;
//					break;
//				}
//				else
//				{
//					state = WATER_STATUS_CHECK;
//					water_ready = 1;
//					break;
//				}
//			}

//	case (MIX_PROCEDURE):
//
//			if (!is_counting)
//			{
//				stepper_step_angle(360, 0, 10);
//				HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_SET);
//				is_counting = 1;
//				time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//			}
//			else
//			{
//				if (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 7000000)
//				{
//					HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);
//					state = WATER_STATUS_CHECK;
//					water_ready = 1;
//					break;
//				}
//			}
//
//	case(DHT11_PROCEDURE):
//
//			if (!dht11_procedure_done())
//			{
//				DHT11();
//			}
//			else
//			{
//				temp = getTemp();
//				humidity = getRH();
//				break;
//			}
//
//	case(DATASEND_PROCEDURE):
//			break;
	case(LIGHTS_PROCEDURE):
			// STATUS = 1 - LIGHTS ON
			// STATUS = 0 - LIGHTS OFF
			setLight(1);
			break;
	default:
//		// ELECTROVALVE RESET
//		HAL_GPIO_WritePin(ELECTROVALVE_GPIO_Port, ELECTROVALVE_Pin, GPIO_PIN_RESET);
//
//		// WATER PUMP RESET
//		HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_RESET);
//
//		// MIX PUMP RESET
//		HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);
//
//		// CHECK WATER STATUS
//		is_default_check = 1;
//		state = ECPH_PROCEDURE;

		// LIGHTS OFF
		setLight(0);
		break;
	}
}
