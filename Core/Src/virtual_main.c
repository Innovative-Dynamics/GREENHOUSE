/*
 * virtual_main.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#include "virtual_main.h"

_Bool enough_water = 0;
_Bool is_water_ready = 0;

//_Bool ecph_initialization_called = 0;
//_Bool ecph_ready = 0;

volatile uint32_t water_level_gt = ROOT_LEVEL;

_Bool is_default_check = 1; // TRUE == 1; FALSE == 0

_Bool loading = 0;
_Bool unloading = 0;

//_Bool reading_value = 0;
//_Bool is_reading_adc = 0;

//_Bool is_counting = 0;

_Bool dht11_initialization = 0;
_Bool water_status_initialized = 0;

//uint32_t time_prev = 0;
//long long int water_time_prev = 0;

float temp = 0;
float humidity = 0;

//float EC_Value = 0;
//float PH_Value = 0;

// LIGHTS PROCEDURE WORKING DONT TOUCH
// ECPH PROCEDURE WORKING DONT TOUCH
// MIX PROCEDURE WORKING DONT TOUCH
// WATER LOAD PROCEDURE WORKING DONT TOUCH
// WATER UNLOAD PROCEDURE WORKING DONT TOUCH
// WATER READING SENSOR WORKING DONT TOUCH

machine state = TEST;
//uint16_t water_tank_level;

// GLOBAL TIMERS
uint32_t main_time_prev;
uint32_t water_proc_time_prev;
uint32_t lights_proc_time_prev;
_Bool is_water_counting = 0;
_Bool is_lights_counting = 0;
_Bool counting_to_load = 1; // Default cycle
_Bool counting_to_unload = 0;

_Bool lights_off = 0;


void virtual_main()
{
	switch(state)
	{
	case INIT:
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);


		break;
	case HOME:
		/*
		 * WATER CYCLE: After 5m. LOAD, wait 2m. UNLOAD
		 */

		// 5 Minutes: 3e+8 Microseconds
		if (counting_to_load && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 3e+8)
		{
			// Check if the pump is submerged

			if (water_level >= MIN_WATER_LVL && water_level <= MAX_WATER_LVL)
			{
				// WATER LOAD PROCEDURE
				if (is_water_ready)
				{
					state = WATER_LOAD_PROCEDURE;
				}
				else
				{
					state = ECPH_PROCEDURE;
				}
			}
			else
			{
				// ALARM
			}
		}
		// 2 Minutes: 1.2e+8 Microseconds
		else if (counting_to_unload && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 1.2e+8)
		{
			state = WATER_UNLOAD_PROCEDURE;
		}

		/*
		 * LIGHTS CYCLE: 5 Minutes Switch ON/OFF
		 */

		if (__HAL_TIM_GET_COUNTER(&htim2) - lights_proc_time_prev >= 1.8e+9)
		{
			if (!lights_off)
			{
				lights_off = 1;
				lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
			else
			{
				lights_off = 0;
				setLight(0);
				lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
		}

		if (!lights_off)
			setLight(1);

		/*
		 * DHT11
		 */




		/*
		 * SEND DATA
		 */
	case TEST:

//		echo_hc();
		if (!dht11_init)
		{
			dht11_start();
		}

		char data4[32];
		sprintf(data4, "Water Level: %f \n\r", water_level);
		HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);

//		if (!ultrasonic_init)
//		{
//			read_water_level();
//			ultrasonic_init = 1;
//		}
//		else
//		{
//			if (water_level_readed)
//			{
//				char data4[32];
//				sprintf(data4, "Water Level: %i \n\r", water_level);
//				HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);
//				ultrasonic_init = 0;
//				water_level_readed = 0;
//			}
//		}
		break;

	case WATER_LOAD_PROCEDURE:

//			if (!loading)
//			{
//				HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_SET);
//				loading = 1;
//			}
//			else
//			{
				HAL_ADC_Start_IT(&hadc1);

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

//					char data1[32];
//					sprintf(data1, "WATER LEVEL: %f \n\r", get_water_value());
//
//					HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

				if (water_level_gt >= 120)
				{
					// STOP THE PUMP
					HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_RESET);

					// UPDATE THE VALUES
					is_water_ready = 0;
					counting_to_load = 0;
					counting_to_unload = 1;
					water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//					loading = 0;

					state = HOME;
				}
				else
				{
					HAL_GPIO_WritePin(WATER_PUMP_GPIO_Port, WATER_PUMP_Pin, GPIO_PIN_SET);
				}
