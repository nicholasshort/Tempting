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

#define DPS368_I2C_ADDRESS 		(0x77 << 1)

#define DPS368_REG_PSR_B2		0x00
#define DPS368_REG_PSR_B1		0x01
#define DPS368_REG_PSR_B0		0x02
#define DPS368_REG_TMP_B2		0x03
#define DPS368_REG_TMP_B1		0x04
#define DPS368_REG_TMP_B0		0x05
#define DPS368_REG_PRS_CFG		0x06
#define DPS368_REG_TMP_CFG		0x07
#define DPS368_REG_MEAS_CFG 	0x08
#define DPS368_REG_CFG_REG		0x09
#define DPS368_REG_INT_STS		0x0A
#define DPS368_REG_FIFO_STS		0x0B
#define DPS368_REG_RESET		0x0C
#define DPS368_REG_PROD_REV_ID	0x0D
#define DPS368_REG_COEF_START	0x10
#define DPS368_REG_COEF_END		0x21
#define DPS368_REG_COEF_SRCE	0x28

#define DPS368_NUM_COEFFS 18
extern uint8_t DPS368_Calibration_Coeffs[DPS368_NUM_COEFFS];

extern I2C_HandleTypeDef DPS368_I2C_HANDLE;

HAL_StatusTypeDef DPS368_RegWrite(uint8_t regAddr, uint8_t regData);
HAL_StatusTypeDef DPS368_RegRead(uint8_t regAddr, uint8_t* regData);
HAL_StatusTypeDef DPS368_GetTemperature(float* temp);
HAL_StatusTypeDef DPS368_GetPressure(float* pressure);
HAL_StatusTypeDef DPS368_ReadCoeffs(void);
HAL_StatusTypeDef DPS368_Init(void);

#endif /* INC_DPS368_BAROMETER_H_ */
