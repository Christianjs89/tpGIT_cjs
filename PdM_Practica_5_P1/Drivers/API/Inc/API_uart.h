/*
 * API_uart.h
 *
 *  Created on: Nov 19, 2023
 *      Author: chris
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_

#endif /* API_INC_API_UART_H_ */


/* INCLUDES */
//#include "stm32f1xx_hal_uart.h" // incluir funciones de la HAL uart
//#include "stm32f1xx_hal_def.h"
#include "stm32f1xx.h"
#include "stdbool.h"
#include "stm32f1xx_hal_uart.h"


/* DEFINICIONES */
#define SEND_BUFFER_MAX_SIZE 32 // maxima cantidad de caracteres a transmitir
#define RECEIVE_BUFFER_MAX_SIZE 32 // maxima cantidad de caracteres a recibir

#define USE_USART USART2 // UART a utilizar

/* DECLARACION DE TIPO DE DATOS */
typedef bool bool_t;

/* DECLARACIONES DE FUNCIONES */

/* uartInit
 * > Descripcion: Inicializa la uart con la configuracion preestablecida e imprime dicha config a la terminal
 * > Parametro: ninguno
 * > Retorno: retorna un boolt_t true en caso exito o false caso contrario
 */
bool_t uartInit();

/* uartSendString
 * > Descripcion: envia un string por la uart, finalizando en el caracter NULL
 * > Parametro: puntero pstring del tipo uint8_t > utilizar como &miTexto
 * > Retorno: ninguno
 */
void uartSendString(uint8_t * pstring);

/* > Descripcion: envia una cantidad de caracteres contenidas en un string por la uart
 * > Parametro: <pstring> puntero del tipo uint8_t / <size> cantidad de caracteres a enviar rango [1-2^16]
 * > Retorno: ninguno
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size);


void uartReceiveStringSize(uint8_t * pstring, uint16_t size);
void print_uart_config(UART_HandleTypeDef uartHandle);
