#include "Task_RC.h"

void Task_RC(void *parameters)
{
	RC_InitConfig();
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		RC_Data_Update();
		xTaskNotifyGive(TaskStateMachine_Handle);
	}
}

/**
  * @brief  遥控初始化配置
  * @param  none
  * @retval void
  * @note   
  */
void RC_InitConfig(void)
{
  RC_ReceiveData.comm_status.FrameCounter = 0; //帧率计数
  RC_Rx_Mem = MEMORYRESET;
	
  RC_ReceiveData.ch0 = RC_CH_VALUE_OFFSET;
  RC_ReceiveData.ch1 = RC_CH_VALUE_OFFSET;
  RC_ReceiveData.ch2 = RC_CH_VALUE_OFFSET;
  RC_ReceiveData.ch3 = RC_CH_VALUE_OFFSET;

  RC_ReceiveData.Switch_Left = RC_SW_UP;
  RC_ReceiveData.Switch_Right = RC_SW_UP; 

  RC_ReceiveData.mouse.x = MOUSE_SPEED_OFFSET;
  RC_ReceiveData.mouse.y = MOUSE_SPEED_OFFSET;
  RC_ReceiveData.mouse.z = MOUSE_SPEED_OFFSET;

  RC_ReceiveData.mouse.press_left = MOUSE_PRESSED_OFFSET;
  RC_ReceiveData.mouse.press_right = MOUSE_PRESSED_OFFSET;
  RC_ReceiveData.key_board.key_code = KEY_OFFSET;
  
  LastRC_ReceiveData = RC_ReceiveData;
}


void RC_Data_Update(void)
{
  
  LastRC_ReceiveData = RC_ReceiveData;
  
  if (RC_Rx_Mem == MEMORY0)
  {
    RC_ReceiveData.ch0 = ((RCBuffer[0][0] | (RCBuffer[0][1] << 8)) & 0x07ff);        //!< Channel 0
    RC_ReceiveData.ch1 = (((RCBuffer[0][1] >> 3) | (RCBuffer[0][2] << 5)) & 0x07ff); //!< Channel 1
    RC_ReceiveData.ch2 = (((RCBuffer[0][2] >> 6) | (RCBuffer[0][3] << 2) | (RCBuffer[0][4] << 10)) & 0x07ff);
    RC_ReceiveData.ch3 = (((RCBuffer[0][4] >> 1) | (RCBuffer[0][5] << 7)) & 0x07ff); //!< Channel 3

    RC_ReceiveData.Switch_Left = ((RCBuffer[0][5] >> 4) & 0x000C) >> 2; //!< Switch left
    RC_ReceiveData.Switch_Right = ((RCBuffer[0][5] >> 4) & 0x0003);     //!< Switch right

    RC_ReceiveData.mouse.x = RCBuffer[0][6] | (RCBuffer[0][7] << 8);   //!< Mouse X axis
    RC_ReceiveData.mouse.y = RCBuffer[0][8] | (RCBuffer[0][9] << 8);   //!< Mouse Y axis
    RC_ReceiveData.mouse.z = RCBuffer[0][10] | (RCBuffer[0][11] << 8); //!< Mouse Z axis

    RC_ReceiveData.mouse.press_left = RCBuffer[0][12];                            //!< Mouse Left Is Press ?
    RC_ReceiveData.mouse.press_right = RCBuffer[0][13];                           //!< Mouse Right Is Press ?
    RC_ReceiveData.key_board.key_code = RCBuffer[0][14] | (RCBuffer[0][15] << 8); //!< KeyBoard value
  }
  else if (RC_Rx_Mem == MEMORY1)
  {
    RC_ReceiveData.ch0 = (RCBuffer[1][0] | (RCBuffer[1][1] << 8)) & 0x07ff;        //!< Channel 0
    RC_ReceiveData.ch1 = ((RCBuffer[1][1] >> 3) | (RCBuffer[1][2] << 5)) & 0x07ff; //!< Channel 1
    RC_ReceiveData.ch2 = ((RCBuffer[1][2] >> 6) | (RCBuffer[1][3] << 2) | (RCBuffer[1][4] << 10)) & 0x07ff;
    RC_ReceiveData.ch3 = ((RCBuffer[1][4] >> 1) | (RCBuffer[1][5] << 7)) & 0x07ff; //!< Channel 3

    RC_ReceiveData.Switch_Left = ((RCBuffer[1][5] >> 4) & 0x000C) >> 2; //!< Switch left
    RC_ReceiveData.Switch_Right = ((RCBuffer[1][5] >> 4) & 0x0003);     //!< Switch right

    RC_ReceiveData.mouse.x = RCBuffer[1][6] | (RCBuffer[1][7] << 8);   //!< Mouse X axis
    RC_ReceiveData.mouse.y = RCBuffer[1][8] | (RCBuffer[1][9] << 8);   //!< Mouse Y axis
    RC_ReceiveData.mouse.z = RCBuffer[1][10] | (RCBuffer[1][11] << 8); //!< Mouse Z axis

    RC_ReceiveData.mouse.press_left = RCBuffer[1][12];                            //!< Mouse Left Is Press ?
    RC_ReceiveData.mouse.press_right = RCBuffer[1][13];                           //!< Mouse Right Is Press ?
    RC_ReceiveData.key_board.key_code = RCBuffer[1][14] | (RCBuffer[1][15] << 8); //!< KeyBoard value
  }
  else 
  {
    // RC_ReceiveData.ch0 = RC_CH_VALUE_OFFSET; //!< Channel 0
    // RC_ReceiveData.ch1 = RC_CH_VALUE_OFFSET; //!< Channel 1
    // RC_ReceiveData.ch2 = RC_CH_VALUE_OFFSET;
    // RC_ReceiveData.ch3 = RC_CH_VALUE_OFFSET; //!< Channel 3

    // RC_ReceiveData.Switch_Left = RC_SW_MID;  //!< Switch left
    // RC_ReceiveData.Switch_Right = RC_SW_MID; //!< Switch right

    // RC_ReceiveData.mouse.x = MOUSE_SPEED_OFFSET; //!< Mouse X axis
    // RC_ReceiveData.mouse.y = MOUSE_SPEED_OFFSET; //!< Mouse Y axis
    // RC_ReceiveData.mouse.z = MOUSE_SPEED_OFFSET; //!< Mouse Z axis

    // RC_ReceiveData.mouse.press_left = MOUSE_PRESSED_OFFSET;  //!< Mouse Left Is Press ?
    // RC_ReceiveData.mouse.press_right = MOUSE_PRESSED_OFFSET; //!< Mouse Right Is Press ?
    // RC_ReceiveData.key_board.key_code = KEY_OFFSET;          //!< KeyBoard value

    return;
  }
//帧率计数
  RC_ReceiveData.comm_status.FrameCounter++; // 14ms 一次数据
}

