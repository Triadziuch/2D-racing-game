/**
 ******************************************************************************
 * File Name          : app_mems.c
 * Description        : This file provides code for the configuration
 *                      of the STMicroelectronics.X-CUBE-MEMS1.10.0.0 instances.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "app_mems.h"
#include "main.h"
#include <stdio.h>

#include "stm32l4xx_hal.h"
#include "custom.h"
#include "com.h"
#include "demo_serial.h"
#include "bsp_ip_conf.h"
#include "fw_version.h"
#include "motion_fx_manager.h"
#include "lcd.h"
#include "stdbool.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define DWT_LAR_KEY  0xC5ACCE55 /* DWT register unlock key */
#define ALGO_FREQ  100U /* Algorithm frequency 100Hz */
#define ACC_ODR  ((float)ALGO_FREQ)
#define ACC_FS  4 /* FS = <-4g, 4g> */
#define ALGO_PERIOD  (1000U / ALGO_FREQ) /* Algorithm period [ms] */
#define MOTION_FX_ENGINE_DELTATIME  0.01f
#define FROM_MG_TO_G  0.001f
#define FROM_G_TO_MG  1000.0f
#define FROM_MDPS_TO_DPS  0.001f
#define FROM_DPS_TO_MDPS  1000.0f
#define FROM_MGAUSS_TO_UT50  (0.1f/50.0f)
#define FROM_UT50_TO_MGAUSS  500.0f
#define LINE_MAX_LENGTH	80

/* Public variables ----------------------------------------------------------*/
volatile uint8_t DataLoggerActive = 0;
volatile uint32_t SensorsEnabled = 1;
char LibVersion[35];
int LibVersionLen;
volatile uint8_t SensorReadRequest = 0;
uint8_t UseOfflineData = 0;
offline_data_t OfflineData[OFFLINE_DATA_SIZE];
int OfflineDataReadIndex = 0;
int OfflineDataWriteIndex = 0;
int OfflineDataCount = 0;
uint32_t AlgoFreq = ALGO_FREQ;
uint8_t Enabled6X = 0;
static int32_t PushButtonState = GPIO_PIN_RESET;

static char line_buffer[LINE_MAX_LENGTH + 1];
static uint32_t line_length;

/* Extern variables ----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static MOTION_SENSOR_Axes_t AccValue;
static MOTION_SENSOR_Axes_t GyrValue;
static MOTION_SENSOR_Axes_t MagValue;
static float PressValue;
static float TempValue;
static float HumValue;
static volatile uint32_t TimeStamp = 0;
static volatile uint8_t MagCalRequest = 0;
static MOTION_SENSOR_Axes_t MagOffset;
static uint8_t MagCalStatus = 0;

static bool mozna_wyswietlic = false;
static char do_wyswietlenia[LINE_MAX_LENGTH + 1];

static int pomiar_elapsed_time_us, pomiar_time_stamp, pomiar_gettick;
static double pomiar_gyro[3], pomiar_acc[3], pomiar_mag[3];
static double pomiar_quaternion[4], pomiar_rotation[3], pomiar_gravity[3], pomiar_linear_acceleration[3], pomiar_heading, pomiar_headingErr;

/* Private function prototypes -----------------------------------------------*/
static void MX_DataLogFusion_Init(void);
static void MX_DataLogFusion_Process(void);
static void FX_Data_Handler(TMsg *Msg);
static void Init_Sensors(void);
static void RTC_Handler(TMsg *Msg);
static void Accelero_Sensor_Handler(TMsg *Msg);
static void Gyro_Sensor_Handler(TMsg *Msg);
static void Magneto_Sensor_Handler(TMsg *Msg);
static void Pressure_Sensor_Handler(TMsg *Msg);
static void Temperature_Sensor_Handler(TMsg *Msg);
static void Humidity_Sensor_Handler(TMsg *Msg);
static void TIM_Config(uint32_t Freq);
static void DWT_Init(void);
static void DWT_Start(void);
static uint32_t DWT_Stop(void);

