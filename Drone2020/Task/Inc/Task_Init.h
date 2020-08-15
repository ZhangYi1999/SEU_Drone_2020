#ifndef _TASK_INIT_H__
#define _TASK_INIT_H__

#include "Sys_Def.h"

void CanInit(void *parameters);
void DMAInit(void *parameters);
void TIMInit(void *parameters);
void RefereeConnection_Init(void);
void FlashInit(void);

union Wild_Change{float FLOAT;uint32_t INT;};

extern union Wild_Change Wild_Change_Angle_Pitch , Wild_Change_Angle_Yaw;

#endif
