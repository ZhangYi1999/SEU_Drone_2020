#include "Sys_Config.h"

/*-------------------------------- 与裁判系统相关外部变量定义开始-------------------------------- */
uint8_t Judge_Receive_Buffer[REFEREE_DMA_SIZE];
ext_student_interactive_header_data_t header_data;
/*
typedef __packed struct 
{   
  uint16_t data_cmd_id;    
  uint16_t sender_ID;    
  uint16_t receiver_ID; 
}ext_student_interactive_header_data_t;
*/
robot_interactive_data_t robot_interactive_data;
/*
typedef __packed struct 
{ 
  uint8_t* data;
} robot_interactive_data_t;//交互数据
*/
ext_client_custom_character_t custom_character;
graphic_data_struct_t graphic_data[GRAPHIC_NUM];//需要一个数组存储图形信息，同时记录当前有多少图形要画

ext_game_state_t ext_game_state;
ext_game_result_t ext_game_result;
ext_game_robot_HP_t ext_game_robot_HP;
ext_dart_status_t ext_dart_status;
ext_event_data_t ext_event_data;
ext_supply_projectile_action_t ext_supply_projectile_action;
ext_referee_warning_t ext_referee_warning;
ext_dart_remaining_time_t ext_dart_remaining_time;
ext_game_robot_state_t ext_game_robot_state;
ext_power_heat_data_t ext_power_heat_data;
ext_game_robot_pos_t ext_game_robot_pos;
ext_buff_musk_t ext_buff_musk;
aerial_robot_energy_t aerial_robot_energy;
ext_robot_hurt_t ext_robot_hurt;
ext_shoot_data_t ext_shoot_data;
ext_bullet_remaining_t ext_bullet_remaining;
ext_rfid_status_t ext_rfid_status;

uint32_t bullet_max = 0;//判断是否是第一次发射，用于设置弹量上限是250还是500
uint16_t last_energy_point = 0;//存储上一次通信时裁判系统返回的能量点，用于判断无人机是否发射（发射后能量点会从300变为0）
/*-------------------------------- 与裁判系统相关外部变量定义结束-------------------------------- */
int judge1 = 0;
int judge2 = 0;
int judge3 = 0;
void Task_Judge(void *parameters)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		judge1++;
	}
}

/**
  * @brief  USART的空闲中断
  * @param  huart：UART的句柄
  * @retval None
  * @note	用在USART的中断服务函数里
  */
uint8_t counter = 0;
void Referee_IDLECallback(UART_HandleTypeDef *huart)
{
    counter = 0;
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
        if (counter > 0)
        {
            //接收裁判系统数据
            RefereeReceive(counter);
        }
        //重新打开DMA接收
        memset(Judge_Receive_Buffer,0,counter);//清零接收缓冲区
				counter = 0;
				HAL_UART_Receive_DMA(&huart3, (uint8_t*)Judge_Receive_Buffer, REFEREE_DMA_SIZE);
				judge3++;
    }
}

/**
 * @brief  裁判系统接收数据函数
 * @note   
 * @param  JudgeReceive_Counter: 当前接收数据长度
 * @retval None
 */
void RefereeReceive(uint8_t JudgeReceive_Counter)
{
		judge2++;
    uint8_t Judge_SOF = 0;
    uint16_t DataLength, Judge_CmdID;
    //循环扫描头帧
    while (Judge_SOF < JudgeReceive_Counter)
    {
        if (Judge_Receive_Buffer[Judge_SOF] == 0XA5)
        {
            //这一帧数据的指令ID和数据长度
            Judge_CmdID = Judge_Receive_Buffer[JUDGE_CMDID_OFFSET + Judge_SOF] | (Judge_Receive_Buffer[JUDGE_CMDID_OFFSET + 1 + Judge_SOF] << 8);
            DataLength = Judge_Receive_Buffer[JUDGE_DATALENGTH_OFFSET + Judge_SOF] | (Judge_Receive_Buffer[JUDGE_DATALENGTH_OFFSET + 1 + Judge_SOF] << 8);
            //校验一帧数据
            if (Verify_CRC_Check_Sum(Judge_Receive_Buffer + Judge_SOF, DataLength))
            {
                //数据处理，读取
                Referee_Receive_Data_Processing(Judge_SOF, Judge_CmdID);
                //跳到下一帧数据的头帧
                Judge_SOF += JUDGE_DATA_LENGTH(DataLength);
            }
            else
            {
                Judge_SOF += JUDGE_DATA_LENGTH(0);
            }
        }
        else
        {
            Judge_SOF++;
        }
    }
}
/**
  * @brief  裁判系统数据处理
  * @param  SOF：   头帧偏移量
  * @param  CmdID： 数据帧的ID
  * @retval None
  * @note	  None
  */
