/*
 * API_menuMEF.c
 *
 *  Created on: Nov 27, 2023
 *      Author: chris
 */

#include "API_menuMEF.h"
#include "API_delay.h"

static option_t currentState; // tracking de la opcion actual
//static execute_t currentExecute; // tracking de la ejecucion de las opciones

static pulseTimer_t pulseHandle; // tracking del largo del pulso y de la accion que corresponda

static bool_t executingRoutine = false; // flag indicando que hay una rutina en ejecucion = true

// mantener cuenta de blink_led()
static uint8_t count = 0; // inicializa solo una vez a 0
static delay_t delayBlinkHandle;
static blinkPeriod = BLINK_PERIOD;
static blinkRepeat = BLINK_REPEAT;


void menuMEF_init(){
	pulseHandle.nextTime = NEXT_TIME;
	pulseHandle.executeTime = EXECUTE_TIME;
	currentState = OPTION_1;
	// ingreso de parametros
	menuMEF_user_input();
	// enviar OPCION 1 al LCD
	uartSendString("Opcion 1\r\n");
}

// actualiza el estado de la MEF en funcion del largo del pulso
// retorna NEXT, EXECUTE o NONE para establecer el estado actual de la MEF en menuMEF_set_state()
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

		if( executingRoutine == false){
			if(pulseHandle.deltaTime < pulseHandle.nextTime){
				command = NEXT;
				uartSendString("NEXT\r\n");
			} else if (pulseHandle.deltaTime > pulseHandle.executeTime){
				command = EXECUTE;
				uartSendString("EXECUTE\r\n");
			} else{
				command = NONE;
			}
		} else{
			command = STOP;
		}
	}
	return command;
}

// Actualiza el estado actual del menu (MEF)
void menuMEF_set_state(action_t command){

	if(command == STOP){
		blink_stop(LED_PORT,LED_PIN);
		motor_stop();
	}

	if(command == NEXT){
		if(currentState == OPTION_6){ // volver al inicio
			currentState = OPTION_1;
		} else {
			currentState++; // avanzar a la siguiente opcion
		}
	}
	// si hubo un pulso indicando NEXT o EXECUTE, actualizar LCD y ejecutar la rutina correspondiente
	// actualizar solo cuando hay un cambio
	if((command == EXECUTE) || (command == NEXT)){
		menuMEF_update_display(command); // actualizar display LCD
	}
	if(command == EXECUTE){
		currentState = currentState + OPTION_OFFSET; // pasar a estado EXECUTE_x
	}
	menuMEF_execute(currentState); // iniciar/continuar rutina si corresponde
}

void menuMEF_update_display( action_t command){
	char buffer[16];
	// Escribir al LCD
	if((command == NEXT) || (command == BACK)){
		// enviar estado actual 'Opcion x' al LCD
		sprintf(buffer, "Opcion %d\r\n", currentState+1);
		uartSendString(buffer);
	}
	if(command == EXECUTE){
		// enviar estado actual 'Ejecutando x' al LCD
		sprintf(buffer, "Ejecutando %d\r\n", currentState+1);
		uartSendString(buffer);
	}

}

void menuMEF_execute(){
	// ejecutar la opcion seleccionada
	//blink_led(LED_PORT, LED_PIN, 500, 10);
	switch (currentState){
	case EXECUTE_1:
		// ejecutar 1
		led_on(LED_PORT, LED_PIN);
		break;
	case EXECUTE_2:
		// ejecutar 2
		led_off(LED_PORT, LED_PIN);
		break;
	case EXECUTE_3:
		// ejecutar 3
		blink_led(LED_PORT, LED_PIN, blinkPeriod, blinkRepeat);
		break;
	case EXECUTE_4:
		// ejecutar 4
		break;
	case EXECUTE_5:
		// ejecutar 5
		break;
	case EXECUTE_6:
		// ejecutar 6
		break;
	default:
		// error
		break;
	}


}

void menuMEF_reset_state(){
	if((currentState >= EXECUTE_1) & (currentState <= EXECUTE_6) & (executingRoutine == false)){
		currentState = currentState - OPTION_OFFSET;
	}
	executingRoutine = false;
	menuMEF_update_display(BACK);
}

void led_on(GPIO_TypeDef * port, uint16_t pin ){
	executingRoutine = true;
	HAL_GPIO_WritePin(port, pin, SET);
	executingRoutine = false;
	menuMEF_reset_state();
}

void led_off(GPIO_TypeDef * port, uint16_t pin ){
	executingRoutine = true;
	HAL_GPIO_WritePin(port, pin, RESET);
	executingRoutine = false;
	menuMEF_reset_state();
}

void blink_led(GPIO_TypeDef * port, uint16_t pin, uint16_t period, uint8_t repeat){

	if((count == 0) & (executingRoutine == false)){
		//count = 0;
		executingRoutine = true;
		HAL_GPIO_WritePin(port, pin, SET);
		delayInit(&delayBlinkHandle, period );
	}
	// repetir
	if(delayRead(&delayBlinkHandle) == true){
		HAL_GPIO_TogglePin(port, pin);
		count++;
	}
	// blink stop
	if(count >= repeat*2){
		HAL_GPIO_WritePin(port, pin, RESET);
		count = 0;
		executingRoutine = false;
		menuMEF_reset_state();

	}

// blink bloqueante
//	for(uint8_t i=0; i<repeat; i++){
//		HAL_GPIO_TogglePin(port, pin);
//		HAL_Delay(period);
//	}
//	executingRoutine = false;
//	menuMEF_reset_state();
}

void blink_stop(GPIO_TypeDef * port, uint16_t pin){
	HAL_GPIO_WritePin(port, pin, RESET);
	count = 0;
	executingRoutine = false;
	menuMEF_reset_state();
}

void motor_stop(){

}

void menuMEF_user_input(){
	// agregar nueva func en uart para recibir datos hasta presionar ENTER

	uint8_t intro[] = "Ingrese a continuacion los parametros del parpadeo del led en ms"
			"\r\nO presione ENTER para utilizar el valor en []\r\n\r\n";
	uint8_t periodPrompt[64];
	sprintf(periodPrompt,"Ingrese el Periodo [%d]: ", blinkPeriod);
	uint8_t repeatPrompt[64];
	sprintf(repeatPrompt,"Ingrese las repeticiones [%d]: ", blinkRepeat);

	uartSendString(intro);

	uartSendString(periodPrompt);
	uint8_t periodBuffer[4];
	uartReceiveStringSize(periodBuffer, 3);
	uartSendString(periodBuffer);
	uartSendString("\r\n");
	blinkPeriod = (uint8_t) atoi(periodBuffer);

	uartSendString(repeatPrompt);
	uint8_t repeatBuffer[3];
	uartReceiveStringSize(repeatBuffer, 2);
	uartSendString(repeatBuffer);
	uartSendString("\r\n");
	blinkRepeat = (uint8_t) atoi(repeatBuffer);


	uartSendString("Comienzo...\r\n");
}