//			}
			break;

	case(WATER_UNLOAD_PROCEDURE):

//			if (!unloading)
//			{
				// OPEN THE ELECTROVALVE TO UNLOAD WATER
//				unloading = 1;
//			}
//			else
//			{
				HAL_ADC_Start_IT(&hadc1);

				char data2[32];
				sprintf(data2, "Water Level GT: %lui \n\r", water_level_gt);
				HAL_UART_Transmit(&huart2, (uint8_t*)data2, strlen(data2), HAL_MAX_DELAY);

//				if (!is_reading_adc)
//				{
//					HAL_ADC_Start_IT(&hadc1);
//					is_reading_adc = 1;
//				}
//				else if (water_reading_done())
//				{
//					is_reading_adc = 0;
//					reset_water_reading_done();

					if (water_level_gt < 80)
					{
						HAL_GPIO_WritePin(ELECTROVALVE_GPIO_Port, ELECTROVALVE_Pin, GPIO_PIN_RESET);
//
						// UPDATE THE VALUES
						unloading = 0;
						counting_to_load = 1;
						counting_to_unload = 0;
						water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
					}
					else
					{
						HAL_GPIO_WritePin(ELECTROVALVE_GPIO_Port, ELECTROVALVE_Pin, GPIO_PIN_SET);
					}
					break;
//				}
//			}
//
	case(ECPH_PROCEDURE):

			if (!ec_initialized)
			{
				ec_init();
			}
			// CHECK FOR 5 SECONDS
			else
			{
//				if (!is_ec_value_readed())
//				{
					ec_read(&hadc3);
//				}
//				else
//				{

//					EC_Value = get_EC();
//					EC_Value = 0.5;

//					if (!is_counting)
//					{
//						is_counting = 1;
//						time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//					}
//					else
//					{
//						if (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 2000000)
//						{
//							if (!is_ph_value_readed())
//							{
								ph_read(&hadc3);
//							}
//							else
//							{
//								PH_Value = get_PH();

								if (EC < EC_SETPOINT)
								{
									state = MIX_PROCEDURE;
								}
								else
								{
									is_water_ready = 1;
								}

								char data2[32];
								sprintf(data2, "EC: %f \n\r", EC);

								HAL_UART_Transmit(&huart2, (uint8_t*)data2, strlen(data2), HAL_MAX_DELAY);

								char data1[32];
								sprintf(data1, "PH: %f \n\r", PH);

								HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

//								reset_ec_initialized();
//								reset_is_ec_value_readed();
//								reset_is_ph_value_readed();
//								is_counting = 0;
								ec_initialized = 0;
							}

//						}
//					}
//				}
//			}
			break;

	case (MIX_PROCEDURE):
			if (!nutrs_deployed)
			{
				stepper_step_angle(360, 1, 10);

				char data1[32];
				sprintf(data1, "NUTRS DEPLOYED \n\r");
				HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

				HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_SET);
//				is_counting = 1;
				main_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
			else
			{
//				if (!is_counting)
//				{
//					char data1[32];
//					sprintf(data1, "NUTRS DEPLOYED \n\r");
//					HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);
//
//					HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_SET);
//					is_counting = 1;
//					time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//				}
//				else
//				{
					if (__HAL_TIM_GET_COUNTER(&htim2) - main_time_prev >= 7000000)
					{
						char data1[32];
						sprintf(data1, "MIXED \n\r");
						HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

						HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);

//						is_counting = 0;
//						reset_are_nutrs_deployed();
						nutrs_deployed = 0;
//						state = WATER_STATUS_CHECK;
						is_water_ready = 1;
					}
//				}
			}
			break;

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
