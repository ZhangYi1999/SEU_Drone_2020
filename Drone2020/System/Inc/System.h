#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "Sys_Def.h"
#include "Sys_Config.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <string.h>

#include "main.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "freertos.h"
#include "cmsis_os.h"
#include "stm32f4xx_it.h"

extern QueueHandle_t Queue_CANSend;
extern TaskHandle_t TaskProtect_Handle;
extern TaskHandle_t TaskCanComm_Handle;
extern TaskHandle_t TaskPosition_Handle;
extern TaskHandle_t TaskJudge_Handle;
extern TaskHandle_t TaskJetsonComm_Handle;
extern TaskHandle_t TaskRC_Handle;
extern TaskHandle_t TaskStateMachine_Handle;
extern TaskHandle_t TaskGimbal_Handle;
extern TaskHandle_t TaskShoot_Handle;
extern TaskHandle_t TaskDebug_Handle;
extern TaskHandle_t TaskLED_Handle;
extern TaskHandle_t TaskUi_Handle;
extern TaskHandle_t TaskSDIO_Handle;

void Task_Protect(void *parameters);
void Task_CanComm(void *parameters);
void Task_Position(void *parameters);
void Task_Judge(void *parameters);
void Task_JetsonComm(void *parameters);
void Task_RC(void *parameters);
void Task_StateMachine(void *parameters);
void Task_Gimbal(void *parameters);
void Task_Shoot(void *parameters);
void Task_Debug(void *parameters);
void Task_LED(void *parameters);
void Task_Ui(void *parameters);
void Task_SDIO(void *parameters);

void Motor_SetOutput(Motor_type* motor,int16_t output);
void Motor_CanSend(Motor_type* motor);
void Motor_CanSend_Gimbal(Motor_type* Pitch,Motor_type* Yaw);
void Motor_CanSend_Shoot(Motor_type* FricMotor_Left,Motor_type* FricMotor_Right,Motor_type* StirMotor);



void PID_Calculation(Motor_type* motor);

#endif
