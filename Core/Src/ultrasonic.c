/*
 * ultrasonic.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */

#include "ultrasonic.h"
#include "stdbool.h"

uint32_t hc_counter = 0;		//counter timer fro hc_04 distance computaion
float distance_hc = 0;						//distance measured
_Bool pwm_state = 0;					//variable to measured pwm state
_Bool prev_pwm_state = 0;				//variable to measured previous pwm state
_Bool echo_state = 0;					//variable to store echo state
float water_level = 0;

void setup_pwm(void)
{
	//Setup config variable
	TIM_OC_InitTypeDef my_sConfigOC = {0};
	my_sConfigOC.OCMode = TIM_OCMODE_PWM1;
	my_sConfigOC.Pulse = 1;
	my_sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	my_sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;

	//Start PWM
	HAL_TIM_PWM_Stop(&htim8, TIM_CHANNEL_1);
	if (HAL_TIM_PWM_ConfigChannel(&htim8, &my_sConfigOC, TIM_CHANNEL_1) != HAL_OK){Error_Handler();}
	HAL_TIM_PWM_Start_IT(&htim8, TIM_CHANNEL_1);
}

//this function is called when a rising/falling event of the interrupt occurs
void echo_callback (void)
{
	//Read the state of the GPIO
	echo_state = HAL_GPIO_ReadPin(ECHO_GPIO_Port, ECHO_Pin);
	//If the state is HIGH, start counting, else stop
	if(echo_state)
	{
		hc_counter = __HAL_TIM_GET_COUNTER(&htim2);
	}
	else if (!echo_state)
	{
		//Convert distance into
		water_level = (float)(__HAL_TIM_GET_COUNTER(&htim2) - hc_counter)*0.034/2;;
	}
}
