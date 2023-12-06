/*
 * API_menuMEF.c
 *
 *  Created on: Nov 27, 2023
 *      Author: chris
 */

#include "API_menuMEF.h"
//#include "API_delay.h"
//#include "API_lcd_i2c.h"

static option_t currentState; // tracking de la opcion actual

static pulseTimer_t pulseHandle; // tracking del largo del pulso y de la accion que corresponda

static bool_t executingRoutine = false; // flag indicando que hay una rutina en ejecucion = true

// Parametros y variables blink_led()
static uint8_t count = 0; // inicializa solo una vez a 0 > mantiene cuenta de parpadeos
static delay_t delayBlinkHandle; // handle para el delay de parpadeos
static uint16_t blinkPeriod = BLINK_PERIOD_DEFAULT; // almacena el valor del periodo
static uint16_t blinkRepeat = BLINK_REPEAT_DEFAULT; // almacena el valor de las repeticiones

// Parametros y variables drv8825 stepper motor
static delay_t delayMotorHandle; // handle para el delay de activacion del motor
static uint16_t motorTimer = MOTOR_TIMER_DEFAULT;
static uint16_t motorFrequency = MOTOR_FREQ_DEFAULT;

static uint8_t optionText[13][16] = {"1-LED ON       ", "2-LED OFF      ", "3-BLINK LED    ", "4-MOTOR CW+    ", "5-MOTOR CCW-   ", "6-FREE         ",
				"EJECUTANDO_1   ", "EJECUTANDO_2   ", "EJECUTANDO_3   ", "EJECUTANDO_4   ", "EJECUTANDO_5   ", "EJECUTANDO_6   "};

// Inicializacion del menu MEF > comienzo en Opcion 1
// Solicita el ingreso de parametros del usuario por la UART
// Inicializar el LCD indicando estado inicial
// Parametros: ninguno
// retorno: ninguno
void menuMEF_init(){
	pulseHandle.nextTime = NEXT_TIME;
	pulseHandle.executeTime = EXECUTE_TIME;
	currentState = OPTION_1;

	// ingreso de parametros por la terminal
	menuMEF_user_input();

	// Comienzo del LCD
	lcd_init();
	uint8_t welcomeText[] = "* MENU MEF *";
	lcd_print_text(welcomeText, sizeof(welcomeText)/sizeof(welcomeText[0]));

	// Mostrar comienzo en la terminal serie
	uartSendString("\r\n*************** Comienzo MENU MEF ****************\r\n");
	uint8_t buffer[32];
	sprintf(buffer, "%s\r\n", optionText[currentState]);
	uartSendString(buffer);

	// mostrar el estado actual en el LCD
	lcd_set_position(2, 1);
	lcd_print_text(optionText[currentState], sizeof(optionText)/sizeof(optionText[0]));
}


// funcion para editar las variables de accion del largo del pulso
// Parametros:
// <nextTime> [ms] tiempo de pulsado maximo que corresponde al avance a la siguiente opcion Rango[20,1000]
// <executeTime> [ms] tiempo de pulsado minimo que corresponde a la ejecucion de la opcion selecctionada actual Rango[1000,5000]
// Retorno: ninguno
void pulse_config(uint16_t nextTime, uint16_t executeTime){
	pulseHandle.nextTime = nextTime;
	pulseHandle.executeTime = executeTime;
}


// Actualiza el estado de la MEF en funcion del largo del pulso definido en pulseHandle
// Permite la detencio de cualquier rutina al presionar el pulsador
// Parametros: ninguno
// Retorno: NEXT, EXECUTE o NONE para establecer el estado actual de la MEF en menuMEF_set_state()
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
			uartSendString("STOP\r\n");
		}
	}
	return command;
}


// Actualiza el estado actual del menu (MEF) y ejecuta la accion que corresponda
// Parametros: <command> tipo de dato action_t NEXT,NONE o EXECUTE en funcion del largo del pulso
// Retorno: ninguno
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
	if(command == EXECUTE){
		currentState = currentState + OPTION_OFFSET; // pasar a estado EXECUTE_x
	}
	// si hubo un pulso indicando NEXT o EXECUTE, actualizar LCD y ejecutar la rutina correspondiente
	// actualizar solo cuando hay un cambio
	if((command == EXECUTE) || (command == NEXT)){
		menuMEF_update_display(command); // actualizar display LCD
	}
	menuMEF_execute(currentState); // iniciar/continuar rutina si corresponde
}


