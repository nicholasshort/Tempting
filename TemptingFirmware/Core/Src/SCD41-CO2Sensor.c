/*
 * SCD41-CO2Sensor.c
 *
 *  Created on: Apr 8, 2025
 *      Author: nickshort
 */

#include "SCD41-CO2Sensor.h"

static HAL_StatusTypeDef SCD41_VerifyCheckSum(uint8_t read_msb, uint8_t read_lsb, uint8_t crc) {

    uint8_t data[2] = {read_msb, read_lsb};
    uint8_t computed_crc = 0xFF;

    for (uint8_t i = 0; i < 2; i++) {
        computed_crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (computed_crc & 0x80) {
                computed_crc = (computed_crc << 1) ^ 0x31;
            } else {
                computed_crc <<= 1;
            }
        }
    }
    return (computed_crc == crc) ? HAL_OK : HAL_ERROR;

}

static HAL_StatusTypeDef SCD41_GenerateCheckSum(uint8_t write_msb, uint8_t write_lsb, uint8_t* crc) {

    uint8_t data[2] = {write_msb, write_lsb};
    uint8_t computed_crc = 0xFF;

    for (uint8_t i = 0; i < 2; i++) {
        computed_crc ^= data[i];
        for (uint8_t bit = 0; bit < 8; bit++) {
            if (computed_crc & 0x80) {
                computed_crc = (computed_crc << 1) ^ 0x31;
            } else {
                computed_crc <<= 1;
            }
        }
    }
    *crc = computed_crc;
    return HAL_OK;

}

HAL_StatusTypeDef SCD41_GetSensorVariant(uint16_t* variantCode) {

	RETURN_IF_ERROR(SCD41_ReadSequence(0x202F, variantCode, 1, 1));

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_StartPeriodicMeasurement(void) {

	RETURN_IF_ERROR(SCD41_SendSequence(0x21B1));

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_ReadMeasurement(uint16_t* co2_ppm, float* temp, float* rh) {

	bool dataReady;
	RETURN_IF_ERROR(SCD41_GetDataReadyStatus(&dataReady));
	if (!dataReady)
		return HAL_ERROR;

	// Get rest of data
	uint16_t measurement_data[3];
	RETURN_IF_ERROR(SCD41_ReadSequence(0xEC05, measurement_data, 1, 1));

	*co2_ppm = measurement_data[0];

	// TODO Fix Read sequence with multiple frames to get temperature and rh data
	// For future reference, in debug mode, stepping through multiple frames doesn't produce an error, but the temp and rh values are off. Check Sum seems to be valid for each though.

	*temp = 0.0f;
	*rh = 0.0f;
//	*temp = -45 + 175*((float)measurement_data[1] / (65536 - 1));
//
//	*rh = 100*((float)measurement_data[2] / (65536 - 1));

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_StopPeriodicMeasurement(void) {

	RETURN_IF_ERROR(SCD41_SendSequence(0x3F86));

	HAL_Delay(500); // Wait 500ms for SCD41 to stop as per datasheet

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_GetAmbientPressure(uint32_t* pressure) {

	uint16_t readData;
	RETURN_IF_ERROR(SCD41_ReadSequence(0xE000, &readData, 1, 1));

	*pressure = ((int32_t)readData)*100;

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_SetAmbientPressure(uint32_t pressure) {

	uint16_t encodedPressure = (uint16_t)(pressure / 100);
	RETURN_IF_ERROR(SCD41_WriteSequence(0xE000, encodedPressure));

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_GetDataReadyStatus(bool* dataReady) {

	uint16_t readData;
	RETURN_IF_ERROR(SCD41_ReadSequence(0xE4B8, &readData, 1, 1));

	*dataReady = ((readData & 0x07FF) != 0);

	return HAL_OK;

}


// Command Sequence Functions
HAL_StatusTypeDef SCD41_SendSequence(uint16_t hexCommand) {

	uint8_t hexCommandBytes[2];
	hexCommandBytes[0] = (uint8_t)(hexCommand >> 8);
	hexCommandBytes[1] = (uint8_t)hexCommand;

	RETURN_IF_ERROR(HAL_I2C_Master_Transmit(&SCD41_I2C_HANDLE, SCD41_I2C_ADDRESS, hexCommandBytes, 2, HAL_MAX_DELAY));

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_ReadSequence(uint16_t hexCommand, uint16_t* readData, uint8_t numReadFrames, uint16_t waitTime) {

	uint8_t hexCommandBytes[2];
	hexCommandBytes[0] = (uint8_t)(hexCommand >> 8);
	hexCommandBytes[1] = (uint8_t)hexCommand;

	RETURN_IF_ERROR(HAL_I2C_Master_Transmit(&SCD41_I2C_HANDLE, SCD41_I2C_ADDRESS, hexCommandBytes, 2, HAL_MAX_DELAY));

	if (waitTime > 0)
		HAL_Delay(waitTime);

	for (uint8_t i = 0; i < numReadFrames; i++) {
		uint8_t readBuff[3];
		RETURN_IF_ERROR(HAL_I2C_Master_Receive(&SCD41_I2C_HANDLE, SCD41_I2C_ADDRESS, readBuff, 3, HAL_MAX_DELAY));

		RETURN_IF_ERROR(SCD41_VerifyCheckSum(readBuff[0], readBuff[1], readBuff[2]));

		readData[i] = readBuff[0] << 8 | readBuff[1];

	}


	return HAL_OK;

}

HAL_StatusTypeDef SCD41_WriteSequence(uint16_t hexCommand, uint16_t writeData) {

	uint8_t writeDataBytes[3];
	writeDataBytes[0] = (uint8_t)(writeData >> 8);
	writeDataBytes[1] = (uint8_t)writeData;

	SCD41_GenerateCheckSum(writeDataBytes[0], writeDataBytes[1], &writeDataBytes[2]);

	RETURN_IF_ERROR(HAL_I2C_Mem_Write(&SCD41_I2C_HANDLE, SCD41_I2C_ADDRESS, hexCommand, I2C_MEMADD_SIZE_16BIT, writeDataBytes, 3, HAL_MAX_DELAY));

	return HAL_OK;

}

HAL_StatusTypeDef SCD41_SendFetchSequence(uint16_t hexCommand, uint16_t writeData, uint16_t* readData, uint16_t waitTime) {

	return HAL_OK;

}
