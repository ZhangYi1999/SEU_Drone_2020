#ifndef _TASK_DEBUG_H__
#define _TASK_DEBUG_H__

#include "Sys_Def.h"

extern uint8_t Head[2];
extern uint8_t Tail[2];

typedef union
{
	float SrcData;
	uint8_t Data[4];
}Encode_type;
#endif
