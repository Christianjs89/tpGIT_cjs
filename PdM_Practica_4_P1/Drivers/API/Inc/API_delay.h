/*
 * API_delay.h
 *
 *  Created on: Nov 2, 2023
 *      Author: chris
 */

#ifndef API_INC_API_DELAY_H_
#define API_INC_API_DELAY_H_

/* Includes */
#include <stdint.h>
#include <stdbool.h>


/* Definicion de data types */
typedef uint32_t tick_t; // stdint.h requerida para esta definicion de tipo de datos
typedef bool bool_t; // stdbool.h requerida para esta definicion de variable booleana

// nuevo tipo de estructura de dato para controlar y configurar el timer (handle)
typedef struct{
	tick_t startTime; // valor actual del contador de ms al iniciar el timer
	tick_t duration; // delay elegido para el timer
	bool_t running; // flag de estado del timer
} delay_t;


/* Declaracion de prototipos de funcion */

/*
 * delayInit >> Configuracion del delay *
 * 	delay >> delay handle que porta la config y el estado del delay
 * 	duration >> duracion del delay en ms
 */
void delayInit( delay_t * delay, tick_t duration ); // asignar duracion y setear el flag running

/*
 * delayRead >> comparacion del contador del timer con el delay requerido
 * La funcion comienza el conteo si no se ha iniciado todavia.
 * 	delay >> delay handle que porta la config y estado del delay
 */
bool_t delayRead( delay_t * delay ); // verifica el flag running y compara el contador/duracion. Inicia el timer si no comenzo

/*
 * delayWrite >> modifica el delay del temporizador
 * 	duration >> valor deseado del temporizador
 */
void delayWrite( delay_t * delay, tick_t duration ); // cambiar la duracion del delay




#endif /* API_INC_API_DELAY_H_ */