void line_append(uint8_t value)
{
	if (value == '\r' || value == '\n') {
		// odebraliśmy znak końca linii
		if (line_length > 0) {
			// jeśli bufor nie jest pusty to dodajemy 0 na końcu linii
			line_buffer[line_length] = '\0';
			// przetwarzamy dane
			strcpy(do_wyswietlenia, line_buffer);
			mozna_wyswietlic = true;
			// zaczynamy zbieranie danych od nowa
			line_length = 0;
		}
	}
	else {
		if (line_length >= LINE_MAX_LENGTH) {
			// za dużo danych, usuwamy wszystko co odebraliśmy dotychczas
			line_length = 0;
		}
		// dopisujemy wartość do bufora
		line_buffer[line_length++] = value;
	}
}

uint8_t byte;

void SystemClock_Config(void);

/* This callback is called by the HAL_UART_IRQHandler when the given number of bytes are received */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART2)
  {
    /* Transmit one byte with 100 ms timeout */
	//lcd_print(1, 1, byte);
	line_append(byte);
    /* Receive one byte in interrupt mode */
    HAL_UART_Receive_IT(&huart2, &byte, 1);
  }
}

void uart_gpio_init()
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __GPIOA_CLK_ENABLE();

  /**USART2 GPIO Configuration
  PA2     ------> USART2_TX
  PA3     ------> USART2_RX
  */
  GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void uart_init()
{
  __USART2_CLK_ENABLE();

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);

  /* Peripheral interrupt init*/
  HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void MX_MEMS_Init(void)
{
	/* USER CODE BEGIN SV */

	/* USER CODE END SV */

	/* USER CODE BEGIN MEMS_Init_PreTreatment */

	/* USER CODE END MEMS_Init_PreTreatment */

	/* Initialize the peripherals and the MEMS components */

	MX_DataLogFusion_Init();

	// Inicjalizacja wyświetlacza
	lcd_init(_LCD_4BIT, _LCD_FONT_5x8, _LCD_2LINE);
	lcd_clear();

	uart_gpio_init();
	uart_init();
	HAL_UART_Receive_IT(&huart2, &byte, 1);
	/* USER CODE BEGIN MEMS_Init_PostTreatment */

	/* USER CODE END MEMS_Init_PostTreatment */
}

/*
 * LM background task
 */
void MX_MEMS_Process(void)
{
	/* USER CODE BEGIN MEMS_Process_PreTreatment */

	/* USER CODE END MEMS_Process_PreTreatment */

	MX_DataLogFusion_Process();

	/* USER CODE BEGIN MEMS_Process_PostTreatment */
	if (mozna_wyswietlic){
		lcd_clear();
		lcd_print(1, 1, do_wyswietlenia);
		mozna_wyswietlic = false;
	}
	/* USER CODE END MEMS_Process_PostTreatment */
}

/* Exported functions --------------------------------------------------------*/
/**
 * @brief  Period elapsed callback
 * @param  htim pointer to a TIM_HandleTypeDef structure that contains
 *              the configuration information for TIM module.
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == BSP_IP_TIM_Handle.Instance)
	{
		SensorReadRequest = 1;
	}
}

/* Private functions ---------------------------------------------------------*/
/**
 * @brief  Initialize the application
 * @retval None
 */

