/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define PI_WKUP_Pin GPIO_PIN_13
#define PI_WKUP_GPIO_Port GPIOC
#define CURR_MON_Pin GPIO_PIN_1
#define CURR_MON_GPIO_Port GPIOB
#define PWR_CTL_Pin GPIO_PIN_13
#define PWR_CTL_GPIO_Port GPIOE
#define CHRG_LVL_1_Pin GPIO_PIN_15
#define CHRG_LVL_1_GPIO_Port GPIOB
#define CHRG_LVL_2_Pin GPIO_PIN_9
#define CHRG_LVL_2_GPIO_Port GPIOD
#define PWR_MODE_Pin GPIO_PIN_10
#define PWR_MODE_GPIO_Port GPIOD
#define CHRG_LVL_4_Pin GPIO_PIN_12
#define CHRG_LVL_4_GPIO_Port GPIOD
#define CHRG_OK_Pin GPIO_PIN_13
#define CHRG_OK_GPIO_Port GPIOD
#define AC_OK_Pin GPIO_PIN_14
#define AC_OK_GPIO_Port GPIOD
#define CHRG_LVL_3_Pin GPIO_PIN_9
#define CHRG_LVL_3_GPIO_Port GPIOA
#define IN_DG_Pin GPIO_PIN_10
#define IN_DG_GPIO_Port GPIOA
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
