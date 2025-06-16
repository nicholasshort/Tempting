/*
 * SCD41-CO2Sensor.h
 *
 *  Created on: Apr 8, 2025
 *      Author: nickshort
 */

#ifndef INC_SCD41_CO2SENSOR_H_
#define INC_SCD41_CO2SENSOR_H_

#include "main.h"
#include "i2c.h"
#include "stdbool.h"

#define SCD41_I2C_HANDLE		hi2c1

#define SCD41_I2C_ADDRESS 		(0x62 << 1)

extern I2C_HandleTypeDef SCD41_I2C_HANDLE;

// Test commands
HAL_StatusTypeDef SCD41_GetSensorVariant(uint16_t* variantCode);

// Basic commands
HAL_StatusTypeDef SCD41_StartPeriodicMeasurement(void);
HAL_StatusTypeDef SCD41_ReadMeasurement(uint16_t* co2_ppm, float* temp, float* rh);
HAL_StatusTypeDef SCD41_StopPeriodicMeasurement(void);

// On-chip output signal compensation
HAL_StatusTypeDef SCD41_GetAmbientPressure(uint32_t* pressure);
HAL_StatusTypeDef SCD41_SetAmbientPressure(uint32_t pressure);

// Low power periodic measurement functions
HAL_StatusTypeDef SCD41_GetDataReadyStatus(bool* dataReady);

// Command Sequence Functions
HAL_StatusTypeDef SCD41_SendSequence(uint16_t hexCommand);
HAL_StatusTypeDef SCD41_ReadSequence(uint16_t hexCommand, uint16_t* readData, uint8_t numReadFrames, uint16_t waitTime);
HAL_StatusTypeDef SCD41_WriteSequence(uint16_t hexCommand, uint16_t writeData);
HAL_StatusTypeDef SCD41_SendFetchSequence(uint16_t hexCommand, uint16_t writeData, uint16_t* readData, uint16_t waitTime);

#endif /* INC_SCD41_CO2SENSOR_H_ */
