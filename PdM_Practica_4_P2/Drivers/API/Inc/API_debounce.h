/*
 * API_debounce.h
 *
 *  Created on: Nov 10, 2023
 *      Author: christian Saiz
 *
 *  La libreria contiene las funciones de anti rebote del sistema modelado como Maquina de Estados Finitos (MEF)
 */

// Seleccionar F4= NUCLEO-F401 , NUCLEO-F103 = F1. No esta probado en la F4
#define NUCLEO_BOARD F1
//#define NUCLEO_BOARD F4

#ifndef API_INC_API_DEBOUNCE_H_
#define API_INC_API_DEBOUNCE_H_

#endif /* API_INC_API_DEBOUNCE_H_ */

/* INCLUDES */

#include "API_delay.h" // incluir libreria delay para utilizar en API_debounce

#include "stm32f1xx.h" // contiene la definicion de SET/RESET pin
#include "stm32f1xx_hal_gpio.h" // contiene la definicion de puertos y pines de la f103
//#include "stm32f4xx.h" // contiene la definicion de SET/RESET pin
//#include "stm32f4xx_hal_gpio.h" // contiene la definicion de puertos y pines de la f103


/* DEFINES */
#define DEBOUNCE_DELAY 40 // delay del sistema antirebote en milisegundos

// definicion de pines y puertos (NUCLEO-F103RB) para utilizar con esta libreria
// Parecen estar definidos con los mismos nombres para la F401, pero verificar.
#ifndef LD2_Pin
#define LD2_Pin GPIO_PIN_5
#endif

#ifndef LD2_GPIO_Port
#define LD2_GPIO_Port GPIOA
#endif

#ifndef B1_Pin
#define B1_Pin GPIO_PIN_13
#endif

#ifndef B1_GPIO_Port
#define B1_GPIO_Port GPIOC
#endif


/* FUNCTION PROTOTYPES */

/* debounceFSM_init
 * >> Utilizar esta funcion para inicializar la MEF a su estado inicial (BUTTON_UP)
 * >> Inicializa el delay (definido en DEBOUNCE_DELAY) a utilizar por la funcion debounceFSM_update()
 *
 */
void debounceFSM_init();

/* debounceFSM_update
 * >> Lee el estado del pulsador actualizando el estado del mismo segun debounceState_t
 * >> Actualiza la variable que lee readKey()=true al detectarse un flanco descendente del pulsador
 * >> La deteccion del flanco se realiza con el temporizado definido en DEBOUNCE_DELAY de manera no bloqueante
 * >> La funcion debe ser llamada sin interrupciones para cumplir su objetivo
 */
void debounceFSM_update();

/* buttonPressed
 * >> funcion ejecutada al detectar el boton presionado con la logica anti rebote
 */
void buttonPressed();

/* buttonReleased
 * >> funcion ejecutada al detectar el boton liberado con la logica anti rebote
 */
void buttonReleased();

/* readKey
 * >> La funcion retorna true si se detecto un flanco descendente con la logica anti rebote
 * >> Se resetea automaticamente a false si no hay detecciones nuevas
 * */
bool_t readKey();


