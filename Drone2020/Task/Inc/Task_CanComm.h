#ifndef _TASK_CANCOMM_H__
#define _TASK_CANCOMM_H__

#include "Sys_Def.h"

typedef enum
{
	Can1=0,
	Can2
}Can_Channel;

typedef struct
{
	Can_Channel CANx;
	CAN_TxHeaderTypeDef SendCanHeader;
	uint8_t SendCanTxMsg[8];
}CanSend_type;



extern CanSend_type Can_TX_Message;

int16_t delta_angle(int16_t speed, uint16_t angle_last, uint16_t angle_now);
#endif
