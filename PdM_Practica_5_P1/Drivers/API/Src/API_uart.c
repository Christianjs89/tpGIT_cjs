/*
 * API_uart.c
 *
 *  Created on: Nov 19, 2023
 *      Author: chris
 */

#include "API_uart.h"


/* DECLARACION DE VARIABLES */
static UART_HandleTypeDef uartHandle; /* handle de la uart con la configuracion y estado del periferico.
 	 	 	 	 	 	 	 	* Debe poder invocarse en main.c */



/* DEFINICION DE FUNCIONES */

/* > Descripcion: Inicializa la uart con la configuracion preestablecida y envia dicha config a la terminal
 * > Parametro: ninguno
 * > Retorno: retorna un boolt_t true en caso exito o false caso contrario
 */
bool_t uartInit(){
	// uartHandle >> estructura de dato UART_HandleTypeDef declarada en API_uart.h
	bool_t returnFlag = false;

	uartHandle.Instance = USE_USART; // instancia de la uart >> usb conectado a usart2
	uartHandle.Init.BaudRate = 9600; // baudrate tipico
	uartHandle.Init.WordLength = UART_WORDLENGTH_8B; // tamano de los datos >> 8 bits
	uartHandle.Init.StopBits = UART_STOPBITS_1; // cantidad de stop bits >> 1 solo
	uartHandle.Init.Parity = UART_PARITY_NONE; // tipo de paridad >> ninguno
	uartHandle.Init.Mode = UART_MODE_TX_RX; // modo recepcion/recepcion habilitado
	uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE; // sin control de flujo por hardware
	uartHandle.Init.OverSampling = UART_OVERSAMPLING_16; // cantidad de muestras por tiempo de bit

	if (HAL_UART_Init(&uartHandle) == HAL_OK){ // retornar true si es exitosa la configuracion
		print_uart_config(uartHandle);
		returnFlag = true;
	}

	//send_uart_config(uartHandle); // enviar a la terminal la configuracion

	return returnFlag;
}

/* > Descripcion: envia un string (buffer <pstring>) por la uart, finalizando en el caracter NULL
 * > Parametro: <pstring> puntero del tipo uint8_t
 * 				El buffer de envio tendra una cantidad maxima de caracteres definida en <SEND_BUFFER_MAX_SIZE>
 * > Retorno: ninguno
 */
void uartSendString(uint8_t * pstring){
	// pstring se envia sin tamano debe recorrerse incrementando la direccion por aritmetica de pointers
	uint16_t index = 0; // indice para recorrer array pstring, admite hasta 2^16 caracteres
	while(1){
		if( (*(pstring+index) == '\0') || (index > SEND_BUFFER_MAX_SIZE) ) break; // al encontrarse el NULL salir de funcion
		HAL_UART_Transmit(&uartHandle, (pstring+index), 1, 100); // enviar caracter x caracter
		index++;
	}
}

