#ifndef _TASK_SDIO_H__
#define _TASK_SDIO_H__

#include "Sys_Def.h"

void Task_SDIO(void *parameters);
void Update_SDtime(void);
void AppendBlank(char *str, int pre_len, int blank);
void DtatPrepareSD(void);
//static void Error_Handler(void);

extern int game_num;

#endif
