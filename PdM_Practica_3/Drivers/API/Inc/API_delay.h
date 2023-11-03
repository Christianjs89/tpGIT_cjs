/*
 * API_delay.h
 *
 *  Created on: Nov 2, 2023
 *      Author: chris
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_



#endif /* API_INC_API_DELAY_H_ */

/* Includes */
#include <stdint.h>
#include <stdbool.h>


/* Definicion de data types */
typedef uint32_t tick_t; // stdint.h requerida para esta definicion
typedef bool bool_t; // stdbool.h requerida para esta definicion

// nuevo tipo de dato struct para configurar el timer
typedef struct{
	tick_t startTime; // valor actual del contador de ms al iniciar el timer
	tick_t duration; // delay elegido para el timer
	bool_t running; // flag de estado del timer
} delay_t;


/* Declaracion de prototipos de funcion */
void delayInit( delay_t * delay, tick_t duration ); // asignar duracion y setear el flag running
bool_t delayRead( delay_t * delay ); // verifica el flag running y compara el contador/duracion. Inicia el timer si no comenzo
void delayWrite( delay_t * delay, tick_t duration ); // cambiar la duracion del delay