static void MX_DataLogFusion_Init(void)
{
	float ans_float;

	/* Initialize button */
	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	/* Check what is the Push Button State when the button is not pressed. It can change across families */
	PushButtonState = (BSP_PB_GetState(BUTTON_KEY)) ?  0 : 1;

	/* Initialize LED */
	BSP_LED_Init(LED2);

	/* Initialize Virtual COM Port */
	BSP_COM_Init(COM1);

	/* Initialize Timer */
	BSP_IP_TIM_Init();

	/* Configure Timer to run with desired algorithm frequency */
	TIM_Config(ALGO_FREQ);

	/* Initialize (disabled) sensors */
	Init_Sensors();

	/* Sensor Fusion API initialization function */
	MotionFX_manager_init();

	/* OPTIONAL */
	/* Get library version */
	MotionFX_manager_get_version(LibVersion, &LibVersionLen);

	/* Enable magnetometer calibration */
	MotionFX_manager_MagCal_start(ALGO_PERIOD);

	/* Test if calibration data are available */
	MFX_MagCal_output_t mag_cal_test;
	MotionFX_MagCal_getParams(&mag_cal_test);

	/* If calibration data are available load HI coefficients */
	if (mag_cal_test.cal_quality == MFX_MAGCALGOOD)
	{
		ans_float = (mag_cal_test.hi_bias[0] * FROM_UT50_TO_MGAUSS);
		MagOffset.x = (int32_t)ans_float;
		ans_float = (mag_cal_test.hi_bias[1] * FROM_UT50_TO_MGAUSS);
		MagOffset.y = (int32_t)ans_float;
		ans_float = (mag_cal_test.hi_bias[2] * FROM_UT50_TO_MGAUSS);
		MagOffset.z = (int32_t)ans_float;

		MagCalStatus = 1;
	}

	DWT_Init();

	BSP_LED_On(LED2);
	HAL_Delay(500);
	BSP_LED_Off(LED2);

	/* Start receiving messages via DMA */
	UART_StartReceiveMsg();
}

/**
 * @brief  Process of the application
 * @retval None
 */
static void MX_DataLogFusion_Process(void)
{
	static TMsg msg_dat;
	static TMsg msg_cmd;
	static int discarded_count = 0;

	/* Acquire data from enabled sensors and fill Msg stream */
	RTC_Handler(&msg_dat);
	Accelero_Sensor_Handler(&msg_dat);
	Gyro_Sensor_Handler(&msg_dat);
	Magneto_Sensor_Handler(&msg_dat);
	Humidity_Sensor_Handler(&msg_dat);
	Temperature_Sensor_Handler(&msg_dat);
	Pressure_Sensor_Handler(&msg_dat);

	/* Sensor Fusion specific part */
	FX_Data_Handler(&msg_dat);

	/* Send data stream */
	//INIT_STREAMING_HEADER(&msg_dat);
	//msg_dat.Len = STREAMING_MSG_LENGTH;

	if (UseOfflineData == 1U)
	{
		OfflineDataCount--;
		if (OfflineDataCount < 0)
		{
			OfflineDataCount = 0;
		}

		OfflineDataReadIndex++;
		if (OfflineDataReadIndex >= OFFLINE_DATA_SIZE)
		{
			OfflineDataReadIndex = 0;
		}

		if (OfflineDataCount > 0)
		{
			SensorReadRequest = 1;
		}
	}

	if (discarded_count >= SAMPLETODISCARD)
	{
		//UART_SendMsg(&msg_dat);
	}
	else
	{
		discarded_count++;
	}
}

/**
 * @brief  Initialize all sensors
 * @param  None
 * @retval None
 */
static void Init_Sensors(void)
{
	BSP_SENSOR_ACC_Init();
	BSP_SENSOR_GYR_Init();
	BSP_SENSOR_MAG_Init();
	BSP_SENSOR_PRESS_Init();
	BSP_SENSOR_TEMP_Init();
	BSP_SENSOR_HUM_Init();

	BSP_SENSOR_ACC_SetOutputDataRate(ACC_ODR);
	BSP_SENSOR_ACC_SetFullScale(ACC_FS);
}

/**
 * @brief  Handles the time+date getting/sending
 * @param  Msg the time+date part of the stream
 * @retval None
 */
