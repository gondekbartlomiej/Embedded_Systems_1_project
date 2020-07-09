/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include<stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FilterN 50
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;
DMA_HandleTypeDef hdma_spi1_tx;
DMA_HandleTypeDef hdma_spi1_rx;

TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
GPIO_PinState disp_state [4][8];

const uint16_t  setup = 0x20C7; //PD- 1 power delivery - 0-off / 1-on
								//DR- 1 data rate      - 0-100Hz / 1-400hz
								//FS- 0 full scale     - 0-2G range / 1-9G range
								//0-  0 null reg       - must remain 0
								//    C
								//-------------------------------------------------
								//0-  0 null reg       - must remain 0
								//Zen-1 enable Z axis  - 0-off / 1-on
								//Yen-1 enable Y axis  - 0-off / 1-on
								//Xen-1 enable X axis  - 0-off / 1-on
								//    7


const uint16_t  tx_buffer[3] ={0xE900, 0x0000, 0x0000};
volatile int8_t rx_buffer [6];

volatile uint8_t active_axis = 0;
volatile int gyro_read[3];

volatile char msg[50];

char display[4];
volatile int size;

volatile int cnt = 0;
volatile int8_t x_reg[FilterN];
volatile int8_t y_reg[FilterN];
volatile int8_t z_reg[FilterN];

volatile int16_t x_sum=0;
volatile int16_t y_sum=0;
volatile int16_t z_sum=0;

volatile uint8_t index = 0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI1_Init(void);
static void MX_TIM10_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

void disp_buffer_update(const uint8_t com, const char digit){
  if(com >= 0 && com < 4){
    switch (digit){
    
    case '0':  //put 0 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_SET;
      disp_state[com][5] = GPIO_PIN_SET;
      disp_state[com][6] = GPIO_PIN_RESET;
      break;

    case '1':  //put 1 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_RESET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_RESET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_RESET;
      disp_state[com][6] = GPIO_PIN_RESET;
      break;

    case '2':  //put 2 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_RESET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_SET;
      disp_state[com][5] = GPIO_PIN_RESET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;

    case '3':  //put 3 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_RESET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;

    case '4':  //put 4 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_RESET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_RESET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_SET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;
    
    case '5':  //put 5 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_RESET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_SET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;

    case '6':  //put 6 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_RESET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_SET;
      disp_state[com][5] = GPIO_PIN_SET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;

    case '7':  //put 7 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_RESET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_RESET;
      disp_state[com][6] = GPIO_PIN_RESET;
      break;

    case '8':  //put 8 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_SET;
      disp_state[com][5] = GPIO_PIN_SET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;

    case '9':  //put 9 on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_SET;
      disp_state[com][1] = GPIO_PIN_SET;
      disp_state[com][2] = GPIO_PIN_SET;
      disp_state[com][3] = GPIO_PIN_SET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_SET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;

    case 'o':  //put o on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_RESET;
	  disp_state[com][1] = GPIO_PIN_RESET;
	  disp_state[com][2] = GPIO_PIN_SET;
	  disp_state[com][3] = GPIO_PIN_SET;
	  disp_state[com][4] = GPIO_PIN_SET;
	  disp_state[com][5] = GPIO_PIN_RESET;
	  disp_state[com][6] = GPIO_PIN_SET;
	  break;
    
    case '-':  //put '-' on <com> port of display buffer
      disp_state[com][0] = GPIO_PIN_RESET;
      disp_state[com][1] = GPIO_PIN_RESET;
      disp_state[com][2] = GPIO_PIN_RESET;
      disp_state[com][3] = GPIO_PIN_RESET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_RESET;
      disp_state[com][6] = GPIO_PIN_SET;
      break;
    

    default:
      disp_state[com][0] = GPIO_PIN_RESET;
      disp_state[com][1] = GPIO_PIN_RESET;
      disp_state[com][2] = GPIO_PIN_RESET;
      disp_state[com][3] = GPIO_PIN_RESET;
      disp_state[com][4] = GPIO_PIN_RESET;
      disp_state[com][5] = GPIO_PIN_RESET;
      disp_state[com][6] = GPIO_PIN_RESET;
      break;
    }
  }
}

