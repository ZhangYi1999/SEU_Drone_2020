#ifndef _SYS_DEF_H_
#define _SYS_DEF_H_

#include "stm32f4xx_hal.h"
#include "stm32f4xx_it.h"
#include "main.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "freertos.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "sdio.h"

#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <string.h>
#include <arm_math.h>

#include "Func_Imu_OB.h"
#include "ff_gen_drv.h"
#include "sd_diskio.h"

/*-------------------------------- 枚举变量定义开始-------------------------------- */
typedef enum
{
	offline=0,
	online
}Link_Status;

typedef struct
{
	Link_Status link_status;
	uint16_t FrameCounter;
}Comm_Status;

typedef enum
{
	GM6020=0,
	GM3508,
	GM2006,
	Snail
}Motor_Model;

typedef enum
{
	Can=0,
	PWM
}Motor_CommMethod;

typedef enum
{
	single_loop=0,
	double_loop,
	open_loop
}Motor_ControlMethod;

typedef enum{
	ButtonPressedOnce = 0,
	ButtonTaskFinish
}UserButtonStatus_Enum;

/*-------------------------------- 枚举变量定义结束-------------------------------- */

/*-------------------------------- 基本变量结构体定义开始-------------------------------- */
typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float Current_Error;
	float Last_Error;
	float Sum_Integration;
	float Maximum_Integration;
	float Dead_Band;
	float Output;
}PID_type;


typedef struct
{
	Motor_Model model;
	Motor_CommMethod communication;
	Motor_ControlMethod control;
	Comm_Status comm_status;
	int comm_id;
	
	float RealSpeed;
	float TargetSpeed;
	float RealAngle;
	float TargetAngle;
	uint16_t Last_MechanicalAngle;
	uint16_t MechanicalAngle;
	int16_t	RotationalSpeed;
	int16_t RealTorque;
	int16_t RealCurrent;
	
	int16_t NeedOutput;
	int16_t MaximumOutput;
	PID_type SpeedPID;
	PID_type AnglePID;
}Motor_type;

/*-------------------------------- 基本变量结构体定义结束-------------------------------- */


#endif
