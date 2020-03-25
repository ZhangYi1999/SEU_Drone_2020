#include "Task_CanComm.h"

CanSend_type Can_TX_Message;

void Task_CanComm(void *parameters)
{
	while(1)
	{
		xQueueReceive(Queue_CANSend, &Can_TX_Message, portMAX_DELAY);
		switch(Can_TX_Message.CANx)
		{
			case Can1:
			#ifdef CAN_ONE_USED
				HAL_CAN_AddTxMessage(&hcan1,&Can_TX_Message.SendCanHeader,Can_TX_Message.SendCanTxMsg,(uint32_t*)CAN_TX_MAILBOX0);
			#endif
				break;
			case Can2:
			#ifdef CAN_TWO_USED
				HAL_CAN_AddTxMessage(&hcan2,&Can_TX_Message.SendCanHeader,Can_TX_Message.SendCanTxMsg,(uint32_t*)CAN_TX_MAILBOX1);
			#endif
				break;
			default:
				break;
		}
	}
}

CAN_RxHeaderTypeDef RxHeader;
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	
  uint8_t aData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &RxHeader, aData);
	switch(RxHeader.StdId)
	{
		case 0x202:
			FricMotor_Left.Last_MechanicalAngle = FricMotor_Left.MechanicalAngle;
			FricMotor_Left.MechanicalAngle = aData[0] << 8 | aData[1];
			FricMotor_Left.RotationalSpeed = aData[2] << 8 | aData[3];
			FricMotor_Left.RealCurrent = aData[4] << 8 | aData[5];
			FricMotor_Left.comm_status.FrameCounter++;
			FricMotor_Left.comm_status.link_status = online;
			break;
		case 0x201:
			FricMotor_Right.Last_MechanicalAngle = FricMotor_Right.MechanicalAngle;
			FricMotor_Right.MechanicalAngle = aData[0] << 8 | aData[1];
			FricMotor_Right.RotationalSpeed = aData[2] << 8 | aData[3];
			FricMotor_Right.RealCurrent = aData[4] << 8 | aData[5];
			FricMotor_Right.comm_status.FrameCounter++;
			FricMotor_Right.comm_status.link_status = online;
			break;
		case 0x208:
			YawMotor.Last_MechanicalAngle = YawMotor.MechanicalAngle;
			YawMotor.MechanicalAngle = aData[0] << 8 | aData[1];
			YawMotor.RotationalSpeed = aData[2] << 8 | aData[3];
			YawMotor.RealCurrent = aData[4] << 8 | aData[5];
			YawMotor.comm_status.FrameCounter++;
			YawMotor.comm_status.link_status = online;			
			break;
		case 0x207:
			PitchMotor.Last_MechanicalAngle = PitchMotor.MechanicalAngle;
			PitchMotor.MechanicalAngle = aData[0] << 8 | aData[1];
			PitchMotor.RotationalSpeed = aData[2] << 8 | aData[3];
			PitchMotor.RealCurrent = aData[4] << 8 | aData[5];
			PitchMotor.comm_status.FrameCounter++;
			PitchMotor.comm_status.link_status = online;
			break;
		case 0x204:
			StirMotor.Last_MechanicalAngle = StirMotor.MechanicalAngle;
			StirMotor.MechanicalAngle = aData[0] << 8 | aData[1];
			StirMotor.RotationalSpeed = aData[2] << 8 | aData[3];
			StirMotor.RealCurrent = aData[4] << 8 | aData[5];
			StirMotor.comm_status.FrameCounter++;
			StirMotor.comm_status.link_status = online;		

			StirMotor.RealAngle += delta_angle(StirMotor.RotationalSpeed, StirMotor.Last_MechanicalAngle,StirMotor.MechanicalAngle)*360.0/(8191*StirMotor_Reduction_Ratio);
			break;
		default:
			break;
	}
}	

int16_t delta_angle(int16_t speed, uint16_t mechanical_angle_last, uint16_t mechanical_angle_now)
{
	if (speed > 0)
	{
		if (mechanical_angle_last > mechanical_angle_now + 100) 
			return (mechanical_angle_now - mechanical_angle_last + 8192);
		else
			return (mechanical_angle_now - mechanical_angle_last);
	}
	else if (speed < 0)
	{
		if (mechanical_angle_last + 100 < mechanical_angle_now)
			return (mechanical_angle_now - mechanical_angle_last - 8192);
		else
			return (mechanical_angle_now - mechanical_angle_last);
	}
	else
		return (mechanical_angle_now - mechanical_angle_last);
}
