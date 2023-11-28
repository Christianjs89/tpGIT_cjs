/*
 * API_menuMEF.h
 *
 *  Created on: Nov 27, 2023
 *      Author: chris
 */

#ifndef API_INC_API_MENUMEF_H_
#define API_INC_API_MENUMEF_H_

#include "stdint.h"

// tipo de datos de opciones disponibles
typedef enum{OPTION_1,OPTION_2,OPTION_3,OPTION_4,OPTION_5,OPTION_6} option_t;
// tipo de datos de ejecuciones disponibles
typedef enum{EXECUTING_1,EXECUTING_2,EXECUTING_3,EXECUTING_4,EXECUTING_5,EXECUTING_6, IDLE} execute_t;
// tipo de datos de la accion resultante del pulsador
typedef enum{NONE, NEXT, EXECUTE} action_t;
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
action_t button_track();

// Inicializacion del menu MEF
void menuMEF_init();

// actualizacion del estado de la MEF (menu de opciones)
// retorna la accion que corresponda con el pulso detectado
action_t menuMEF_update(void);

#endif /* API_INC_API_MENUMEF_H_ */