static void RTC_Handler(TMsg *Msg)
{
	uint8_t sub_sec = 0;
	RTC_DateTypeDef sdatestructureget;
	RTC_TimeTypeDef stimestructure;
	uint32_t ans_uint32;
	int32_t ans_int32;
	uint32_t RtcSynchPrediv = hrtc.Init.SynchPrediv;

	if (UseOfflineData == 1)
	{
		Msg->Data[3] = (uint8_t)OfflineData[OfflineDataReadIndex].hours;
		Msg->Data[4] = (uint8_t)OfflineData[OfflineDataReadIndex].minutes;
		Msg->Data[5] = (uint8_t)OfflineData[OfflineDataReadIndex].seconds;
		Msg->Data[6] = (uint8_t)OfflineData[OfflineDataReadIndex].subsec;
	}
	else
	{
		(void)HAL_RTC_GetTime(&hrtc, &stimestructure, FORMAT_BIN);
		(void)HAL_RTC_GetDate(&hrtc, &sdatestructureget, FORMAT_BIN);

		/* To be MISRA C-2012 compliant the original calculation:
       sub_sec = ((((((int)RtcSynchPrediv) - ((int)stimestructure.SubSeconds)) * 100) / (RtcSynchPrediv + 1)) & 0xFF);
       has been split to separate expressions */
		ans_int32 = (RtcSynchPrediv - (int32_t)stimestructure.SubSeconds) * 100;
		ans_int32 /= RtcSynchPrediv + 1;
		ans_uint32 = (uint32_t)ans_int32 & 0xFFU;
		sub_sec = (uint8_t)ans_uint32;

		Msg->Data[3] = (uint8_t)stimestructure.Hours;
		Msg->Data[4] = (uint8_t)stimestructure.Minutes;
		Msg->Data[5] = (uint8_t)stimestructure.Seconds;
		Msg->Data[6] = sub_sec;
	}
}

/**
 * @brief  Sensor Fusion data handler
 * @param  Msg the Sensor Fusion data part of the stream
 * @retval None
 */

void f2CharArray(double num, char charArray[]) {
	// Formatowanie liczby do ciągu znaków
	sprintf(charArray, "%+010.7f", num);
}
static void FX_Data_Handler(TMsg *Msg)
{
	uint32_t elapsed_time_us = 0U;
	MFX_input_t data_in;
	MFX_input_t *pdata_in = &data_in;
	MFX_output_t data_out;
	MFX_output_t *pdata_out = &data_out;

	if (ACCELEROMETER_SENSOR == ACCELEROMETER_SENSOR)
	{
		if (GYROSCOPE_SENSOR == GYROSCOPE_SENSOR)
		{
			if (MAGNETIC_SENSOR == MAGNETIC_SENSOR)
			{
				/* Convert angular velocity from [mdps] to [dps] */
				data_in.gyro[0] = (float)GyrValue.x * FROM_MDPS_TO_DPS;
				data_in.gyro[1] = (float)GyrValue.y * FROM_MDPS_TO_DPS;
				data_in.gyro[2] = (float)GyrValue.z * FROM_MDPS_TO_DPS;

				/* Convert acceleration from [mg] to [g] */
				data_in.acc[0] = (float)AccValue.x * FROM_MG_TO_G;
				data_in.acc[1] = (float)AccValue.y * FROM_MG_TO_G;
				data_in.acc[2] = (float)AccValue.z * FROM_MG_TO_G;

				/* Convert magnetic field intensity from [mGauss] to [uT / 50] */
				data_in.mag[0] = (float)MagValue.x * FROM_MGAUSS_TO_UT50;
				data_in.mag[1] = (float)MagValue.y * FROM_MGAUSS_TO_UT50;
				data_in.mag[2] = (float)MagValue.z * FROM_MGAUSS_TO_UT50;

				/* Run Sensor Fusion algorithm */
				MotionFX_manager_run(pdata_in, pdata_out, MOTION_FX_ENGINE_DELTATIME);

				char r1[20], r2[20];
				f2CharArray((double)pdata_out->rotation[1], r1);
				f2CharArray((double)pdata_out->rotation[2], r2);
				printf("%s %s\r\n\r\n", r1, r2);
			}
		}
	}
}
/**
 * @brief  BSP Push Button callback
 * @param  Button Specifies the pin connected EXTI line
 * @retval None.
 */
void BSP_PB_Callback(Button_TypeDef Button)
{
	MagCalRequest = 1U;
}

/**
 * @brief  Handles the ACC axes data getting/sending
 * @param  Msg the ACC part of the stream
 * @retval None
 */
