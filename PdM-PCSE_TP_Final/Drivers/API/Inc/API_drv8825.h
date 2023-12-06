/*
 * API_drv8825.h
 *
 *  Created on: Dec 5, 2023
 *      Author: chris
 */

#ifndef API_INC_API_DRV8825_H_
#define API_INC_API_DRV8825_H_

//#include "stm32f1xx_hal_tim.h"
#include "stm32f1xx_hal.h"
#include "stdbool.h"

#define MOTOR_DURATION (uint16_t)10000 // duracion de accionamiento del motor en ms
#define MOTOR_FREQ (uint8_t)10 // frecuenca de pulso del controlador DRV8825 (STEP)

typedef enum{CW,CCW} rotation_t;

typedef enum{MOTOR_STOP, MOTOR_RUN_CW, MOTOR_RUN_CCW} motorState_t; // estado actual del motor

typedef enum{MOTOR_ENABLE, MOTOR_DISABLE} enable_t;


typedef struct{
	uint8_t frequency; // frecuencia pulsación paso del motor
	enable_t enable; // habilitación del controlador drv8825
	rotation_t direction; // sentido de rotación motor
	uint16_t duration; // tiempo de accionamiento en ms
	motorState_t state;
} motorControl_t;


#define STEP_Pin GPIO_PIN_0
#define STEP_GPIO_Port GPIOA
#define DIRECTION_Pin GPIO_PIN_1
#define DIRECTION_GPIO_Port GPIOA
#define ENABLE_Pin GPIO_PIN_4
#define ENABLE_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5
#define LD2_GPIO_Port GPIOA

#define LED_PORT LD2_GPIO_Port
#define LED_PIN LD2_Pin


void motor_init();

void motor_send_command(motorState_t command);

void motor_config( uint8_t frequency, uint16_t duration);

void timer_init(void);

#endif /* API_INC_API_DRV8825_H_ */
