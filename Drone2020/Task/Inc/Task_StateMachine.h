#ifndef _TASK_STATE_H__
#define _TASK_STATE_H__
#include "System.h"

#define KEY_FRICHIGH     				Get_Keyboard_Val(&RC_ReceiveData,KEY_F)
#define KEY_FRICLOW      				Get_Keyboard_Val(&RC_ReceiveData,KEY_G)
#define KEY_ACCELERATE   				Get_Keyboard_Val(&RC_ReceiveData,KEY_SHIFT)
#define KEY_LOWSPEED     				Get_Keyboard_Val(&RC_ReceiveData,KEY_CTRL)
#define KEY_MAGEZINE     				Get_Keyboard_Val(&RC_ReceiveData,KEY_R)
#define KEY_AVOID        				Get_Keyboard_Val(&RC_ReceiveData,KEY_V)
#define KEY_BIGBUFFERCCW  			    Get_Keyboard_Val(&RC_ReceiveData,KEY_Z)
#define KEY_SMALLBUFFER   			    Get_Keyboard_Val(&RC_ReceiveData,KEY_X)
#define KEY_SHOOT        				Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_LEFT)
#define KEY_AUTO_SHOOT   				Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_RIGHT)
#define KEY_LEFT_ROTATE  				Get_Keyboard_Val(&RC_ReceiveData,KEY_Q)
#define KEY_RIGHT_ROTATE 				Get_Keyboard_Val(&RC_ReceiveData,KEY_E)
#define KEY_SHOOTNUM_CHANGE             Get_Keyboard_Val(&RC_ReceiveData,KEY_C)

void StatusMachine_Init(void);
void StatusMachine_Update(void);
#endif
