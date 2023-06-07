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

uint32_t water_level_gt = ROOT_LEVEL;

_Bool is_default_check = 1; // TRUE == 1; FALSE == 0

_Bool loading = 0;
_Bool unloading = 0;

//_Bool reading_value = 0;
//_Bool is_reading_adc = 0;

//_Bool is_counting = 0;

_Bool dht11_data_ready = 0;
_Bool water_status_initialized = 0;

//uint32_t time_prev = 0;
//long long int water_time_prev = 0;

float temp = 0;
float humidity = 0;

// DHT11
uint8_t RHI, RHD, TCI, TCD;
uint16_t SUM;
float tCelsius = 0;
float tFahrenheit = 0;
float RH = 0;

//float EC_Value = 0;
//float PH_Value = 0;

// LIGHTS PROCEDURE WORKING DONT TOUCH
// ECPH PROCEDURE WORKING DONT TOUCH
// MIX PROCEDURE WORKING DONT TOUCH
// WATER LOAD PROCEDURE WORKING DONT TOUCH
// WATER UNLOAD PROCEDURE WORKING DONT TOUCH
// WATER READING SENSOR WORKING DONT TOUCH

machine state = INIT;
//uint16_t water_tank_level;

		char data40[32];
// GLOBAL TIMERS
uint32_t send_data_time_prev;
uint32_t ev_time_prev;
uint32_t mix_time_prev;
uint32_t water_proc_time_prev;
uint32_t lights_proc_time_prev;

_Bool is_water_counting = 0;
_Bool is_lights_counting = 0;
_Bool counting_to_load = 0; // Default cycle
_Bool counting_to_unload = 1;
_Bool is_ev_counting = 0;

uint8_t water_status = 0; // 0 WATER OK; 1 WATER LEVEL TOO LOW; 2 WATER LEVEL TOO HIGH

_Bool lights_off = 0;
_Bool ecph_read = 0;

char datahome[32];
char datadht11[32];

void virtual_main()
{
	switch(state)
	{
	case INIT:
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		send_data_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		mix_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		ev_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//		setLight(1);

		counting_to_load = 1; // Default cycle
		counting_to_unload = 0;

		 tCelsius = 27.0;
		 RH = 12.0;
		 EC = 1.5;
		 PH = 7.0;
		 water_status = 0;

		state = HOME;

//		char datainit[32];
//		sprintf(datainit, "INIT STATE \n\r");
//		HAL_UART_Transmit(&huart2, (uint8_t*)datainit, strlen(datainit), HAL_MAX_DELAY);
		break;

	case HOME:
//		sprintf(datahome, "HOME STATE \n\r");
//		HAL_UART_Transmit(&huart2, (uint8_t*)datahome, strlen(datahome), HAL_MAX_DELAY);

//		char datahomeproc[64];
//		sprintf(datahomeproc, "HOME PROCEDURE - Water Level GT: %lu \n\r", water_level_gt);
//		HAL_UART_Transmit(&huart2, (uint8_t*)datahomeproc, strlen(datahomeproc), HAL_MAX_DELAY);
//
//		char datahomep[64];
//		sprintf(datahomep, "HOME PROCEDURE - Water Level: %f \n\r", water_level);
//		HAL_UART_Transmit(&huart2, (uint8_t*)datahomep, strlen(datahomep), HAL_MAX_DELAY);

//		sprintf(data40, "Water Level: %f \n\r", water_level);
//		HAL_UART_Transmit(&huart2, (uint8_t*)data40, strlen(data40), HAL_MAX_DELAY);
		/*
		 * WATER CYCLE: After 5m. LOAD, wait 2m. UNLOAD
		 */

		// 5 Minutes: 3e+8 Microseconds
//		if (counting_to_load && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 1e+7)
//		{
//			// Check if the pump is submerged
//
//			if (water_level >= MIN_WATER_LVL)
//			{
//				if (water_level <= MAX_WATER_LVL)
//				{
//					// WATER LOAD PROCEDURE
//					if (is_water_ready)
//					{
//						state = WATER_LOAD_PROCEDURE;
//					}
//					else
//					{
//						if (!ecph_read)
//						{
//							state = ECPH_PROCEDURE;
//						}
//						else
//						{
//							if (EC < EC_SETPOINT)
//							{
//								state = MIX_PROCEDURE;
//							}
//							else
//							{
//								is_water_ready = 1;
//								ecph_read = 0;
//							}
//						}
//					}
//
//					water_status = 0;
//				}
//				else
//				{
//					water_status = 2;
//				}
//			}
//			else
//			{
//				water_status = 1;
//			}
//		}
//		// 2 Minutes: 1.2e+8 Microseconds
//		if (counting_to_unload && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 1e+7)
//		{
//			state = WATER_UNLOAD_PROCEDURE;
//		}

		/*
		 * LIGHTS CYCLE: 5 Minutes Switch ON/OFF
		 */

//		if (__HAL_TIM_GET_COUNTER(&htim2) - lights_proc_time_prev >= 3e+7)
//		{
//			if (!lights_off)
//			{
//				lights_off = 1;
//				setLight(0);
//			}
//			else
//			{
//				lights_off = 0;
//				setLight(1);
//			}
//
//			lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//		}


		/*
		 * SEND DATA
		 */
		if (__HAL_TIM_GET_COUNTER(&htim2) - send_data_time_prev >= 1e+7)
		{
//			if (!dht11_data_ready)
//			{
//				state = DHT11_PROCEDURE;
//			}
//			else
//			{
				// tCelsius;RH;EC;PH;WATER_STATUS;
				// tCelsius: float
				// RH: float
				// EC: float
				// PH: uint8_t
				// Water_status: Int

				uint8_t max_size = snprintf(NULL, 0, "%f;%f;%f;%u;%u;\n\r", tCelsius, RH, EC, PH, water_status) + 1;

				char data_to_send[max_size];
				sprintf(data_to_send, "%f;%f;%f;%u;%u; \n\r", tCelsius, RH, EC, PH, water_status);
				HAL_UART_Transmit(&huart1, (uint8_t*)data_to_send, strlen(data_to_send), HAL_MAX_DELAY);

				char datatt[32];
				sprintf(datatt, "Data Size: %i \n\r", max_size);
				HAL_UART_Transmit(&huart2, (uint8_t*)datatt, strlen(datatt), HAL_MAX_DELAY);

				dht11_data_ready = 0;
				send_data_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//			}
		}
		break;

	case DHT11_PROCEDURE:
		if(DHT11_Start())
		{
			RHI = DHT11_Read(); // Relative humidity integral
			RHD = DHT11_Read(); // Relative humidity decimal
			TCI = DHT11_Read(); // Celsius integral
			TCD = DHT11_Read(); // Celsius decimal
			SUM = DHT11_Read(); // Check sum
			if (RHI + RHD + TCI + TCD == SUM)
			{
				// Can use RHI and TCI for any purposes if whole number only needed
				tCelsius = (float)TCI + (float)(TCD/10.0);
				tFahrenheit = tCelsius * 9/5 + 32;
				RH = (float)RHI + (float)(RHD/10.0);

				state = HOME;
				dht11_data_ready = 1;

				char data4[32];
				sprintf(data4, "tCelsius: %f \n\r", tCelsius);
				HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);
				char data5[32];
				sprintf(data5, "RH: %f \n\r", RH);
				HAL_UART_Transmit(&huart2, (uint8_t*)data5, strlen(data5), HAL_MAX_DELAY);

				// Can use tCelsius, tFahrenheit and RH for any purposes
			}
		}
		break;

	case WATER_LOAD_PROCEDURE:

				HAL_ADC_Start_IT(&hadc1);
				char datawlp[64];
				sprintf(datawlp, "WATER LOAD PROCEDURE - Water Level GT: %lui \n\r", water_level_gt);
				HAL_UART_Transmit(&huart2, (uint8_t*)datawlp, strlen(datawlp), HAL_MAX_DELAY);

				if (water_level_gt >= 180)
				{
					// STOP THE PUMP
					HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

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
					HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_SET);
				}
