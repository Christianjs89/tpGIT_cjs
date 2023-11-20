/*
 * API_debounce.c
 *
 *  Created on: Nov 10, 2023
 *      Author: chris
 */

/* INCLUDES */
#include "API_debounce.h"


/* DEFINICION TIPO DE DATOS */

// Definicion de tipo de dato debounceState_t privada en este archivo .c
typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING
} debounceState_t;

// Definicion de tipo bool_t
typedef bool bool_t;


/* DECLARACION DE VARIABLES */

// declaracion de variable del tipo debounceState_t global privada (static) a este archivo .c
static debounceState_t estado;

// Declaracion de variable para usar con readKey(). false/true solo detectar flanco descendente valido
static bool_t fallingState;

// declaracion de variable tipo delay_t para llevar control del timer
static delay_t delayHandle;


/* DEFINICION DE FUNCIONES */

// Definicion de inicializacion del anti rebote
void debounceFSM_init(){
	estado = BUTTON_UP;
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET); // iniciar con led apagado (salida)
	delayInit(&delayHandle, DEBOUNCE_DELAY);
}

// Definicion de actualizacion del valor del pulsador con anti rebote por software
void debounceFSM_update(){
	switch(estado){
	case BUTTON_UP:
		// si el pin esta low pasar al estado BUTTON_FALLING
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){
			estado = BUTTON_FALLING;
		}
		break;
	case BUTTON_FALLING:
		// si el pin esta high luego de 40 ms, volver a BUTTON_UP
		// si el pin esta low luego de 40 ms, cambiar a BUTTON_DOWN
		if(delayRead(&delayHandle) == true){ // si transcurrio el debounceDelay, muestrear pulsador nuevamente. Iniciar delay si es llamado por 1ra vez
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1){ // pulsador no presionado >> 1
				estado = BUTTON_UP; // si sigue high, el estado vuelve a BUTTON_UP
			} else { // pulsador presionado >> 0
				estado = BUTTON_DOWN;
				// ejecutar buttonPressed()
				buttonPressed();
				// Reemplazo del encendido del led por la deteccion unica del flanco descendente
				fallingState = true;
			}
		}
		break;
	case BUTTON_DOWN:
		// si el pin esta high, pasar al estado BUTTON_RAISING
		if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 1){
			estado = BUTTON_RAISING;
		}
		break;
	case BUTTON_RAISING:
		// si el pin esta low luego de 40 ms, volver a BUTTON_DOWN
		// si el pin esta high luego de 40 ms, cambiar a BUTTON_UP
		if(delayRead(&delayHandle) == true){ // si transcurrio el debounceDelay, muestrear pulsador nuevamente
			if(HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == 0){ // pulsador presionado >> 0
				estado = BUTTON_DOWN; // si sigue low, el estado vuelve a BUTTON_DOWN
			} else { // pulsador no presionado >> 1
				estado = BUTTON_UP;
				// ejecutar buttonReleased()
				buttonReleased();
			}
		}

		break;
	default:
		// checkear errores
		// reiniciar estado
		debounceFSM_init();
		break;
	}
}

bool_t readKey(){
	bool_t result = false;
	// retornar true solo si se detecta flanco descendente
	if(fallingState == true){
		fallingState = false; // resetear variable privada
		result = true;
	}
	return result;
}


// Definicion de funcion para boton presionado
void buttonPressed(){
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, SET);
	// ejecutar la accion deseada al detectar pulsado
	uartSendString("\r\nFlanco descendente!\r\n");
}

// Definicion de funcion para boton liberado
void buttonReleased(){
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, RESET);
	// ejecutar la accion deseada al detectar liberacion
	uartSendString("\r\nFlanco ascendente!\r\n");
}

