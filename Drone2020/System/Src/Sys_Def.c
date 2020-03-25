#include "Sys_Def.h"

Motor_type PitchMotor;
Motor_type YawMotor;
Motor_type FricMotor_Left;
Motor_type FricMotor_Right;
Motor_type StirMotor;

RCDecoding_type RC_ReceiveData,LastRC_ReceiveData;
uint8_t RCBuffer[2][RC_FRAME_LEN+RC_FRAME_LEN_BACK];
uint8_t RC_Rx_Mem;

uint8_t Judge_Receive_Buffer[130];

GYRO_type GYRO;

CanSend_type CAN_Tx_Msg;

Gimbal_type Gimbal;

JetsonFlag_Struct JetsonFlag[JETSONFLAG_LEN];
uint8_t Jetson_Seq;
STMToJetson_Struct_Gyro DataSendToJetson_gyro;
JetsonToSTM_Struct DataRecFromJetson,DataRecFromJetson_Temp;
STMToJetson_Struct DataSendToJetson;
CommStatus_Struct CommStatus;
float Pitch_Desire, Yaw_Desire;
float Pitch_otcAngle,Yaw_otcAngle;
float Jetson_AnglePitch;
float Jetson_AngleYaw;
float Jetson_VelocityPitch;
float Jetson_VelocityYaw;

GYRO_type PersonalGYRO,PersonalGYRO2;
uint8_t GYROBuffer[2][PersonalGYRO_rx_len],GYROBuffer2[2][PersonalGYRO_rx_len];
uint8_t GYRO_Rx_Mem,GYRO_Rx_Mem2;


void PID_Calculation(Motor_type* motor)
{
	if(motor->control==open_loop)
		return;
	else if(motor->control==single_loop)
	{
		motor->SpeedPID.Last_Error=motor->SpeedPID.Current_Error;
		motor->SpeedPID.Current_Error=motor->TargetSpeed-motor->RealSpeed;
		//motor->SpeedPID.Current_Error=Limit(motor->SpeedPID.Current_Error,motor->SpeedPID.Dead_Band);
		
		motor->SpeedPID.Sum_Integration += motor->SpeedPID.Ki * motor->SpeedPID.Current_Error;
		motor->SpeedPID.Sum_Integration=Limit(motor->SpeedPID.Sum_Integration,motor->SpeedPID.Maximum_Integration);
		
		motor->SpeedPID.Output = motor->SpeedPID.Kp * motor->SpeedPID.Current_Error \
													 + motor->SpeedPID.Sum_Integration \
													 + motor->SpeedPID.Kd * (motor->SpeedPID.Current_Error - motor->SpeedPID.Last_Error);
		motor->SpeedPID.Output = Limit(motor->SpeedPID.Output,motor->MaximumOutput);
		motor->NeedOutput = motor->SpeedPID.Output;
	}
	else if(motor->control==double_loop)
	{
		motor->AnglePID.Last_Error = motor->AnglePID.Current_Error;
		motor->AnglePID.Current_Error = motor->TargetAngle - motor->RealAngle;
		if(__fabs(motor->AnglePID.Current_Error) < motor->AnglePID.Dead_Band)
			motor->AnglePID.Current_Error = 0;
		
		motor->AnglePID.Sum_Integration += motor->AnglePID.Ki * motor->AnglePID.Current_Error;
		motor->AnglePID.Sum_Integration=Limit(motor->AnglePID.Sum_Integration,motor->AnglePID.Maximum_Integration);
		
		motor->AnglePID.Output = motor->AnglePID.Kp * motor->AnglePID.Current_Error \
													 + motor->AnglePID.Sum_Integration \
													 + motor->AnglePID.Kd * (motor->AnglePID.Current_Error - motor->AnglePID.Last_Error);
		
		motor->TargetSpeed = motor->AnglePID.Output;
		
		motor->SpeedPID.Last_Error = motor->SpeedPID.Current_Error;
		motor->SpeedPID.Current_Error = motor->TargetSpeed-motor->RealSpeed;
		//motor->SpeedPID.Current_Error=Limit(motor->SpeedPID.Current_Error,motor->SpeedPID.Dead_Band);
		
		motor->SpeedPID.Sum_Integration += motor->SpeedPID.Ki * motor->SpeedPID.Current_Error;
		motor->SpeedPID.Sum_Integration=Limit(motor->SpeedPID.Sum_Integration,motor->SpeedPID.Maximum_Integration);
		
		motor->SpeedPID.Output = motor->SpeedPID.Kp * motor->SpeedPID.Current_Error \
													 + motor->SpeedPID.Sum_Integration \
													 + motor->SpeedPID.Kd * (motor->SpeedPID.Current_Error - motor->SpeedPID.Last_Error);
		
		motor->SpeedPID.Output = Limit(motor->SpeedPID.Output,motor->MaximumOutput);
		motor->NeedOutput = motor->SpeedPID.Output;
	}
	else
		return;
	
	motor->NeedOutput = Limit(motor->NeedOutput,motor->MaximumOutput);
}
