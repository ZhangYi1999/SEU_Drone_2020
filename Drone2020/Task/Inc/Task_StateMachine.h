#ifndef _TASK_STATEMACHINE_H__
#define _TASK_STATEMACHINE_H__

#include "Sys_Def.h"

#define KEY_FRICHIGH     				Get_Keyboard_Val(&RC_ReceiveData,KEY_F)//using
#define KEY_SHOOTSTATUS      		Get_Keyboard_Val(&RC_ReceiveData,KEY_G)//using
//#define KEY_V		      					Get_Keyboard_Val(&RC_ReceiveData,KEY_V)
//#define KEY_CTRL     				Get_Keyboard_Val(&RC_ReceiveData,KEY_CTRL)
//#define KEY_     				Get_Keyboard_Val(&RC_ReceiveData,KEY_R)
#define KEY_BIGBUFFERCCW  			    Get_Keyboard_Val(&RC_ReceiveData,KEY_Z)

#define KEY_SHOOT_OUTPOST      		    Get_Keyboard_Val(&RC_ReceiveData,KEY_X)//前哨站
#define KEY_SHOOT_BASE      		    Get_Keyboard_Val(&RC_ReceiveData,KEY_C)//基地

#define KEY_SHOOT        				Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_LEFT)//using
#define KEY_AUTO_SHOOT   				Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_RIGHT)//using

#define KEY_WILD_CHANGE   				Get_Keyboard_Val(&RC_ReceiveData,KEY_SHIFT)
#define KEY_WILD_CHANGE_LEFT   			Get_Keyboard_Val(&RC_ReceiveData,KEY_A)
#define KEY_WILD_CHANGE_RIGHT   		Get_Keyboard_Val(&RC_ReceiveData,KEY_D)
#define KEY_WILD_CHANGE_UP				Get_Keyboard_Val(&RC_ReceiveData,KEY_W)
#define KEY_WILD_CHANGE_DOWN			Get_Keyboard_Val(&RC_ReceiveData,KEY_S)

#define KEY_LEFT_ROTATE  				Get_Keyboard_Val(&RC_ReceiveData,KEY_Q)
#define KEY_RIGHT_ROTATE 				Get_Keyboard_Val(&RC_ReceiveData,KEY_E)
#define KEY_SHOOTNUM_CHANGE             Get_Keyboard_Val(&RC_ReceiveData,KEY_C)

void StatusMachine_Init(void);
void StatusMachine_Update(void);
#endif
