/*
 * SSD1309-OLED.c
 *
 *  Created on: Apr 11, 2025
 *      Author: nickshort
 */

#include "SSD1309-OLED.h"

static uint8_t ssd1309_buffer[SSD1309_WIDTH * SSD1309_HEIGHT / 8];

HAL_StatusTypeDef SSD1309_Reset(void) {

    SSD1309_RST_LOW();
    HAL_Delay(10);
    SSD1309_RST_HIGH();
    HAL_Delay(10);

    return HAL_OK;

}

HAL_StatusTypeDef SSD1309_SendCommand(uint8_t cmd) {

    SSD1309_CS_LOW();
    SSD1309_DC_LOW();
    HAL_StatusTypeDef ret = HAL_SPI_Transmit(&SSD1309_SPI_HANDLE, &cmd, 1, HAL_MAX_DELAY);
    SSD1309_CS_HIGH();

    return ret;

}

HAL_StatusTypeDef SSD1309_SendData(uint8_t data) {

    SSD1309_CS_LOW();
    SSD1309_DC_HIGH();
    HAL_StatusTypeDef ret = HAL_SPI_Transmit(&SSD1309_SPI_HANDLE, &data, 1, HAL_MAX_DELAY);
    SSD1309_CS_HIGH();

    return ret;

}


HAL_StatusTypeDef SSD1309_Init(void) {

//	RETURN_IF_ERROR(SSD1309_Reset());
//
//    RETURN_IF_ERROR(SSD1309_SendCommand(0xAE)); // Display OFF
//
//    RETURN_IF_ERROR(SSD1309_SendCommand(0x20)); RETURN_IF_ERROR(SSD1309_SendCommand(0x00)); // Set memory addressing mode to horizontal
//
//    RETURN_IF_ERROR(SSD1309_Clear()); // Clear Buffer
//    RETURN_IF_ERROR(SSD1309_Update()); // Update Screen
//
//    RETURN_IF_ERROR(SSD1309_SendCommand(0xAF));

	SSD1309_Reset();

	SSD1309_SendCommand(0xAE);        // Display OFF
	SSD1309_SendCommand(0x81);        // Set contrast
	SSD1309_SendCommand(0xFF);        // Max brightness
	SSD1309_SendCommand(0xA5);        // Entire display ON (ignores GDDRAM)
	SSD1309_SendCommand(0xAF);        // Display ON


    return HAL_OK;

}

HAL_StatusTypeDef SSD1309_Clear(void) {

    for (uint16_t i = 0; i < sizeof(ssd1309_buffer); i++) {
        ssd1309_buffer[i] = 0x00;
    }
    return HAL_OK;

}

HAL_StatusTypeDef SSD1309_Update(void) {

    for (uint8_t page = 0; page < (SSD1309_HEIGHT / 8); page++) {
    	SSD1309_DC_HIGH();
        SSD1309_CS_LOW();
        HAL_StatusTypeDef ret = HAL_SPI_Transmit(&SSD1309_SPI_HANDLE,
            &ssd1309_buffer[SSD1309_WIDTH * page],
            SSD1309_WIDTH,
            HAL_MAX_DELAY);
        SSD1309_CS_HIGH();
        if (ret != HAL_OK) return ret;
    }

    return HAL_OK;

}

HAL_StatusTypeDef SSD1309_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {

    if (x >= SSD1309_WIDTH || y >= SSD1309_HEIGHT) return HAL_ERROR;

    uint16_t index = x + (y / 8) * SSD1309_WIDTH;
    if (color)
        ssd1309_buffer[index] |= (1 << (y % 8));
    else
        ssd1309_buffer[index] &= ~(1 << (y % 8));

    return HAL_OK;

}
