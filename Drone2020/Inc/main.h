/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#define GYRO_TX_Pin GPIO_PIN_1
#define GYRO_TX_GPIO_Port GPIOE
#define GYRO_RX_Pin GPIO_PIN_0
#define GYRO_RX_GPIO_Port GPIOE
#define IMU_HEATING_Pin GPIO_PIN_5
#define IMU_HEATING_GPIO_Port GPIOB
#define JetsonTX2_TX_Pin GPIO_PIN_14
#define JetsonTX2_TX_GPIO_Port GPIOG
#define SD_CLK_Pin GPIO_PIN_12
#define SD_CLK_GPIO_Port GPIOC
#define SWDCLK_Pin GPIO_PIN_14
#define SWDCLK_GPIO_Port GPIOA
#define SWDIO_Pin GPIO_PIN_13
#define SWDIO_GPIO_Port GPIOA
#define DR16_RX_Pin GPIO_PIN_7
#define DR16_RX_GPIO_Port GPIOB
#define CAN1_RX_Pin GPIO_PIN_0
#define CAN1_RX_GPIO_Port GPIOD
#define SD_DATA3_Pin GPIO_PIN_11
#define SD_DATA3_GPIO_Port GPIOC
#define SD_DATA2_Pin GPIO_PIN_10
#define SD_DATA2_GPIO_Port GPIOC
#define JetsonTX2_RX_Pin GPIO_PIN_9
#define JetsonTX2_RX_GPIO_Port GPIOG
#define CAN1_TX_Pin GPIO_PIN_1
#define CAN1_TX_GPIO_Port GPIOD
#define SD_CMD_Pin GPIO_PIN_2
#define SD_CMD_GPIO_Port GPIOD
#define CampInput_Pin GPIO_PIN_0
#define CampInput_GPIO_Port GPIOF
#define SD_DATA1_Pin GPIO_PIN_9
#define SD_DATA1_GPIO_Port GPIOC
#define SD_DATA0_Pin GPIO_PIN_8
#define SD_DATA0_GPIO_Port GPIOC
#define FricMotor_Left_Power_Pin GPIO_PIN_4
#define FricMotor_Left_Power_GPIO_Port GPIOH
#define FricMotor_Right_Power_Pin GPIO_PIN_5
#define FricMotor_Right_Power_GPIO_Port GPIOH
#define GYRO_Status_Pin GPIO_PIN_7
#define GYRO_Status_GPIO_Port GPIOG
#define ShootPermission_Status_Pin GPIO_PIN_6
#define ShootPermission_Status_GPIO_Port GPIOG
#define SPI5_NSS_Pin GPIO_PIN_6
#define SPI5_NSS_GPIO_Port GPIOF
#define Auto_Status_Pin GPIO_PIN_5
#define Auto_Status_GPIO_Port GPIOG
#define MultiRound_Status_Pin GPIO_PIN_4
#define MultiRound_Status_GPIO_Port GPIOG
#define SingleRound_Status_Pin GPIO_PIN_3
#define SingleRound_Status_GPIO_Port GPIOG
#define Can_Status_Pin GPIO_PIN_2
#define Can_Status_GPIO_Port GPIOG
#define FricMotor_Status_Pin GPIO_PIN_1
#define FricMotor_Status_GPIO_Port GPIOG
#define TOF_Rx_Pin GPIO_PIN_1
#define TOF_Rx_GPIO_Port GPIOA
#define Debug_TX_Pin GPIO_PIN_8
#define Debug_TX_GPIO_Port GPIOE
#define Protect_Status_Pin GPIO_PIN_11
#define Protect_Status_GPIO_Port GPIOE
#define Judge_RX_Pin GPIO_PIN_9
#define Judge_RX_GPIO_Port GPIOD
#define Judge_TX_Pin GPIO_PIN_8
#define Judge_TX_GPIO_Port GPIOD
#define Normal_Status_Pin GPIO_PIN_14
#define Normal_Status_GPIO_Port GPIOF
#define Debug_RX_Pin GPIO_PIN_7
#define Debug_RX_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
