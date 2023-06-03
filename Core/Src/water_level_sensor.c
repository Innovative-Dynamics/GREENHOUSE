/*
 * water_level_sensor.c
 *
 *  Created on: Jun 3, 2023
 *      Author: envy0
 */

#include "adc.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "stdio.h"

float vWaterLevel = 128 ;
_Bool water_flag = 0;
int convCompleted = 0;
int treeshold = 124;

_Bool get_water_level ()
{
	return water_flag;
}

void convertLevel(float read){

	if (read >= treeshold) {
		water_flag = 1; // 1
	}
	else {
		water_flag = 0; // 0
	}

}


void readLevelPolling(){

	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	HAL_ADC_Stop(&hadc1);
	vWaterLevel = HAL_ADC_GetValue(&hadc1);
	convertLevel(vWaterLevel);
}

void readLevelInterrupt(){

	HAL_ADC_Start_IT(&hadc1);

	convertLevel(vWaterLevel);

//	if(flagWater==full){
//		char *a="\nvWaterLevel interrupt: alto";
//		HAL_UART_Transmit(&huart2, (uint8_t*)a, strlen(a),0xFFFF);
//		char b[32];
//		sprintf(b, "%.2f\n", vWaterLevel);
//		HAL_UART_Transmit(&huart2, (uint8_t*)b, strlen(b), 0xFFFF);
////		char b[32];
////		sprintf(b, "%.2f\n", alto); //converte float a stringa
////		HAL_UART_Transmit(&huart2, (uint8_t*)b, strlen(b),0xFFFF);
//	}
//	if(flagWater==empty){
//		char *a="\nvWaterLevel interrupt: basso";
//		HAL_UART_Transmit(&huart2, (uint8_t*)a, strlen(a), 0xFFFF);
//		char b[32];
//		sprintf(b, "%.2f\n", vWaterLevel); //converte float a stringa
//		HAL_UART_Transmit(&huart2, (uint8_t*)b, strlen(b), 0xFFFF);
////		char b[32];
////		sprintf(b, "%.2f\n", basso); //converte float a stringa
////		HAL_UART_Transmit(&huart2, (uint8_t*)b, strlen(b),0xFFFF);
//	}
//
//	HAL_Delay(2000);

}
//
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	HAL_ADC_Stop_IT(&hadc1);
//	vWaterLevel = HAL_ADC_GetValue(&hadc1);
//
//}
