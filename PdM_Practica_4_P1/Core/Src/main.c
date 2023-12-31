/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <API_delay.h> // Nueva libreria API Practica_3

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DEBOUNCE_DELAY 40 // delay del sistema antirebote en milisegundos

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
debounceState_t estadoHandle; // handle al estado actual. Solo puede ser igual a un valor definido en debounceState_t
delay_t delayHandle; // handle para controlar el delay no bloqueante entre muestreos del pulsador (40 ms)

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */
void debounceFSM_init(debounceState_t * estado); // cargar estado inicial
void debounceFSM_update(debounceState_t * estado, delay_t * delay); // leer entradas, logica de transicion de estados, actualizar salidas
void buttonPressed(void); // enceder led
void buttonReleased(void); // apagar led

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
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  debounceFSM_init(&estadoHandle);
  delayInit(&delayHandle, DEBOUNCE_DELAY); // inicializar configuracion del delay anti rebote

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */


  while(1){

	  debounceFSM_update(&estadoHandle, &delayHandle);

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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
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
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/*
 * 	debounceState_t: BUTTON_UP, BUTTON_FALLING,	BUTTON_DOWN, BUTTON_RAISING
 */
void debounceFSM_init(debounceState_t * estado){
	estado = BUTTON_UP;
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET); // iniciar con led apagado (salida)
}


void debounceFSM_update(debounceState_t * estado, delay_t * delay){
	/* Cuidado con delay:
	 * >> aplicar a la funcion delayRead() como delay y no como &delay
	 * >> delay ya entra a esta funcion como el pointer a la direccion del struct delay
	 * >> delay se modifica dentro de esta funcion y sus cambios se ven reflejados en delayHandle global
	 */

	/*
	 * debouncing bloqueante
	if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){
		HAL_Delay(50);
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){
			HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
		}
	}*/

	switch(*estado){
	case BUTTON_UP:
		// si el pin esta low pasar al estado BUTTON_FALLING
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){
			*estado = BUTTON_FALLING;
		}
		break;
	case BUTTON_FALLING:
		// si el pin esta high luego de 40 ms, volver a BUTTON_UP
		// si el pin esta low luego de 40 ms, cambiar a BUTTON_DOWN
		if(delayRead(delay) == true){ // si transcurrio el debounceDelay, muestrear pulsador nuevamente. Iniciar delay si es llamado por 1ra vez
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1){ // pulsador no presionado >> 1
				*estado = BUTTON_UP; // si sigue high, el estado vuelve a BUTTON_UP
			} else { // pulsador presionado >> 0
				*estado = BUTTON_DOWN;
				// ejecutar buttonPressed()
				buttonPressed();
			}
		}
		break;
	case BUTTON_DOWN:
		// si el pin esta high, pasar al estado BUTTON_RAISING
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1){
			*estado = BUTTON_RAISING;
		}
		break;
	case BUTTON_RAISING:
		// si el pin esta low luego de 40 ms, volver a BUTTON_DOWN
		// si el pin esta high luego de 40 ms, cambiar a BUTTON_UP
		if(delayRead(delay) == true){ // si transcurrio el debounceDelay, muestrear pulsador nuevamente
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){ // pulsador presionado >> 0
				*estado = BUTTON_DOWN; // si sigue low, el estado vuelve a BUTTON_DOWN
			} else { // pulsador no presionado >> 1
				*estado = BUTTON_UP;
				// ejecutar buttonReleased()
				buttonReleased();
			}
		}

		break;
	default:
		// checkear errores
		// reiniciar estado
		debounceFSM_init(estado);
		break;
	}
}

void buttonPressed(void){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, SET);
}

void buttonReleased(void){
	HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);
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