void Referee_Receive_Data_Processing(uint8_t SOF, uint16_t CmdID)
{
    switch (CmdID)
    {
    //1.	比赛机器人状态(0x0001)	10Hz
    case GAME_STATE:
    {
        memcpy(&ext_game_state, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAME_STATE_DATA_SIZE);		
        break;
    }
    //2.	比赛结果(0x0002)
    case GAME_RESULT:
    {
        memcpy(&ext_game_result, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAME_RESULT_DATA_SIZE);
        break;
    }
    //3.	比赛机器人血量数据（0x0003）
    case ROBOT_HP:
    {
        memcpy(&ext_game_robot_HP, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), ROBOT_HP_DATA_SIZE);
        break;
    }
    //4.飞镖发射状态(0x0004)
    case DART_STATUS:
    {
        memcpy(&ext_dart_status, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), DART_STATUS_DATE_SIZE);
        break;
    }
    //6.场地事件数据（0x0101）
    case EVENT_DATA:
    {
        memcpy(&ext_event_data, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), EVENTDATA_DATA_SIZE);
        break;
    }
    //7.场地补给站动作标识数据（0x0102）
    case SUPPLY_ACTION:
    {
        memcpy(&ext_supply_projectile_action, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), SUPPLY_ACTION_DATA_SIZE);
        break;
    }
    //8.裁判警告数据(0x0104)
    case REFEREE_WARNING:
    {
        memcpy(&ext_referee_warning, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), REFEREE_WARNING_DATE_SIZE);
        break;
    }
    //9.飞镖发射口倒计时(0x0105)
    case DART_REMAINING_TIME:
    {
        memcpy(&ext_dart_remaining_time, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), DART_REMAINING_TIME_DATE_SIZE);
        break;
    }
    //10.机器人状态数据（0x0201）
    case GAME_ROBOT_STATE:
    {
        memcpy(&ext_game_robot_state, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAMEROBOT_STATE_DATA_SIZE);
        break;
    }
    //11.实时功率热量数据(0x0202)
    case POWER_HEAT_DATA:
    {
        memcpy(&ext_power_heat_data, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), POWER_HEAT_DATA_SIZE);
        break;
    }
    //10.机器人位置数据(0x0203)10Hz
    case GAME_ROBOT_POS:
    {
        memcpy(&ext_game_robot_pos, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), GAME_ROBOT_POS_DATA_SIZE);
        break;
    }
    case BUFF_MUSK:
    {
        memcpy(&ext_buff_musk, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), BUFF_MUSK_DATA_SIZE);
        break;
    }
    case AERIAL_ROBOT_ENERGY:
    {
        last_energy_point  = aerial_robot_energy.energy_point;
			
        memcpy(&aerial_robot_energy, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), AERIAL_ROBOT_ENERGY_DATA_SIZE);
			
			  //如果开始发射，即能量从300变为0，即last_energy_point是300，当前裁判系统返回的能量不是300。如果这时候bullet_max是初始化值为0，那么就判定为第一次发射，最大弹量就是250
			  //如果开始发射，即能量从300变为0，即last_energy_point是300，当前裁判系统返回的能量不是300。如果这时候bullet_max不是初始化值0，是250，就说明被修改过一次，那么就判定为第二次发射，最大弹量就是500
			  //如果bullet_max已经是500，就说明是第三次或者之后发射，就不会进判断语句，最大弹量就继续是500
        if(bullet_max == 0 || bullet_max == 250)
        {
            if(bullet_max == 0 && last_energy_point == 300 && aerial_robot_energy.energy_point <300)//第一次发射
                bullet_max = 250;
            if(bullet_max == 250 && last_energy_point == 300 && aerial_robot_energy.energy_point <300)//第二次发射
                bullet_max = 500;
        }

        break;
    }
    case ROBOT_HURT:
    {
        memcpy(&ext_robot_hurt, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), ROBOT_HURT_DATA_SIZE);
        break;
    }
    case SHOOT_DATA:
    {
        memcpy(&ext_shoot_data, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), SHOOTDATA_DATA_SIZE);
        break;
    }
    case REMAIN_BULLET:
    {
        memcpy(&ext_bullet_remaining, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), REMAIN_BULLET_DATA_SIZE);
        break;
    }
    case RFID_STATE:
    {
        memcpy(&ext_rfid_status, (Judge_Receive_Buffer + JUDGE_DATA_OFFSET + SOF), RFID_STATE_DATA_SIZE);
        break;
    }
    }
}

