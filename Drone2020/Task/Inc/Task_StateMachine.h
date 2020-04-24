#ifndef _TASK_STATE_H__
#define _TASK_STATE_H__
#include "System.h"

#define KEY_FRICHIGH     				Get_Keyboard_Val(&RC_ReceiveData,KEY_F)//using
#define KEY_FRICLOW      				Get_Keyboard_Val(&RC_ReceiveData,KEY_G)
#define KEY_ACCELERATE   				Get_Keyboard_Val(&RC_ReceiveData,KEY_SHIFT)
#define KEY_LOWSPEED     				Get_Keyboard_Val(&RC_ReceiveData,KEY_CTRL)
#define KEY_MAGEZINE     				Get_Keyboard_Val(&RC_ReceiveData,KEY_R)
#define KEY_AVOID        				Get_Keyboard_Val(&RC_ReceiveData,KEY_V)
#define KEY_BIGBUFFERCCW  			    Get_Keyboard_Val(&RC_ReceiveData,KEY_Z)

#define KEY_SHOOT_OUTPOST      		    Get_Keyboard_Val(&RC_ReceiveData,KEY_X)//前哨站
#define KEY_SHOOT_BASE      		    Get_Keyboard_Val(&RC_ReceiveData,KEY_C)//基地

#define KEY_SHOOT        				Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_LEFT)//using
#define KEY_AUTO_SHOOT   				Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_RIGHT)//using

#define KEY_WILD_CHANGE   				Get_Keyboard_Val(&RC_ReceiveData,KEY_SHIFT)//using
#define KEY_WILD_CHANGE_LEFT   			Get_Keyboard_Val(&RC_ReceiveData,KEY_A)//using
#define KEY_WILD_CHANGE_RIGHT   		Get_Keyboard_Val(&RC_ReceiveData,KEY_D)//using
#define KEY_WILD_CHANGE_UP				Get_Keyboard_Val(&RC_ReceiveData,KEY_W)//using
#define KEY_WILD_CHANGE_DOWN			Get_Keyboard_Val(&RC_ReceiveData,KEY_S)//using

#define KEY_LEFT_ROTATE  				Get_Keyboard_Val(&RC_ReceiveData,KEY_Q)
#define KEY_RIGHT_ROTATE 				Get_Keyboard_Val(&RC_ReceiveData,KEY_E)
#define KEY_SHOOTNUM_CHANGE             Get_Keyboard_Val(&RC_ReceiveData,KEY_C)

void StatusMachine_Init(void);
void StatusMachine_Update(void);
#endif
