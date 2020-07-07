#include "Sys_Config.h"

//TOF专用
uint8_t TOFBuffer[19];
float height_data_temp[4];

void TOF_Data_Update()
{
	//把TOF返回的ASCII转化为INT
    for(int i = 0;i <= 12;i++)
    {
			if(TOFBuffer[i] == '~')
			{
				for(int j = 0;j <= 3;j++)
				{
					if(i+9<=18) height_data_temp[j] = CharToInt(TOFBuffer[i+9]);
					else height_data_temp[j] = CharToInt(TOFBuffer[i-10]);
					i++;
				}
			}
    }
    //哈哈，这个写法是挺憨的，不想费心去搞个循环了，万一哪里出了错就很没必要，再说这样也挺直观的，
	  //这就是在把heightheight_data_temp[4]里的数据给变成一个十进制数
    float height_temp = (float)(height_data_temp[0] * pow(16,3) + height_data_temp[1] * pow(16,2) \
    + height_data_temp[2] * pow(16,1) + height_data_temp[0] * pow(16,0));
		
    Gimbal.position.Height = height_temp / 1000;
}

void TOF_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	//判断空闲中断
	if (__HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE)!= RESET)
	{
		if(__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE))
		{
			__HAL_UART_CLEAR_IDLEFLAG(huart);
			uint32_t DMA_FLAGS = __HAL_DMA_GET_TC_FLAG_INDEX(huart->hdmarx);    
			__HAL_DMA_DISABLE(huart->hdmarx);
			__HAL_DMA_CLEAR_FLAG(huart->hdmarx,DMA_FLAGS);
			__HAL_DMA_SET_COUNTER(huart->hdmarx,19);
			TOF_Data_Update();
			__HAL_DMA_ENABLE(huart->hdmarx);
			HAL_UART_Receive_DMA(&huart4,TOFBuffer,19);
		}
	}
}