void put_dot(uint8_t place){
	switch(place){
	case 0:
		disp_state[0][7] = GPIO_PIN_SET;
		disp_state[1][7] = GPIO_PIN_RESET;
		disp_state[2][7] = GPIO_PIN_RESET;
		disp_state[3][7] = GPIO_PIN_RESET;
		break;

	case 1:
		disp_state[0][7] = GPIO_PIN_RESET;
		disp_state[1][7] = GPIO_PIN_SET;
		disp_state[2][7] = GPIO_PIN_RESET;
		disp_state[3][7] = GPIO_PIN_RESET;
		break;

	case 2:
		disp_state[0][7] = GPIO_PIN_RESET;
		disp_state[1][7] = GPIO_PIN_RESET;
		disp_state[2][7] = GPIO_PIN_SET;
		disp_state[3][7] = GPIO_PIN_RESET;
		break;

	case 3:
		disp_state[0][7] = GPIO_PIN_RESET;
		disp_state[1][7] = GPIO_PIN_RESET;
		disp_state[2][7] = GPIO_PIN_RESET;
		disp_state[3][7] = GPIO_PIN_SET;
		break;

	default:
		disp_state[0][7] = GPIO_PIN_RESET;
		disp_state[1][7] = GPIO_PIN_RESET;
		disp_state[2][7] = GPIO_PIN_RESET;
		disp_state[3][7] = GPIO_PIN_RESET;
		break;
	}
}