static void Accelero_Sensor_Handler(TMsg *Msg)
{
	if (ACCELEROMETER_SENSOR == ACCELEROMETER_SENSOR){
		if (UseOfflineData == 1){
			AccValue.x = OfflineData[OfflineDataReadIndex].acceleration_x_mg;
			AccValue.y = OfflineData[OfflineDataReadIndex].acceleration_y_mg;
			AccValue.z = OfflineData[OfflineDataReadIndex].acceleration_z_mg;
		}
		else{
			BSP_SENSOR_ACC_GetAxes(&AccValue);
		}
	}
}

/**
 * @brief  Handles the GYR axes data getting/sending
 * @param  Msg the GYR part of the stream
 * @retval None
 */
static void Gyro_Sensor_Handler(TMsg *Msg)
{
	if (GYROSCOPE_SENSOR == GYROSCOPE_SENSOR)
	{
		if (UseOfflineData == 1)
		{
			GyrValue.x = OfflineData[OfflineDataReadIndex].angular_rate_x_mdps;
			GyrValue.y = OfflineData[OfflineDataReadIndex].angular_rate_y_mdps;
			GyrValue.z = OfflineData[OfflineDataReadIndex].angular_rate_z_mdps;
		}
		else
		{
			DWT_Start();
			BSP_SENSOR_GYR_GetAxes(&GyrValue);
		}
	}
}

/**
 * @brief  Handles the MAG axes data getting/sending
 * @param  Msg the MAG part of the stream
 * @retval None
 */
static void Magneto_Sensor_Handler(TMsg *Msg)
{
	float ans_float;
	MFX_MagCal_input_t mag_data_in;
	MFX_MagCal_output_t mag_data_out;

	if (MAGNETIC_SENSOR == MAGNETIC_SENSOR)
	{
		if (UseOfflineData == 1)
		{
			MagValue.x = OfflineData[OfflineDataReadIndex].magnetic_field_x_mgauss;
			MagValue.y = OfflineData[OfflineDataReadIndex].magnetic_field_y_mgauss;
			MagValue.z = OfflineData[OfflineDataReadIndex].magnetic_field_z_mgauss;
		}
		else
		{
			BSP_SENSOR_MAG_GetAxes(&MagValue);

			if (MagCalStatus == 0U)
			{
				mag_data_in.mag[0] = (float)MagValue.x * FROM_MGAUSS_TO_UT50;
				mag_data_in.mag[1] = (float)MagValue.y * FROM_MGAUSS_TO_UT50;
				mag_data_in.mag[2] = (float)MagValue.z * FROM_MGAUSS_TO_UT50;

				pomiar_time_stamp = TimeStamp;
				mag_data_in.time_stamp = (int)TimeStamp;
				TimeStamp += (uint32_t)ALGO_PERIOD;

				MotionFX_manager_MagCal_run(&mag_data_in, &mag_data_out);

				if (mag_data_out.cal_quality == MFX_MAGCALGOOD)
				{
					MagCalStatus = 1;

					ans_float = (mag_data_out.hi_bias[0] * FROM_UT50_TO_MGAUSS);
					MagOffset.x = (int32_t)ans_float;
					ans_float = (mag_data_out.hi_bias[1] * FROM_UT50_TO_MGAUSS);
					MagOffset.y = (int32_t)ans_float;
					ans_float = (mag_data_out.hi_bias[2] * FROM_UT50_TO_MGAUSS);
					MagOffset.z = (int32_t)ans_float;

					/* Disable magnetometer calibration */
					MotionFX_manager_MagCal_stop(ALGO_PERIOD);
				}
			}

			MagValue.x = (int32_t)(MagValue.x - MagOffset.x);
			MagValue.y = (int32_t)(MagValue.y - MagOffset.y);
			MagValue.z = (int32_t)(MagValue.z - MagOffset.z);
		}
	}
}

/**
 * @brief  Handles the PRESS sensor data getting/sending.
 * @param  Msg the PRESS part of the stream
 * @retval None
 */
