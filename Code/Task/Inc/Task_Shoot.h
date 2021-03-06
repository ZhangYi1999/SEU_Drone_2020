#ifndef _TASK_SHOOT_H__
#define _TASK_SHOOT_H__

#include "Sys_Def.h"

#define FircMotor_Left_Power_On         HAL_GPIO_WritePin(FricMotor_Left_Power_GPIO_Port,FricMotor_Left_Power_Pin,GPIO_PIN_SET)
#define FircMotor_Left_Power_Off        HAL_GPIO_WritePin(FricMotor_Left_Power_GPIO_Port,FricMotor_Left_Power_Pin,GPIO_PIN_RESET)
#define FricMotor_Right_Power_On        HAL_GPIO_WritePin(FricMotor_Right_Power_GPIO_Port,FricMotor_Right_Power_Pin,GPIO_PIN_SET)
#define FricMotor_Right_Power_Off       HAL_GPIO_WritePin(FricMotor_Right_Power_GPIO_Port,FricMotor_Right_Power_Pin,GPIO_PIN_RESET)


struct Motor_type;

extern Motor_type FricMotor_Left;
extern Motor_type FricMotor_Right;
extern Motor_type StirMotor;

void Shoot_Init(void);
void Shoot_GetInfo(void);
void Shoot_Control(void);
void Motor_Blocked_Detect(Motor_type* motor);

#endif
