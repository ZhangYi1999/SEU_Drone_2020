#include "Sys_Config.h"

//TOF专用
uint8_t TOFBuffer[TOF_DMA_SIZE];
float height_data_temp[4];

int tof=0;

void Task_TOF(void *parameters)
{

		TickType_t xLastWakeUpTime;
		xLastWakeUpTime = xTaskGetTickCount();
    while (1)
    {
			tof++;
			TOF_Data_Update();
			vTaskDelayUntil(&xLastWakeUpTime,20);

    }

}
//void TOF_UART_IRQHandler(UART_HandleTypeDef *huart)
//{
////	BaseType_t xHigherPriorityTaskToWaken = pdFALSE; //在后面的通知中通知某种消息打开Jetson任务
////	uint8_t usart_this_time_rx_len = 0;              //此次接收长度
////	DMA_HandleTypeDef *hdma_uart_rx = huart->hdmarx; //这个句柄指向了串口把接收到的东西放到那

////	if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE) != RESET) //判断串口进入了某种中断
////	{
////			//clear the idle pending flag 清除空闲挂起标识
////			(void)huart->Instance->SR;
////			(void)huart->Instance->DR;

////			__HAL_UART_CLEAR_IDLEFLAG(huart);
////			__HAL_DMA_DISABLE(hdma_uart_rx); //关闭dma中断


////			__HAL_DMA_SET_COUNTER(hdma_uart_rx, 19); //为什么 作用是清空dma空间？
////			__HAL_DMA_ENABLE(hdma_uart_rx);

////			vTaskNotifyGiveFromISR(TaskJetsonComm_Handle, &xHigherPriorityTaskToWaken);
////			portYIELD_FROM_ISR(xHigherPriorityTaskToWaken); //为什么 这个函数干什么的

////			}
//}

void TOF_Data_Update()
{
	//把TOF返回的ASCII转化为INT
    for(int i = 9;i <= 12;i++)
    {
				height_data_temp[i-9] = CharToInt(TOFBuffer[i]);
    }
    //哈哈，这个写法是挺憨的，不想费心去搞个循环了，万一哪里出了错就很没必要，再说这样也挺直观的，
	  //这就是在把heightheight_data_temp[4]里的数据给变成一个十进制数
    float height_temp = (float)(height_data_temp[0] * pow(16,3) + height_data_temp[1] * pow(16,2) \
    + height_data_temp[2] * pow(16,1) + height_data_temp[0] * pow(16,0));
		
    Gimbal.position.Height = height_temp / 1000;
}
