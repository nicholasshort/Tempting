/*
 * RGB_LED.c
 *
 *  Created on: May 13, 2025
 *      Author: nick
 */

#include <RGB_LED.h>

void RGB_LED_Init() {

	htim2.Instance->CCR1 = 100;
	htim2.Instance->CCR2 = 100;
	htim2.Instance->CCR3 = 100;

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

}

void RGB_LED_Set_Intensity(uint8_t red, uint8_t green, uint8_t blue) {

	htim2.Instance->CCR1 = 100 - red;
	htim2.Instance->CCR2 = 100 - green;
	htim2.Instance->CCR3 = 100 - blue;

}