/**
  * @brief  遥控串口中断配置
  * @param  UART_HandleTypeDef *huart 遥控对应的串口
  * @retval void
  * @note
  */
void RC_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	static uint8_t this_time_rx_len = 0;
  if (__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_IDLE) != RESET)
  {
    //clear the idle pending flag
    (void)USART1->SR;
    (void)USART1->DR;

    __HAL_UART_CLEAR_IDLEFLAG(huart);

    __HAL_DMA_DISABLE(&hdma_usart1_rx);
    this_time_rx_len = (RC_FRAME_LEN+RC_FRAME_LEN_BACK) - __HAL_DMA_GET_COUNTER(&hdma_usart1_rx);

    if ((hdma_usart1_rx.Instance->CR & DMA_SxCR_CT) != RESET)
    {
      /* Current memory buffer used is Memory 1 */
      hdma_usart1_rx.Instance->CR &= (uint32_t)(~DMA_SxCR_CT);
      RC_Rx_Mem = MEMORY1;
    }
    else
    {
      /* Current memory buffer used is Memory 0 */
      hdma_usart1_rx.Instance->CR |= (uint32_t)DMA_SxCR_CT;
      RC_Rx_Mem = MEMORY0;
    }

    if (this_time_rx_len != RC_FRAME_LEN)  RC_Rx_Mem = MEMORYRESET;
		
    __HAL_DMA_SET_COUNTER(&hdma_usart1_rx, (RC_FRAME_LEN+RC_FRAME_LEN_BACK));

    __HAL_DMA_ENABLE(&hdma_usart1_rx);

		//发送任务通知给遥控任务
    vTaskNotifyGiveFromISR(TaskRC_Handle,&xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

/**
  * @brief  获取通道值
  * @param  通道号  RC_CH0 RC_CH1 RC_CH2 RC_CH3 
  * @retval 通道数值  (相对零点)  -660~660
  */
int16_t Get_Channel_Val(RCDecoding_type *RC_ReceiveData,uint8_t channel_num)
{
  switch (channel_num)
  {
  case RC_CH0:
    if(abs(((*RC_ReceiveData).ch0 - RC_CH_VALUE_OFFSET)) < 10)
      return 0;
    else
      return ((*RC_ReceiveData).ch0 - RC_CH_VALUE_OFFSET);
  case RC_CH1:
    if(abs(((*RC_ReceiveData).ch1 - RC_CH_VALUE_OFFSET)) < 10)
      return 0;
    else
      return ((*RC_ReceiveData).ch1 - RC_CH_VALUE_OFFSET);
  case RC_CH2:
    if(abs(((*RC_ReceiveData).ch2 - RC_CH_VALUE_OFFSET)) < 10)
      return 0;
    else
      return ((*RC_ReceiveData).ch2 - RC_CH_VALUE_OFFSET);
  case RC_CH3:
    if(abs(((*RC_ReceiveData).ch3 - RC_CH_VALUE_OFFSET)) < 10)
      return 0;
    else
      return ((*RC_ReceiveData).ch3 - RC_CH_VALUE_OFFSET);
  default:
    return RC_CH_VALUE_OFFSET;
  }
}
/**
  * @brief  获取开关值
  * @param  RC_SW_Right  RC_SW_Left
  * @retval RC_SW_UP  RC_SW_MID   RC_SW_DOWN
  */
uint8_t Get_Switch_Val(RCDecoding_type *RC_ReceiveData,uint8_t switch_num)
{
  switch (switch_num)
  {
  case RC_SW_Right:
    return (*RC_ReceiveData).Switch_Right;
  case RC_SW_Left:
    return (*RC_ReceiveData).Switch_Left;
  default:
    return RC_SW_MID;
  }
}
/**
  * @brief  获取鼠标平移速度
  * @param  MOUSE_X MOUSE_Y MOUSE_Z
  * @retval 平移速度
  */
int16_t Get_Mouse_Speed(RCDecoding_type *RC_ReceiveData,uint8_t xyz)
{
  switch (xyz)
  {
  case MOUSE_X:
    return (*RC_ReceiveData).mouse.x;
  case MOUSE_Y:
    return (*RC_ReceiveData).mouse.y;
  case MOUSE_Z:
    return (*RC_ReceiveData).mouse.z;
  default:
    return MOUSE_SPEED_OFFSET;
  }
}
/**
  * @brief  获取鼠标键值
  * @param  MOUSE_LEFT  MOUSE_RIGHT
  * @retval 键值
  */
uint8_t Get_Mouse_Pressed(RCDecoding_type *RC_ReceiveData,uint8_t button)
{
  switch (button)
  {
  case MOUSE_LEFT:
    return (*RC_ReceiveData).mouse.press_left;
  case MOUSE_RIGHT:
    return (*RC_ReceiveData).mouse.press_right;
  default:
    return MOUSE_PRESSED_OFFSET;
  }
}
/**
  * @brief  获取键盘键值
  * @param  需要获取的按键
  * @retval 是否按下 1 or 0
  */
uint8_t Get_Keyboard_Val(RCDecoding_type *RC_ReceiveData,uint8_t key)
{
  switch (key)
  {
  case KEY_W:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_W)&&1);
  case KEY_S:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_S)&&1);
  case KEY_A:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_A)&&1);
  case KEY_D:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_D)&&1);  
	case KEY_SHIFT:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_SHIFT)&&1);
  case KEY_CTRL:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_CTRL)&&1); 
	case KEY_Q:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_Q)&&1);
  case KEY_E:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_E)&&1);	
	case KEY_R:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_R)&&1);
	case KEY_F:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_F)&&1);
  case KEY_G:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_G)&&1);
  case KEY_Z:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_Z)&&1);
	case KEY_X:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_X)&&1);
	case KEY_C:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_C)&&1);
	case KEY_V:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_V)&&1);
	case KEY_B:
    return (((*RC_ReceiveData).key_board.key_code & KEY_PRESSED_OFFSET_B)&&1);
  default:
    return KEY_OFFSET;
  }
}
