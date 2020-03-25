#include "System.h"

QueueHandle_t Queue_CANSend;

__weak void Task_Protect(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_CanComm(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_Position(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}
__weak void Task_Judge(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_JetsonComm(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_RC(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_StateMachine(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_Gimbal(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_Shoot(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

__weak void Task_Debug(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}
__weak void Task_LED(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
	}
}

void Motor_SetOutput(Motor_type* motor,int16_t output)
{
	motor->NeedOutput = output;
}

void Motor_CanSend(Motor_type* motor)
{
	uint8_t offset;
	CanSend_type Package_to_Send;
	Package_to_Send.CANx = Can1;
	if(motor->comm_id < 0x205)
	{
		Package_to_Send.SendCanHeader.StdId = 0x200;
		offset = (motor->comm_id - 0x200) * 2 - 1;
	}
	else
	{
		Package_to_Send.SendCanHeader.StdId = 0x1FF;
		offset = (motor->comm_id - 0x204) * 2 - 1;
	}
	Package_to_Send.SendCanHeader.ExtId = 0;
	Package_to_Send.SendCanHeader.DLC = 8;
	Package_to_Send.SendCanHeader.IDE = CAN_ID_STD;
	Package_to_Send.SendCanHeader.RTR = CAN_RTR_DATA;
	Package_to_Send.SendCanTxMsg[0] = 0x00;
	Package_to_Send.SendCanTxMsg[1] = 0x00;
	Package_to_Send.SendCanTxMsg[2] = 0x00;
	Package_to_Send.SendCanTxMsg[3] = 0x00;
	Package_to_Send.SendCanTxMsg[4] = 0x00;
	Package_to_Send.SendCanTxMsg[5] = 0x00;
	Package_to_Send.SendCanTxMsg[6] = 0x00;
	Package_to_Send.SendCanTxMsg[7] = 0x00;
	Package_to_Send.SendCanTxMsg[offset-1] = (uint8_t)(motor->NeedOutput>>8);
	Package_to_Send.SendCanTxMsg[offset] = (uint8_t)(motor->NeedOutput);
	
	xQueueSend(Queue_CANSend, &Package_to_Send, 3 / portTICK_RATE_MS);
}


void Motor_CanSend_Gimbal(Motor_type* Pitch,Motor_type* Yaw)
{
	uint8_t offset1,offset2;
	CanSend_type Package_to_Send;
	Package_to_Send.CANx = Can1;
	if(Pitch->comm_id < 0x205)
	{
		Package_to_Send.SendCanHeader.StdId = 0x200;
		offset1 = (Pitch->comm_id - 0x200) * 2 - 1;
		offset2 = (Yaw->comm_id - 0x200) * 2 - 1;
	}
	else
	{
		Package_to_Send.SendCanHeader.StdId = 0x1FF;
		offset1 = (Pitch->comm_id - 0x204) * 2 - 1;
		offset2 = (Yaw->comm_id - 0x204) * 2 - 1;
	}
	Package_to_Send.SendCanHeader.ExtId = 0;
	Package_to_Send.SendCanHeader.DLC = 8;
	Package_to_Send.SendCanHeader.IDE = CAN_ID_STD;
	Package_to_Send.SendCanHeader.RTR = CAN_RTR_DATA;
	Package_to_Send.SendCanTxMsg[0] = 0x00;
	Package_to_Send.SendCanTxMsg[1] = 0x00;
	Package_to_Send.SendCanTxMsg[2] = 0x00;
	Package_to_Send.SendCanTxMsg[3] = 0x00;
	Package_to_Send.SendCanTxMsg[4] = 0x00;
	Package_to_Send.SendCanTxMsg[5] = 0x00;
	Package_to_Send.SendCanTxMsg[6] = 0x00;
	Package_to_Send.SendCanTxMsg[7] = 0x00;
	Package_to_Send.SendCanTxMsg[offset1-1] = (uint8_t)(Pitch->NeedOutput>>8);
	Package_to_Send.SendCanTxMsg[offset1] = (uint8_t)(Pitch->NeedOutput);
	Package_to_Send.SendCanTxMsg[offset2-1] = (uint8_t)(Yaw->NeedOutput>>8);
	Package_to_Send.SendCanTxMsg[offset2] = (uint8_t)(Yaw->NeedOutput);
	
	xQueueSend(Queue_CANSend, &Package_to_Send, 3 / portTICK_RATE_MS);
}

void Motor_CanSend_Shoot(Motor_type* FricMotor_Left,Motor_type* FricMotor_Right,Motor_type* StirMotor)
{
	uint8_t offset1,offset2,offset3;
	CanSend_type Package_to_Send;
	Package_to_Send.CANx = Can1;
	if(FricMotor_Left->comm_id < 0x205)
	{
		Package_to_Send.SendCanHeader.StdId = 0x200;
		offset1 = (FricMotor_Left->comm_id - 0x200) * 2 - 1;
		offset2 = (FricMotor_Right->comm_id - 0x200) * 2 - 1;
		offset3 = (StirMotor->comm_id - 0x200) * 2 - 1;
	}
	else
	{
		Package_to_Send.SendCanHeader.StdId = 0x204;
		offset1 = (FricMotor_Left->comm_id - 0x204) * 2 - 1;
		offset2 = (FricMotor_Right->comm_id - 0x204) * 2 - 1;
		offset3 = (StirMotor->comm_id - 0x204) * 2 - 1;
	}
	Package_to_Send.SendCanHeader.ExtId = 0;
	Package_to_Send.SendCanHeader.DLC = 8;
	Package_to_Send.SendCanHeader.IDE = CAN_ID_STD;
	Package_to_Send.SendCanHeader.RTR = CAN_RTR_DATA;
	Package_to_Send.SendCanTxMsg[0] = 0x00;
	Package_to_Send.SendCanTxMsg[1] = 0x00;
	Package_to_Send.SendCanTxMsg[2] = 0x00;
	Package_to_Send.SendCanTxMsg[3] = 0x00;
	Package_to_Send.SendCanTxMsg[4] = 0x00;
	Package_to_Send.SendCanTxMsg[5] = 0x00;
	Package_to_Send.SendCanTxMsg[6] = 0x00;
	Package_to_Send.SendCanTxMsg[7] = 0x00;
	Package_to_Send.SendCanTxMsg[offset1-1] = (uint8_t)(FricMotor_Left->NeedOutput>>8);
	Package_to_Send.SendCanTxMsg[offset1] = (uint8_t)(FricMotor_Left->NeedOutput);
	Package_to_Send.SendCanTxMsg[offset2-1] = (uint8_t)(FricMotor_Right->NeedOutput>>8);
	Package_to_Send.SendCanTxMsg[offset2] = (uint8_t)(FricMotor_Right->NeedOutput);
	Package_to_Send.SendCanTxMsg[offset3-1] = (uint8_t)(StirMotor->NeedOutput>>8);
	Package_to_Send.SendCanTxMsg[offset3] = (uint8_t)(StirMotor->NeedOutput);
	
	xQueueSend(Queue_CANSend, &Package_to_Send, 3 / portTICK_RATE_MS);
}