static void Pressure_Sensor_Handler(TMsg *Msg)
{
	if (PRESSURE_SENSOR == PRESSURE_SENSOR)
	{
		if (UseOfflineData == 1)
		{
			PressValue = OfflineData[OfflineDataReadIndex].pressure;
		}
		else
		{
			BSP_SENSOR_PRESS_GetValue(&PressValue);
		}

		(void)memcpy(&Msg->Data[7], (void *)&PressValue, sizeof(float));
	}
}

/**
 * @brief  Handles the TEMP axes data getting/sending
 * @param  Msg the TEMP part of the stream
 * @retval None
 */
static void Temperature_Sensor_Handler(TMsg *Msg)
{
	if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR)
	{
		if (UseOfflineData == 1)
		{
			TempValue = OfflineData[OfflineDataReadIndex].temperature;
		}
		else
		{
			BSP_SENSOR_TEMP_GetValue(&TempValue);
		}

		(void)memcpy(&Msg->Data[11], (void *)&TempValue, sizeof(float));
	}
}

/**
 * @brief  Handles the HUM axes data getting/sending
 * @param  Msg the HUM part of the stream
 * @retval None
 */
static void Humidity_Sensor_Handler(TMsg *Msg)
{
	if (HUMIDITY_SENSOR == HUMIDITY_SENSOR)
	{
		if (UseOfflineData == 1)
		{
			HumValue = OfflineData[OfflineDataReadIndex].humidity;
		}
		else
		{
			BSP_SENSOR_HUM_GetValue(&HumValue);
		}

		(void)memcpy(&Msg->Data[15], (void *)&HumValue, sizeof(float));;
	}
}

/**
 * @brief  Timer configuration
 * @param  Freq the desired Timer frequency
 * @retval None
 */
static void TIM_Config(uint32_t Freq)
{
	const uint32_t tim_counter_clock = 2000; /* TIM counter clock 2 kHz */
	uint32_t prescaler_value = (uint32_t)((SystemCoreClock / tim_counter_clock) - 1);
	uint32_t period = (tim_counter_clock / Freq) - 1;

	BSP_IP_TIM_Handle.Init.Prescaler = prescaler_value;
	BSP_IP_TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	BSP_IP_TIM_Handle.Init.Period = period;
	BSP_IP_TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	BSP_IP_TIM_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&BSP_IP_TIM_Handle) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief  Initialize DWT register for counting clock cycles purpose
 * @param  None
 * @retval None
 */
static void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; /* Disable counter */
}

/**
 * @brief  Start counting clock cycles
 * @param  None
 * @retval None
 */
static void DWT_Start(void)
{
	DWT->CYCCNT = 0; /* Clear count of clock cycles */
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; /* Enable counter */
}

/**
 * @brief  Stop counting clock cycles and calculate elapsed time in [us]
 * @param  None
 * @retval Elapsed time in [us]
 */
static uint32_t DWT_Stop(void)
{
	volatile uint32_t cycles_count = 0U;
	uint32_t system_core_clock_mhz = 0U;

	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk; /* Disable counter */
	cycles_count = DWT->CYCCNT; /* Read count of clock cycles */

	/* Calculate elapsed time in [us] */
	system_core_clock_mhz = SystemCoreClock / 1000000U;
	return cycles_count / system_core_clock_mhz;
}

int get_pomiar_elapsed_time_us(void)		{return pomiar_elapsed_time_us;};
int get_pomiar_time_stamp(void)				{return pomiar_time_stamp;};
int get_pomiar_gettick(void)				{return pomiar_gettick;};
double *get_pomiar_gyro(void)				{return pomiar_gyro;};
double *get_pomiar_acc(void)				{return pomiar_acc;};
double *get_pomiar_mag(void)				{return pomiar_mag;};
double *get_pomiar_quaternion(void)			{return pomiar_quaternion;};
double *get_pomiar_rotation(void)			{return pomiar_rotation;};
double *get_pomiar_gravity(void)			{return pomiar_gravity;};
double *get_pomiar_linear_acceleration(void){return pomiar_linear_acceleration;};
double get_pomiar_heading(void)				{return pomiar_heading;};
double get_pomiar_headingErr(void)			{return pomiar_headingErr;};

#ifdef __cplusplus
}
#endif
