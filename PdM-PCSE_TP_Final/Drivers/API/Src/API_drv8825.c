/*
 * API_drv8825.c
 *
 *  Created on: Dec 5, 2023
 *      Author: chris
 */

#include "API_drv8825.h"


static TIM_HandleTypeDef motorTimerHandle;

static motorControl_t motorControlHandle;

static bool motorRunning = false;

//#define MOTOR_DURATION (uint16_t)10000 // duracion de accionamiento del motor en ms
//#define MOTOR_FREQ (uint8_t)10 // frecuenca de pulso del controlador DRV8825 (STEP)

void motor_init(){
	motorControlHandle.direction = CW;
	motorControlHandle.duration = MOTOR_DURATION;
	motorControlHandle.enable = MOTOR_DISABLE;
	motorControlHandle.frequency = MOTOR_FREQ;
	motorControlHandle.state = MOTOR_STOP;
}

void motor_send_command(motorState_t command){ //STOP, RUN_CW, RUN_CCW
	if(motorRunning == false){
		motorTimerHandle.Init.Period = motorControlHandle.frequency-1;
	}

	switch(command){
	case MOTOR_STOP:
		HAL_GPIO_WritePin(LED_PORT, LED_PIN, RESET);
		HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, SET); // disable driver
		HAL_TIM_OC_Stop(&motorTimerHandle, TIM_CHANNEL_1); // stop timer
		motorRunning = false;
		break;
	case MOTOR_RUN_CW:
		if(motorRunning == false){
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, SET);
			HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, RESET); // enable driver
			HAL_GPIO_WritePin(DIRECTION_GPIO_Port, DIRECTION_Pin, SET); // direction
			HAL_TIM_OC_Start(&motorTimerHandle, TIM_CHANNEL_1); // Start STEP signal
			motorRunning = true;
			HAL_Delay(5000);
			motorRunning = false;
			HAL_TIM_OC_Stop(&motorTimerHandle, TIM_CHANNEL_1); // stop timer
			HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, SET); // enable driver
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, RESET);
		}
		break;
	case MOTOR_RUN_CCW:
		if(motorRunning == false){
			HAL_GPIO_WritePin(LED_PORT, LED_PIN, SET);
			HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, RESET); // enable driver
			HAL_GPIO_WritePin(DIRECTION_GPIO_Port, DIRECTION_Pin, RESET); // direction
			HAL_TIM_OC_Start(&motorTimerHandle, TIM_CHANNEL_1); // Start STEP signal
			motorRunning = true;
			HAL_Delay(5000);
			motorRunning = false;
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

	// stepper
//	HAL_GPIO_WritePin(LED_PORT, LED_PIN, SET);
//
//	HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, RESET); // enable driver
//	HAL_GPIO_WritePin(DIRECTION_GPIO_Port, DIRECTION_Pin, SET); // change direction
//
//	HAL_TIM_OC_Start(&motorTimerHandle, TIM_CHANNEL_1); // Start STEP signal >> counter toggle to toggle every 20/1000 sec = 50hz
//	HAL_Delay(60000); // run for 60 seconds
//
//	HAL_GPIO_WritePin(ENABLE_GPIO_Port, ENABLE_Pin, SET); // disable driver
//	HAL_TIM_OC_Stop(&motorTimerHandle, TIM_CHANNEL_1); // stop timer
//
//	HAL_GPIO_WritePin(LED_PORT, LED_PIN, RESET);


}

// tiene que inicializarse el timer de nuevo
void motor_config( uint8_t frequency, uint16_t duration){
	motorControlHandle.duration = duration;
	motorControlHandle.frequency = frequency;
}

void timer_init(void){

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  motorTimerHandle.Instance = TIM2;
  motorTimerHandle.Init.Prescaler = 1000-1;
  motorTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  motorTimerHandle.Init.Period = 100-1;
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
