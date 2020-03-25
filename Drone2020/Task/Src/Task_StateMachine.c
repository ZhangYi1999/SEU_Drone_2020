#include "Task_StateMachine.h"
#include "Task_RC.h"

void Task_StateMachine(void *parameters)
{
	StatusMachine_Init();
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		StatusMachine_Update();
	}
}

/**
  * @brief  状态机初始化
  * @param  void
  * @retval void
  */
void StatusMachine_Init(void)
{
	Gimbal.control_mode = ControlMode_Protect;
	Gimbal.fric_status = FricStatus_Stop;
	Gimbal.stir_status = StirStatus_Stop;
	Gimbal.shoot_status = ShootStatus_Stop;
}

void StatusMachine_Update(void)
{
    static uint8_t Pulse = 0;

    switch (Get_Switch_Val(&RC_ReceiveData, RC_SW_Right))
    {
			case RC_SW_MID:
			{
				Gimbal.control_mode = ControlMode_Manual;
				break;
			}
			case RC_SW_DOWN:
			{
				Gimbal.control_mode = ControlMode_Auto;
				break;
			}
			default:
			{
				Gimbal.control_mode = ControlMode_Protect;
				break;
			}
    }

    switch (Gimbal.control_mode)
    {
			case ControlMode_Manual:
			{
				if (Get_Switch_Val(&RC_ReceiveData, RC_SW_Left) == RC_SW_UP)
				{
					if(Pulse==0)
					{
						Pulse=1;
						
						if(Gimbal.shoot_status == ShootStatus_Stop)
							Gimbal.shoot_status = ShootStatus_SingleRound;
						else if(Gimbal.shoot_status == ShootStatus_SingleRound)
							Gimbal.shoot_status = ShootStatus_MultiRound;
						else
							Gimbal.shoot_status = ShootStatus_Stop;
						
						if(Gimbal.shoot_status != ShootStatus_Stop)
							Gimbal.fric_status = FricStatus_Working;
						else
							Gimbal.fric_status = FricStatus_Stop;				
					}
				}
				else if(Get_Switch_Val(&RC_ReceiveData, RC_SW_Left) == RC_SW_DOWN)
				{
					if(Pulse==0)
					{
						Pulse=1;
						if(Gimbal.shoot_status != ShootStatus_Stop)
							Gimbal.stir_status = StirStatus_Working;
						else
							Gimbal.stir_status = StirStatus_Stop;
					}					
				}
				else
				{
					Pulse=0;					
					Gimbal.stir_status = StirStatus_Stop;
				}
				break;
			} 
			case ControlMode_Auto:
			{
				if(Get_Mouse_Pressed(&RC_ReceiveData,MOUSE_LEFT))
				{
					if(Gimbal.shoot_status != ShootStatus_Stop)
						Gimbal.stir_status = StirStatus_Working;
				}
				else
				{
					Gimbal.stir_status = StirStatus_Stop;
				}
				break;				 
			}
			case ControlMode_Protect:
			{
				Gimbal.fric_status = FricStatus_Stop;
				Gimbal.shoot_status = ShootStatus_Stop;
				break;
			}
    }
}
