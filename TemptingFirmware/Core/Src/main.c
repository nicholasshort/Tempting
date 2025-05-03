/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "memorymap.h"
#include "rf.h"
#include "spi.h"
#include "tim.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum {
    AIR_QUALITY_EXCELLENT,   // 400-500 ppm (Green)
    AIR_QUALITY_GOOD,        // 500-800 ppm (Light Green)
    AIR_QUALITY_FAIR,        // 800-1000 ppm (Yellow)
    AIR_QUALITY_MEDIOCRE,    // 1000-1500 ppm (Orange)
    AIR_QUALITY_BAD,         // 1500+ ppm (Red)
    AIR_QUALITY_HEAVILY_CONTAMINATED, // 1900+ ppm (Dark Red)
    AIR_QUALITY_VENTILATION_REQUIRED  // 1700+ ppm (Flashing Red)
} AirQualityLevel;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t led_rgb_intensity = 0;
uint16_t co2_ppm = 0;
AirQualityLevel current_air_quality = AIR_QUALITY_GOOD;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
AirQualityLevel Get_Air_Quality(uint16_t co2_ppm) {
    if (co2_ppm <= 500) return AIR_QUALITY_EXCELLENT;
    else if (co2_ppm <= 800) return AIR_QUALITY_GOOD;
    else if (co2_ppm <= 1000) return AIR_QUALITY_FAIR;
    else if (co2_ppm <= 1500) return AIR_QUALITY_MEDIOCRE;
    else if (co2_ppm <= 1900) return AIR_QUALITY_BAD;
    else return AIR_QUALITY_HEAVILY_CONTAMINATED;
}

void LED_RGB_Init() {

	htim2.Instance->CCR1 = 100;
	htim2.Instance->CCR2 = 100;
	htim2.Instance->CCR3 = 100;

	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);

}

void LED_RGB_Set_Intensity(uint8_t red, uint8_t green, uint8_t blue) {

	htim2.Instance->CCR1 = 100 - red;
	htim2.Instance->CCR2 = 100 - green;
	htim2.Instance->CCR3 = 100 - blue;

}

void LED_RGB_Update_Colour() {

	switch (current_air_quality) {
		case AIR_QUALITY_EXCELLENT:
			LED_RGB_Set_Intensity(0, (uint8_t)(0.10*led_rgb_intensity), 0); // Green
			break;
		case AIR_QUALITY_GOOD:
			LED_RGB_Set_Intensity((uint8_t)(0.5*led_rgb_intensity), (uint8_t)(0.10*led_rgb_intensity), 0); // Light Green
			break;
		case AIR_QUALITY_FAIR:
			LED_RGB_Set_Intensity(led_rgb_intensity, (uint8_t)(0.10*led_rgb_intensity), 0); // Yellow
			break;
		case AIR_QUALITY_MEDIOCRE:
			LED_RGB_Set_Intensity(led_rgb_intensity, (uint8_t)(0.04*led_rgb_intensity), 0); // Orange
			break;
		case AIR_QUALITY_BAD:
			LED_RGB_Set_Intensity(led_rgb_intensity, 0, 0); // Red
			break;
		case AIR_QUALITY_HEAVILY_CONTAMINATED:
			LED_RGB_Set_Intensity(led_rgb_intensity, 0, 0); // Red
			break;
		case AIR_QUALITY_VENTILATION_REQUIRED:
			LED_RGB_Set_Intensity(led_rgb_intensity, 0, 0); // Red
			break;
		default:
			LED_RGB_Set_Intensity(0, 0, 0);
			break;
	}

}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {

	// Colour Select Button
	if (GPIO_Pin == GPIO_PIN_4) {

		led_rgb_intensity += 25;

		if (led_rgb_intensity > 100)
			led_rgb_intensity = 0;

	}

	// Intensity Select Button
	else if (GPIO_Pin == GPIO_PIN_5) {

		if (led_rgb_intensity <= 0)
				led_rgb_intensity = 125;

		led_rgb_intensity -= 25;

	}

	LED_RGB_Update_Colour();

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_USB_PCD_Init();
  MX_RF_Init();
  /* USER CODE BEGIN 2 */
  LED_RGB_Init();


  HAL_StatusTypeDef ret;

  ret = DPS368_Init();
  if (ret != 0)
  	  Error_Handler();

  ret = SSD1309_Init();
  if (ret != 0)
	  Error_Handler();

  ret = SCD41_StopPeriodicMeasurement();
  if (ret != 0)
	  Error_Handler();

  ret = SCD41_StartPeriodicMeasurement();
  if (ret != 0)
	  Error_Handler();

  ret = SSD1309_DrawPixel(64, 32, 1);
  ret = SSD1309_DrawPixel(65, 32, 1);
  ret = SSD1309_DrawPixel(64, 33, 1);
  ret = SSD1309_DrawPixel(65, 33, 1);
  if (ret != 0)
   	  Error_Handler();

  ret = SSD1309_Update();
  if (ret != 0)
  	  Error_Handler();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_Delay(5000);

	  float temp_dps368, pressure;

	  ret = DPS368_GetTemperature(&temp_dps368);
	  if (ret != 0)
		Error_Handler();

	  HAL_Delay(100);

	  ret = DPS368_GetPressure(&pressure);
	  if (ret != 0)
	  	Error_Handler();

	  HAL_Delay(100);

	  uint32_t gotten_pressure;
	  ret = SCD41_GetAmbientPressure(&gotten_pressure);
	  if (ret != 0)
		Error_Handler();

	  HAL_Delay(100);

	  ret = SCD41_SetAmbientPressure((uint32_t)pressure);
	  if (ret != 0)
		Error_Handler();

	  HAL_Delay(100);

	  float temp_scd41, rh;
	  ret = SCD41_ReadMeasurement(&co2_ppm, &temp_scd41, &rh);
	  if (ret != 0)
	  	Error_Handler();

	  current_air_quality = Get_Air_Quality(co2_ppm);

	  LED_RGB_Update_Colour();

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_MEDIUMHIGH);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
  RCC_OscInitStruct.PLL.PLLN = 32;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the SYSCLKSource, HCLK, PCLK1 and PCLK2 clocks dividers
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK4|RCC_CLOCKTYPE_HCLK2
                              |RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.AHBCLK2Divider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.AHBCLK4Divider = RCC_SYSCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_SMPS|RCC_PERIPHCLK_RFWAKEUP;
  PeriphClkInitStruct.RFWakeUpClockSelection = RCC_RFWKPCLKSOURCE_HSE_DIV1024;
  PeriphClkInitStruct.SmpsClockSelection = RCC_SMPSCLKSOURCE_HSI;
  PeriphClkInitStruct.SmpsDivSelection = RCC_SMPSCLKDIV_RANGE1;

  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN Smps */

  /* USER CODE END Smps */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