void switch_disp_com(){
	static uint8_t active_com = 0;

	++active_com;
	if(active_com > 3) active_com = 0;

	switch(active_com){
	case 0:
		HAL_GPIO_WritePin(disp_COM1_GPIO_Port, disp_COM1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(disp_COM2_GPIO_Port, disp_COM2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM3_GPIO_Port, disp_COM3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM4_GPIO_Port, disp_COM4_Pin, GPIO_PIN_SET);
		break;

	case 1:
		HAL_GPIO_WritePin(disp_COM1_GPIO_Port, disp_COM1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM2_GPIO_Port, disp_COM2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(disp_COM3_GPIO_Port, disp_COM3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM4_GPIO_Port, disp_COM4_Pin, GPIO_PIN_SET);
		break;

	case 2:
		HAL_GPIO_WritePin(disp_COM1_GPIO_Port, disp_COM1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM2_GPIO_Port, disp_COM2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM3_GPIO_Port, disp_COM3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(disp_COM4_GPIO_Port, disp_COM4_Pin, GPIO_PIN_SET);
		break;

	case 3:
		HAL_GPIO_WritePin(disp_COM1_GPIO_Port, disp_COM1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM2_GPIO_Port, disp_COM2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM3_GPIO_Port, disp_COM3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(disp_COM4_GPIO_Port, disp_COM4_Pin, GPIO_PIN_RESET);
		break;

	default:
		HAL_GPIO_WritePin(disp_COM1_GPIO_Port, disp_COM1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(disp_COM2_GPIO_Port, disp_COM2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(disp_COM3_GPIO_Port, disp_COM3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(disp_COM4_GPIO_Port, disp_COM4_Pin, GPIO_PIN_RESET);
		break;

	}
	HAL_GPIO_WritePin(disp_A_GPIO_Port, disp_A_Pin, disp_state[active_com][0]);
	HAL_GPIO_WritePin(disp_B_GPIO_Port, disp_B_Pin, disp_state[active_com][1]);
	HAL_GPIO_WritePin(disp_C_GPIO_Port, disp_C_Pin, disp_state[active_com][2]);
	HAL_GPIO_WritePin(disp_D_GPIO_Port, disp_D_Pin, disp_state[active_com][3]);
	HAL_GPIO_WritePin(disp_E_GPIO_Port, disp_E_Pin, disp_state[active_com][4]);
	HAL_GPIO_WritePin(disp_F_GPIO_Port, disp_F_Pin, disp_state[active_com][5]);
	HAL_GPIO_WritePin(disp_G_GPIO_Port, disp_G_Pin, disp_state[active_com][6]);
	HAL_GPIO_WritePin(disp_DP_GPIO_Port, disp_DP_Pin, disp_state[active_com][7]);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	uint8_t i = 0;
	if(htim->Instance==TIM10){

		switch_disp_com();

		x_reg[index] = rx_buffer[0];
		y_reg[index] = rx_buffer[2];
		z_reg[index] = rx_buffer[4];

		if(++index > FilterN-1) index = 0;

		x_sum=0;
		y_sum=0;
		z_sum=0;
		for(i=0; i<FilterN; i++){
			x_sum += x_reg[i];
			y_sum += y_reg[i];
			z_sum += z_reg[i];
		}




		//X - axis
		gyro_read[0] = ((2*x_sum/FilterN)/1.27);

		//Y - axis
		gyro_read[1] = ((2*y_sum/FilterN)/1.27);

		//Z - axis
		gyro_read[2] = ((2*z_sum/FilterN)/1.27);

		if(cnt){
			sprintf(display, "o5 %d", active_axis);
			put_dot((uint8_t)5);
			--cnt;
		}
		else{
			if(gyro_read[active_axis] < 0)
				sprintf(display, "%04d", gyro_read[active_axis]);
			else
				sprintf(display, " %03d", gyro_read[active_axis]);

			put_dot((uint8_t) 1);
		}
		disp_buffer_update(0, display[0]);
		disp_buffer_update(1, display[1]);
		disp_buffer_update(2, display[2]);
		disp_buffer_update(3, display[3]);




		HAL_GPIO_WritePin(gyro_CS_GPIO_Port, gyro_CS_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(gyro_CS_GPIO_Port, gyro_CS_Pin, GPIO_PIN_RESET);
		HAL_SPI_TransmitReceive_DMA(&hspi1, tx_buffer, rx_buffer, 3);
	}
}


void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
  if(GPIO_Pin == button_IT_Pin){

//	  HAL_Delay(20);
	  if(HAL_GPIO_ReadPin(button_IT_GPIO_Port, button_IT_Pin)==GPIO_PIN_SET){
		  cnt = 200;
		  active_axis += 1;
		  if(active_axis > 2) active_axis = 0;
	  }

  }
}


/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM10_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */


  HAL_TIM_Base_Start_IT(&htim10);


  HAL_GPIO_WritePin(gyro_CS_GPIO_Port, gyro_CS_Pin, GPIO_PIN_RESET);
  HAL_SPI_Transmit(&hspi1, &setup, 1, 1000);
  HAL_GPIO_WritePin(gyro_CS_GPIO_Port, gyro_CS_Pin, GPIO_PIN_SET);


  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {




	  size = sprintf(msg, "[X]: %+05dG, [Y]: %+05dG, [Z]: %+05dG\n\r",
				 	 gyro_read[0],
					 gyro_read[1],
					 gyro_read[2] );

	  msg[ 6]=msg[ 7]; msg[ 7]='.';
	  msg[19]=msg[20]; msg[20]='.';
	  msg[32]=msg[33]; msg[33]='.';



	  HAL_UART_Transmit(&huart2, msg, size, 100);

	  HAL_Delay(250);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
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
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 100;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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
  hspi1.Init.DataSize = SPI_DATASIZE_16BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
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
  htim10.Init.Prescaler = 9;
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 49999;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/** 
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void) 
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream2_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream2_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream2_IRQn);

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
  HAL_GPIO_WritePin(GPIOC, disp_G_Pin|disp_D_Pin|disp_E_Pin|disp_C_Pin 
                          |disp_B_Pin|disp_F_Pin|disp_A_Pin|disp_DP_Pin 
                          |disp_COM4_Pin|disp_COM3_Pin|disp_COM2_Pin|disp_COM1_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, gyro_CS_Pin|LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : disp_G_Pin disp_D_Pin disp_E_Pin disp_C_Pin 
                           disp_B_Pin disp_F_Pin disp_A_Pin disp_DP_Pin 
                           disp_COM4_Pin disp_COM3_Pin disp_COM2_Pin disp_COM1_Pin */
  GPIO_InitStruct.Pin = disp_G_Pin|disp_D_Pin|disp_E_Pin|disp_C_Pin 
                          |disp_B_Pin|disp_F_Pin|disp_A_Pin|disp_DP_Pin 
                          |disp_COM4_Pin|disp_COM3_Pin|disp_COM2_Pin|disp_COM1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : gyro_CS_Pin LD2_Pin */
  GPIO_InitStruct.Pin = gyro_CS_Pin|LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : button_IT_Pin */
  GPIO_InitStruct.Pin = button_IT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(button_IT_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
