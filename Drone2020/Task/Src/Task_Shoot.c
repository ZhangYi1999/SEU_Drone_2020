#include "Task_Shoot.h"


void Task_Shoot(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	Shoot_Init();
	FircMotor_Left_Power_On;
	FricMotor_Right_Power_On;
	HAL_GPIO_WritePin(GPIOH,GPIO_PIN_2,GPIO_PIN_SET);
	while(1)
	{
		Shoot_GetInfo();
		Shoot_Control();
		vTaskDelayUntil(&xLastWakeUpTime,2);
	}
}


void Shoot_Init(void)
{
	FricMotor_Left.model = GM3508;
	FricMotor_Left.communication = Can;
	FricMotor_Left.control = single_loop;
	FricMotor_Left.comm_status.link_status = offline;
	FricMotor_Left.comm_status.FrameCounter = 0; 
	FricMotor_Left.comm_id = 0x202;
	
	FricMotor_Left.RealSpeed = 0;
	FricMotor_Left.TargetSpeed = 0;
	FricMotor_Left.RealAngle = 0;
	FricMotor_Left.TargetAngle = 0;
	FricMotor_Left.MechanicalAngle = 0;
	FricMotor_Left.RotationalSpeed = 0;
	FricMotor_Left.RealTorque = 0;

	FricMotor_Left.NeedOutput = 0;
	FricMotor_Left.MaximumOutput = FricMotor_MaximumOutput;
	FricMotor_Left.SpeedPID.Kp = FricMotor_SpeedPID_Kp;
	FricMotor_Left.SpeedPID.Ki = FricMotor_SpeedPID_Ki;
	FricMotor_Left.SpeedPID.Kd = FricMotor_SpeedPID_Kd;
	FricMotor_Left.SpeedPID.Dead_Band = FricMotor_SpeedPID_DeadBand;
	FricMotor_Left.SpeedPID.Maximum_Integration = FricMotor_SpeedPID_Maximum_Integration;
	FricMotor_Left.SpeedPID.Current_Error = 0;
	FricMotor_Left.SpeedPID.Last_Error = 0;
	FricMotor_Left.SpeedPID.Sum_Integration = 0;
	FricMotor_Left.SpeedPID.Output = 0;
	FricMotor_Left.AnglePID.Kp = FricMotor_AnglePID_Kp;
	FricMotor_Left.AnglePID.Ki = FricMotor_AnglePID_Ki;
	FricMotor_Left.AnglePID.Kd = FricMotor_AnglePID_Kd;
	FricMotor_Left.AnglePID.Dead_Band = FricMotor_AnglePID_DeadBand;
	FricMotor_Left.AnglePID.Maximum_Integration = FricMotor_AnglePID_Maximum_Integration;
	FricMotor_Left.AnglePID.Current_Error = 0;
	FricMotor_Left.AnglePID.Last_Error = 0;
	FricMotor_Left.AnglePID.Sum_Integration = 0;
	FricMotor_Left.AnglePID.Output = 0;
	
	FricMotor_Right.model = GM3508;
	FricMotor_Right.communication = Can;
	FricMotor_Right.control = single_loop;
	FricMotor_Right.comm_status.link_status = offline;
	FricMotor_Right.comm_status.FrameCounter = 0; 
	FricMotor_Right.comm_id = 0x201;
	
	FricMotor_Right.RealSpeed = 0;
	FricMotor_Right.TargetSpeed = 0;
	FricMotor_Right.RealAngle = 0;
	FricMotor_Right.TargetAngle = 0;
	FricMotor_Right.MechanicalAngle = 0;
	FricMotor_Right.RotationalSpeed = 0;
	FricMotor_Right.RealTorque = 0;

	FricMotor_Right.NeedOutput = 0;
	FricMotor_Right.MaximumOutput = FricMotor_MaximumOutput;
	FricMotor_Right.SpeedPID.Kp = FricMotor_SpeedPID_Kp;
	FricMotor_Right.SpeedPID.Ki = FricMotor_SpeedPID_Ki;
	FricMotor_Right.SpeedPID.Kd = FricMotor_SpeedPID_Kd;
	FricMotor_Right.SpeedPID.Dead_Band = FricMotor_SpeedPID_DeadBand;
	FricMotor_Right.SpeedPID.Maximum_Integration = FricMotor_SpeedPID_Maximum_Integration;
	FricMotor_Right.SpeedPID.Current_Error = 0;
	FricMotor_Right.SpeedPID.Last_Error = 0;
	FricMotor_Right.SpeedPID.Sum_Integration = 0;
	FricMotor_Right.SpeedPID.Output = 0;
	FricMotor_Right.AnglePID.Kp = FricMotor_AnglePID_Kp;
	FricMotor_Right.AnglePID.Ki = FricMotor_AnglePID_Ki;
	FricMotor_Right.AnglePID.Kd = FricMotor_AnglePID_Kd;
	FricMotor_Right.AnglePID.Dead_Band = FricMotor_AnglePID_DeadBand;
	FricMotor_Right.AnglePID.Maximum_Integration = FricMotor_AnglePID_Maximum_Integration;
	FricMotor_Right.AnglePID.Current_Error = 0;
	FricMotor_Right.AnglePID.Last_Error = 0;
	FricMotor_Right.AnglePID.Sum_Integration = 0;
	FricMotor_Right.AnglePID.Output = 0;	
	
	StirMotor.model = GM2006;
	StirMotor.communication = Can;
	StirMotor.control = single_loop;
	StirMotor.comm_status.link_status = offline;
	StirMotor.comm_status.FrameCounter = 0; 
	StirMotor.comm_id = 0x204;
	
	StirMotor.RealSpeed = 0;
	StirMotor.TargetSpeed = 0;
	StirMotor.RealAngle = 0;
	StirMotor.TargetAngle = 0;
	StirMotor.MechanicalAngle = 0;
	StirMotor.RotationalSpeed = 0;
	StirMotor.RealTorque = 0;

	StirMotor.NeedOutput = 0;
	StirMotor.MaximumOutput = StirMotor_MaximumOutput;
	StirMotor.SpeedPID.Kp = StirMotor_SpeedPID_Kp;
	StirMotor.SpeedPID.Ki = StirMotor_SpeedPID_Ki;
	StirMotor.SpeedPID.Kd = StirMotor_SpeedPID_Kd;
	StirMotor.SpeedPID.Dead_Band = StirMotor_SpeedPID_DeadBand;
	StirMotor.SpeedPID.Maximum_Integration = StirMotor_SpeedPID_Maximum_Integration;
	StirMotor.SpeedPID.Current_Error = 0;
	StirMotor.SpeedPID.Last_Error = 0;
	StirMotor.SpeedPID.Sum_Integration = 0;
	StirMotor.SpeedPID.Output = 0;
	StirMotor.AnglePID.Kp = StirMotor_AnglePID_Kp;
	StirMotor.AnglePID.Ki = StirMotor_AnglePID_Ki;
	StirMotor.AnglePID.Kd = StirMotor_AnglePID_Kd;
	StirMotor.AnglePID.Dead_Band = StirMotor_AnglePID_DeadBand;
	StirMotor.AnglePID.Maximum_Integration = StirMotor_AnglePID_Maximum_Integration;
	StirMotor.AnglePID.Current_Error = 0;
	StirMotor.AnglePID.Last_Error = 0;
	StirMotor.AnglePID.Sum_Integration = 0;
	StirMotor.AnglePID.Output = 0;	
}	

