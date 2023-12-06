/*
 * API_drv8825.h
 *
 *  Created on: Dec 5, 2023
 *      Author: chris
 *
 *      Libreria para el comando del controlador de stepper motors TI DRV8825
 */

#ifndef API_INC_API_DRV8825_H_
#define API_INC_API_DRV8825_H_

#include "stm32f1xx_hal.h"
#include "stdbool.h"

//typedef enum{CW,CCW} rotation_t; // tipo de dato

typedef enum{MOTOR_STOP, MOTOR_RUN_CW, MOTOR_RUN_CCW} motorState_t; // tipo de dato de instruccion de accionamiento del motor

typedef enum{MOTOR_ENABLE, MOTOR_DISABLE} enable_t; // tipo de dato del estado de habilitacion del controlador DRV8825

// tipo de dato de parametros de control del DRV8825
typedef struct{
	uint8_t frequency; // frecuencia pulsación paso del motor
	enable_t enable; // habilitación del controlador drv8825
	//rotation_t direction; // sentido de rotación motor > no usado
	//motorState_t state; // no s
} motorControl_t;

// Definicion de pines de control del DRV8825
#define STEP_Pin GPIO_PIN_0 // Pin de pulsos para el controlador drv8825
#define STEP_GPIO_Port GPIOA
#define DIRECTION_Pin GPIO_PIN_1 // pin de direccion de giro del controlador drv8825
#define DIRECTION_GPIO_Port GPIOA
#define ENABLE_Pin GPIO_PIN_4 // pin de habilitacion del controlador drv8825
#define ENABLE_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_5 // pin de LED de indicacion de motor en funcionamiento
#define LD2_GPIO_Port GPIOA
#define LED_PORT LD2_GPIO_Port
#define LED_PIN LD2_Pin

// Inicializacion de parametros del controlador DRV8825
// Parametros: ninguno
// Retorno: ninguno
void motor_init();

// Accionamiento del motor
// Parametros: <command> motorState_t es instruccion del motor > parada, giro horario o anti-horario
// Retorno: ninguno
void motor_send_command(motorState_t command);

// Cambio de configuracion de la "frecuencia" of velocidad de giro del motor
// Asigna el nuevo valor del 'Pulse" al TIMER2
// Parametros:
// <freq> "frecuencia" de giro del motor > CUIDADO: no es frecuencia sino periodo, es decir, a mayor <freq> mas lento gira el motor
// Corresponde al 'Pulse' del contador de TIM2 (output compare) utilizado como senal de 'STEP' al controlador DRV8825.
void motor_config( uint8_t frequency);

// Inicializacion y configuracion del TIMER 2
// Debe utilizarse esta funcion al variar la frecuencia deseada del STEP del DRV8825
// Parametros: ninguno
// Retorno: ninguno
void timer_init(void);

#endif /* API_INC_API_DRV8825_H_ */
