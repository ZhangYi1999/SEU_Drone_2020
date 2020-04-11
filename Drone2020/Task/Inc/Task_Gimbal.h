#ifndef _TASK_GIMBAL_H__
#define _TASK_GIMBAL_H__
#include "System.h"

#define PitchAngle_StepLength   0.8
#define YawAngle_StepLength     0.5

void Gimbal_Init(void);
void Gimbal_SetPosition(void);
void Gimbal_Control(void);
void Gimbal_Protect(void);
int Gimbal_StartMove(void);
#endif
