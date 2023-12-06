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
#include "API_uart.h"
#include "stdio.h"

#include "API_delay.h"
#include "API_lcd_i2c.h"

#include "API_drv8825.h"

// tiempos de pulso del boton
#define NEXT_TIME 500 // T0 tiempo maximo a considerar como 'avance' de opcion
#define EXECUTE_TIME 1000 // T1 tiempo minimo a considerar como 'ejecutar' opcion actual

// GPIO a utilizar como salida del led
#define LED_PORT LD2_GPIO_Port // puerto
#define LED_PIN LD2_Pin // pin

// Parametros blinky por default > si el usuario presiona ENTER, se utilizan estos valores
#define BLINK_PERIOD_DEFAULT 500 // periodo de parpadeo en ms
#define BLINK_REPEAT_DEFAULT 10 // cantidad de repeticiones de parpadeo
// Max y min parametros blinky > limites de ingreso del usuario
#define BLINK_PERIOD_MAX 2000 // ms maximo
#define BLINK_PERIOD_MIN 100 // ms minimo
#define BLINK_REPEAT_MAX 20
#define BLINK_REPEAT_MIN 1

// Parametros motor por default > si el usuario presiona ENTER, se utilizan estos valores
#define MOTOR_TIMER_DEFAULT 10000 // tiempo de activacion/temporizado del motor en ms
#define MOTOR_FREQ_DEFAULT 20 // "frecuencia" > contador del output compare TIM2
// Max y min parametros temporizado y "frecuencia" del motor
#define MOTOR_TIMER_MAX 20000 // ms maximo
#define MOTOR_TIMER_MIN 2000 // ms minimo
#define MOTOR_FREQ_MAX 150
#define MOTOR_FREQ_MIN 1


#define OPTION_OFFSET 6 // diferencia entre OPTION_x y EXECUTE_x > NO MODIFICAR
// tipo de datos de opciones disponibles
typedef enum{OPTION_1,OPTION_2,OPTION_3,OPTION_4,OPTION_5,OPTION_6,
				EXECUTE_1,EXECUTE_2, EXECUTE_3, EXECUTE_4, EXECUTE_5, EXECUTE_6} option_t;

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

// Inicializacion del menu MEF > comienzo en Opcion 1
// Solicita el ingreso de parametros del usuario por la UART
// Inicializar el LCD indicando estado inicial
void menuMEF_init();

// actualizacion del estado de la MEF (menu de opciones)
// retorna la accion que corresponda con el pulso detectado
action_t menuMEF_update(void);

// Actualiza el estado actual del menu (MEF) y ejecuta la accion que corresponda
// Parametros: <command> tipo de dato action_t NEXT,NONE o EXECUTE en funcion del largo del pulso
// Retorno: ninguno
void menuMEF_set_state(action_t command);

// Actualiza el LCD con el estado actual de la MEF
// Parametros: <command> tipo de dato action_t NEXT,NONE o EXECUTE en funcion del largo del pulso
// Retorno: ninguno
void menuMEF_update_display(action_t command);

// Es llamado de menuMEF_set_state() y ejecuta el estado actual de la MEF
// Si no se encuentra en un estado de ejecucion, simplemente ejcuta default (ninguna accion)
// Parametros: ninguno
// Retorno: ninguno
void menuMEF_execute();

// Resetea el estado de la MEF al terminar la ejecucion de una rutina
// Actualiza el LCD y la terminal serie
// Parametros: ninguno
// Retorno: ninguno
void menuMEF_reset_state();

// Enciende el LED indicado en port/pin
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
// Retorno: ninguno
void led_on(GPIO_TypeDef * port, uint16_t pin );

// Apaga el LED indicado en port/pin
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
// Retorno: ninguno
void led_off(GPIO_TypeDef * port, uint16_t pin );

// Rutina de parpadeo del led definido en port/pin con el periodo <period> y una cantidad de veces <repeat>
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
// <period> tiempo en ms del medio ciclo del parpadeo, <repeat> cantidad de repeticiones de parpadeo
// Retorno: ninguno
void blink_led(GPIO_TypeDef * port, uint16_t pin, uint16_t period, uint8_t repeat);

// Detencion del parpadeo (apagado LED) y reseteo de variable de contador de repeticiones
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
void blink_stop(GPIO_TypeDef * port, uint16_t pin);

// Accionamiento del stepper motor (Haydon 36362-12 12V 4.6W) controlador por el DRV8825
// Parametros: <command> instruccion de comando del motor tipo motorState_t (parada, giro horario y anti horario)
// <freq> "frecuencia" de giro del motor > CUIDADO: no es frecuencia sino periodo, es decir, a mayor <freq> mas lento gira el motor
// Corresponde al 'Pulse' del contador de TIM2 (output compare) utilizado como senal de 'STEP' al controlador DRV8825.
// <time> duracion del accionamiento en ms
void motor_run(motorState_t command, uint16_t freq, uint16_t time );

// Parada del motor y reseteo de la MEF
// Parametros: ninguno
// Retorno: ninguno
void motor_stop();

// Ingreso de parametros de parpadeo y accionamiento de motor por parte del usuario
// Los parametros a ingresar son blinkPeriod, blinkRepeat, motorTimer, y motorFrequency
// Parametros: ninguno
// Reteorno: ninguno
void menuMEF_user_input();

// Rutina de ingreso de datos por la UART con limites min/max y reingreso en caso de valores fuera de rango
// Parametros: <min> valor minimo permitido a ingresar
// <max> valor maximo permitido a ingresar, <defaultValue> valor por defecto a utilizar si se presiona ENTER
// Retorno: retorna el numero entero uint16_6 ingresado
uint16_t get_value(uint16_t min, uint16_t max, uint16_t defaultValue);

#endif /* API_INC_API_MENUMEF_H_ */
