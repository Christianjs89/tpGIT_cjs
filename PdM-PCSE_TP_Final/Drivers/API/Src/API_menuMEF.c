/*
 * API_menuMEF.c
 *
 *  Created on: Nov 27, 2023
 *      Author: chris
 */

#include "API_menuMEF.h"
#include "API_debounce.h"

static option_t currentOption; // tracking de la opcion actual
static execute_t currentExecute; // tracking de la ejecucion de las opciones

static pulseTimer_t pulseHandle; // tracking del largo del pulso y de la accion que corresponda


void menuMEF_init(){
	pulseHandle.nextTime = 500;
	pulseHandle.executeTime = 2000;
	currentOption = OPTION_1;
	currentExecute = IDLE;
}

action_t menuMEF_update(void){
	action_t command = NONE;

	if(readKey_down() == true){
		// comenzar conteo
		pulseHandle.startTime = HAL_GetTick(); // tiempo desde boton presionado
	}

	if(readKey_up() == true){
		// terminar conteo y pasar de opcion o ejecutarla
		pulseHandle.stopTime = HAL_GetTick(); // tiempo desde boton presionado
		pulseHandle.deltaTime = pulseHandle.stopTime - pulseHandle.startTime; // if stop>start
		if(pulseHandle.deltaTime < pulseHandle.nextTime){
			command = NEXT;
		} else if (pulseHandle.deltaTime > pulseHandle.executeTime){
			command = EXECUTE;
		} else{
			command = NONE;
		}
	}

	return command;
}
