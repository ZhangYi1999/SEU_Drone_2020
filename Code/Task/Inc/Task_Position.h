#ifndef _TASK_POSITION_H__
#define _TASK_POSITION_H__

#include "Sys_Def.h"

#define MEMORYRESET 2

#define PersonalGYRO_rx_len 79
#define PersonalGYROFrameLength 50 

#define GYROFRAMEHEAD0 0X5A
#define GYROFRAMEHEAD1 0XA5

typedef enum
{
	HI219=0,
} GYRO_Model;

typedef struct
{
	GYRO_Model model;
	Comm_Status comm_status;
	uint32_t comm_id;
	float PitchAngle;
	float YawAngle;
	float RollAngle;
	float Gyro_X;
	float Gyro_Y;
	float Gyro_Z;
} GYRO_type;

extern GYRO_type PersonalGYRO,PersonalGYRO2;
extern GYRO_type GYRO;
extern uint8_t GYROBuffer[2][PersonalGYRO_rx_len],GYROBuffer2[2][PersonalGYRO_rx_len];
extern uint8_t GYRO_Rx_Mem,GYRO_Rx_Mem2;

void PositionInit(void);
void Personal_GYRO_Receive(GYRO_type *gyro);
void Gimbal_PositionSet(void);

float LowFilter(float last_value,float current_value);
float AverageFilter(float current_value);
#endif
