#ifndef _TASK_INIT_H__
#define _TASK_INIT_H__

#include "Sys_Def.h"

void CanInit(void *parameters);
void DMAInit(void *parameters);
void TIMInit(void *parameters);
void RefereeConnection_Init(void);
void FlashInit(void);

#endif
