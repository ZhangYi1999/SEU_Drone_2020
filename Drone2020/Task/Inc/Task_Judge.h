#ifndef _TASK_JUDGE_H__
#define _TASK_JUDGE_H__

#include "System.h"

/*--------------偏移位置----------------*/
//接收数据
#define JUDGE_SOF_OFFSET (0)
#define JUDGE_DATALENGTH_OFFSET (1)
#define JUDGE_SEQ_OFFSET (3)
#define JUDGE_CRC8_OFFSET (4)
#define JUDGE_CMDID_OFFSET (5)
#define JUDGE_DATA_OFFSET (7)
#define JUDGE_CRC16_OFFSET(n) (n + JUDGE_DATA_OFFSET)
//发送数据
#define TRAMSINIT_LENGTH 128
#define TRAMSINIT_HEAD_OFFSET 0
#define TRAMSINIT_SENDID_OFFSET 2
#define TRAMSINIT_CLIENT_OFFSET 4

//CmdID(2-Byte)
#define GAME_STATE              0X0001          //比赛状态数据，1Hz 周期发送
#define GAME_RESULT             0X0002          //比赛结果数据，比赛结束后发送
#define ROBOT_HP                0X0003          //比赛机器人存活数据，1Hz 周期发送
#define DART_STATUS             0X0004          //飞镖发射状态，飞镖发射时发送 
#define EVENT_DATA              0X0101          //场地事件数据，事件改变后发送
#define SUPPLY_ACTION           0X0102          //场地补给站动作标识数据，动作改变后发送
#define REFEREE_WARNING         0X0104          //裁判警告数据，警告发生后发送 
#define DART_REMAINING_TIME     0X0105          //飞镖发射口倒计时，1Hz 周期发送 
#define GAME_ROBOT_STATE        0X0201          //机器人状态数据，10Hz 周期发送
#define POWER_HEAT_DATA         0X0202          //实时功率热量数据，50Hz 周期发送
#define GAME_ROBOT_POS          0X0203          //机器人位置数据，10Hz 发送
#define BUFF_MUSK               0X0204          //机器人增益数据，增益状态改变后发送
#define AERIAL_ROBOT_ENERGY     0X0205          //空中机器人能量状态数据，10Hz 周期发送，只有空中机器人主控发送
#define ROBOT_HURT              0X0206          //伤害状态数据，伤害发生后发送
#define SHOOT_DATA              0X0207          //实时射击数据，子弹发射后发送
#define REMAIN_BULLET           0X0208          //弹丸剩余发射数，仅空中机器人，哨兵机器人以及 ICRA 机器人发送，1Hz 周期发送
#define RFID_STATE              0X0209          //机器人 RFID 状态，1Hz 周期发送 
#define INTERACTIVE_HEADER      0X0301          //交互数据接收信息，发送频率：上限 10Hz
#define CLIENTID                0xD180          //向客户端发送ID

//DataSize
#define GAME_STATE_DATA_SIZE            (3)
#define GAME_RESULT_DATA_SIZE           (1)
#define ROBOT_HP_DATA_SIZE              (32)
#define DART_STATUS_DATE_SIZE           (3)
#define EVENTDATA_DATA_SIZE             (4)
#define SUPPLY_ACTION_DATA_SIZE         (4)
#define REFEREE_WARNING_DATE_SIZE       (2)
#define DART_REMAINING_TIME_DATE_SIZE   (1)
#define GAMEROBOT_STATE_DATA_SIZE       (18)
#define POWER_HEAT_DATA_SIZE            (16)
#define GAME_ROBOT_POS_DATA_SIZE        (16)
#define BUFF_MUSK_DATA_SIZE             (1)
#define AERIAL_ROBOT_ENERGY_DATA_SIZE   (3)
#define ROBOT_HURT_DATA_SIZE            (1)
#define SHOOTDATA_DATA_SIZE             (6)
#define REMAIN_BULLET_DATA_SIZE         (2)
#define RFID_STATE_DATA_SIZE            (4)
#define INTERACTIVEHEADER_DATA_SIZE(n) (n + 9)
#define JUDGE_DATA_LENGTH(n) (n + 9)

/*--------------------------------------------------校验函数--------------------------------------------------*/
void RefereeReceive(uint8_t JudgeReceive_Counter);
void Referee_Receive_Data_Processing(uint8_t SOF, uint16_t CmdID);

#endif
