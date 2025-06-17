/*
 * DPS368-Barometer.h
 *
 *  Created on: Apr 7, 2025
 *      Author: nickshort
 */

#ifndef INC_DPS368_BAROMETER_H_
#define INC_DPS368_BAROMETER_H_

#include "main.h"
#include "i2c.h"

#define DPS368_I2C_HANDLE		hi2c1

extern I2C_HandleTypeDef DPS368_I2C_HANDLE;

HAL_StatusTypeDef DPS368_GetTemperature(float* temp);
HAL_StatusTypeDef DPS368_GetPressure(float* pressure);
HAL_StatusTypeDef DPS368_Init(void);

#endif /* INC_DPS368_BAROMETER_H_ */
