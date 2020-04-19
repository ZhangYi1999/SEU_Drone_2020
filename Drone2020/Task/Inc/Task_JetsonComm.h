#ifndef __TASK_JETSONCOMM_H
#define __TASK_JETSONCOMM_H

#include "System.h"

#define CommSetUp (uint16_t)0xCCCC //通讯建立
#define RecordAngle (uint16_t)0xffff //记录角度
#define RequestTrans (uint16_t)0xbbbb //请求发送

//比赛红蓝方
#define BlueTeam (uint16_t)0xDDDD
#define RedTeam  (uint16_t)0xEEEE

//发射方式
#define NoFire (uint16_t)(0x00 << 8)     //不发射
#define SingleFire (uint16_t)(0x01 << 8) //点射
#define BurstFire (uint16_t)(0x02 << 8)  //连发
#define TopFire (uint16_t) (0x04<<8)		 //陀螺发射

//发射速度（高速低速）
#define HighBulletSpeed (uint16_t)(0x01)
#define LowBulletSpeed (uint16_t)(0x02)
#define MidBulletSpeed (uint16_t)(0x05)

//所需控制模式
#define ManualMode (uint8_t)(0x00)     //手动控制
#define SmallBuffMode (uint8_t)(0x01)  //小符模式
#define BigBuffModeCCW (uint8_t)(0x02) //大符模式逆时针
#define BigBuffModeCW (uint8_t)(0x03)  //大符模式顺时针
#define AutoShootMode (uint8_t)(0x04)  //自动射击

//哨兵云台工作模式
#define RotatinPatrol (uint8_t)(0x01) //旋转巡逻
#define PatrolArmor0 (uint8_t)(0x02)  //巡逻装甲板0
#define PatrolArmor1 (uint8_t)(0x03)  //巡逻装甲板1
#define ServoMode (uint8_t)(0x04)     //伺服打击

void JetsonCommUart_Config(UART_HandleTypeDef *huart);
void JetsonComm_Control(UART_HandleTypeDef *huart);

void kalman_filter_init(kalman_filter_t *F, kalman_filter_init_t *I);
float *amended_kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2, float signal3);
float *kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2, float signal3);

void KF_Init(void);
void KF_Cal_Desire(void);
extern void Version_Init(void);


#endif
