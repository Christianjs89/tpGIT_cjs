/*
 * API_lcd_i2c.c
 *
 *  Created on: Nov 15, 2023
 *      Author: chris
 */

#include "API_lcd_i2c.h"

static I2C_HandleTypeDef i2cHandle; // bring all the i2c config from main.c here


/*************************************** LCD INSTRUCTIONS *********************************************
 *
 * RS  RW  DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
 * 0	0	0	0	0	0	0	0	0	1		>> CLEAR SCREEN
 * 0	0	0	0	0	0	0	0	1	x		>> RETURN HOME (GO TO POSITION 0;0)
 * 0	0	0	0	0	0	0	1	ID	S		>> ENTRY MODE ID= 1-increment 0-decrement / S=1-shift display
 * 0	0	0	0	0	0	1	D	C	B		>> DISPLAY CONTROL D=1-display ON, C=1-Cursor ON, B=1-blink ON
 * 0	0	0	0	0	1	SC	RL	x	x		>> CURSOR DISPLAY SHIFT SC=1-display shift 0-cursor move, RL=1-right 0-left
 * 0	0	0	0	1	DL	N	F	x	x		>> Function Set DL=1-8bit mode 0-4bit mode, N=1-2 lines 0-1line, F=15x10 0-5x8
 * 0	0	0	1	ACG	ACG	ACG	ACG	ACG	ACG		>> CGRAM address, data is sent after this command
 * 0	0	1	ADD	ADD	ADD	ADD	ADD	ADD	ADD		>> DDRAM address (lcd position), data is sent after this command
 *
 */
/**************************** WRITE TO LCD FUNCTIONS ***************************/

/* Serial->Parallel: output Byte Px: D7 D6 D5 D4 BT E RW RS */


// send a byte to the LCD / chose RS_COMMAND/RS_DATA, RW_READ/RW_WRITE
// Envia un byte de dato/comando al LCD
// Parametros:
// <rs> tipo de byte de comando o instruccion
// <RW> tipo de comunicacion con el LCD (lectura/escritura
// Retorno: ninguno
void lcd_send_byte(uint8_t byte, bool rs, bool rw){
	// byte contains 8 bits of information  / byteType can be INSTRUCTION or DATA
	uint8_t upperByte = (byte & HIGH_NIBBLE); // mask with 1111 0000
	uint8_t lowerByte = (byte << 4); // shift 4 to the left so lower nibble becomes high

	// add rs and rw bits
	upperByte |= ((rs<<RS_POS) | (rw<<RW_POS) | (BT_ON<<BT_POS));
	lowerByte |= ((rs<<RS_POS) | (rw<<RW_POS) | (BT_ON<<BT_POS));
	// E bit 1 0 1 0 to create pulses in LCD controller

	uint8_t byteSequence[4] = {
			(upperByte | (E_HIGH<<E_POS)),
			(upperByte | (E_LOW<<E_POS)),
			(lowerByte | (E_HIGH<<E_POS)),
			(lowerByte | (E_LOW<<E_POS))
	};

	send_bytes_i2c(LCD_ADDRESS, byteSequence, 4, I2C_WRITE);
	HAL_Delay(5);

}

// send bytes over I2C / specify if it's a READ/WRITE operation
// Envia un byte por I2C, especificando si la instruccion es de lectura o escritura al modulo I2C
// Parametros: <slaveAddress> direccion del modulo I2C (7 bits)
// <byteSequence> vector con los bytes a transmitir
// <sequenceSize> tamano del vector con bytes a transmitir
// <i2c_rw> modo de comunicacion read/write al al modulo I2C
// Retorno: ninguno
void send_bytes_i2c(uint8_t slaveAddress, uint8_t byteSequence[], uint8_t sequenceSize, bool i2c_rw){
	slaveAddress = ((slaveAddress<<1) | i2c_rw); // i2c WRITE or READ
	HAL_I2C_Master_Transmit(&i2cHandle, slaveAddress, byteSequence, sequenceSize, 100);
}

// Despeja la pantalla LCD por completo
// Parametros: ninguno
// Retorno: ninguno
void lcd_clear(){
	lcd_send_byte(0x01, RS_INSTRUCTION, RW_WRITE);
}

// Posicionar el cursor en la posicion 0,0 del LCD
// Parametros: ninguno
// Retorno: ninguno
void return_home(){
	lcd_send_byte(0x02, RS_INSTRUCTION, RW_WRITE);
}


// Posiciona el cursor del LCD en la fila y columna seleccionada
// Parametros: <row> fila [1,2] , <column> columna [1,16]
// Retorno: ninguno
void lcd_set_position(uint8_t row, uint8_t column){
	// for LCD 16x2 check ranges
	if(row>2) row = 2;
	if(row<1) row = 1;
	if(column>16) column = 16;
	if(column<1) column = 1;

	uint8_t ddram = ddram_address_16x2[row-1][column-1];
	uint8_t ddram_cmd = ddram | (1<<7); // add a 1 in DB7 for DDRAM command
	lcd_send_byte(ddram_cmd, RS_INSTRUCTION, RW_WRITE); // send address as instruction, not data
}


// Imprime un vector de caracteres al LCD a partir de la ultima posicion en uso
// Utilizar en conjunto con lcd_set_position
// Parametros: <text> vector de caracteres a imprimir , <size> tamano del vector de caracteres
// Retorno: ninguno
void lcd_print_text(uint8_t text[], uint8_t size){

	for(uint8_t i = 0; i < size-1; i++){ // -1 to exclude the '/0' char
		HAL_Delay(1); // without this some chars go missing
		lcd_send_byte(text[i], RS_DATA, RW_WRITE);
	}
}


