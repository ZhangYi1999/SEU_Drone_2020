#include "Task_LED.h"


void Task_LED(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	
	static uint8_t ShootBlink = 0;
	
	while(1)
	{
		System_Work_LED;
		
		if(GYRO.comm_status.link_status == online)
			GyroOnline_LED;
		else
			GyroOffline_LED;
		
		if(Gimbal.control_mode == ControlMode_Protect)
		{
			System_Protect_LED;
		}
		else if(Gimbal.control_mode == ControlMode_PC)
		{
			AutoAim_Work_LED;
		}
		else
		{
			System_Normal_LED;
			AutoAim_Stop_LED;
		}
		
		if(Gimbal.fric_status != FricStatus_Stop)
		{
			FricMotor_Work_LED;
		}
		else
		{
			FricMotor_Stop_LED;
		}
		
		if(Gimbal.shoot_status == ShootStatus_SingleRound)
		{
			if(Gimbal.stir_status == StirStatus_Working)
			{
				if(ShootBlink>0)
				{
					ShootBlink--;
					SingleShoot_Work_LED;
				}
			}
			else
			{
				ShootBlink=2;
				SingleShoot_Ready_LED;
			}			
		}
		else if(Gimbal.shoot_status == ShootStatus_MultiRound)
		{
			SingleShoot_Stop_LED;
			if(Gimbal.stir_status == StirStatus_Working)
			{
				MultiShoot_Work_LED;
			}
			else
			{
				MultiShoot_Ready_LED;
			}
		}
		else
		{
			Shoot_Stop_LED;
		}
			
		vTaskDelayUntil(&xLastWakeUpTime,100);
	}
}


