#ifndef _TASK_LED_H__
#define _TASK_LED_H__
#include "System.h"

#define System_Work_LED            HAL_GPIO_TogglePin(Normal_Status_GPIO_Port,Normal_Status_Pin)
#define System_Protect_LED         HAL_GPIO_WritePin(Protect_Status_GPIO_Port,Protect_Status_Pin,GPIO_PIN_RESET)
#define System_Normal_LED          HAL_GPIO_WritePin(Protect_Status_GPIO_Port,Protect_Status_Pin,GPIO_PIN_SET)
#define FricMotor_Work_LED			   HAL_GPIO_WritePin(FricMotor_Status_GPIO_Port,FricMotor_Status_Pin,GPIO_PIN_RESET)
#define FricMotor_Stop_LED			   HAL_GPIO_WritePin(FricMotor_Status_GPIO_Port,FricMotor_Status_Pin,GPIO_PIN_SET)
#define CanBus_Work_LED						 HAL_GPIO_WritePin(Can_Status_GPIO_Port,Can_Status_Pin,GPIO_PIN_RESET)
#define CanBus_Block_LED					 HAL_GPIO_WritePin(Can_Status_GPIO_Port,Can_Status_Pin,GPIO_PIN_SET)
#define Shoot_Stop_LED             HAL_GPIO_WritePin(SingleRound_Status_GPIO_Port,SingleRound_Status_Pin,GPIO_PIN_SET); \
																	 HAL_GPIO_WritePin(MultiRound_Status_GPIO_Port,MultiRound_Status_Pin,GPIO_PIN_SET)
#define SingleShoot_Ready_LED      HAL_GPIO_WritePin(SingleRound_Status_GPIO_Port,SingleRound_Status_Pin,GPIO_PIN_RESET)
#define SingleShoot_Work_LED       HAL_GPIO_TogglePin(SingleRound_Status_GPIO_Port,SingleRound_Status_Pin)
#define SingleShoot_Stop_LED      HAL_GPIO_WritePin(SingleRound_Status_GPIO_Port,SingleRound_Status_Pin,GPIO_PIN_SET)
#define MultiShoot_Ready_LED       HAL_GPIO_WritePin(MultiRound_Status_GPIO_Port,MultiRound_Status_Pin,GPIO_PIN_RESET)
#define MultiShoot_Work_LED  			 HAL_GPIO_TogglePin(MultiRound_Status_GPIO_Port,MultiRound_Status_Pin)
#define AutoAim_Stop_LED           HAL_GPIO_WritePin(Auto_Status_GPIO_Port,Auto_Status_Pin,GPIO_PIN_SET)
#define AutoAim_Work_LED           HAL_GPIO_WritePin(Auto_Status_GPIO_Port,Auto_Status_Pin,GPIO_PIN_RESET)
#define ShootForbidden_LED         HAL_GPIO_WritePin(ShootPermission_Status_GPIO_Port,ShootPermission_Status_Pin,GPIO_PIN_SET)
#define ShootPermission_LED        HAL_GPIO_WritePin(ShootPermission_Status_GPIO_Port,ShootPermission_Status_Pin,GPIO_PIN_RESET)
#define GyroOnline_LED						 HAL_GPIO_WritePin(GYRO_Status_GPIO_Port,GYRO_Status_Pin,GPIO_PIN_RESET)
#define GyroOffline_LED						 HAL_GPIO_WritePin(GYRO_Status_GPIO_Port,GYRO_Status_Pin,GPIO_PIN_SET)
#endif
