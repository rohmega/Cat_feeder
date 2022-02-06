/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f4xx_hal.h"
#include "spi_ili9341.h"
#include "stdio.h"
#include "string.h"
#include "interface.h"
#include "stdbool.h"
#include "stdint.h"
#include "consolidate.h"
#include "stm32_ds3231.h"
#include "tone.h"
#include "w25qxx.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;
TIM_HandleTypeDef htim10;
TIM_HandleTypeDef htim11;

/* USER CODE BEGIN PV */
uint8_t buffer1[8]={1,2,3,4,5,6,7,8};
uint8_t buffer2[8];

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM3_Init(void);
static void MX_I2C1_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM10_Init(void);
static void MX_TIM11_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
//int A=0;
//int B=0;

short feed_time[4][2];
short feed_amount[4];
extern short update;

bool feed_status=1;
short feed_number=3;
uint8_t curr_day=0;
uint8_t prev_day=40;


long text_colour;

short prev_menu=-1;

short menu=0;
short sub_menu=-1;

short prev_sub_menu=-1;
uint16_t screen_brightness=32000;
long speed=4000; //2500

bool time_switch=0;
long time_switch_old;


bool feeding=false;
bool A_state=false;
bool B_state=true;
short selection,prev_selection;
short selection_wheel=-1;
bool sound_feed=1;
bool food_shake=1;
short sound_intensity=1; //0 uit 1 zacht,2 middel, 3 hard
short time_out=4; //0=10s, 1=20s, 30s, 1min, 5min,
bool refresh=0;
short feed_cal=5; //hoeveelheid voer calibratie
uint16_t sector_increment=65535;
uint8_t curr_sector=1;
long timeout_old=0;
long timeout_time=0;

long time_check_old=0;

short curr_time[2][2];

short skip_meal=0;

// external variables:
bool pressed;
bool button_pressed=false;
bool button_long_pressed=false;

extern short skip;

bool screen_off=0;

uint16_t time_now,time_old,time_diff;
long tone_old,tone_now,tone_diff;


_RTC rtc = {
		.Year = 21, .Month = 6, .Date = 24,
		.DaysOfWeek = THURSDAY,
		.Hour = 21, .Min = 6, .Sec = 00
};

//_RTC rtc;

uint8_t buffer[22];

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */


