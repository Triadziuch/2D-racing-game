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

char znak[8] = {0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00};
int it = 0;

/* Private function prototypes -----------------------------------------------*/
static void MX_DataLogFusion_Init(void);
static void MX_DataLogFusion_Process(void);
static void FX_Data_Handler();
static void Init_Sensors(void);
static void RTC_Handler();
static void Accelero_Sensor_Handler();
static void Gyro_Sensor_Handler();
static void Magneto_Sensor_Handler();
static void Pressure_Sensor_Handler();
static void Temperature_Sensor_Handler();
static void Humidity_Sensor_Handler();
static void TIM_Config(uint32_t Freq);
static void DWT_Init(void);
static void DWT_Start(void);
static uint32_t DWT_Stop(void);

void line_append(uint8_t value)
{
	if (value == '\r' || value == '\n') {
		if (line_length > 0) {
			line_buffer[line_length] = '\0';
			strcpy(do_wyswietlenia, line_buffer);
			line_length = 0;
			wyswietl();
		}
	}
	else {
		if (line_length >= LINE_MAX_LENGTH)
			line_length = 0;
		line_buffer[line_length++] = value;
	}
}

void wyswietl()
{
	char dystans[5], punkty[8], predkosc[5], zycia[3], isEnd[3];
	sscanf(do_wyswietlenia, "%s %s %s %s %s", dystans, punkty, predkosc, zycia, isEnd);

	if(isEnd[0] != 0){
		lcd_clear();
		// Punkty
		lcd_print(1, 1, punkty);

		// Dystans
		char linia2[16] = "   km       km/h";
		if (dystans[1] == NULL)
			linia2[2] = dystans[0];
		else if (dystans[2] == NULL){
			linia2[1] = dystans[0];
			linia2[2] = dystans[1];
		}
		else{
			linia2[0] = dystans[0];
			linia2[1] = dystans[1];
			linia2[2] = dystans[2];
		}

		// Predkosc
		if (predkosc[2] == NULL){
			linia2[10] = predkosc[0];
			linia2[11] = predkosc[1];
		}
		else{
			linia2[9] = predkosc[0];
			linia2[10] = predkosc[1];
			linia2[11] = predkosc[2];
		}
		lcd_print(2, 1, linia2);

		// Serca
		int polozenie_serc = 12;
		for(int i = 0; i < atoi(zycia); i++){
			lcd_gotoxy(1, polozenie_serc++);
			lcd_char_cp(0);
		}
	}
	else
		lcd_clear();
}

void SystemClock_Config(void);

uint8_t byte;
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART2){
		line_append(byte);
		HAL_UART_Receive_IT(&huart2, &byte, 1);
	}
}

void uart_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStruct;

	__GPIOA_CLK_ENABLE();

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

	HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(USART2_IRQn);
}

void MX_MEMS_Init(void)
{
	MX_DataLogFusion_Init();

	// Inicjalizacja wyświetlacza
	lcd_init(_LCD_4BIT, _LCD_FONT_5x8, _LCD_2LINE);
	lcd_clear();

	uart_gpio_init();
	uart_init();
	HAL_UART_Receive_IT(&huart2, &byte, 1);

	// Inicjalizacja znaku serca
	lcd_cmd(0x40);

	for (int i=0;i<8;i++)
		lcd_char_cp(znak[i]);
}

void MX_MEMS_Process(void)
{
	MX_DataLogFusion_Process();

	if (mozna_wyswietlic){
		if (it++ % 2 == 0)
			wyswietl();
		mozna_wyswietlic = false;
	}
}

static void MX_DataLogFusion_Process(void)
{
	RTC_Handler();
	Accelero_Sensor_Handler();
	Gyro_Sensor_Handler();
	Magneto_Sensor_Handler();
	Humidity_Sensor_Handler();
	Temperature_Sensor_Handler();
	Pressure_Sensor_Handler();

	FX_Data_Handler();
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == BSP_IP_TIM_Handle.Instance)
		SensorReadRequest = 1;
}

/* Private functions ---------------------------------------------------------*/
static void MX_DataLogFusion_Init(void)
{
	float ans_float;

	BSP_PB_Init(BUTTON_KEY, BUTTON_MODE_EXTI);

	PushButtonState = (BSP_PB_GetState(BUTTON_KEY)) ?  0 : 1;

	BSP_LED_Init(LED2);

	BSP_COM_Init(COM1);

	BSP_IP_TIM_Init();

	TIM_Config(ALGO_FREQ);

	Init_Sensors();

	MotionFX_manager_init();

	MotionFX_manager_get_version(LibVersion, &LibVersionLen);

	MotionFX_manager_MagCal_start(ALGO_PERIOD);

	MFX_MagCal_output_t mag_cal_test;
	MotionFX_MagCal_getParams(&mag_cal_test);

	if (mag_cal_test.cal_quality == MFX_MAGCALGOOD){
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

	UART_StartReceiveMsg();
}



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

static void RTC_Handler()
{
	uint8_t sub_sec = 0;
	RTC_DateTypeDef sdatestructureget;
	RTC_TimeTypeDef stimestructure;
	uint32_t ans_uint32;
	int32_t ans_int32;
	uint32_t RtcSynchPrediv = hrtc.Init.SynchPrediv;

	(void)HAL_RTC_GetTime(&hrtc, &stimestructure, FORMAT_BIN);
	(void)HAL_RTC_GetDate(&hrtc, &sdatestructureget, FORMAT_BIN);
	ans_int32 = (RtcSynchPrediv - (int32_t)stimestructure.SubSeconds) * 100;
	ans_int32 /= RtcSynchPrediv + 1;
	ans_uint32 = (uint32_t)ans_int32 & 0xFFU;
	sub_sec = (uint8_t)ans_uint32;
}

// Formatowanie liczby do ciągu znaków
void f2CharArray(double num, char charArray[])
{
	sprintf(charArray, "%+010.7f", num);
}

static void FX_Data_Handler()
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
				data_in.gyro[0] = (float)GyrValue.x * FROM_MDPS_TO_DPS;
				data_in.gyro[1] = (float)GyrValue.y * FROM_MDPS_TO_DPS;
				data_in.gyro[2] = (float)GyrValue.z * FROM_MDPS_TO_DPS;

				data_in.acc[0] = (float)AccValue.x * FROM_MG_TO_G;
				data_in.acc[1] = (float)AccValue.y * FROM_MG_TO_G;
				data_in.acc[2] = (float)AccValue.z * FROM_MG_TO_G;

				data_in.mag[0] = (float)MagValue.x * FROM_MGAUSS_TO_UT50;
				data_in.mag[1] = (float)MagValue.y * FROM_MGAUSS_TO_UT50;
				data_in.mag[2] = (float)MagValue.z * FROM_MGAUSS_TO_UT50;

				MotionFX_manager_run(pdata_in, pdata_out, MOTION_FX_ENGINE_DELTATIME);

				char r1[20], r2[20];
				f2CharArray((double)pdata_out->rotation[1], r1);
				f2CharArray((double)pdata_out->rotation[2], r2);
				printf("%s %s\r\n\r\n", r1, r2);
			}
		}
	}
}

