/*
 * API_uart.h
 *
 *  Created on: Nov 19, 2023
 *      Author: chris
 */

#ifndef API_INC_API_UART_H_
#define API_INC_API_UART_H_



/* INCLUDES */
#include "stm32f1xx.h"
#include "stdbool.h"
#include "stm32f1xx_hal_uart.h"


/* DEFINICIONES */
#define SEND_BUFFER_MAX_SIZE 200 // maxima cantidad de caracteres a transmitir
#define RECEIVE_BUFFER_MAX_SIZE 200 // maxima cantidad de caracteres a recibir

#define USE_USART USART2 // UART a utilizar

/* DECLARACION DE TIPO DE DATOS */
typedef bool bool_t;

/* DECLARACIONES DE FUNCIONES */

/* > Descripcion: Inicializa la uart con la configuracion preestablecida y envia dicha config a la terminal
 * > Parametro: ninguno
 * > Retorno: retorna un boolt_t true en caso exito o false caso contrario
 */
bool_t uartInit();

/* > Descripcion: envia un string (buffer <pstring>) por la uart, finalizando en el caracter NULL
 * > Parametro: <pstring> puntero del tipo uint8_t
 * 				El buffer de envio tendra una cantidad maxima de caracteres definida en <SEND_BUFFER_MAX_SIZE>
 * > Retorno: ninguno
 */
void uartSendString(uint8_t * pstring);

/* > Descripcion: envia una cantidad de caracteres contenidas en un string (buffer <pstring>) por la uart
 * > Parametro: <pstring> puntero del tipo uint8_t / <size> cantidad de caracteres a enviar rango [1-2^16]
 * 				El buffer de envio tendra una cantidad maxima de caracteres definida en <SEND_BUFFER_MAX_SIZE>
 * > Retorno: ninguno
 */
void uartSendStringSize(uint8_t * pstring, uint16_t size);

/* > Descripcion: recibe una cantidad de caracteres contenidas en un string por la uart y las almacena en el buffer <pstring>
 * > Parametro: <pstring> puntero del tipo uint8_t / <size> cantidad de caracteres a recibir rango [1-2^16]
 * > 			El buffer de recepcion tendra una cantidad maxima de caracteres definida en <RECEIVE_BUFFER_MAX_SIZE>
 * > Retorno: ninguno
 */
void uartReceiveStringSize(uint8_t * pstring, uint16_t size);

void uartReceiveString(uint8_t * pstring, uint16_t size);

/* > Descripcion: imprime a la terminal serie la configuracion de la UART
 * > Parametro: <uart> handler de la uart
 * > Retorno: ninguno
 */
void print_uart_config(UART_HandleTypeDef uartHandle);




#endif /* API_INC_API_UART_H_ */


