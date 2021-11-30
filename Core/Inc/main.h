/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define B_Pin GPIO_PIN_14
#define B_GPIO_Port GPIOC
#define B_EXTI_IRQn EXTI15_10_IRQn
#define A_Pin GPIO_PIN_15
#define A_GPIO_Port GPIOC
#define A_EXTI_IRQn EXTI15_10_IRQn
#define SW_Pin GPIO_PIN_0
#define SW_GPIO_Port GPIOA
#define SPI1_RESET_Pin GPIO_PIN_2
#define SPI1_RESET_GPIO_Port GPIOA
#define SPI1_RS_Pin GPIO_PIN_3
#define SPI1_RS_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define DC_Pin GPIO_PIN_0
#define DC_GPIO_Port GPIOB
#define RESET_Pin GPIO_PIN_1
#define RESET_GPIO_Port GPIOB
#define CS_Pin GPIO_PIN_2
#define CS_GPIO_Port GPIOB
#define LED2_Pin GPIO_PIN_13
#define LED2_GPIO_Port GPIOB
#define BLED_Pin GPIO_PIN_8
#define BLED_GPIO_Port GPIOA
#define EN_Pin GPIO_PIN_9
#define EN_GPIO_Port GPIOA
#define MS1_Pin GPIO_PIN_10
#define MS1_GPIO_Port GPIOA
#define MS2_Pin GPIO_PIN_11
#define MS2_GPIO_Port GPIOA
#define MS3_Pin GPIO_PIN_12
#define MS3_GPIO_Port GPIOA
#define RST_Pin GPIO_PIN_15
#define RST_GPIO_Port GPIOA
#define SLEEP_Pin GPIO_PIN_3
#define SLEEP_GPIO_Port GPIOB
#define STEP_Pin GPIO_PIN_4
#define STEP_GPIO_Port GPIOB
#define DIR_Pin GPIO_PIN_5
#define DIR_GPIO_Port GPIOB
#define I2C_SCL_Pin GPIO_PIN_6
#define I2C_SCL_GPIO_Port GPIOB
#define I2C_SDA_Pin GPIO_PIN_7
#define I2C_SDA_GPIO_Port GPIOB
#define I2C_RST_Pin GPIO_PIN_8
#define I2C_RST_GPIO_Port GPIOB
#define SPI2_CS_Pin GPIO_PIN_9
#define SPI2_CS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
