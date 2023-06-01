///*
// * lights.c
// *
// *  Created on: May 31, 2023
// *      Author: envy0
// */
//
// /* Includes ------------------------------------------------------------------*/
//#include "lights.h"
//
//int violetColor400[3] = {131, 0, 181};
//int redColor680[3] = {255,0,0};
//int whiteBlueColor460[3] = {0, 123, 255};
//int nullColor[3] = {0,0,0};
//
//float B = 1.3*pow(10.0,7);
//float voltage = 0.0;
//float m = -1.4;
//int rRef = 10000;
//
//float voltage_source = 3.3;
//int dataSentFlag = 0;
//
//_Bool led_colors_ready = 0;
//_Bool led_brightness_ready = 0;
//
//extern _Bool is_reading_adc;
//
//uint8_t LED_Data[LEDUsed][4]; //Matrix to be used to store LED data
//uint8_t LED_Mode[LEDUsed][4]; //Matrix to be used for LED brightness
//uint32_t pwmData[(24*LEDUsed)+50]; //24-bit wide communication per each LED
//
//
//void read_natural_light(ADC_HandleTypeDef* hadc)
//{
//
//	if (!is_reading_adc)
//	{
//		HAL_ADC_Start_IT(&hadc2);
//		is_reading_adc = 1;
//	}
//	else
//	{
//		if (lights_reading_done())
//		{
//			voltage = get_lights_voltage_value();
//
//			voltage = voltage * voltage_source / 1024;
//
//			float light_value = B * pow((voltage_source / voltage - 1)  * rRef, m);
//
//			if (light_value < LOW_LIGHT_THREESHOLD)
//			{
//				setBrightness(45);
//			}
//			else if (light_value > HIGH_LIGHT_THREESHOLD)
//			{
//				setBrightness(1);
//			}
//			else
//			{
//				setBrightness(25);
//			}
//
//			is_reading_adc = 0;
//
//			led_brightness_ready = 1;
//		}
//	}
//}
//
//
///*
// * With SetLed function we store data G,R,B data for each LED (row)
// */
//
//
//void setLed (int LEDnum, int color[3])
//{
//	LED_Data[LEDnum][0]= LEDnum;
//	LED_Data[LEDnum][1]= color[1];
//	LED_Data[LEDnum][2]= color[0];
//	LED_Data[LEDnum][3]= color[2];
//}
//
//
///*
// * With SetBrightness function we store data B,G,R,B data for each LED (row)
// */
//
//
//void setBrightness (int brightness)
//{
//	if (USE_BRIGHTNESS){
//		if(brightness > 45) brightness = 45; //If call value exceed limits
//		for (int i =0; i < LEDUsed; i++){
//			LED_Mode[i][0] = LED_Data[i][0];
//			for(int j=1; j<4; j++){
//				float angle = 90-brightness;
//				angle = angle *PI/180;
////				angle = -0.02 * angle + 1.0;
//				LED_Mode[i][j]=(LED_Data[i][j])/(tan(angle)); //use tan to increase linearity of lights' control
//			}
//		}
//	}
//}
//
//
///*
// * This function will communicate a bitwise message of 24bit*Num_LED
// *
// */
//
//
//void ws2812SendData(TIM_HandleTypeDef* htim)
//{
//	uint32_t indx=0;
//	uint32_t color;
//
///*
// * Check if we need to use Brightness values or noBrightness values set color of each led
// * as a 24 bit wide string format of bit FOR EACH LED USED
// */
//	for (int i= 0; i<LEDUsed; i++){
//
//		if (USE_BRIGHTNESS)
//				color = ((LED_Mode[i][1]<<16) | (LED_Mode[i][2]<<8) | (LED_Mode[i][3]));
//		else //If we don't use Brightness we can go over
//				color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
//
//
//		for (int i=23; i>=0; i--) //need to start from LSB on MSB - DataSheet
//		{
//			if (color&(1<<i)){
//				pwmData[indx] = 60; // 2/3 of 90 duty cycle to write 1
//			}
//			else pwmData[indx] = 30;  // 1/3 of 90 duty cycle to write 0
//		indx++;
//		}
//	}
//
//	//Then i need to store 50 pwm signal ->1.25us*50 = 62.5us>50 us for sure to 0 before as close communication singal - DataSheet WS2812
//	for (int i=0; i<50; i++){
//
//		pwmData[indx] = 0;
//		indx++;
//	}
//
//	HAL_TIM_PWM_Start_DMA(htim, TIM_CHANNEL_1, (uint32_t *)pwmData, (24*LEDUsed)+50);
//	while (!dataSentFlag){}; //Giving time to callback function to stop and set datasentflag to 1
//
//	dataSentFlag = 0; //To eventually being called another time
//
//}
//
//
//// Function to be used to end writing the same data on pwm stream data line
//void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){
//
//	HAL_TIM_PWM_Stop_DMA(htim, TIM_CHANNEL_1);
//	dataSentFlag = 1;
//}
//
//
//void set_lights(TIM_HandleTypeDef* htim, ADC_HandleTypeDef* hadc, _Bool status)
//{
//	// STATUS = 1 - LIGHTS ON
//	// STATUS = 0 - LIGHTS OFF
//	if (!led_colors_ready)
//	{
//		if (status == 1)
//		{
//		    for (int i = 0; i < LEDUsed; i++)
//		    {
//		        if (i == 0 || i == 4 || i == 5 || i == 9)
//		        {
//		            setLed(i, violetColor400);
//		        }
//		        else
//		        {
//		            setLed(i, redColor680);
//		        }
//		    }
//		}
//		else
//		{
//		    for (int i = 0; i < LEDUsed; i++)
//		    {
//		        setLed(i, nullColor);
//		    }
//		}
//
//		led_colors_ready = 1;
//	}
//	else if (!led_brightness_ready)
//	{
//		read_natural_light(hadc);
//	}
//	else
//	{
//		ws2812SendData(htim);
//
//		led_colors_ready = 0;
//		led_brightness_ready = 0;
//	}
//}

