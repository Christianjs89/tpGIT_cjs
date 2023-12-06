/*
 * API_drv8825.c
 *
 *  Created on: Dec 5, 2023
 *      Author: chris
 */

#include "API_drv8825.h"


static TIM_HandleTypeDef motorTimerHandle; // handle del periferico TIMER 2 para la senal de STEP del DRV8825

static bool motorRunning = false; // flag de motor en funcionamiento

motorControl_t motorControlHandle; // estructura de control del DRV8825


// Inicializacion de parametros del controlador DRV8825
// Parametros: ninguno
// Retorno: ninguno
void motor_init(){
	//motorControlHandle.direction = CW;
	motorControlHandle.enable = MOTOR_DISABLE;
	//motorControlHandle.frequency = MOTOR_FREQ;
	//motorControlHandle.state = MOTOR_STOP;
}


// Accionamiento del motor
// Parametros: <command> motorState_t es instruccion del motor > parada, giro horario o anti-horario
// Retorno: ninguno
void motor_send_command(motorState_t command){ //STOP, RUN_CW, RUN_CCW

	switch(command){
	case MOTOR_STOP:
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, RESET);
		HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, SET); // disable driver
		HAL_TIM_OC_Stop(&motorTimerHandle, TIM_CHANNEL_1); // stop timer
		motorRunning = false;
		break;
	case MOTOR_RUN_CW:
		if(motorRunning == false){
			//HAL_GPIO_WritePin(LED_PORT, LED_PIN, SET);
			HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, RESET); // enable driver
			HAL_GPIO_WritePin(DIRECTION_GPIO_Port, DIRECTION_Pin, SET); // direction
			HAL_TIM_OC_Start(&motorTimerHandle, TIM_CHANNEL_1); // Start STEP signal
			motorRunning = true;
		}
		break;
	case MOTOR_RUN_CCW:
		if(motorRunning == false){
			//HAL_GPIO_WritePin(LED_PORT, LED_PIN, SET);
			HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, RESET); // enable driver
			HAL_GPIO_WritePin(DIRECTION_GPIO_Port, DIRECTION_Pin, RESET); // direction
			HAL_TIM_OC_Start(&motorTimerHandle, TIM_CHANNEL_1); // Start STEP signal
			motorRunning = true;
		}
		break;
	default:
		//error >> stop motor
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, RESET);
		HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, SET); // disable driver
		HAL_TIM_OC_Stop(&motorTimerHandle, TIM_CHANNEL_1); // stop timer
		motorRunning = false;
		break;
	}

}


// Cambio de configuracion de la "frecuencia" of velocidad de giro del motor
// Asigna el nuevo valor del 'Pulse" al TIMER2
// Parametros:
// <freq> "frecuencia" de giro del motor > CUIDADO: no es frecuencia sino periodo, es decir, a mayor <freq> mas lento gira el motor
// Corresponde al 'Pulse' del contador de TIM2 (output compare) utilizado como senal de 'STEP' al controlador DRV8825.
void motor_config( uint8_t frequency){
	motorControlHandle.frequency = frequency;
	timer_init();
}


// Inicializacion y configuracion del TIMER 2
// Debe utilizarse esta funcion al variar la frecuencia deseada del STEP del DRV8825
// Parametros: ninguno
// Retorno: ninguno
void timer_init(void){

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  motorTimerHandle.Instance = TIM2;
  motorTimerHandle.Init.Prescaler = 1000-1; // timer contador top > resetea a 0 en 999
  motorTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  motorTimerHandle.Init.Period = motorControlHandle.frequency - 1; // toggle pin al alcanzar el contador este valor
  motorTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  motorTimerHandle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&motorTimerHandle) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&motorTimerHandle, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_OC_Init(&motorTimerHandle) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&motorTimerHandle, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_TOGGLE;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_OC_ConfigChannel(&motorTimerHandle, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&motorTimerHandle);

}
