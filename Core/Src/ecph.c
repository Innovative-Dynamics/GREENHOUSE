/*
 * ecph.c
 *
 *  Created on: May 29, 2023
 *      Author: Innovative Dynamics
 */

#include "ecph.h"

// EC METER VARS.
float adEC = 0;
float VdropEC = 0;
float RWater = 0;

// PH METER VARS.
float adPH = 0;
float VdropPH = 0;

uint32_t ecph_time_prev = 0;
_Bool is_counting_ec = 0;


void ec_init ()
{
	if (is_counting_ec == 0)
	{
		is_counting_ec = 1;
		HAL_GPIO_WritePin(EC_WRITE_GPIO_Port, EC_WRITE_Pin, GPIO_PIN_SET);
		ecph_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
	}
	else
	{
		if (__HAL_TIM_GET_COUNTER(&htim2) - ecph_time_prev >= 6000000)
		{
			is_counting_ec = 0;
			ec_initialized = 1;
		}
	}
}

void ec_read (ADC_HandleTypeDef* hadc)
{
	set_ec_channel(); // Set the ADC to read on the EC channel

	HAL_ADC_Start(hadc); // ADC Conversion
	HAL_ADC_PollForConversion(hadc, 100);
	adEC = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);

	VdropEC= (Vin *adEC) / 1024.0; // From voltage to bit
	RWater = (VdropEC*R1) / (Vin-VdropEC);
	EC = 1000 / (RWater*K1); // mS/cm
}

void ph_read (ADC_HandleTypeDef* hadc)
{
	set_ph_channel(); // Set the ADC to read on the pH channel

	HAL_ADC_Start(hadc); // ADC Conversion
	HAL_ADC_PollForConversion(hadc, 100);
	adPH = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);

	VdropPH = (Vin * adPH) / 1024.0; // From voltage to bit

	// pH values conversion
	if (VdropPH == 0)
		PH = 3.0;
	else if (VdropPH > 0 && VdropPH <= ph_interval)
		PH = 3.5;
	else if (VdropPH > ph_interval && VdropPH <= ph_interval*2)
		PH = 4.0;
	else if (VdropPH > ph_interval*2 && VdropPH <= ph_interval*3)
		PH = 4.5;
	else if (VdropPH > ph_interval*3 && VdropPH <= ph_interval*4)
		PH = 5.0;
	else if (VdropPH > ph_interval*4 && VdropPH <= ph_interval*5)
		PH = 5.5;
	else if (VdropPH > ph_interval*5 && VdropPH <= ph_interval*6)
		PH = 6.0;
	else if (VdropPH > ph_interval*6 && VdropPH <= ph_interval*7)
		PH = 6.5;
	else if (VdropPH > ph_interval*7 && VdropPH <= ph_interval*8)
		PH = 7.0;
	else if (VdropPH > ph_interval*8 && VdropPH <= ph_interval*9)
		PH = 7.5;
	else if (VdropPH > ph_interval*9 && VdropPH <= ph_interval*10)
		PH = 8.0;
	else if (VdropPH > ph_interval*10 && VdropPH <= ph_interval*11)
		PH = 8.5;
	else if (VdropPH > ph_interval*11 && VdropPH <= ph_interval*12)
		PH = 9.0;
	else if (VdropPH > ph_interval*12 && VdropPH <= ph_interval*13)
		PH = 9.5;
	else
		PH = 10.0;
}

void set_ec_channel ()
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_10B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = EC_CHANNEL;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

void set_ph_channel()
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc3.Init.Resolution = ADC_RESOLUTION_10B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = PH_CHANNEL;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}
