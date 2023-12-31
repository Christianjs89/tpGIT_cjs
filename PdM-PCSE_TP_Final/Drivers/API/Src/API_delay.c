/*
 * API_delay.c
 *
 *  Created on: Nov 2, 2023
 *      Author: chris
 */


/*---------------------------------------------------------------------------------------*/
#include <API_delay.h>


/**
	* @brief Inicializacion del temporizador
	* @param delay struct {startTime, duration, running}
	* @retval Ninguno
	*/
void delayInit( delay_t * delay, tick_t duration ){
	if(duration > 10000) duration = 10000; // limitar el tiempo a 10 segundos
	if(duration < 5) duration = 5; // limitar el tiempo minimo a 5 ms

	delay->duration = duration;
	delay->running = false;
}

/**
	* @brief Comparacion de delay vs contador milisegundos. Inicia timer si no esta en func
	* @param delay struct {startTime, duration, running}
	* @retval bool true si se cumplio el tiempo / false si no
	*/
bool_t delayRead( delay_t * delay ){

	if(delay->running == false){ // activar timer
		delay->running = true;
		delay->startTime = HAL_GetTick(); // tomar valor de contador en [ms] actual
		return false; // salir de la funcion
	} else {
		// si running == true ya estaba activo el timer, comparar valor actual con duration
		if(HAL_GetTick() - delay->startTime > delay->duration){ // si es timeout
			delay->running = false; // reset el flag
			return true;
		} else {
			// si no es timeout
			return false;
		}
	}
}

/**
	* @brief Cambiar valor de duration
	* @param delay struct {startTime, duration, running}
	* @retval Ninguno
	*/
void delayWrite( delay_t * delay, tick_t duration ){
	if(duration > 10000) duration = 10000; // limitar el tiempo a 10 segundos
	if(duration < 5) duration = 5; // limitar el tiempo minimo a 5 ms

	delay->duration = duration;
}
