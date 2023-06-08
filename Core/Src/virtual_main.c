/*
 * virtual_main.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#include "virtual_main.h"

_Bool is_water_ready = 0; // To check for nutrients

float water_level = 0;
uint32_t water_level_gt = 0;

// ECPH & NUTRIENTS
_Bool ecph_read = 0;
_Bool nutrs_deployed = 0;

// DHT11
uint8_t RHI, RHD, TCI, TCD;
uint16_t SUM;
float tCelsius = 0; // Temp.
float tFahrenheit = 0;
float RH = 0; // Humidity
_Bool dht11_data_ready = 0;

// MACHINE STATE STATUS
machine state = INIT;

// GLOBAL TIMERS
uint32_t send_data_time_prev;
uint32_t ev_time_prev;
uint32_t mix_time_prev;
uint32_t water_proc_time_prev;
uint32_t lights_proc_time_prev;

// GLOBAL TIMERS FLAGS
_Bool is_water_counting = 0;
_Bool is_lights_counting = 0;
_Bool counting_to_load = 0; // Default cycle
_Bool counting_to_unload = 1;
_Bool is_ev_counting = 0;

// WARNING
uint8_t water_status = 0; // 0 WATER OK; 1 WATER LEVEL TOO LOW; 2 WATER LEVEL TOO HIGH
uint8_t EC_status = 0; // 0 EC OK; 1 EC TOO LOW; 2 EC TOO HIGH
uint8_t PH_status = 0; // 0 PH OK; 1 PH TOO LOW; 2 PH TOO HIGH

// LIGHTS CHECK
_Bool lights_off = 0;
int count = 0;

// BLUE BUTTON
GPIO_PinState pin_state;

char data4[32];
char datasss[1279];

void virtual_main()
{
	pin_state = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

	if (pin_state == 0)
	{
	  state = INIT;
	}

	switch(state)
	{
	case INIT:

		// LIGHTS ON
		lights_off = 0;
		setLight(0);
		setLight(1);

		// RESET PUMPS
		HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

		// STEPPER RESET
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4

		// Default cycle
		counting_to_load = 1;
		counting_to_unload = 0;

		// ECPH COUNT
		count = 0;

		// ECPH Reading reset
		ecph_read = 0;

		state = ECPH_PROCEDURE;

		// TIMER INITIALIZATION
		__HAL_TIM_SET_COUNTER(&htim2, 0);

		water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		send_data_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		mix_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		ev_time_prev = __HAL_TIM_GET_COUNTER(&htim2);

		break;

	case HOME:
		/*
		 * WATER CYCLE: After 5m. LOAD, wait 2m. UNLOAD
		 */
