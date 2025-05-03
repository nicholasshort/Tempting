/*
 * DPS368-Barometer.c
 *
 *  Created on: Apr 7, 2025
 *      Author: nickshort
 */

#include "DPS368-Barometer.h"

static float raw_scaled_pressure = 0.0f;
static float raw_scaled_temp = 0.0f;

uint8_t DPS368_Calibration_Coeffs[DPS368_NUM_COEFFS] = {0};

HAL_StatusTypeDef DPS368_ReadCoeffs() {

	HAL_Delay(100); // Wait for coefficients to be saved to registers on startup

	HAL_StatusTypeDef i2c_status;

	for (uint8_t regAddr = DPS368_REG_COEF_START; regAddr <= DPS368_REG_COEF_END; regAddr++) {

		i2c_status =  DPS368_RegRead(regAddr, &DPS368_Calibration_Coeffs[regAddr - DPS368_REG_COEF_START]);

		if(i2c_status != HAL_OK) {

			return regAddr;

		}

	}

	return HAL_OK;

}

HAL_StatusTypeDef DPS368_Init() {

	RETURN_IF_ERROR(DPS368_ReadCoeffs());

	RETURN_IF_ERROR(DPS368_RegWrite(DPS368_REG_PRS_CFG, 0x05)); // 32x Pressure Oversampling Rate (Must use bit shift!!)
	uint8_t data;
	DPS368_RegRead(DPS368_REG_PRS_CFG, &data);

	uint8_t temp_sense_source = 0;
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_COEF_SRCE, &temp_sense_source));
	temp_sense_source &= 0x80;

	uint8_t temp_config = 0x05 | temp_sense_source; // 32x Temperature Oversampling Rate (Must use bit shift!!) and use correct temp sensor according to coeffs
	RETURN_IF_ERROR(DPS368_RegWrite(DPS368_REG_TMP_CFG, temp_config));
	DPS368_RegRead(DPS368_REG_TMP_CFG, &data);

	RETURN_IF_ERROR(DPS368_RegWrite(DPS368_REG_CFG_REG, 0x0C)); // Enable bit shift for temp and pressure data reegisters
	DPS368_RegRead(DPS368_REG_CFG_REG, &data);

	return HAL_OK;

}

HAL_StatusTypeDef DPS368_GetTemperature(float* temp) {

	RETURN_IF_ERROR(DPS368_RegWrite(DPS368_REG_MEAS_CFG, 0x02)); // Set command mode for temp measurement

	HAL_Delay(100);

	uint8_t temp_msb, temp_lsb, temp_xlsb;
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_TMP_B2, &temp_msb));
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_TMP_B1, &temp_lsb));
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_TMP_B0, &temp_xlsb));
	int32_t raw_temp = (temp_msb << 16) | (temp_lsb << 8) | temp_xlsb;
	if (raw_temp & 0x00800000) raw_temp |= 0xFF000000; // Sign extend since 24 bit in 32 bit frame

	float kT = 516096.0f; // 32 oversampling scale factor

	raw_scaled_temp = raw_temp / kT;

	int16_t c0 = ((DPS368_Calibration_Coeffs[0] << 4) | (DPS368_Calibration_Coeffs[1] >> 4));
	if (c0 & 0x0800) c0 |= 0xF000; // Sign extend since its 12 bit in 16 bit frame

	int16_t c1 = ((DPS368_Calibration_Coeffs[1] & 0x0F) << 8) | DPS368_Calibration_Coeffs[2];
	if (c1 & 0x0800) c1 |= 0xF000; // Sign extend since its 12 bit in 16 bit frame

	*temp = (c0*0.5 + c1*raw_scaled_temp);

	return HAL_OK;

}

HAL_StatusTypeDef DPS368_GetPressure(float* pressure) {

	RETURN_IF_ERROR(DPS368_RegWrite(DPS368_REG_MEAS_CFG, 0x01)); // Set command mode for pressure measurement

	HAL_Delay(100);

	uint8_t pressure_msb, pressure_lsb, pressure_xlsb;
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_PSR_B2, &pressure_msb));
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_PSR_B1, &pressure_lsb));
	RETURN_IF_ERROR(DPS368_RegRead(DPS368_REG_PSR_B0, &pressure_xlsb));
	int32_t raw_pressure = (pressure_msb << 16) | (pressure_lsb << 8) | pressure_xlsb;
	if (raw_pressure & 0x00800000) raw_pressure |= 0xFF000000; // Sign extend since 24 bit in 32 bit frame

	float kP = 516096.0f; // 32 oversampling scale factor

	raw_scaled_pressure = raw_pressure / kP;

	int32_t c00 = (DPS368_Calibration_Coeffs[3] << 12) | (DPS368_Calibration_Coeffs[4] << 4) | ((DPS368_Calibration_Coeffs[5] & 0xF0) >> 4);
	if (c00 & 0x00080000) c00 |= 0xFFF00000; // Sign extend since its 20 bit in 32 bit frame

	int32_t c10 = ((DPS368_Calibration_Coeffs[5] & 0x0F) << 16) | (DPS368_Calibration_Coeffs[6] << 8) | (DPS368_Calibration_Coeffs[7]);
	if (c10 & 0x00080000) c10 |= 0xFFF00000; // Sign extend since its 20 bit in 32 bit frame

	int16_t c01 = (DPS368_Calibration_Coeffs[8] << 8) | (DPS368_Calibration_Coeffs[9]);
	int16_t c11 = (DPS368_Calibration_Coeffs[10] << 8) | (DPS368_Calibration_Coeffs[11]);
	int16_t c20 = (DPS368_Calibration_Coeffs[12] << 8) | (DPS368_Calibration_Coeffs[13]);
	int16_t c21 = (DPS368_Calibration_Coeffs[14] << 8) | (DPS368_Calibration_Coeffs[15]);
	int16_t c30 = (DPS368_Calibration_Coeffs[16] << 8) | (DPS368_Calibration_Coeffs[17]);

	*pressure = c00 + raw_scaled_pressure*(c10 + raw_scaled_pressure*(c20 + raw_scaled_pressure*c30)) + raw_scaled_temp*c01 + raw_scaled_temp*raw_scaled_pressure*(c11*raw_scaled_pressure*c21);

	return HAL_OK;

}


HAL_StatusTypeDef DPS368_RegWrite(uint8_t regAddr, uint8_t regData) {

	return HAL_I2C_Mem_Write(&DPS368_I2C_HANDLE, DPS368_I2C_ADDRESS, regAddr, I2C_MEMADD_SIZE_8BIT, &regData, 1, HAL_MAX_DELAY);

}

HAL_StatusTypeDef DPS368_RegRead(uint8_t regAddr, uint8_t* regData) {

	return HAL_I2C_Mem_Read(&DPS368_I2C_HANDLE, DPS368_I2C_ADDRESS, regAddr, I2C_MEMADD_SIZE_8BIT, regData, 1, HAL_MAX_DELAY);

}