void Shoot_GetInfo(void)
{
	FricMotor_Left.RealSpeed = FricMotor_Left.RotationalSpeed;
	FricMotor_Right.RealSpeed = FricMotor_Right.RotationalSpeed;
	StirMotor.RealSpeed = StirMotor.RotationalSpeed;
	if(StirMotor.RealAngle > 360 && Gimbal.stir_status == StirStatus_Stop)
	{
		LimitIn360(StirMotor.RealAngle);
		LimitIn360(StirMotor.TargetAngle);
	}
}

int16_t value = 0;
void Shoot_Control(void)
{
	static uint8_t StirMotor_SetAngle_Flag=0;
	if(Gimbal.control_mode != ControlMode_Protect)
	{
		if(Gimbal.fric_status == FricStatus_Working)
		{
			FricMotor_Right.TargetSpeed = FricMotor_WorkingSpeed;
			FricMotor_Left.TargetSpeed = FricMotor_WorkingSpeed*-1;
		}
		else
		{
			FricMotor_Right.TargetSpeed = 0;
			FricMotor_Left.TargetSpeed = 0;
		}
		PID_Calculation(&FricMotor_Right);
		PID_Calculation(&FricMotor_Left);
		
		Motor_Blocked_Detect(&StirMotor);
		
		if(StirMotor.comm_status.link_status == online)
		{
			if(Gimbal.shoot_status == ShootStatus_MultiRound)
			{
				if(Gimbal.stir_status == StirStatus_Working)
				{
					if (BlockedWarningTimes <= 0)
					{
						StirMotor.TargetSpeed = StirMotor_WorkingSpeed;
					}
					else
					{
						StirMotor.TargetSpeed = -StirMotor_WorkingSpeed;
						BlockedWarningTimes--;
					}
					StirMotor.control = single_loop;
				}
				else
				{
					StirMotor.TargetSpeed = 0;
					StirMotor.control = single_loop;
				}
			}
			else if(Gimbal.shoot_status == ShootStatus_SingleRound)
			{
				if(Gimbal.stir_status == StirStatus_Working && StirMotor_SetAngle_Flag == 0)
				{
					StirMotor.TargetAngle = StirMotor.RealAngle+StirMotor_WorkingAngle;
					StirMotor.control = double_loop;
					StirMotor_SetAngle_Flag=1;
				}
				if(Gimbal.stir_status == StirStatus_Stop)
				{
					StirMotor_SetAngle_Flag=0;
				}
				StirMotor.TargetSpeed = 0;
				StirMotor.control = single_loop;
			}
			else
			{
				StirMotor.TargetSpeed = 0;
				StirMotor.control = single_loop;
			}
		}
		else
		{
			StirMotor.TargetSpeed = 0;
			StirMotor.TargetAngle = 0;
			StirMotor.control = single_loop;
		}
		PID_Calculation(&StirMotor);
		Motor_CanSend_Shoot(&FricMotor_Left,&FricMotor_Right,&StirMotor);
	}
	else
	{
		FricMotor_Right.TargetSpeed = 0;
		FricMotor_Left.TargetSpeed = 0;
		StirMotor.TargetSpeed = 0;
		StirMotor.TargetAngle = 0;
		StirMotor.control = single_loop;
		FricMotor_Left.NeedOutput = 0;
		FricMotor_Right.NeedOutput = 0;
		StirMotor.NeedOutput = 0;
		Motor_CanSend_Shoot(&FricMotor_Left,&FricMotor_Right,&StirMotor);
	}
}


void Motor_Blocked_Detect(Motor_type* motor)
{
  static uint8_t BlockedTimes = 0;

  if(BlockedWarningTimes <= 0)
  {
    if(__fabs(motor->RealSpeed) < 100 && __fabs(motor->SpeedPID.Sum_Integration) == motor->SpeedPID.Maximum_Integration && __fabs(motor->TargetSpeed) > 1000) 
      BlockedTimes++;
    else
      BlockedTimes = 0;

    if(BlockedTimes >= 20)
    {
      BlockedWarningTimes = 40;
      BlockedTimes = 0;
    }
  }
}

