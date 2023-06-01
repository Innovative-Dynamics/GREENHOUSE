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
double somma = 0;
double media = 0;

_Bool reading_ec = 0;
int reading_ph = 0;

int ph_read_value = 0;

_Bool is_counting_ec = 0;
_Bool is_reading = 0;

_Bool ec_initialized = 0;

int ec_value_readed = 0;

_Bool ph_value_readed = 0;


extern long long int time_prev;


_Bool is_ec_initialized ()
{
	return ec_initialized;
}


void reset_ec_initialized ()
{
	ec_initialized = 0;
}


_Bool read_ec ()
{
	return reading_ec;
}


void set_adEC (float adEC_val)
{
	adEC = adEC_val;
}


_Bool is_ec_value_readed ()
{
	return ec_value_readed;
}


void reset_is_ec_value_readed ()
{
	ec_value_readed = 0;
}


_Bool read_ph ()
{
	return reading_ph;
}


void set_adPH (float adPH_val)
{
	adPH = adPH_val;
}


_Bool is_ph_value_readed ()
{
	return ph_value_readed;
}


void reset_is_ph_value_readed ()
{
	ph_value_readed = 0;
}


float get_EC ()
{
	return EC;
}


float get_PH ()
{
	return PH;
}


void ec_init ()
{
	if (is_counting_ec == 0)
	{
		time_prev = __HAL_TIM_GET_COUNTER(&htim2);
		is_counting_ec = 1;
		HAL_GPIO_WritePin(EC_WRITE_GPIO_Port, EC_WRITE_Pin, GPIO_PIN_SET); // Imposto il pin ECPower(PC0 -> A5) a livello alto: 3.3v
	}
	else
	{

		if (__HAL_TIM_GET_COUNTER(&htim2) - time_prev >= 6000000)
		{
			is_counting_ec = 0;
			ec_initialized = 1;
		}
	}
}


//void ec_read (ADC_HandleTypeDef *hadc)
//{
//	set_ec_channel();
//
////	reading_ec = 1;
////	reading_ph = 0;
////
////	HAL_ADC_Start_IT(hadc);
//
//	HAL_ADC_Start(&hadc3);
//	HAL_ADC_PollForConversion(&hadc3, 100);
//	adEC = HAL_ADC_GetValue(&hadc3);
////	HAL_ADC_Stop(&hadc3);
//
//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
//
//	VdropEC= (Vin * adEC) / 1024; //converto tot bit(adEC) in tensione(VdropEC) (precision_ADC = 2^10 -> = 1024)
//	RWater = (VdropEC * R1) / (Vin - VdropEC);
//	EC = 1000 / (RWater * K1); //mS/cm
//
//	// GESTIONE DATI EC
//	if (EC <= 0.9)
//	{
//		EC = EC - 0.3;
//	}
//	else if (EC > 1.5 && EC <= 1.6)
//	{
//		EC = EC + 0.25;
//	}
//	else if (EC > 1.6 && EC <= 1.7)
//	{
//		EC = EC + 0.35;
//	}
//
//	char data[32];
//	sprintf(data, "EC: %f \n\r", EC);
//
//	HAL_UART_Transmit(&huart2, (uint8_t*)data, strlen(data), HAL_MAX_DELAY);
//
//	ec_value_readed = 1;
//}

// Funzionamento EC meter
void ec_read(ADC_HandleTypeDef *hadc)
{ //funzione dichiarata nel .h e chiamata dal main

//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET); //Imposto il pin ECPower(PC0 -> A5) a livello alto: 3.3v
//
//	HAL_Delay(6000); //Attendo che il PC0 si stabilizzi a 3.3v

	// Seleziona il canale ADC desiderato (CHANNEL2)
	set_ec_channel();

//	HAL_ADC_Start_IT(hadc);

	HAL_ADC_Start(&hadc3); // avvia la conversione ADC da EC_Read(PC1 -> A4)
	HAL_ADC_PollForConversion(&hadc3, 100); // attendi la fine della conversione
	adEC = HAL_ADC_GetValue(&hadc3);//Leggo il valore analog

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET); //Imposto il pin ECPower(PC0 -> A5) a livello basso: 0v

	VdropEC= (Vin *adEC) / 1024.0; //converto tot bit(adEC) in tensione(VdropEC) (precision_ADC = 2^10 -> = 1024)
	RWater = (VdropEC*R1) / (Vin-VdropEC); // prova ad aumentare la precisione a 2^12
	EC = 1000/ (RWater*K1); //mS/cm

    // GESTIONE DATI EC
//	if (EC <= 0.9)
//	{
//	    EC = EC - 0.3;
//	}
//	else if (EC > 1.5 && EC <= 1.6)
//	{
//	    EC = EC + 0.25;
//	}
//	else if (EC > 1.6 && EC <= 1.7)
//	{
//	    EC = EC + 0.35;
//	}
//
//	HAL_Delay(2000); // attendo che il PA7 si stabilizzi a 0v
//
//	return EC;
	ec_value_readed = 1;
}

void ph_read (ADC_HandleTypeDef *hadc)
{
	set_ph_channel();

//	reading_ec = 0;
//	reading_ph = 1;

//	HAL_ADC_Start_IT(hadc); // avvia la conversione ADC da EC_Read(PC1 -> A4)

	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, 100);
	adPH = HAL_ADC_GetValue(&hadc3);
//	HAL_ADC_Stop(&hadc3);

	VdropPH = (Vin * adPH) / 1024.0; //converto bit(adPH) in tensione(VdropPH) (precision_ADC = 2^10 -> = 1024)

	if (VdropPH == 0)
		PH = 0;
	else if (VdropPH > 0 && VdropPH <= ph_interval)
		PH = 1;
	else if (VdropPH > ph_interval && VdropPH <= ph_interval*2)
		PH = 2;
	else if (VdropPH > ph_interval*2 && VdropPH <= ph_interval*3)
		PH = 3;
	else if (VdropPH > ph_interval*3 && VdropPH <= ph_interval*4)
		PH = 4;
	else if (VdropPH > ph_interval*4 && VdropPH <= ph_interval*5)
		PH = 5;
	else if (VdropPH > ph_interval*5 && VdropPH <= ph_interval*6)
		PH = 6;
	else if (VdropPH > ph_interval*6 && VdropPH <= ph_interval*7)
		PH = 7;
	else if (VdropPH > ph_interval*7 && VdropPH <= ph_interval*8)
		PH = 8;
	else if (VdropPH > ph_interval*8 && VdropPH <= ph_interval*9)
		PH = 9;
	else if (VdropPH > ph_interval*9 && VdropPH <= ph_interval*10)
		PH = 10;
	else if (VdropPH > ph_interval*10 && VdropPH <= ph_interval*11)
		PH = 11;
	else if (VdropPH > ph_interval*11 && VdropPH <= ph_interval*12)
		PH = 12;
	else if (VdropPH > ph_interval*12 && VdropPH <= ph_interval*13)
		PH = 13;
	else
	{
		PH = 14;
	}

	ph_value_readed = 1;
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
  sConfig.Channel = ADC_CHANNEL_2;
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
  sConfig.Channel = ADC_CHANNEL_3;
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
