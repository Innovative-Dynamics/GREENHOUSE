/*
 * virtual_main.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#include "virtual_main.h"

// ADC REASDINGS
float water_level = 0;
uint32_t water_level_gt = 0;

// ECPH & NUTRIENTS
_Bool ecph_read = 0;
_Bool nutrs_deployed = 0;
_Bool is_water_ready = 0; // To check for nutrients

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
_Bool counting_to_load = 0;
_Bool counting_to_unload = 1;
_Bool is_ev_counting = 0;

// WARNING
uint8_t water_status = 0;
_Bool warning = 0;

// LIGHTS CHECK
_Bool lights_status = 0;

// SIMULATION
int count = 0;

// BLUE BUTTON
GPIO_PinState pin_state;

// DATA
char data_home[512];

void virtual_main()
{
	// BLUE BUTTON GREENHOUSE RESET
	pin_state = HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin);

	if (pin_state == 0)
	{
	  state = INIT;
	}

	switch(state)
	{
	case INIT:

		// LIGHTS
		lights_status = 0;
		setLight(0);
		setLight(1);

		// PUMPS RESET
		HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

		// STEPPER RESET
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4

		// WATER CYCLE
		counting_to_load = 1;
		counting_to_unload = 0;

		// ECPH
		count = 0;
		ecph_read = 0;

		// WARNINGS
		warning = 0;

		// TIMERS
		__HAL_TIM_SET_COUNTER(&htim2, 0);
		water_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		send_data_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		mix_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		ev_time_prev = __HAL_TIM_GET_COUNTER(&htim2);

		state = HOME;
		break;

	case HOME:
		/*
		 * WATER CYCLE
		 */

		// DEBUG
		sprintf(data_home, "HOME PROCEDURE - Water Level: %f\n\r", water_level);
		HAL_UART_Transmit(&huart2, (uint8_t*)data_home, strlen(data_home), HAL_MAX_DELAY);

		if (counting_to_load && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= WATER_LOAD_TIME) // 10s
		{
		    if (water_level >= MIN_WATER_LVL && water_level <= MAX_WATER_LVL)
		    {
		        if (is_water_ready)
		        {
		            state = WATER_LOAD_PROCEDURE;
		        }
		        else if (!ecph_read)
		        {
		            state = ECPH_PROCEDURE;
		        }
		        else if (EC < EC_LOWER_BOUND)
		        {
		        	// WARNING: EC TOO LOW
		            warning = 1;
		            state = MIX_PROCEDURE;
		        }
		        else if (EC > EC_UPPER_BOUND)
		        {
		            // WARNING: EC TOO HIGH
		        	warning = 1;
		            ecph_read = 0;
		        }
		        else if (PH == PH_SETPOINT)
		        {
		            ecph_read = 0;
		            is_water_ready = 1;
		        }
		        else if (PH > PH_SETPOINT)
		        {
		        	// WARNING: PH TOO HIGH
		        	warning = 1;
		            ecph_read = 0;
		        }
		        else
		        {
		        	// WARNING: PH TOO LOW
		        	warning = 1;
		            ecph_read = 0;
		        }
		        water_status = 0;
		    }
		    else if (water_level > MAX_WATER_LVL)
		    {
		    	// WARNING: WATER TOO HIGH
		        water_status = 2;
		        warning = 1;
		    }
		    else
		    {
		    	// WARNING: WATER TOO LOW
		        water_status = 1;
		        warning = 1;
		    }
		}
		if (counting_to_unload && __HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= WATER_UNLOAD_TIME)
		{
			state = WATER_UNLOAD_PROCEDURE;
			count = 0;
		}

		/*
		 * LIGHTS CYCLE
		 */

		if (__HAL_TIM_GET_COUNTER(&htim2) - lights_proc_time_prev >= LIGHTS_SWITCH_TIME)
		{
			lights_status = !lights_status;
			lights_proc_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		}

		setLight(lights_status);

		/*
		 * DATA SEND
		 */

		if (__HAL_TIM_GET_COUNTER(&htim2) - send_data_time_prev >= SEND_DATA_TIME || warning)
		{
			if (!dht11_data_ready)
			{
				state = DHT11_PROCEDURE;
			}
			else
			{
				uint8_t max_size = snprintf(NULL, 0, "%f;%f;%f;%f;%u;\n\r", tCelsius, RH, EC, PH, water_status) + 1;
				char data_to_send[max_size];
				sprintf(data_to_send, "%f;%f;%f;%f;%u;\n\r", tCelsius, RH, EC, PH, water_status);
				HAL_UART_Transmit(&huart1, (uint8_t*)data_to_send, strlen(data_to_send), HAL_MAX_DELAY);

				// DEBUG
				HAL_UART_Transmit(&huart2, (uint8_t*)data_to_send, strlen(data_to_send), HAL_MAX_DELAY);

				send_data_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
				if (warning)
					warning = 0;
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
				tCelsius = (float)TCI + (float)(TCD/10.0);
				tFahrenheit = tCelsius * 9/5 + 32;
				RH = (float)RHI + (float)(RHD/10.0);

				dht11_data_ready = 1;

				// DEBUG
				char data_t_celsius[512];
				sprintf(data_t_celsius, "DHT11 PROCEDURE - tCelsius: %f\n\r", tCelsius);
				HAL_UART_Transmit(&huart2, (uint8_t*)data_t_celsius, strlen(data_t_celsius), HAL_MAX_DELAY);
				char data_humidity[512];
				sprintf(data_humidity, "DHT11 PROCEDURE - RH: %f\n\r", RH);
				HAL_UART_Transmit(&huart2, (uint8_t*)data_humidity, strlen(data_humidity), HAL_MAX_DELAY);
			}
		}
		state = HOME;
		break;

	case WATER_LOAD_PROCEDURE:
			HAL_ADC_Start_IT(&hadc1);

			// DEBUG
			char datawlp[512];
			sprintf(datawlp, "WATER LOAD PROCEDURE - Water Level GT: %lu\n\r", water_level_gt);
			HAL_UART_Transmit(&huart2, (uint8_t*)datawlp, strlen(datawlp), HAL_MAX_DELAY);

			if (water_level_gt > ROOT_LEVEL)
			{
				HAL_GPIO_WritePin(LOAD_WATER_PUMP_GPIO_Port, LOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

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
			HAL_ADC_Start_IT(&hadc1);
			// DEBUG
			char datawulp[512];
			sprintf(datawulp, "WATER UNLOAD PROCEDURE - Water Level GT: %lu\n\r", water_level_gt);
			HAL_UART_Transmit(&huart2, (uint8_t*)datawulp, strlen(datawulp), HAL_MAX_DELAY);

			if (__HAL_TIM_GET_COUNTER(&htim2) - water_proc_time_prev >= 40e+6)
			{

				HAL_GPIO_WritePin(UNLOAD_WATER_PUMP_GPIO_Port, UNLOAD_WATER_PUMP_Pin, GPIO_PIN_RESET);

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

				// DEBUG
				char data_ec[512];
				sprintf(data_ec, "ECPH PROCEDURE - EC: %f\n\r", EC);
				HAL_UART_Transmit(&huart2, (uint8_t*)data_ec, strlen(data_ec), HAL_MAX_DELAY);
				char data_ph[512];
				sprintf(data_ph, "ECPH PROCEDURE - PH: %f\n\r", PH);
				HAL_UART_Transmit(&huart2, (uint8_t*)data_ph, strlen(data_ph), HAL_MAX_DELAY);

				ecph_read = 1;
				ec_initialized = 0;
			}
			state = HOME;
			break;

	case (MIX_PROCEDURE):
			if (!nutrs_deployed)
			{
				stepper_step_angle(1, 10);

				// DEBUG
				char data_stepper[512];
				sprintf(data_stepper, "MIX PROCEDURE - STEPPER DONE\n\r");
				HAL_UART_Transmit(&huart2, (uint8_t*)data_stepper, strlen(data_stepper), HAL_MAX_DELAY);

				HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_SET);
				mix_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
			}
			else
			{
				if (__HAL_TIM_GET_COUNTER(&htim2) - mix_time_prev >= 7000000)
				{

					HAL_GPIO_WritePin(MIX_PUMP_GPIO_Port, MIX_PUMP_Pin, GPIO_PIN_RESET);

					// DEBUG
					char data_mixing[512];
					sprintf(data_mixing, "MIXED\n\r");
					HAL_UART_Transmit(&huart2, (uint8_t*)data_mixing, strlen(data_mixing), HAL_MAX_DELAY);

					count++;
					nutrs_deployed = 0;
					ecph_read = 0;
				}
			}
			state = HOME;
			break;
	case TEST:
		break;

	default:
		state = INIT;
		break;
	}
}
