/*
 * port_uart.c
 *
 *  Created on: Dec 6, 2023
 *      Author: chris
 *
 *      Wrapper de funciones especificas del periferico UART
 */

#include "stm32f1xx.h"
#include "stdbool.h"

typedef bool bool_t;

static UART_HandleTypeDef uartHandle;

/* > Descripcion: Inicializa la uart con la configuracion preestablecida y envia dicha config a la terminal
 * > Parametro: ninguno
 * > Retorno: retorna un boolt_t true en caso exito o false caso contrario
 */
bool_t uartInit(){
	// uartHandle >> estructura de dato UART_HandleTypeDef declarada en API_uart.h
	bool_t returnFlag = false;

	uartHandle.Instance = USART2; // instancia de la uart >> usb conectado a usart2
	uartHandle.Init.BaudRate = 9600; // baudrate tipico
	uartHandle.Init.WordLength = UART_WORDLENGTH_8B; // tamano de los datos >> 8 bits
	uartHandle.Init.StopBits = UART_STOPBITS_1; // cantidad de stop bits >> 1 solo
	uartHandle.Init.Parity = UART_PARITY_NONE; // tipo de paridad >> ninguno
	uartHandle.Init.Mode = UART_MODE_TX_RX; // modo recepcion/recepcion habilitado
	uartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE; // sin control de flujo por hardware
	uartHandle.Init.OverSampling = UART_OVERSAMPLING_16; // cantidad de muestras por tiempo de bit

	if (HAL_UART_Init(&uartHandle) == HAL_OK){ // retornar true si es exitosa la configuracion
		//print_uart_config(uartHandle); // imprimir configuracion si la config fue exitosa
		returnFlag = true;
	}

	return returnFlag;
}

void uart_transmit( uint8_t * pData, uint16_t Size, uint32_t Timeout){
	HAL_UART_Transmit(&uartHandle, pData, Size, Timeout); // enviar caracter x caracter
}

void uart_receive( uint8_t * pData, uint16_t Size, uint32_t Timeout){
	HAL_UART_Receive(&uartHandle, pData, Size, Timeout);
}
