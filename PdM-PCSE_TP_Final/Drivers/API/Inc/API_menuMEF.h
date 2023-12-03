/*
 * API_menuMEF.h
 *
 *  Created on: Nov 27, 2023
 *      Author: chris
 */

#ifndef API_INC_API_MENUMEF_H_
#define API_INC_API_MENUMEF_H_

#include "API_debounce.h"
#include "stdint.h"
//#include "API_lcd_i2c.h"
#include "API_uart.h"
#include "stdio.h"

// tiempos de pulso
#define NEXT_TIME 500 // tiempo maximo a considerar como 'avance' de opcion
#define EXECUTE_TIME 1500 // tiempo minimo a considerar como 'ejecutar' opcion actual

#define BLINK_PERIOD 500 // periodo de parpadeo en ms
#define BLINK_REPEAT 10 // cantidad de repeticiones de parpadeo

// definit GPIO a utilizar como salida del led
#define LED_PORT LD2_GPIO_Port
#define LED_PIN LD2_Pin

#define OPTION_OFFSET 6 // diferencia entre OPTION_x y EXECUTE_x
// tipo de datos de opciones disponibles
typedef enum{OPTION_1,OPTION_2,OPTION_3,OPTION_4,OPTION_5,OPTION_6,
				EXECUTE_1,EXECUTE_2, EXECUTE_3, EXECUTE_4, EXECUTE_5, EXECUTE_6} option_t;

// tipo de datos de ejecuciones disponibles
//typedef enum{EXECUTING_1,EXECUTING_2,EXECUTING_3,EXECUTING_4,EXECUTING_5,EXECUTING_6, IDLE} execute_t;
//typedef enum{EXECUTE, IDLE} execute_t;
// tipo de datos de la accion resultante del pulsador
typedef enum{NONE, NEXT, BACK, EXECUTE, STOP} action_t;
// tipo de dato para llevar la contabilidad del largo del pulso del boton
typedef struct{
	uint32_t startTime; // inicio del temporizado para asignar HAL_getTick() en flanco descendente
	uint32_t stopTime; // fin del temporizado para asignar HAL_getTick() en flanco ascendente
	uint32_t deltaTime; // diferencia entre startTime y stopTime
	uint16_t nextTime; // [ms] tiempo de pulsado maximo que corresponde al avance a la siguiente opcion
	uint16_t executeTime; // [ms] tiempo de pulsado minimo que corresponde a la ejecucion de la opcion selecctionada actual
} pulseTimer_t;

// funcion para editar las variables de accion del largo del pulso
// [ms] tiempo de pulsado maximo que corresponde al avance a la siguiente opcion Rango[20,1000]
// [ms] tiempo de pulsado minimo que corresponde a la ejecucion de la opcion selecctionada actual Rango[1000,5000]
void pulse_config(uint16_t nextTime, uint16_t executeTime);

// funcion para efectuar el tracking del pulsador
// retorna la accion que corresponda a la duracion del pulso
//action_t button_track();

// Inicializacion del menu MEF
void menuMEF_init();

// actualizacion del estado de la MEF (menu de opciones)
// retorna la accion que corresponda con el pulso detectado
action_t menuMEF_update(void);

void menuMEF_set_state(action_t command);

void menuMEF_update_display(action_t command);

void menuMEF_execute();

void menuMEF_reset_state();

void led_on(GPIO_TypeDef * port, uint16_t pin );

void led_off(GPIO_TypeDef * port, uint16_t pin );

void blink_led(GPIO_TypeDef * port, uint16_t pin, uint16_t period, uint8_t repeat);

void blink_stop(GPIO_TypeDef * port, uint16_t pin);

void motor_stop();

void menuMEF_user_input();

#endif /* API_INC_API_MENUMEF_H_ */