/* > Descripcion: envia una cantidad de caracteres contenidas en un string (buffer <pstring>) por la uart
 * > Parametro: <pstring> puntero del tipo uint8_t / <size> cantidad de caracteres a enviar rango [1-2^16]
 * 				El buffer de envio tendra una cantidad maxima de caracteres definida en <SEND_BUFFER_MAX_SIZE>
 * > Retorno: ninguno
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size){
	if( size > SEND_BUFFER_MAX_SIZE) size = SEND_BUFFER_MAX_SIZE;
	if( size < 1) size = 1;

	uint16_t index = 0;
	while(1){
		if(( *(pstring+index) == '\0') || (index == size) ) break; // finalizar while al encontrar NULL o alzancar la cant de caracteres indicada
		HAL_UART_Transmit(&uartHandle, (pstring+index), 1, 100); // enviar caracter x caracter
		index++;
	}
}

/* > Descripcion: recibe una cantidad de caracteres contenidas en un string por la uart y las almacena en el buffer <pstring>
 * > Parametro: <pstring> puntero del tipo uint8_t / <size> cantidad de caracteres a recibir rango [1-2^16]
 * > 			El buffer de recepcion tendra una cantidad maxima de caracteres definida en <RECEIVE_BUFFER_MAX_SIZE>
 * > Retorno: ninguno
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size){
	// limitar el rango del buffer de entrada
	if(size > RECEIVE_BUFFER_MAX_SIZE) size = RECEIVE_BUFFER_MAX_SIZE;
	if(size < 1 ) size = 1;

	uint16_t index = 0;
	while(1){
		if( index == size ) break; // finalizar while al alzancar la cant de caracteres indicada
		HAL_UART_Receive(&uartHandle, (pstring+index), 1, HAL_MAX_DELAY); // recibir y esperar cada caracter hasta <size>
		index++;
	}

	*(pstring+index) = '\0'; // agregar el NULL al final para conformar el string
}


void print_uart_config(UART_HandleTypeDef uart){
	uartSendString("\r\n**************************** INICIO ****************************\r\n");
	// Instance
	if (uart.Instance == USART1){
		uartSendString("* Instance:		USART1\r\n");
	}
	else if (uart.Instance == USART2){
		uartSendString("* Instance:		USART2\r\n");
	}
	else if (uart.Instance == USART3) {
		uartSendString("* Instance:		USART3\r\n");
	}
	else {
		uartSendString("* Instance:		ERROR\r\n");
	}

	// baudrate
	char br[32];
	sprintf(br,"* BaudRate:		%d\r\n", uart.Init.BaudRate);
	uartSendString(br);

	// WordLength
	if (uart.Init.WordLength == UART_WORDLENGTH_8B){
		uartSendString("* WordLength:		UART_WORDLENGTH_8B\r\n");
	}
	else if (uart.Init.WordLength == UART_WORDLENGTH_9B){
		uartSendString("* WordLength:		UART_WORDLENGTH_9B\r\n");
	}
	else{
		uartSendString("* WordLength:		ERROR\r\n");
	}

	// stopbits
	if ( uart.Init.StopBits == UART_STOPBITS_1){
		uartSendString("* StopBits:		UART_STOPBITS_1\r\n");
	}
	else if (uart.Init.StopBits == UART_STOPBITS_2){
		uartSendString("* StopBits:		UART_STOPBITS_2\r\n");
	}
	else{
		uartSendString("* StopBits:		ERROR\r\n");
	}

	// Parity
	if (uart.Init.Parity == UART_PARITY_NONE){
		uartSendString("* Parity:		UART_PARITY_NONE\r\n");
	}
	else if (uart.Init.Parity == UART_PARITY_EVEN){
		uartSendString("* Parity:		UART_PARITY_EVEN\r\n");
	}
	else if (uart.Init.Parity == UART_PARITY_ODD){
		uartSendString("* Parity:		UART_PARITY_ODD\r\n");
	}
	else{
		uartSendString("* Parity:		ERROR\r\n");
	}

	// mode
	if (uart.Init.Mode == UART_MODE_TX_RX){
		uartSendString("* Mode:			UART_MODE_TX_RX\r\n");
	}
	else if (uart.Init.Mode == UART_MODE_RX){
		uartSendString("* Mode:			UART_MODE_RX\r\n");
	}
	else if (uart.Init.Mode == UART_MODE_TX){
		uartSendString("* Mode:			UART_MODE_TX\r\n");
	}
	else{
		uartSendString("* Mode:			ERROR\r\n");
	}

	// flow control
	if (uart.Init.HwFlowCtl == UART_HWCONTROL_NONE){
		uartSendString("* HwFlowCtl:		UART_HWCONTROL_NONE\r\n");
	}
	else if (uart.Init.HwFlowCtl == UART_HWCONTROL_RTS){
		uartSendString("* HwFlowCtl:		UART_HWCONTROL_RTS\r\n");
	}
	else if (uart.Init.HwFlowCtl == UART_HWCONTROL_CTS){
		uartSendString("* HwFlowCtl:		UART_HWCONTROL_CTS\r\n");
	}
	else if (uart.Init.HwFlowCtl == UART_HWCONTROL_RTS_CTS){
		uartSendString("* HwFlowCtl:		UART_HWCONTROL_RTS_CTS\r\n");
	}
	else{
		uartSendString("* HwFlowCtl:		ERROR\r\n");
	}

	// oversampling
	if (uart.Init.OverSampling == UART_OVERSAMPLING_16){
		uartSendString("* OverSampling:		UART_OVERSAMPLING_16\r\n");
	}
	else{
		uartSendString("* OverSampling:		ERROR\r\n");
	}

	uartSendString("****************************************************************\r\n\r\n");
}

