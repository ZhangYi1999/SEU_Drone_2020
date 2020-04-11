#ifndef _TASK_CANCOMM_H__
#define _TASK_CANCOMM_H__
#include "System.h"

extern CanSend_type Can_TX_Message;

int16_t delta_angle(int16_t speed, uint16_t angle_last, uint16_t angle_now);
#endif
