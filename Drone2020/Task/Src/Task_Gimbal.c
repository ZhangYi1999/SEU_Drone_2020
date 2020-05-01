#include "Task_Gimbal.h"
#include "Task_RC.h"

/*
云台控制逻辑
首先init
然后在setposition中将陀螺仪的角度赋给电机真实角度,即电机通过陀螺仪得知真实角度,以避免由于安装造成需要修改代码
再然后在control中通过对电机的target angle进行赋值,然后PID计算使得电机转至目标角度
*/

void Task_Gimbal(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	Gimbal_Init();

	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,2);
		if(GYRO.comm_status.link_status == online)
		{
			Gimbal_SetPosition();
			Gimbal_Control();
		}
		else
		{
			
		}
	}
}

void Gimbal_Init(void)
{
	PitchMotor.model = GM6020;
	PitchMotor.communication = Can;
	PitchMotor.control = double_loop;
	PitchMotor.comm_status.link_status = offline;
	PitchMotor.comm_status.FrameCounter = 0; 
	PitchMotor.comm_id = 0x207;
	
	PitchMotor.RealSpeed = 0;
	PitchMotor.TargetSpeed = 0;
	PitchMotor.RealAngle = 0;
	PitchMotor.TargetAngle = 0;
	PitchMotor.MechanicalAngle = 0;
	PitchMotor.RotationalSpeed = 0;
	PitchMotor.RealTorque = 0;

	PitchMotor.NeedOutput = 0;
	PitchMotor.MaximumOutput = PitchMotor_MaximumOutput;
	PitchMotor.SpeedPID.Kp = PitchMotor_SpeedPID_Kp;
	PitchMotor.SpeedPID.Ki = PitchMotor_SpeedPID_Ki;
	PitchMotor.SpeedPID.Kd = PitchMotor_SpeedPID_Kd;
	PitchMotor.SpeedPID.Dead_Band = PitchMotor_SpeedPID_DeadBand;
	PitchMotor.SpeedPID.Maximum_Integration = PitchMotor_SpeedPID_Maximum_Integration;
	PitchMotor.SpeedPID.Current_Error = 0;
	PitchMotor.SpeedPID.Last_Error = 0;
	PitchMotor.SpeedPID.Sum_Integration = 0;
	PitchMotor.SpeedPID.Output = 0;
	PitchMotor.AnglePID.Kp = PitchMotor_AnglePID_Kp;
	PitchMotor.AnglePID.Ki = PitchMotor_AnglePID_Ki;
	PitchMotor.AnglePID.Kd = PitchMotor_AnglePID_Kd;
	PitchMotor.AnglePID.Dead_Band = PitchMotor_AnglePID_DeadBand;
	PitchMotor.AnglePID.Maximum_Integration = PitchMotor_AnglePID_Maximum_Integration;
	PitchMotor.AnglePID.Current_Error = 0;
	PitchMotor.AnglePID.Last_Error = 0;
	PitchMotor.AnglePID.Sum_Integration = 0;
	PitchMotor.AnglePID.Output = 0;
	
	YawMotor.model = GM6020;
	YawMotor.communication = Can;
	YawMotor.control = double_loop;
	YawMotor.comm_status.link_status = offline;
	YawMotor.comm_status.FrameCounter = 0; 
	YawMotor.comm_id = 0x208;
	
	YawMotor.RealSpeed = 0;
	YawMotor.TargetSpeed = 0;
	YawMotor.RealAngle = 0;
	YawMotor.TargetAngle = 0;
	YawMotor.MechanicalAngle = 0;
	YawMotor.RotationalSpeed = 0;
	YawMotor.RealTorque = 0;

	YawMotor.NeedOutput = 0;
	YawMotor.MaximumOutput = YawMotor_MaximumOutput;
	YawMotor.SpeedPID.Kp = YawMotor_SpeedPID_Kp;
	YawMotor.SpeedPID.Ki = YawMotor_SpeedPID_Ki;
	YawMotor.SpeedPID.Kd = YawMotor_SpeedPID_Kd;
	YawMotor.SpeedPID.Dead_Band = YawMotor_SpeedPID_DeadBand;
	YawMotor.SpeedPID.Maximum_Integration = YawMotor_SpeedPID_Maximum_Integration;
	YawMotor.SpeedPID.Current_Error = 0;
	YawMotor.SpeedPID.Last_Error = 0;
	YawMotor.SpeedPID.Sum_Integration = 0;
	YawMotor.SpeedPID.Output = 0;
	YawMotor.AnglePID.Kp = YawMotor_AnglePID_Kp;
	YawMotor.AnglePID.Ki = YawMotor_AnglePID_Ki;
	YawMotor.AnglePID.Kd = YawMotor_AnglePID_Kd;
	YawMotor.AnglePID.Dead_Band = YawMotor_AnglePID_DeadBand;
	YawMotor.AnglePID.Maximum_Integration = YawMotor_AnglePID_Maximum_Integration;
	YawMotor.AnglePID.Current_Error = 0;
	YawMotor.AnglePID.Last_Error = 0;
	YawMotor.AnglePID.Sum_Integration = 0;
	YawMotor.AnglePID.Output = 0;
}