void BSP_PB_Callback(Button_TypeDef Button)
{
	MagCalRequest = 1U;
}

static void Accelero_Sensor_Handler()
{
	if (ACCELEROMETER_SENSOR == ACCELEROMETER_SENSOR)
		BSP_SENSOR_ACC_GetAxes(&AccValue);
}

static void Gyro_Sensor_Handler()
{
	if (GYROSCOPE_SENSOR == GYROSCOPE_SENSOR){
		DWT_Start();
		BSP_SENSOR_GYR_GetAxes(&GyrValue);
	}
}

static void Magneto_Sensor_Handler()
{
	float ans_float;
	MFX_MagCal_input_t mag_data_in;
	MFX_MagCal_output_t mag_data_out;

	if (MAGNETIC_SENSOR == MAGNETIC_SENSOR){
		BSP_SENSOR_MAG_GetAxes(&MagValue);

		if (MagCalStatus == 0U){
			mag_data_in.mag[0] = (float)MagValue.x * FROM_MGAUSS_TO_UT50;
			mag_data_in.mag[1] = (float)MagValue.y * FROM_MGAUSS_TO_UT50;
			mag_data_in.mag[2] = (float)MagValue.z * FROM_MGAUSS_TO_UT50;
			mag_data_in.time_stamp = (int)TimeStamp;

			TimeStamp += (uint32_t)ALGO_PERIOD;

			MotionFX_manager_MagCal_run(&mag_data_in, &mag_data_out);

			if (mag_data_out.cal_quality == MFX_MAGCALGOOD){
				MagCalStatus = 1;

				ans_float = (mag_data_out.hi_bias[0] * FROM_UT50_TO_MGAUSS);
				MagOffset.x = (int32_t)ans_float;
				ans_float = (mag_data_out.hi_bias[1] * FROM_UT50_TO_MGAUSS);
				MagOffset.y = (int32_t)ans_float;
				ans_float = (mag_data_out.hi_bias[2] * FROM_UT50_TO_MGAUSS);
				MagOffset.z = (int32_t)ans_float;

				MotionFX_manager_MagCal_stop(ALGO_PERIOD);
			}
		}

		MagValue.x = (int32_t)(MagValue.x - MagOffset.x);
		MagValue.y = (int32_t)(MagValue.y - MagOffset.y);
		MagValue.z = (int32_t)(MagValue.z - MagOffset.z);
	}
}

static void Pressure_Sensor_Handler()
{
	if (PRESSURE_SENSOR == PRESSURE_SENSOR)
		BSP_SENSOR_PRESS_GetValue(&PressValue);
}

static void Temperature_Sensor_Handler()
{
	if (TEMPERATURE_SENSOR == TEMPERATURE_SENSOR)
		BSP_SENSOR_TEMP_GetValue(&TempValue);
}

static void Humidity_Sensor_Handler()
{
	if (HUMIDITY_SENSOR == HUMIDITY_SENSOR)
		BSP_SENSOR_HUM_GetValue(&HumValue);
}

static void TIM_Config(uint32_t Freq)
{
	const uint32_t tim_counter_clock = 2000;
	uint32_t prescaler_value = (uint32_t)((SystemCoreClock / tim_counter_clock) - 1);
	uint32_t period = (tim_counter_clock / Freq) - 1;

	BSP_IP_TIM_Handle.Init.Prescaler = prescaler_value;
	BSP_IP_TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
	BSP_IP_TIM_Handle.Init.Period = period;
	BSP_IP_TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	BSP_IP_TIM_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&BSP_IP_TIM_Handle) != HAL_OK)
		Error_Handler();
}

static void DWT_Init(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
}

static void DWT_Start(void)
{
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static uint32_t DWT_Stop(void)
{
	volatile uint32_t cycles_count = 0U;
	uint32_t system_core_clock_mhz = 0U;

	DWT->CTRL &= ~DWT_CTRL_CYCCNTENA_Msk;
	cycles_count = DWT->CYCCNT;

	system_core_clock_mhz = SystemCoreClock / 1000000U;
	return cycles_count / system_core_clock_mhz;
}
#ifdef __cplusplus
}
#endif
