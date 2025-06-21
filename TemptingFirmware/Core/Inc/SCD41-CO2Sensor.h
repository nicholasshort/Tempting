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

#endif /* INC_SCD41_CO2SENSOR_H_ */
