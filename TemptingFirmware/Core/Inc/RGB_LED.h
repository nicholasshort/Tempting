/*
 * RGB_LED.h
 *
 *  Created on: May 13, 2025
 *      Author: nick
 */

#ifndef INC_RGB_LED_H_
#define INC_RGB_LED_H_

#include "main.h"
#include "tim.h"

#define RGB_LED_RED_MAX_INTENSITY		100
#define RGB_LED_GREEN_MAX_INTENSITY 	50
#define RGB_LED_BLUE_MAX_INTENSITY 		100

void RGB_LED_Init();
void RGB_LED_Set_Intensity(uint8_t red, uint8_t green, uint8_t blue);

#endif /* INC_RGB_LED_H_ */