int main(void)
{
	/* USER CODE BEGIN 1 */
	HAL_GPIO_WritePin(BLED_GPIO_Port,BLED_Pin,GPIO_PIN_SET);
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	//HAL_TIM_Base_Start_IT(&htim1);

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_SPI1_Init();
	MX_TIM1_Init();
	MX_SPI2_Init();
	MX_TIM2_Init();
	MX_TIM3_Init();
	MX_I2C1_Init();
	MX_TIM4_Init();
	MX_TIM10_Init();
	MX_TIM11_Init();
	/* USER CODE BEGIN 2 */



	TFT9341_ini(240, 320);
	TFT9341_SetRotation(1);
	TFT9341_FillScreen(0x000);


	//extern short feed_time[4][2];
	feed_time[0][0]=8;
	feed_time[0][1]=0;
	feed_time[1][0]=13;
	feed_time[1][1]=1;
	feed_time[2][0]=18;
	feed_time[2][1]=0;
	feed_time[3][0]=0;
	feed_time[3][1]=10;
	//short feed_amount[4];
	//char curr_day[2];
	//short curr_time[2];
	//long text_colour;
	//short feed_status=1;
	//curr_time[0]=19;
	//curr_time[1]=47;
	//short feed_number=1;
	feed_amount[0]=7;
	feed_amount[1]=10;
	feed_amount[2]=10;
	feed_amount[3]=7;
	prev_selection=-1;



	pressed=false;
	char str[5];

	button_pressed=false;

	uint16_t time_now,time_old,time_diff;



	//draw_main(feed_time,feed_amount,curr_day,selection,text_colour,feed_status,feed_number);  //void draw_main(short feed_time[4][2],short feed_amount[4][1],char curr_day[2],short curr_time[2],short selection,long text_colour,short feed_status)
	/*
	int hour,min;
	hour=23;
	min=14;
	 */
	GPIO_PinState SW_state=GPIO_PIN_RESET;
	//HAL_GPIO_WritePin(BLED_GPIO_Port,BLED_Pin,GPIO_PIN_RESET);
	//char curr_time [50];
	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_Base_Start(&htim2);
	HAL_TIM_Base_Start(&htim3);
	HAL_TIM_Base_Start(&htim4);
	HAL_TIM_Base_Start(&htim10);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//TIM1->CCR1=500;
	DS3231_Init(&hi2c1);

	//TFT9341_DrawRect(0xffff,30,70,130,170);
	//button(100,100,4);
	//test(150,100);
	//draw_power();
	//draw_main(feed_time,feed_amount,curr_day,selection,text_colour,feed_status,feed_number);
	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	//	short count=0;
	HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET); //RESET is turn on
	HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_SET); //set is turn on
	HAL_GPIO_WritePin(RST_GPIO_Port,RST_Pin,GPIO_PIN_SET); //set is turn on
	HAL_GPIO_WritePin(MS1_GPIO_Port,MS1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MS2_GPIO_Port,MS2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(MS3_GPIO_Port,MS3_Pin,GPIO_PIN_SET);
	menu=0;
	sub_menu=-1;
	skip=0;

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);

	//sub_menu=0;

	//DS3231_SetTime(&rtc);
	HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
	//play_tune();
	//play_tune();
	//play_tune();
	//play_tune();
	//play_tune();
	HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
	W25qxx_Init();
	//W25qxx_EraseSector(1);
	//W25qxx_WriteSector(buffer1, 1, 0, 8);


	/*

	write_vars(buffer);

	for(int i=0;i<22;i++)
	{
		TFT9341_SetFont(&Font8);
		sprintf(str,"%d",buffer[i]);
		TFT9341_String(50,10*i,str);
		sprintf(str,"%d",i);
				TFT9341_String(5,10*i,str);
		buffer[i]=0;
	}
	read_vars();
	 */

	switch(time_out)
	{
	case 0:
		timeout_time=10000; // in ms
		break;
	case 1:
		timeout_time=20000; // in ms
		break;
	case 2:
		timeout_time=30000; // in ms
		break;
	case 3:
		timeout_time=60000; // in ms
		break;
	case 4:
		timeout_time=300000; // in ms
		break;
	}
	read_vars();
	DS3231_GetTime(&rtc);
	curr_day=rtc.DaysOfWeek;
	prev_day=curr_day;
	play_tune();
	//HAL_GPIO_WritePin(BLED_GPIO_Port,BLED_Pin,GPIO_PIN_SET);
	//feed_cat(1000);
	TIM1->CCR1=screen_brightness;
	HAL_GPIO_WritePin(BLED_GPIO_Port,BLED_Pin,GPIO_PIN_RESET);
	while (1)
	{
		menu_draw(); //takes care of all drawing actions on the screen
		selection_action();



		//W25qxx_ReadSector(buffer2, 1, 0, 8);
		//play_tone("a4");

		//play_tone("b3",1000);
		//play_tone("c3",1000);
		//play_tone("d3",1000);
		//HAL_GPIO_WritePin(BLED_GPIO_Port,BLED_Pin,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(LED2_GPIO_Port,LED2_Pin,GPIO_PIN_SET);

		//speed=2500;
		//motor_intensity(0);

		/*
		while(1)
		{
			sound_feed=1;
			food_shake=0;
			feed_cat(10000);
			HAL_Delay(10000);
		}
		*/

		//speed=65000;
		//motor_intensity(2);
		//feed_cat(5000);


		if(TIM2->CNT-timeout_old>timeout_time)
		{
			//scherm uitdoen
			screen_off=1;
			TFT9341_FillScreen(0x0000);
			TIM1->CCR1=0;
		}
		else if(screen_off==1)
		{
			TIM1->CCR1=screen_brightness;
			screen_off=0;
			update=1;
		}



		// check de voertijden elke 10sec
		if(TIM2->CNT-time_check_old>10000 && (sub_menu!=0 || menu!=1 || sub_menu!=2))
		{
			DS3231_GetTime(&rtc); //get the current time
			time_check_old=TIM2->CNT;
			feed_cat_time();
		}

		/*
		DS3231_GetTime(&rtc);
		TFT9341_SetTextColor(0xffff);
		TFT9341_SetFont(&Font16);

		char str[5];

		curr_day=rtc.DaysOfWeek;
		sprintf(str,"%d",rtc.DaysOfWeek);
		 */


		//TFT9341_String(250,15,str);

		/*
		TFT9341_SetFont(&Font16);
		TFT9341_String(250,15,time_text(rtc.Hour,rtc.Min));
		 */


		// alle switch-case eigenlijk hier voor functies



		/*

		for(int i=0;i<22;i++)
		{
			TFT9341_SetFont(&Font8);
			sprintf(str,"%d",buffer[i]);
			TFT9341_String(5,5+10*i,str);
			buffer[i]=0;
		}
		read_vars();
		for(int i=0;i<22;i++)
		{
			TFT9341_SetFont(&Font8);
			sprintf(str,"%d",buffer[i]);
			TFT9341_String(5,5+10*i,str);
		}
		 */

		curr_day=rtc.DaysOfWeek;


		//draw_selection();

		/*
		sprintf(str,"%d",sub_menu);
		TFT9341_String(250,50,str);
		sprintf(str,"%d",menu);
		TFT9341_String(250,70,str);
		sprintf(str,"%d",selection_wheel);
		TFT9341_String(250,90,str);
		sprintf(str,"%d",skip);
		TFT9341_String(250,110,str);
		 */


		/*
		if(TIM3->CNT>9623)
		{

			HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
			TIM3->CNT = 0;
			count++;
			//HAL_GPIO_WritePin(DIR_GPIO_Port,DIR_Pin,GPIO_PIN_SET);
		}
		if(count==2)
		{
			HAL_GPIO_TogglePin(DIR_GPIO_Port,DIR_Pin);
			count=0;
		}
		 */





		time_now=TIM10->CNT;
		time_diff=time_now-time_old;

		tone_now=TIM4->CNT;
		tone_diff=tone_now-tone_old;
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */

		// CHECK BUTTON PRESS
		if(pressed)
		{
			// keep track of elapsed time for pausing (use tim2)
			// also sleep etc. 
			//play_tune();
		}
		if(button_long_pressed)
		{
			//HAL_GPIO_TogglePin(EN_GPIO_Port,EN_Pin);
			button_long_pressed=false;
			//feed_cat(10000);
		}



		//if(__HAL_TIM_GetCounter(&htim2)>=100)
		//{


		// 	------------------------
		//	BUTTON handling routine
		//
		if(HAL_GPIO_ReadPin(SW_GPIO_Port,SW_Pin)==GPIO_PIN_RESET)  //ingedrukt
		{
			if(SW_state==GPIO_PIN_RESET)
			{
				time_old=TIM10->CNT;
			}
			time_diff=TIM10->CNT-time_old;
			if(time_diff>1000)
			{
				TIM3->CNT=0;
				HAL_GPIO_WritePin(DIR_GPIO_Port, DIR_Pin, GPIO_PIN_RESET);

				HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_SET);
				HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET);
				while(HAL_GPIO_ReadPin(SW_GPIO_Port,SW_Pin)==GPIO_PIN_RESET)
				{
					if(TIM3->CNT>speed)
					{
						TIM3->CNT=0;
						motor_intensity(0);
						HAL_GPIO_TogglePin(STEP_GPIO_Port,STEP_Pin);
					}
				}
				HAL_GPIO_WritePin(SLEEP_GPIO_Port,SLEEP_Pin,GPIO_PIN_RESET);
				HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET);
			}
			//sprintf(str,"%d   ",time_diff);
			//TFT9341_String(220, 5, str);
			//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
			if(SW_state==GPIO_PIN_RESET)
			{

			}
			SW_state=GPIO_PIN_SET;

		}
		if(HAL_GPIO_ReadPin(SW_GPIO_Port,SW_Pin)==GPIO_PIN_SET)  //weer losgelaten
		{
			time_diff=TIM10->CNT-time_old;
			if(SW_state==GPIO_PIN_SET)
			{

				//time_old=TIM10->CNT;

				if(time_diff>80 && time_diff<1000 && button_pressed==false)
				{
					HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
					button_pressed=true;

					if(sound_intensity!=0)
					{
						motor_intensity(sound_intensity);
						play_tone("b4",35);
					}

					timeout_old=TIM2->CNT;
				}
				if((time_diff)>1000)
				{
					button_long_pressed=true;
					//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
				}
			}
			//time_old=TIM10->CNT;
			//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
			//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin);
			SW_state=GPIO_PIN_RESET; //één keer achter elkaar
		}
		// ------------------------




		//draw_main(feed_time,feed_amount,curr_day,selection,text_colour,feed_status,feed_number);  //void draw_main(short feed_time[4][2],short feed_amount[4][1],char curr_day[2],short curr_time[2],short selection,long text_colour,short feed_status)
		/*
		if(feed_number<4)
		{
			feed_number++;
		}
		else
		{
			feed_number=1;
		}
		 */


		if(time_switch==0 && menu==0 && screen_off==0)
		{
			if(TIM2->CNT-time_switch_old>1000 && menu==0)
			{
				DS3231_GetTime(&rtc);
				time_switch_old=TIM2->CNT;
				time_switch=1;
				TFT9341_SetTextColor(0xFE0A);
				TFT9341_SetFont(&Font20);
				TFT9341_String(235, 15,time_text(rtc.Hour, rtc.Min, 1));
			}
		}
		else
		{
			if(TIM2->CNT-time_switch_old>1000 && menu==0 && screen_off==0)
			{
				time_switch=0;
				TFT9341_SetTextColor(0xFE0A);
				TFT9341_SetFont(&Font20);
				TFT9341_String(235, 15,time_text(rtc.Hour, rtc.Min, 0));
				time_switch_old=TIM2->CNT;
			}

		}



		/*
		// KEEP TIME seperate
		if(menu==0&&feeding==false)
		{
			TFT9341_SetFont(&Font20);
			TFT9341_SetTextColor(0xFFFF);
			char str[4];
			if(TIM2->CNT>5000&&feeding==false)
			{
				TFT9341_SetFont(&Font20);

				sprintf(str,"%d:%d",curr_time[0],curr_time[1]);
				TFT9341_String(250,15,str);
			}
			else if(feeding==false)
			{

				sprintf(str,"%d %d",curr_time[0],curr_time[1]);
				TFT9341_String(250,15,str);
			}
		}

		 */

		/*
		//sprintf(str,"%d",selection);
		sprintf(str,"%d",time_old);
		TFT9341_String(250,50,str);
		//sprintf(str,"%d",menu);

		sprintf(str,"%d",menu);
		TFT9341_String(250,65,str);
		 */



		/*
		sprintf(str,"%d",time_diff);
		TFT9341_String(250,80,str);
		sprintf(str,"%d",button_pressed);
		TFT9341_String(250,100,str);
		sprintf(str,"%d",button_long_pressed);
		TFT9341_String(250,180,str);
		 */



		pressed=false;
	}





	/*
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET);
		HAL_Delay(500);
		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET);
		HAL_Delay(500);
	 */


	/*
				sprintf(curr_time,"%d:%d",hour,min);
		TFT9341_String(100,100,curr_time);
		HAL_Delay(1000);
		sprintf(curr_time,"%d %d",hour,min);
		TFT9341_String(100,100,curr_time);
		HAL_Delay(1000);

	 */
	//sprintf(curr_time,"%d",__HAL_TIM_GetCounter(&htim1));
	//TFT9341_String(100,100,curr_time);
	//HAL_Delay(5);


	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 100;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 4;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}
	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief I2C1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C1_Init(void)
{

	/* USER CODE BEGIN I2C1_Init 0 */

	/* USER CODE END I2C1_Init 0 */

	/* USER CODE BEGIN I2C1_Init 1 */

	/* USER CODE END I2C1_Init 1 */
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 = 0;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 = 0;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
	if (HAL_I2C_Init(&hi2c1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN I2C1_Init 2 */

	/* USER CODE END I2C1_Init 2 */

}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void)
{

	/* USER CODE BEGIN SPI1_Init 0 */

	/* USER CODE END SPI1_Init 0 */

	/* USER CODE BEGIN SPI1_Init 1 */

	/* USER CODE END SPI1_Init 1 */
	/* SPI1 parameter configuration*/
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI1_Init 2 */

	/* USER CODE END SPI1_Init 2 */

}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void)
{

	/* USER CODE BEGIN SPI2_Init 0 */

	/* USER CODE END SPI2_Init 0 */

	/* USER CODE BEGIN SPI2_Init 1 */

	/* USER CODE END SPI2_Init 1 */
	/* SPI2 parameter configuration*/
	hspi2.Instance = SPI2;
	hspi2.Init.Mode = SPI_MODE_MASTER;
	hspi2.Init.Direction = SPI_DIRECTION_2LINES;
	hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi2.Init.NSS = SPI_NSS_SOFT;
	hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
	hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi2.Init.CRCPolynomial = 10;
	if (HAL_SPI_Init(&hspi2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN SPI2_Init 2 */

	/* USER CODE END SPI2_Init 2 */

}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void)
{

	/* USER CODE BEGIN TIM1_Init 0 */

	/* USER CODE END TIM1_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};
	TIM_OC_InitTypeDef sConfigOC = {0};
	TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

	/* USER CODE BEGIN TIM1_Init 1 */

	/* USER CODE END TIM1_Init 1 */
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = 4;
	htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim1.Init.Period = 65535;
	htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	htim1.Init.RepetitionCounter = 0;
	htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 0;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
	sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
	if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_Handler();
	}
	sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
	sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
	sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
	sBreakDeadTimeConfig.DeadTime = 0;
	sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
	sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
	sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
	if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM1_Init 2 */

	/* USER CODE END TIM1_Init 2 */
	HAL_TIM_MspPostInit(&htim1);

}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void)
{

	/* USER CODE BEGIN TIM2_Init 0 */

	/* USER CODE END TIM2_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM2_Init 1 */

	/* USER CODE END TIM2_Init 1 */
	htim2.Instance = TIM2;
	htim2.Init.Prescaler = 50000;
	htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim2.Init.Period = 4294967295;
	htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM2_Init 2 */

	/* USER CODE END TIM2_Init 2 */

}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void)
{

	/* USER CODE BEGIN TIM3_Init 0 */

	/* USER CODE END TIM3_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM3_Init 1 */

	/* USER CODE END TIM3_Init 1 */
	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 3;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = 65535;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM3_Init 2 */

	/* USER CODE END TIM3_Init 2 */

}