// Actualiza el LCD con el estado actual de la MEF
// Parametros: <command> tipo de dato action_t NEXT,NONE o EXECUTE en funcion del largo del pulso
// Retorno: ninguno
void menuMEF_update_display( action_t command){
	char buffer[32];
	// Escribir al LCD
	if((command == NEXT) || (command == BACK)){
		// enviar estado actual 'Opcion x' al LCD y a la terminal
		sprintf(buffer, "%s\r\n", optionText[currentState]);
		uartSendString(buffer);
		lcd_set_position(2, 1);
		lcd_print_text(optionText[currentState], sizeof(optionText)/sizeof(optionText[0]));
	}
	if(command == EXECUTE){
		// enviar estado actual 'Ejecutando x' al LCD y a la terminal
		sprintf(buffer, "%s\r\n", optionText[currentState]);
		uartSendString(buffer);
		lcd_set_position(2, 1);
		//lcd_print_text(optionText[currentState+OPTION_OFFSET], sizeof(optionText)/sizeof(optionText[0]));
		lcd_print_text(optionText[currentState], sizeof(optionText)/sizeof(optionText[0]));
	}
}


// Es llamado de menuMEF_set_state() y ejecuta el estado actual de la MEF
// Si no se encuentra en un estado de ejecucion, simplemente ejcuta default (ninguna accion)
// Parametros: ninguno
// Retorno: ninguno
void menuMEF_execute(){
	// ejecutar la opcion seleccionada
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
		// ejecutar 4 motor CW
		motor_run(MOTOR_RUN_CW, motorFrequency, motorTimer);
		break;
	case EXECUTE_5:
		// ejecutar 5 motor CCW
		motor_run(MOTOR_RUN_CCW, motorFrequency, motorTimer);
		break;
	case EXECUTE_6:
		// ejecutar 6
		// free
		break;
	default:
		// no ejecutar nada
		break;
	}
}


// Resetea el estado de la MEF al terminar la ejecucion de una rutina
// Actualiza el LCD y la terminal serie
// Parametros: ninguno
// Retorno: ninguno
void menuMEF_reset_state(){
	if((currentState >= EXECUTE_1) & (currentState <= EXECUTE_6) & (executingRoutine == false)){
		currentState = currentState - OPTION_OFFSET;
	}
	executingRoutine = false;
	menuMEF_update_display(BACK);
}


// Enciende el LED indicado en port/pin
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
// Retorno: ninguno
void led_on(GPIO_TypeDef * port, uint16_t pin ){
	executingRoutine = true;
	HAL_GPIO_WritePin(port, pin, SET);
	executingRoutine = false;
	menuMEF_reset_state();
}


// Apaga el LED indicado en port/pin
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
// Retorno: ninguno
void led_off(GPIO_TypeDef * port, uint16_t pin ){
	executingRoutine = true;
	HAL_GPIO_WritePin(port, pin, RESET);
	executingRoutine = false;
	menuMEF_reset_state();
}


