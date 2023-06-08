/*
 * ecph.c
 *
 *  Created on: May 29, 2023
 *      Author: envy0
 */


#include "ecph.h"

// EC METER VARS.
float EC = 0;
float adEC = 0;
float VdropEC = 0;
float RWater = 0;

// PH METER VARS.
float adPH = 0;
float VdropPH = 0;
float PH = 0;

_Bool is_counting_ec = 0;
_Bool ec_initialized = 0;

uint32_t ecph_time_prev;

void ec_init ()
{
	if (is_counting_ec == 0)
	{
		ecph_time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		is_counting_ec = 1;
		HAL_GPIO_WritePin(EC_WRITE_GPIO_Port, EC_WRITE_Pin, GPIO_PIN_SET); // Imposto il pin ECPower(PC0 -> A5) a livello alto: 3.3v
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

	set_ec_channel();

	HAL_ADC_Start(hadc); // avvia la conversione ADC da EC_Read(PC1 -> A4)
	HAL_ADC_PollForConversion(hadc, 100); // attendi la fine della conversione
	adEC = HAL_ADC_GetValue(hadc);//Leggo il valore analog
	HAL_ADC_Stop(hadc);

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //Imposto il pin ECPower(PC0 -> A5) a livello basso: 0v

	VdropEC= (Vin *adEC) / 1024.0; //converto tot bit(adEC) in tensione(VdropEC) (precision_ADC = 2^10 -> = 1024)
	RWater = (VdropEC*R1) / (Vin-VdropEC); // prova ad aumentare la precisione a 2^12
	EC = 1000 / (RWater*K1); //mS/cm

//	char data4[32];
//	sprintf(data4, "EC Pre Gestione: %f \n\r", EC);
//	HAL_UART_Transmit(&huart2, (uint8_t*)data4, strlen(data4), HAL_MAX_DELAY);
//
//    // GESTIONE DATI EC
//	if (EC <= 0.3)
//	{
//		EC = 0;
//	}
//	else if (EC >= 0.3 && EC <= 0.9)
//	{
//		EC = EC - 0.2;
//	}
//	else if (EC > 1.5 && EC <= 1.6)
//	{
//	    EC = EC + 0.25;
//	}
//	else if (EC > 1.6 && EC <= 1.7)
//	{
//	    EC = EC + 0.35;
//	}
}

void ph_read (ADC_HandleTypeDef* hadc)
{
	set_ph_channel();

	HAL_ADC_Start(hadc);
	HAL_ADC_PollForConversion(hadc, 100);
	adPH = HAL_ADC_GetValue(hadc);
	HAL_ADC_Stop(hadc);

	VdropPH = (Vin * adPH) / 1024.0; //converto bit(adPH) in tensione(VdropPH) (precision_ADC = 2^10 -> = 1024)

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

//	char data1[32];
//	sprintf(data1, "PH: %u \n\r", PH);
//	HAL_UART_Transmit(&huart2, (uint8_t*)data1, strlen(data1), HAL_MAX_DELAY);


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
