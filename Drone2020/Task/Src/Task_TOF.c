#include "Task_TOF.h"

void Task_TOF(void *parameters)
{
    while (1)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        TOF_Data_Update();
    }

}
void TOF_UART_IRQHandler(UART_HandleTypeDef *huart)
{
    BaseType_t xHigherPriorityTaskToWaken = pdFALSE;
    uint8_t counter;

    //判断空闲中断
    if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
    {
        //清除空闲中断标志位
        __HAL_UART_CLEAR_IDLEFLAG(huart);
        //关闭DMA接收
        __HAL_DMA_DISABLE(huart->hdmarx);
        //记录接收到的字节数
        counter = REFEREE_DMA_SIZE - __HAL_DMA_GET_COUNTER(huart->hdmarx);
        //有接收到数据，而不是发送完成后的空闲！
        if (counter == TOF_DMA_SIZE)
        {
            vTaskNotifyGiveFromISR(TaskTOF_Handle, &xHigherPriorityTaskToWaken);
            portYIELD_FROM_ISR(xHigherPriorityTaskToWaken); //为什么 这个函数干什么的   
        }
        //重新打开DMA接收
        __HAL_DMA_SET_COUNTER(huart->hdmarx, TOF_DMA_SIZE);
        __HAL_DMA_ENABLE(huart->hdmarx);
    }
}

void TOF_Data_Update()
{
	//把TOF返回的ASCII转化为INT
    for(int i = 9;i <= 12;i++)
    {
				height_data_temp[i-9] = CharToInt(TOFBuffer[i]);
    }
    //哈哈，这个写法是挺憨的，不想费心去搞个循环了，万一哪里出了错就很没必要，再说这样也挺直观的，
	  //这就是在把heightheight_data_temp[4]里的数据给变成一个十进制数
    float height_temp = height_data_temp[0] * pow(16,3) + height_data_temp[1] * pow(16,2) \
    + height_data_temp[2] * pow(16,1) + height_data_temp[0] * pow(16,0);
		
    Gimbal.position.Height = height_temp / 1000;
}