/*
 * lights.c
 *
 *  Created on: May 22, 2023
 *      Author: envy0
 */

/*
 * lightLowLevelProcedure.c
 *
 *  Created on: 21 Apr 2023
 *      Author: Favali Filippo
 *  	Updated on 16 May 2023 - ADC interrupt reading - 1 light
 */

 /* Includes ------------------------------------------------------------------*/
/*
 * lightLowLevelProcedure.c
 *
 *  Created on: 21 Apr 2023
 *      Author: Favali Filippo
 *  	Updated on 16 May 2023 - ADC interrupt reading - 1 light
 */

 /* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "gpio.h"
#include "lights.h"
#include "math.h"
#include "usart.h"
#include <stdio.h>
 //Define photoResistor____________________________

#define HIGH_LIGHT_THREESHOLD 200
#define LOW_LIGHT_THREESHOLD 100
lightThreeshold naturalLightLevel; //natural light level set to low
#define voltageSource 3.3  //Voltage divider VCC ref
#define luxFactor = 500000

float B = 1.3*pow(10.0,7);
float voltageValue = 0.0;
float m = -1.4;
 //Define WS2812 ledStripe_________________________

#define LEDUsed 10
#define USE_BRIGHTNESS 1 //1 if you want to control brightness, 0 if not
#define PI 3.14
uint8_t LED_Data[LEDUsed][4]; //Matrix to be used to store LED data
uint8_t LED_Mode[LEDUsed][4]; //Matrix to be used for LED brightness

int violetColor400[3] = {131, 0, 181};
int redColor680[3] = {255,0,0};
int whiteBlueColor460[3] = {0, 123, 255};
int nullColor[3] = {0,0,0};

int dataSentFlag = 0;

// Violet 400nm light #8300b5 ---> (131, 0, 181)
// Red 680nm light ---> (255, 0, 0)
// whiteBlue 460nm light #007bff --> (0, 123, 255)


void readNaturalLight(){

	int rRef = 10000;
//	HAL_ADC_Start(&hadc2);
	HAL_ADC_Start_IT(&hadc2);
//	HAL_ADC_PollForConversion(&hadc2,100);
//	HAL_ADC_Stop(&hadc2);
//	voltageValue = HAL_ADC_GetValue(&hadc2);

	voltageValue = voltageValue*voltageSource/1024;
	float lightValue = B * pow((voltageSource/voltageValue - 1)  * rRef, m);

	if (lightValue < LOW_LIGHT_THREESHOLD){
			naturalLightLevel = Low;
		}
	else if (lightValue > HIGH_LIGHT_THREESHOLD){
		naturalLightLevel = High;
		}
	else {
		naturalLightLevel = Med;
	}

}

//when completed conversion occurs the result it has to be stored in the variable


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	HAL_ADC_Stop_IT(&hadc2);
	voltageValue = HAL_ADC_GetValue(&hadc2);

}


/*
 * With SetLed function we store data G,R,B data for each LED (row)
 */
