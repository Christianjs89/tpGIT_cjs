//Includes Necesarios
#include <stdint.h>
#include <stdbool.h>
#include "stm32f4xx_hal.h"



typedef enum{
	BUTTON_UP,
	BUTTON_FALLING,
	BUTTON_DOWN,
	BUTTON_RAISING,
} debounceState_t;

typedef bool bool_t;
// Variable Necesarias:



//Declaraciones de Funciones Publicas
void debounceFSM_init(void);      // Inicializar la maquina de etado
void debounceFSM_update(void);    // Actuliza el estado de la maquin de estado
void buttonPressed(void);         //Da la salida de boton precionado (enciende el led)
void buttonReleased(void);        //Da la salida de boton NO precionado (Apaga el led)
bool_t readKey(void);




