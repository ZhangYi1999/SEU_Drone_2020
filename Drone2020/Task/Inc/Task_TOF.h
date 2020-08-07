#ifndef _TASK_TOF_H__
#define _TASK_TOF_H__

#include "Sys_Def.h"

#define TOF_DMA_SIZE 19

extern uint8_t TOFBuffer[TOF_DMA_SIZE];
extern float height_data_temp[4];

void Task_TOF(void *parameters);
void TOF_Data_Update(void );

#endif