void Gimbal_SetPosition(void)
{
	static int first =1;
	if(first == 1)
	{
		PitchMotor.TargetAngle = Gimbal.position.PitchAngle;
		PitchMotor.SpeedPID.Sum_Integration = 0;
		YawMotor.TargetAngle = Gimbal.position.YawAngle;
		first = 0;
	}
	PitchMotor.RealAngle = Gimbal.position.PitchAngle;
	PitchMotor.RealSpeed = Gimbal.position.PitchSpeed;

	YawMotor.RealAngle = Gimbal.position.YawAngle;
	YawMotor.RealSpeed = Gimbal.position.YawSpeed;

	Gimbal.position.x = ext_game_robot_pos.x;
	Gimbal.position.y = ext_game_robot_pos.y;
	Gimbal.position.z = ext_game_robot_pos.z;
	
}

void Gimbal_Control(void)
{
	static int runonce =0;

	if(runonce == 1)
	{
		YawMotor.TargetAngle = Gimbal.position.YawAngle;
		if(Gimbal_StartMove() == 3)
			runonce = 0;
		runonce =0;
		PID_Calculation(&PitchMotor);
		PID_Calculation(&YawMotor);
	}
	else
	{
		if(Gimbal.control_mode == ControlMode_Manual)
		{
			PitchMotor.TargetAngle += 0.25 * PitchAngle_StepLength * Get_Channel_Val(&RC_ReceiveData, RC_CH3) / RC_CH_MAX_RELATIVE;
			YawMotor.TargetAngle += 0.5 * YawAngle_StepLength * Get_Channel_Val(&RC_ReceiveData, RC_CH0) / RC_CH_MAX_RELATIVE;
			
			if(PitchMotor.TargetAngle > PitchAngle_Uplimit)
				PitchMotor.TargetAngle = PitchAngle_Uplimit;
			else if(PitchMotor.TargetAngle<PitchAngle_Downlimit)
				PitchMotor.TargetAngle = PitchAngle_Downlimit;
			
			if(YawMotor.TargetAngle > 155)
				YawMotor.TargetAngle = 155;
			else if(YawMotor.TargetAngle<-155)
				YawMotor.TargetAngle = -155;
			
			PID_Calculation(&PitchMotor);
			PID_Calculation(&YawMotor);
			
		}

		else if(Gimbal.control_mode == ControlMode_PC)
		{	
			if (Gimbal.aim_mode == AimMode_Manual)
			{
				YawMotor.TargetAngle += 0.01f * (Get_Mouse_Speed(&RC_ReceiveData, MOUSE_X));
				PitchMotor.TargetAngle -= 0.01f * (Get_Mouse_Speed(&RC_ReceiveData, MOUSE_Y));
			}
			else if (Gimbal.aim_mode == AimMode_Auto)
			{
				YawMotor.TargetAngle = Desire_Angle_Yaw + Wild_Change_Angle_Pitch.FLOAT;
				PitchMotor.TargetAngle = Desire_Angle_Pitch + Wild_Change_Angle_Yaw.FLOAT; 
				
			}
			else if (Gimbal.aim_mode == AimMode_Outpost)
			{
				YawMotor.TargetAngle = Outpost_Angle_Yaw;
				PitchMotor.TargetAngle = Outpost_Angle_Pitch;
			}
			else if (Gimbal.aim_mode == AimMode_Base)
			{
				YawMotor.TargetAngle = Base_Angle_Yaw;
				PitchMotor.TargetAngle = Base_Angle_Pitch;
			}
			/*限制幅度*/
			if(PitchMotor.TargetAngle > PitchAngle_Uplimit)
				PitchMotor.TargetAngle = PitchAngle_Uplimit;
			else if(PitchMotor.TargetAngle < PitchAngle_Downlimit)
				PitchMotor.TargetAngle = PitchAngle_Downlimit;
			
			if(YawMotor.TargetAngle > 155)
				YawMotor.TargetAngle = 155;
			else if(YawMotor.TargetAngle<-155)
				YawMotor.TargetAngle = -155;
			
			PID_Calculation(&PitchMotor);
			PID_Calculation(&YawMotor);
		}

		else if(Gimbal.control_mode == ControlMode_Protect)
		{
			PitchMotor.TargetAngle = Gimbal.position.PitchAngle;
			PitchMotor.SpeedPID.Sum_Integration = 0;
			YawMotor.TargetAngle = Gimbal.position.YawAngle;
			PitchMotor.NeedOutput = 0;
			YawMotor.NeedOutput = 0;
		}
	}

	PitchMotor.NeedOutput = PitchMotor.NeedOutput*-1;
	Motor_CanSend_Gimbal(&PitchMotor,&YawMotor);
}

void Gimbal_Protect(void)
{
}

int Gimbal_StartMove(void)
{
	static uint8_t step = 0;
	
	if(PitchMotor.RealAngle < 9 && step == 0)
	{
		if(PitchMotor.TargetAngle < 10)
			PitchMotor.TargetAngle += 0.04f;
		return step;
	}
	else if(step == 0)
		step = 1;
	
	if(PitchMotor.RealAngle > -29 && step == 1)
	{
		if(PitchMotor.TargetAngle > -30)
			PitchMotor.TargetAngle -= 0.08f;
		return step;
	}
	else if(step == 1)
		step = 2;
	
	if(PitchMotor.RealAngle < -1 && step == 2)
	{
		if(PitchMotor.TargetAngle < 0)
			PitchMotor.TargetAngle += 0.08f;
		return step;
	}
	else if(step == 2)
		step = 3;
	return step;
}




