#ifndef _TASK_GIMBAL_H__
#define _TASK_GIMBAL_H__

#include "Sys_Def.h"

#define PitchAngle_StepLength   0.8
#define YawAngle_StepLength     0.5

typedef struct
{
	float PitchAngle;
	float YawAngle;
	float RollAngle;
	float PitchSpeed;
	float YawSpeed;
	float RollSpeed;
	float x;
	float y;
	float z;
	float Height;
}Gimbal_Position;


typedef enum
{
	FricStatus_Stop=0,
	FricStatus_Working
}FricStatus_Enum;

typedef enum
{
	StirStatus_Stop=0,
	StirStatus_Working
}StirStatus_Enum;

typedef enum
{
	ShootStatus_Stop=0,
	ShootStatus_SingleRound,
	ShootStatus_MultiRound,
}ShootStatus_Enum;

typedef enum
{
	ControlMode_Protect=0,            
	ControlMode_Manual,
	ControlMode_PC,
}ControlMode_Enum;


typedef enum
{
	AimMode_Protect=0,            
	AimMode_Manual,
	AimMode_Auto,
    AimMode_Outpost,
    AimMode_Base,
}AimMode_Enum;

typedef struct
{
    Gimbal_Position position;
    FricStatus_Enum fric_status;
    StirStatus_Enum stir_status;
    ShootStatus_Enum shoot_status;
    ControlMode_Enum control_mode;
    AimMode_Enum aim_mode;
}Gimbal_type;

struct Motor_type;

extern Motor_type PitchMotor;
extern Motor_type YawMotor;
extern Gimbal_type Gimbal;

void Gimbal_Init(void);
void Gimbal_SetPosition(void);
void Gimbal_Control(void);
void Gimbal_Protect(void);
int Gimbal_StartMove(void);

#endif