//			}
			break;

	case(WATER_UNLOAD_PROCEDURE):
			// NEW READ FROM THE WATER LEVEL GT SENSOR
			HAL_ADC_Start_IT(&hadc1);

			char datawulp[64];
			sprintf(datawulp, "WATER UNLOAD PROCEDURE - Water Level GT: %lu \n\r", water_level_gt);
			HAL_UART_Transmit(&huart2, (uint8_t*)datawulp, strlen(datawulp), HAL_MAX_DELAY);

			if (water_level_gt < 100)
			{
				// STOP THE PUMP
				HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

				// UPDATE THE VALUES
				counting_to_load = 1;
				counting_to_unload = 0;
				water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
			else
			{
				HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_SET);
			}

			state = HOME;
			break;

	case(ECPH_PROCEDURE):

			if (!ec_initialized)
			{
				ec_init();
			}
			// CHECK FOR 5 SECONDS
			else
			{
//				ec_read(&hadc3);
				ph_read(&hadc3);

				char data2[32];
				sprintf(data2, "EC: %f \n\r", EC);
				HAL_UART_Transmit(&huart2, (uint8_t*)data2, strlen(data2), HAL_MAX_DELAY);

				char data1[32];
				sprintf(data1, "PH: %u \n\r", PH);
				HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

				ecph_read = 1;
				ec_initialized = 0;
			}

			state = HOME;
			break;

	case (MIX_PROCEDURE):
			if (!nutrs_deployed)
			{
				stepper_step_angle(360, 1, 10);

				char data1[32];
				sprintf(data1, "NUTRS DEPLOYED \n\r");
				HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

				HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_SET);
				mix_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
			else
			{
				if (__HAL_TIM_GET_COUNTER(&htim2) - mix_time_prev >= 7000000)
				{
					char data1[32];
					sprintf(data1, "MIXED \n\r");
					HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

					HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);

					EC = 1.5;

					nutrs_deployed = 0;
					ecph_read = 0;
				}
			}
			state = HOME;
			break;

	case(LIGHTS_PROCEDURE):
//			setLight(1);
						HAL_ADC_Start_IT(&hadc1);
						char datatest[64];
						sprintf(datatest, "WATER LOAD PROCEDURE - Water Level GT: %lu \n\r", water_level_gt);
						HAL_UART_Transmit(&huart2, (uint8_t*)datatest, strlen(datatest), HAL_MAX_DELAY);
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
