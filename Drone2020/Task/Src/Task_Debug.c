#include "Task_Debug.h"

uint8_t Head[2];
uint8_t Tail[2];

void Task_Debug(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	
	Head[0] = 0x03;
	Head[1] = 0xFC;
	Tail[0] = 0xFC;
	Tail[1] = 0x03;
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,5);
		Encode_type data;
		data.SrcData = PitchMotor.RealAngle;
		HAL_UART_Transmit(&huart7,Head,2,100);
		HAL_UART_Transmit(&huart7,data.Data,4,100);
		HAL_UART_Transmit(&huart7,Tail,2,100);
	}
}
