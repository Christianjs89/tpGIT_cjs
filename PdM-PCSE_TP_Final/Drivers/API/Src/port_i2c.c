/*
 * port_i2c.c
 *
 *  Created on: Dec 6, 2023
 *      Author: chris
 *
 *      Wrapper de funciones especificas del periferico I2C
 */

#include "stm32f1xx.h"

static I2C_HandleTypeDef i2cHandle; // bring all the i2c config from main.c here

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

// Wrapper de la funcion especifica HAL de transmision I2C
void i2c_transmit(uint16_t DevAddress, uint8_t * pData, uint16_t Size, uint32_t Timeout){
	HAL_I2C_Master_Transmit(&i2cHandle, DevAddress, pData, Size, Timeout);
}