// Control de la luz de fondo del LCD
// Parametros: <state> 1-encendido 0-apagado
// Retorno: ninguno
void control_backlight(bool state){
	uint8_t cmd = (state<<BT_POS); // send straight to I2C / not part of the LCD controller
	uint8_t slaveAddress = ((LCD_ADDRESS<<1) | I2C_WRITE); // i2c WRITE or READ
	HAL_I2C_Master_Transmit(&i2cHandle, slaveAddress, cmd, 1, 100);
}


// Desplazamiento del display y/o cursor
// Cada llamado desplaza el cursor y/o display 1 espacio
// Parametros: <shiftType> desplazamiento del cursor o display , <direction> a la izquierda or derecha
// Retorno: ninguno
void shift_display(bool shiftType, bool direction){ // DISPLAY_SHIFT / CURSOR_SHIFT - SHIFT_RIGHT / SHIFT_LEFT
	uint8_t cmd = ((direction<<SHIFT_DIRECTION_POS)|(shiftType<<CURSOR_DISPLAY_SHIFT_POS));
	lcd_send_byte(0x1C, 0, 0); // shift display to the right
}


// Creacion de un caracter nuevo en la memoria CGRAM del LCD
// Parametros: <index> posicion en memoria donde almacenar el vector de 8 bytes del caracter [0,7]
// <mychar> vector de 8 bytes correspondientes al caracter deseado a almacenar en la CGRAM
// Retorno: ninguno
void create_character(uint8_t index, uint8_t mychar[]){
	// https://maxpromer.github.io/LCD-Character-Creator/
	if(index<0) index = 0;
	if(index>7) index = 7;

	lcd_send_byte(0x40 + 8*index, RS_INSTRUCTION, RW_WRITE); // set cgram memory from 0x40, 0x48, 0x50, etc +8
	HAL_Delay(50);

	for(uint8_t i = 0; i < 8; i++){
		lcd_send_byte(mychar[i], RS_DATA, RW_WRITE);
		HAL_Delay(10);
	}
	HAL_Delay(100);

}


// Sin uso, debe enviarse el set_position luego de crear el caracter
//void lcd_print_custom_character(uint8_t customCharArray[], uint8_t index){
//	create_character(index, customCharArray);
//	lcd_send_byte(index, RS_DATA, RW_WRITE);
//}


// Inicializacion del LCD de acuerdo con el proceso descripto en la hoja de datos
// Parametros: ninguno
// Retorno: ninguno
void lcd_init(){
	// initialization sequence p46 HD44780 datasheet
	HAL_Delay(60); // wait >40 ms
	lcd_send_byte(0x30, RS_INSTRUCTION, RW_WRITE); // 0 0 1 1 x x x x -> 0x30
	HAL_Delay(10); // wait > 4 ms
	lcd_send_byte(0x30, RS_INSTRUCTION, RW_WRITE);
	HAL_Delay(10); // wait > 0.1 ms
	lcd_send_byte(0x30, RS_INSTRUCTION, RW_WRITE);
	HAL_Delay(10);
	lcd_send_byte(0x20, RS_INSTRUCTION, RW_WRITE); // 0 0 1 DL=0 x x x x -> 0x20 / Function set: DL=4-bit mode
	// start in 4 bit mode
	lcd_send_byte(0x28, RS_INSTRUCTION, RW_WRITE); // 0 0 1 0 N=1 F=0 x x -> 0x28 / Function set: N=2-lines, F=5x8
	HAL_Delay(10);
	lcd_send_byte(0x08, RS_INSTRUCTION, RW_WRITE); // 0 0 0 0 1 D=0 C=0 B=0 -> 0x08 / Display control: D=display off, C=cursor off, B=blink off
	HAL_Delay(10);
	lcd_clear();
	HAL_Delay(10);
	lcd_send_byte(0x06, RS_INSTRUCTION, RW_WRITE); // 0 0 0 0 0 1 ID=1 S=0 -> 0x06/ Entry mode: ID=increment, S=no display shift
	HAL_Delay(10);
	lcd_send_byte(0x0C, RS_INSTRUCTION, RW_WRITE); // 0 0 0 0 1 D=1 C=0 B=0 -> 0x0C / Display control:D-display on, C=cursor off, B=blink off
	HAL_Delay(10);
	lcd_clear();
	HAL_Delay(10);
	return_home();
	HAL_Delay(10);
}

// Vinculacion del handle de I2C en caso de crear el handle en main.c
// Sin uso en el programa actual > handle local privado
void i2c_linker(I2C_HandleTypeDef * i2cInstance){
	i2cHandle = *i2cInstance;
}

// Inicializacion del periferico I2C1 configurado con CubeMX
// Parametros: ninguno
// Retorno: ninguno
void i2c_init(void){

	i2cHandle.Instance = I2C1;
	i2cHandle.Init.ClockSpeed = 100000;
	i2cHandle.Init.DutyCycle = I2C_DUTYCYCLE_2;
	i2cHandle.Init.OwnAddress1 = 0;
	i2cHandle.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	i2cHandle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2cHandle.Init.OwnAddress2 = 0;
	i2cHandle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2cHandle.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&i2cHandle) != HAL_OK)
  {
    //Error_Handler();
  }
}
