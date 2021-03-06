#include "Sys_Config.h"

TaskHandle_t TaskProtect_Handle;
TaskHandle_t TaskCanComm_Handle;
TaskHandle_t TaskPosition_Handle;
TaskHandle_t TaskJudge_Handle;
TaskHandle_t TaskJetsonComm_Handle;
TaskHandle_t TaskRC_Handle;
TaskHandle_t TaskStateMachine_Handle;
TaskHandle_t TaskGimbal_Handle;
TaskHandle_t TaskShoot_Handle;
TaskHandle_t TaskDebug_Handle;
TaskHandle_t TaskLED_Handle;
TaskHandle_t TaskUi_Handle;
TaskHandle_t TaskSDIO_Handle;
TaskHandle_t TaskTOF_Handle;

void Task_Init_Config(void const * argument)
{
	taskENTER_CRITICAL();
	
	Queue_CANSend = xQueueCreate(20,sizeof(CanSend_type));
	
	CanInit(NULL);
	DMAInit(NULL);
	TIMInit(NULL);
	mpu_device_init();
	init_quaternion();
	HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);
	__HAL_TIM_SET_COMPARE(&htim3,IMU_HEATING_Pin,HEAT_MID);	

	xTaskCreate(Task_Protect, "Task_Protect", 256, NULL, 7, &TaskProtect_Handle);
	xTaskCreate(Task_CanComm, "Task_CanComm", 256, NULL, 6, &TaskCanComm_Handle);
	xTaskCreate(Task_Position, "Task_Position", 256, NULL, 5, &TaskPosition_Handle);
	xTaskCreate(Task_Judge, "Task_Judge", 256, NULL, 5, &TaskJudge_Handle);
	xTaskCreate(Task_JetsonComm, "Task_JetsonComm", 256, NULL, 4, &TaskJetsonComm_Handle);
	xTaskCreate(Task_RC, "Task_RC", 256, NULL, 4, &TaskRC_Handle);
	xTaskCreate(Task_StateMachine, "Task_StateMachine", 256, NULL, 4, &TaskStateMachine_Handle);
	xTaskCreate(Task_Gimbal, "Task_Gimbal", 256, NULL, 3, &TaskGimbal_Handle);
	xTaskCreate(Task_Shoot, "Task_Shoot", 256, NULL, 3, &TaskShoot_Handle);
	xTaskCreate(Task_Ui, "Task_Ui", 256, NULL, 3, &TaskUi_Handle);
	xTaskCreate(Task_Debug, "Task_Debug", 125, NULL, 3, &TaskDebug_Handle);
	xTaskCreate(Task_TOF, "Task_TOF", 125, NULL, 3, &TaskTOF_Handle);
	
	xTaskCreate(Task_LED, "Task_LED", 125, NULL, 2, &TaskLED_Handle);
	xTaskCreate(Task_SDIO, "Task_SDIO", 512, NULL, 2, &TaskSDIO_Handle);

	vTaskDelete(NULL);
	taskEXIT_CRITICAL();
}


void CanInit(void *parameters)
{
#ifdef CAN_ONE_USED
	uint32_t FilterBank1, FilterFIFO1;
  CAN_FilterTypeDef sFilterConfig1;
	
	FilterBank1 = 0;
  FilterFIFO1 = CAN_RX_FIFO0;
	
	sFilterConfig1.FilterBank = FilterBank1;
	sFilterConfig1.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig1.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig1.FilterIdHigh = 0x0000;
	sFilterConfig1.FilterIdLow = 0x0000;
	sFilterConfig1.FilterMaskIdHigh = 0x0000;
	sFilterConfig1.FilterMaskIdLow = 0x0000;
	sFilterConfig1.FilterFIFOAssignment = FilterFIFO1;
	sFilterConfig1.FilterActivation = ENABLE;
	sFilterConfig1.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig1);
	HAL_CAN_Start(&hcan1);
	
	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
#endif

#ifdef CAN_TWO_USED
	uint32_t FilterBank2, FilterFIFO2;
  CAN_FilterTypeDef sFilterConfig2;
	
	FilterBank2 = 14;
  FilterFIFO2 = CAN_RX_FIFO1;
	
	sFilterConfig2.FilterBank = FilterBank2;
	sFilterConfig2.FilterMode = CAN_FILTERMODE_IDMASK;
	sFilterConfig2.FilterScale = CAN_FILTERSCALE_32BIT;
	sFilterConfig2.FilterIdHigh = 0x0000;
	sFilterConfig2.FilterIdLow = 0x0000;
	sFilterConfig2.FilterMaskIdHigh = 0x0000;
	sFilterConfig2.FilterMaskIdLow = 0x0000;
	sFilterConfig2.FilterFIFOAssignment = FilterFIFO2;
	sFilterConfig2.FilterActivation = ENABLE;
	sFilterConfig2.SlaveStartFilterBank = 14;
	HAL_CAN_ConfigFilter(&hcan2, &sFilterConfig2);
	HAL_CAN_Start(&hcan2);
	
	HAL_CAN_ActivateNotification(&hcan2, CAN_IT_RX_FIFO1_MSG_PENDING);
#endif
}

void DMAInit(void *parameters)
{
#ifdef DMA_RC_USED
	SET_BIT(huart1.Instance->CR3, USART_CR3_DMAR);
	HAL_DMAEx_MultiBufferStart(huart1.hdmarx,(uint32_t)&(huart1.Instance->DR),(uint32_t)&RCBuffer[0][0],(uint32_t)&RCBuffer[1][0],(RC_FRAME_LEN+RC_FRAME_LEN_BACK));
  	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
#endif

#ifdef DMA_Judge_USED
	HAL_UART_Receive_DMA(&huart3, (uint8_t*)Judge_Receive_Buffer, REFEREE_DMA_SIZE);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
#endif

#ifdef DMA_JetsonTX2_USED
	HAL_UART_Receive_DMA(&huart6, (uint8_t*)JetsonBuffer, 100);
	__HAL_UART_ENABLE_IT(&huart6, UART_IT_IDLE);
	__HAL_UART_CLEAR_IDLEFLAG(&huart6);
#endif

#ifdef DMA_GYRO_USED
	SET_BIT(huart8.Instance->CR3, USART_CR3_DMAR);
	 HAL_DMAEx_MultiBufferStart(huart8.hdmarx, (uint32_t)&huart8.Instance->DR, (uint32_t)&GYROBuffer[0][0], (uint32_t)&GYROBuffer[1][0], PersonalGYRO_rx_len);
	 __HAL_UART_ENABLE_IT(&huart8, UART_IT_IDLE);
#endif

#ifdef DMA_TOF_USED
	HAL_UART_Receive_DMA(&huart4, TOFBuffer, 19);
#endif

}

void TIMInit(void *parameters)
{
}
