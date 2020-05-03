#include "Task_Position.h"
#include "Func_Imu_OB.h"

void Task_Position(void *parameters)
{
	TickType_t xLastWakeUpTime,Start_Time,Present_Time;
	xLastWakeUpTime = xTaskGetTickCount();
	Start_Time = xTaskGetTickCount();
	PositionInit();
	while(1)
	{
#ifdef PERSONALGYRO_USED
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		Personal_GYRO_Receive(&GYRO);
#endif
#ifdef ONBOARDIMU_USED
		vTaskDelayUntil(&xLastWakeUpTime, 2);
		mpu_get_data();
		imu_ahrs_update();
		imu_attitude_update();
		imu_temp_ctrl();
#endif
		Present_Time = xTaskGetTickCount();
		if(Present_Time - Start_Time > 10000)    //陀螺仪上电10s后，自动较准完成
		GYRO.comm_status.link_status = online;
		Gimbal_PositionSet();
	}
}

void PositionInit(void)
{
	Gimbal.position.PitchAngle = 0;
	Gimbal.position.RollAngle = 0;
	Gimbal.position.YawAngle = 0;
	Gimbal.position.PitchSpeed = 0;
	Gimbal.position.YawSpeed = 0;
	Gimbal.position.RollSpeed = 0;
	
	GYRO.comm_id = 1;
	GYRO.comm_status.link_status= offline;
	GYRO.comm_status.FrameCounter= 0;
	GYRO.model = HI219;
	GYRO.PitchAngle = 0;


}

void Personal_GYRO_Receive(GYRO_type *gyro)
{
	gyro->PitchAngle = -((int16_t)(GYROBuffer[GYRO_Rx_Mem][7] | GYROBuffer[GYRO_Rx_Mem][8] << 8)) / 100.0f; 
	gyro->RollAngle =  ((int16_t)(GYROBuffer[GYRO_Rx_Mem][9] | GYROBuffer[GYRO_Rx_Mem][10] << 8)) / 100.0f;
	gyro->YawAngle = - ((int16_t)(GYROBuffer[GYRO_Rx_Mem][11] | GYROBuffer[GYRO_Rx_Mem][12] << 8)) / 10.0f;
	gyro->Gyro_X = SpliceFloat(&GYROBuffer[GYRO_Rx_Mem][26]);
	gyro->Gyro_Y = SpliceFloat(&GYROBuffer[GYRO_Rx_Mem][30]);
	gyro->Gyro_Z = SpliceFloat(&GYROBuffer[GYRO_Rx_Mem][34]);

	gyro->comm_status.FrameCounter++;
}

void Gimbal_PositionSet(void)
{
#ifdef PERSONALGYRO_USED
	Gimbal.position.PitchAngle = GYRO.PitchAngle;
	Gimbal.position.RollAngle = GYRO.RollAngle;
	Gimbal.position.YawAngle = GYRO.YawAngle;
	Gimbal.position.PitchSpeed = GYRO.Gyro_Y*-1;
	Gimbal.position.YawSpeed = GYRO.Gyro_Z*-1;
	Gimbal.position.RollSpeed = GYRO.Gyro_X;
#endif
#ifdef ONBOARDIMU_USED
	float last_pit = 0,last_yaw = 0;

	Gimbal.position.PitchAngle = (6754 - PitchMotor.MechanicalAngle) * 0.044f;
	last_pit = imu.pit;

	Gimbal.position.RollAngle = imu.rol;
	if(YawMotor.MechanicalAngle < 7568)
		Gimbal.position.YawAngle = (YawMotor.MechanicalAngle - 3472) * 0.044f;
	else
		Gimbal.position.YawAngle = (YawMotor.MechanicalAngle - 3472 - 8191) * 0.044f;
	last_yaw = imu.yaw;

	Gimbal.position.PitchSpeed = imu.wy * 57.3f;
	Gimbal.position.YawSpeed = imu.wz * -57.3f;
	Gimbal.position.RollSpeed = imu.wx * 57.3f;
#endif
}

uint8_t usart_this_time_rx_len;
void Uart_Config_In_IRQHandle(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskToWaken = pdFALSE;
	usart_this_time_rx_len = 0;
	DMA_HandleTypeDef *hdma_uart_rx = huart->hdmarx;
	if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET)
	{
		//clear the idle pending flag
		(void)huart->Instance->SR;
		(void)huart->Instance->DR;

		__HAL_UART_CLEAR_IDLEFLAG(huart);
		__HAL_DMA_DISABLE(hdma_uart_rx);
		usart_this_time_rx_len = PersonalGYRO_rx_len - __HAL_DMA_GET_COUNTER(hdma_uart_rx);

		if ((hdma_uart_rx->Instance->CR & DMA_SxCR_CT) != RESET)
		{
			/* Current memory buffer used is Memory 1 */
			hdma_uart_rx->Instance->CR &= (uint32_t)(~DMA_SxCR_CT);
			GYRO_Rx_Mem = MEMORY1;
		}
		else
		{
			/* Current memory buffer used is Memory 0 */
			hdma_uart_rx->Instance->CR |= (uint32_t)DMA_SxCR_CT;
			GYRO_Rx_Mem = MEMORY0;
		}
		if(usart_this_time_rx_len == PersonalGYROFrameLength\
			&& GYROBuffer[GYRO_Rx_Mem][0] == GYROFRAMEHEAD0 \
			&& GYROBuffer[GYRO_Rx_Mem][1] == GYROFRAMEHEAD1) 
			//ע̍лϢ֪ͨ
			vTaskNotifyGiveFromISR(TaskPosition_Handle, &xHigherPriorityTaskToWaken);
		else 
			GYRO_Rx_Mem = MEMORYRESET;
	
		__HAL_DMA_SET_COUNTER(hdma_uart_rx, PersonalGYRO_rx_len);
		__HAL_DMA_ENABLE(hdma_uart_rx);

		portYIELD_FROM_ISR(xHigherPriorityTaskToWaken);
	}
}



float a=0.15f;
float LowFilter(float last_value,float current_value)
{
	return (1-a)*current_value + a*last_value;
}

#define N 30   
float value_buf[N];
uint8_t i=0;  
float AverageFilter(float current_value)
{  
   uint8_t count;  
	 //uint8_t n,m;
   float  sum=0.0f;//,temp=0;
   value_buf[i++] = current_value;  
   if ( i == N )   i = 0;  
   for ( count=0;count<N;count++)  
			sum += value_buf[count];  
   return (float)(sum/N);  
} 