//
		sprintf(data4, "Water Level: %f \n\r", water_level);
		HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);

		// 5 Minutes: 3e+8 Microseconds
		if (counting_to_load && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 2e+7) // 10s
		{
			// Check if the pump is submerged

			if (water_level >= MIN_WATER_LVL)
			{
				if (water_level <= MAX_WATER_LVL)
				{
					// WATER LOAD PROCEDURE
					if (is_water_ready)
					{
						EC_status = 0;
						state = WATER_LOAD_PROCEDURE;
						sprintf(data4, "HERE");
						HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);
					}
					else
					{
						if (!ecph_read)
						{
							state = ECPH_PROCEDURE;
						}
						else
						{
							if (EC < EC_LOWER_BOUND)
							{
								EC_status = 1;
								state = MIX_PROCEDURE;
							}
							else
							{
								if (EC > EC_UPPER_BOUND)
								{
									// WARNING: Add Water
									EC_status = 2;
								}
								else
								{
//									is_water_ready = 1;
									if (PH == PH_SETPOINT)
									{
										PH_status = 0;
										is_water_ready = 1;
									}
									else
									{
										if (PH > PH_SETPOINT)
											PH_status = 2;
										else
											PH_status = 1;
									}
								}
								ecph_read = 0;
							}
						}
					}

					water_status = 0;
				}
				else
				{
					water_status = 2;
				}
			}
			else
			{
				water_status = 1;
			}
		}
		// 2 Minutes: 1.2e+8 Microseconds
		if (counting_to_unload && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 30e+6)
		{
			state = WATER_UNLOAD_PROCEDURE;
			count = 0;
		}

		/*
		 * LIGHTS CYCLE: 4h Switch ON/OFF
		 */

		if (__HAL_TIM_GET_COUNTER(&htim2) - lights_proc_time_prev >= 12e+7)
		{
			if (!lights_off)
			{
				lights_off = 1;
				setLight(0);
			}
			else
			{
				lights_off = 0;
				setLight(1);
			}

			lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		}

		if(!lights_off)
			setLight(1);
		else
			setLight(0);

		/*
		 * DATA SEND: Every 5 minutes
		 */

		if (__HAL_TIM_GET_COUNTER(&htim2) - send_data_time_prev >= 6e+7)
		{
			if (!dht11_data_ready)
			{
				state = DHT11_PROCEDURE;
			}
			else
			{
				// tCelsius;RH;EC;PH;WATER_STATUS;
				// tCelsius: float
				// RH: float
				// EC: float
				// PH: uint8_t
				// Water_status: Int

				uint8_t max_size = snprintf(NULL, 0, "%f;%f;%f;%f;%u;%u;%u;\n\r", tCelsius, RH, EC, PH, water_status, EC_status, PH_status) + 1;
				char data_to_send[max_size];
				sprintf(data_to_send, "%f;%f;%f;%f;%u;%u;%u;\n\r", tCelsius, RH, EC, PH, water_status, EC_status, PH_status);
				HAL_UART_Transmit(&huart1, (uint8_t*)data_to_send, strlen(data_to_send), HAL_MAX_DELAY);
				HAL_UART_Transmit(&huart2, (uint8_t*)data_to_send, strlen(data_to_send), HAL_MAX_DELAY);

				send_data_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
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

				dht11_data_ready = 1;

				char data4[32];
				sprintf(data4, "tCelsius: %f \n\r", tCelsius);
				HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);
				char data5[32];
				sprintf(data5, "RH: %f \n\r", RH);
				HAL_UART_Transmit(&huart2, (uint8_t*)data5, strlen(data5), HAL_MAX_DELAY);
			}
		}
		state = HOME;
		break;

	case WATER_LOAD_PROCEDURE:
			// NEW READ FROM THE WATER LEVEL GT SENSOR
			HAL_ADC_Start_IT(&hadc1);

			char datawlp[64];
			sprintf(datawlp, "WATER LOAD PROCEDURE - Water Level GT: %lui \n\r", water_level_gt);
			HAL_UART_Transmit(&huart2, (uint8_t*)datawlp, strlen(datawlp), HAL_MAX_DELAY);

			if (water_level_gt > ROOT_LEVEL)
			{
				// STOP THE PUMP
				HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

				// UPDATE THE VALUES
				is_water_ready = 0;
				counting_to_load = 0;
				counting_to_unload = 1;
				water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
			else
			{
				HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_SET);
			}
			state = HOME;
			break;

	case(WATER_UNLOAD_PROCEDURE):
			// NEW READ FROM THE WATER LEVEL GT SENSOR
			HAL_ADC_Start_IT(&hadc1);

			char datawcc[64];
			sprintf(datawcc, "WATER UNLOAD PROCEDURE - Water Level GT: %lu \n\r", water_level_gt);
			HAL_UART_Transmit(&huart2, (uint8_t*)datawcc, strlen(datawcc), HAL_MAX_DELAY);

			if (__HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 45e+6)
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
			else
			{
				ec_read(&hadc3);
				ph_read(&hadc3);

				if (count >= 1)
				{
					EC = 1.5;
					PH = 5.5;
				}

				char data2[32];
				sprintf(data2, "EC: %f \n\r", EC);
				HAL_UART_Transmit(&huart2, (uint8_t*)data2, strlen(data2), HAL_MAX_DELAY);

				char data1[32];
				sprintf(data1, "PH: %f \n\r", PH);
				HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

				ecph_read = 1;
				ec_initialized = 0;
			}
//			state = HOME;
			break;

	case (MIX_PROCEDURE):
			if (!nutrs_deployed)
			{
				stepper_step_angle(1, 10);

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
					count++;
					sprintf(data1, "MIXED \n\r");
					HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

					HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);
//
//					EC = 1.3;
//					PH = 5.5;

					nutrs_deployed = 0;
					ecph_read = 0; // RICORDARSI DI MODIFICARE QUESTO
				}
			}
			state = HOME;
			break;
	case TEST:
//		HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_SET);
//		setLight(1);
//		sprintf(data4, "Water Level: %f \n\r", water_level);
//		HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);
//
//		HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_SET);
//		HAL_ADC_Start_IT(&hadc1);
//
//		char datawcc[64];
//		sprintf(datawcc, "WATER UNLOAD PROCEDURE - Water Level GT: %lu \n\r", water_level_gt);
//		HAL_UART_Transmit(&huart2, (uint8_t*)datawcc, strlen(datawcc), HAL_MAX_DELAY);
//
//		if (__HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 10e+6)
//		{
//			// STOP THE PUMP
//			HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);
//
//			// UPDATE THE VALUES
////			water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_SET);
//		}
		tCelsius = 25 + count;
		RH = 60;
		EC = 1.23;
		PH = 5.4;

//		uint8_t max_size = snprintf(NULL, 0, "%f;%f;%f;%f;%u; \n\r", tCelsius, RH, EC, PH, water_status) + 1;

		sprintf(datasss, "%f;%f;%f;%f;%u;\n\r", tCelsius, RH, EC, PH, water_status);
		HAL_UART_Transmit(&huart1, (uint8_t*)datasss, strlen(datasss), HAL_MAX_DELAY);
		HAL_Delay(10000);

		if (count == 2) // After 20 sec
			water_status = 1;
		else if (count == 4) // After 40 sec
			water_status = 2;

		count++;
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

//		// LIGHTS OFF
//		setLight(0);
		break;
	}
}