void setLed (int LEDnum, int color[3]){

	LED_Data[LEDnum][0]= LEDnum;
	LED_Data[LEDnum][1]= color[1];
	LED_Data[LEDnum][2]= color[0];
	LED_Data[LEDnum][3]= color[2];

}

/*
 * With SetBrightness function we store data B,G,R,B data for each LED (row)
 */

void setBrightness (int brightness)
{
	if (USE_BRIGHTNESS){
		if(brightness > 45) brightness = 45; //If call value exceed limits
		for (int i =0; i < LEDUsed; i++){
			LED_Mode[i][0] = LED_Data[i][0];
			for(int j=1; j<4; j++){
				float angle = 90-brightness;
				angle = angle *PI/180;
//				angle = -0.02 * angle + 1.0;
				LED_Mode[i][j]=(LED_Data[i][j])/(tan(angle)); //use tan to increase linearity of lights' control
			}
		}
	}
}


/*
 * This function will communicate a bitwise message of 24bit*Num_LED
 *
 */

uint32_t pwmData[(24*LEDUsed)+50]; //24-bit wide communication per each LED

void ws2812SendData (void)
{
	uint32_t indx=0;
	uint32_t color;

/*
 * Check if we need to use Brightness values or noBrightness values set color of each led
 * as a 24 bit wide string format of bit FOR EACH LED USED
 */
	for (int i= 0; i<LEDUsed; i++){

		if (USE_BRIGHTNESS)
				color = ((LED_Mode[i][1]<<16) | (LED_Mode[i][2]<<8) | (LED_Mode[i][3]));
		else //If we don't use Brightness we can go over
				color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));


		for (int i=23; i>=0; i--) //need to start from LSB on MSB - DataSheet
		{
			if (color&(1<<i)){
				pwmData[indx] = 60; // 2/3 of 90 duty cycle to write 1
			}
			else pwmData[indx] = 30;  // 1/3 of 90 duty cycle to write 0
		indx++;
		}
	}

	//Then i need to store 50 pwm signal ->1.25us*50 = 62.5us>50 us for sure to 0 before as close communication singal - DataSheet WS2812
	for (int i=0; i<50; i++){

		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, (24*LEDUsed)+50);
	while (!dataSentFlag){}; //Giving time to callback function to stop and set datasentflag to 1

	dataSentFlag = 0; //To eventually being called another time

}

//Function to be used to end writing the same data on pwm stream data line
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim){

	HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
	dataSentFlag = 1;

}

void setLight(int check){
	if (check == 1){

		for(int i = 0; i<LEDUsed; i++){
			switch(i){
			case(0):
				setLed(i, violetColor400);
//				setBrightness(brightness);
				break;
			case(1):
				setLed(i, redColor680);
//				setBrightness(brightness);
				break;
			case(2):
				setLed(i, redColor680);
//				setBrightness(brightness);
				break;
			case(3):
				setLed(i, redColor680);
//				setBrightness(brightness);
				break;
			case(4):
				setLed(i, violetColor400);
//				setBrightness(brightness);
				break;
			case(5):
				setLed(i, violetColor400);
//				setBrightness(brightness);
				break;
			case(6):
				setLed(i, redColor680);
//				setBrightness(brightness);
				break;
			case(7):
				setLed(i, redColor680);
//				setBrightness(brightness);
				break;
			case(8):
				setLed(i, redColor680);
//				setBrightness(brightness);
				break;
			case(9):
				setLed(i, violetColor400);
//				setBrightness(brightness);
				break;
			}

		}
	}
	else  {
		for(int i=0; i<LEDUsed; i++){

			setLed(i, nullColor);
		}
	}

	//Reading natural light level value threshold and save it to naturalLightLevel
	readNaturalLight();

	switch(naturalLightLevel){
		case (Low):
			setBrightness(45);
			break;
		case(Med):
			setBrightness(25);
			break;
		case(High):
			setBrightness(1);
			break;
	}
	//Once data has been defined, send the data to ledStripe
	ws2812SendData();
}
