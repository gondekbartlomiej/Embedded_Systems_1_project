/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define disp_G_Pin GPIO_PIN_0
#define disp_G_GPIO_Port GPIOC
#define disp_D_Pin GPIO_PIN_1
#define disp_D_GPIO_Port GPIOC
#define disp_E_Pin GPIO_PIN_2
#define disp_E_GPIO_Port GPIOC
#define disp_C_Pin GPIO_PIN_3
#define disp_C_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA
#define disp_B_Pin GPIO_PIN_4
#define disp_B_GPIO_Port GPIOC
#define disp_F_Pin GPIO_PIN_5
#define disp_F_GPIO_Port GPIOC
#define button_1_Pin GPIO_PIN_0
#define button_1_GPIO_Port GPIOB
#define disp_A_Pin GPIO_PIN_6
#define disp_A_GPIO_Port GPIOC
#define disp_DP_Pin GPIO_PIN_7
#define disp_DP_GPIO_Port GPIOC
#define disp_COM4_Pin GPIO_PIN_8
#define disp_COM4_GPIO_Port GPIOC
#define disp_COM3_Pin GPIO_PIN_9
#define disp_COM3_GPIO_Port GPIOC
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define disp_COM2_Pin GPIO_PIN_10
#define disp_COM2_GPIO_Port GPIOC
#define disp_COM1_Pin GPIO_PIN_11
#define disp_COM1_GPIO_Port GPIOC
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
