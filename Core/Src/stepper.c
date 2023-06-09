/*
 * step_main.c
 *
 *  Created on: May 19, 2023
 *      Author: Luca Bonori
 */

#include "stepper.h"

float deltaEC = 0;
float angle = 0;
float coeff_angle = 720;

void stepper_set_rpm(int rpm)  // Set rpm--> max 13, min 1,,,  went to 14 rev/min
{
	delay_us(60000000 / stepsperrev / rpm);
}

void stepper_half_drive(int step)
{
	switch (step) {
	case 0:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_SET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4
		break;

	case 1:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_SET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_SET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4
		break;

	case 2:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_SET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4
		break;

	case 3:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_SET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_SET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4
		break;

	case 4:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_SET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_RESET);   // IN4
		break;

	case 5:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_SET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_SET);   // IN4
		break;

	case 6:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_RESET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_SET);   // IN4
		break;

	case 7:
		HAL_GPIO_WritePin(STEP_IN1_GPIO_Port, STEP_IN1_Pin, GPIO_PIN_SET);   // IN1
		HAL_GPIO_WritePin(STEP_IN2_GPIO_Port, STEP_IN2_Pin, GPIO_PIN_RESET);   // IN2
		HAL_GPIO_WritePin(STEP_IN3_GPIO_Port, STEP_IN3_Pin, GPIO_PIN_RESET);   // IN3
		HAL_GPIO_WritePin(STEP_IN4_GPIO_Port, STEP_IN4_Pin, GPIO_PIN_SET);   // IN4
		break;
	}
}

void stepper_step_angle(int direction, int rpm)
{

	deltaEC = EC_SETPOINT - EC;

	char data1[32];
	sprintf(data1, "deltaEC: %f \n\r", deltaEC);
	HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);

	angle = coeff_angle * deltaEC;
	
	if (angle > 0)
    	{

		int numberofsequences = (int)(angle / anglepersequence);

		for (int seq = 0; seq < numberofsequences; seq++)
		{
			if (direction == 0)  // for clockwise
			{
				for (int step = 7; step >= 0; step--)
				{
					stepper_half_drive(step);
					stepper_set_rpm(rpm);
				}
			}
			else if (direction == 1)  // for anti-clockwise
			{
				for (int step = 0; step < 8; step++)
				{
					stepper_half_drive(step);
					stepper_set_rpm(rpm);
				}
			}
		}
	}

	nutrs_deployed = 1;
}
