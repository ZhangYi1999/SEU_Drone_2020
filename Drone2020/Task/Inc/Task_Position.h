#ifndef _TASK_POSITION_H__
#define _TASK_POSITION_H__
#include "System.h"

void PositionInit(void);
void Personal_GYRO_Receive(GYRO_type *gyro);
void Gimbal_PositionSet(void);

float LowFilter(float last_value,float current_value);
float AverageFilter(float current_value);
#endif
