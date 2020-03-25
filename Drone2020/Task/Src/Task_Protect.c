#include "Task_Protect.h"


void Task_Protect(void *parameters)
{
	static Comm_Status GYRO_Last_Status;
	static uint8_t GYRO_offline_time = 0;
	
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
		if(GYRO.comm_status.FrameCounter == GYRO_Last_Status.FrameCounter)
		{
			if(GYRO_offline_time<255)
				GYRO_offline_time++;
		}
		else
			GYRO_offline_time = 0;
		GYRO_Last_Status = GYRO.comm_status;
		if(GYRO_offline_time > 10)								//陀螺仪离线时间超过 5*10 = 50ms时
			GYRO.comm_status.link_status = offline;
	
	}
}
