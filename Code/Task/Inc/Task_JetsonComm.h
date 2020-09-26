#ifndef __TASK_JETSONCOMM_H__
#define __TASK_JETSONCOMM_H__

#include "Sys_Def.h"

#define JetsonCommReservedFrameLEN 5
//帧头帧尾
#define JetsonCommSOF 0x66 //头帧
#define JetsonCommEOF 0x88 //尾帧
#define JETSONFLAG_LEN 16

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

//矩阵相关
#define mat_init arm_mat_init_f32
#define mat_add arm_mat_add_f32
#define mat_sub arm_mat_sub_f32
#define mat_mult arm_mat_mult_f32
#define mat_trans arm_mat_trans_f32
#define mat_inv arm_mat_inverse_f32
#define mat arm_matrix_instance_f32

/*-------------------------------- 与算法通信所用到的结构体定义开始-------------------------------- */
typedef struct
{
    uint8_t SoF;
    uint8_t Seq;
    uint16_t ShootMode;     //高八位发射方式 低八位发射速度等级  (0xFFFF-记录当前角度  0xEEEE-红方  0xDDDD-蓝方  0xCCCC-通信建立  0xBBBB-请求数据发送)
    float Target_Angle_Pitch; //Pitch目标角度
    float Target_Angle_Yaw;   //Yaw目标角度
    /*  哨兵专用   */
    int16_t TargetSpeedOnRail; //目标轨道速度（哨兵用）
    uint8_t SentryGimbalMode;  //哨兵云台攻击模式
    uint8_t EoF;
} JetsonToSTM_Struct;

typedef struct
{
    /*记录读图时的角度*/
    float CurAngle_Pitch;
    float CurAngle_Yaw;
    /*此次角速度*/
    float Speed_Pitch;
    float Speed_Yaw;
    /*是否记录过角度*/
    uint8_t ChangeAngle_flag;
} JetsonFlag_Struct;

typedef struct
{
    uint8_t SoF;
    uint8_t Seq;
    uint8_t NeedMode;   //所需控制模式
    uint8_t ShootSpeed; //射速
    /*  哨兵专用   */
    uint8_t RailNum;      //所处轨道标号
    uint8_t ArmorType;    //被打击装甲板标识
    uint16_t RemainHP;    //剩余血量
    uint8_t Reserved[11]; //保留字节
    uint8_t EoF;
} STMToJetson_Struct;

typedef struct
{
    uint8_t SoF;
    uint8_t Seq;
    float Gimbal_Pitch;
    float Gimbal_Yaw;
    uint8_t EoF;
} STMToJetson_Struct_Gyro;

typedef struct
{
    uint16_t team;
    uint8_t CommSuccess;
} CommStatus_Struct;

typedef struct
{
    float raw_value;
    float filtered_value[2];
    mat xhat;
    mat xhatminus;
    mat z;
    mat A;
    mat H;
    mat AT;
    mat HT;
    mat Q;
    mat R;
    mat P;
    mat Pminus;
    mat K;
    mat B; //
    mat u; //
} kalman_filter_t;

typedef struct
{
    float raw_value;
    float filtered_value[2];
    float xhat_data[2], xhatminus_data[2], z_data[2], Pminus_data[4], K_data[4];
    float P_data[4];
    float AT_data[4], HT_data[4];
    float A_data[4];
    float H_data[4];
    float Q_data[4];
    float R_data[4];
    float B_data[2]; //
    float *u_data;   //
} kalman_filter_init_t;
/*-------------------------------- 与算法通信所用到的结构体定义结束-------------------------------- */

/*-------------------------------- 与算法的通信所需外部变量开始-------------------------------- */

extern float Desire_Angle_Pitch, Desire_Angle_Yaw;  /*Pitch轴与Yaw轴目标角*/
extern float Jetson_Angle_Pitch;			        /*Pitch轴角度变量*/
extern float Jetson_Angle_Yaw;                      /*Yaw轴角度变量*/
extern float Jetson_Speed_Pitch;                    /*Pitch轴角速度变量*/
extern float Jetson_Speed_Yaw;                      /*Yaw轴角速度变量*/
extern float Jetson_Acceleration_Pitch;             /*Pitch轴角加速度变量*/
extern float Jetson_Acceleration_Yaw;               /*Yaw轴角加速度变量*/
extern float Pre_Speed_Pitch;                       /*Pitch轴上次角速度变量*/
extern float Pre_Speed_Yaw;                         /*Yaw轴上次角度变量*/
/*卡尔曼滤波变量设置*/
extern kalman_filter_t KF_Gimbal_Pitch, KF_Gimbal_Yaw;
extern kalman_filter_init_t KF_Gimbal_Pitch_init, KF_Gimbal_Yaw_init;

extern JetsonFlag_Struct JetsonFlag[JETSONFLAG_LEN];                        /*16个结构体用来克服延迟，用于记录数据*/
/*凯哥说,JetsonFlag的的作用如下:
算法计算并返回给我们的是当前角度与目标角度的差值,即Target_Angle
同时因为算法对图像的处理是有延迟的,当JetSon处理完图像之后,可能这时的云台已经指向另一个角度了
那如果还用JetSon结算出来的差值角度,显然是不对的,所以我们要将图像被处理时所对应的角度保存起来
等JetSon结算完成后,再加上当时记录的角度,就得到了最终想要的角度,再赋值给Desire_Angle*/
extern uint8_t JetsonBuffer[100];
extern JetsonToSTM_Struct DataRecFromJetson_Temp, DataRecFromJetson;        /*两个变量克服某些可能覆盖的错误*/
extern STMToJetson_Struct DataSendToJetson;
extern STMToJetson_Struct_Gyro DataSendToJetson_gyro;
extern CommStatus_Struct CommStatus;
/*-------------------------------- 与算法的通信所需外部变量结束-------------------------------- */

void JetsonCommUart_Config(UART_HandleTypeDef *huart);
void JetsonComm_Control(UART_HandleTypeDef *huart);

void kalman_filter_init(kalman_filter_t *F, kalman_filter_init_t *I);
float *amended_kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2, float signal3);
float *kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2, float signal3);

void KF_Init(void);
void KF_Cal_Desire(void);
extern void Version_Init(void);

#endif
