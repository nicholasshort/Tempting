/*
 * SSD1309-OLED.h
 *
 *  Created on: Apr 11, 2025
 *      Author: nickshort
 */

#ifndef INC_SSD1309_OLED_H_
#define INC_SSD1309_OLED_H_

#include "main.h"

#define SSD1309_SPI_HANDLE  hspi1

#define SSD1309_WIDTH    	128
#define SSD1309_HEIGHT   	64

#define SSD1309_RST_PORT    GPIOA
#define SSD1309_RST_PIN     GPIO_PIN_4
#define SSD1309_CS_PORT     GPIOA
#define SSD1309_CS_PIN      GPIO_PIN_6
#define SSD1309_DC_PORT     GPIOA
#define SSD1309_DC_PIN      GPIO_PIN_8

#define SSD1309_CS_LOW()    HAL_GPIO_WritePin(SSD1309_CS_PORT, SSD1309_CS_PIN, GPIO_PIN_RESET)
#define SSD1309_CS_HIGH()   HAL_GPIO_WritePin(SSD1309_CS_PORT, SSD1309_CS_PIN, GPIO_PIN_SET)
#define SSD1309_DC_LOW()    HAL_GPIO_WritePin(SSD1309_DC_PORT, SSD1309_DC_PIN, GPIO_PIN_RESET)
#define SSD1309_DC_HIGH()   HAL_GPIO_WritePin(SSD1309_DC_PORT, SSD1309_DC_PIN, GPIO_PIN_SET)
#define SSD1309_RST_LOW()   HAL_GPIO_WritePin(SSD1309_RST_PORT, SSD1309_RST_PIN, GPIO_PIN_RESET)
#define SSD1309_RST_HIGH()  HAL_GPIO_WritePin(SSD1309_RST_PORT, SSD1309_RST_PIN, GPIO_PIN_SET)

extern SPI_HandleTypeDef 		SSD1309_SPI_HANDLE;

HAL_StatusTypeDef SSD1309_Init(void);
HAL_StatusTypeDef SSD1309_SendCommand(uint8_t cmd);
HAL_StatusTypeDef SSD1309_SendData(uint8_t data);
HAL_StatusTypeDef SSD1309_Clear(void);
HAL_StatusTypeDef SSD1309_Update(void);
HAL_StatusTypeDef SSD1309_DrawPixel(uint8_t x, uint8_t y, uint8_t color);

#endif /* INC_SSD1309_OLED_H_ */