/**
 * @brief TIM4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM4_Init(void)
{

	/* USER CODE BEGIN TIM4_Init 0 */

	/* USER CODE END TIM4_Init 0 */

	TIM_ClockConfigTypeDef sClockSourceConfig = {0};
	TIM_MasterConfigTypeDef sMasterConfig = {0};

	/* USER CODE BEGIN TIM4_Init 1 */

	/* USER CODE END TIM4_Init 1 */
	htim4.Instance = TIM4;
	htim4.Init.Prescaler = 12500;
	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = 65535;
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
	{
		Error_Handler();
	}
	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
	{
		Error_Handler();
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM4_Init 2 */

	/* USER CODE END TIM4_Init 2 */

}

/**
 * @brief TIM10 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM10_Init(void)
{

	/* USER CODE BEGIN TIM10_Init 0 */

	/* USER CODE END TIM10_Init 0 */

	/* USER CODE BEGIN TIM10_Init 1 */

	/* USER CODE END TIM10_Init 1 */
	htim10.Instance = TIM10;
	htim10.Init.Prescaler = 65535;
	htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim10.Init.Period = 65535;
	htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM10_Init 2 */

	/* USER CODE END TIM10_Init 2 */

}

/**
 * @brief TIM11 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM11_Init(void)
{

	/* USER CODE BEGIN TIM11_Init 0 */

	/* USER CODE END TIM11_Init 0 */

	/* USER CODE BEGIN TIM11_Init 1 */

	/* USER CODE END TIM11_Init 1 */
	htim11.Instance = TIM11;
	htim11.Init.Prescaler = 0;
	htim11.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim11.Init.Period = 65535;
	htim11.Init.ClockDivision = TIM_CLOCKDIVISION_DIV4;
	htim11.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
	if (HAL_TIM_Base_Init(&htim11) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN TIM11_Init 2 */

	/* USER CODE END TIM11_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, SPI1_RESET_Pin|SPI1_RS_Pin|SPI1_CS_Pin|EN_Pin
			|MS1_Pin|MS2_Pin|MS3_Pin|RST_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, DC_Pin|RESET_Pin|CS_Pin|LED2_Pin
			|SLEEP_Pin|STEP_Pin|DIR_Pin|I2C_RST_Pin
			|SPI2_CS_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin : LED_Pin */
	GPIO_InitStruct.Pin = LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : B_Pin A_Pin */
	GPIO_InitStruct.Pin = B_Pin|A_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : SW_Pin */
	GPIO_InitStruct.Pin = SW_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(SW_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : SPI1_RESET_Pin SPI1_RS_Pin SPI1_CS_Pin EN_Pin
                           MS1_Pin MS2_Pin MS3_Pin RST_Pin */
	GPIO_InitStruct.Pin = SPI1_RESET_Pin|SPI1_RS_Pin|SPI1_CS_Pin|EN_Pin
			|MS1_Pin|MS2_Pin|MS3_Pin|RST_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pins : DC_Pin RESET_Pin CS_Pin LED2_Pin
                           SLEEP_Pin STEP_Pin DIR_Pin I2C_RST_Pin
                           SPI2_CS_Pin */
	GPIO_InitStruct.Pin = DC_Pin|RESET_Pin|CS_Pin|LED2_Pin
			|SLEEP_Pin|STEP_Pin|DIR_Pin|I2C_RST_Pin
			|SPI2_CS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* EXTI interrupt init*/
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	pressed=true;

	static GPIO_PinState A_state,B_state,prev_A=GPIO_PIN_SET,prev_B=GPIO_PIN_SET;


	if(GPIO_Pin == A_Pin)
	{
		B_state=HAL_GPIO_ReadPin(B_GPIO_Port,B_Pin);

		if(B_state!=prev_B)
		{
			if(B_state==GPIO_PIN_SET)
			{
				if(A_state==GPIO_PIN_RESET)
				{
					selection++;
					timeout_old=TIM2->CNT;
				}
			}
			prev_B=B_state;
		}
	}

	if(GPIO_Pin == B_Pin)
	{
		A_state=HAL_GPIO_ReadPin(A_GPIO_Port,A_Pin);

		if(A_state!=prev_A)
		{
			if(A_state==GPIO_PIN_SET)
			{
				if(B_state==GPIO_PIN_RESET)
				{
					selection--;
					timeout_old=TIM2->CNT;
				}
			}
			prev_A=A_state;
		}
	}

	/*

    if(GPIO_Pin == A_Pin) // If The INT Source Is EXTI Line9 (A9 Pin)
    {
			if(A_state==false)
			{
				TIM1->CNT = 0;
				A_state=true;
				A++;
			}

			if(__HAL_TIM_GetCounter(&htim1)>=500)
			{
				//HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin); // Toggle The Output (LED) Pin
				//A++;
				//TIM1->CNT = 0;
				A_state=false;
				TIM1->CNT = 0;
				A++;
			}
    }
	 */
}



/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