// Rutina de parpadeo del led definido en port/pin con el periodo <period> y una cantidad de veces <repeat>
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
// <period> tiempo en ms del medio ciclo del parpadeo, <repeat> cantidad de repeticiones de parpadeo
// Retorno: ninguno
void blink_led(GPIO_TypeDef * port, uint16_t pin, uint16_t period, uint8_t repeat){

	if((count == 0) & (executingRoutine == false)){
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
}


// Detencion del parpadeo (apagado LED) y reseteo de variable de contador de repeticiones
// Parametros: <port> puerto GPIO, <pin> pin del puerto GPIO
void blink_stop(GPIO_TypeDef * port, uint16_t pin){
	HAL_GPIO_WritePin(port, pin, RESET);
	count = 0;
	executingRoutine = false;
	menuMEF_reset_state();
}


// Accionamiento del stepper motor (Haydon 36362-12 12V 4.6W) controlador por el DRV8825
// Parametros: <command> instruccion de comando del motor tipo motorState_t (parada, giro horario y anti horario)
// <freq> "frecuencia" de giro del motor > CUIDADO: no es frecuencia sino periodo, es decir, a mayor <freq> mas lento gira el motor
// Corresponde al 'Pulse' del contador de TIM2 (output compare) utilizado como senal de 'STEP' al controlador DRV8825.
// <time> duracion del accionamiento en ms
void motor_run(motorState_t command, uint16_t freq, uint16_t time ){
	if(executingRoutine == false){
		executingRoutine = true;
		motor_config(freq);
		motor_send_command(command);
		delayInit(&delayMotorHandle, time );
	}

	if(delayRead(&delayMotorHandle) == true){
		motor_stop();
	}
}


// Parada del motor y reseteo de la MEF
// Parametros: ninguno
// Retorno: ninguno
void motor_stop(){
	motor_send_command(MOTOR_STOP);
	executingRoutine = false;
	menuMEF_reset_state();
}


// Ingreso de parametros de parpadeo y accionamiento de motor por parte del usuario
// Los parametros a ingresar son blinkPeriod, blinkRepeat, motorTimer, y motorFrequency
// Parametros: ninguno
// Reteorno: ninguno
void menuMEF_user_input(){
	// Ingreso valores LED
	uint8_t introLED[] = "######################### COMIENZO DEL PROGRAMA #########################\r\n"
			      "# Ingrese a continuacion los parametros del parpadeo del led en ms\r\n"
				  "# Presione ENTER para utilizar [valor] por defecto:\r\n";
	uint8_t periodPrompt[64];
	uint8_t repeatPrompt[64];
	sprintf(periodPrompt,"# [%d-%d] Periodo [%d]: ", BLINK_PERIOD_MIN, BLINK_PERIOD_MAX, blinkPeriod);
	sprintf(repeatPrompt,"# [%d-%d] Repeticiones [%d]: ", BLINK_REPEAT_MIN, BLINK_REPEAT_MAX, blinkRepeat);

	uartSendString(introLED);

	uartSendString(periodPrompt);
	blinkPeriod = get_value(BLINK_PERIOD_MIN, BLINK_PERIOD_MAX, BLINK_PERIOD_DEFAULT);

	uartSendString(repeatPrompt);
	blinkRepeat = get_value(BLINK_REPEAT_MIN, BLINK_REPEAT_MAX, BLINK_REPEAT_DEFAULT);

	// Ingreso valores MOTOR
	uint8_t introMotor[] = "#\r\n# Ingrese a continuacion los parametros del motor ms\r\n"
						 "# Presione ENTER para utilizar [valor] por defecto:\r\n";
	uint8_t timerPrompt[64];
	uint8_t frequencyPrompt[64];
	sprintf(timerPrompt,"# [%d-%d] Temporizado [%d]: ", MOTOR_TIMER_MIN, MOTOR_TIMER_MAX, motorTimer);
	sprintf(frequencyPrompt,"# [%d-%d] Frecuencia [%d]: ", MOTOR_FREQ_MIN, MOTOR_FREQ_MAX, motorFrequency);

	uartSendString(introMotor);

	uartSendString(timerPrompt);
	motorTimer = get_value(MOTOR_TIMER_MIN, MOTOR_TIMER_MAX, MOTOR_TIMER_DEFAULT);

	uartSendString(frequencyPrompt);
	motorFrequency = get_value(MOTOR_FREQ_MIN, MOTOR_FREQ_MAX, MOTOR_FREQ_DEFAULT);

	uint8_t parameters[64];
	sprintf(parameters,"# Parametros blinky: PERRIODO [%d], REPETIICIONES [%d]\r\n", blinkPeriod, blinkRepeat );
	uartSendString(parameters);
	sprintf(parameters,"# Parametros motor: TEMPORIZADO [%d], FRECUENCIA [%d]\r\n", motorTimer, motorFrequency );
	uartSendString(parameters);

	uint8_t end[] = "#########################################################################\r\n";
	uartSendString(end);
}


// Rutina de ingreso de datos por la UART con limites min/max y reingreso en caso de valores fuera de rango
// Parametros: <min> valor minimo permitido a ingresar
// <max> valor maximo permitido a ingresar, <defaultValue> valor por defecto a utilizar si se presiona ENTER
// Retorno: retorna el numero entero uint16_6 ingresado
uint16_t get_value(uint16_t min, uint16_t max, uint16_t defaultValue){
	uint8_t inputBuffer[10];
	uint16_t value;

	while(1){
		uartReceiveString(inputBuffer, sizeof(inputBuffer)/sizeof(inputBuffer[0]));
		if(inputBuffer[0] == '\0'){
			value = defaultValue;
			uartSendString("\r\n");
			break;
		}
		//uartSendString(periodBuffer);
		uartSendString("\r\n");
		value = (uint16_t) atoi(inputBuffer);
		if( (value <= max) & (value >= min) ){
			break;
		} else{
			uartSendString("\r\n# Valor fuera de rango!\r\nIngresar nuevamente: ");
		}
	}
	return value;
}
