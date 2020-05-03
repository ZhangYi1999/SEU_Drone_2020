#include "Sys_Def.h"

/*-------------------------------- 基本外部变量定义开始------------------------------------- */
Motor_type PitchMotor;
Motor_type YawMotor;
Motor_type FricMotor_Left;
Motor_type FricMotor_Right;
Motor_type StirMotor;

CanSend_type CAN_Tx_Msg;

Gimbal_type Gimbal;

//陀螺仪专用
GYRO_type PersonalGYRO,PersonalGYRO2;
GYRO_type GYRO;
uint8_t GYROBuffer[2][PersonalGYRO_rx_len],GYROBuffer2[2][PersonalGYRO_rx_len];
uint8_t GYRO_Rx_Mem,GYRO_Rx_Mem2;

//遥控专用
RCDecoding_type RC_ReceiveData,LastRC_ReceiveData;
uint8_t RCBuffer[2][RC_FRAME_LEN+RC_FRAME_LEN_BACK];
uint8_t RC_Rx_Mem;

//TOF专用
uint8_t TOFBuffer[TOF_DMA_SIZE];
uint32_t height_data_temp[4];
/*-------------------------------- 基本外部变量定义结束-------------------------------------- */

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
graphic_data_struct_t graphic_data[GRAPHIC_NUM];

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

uint16_t bullet_remaining_num;
uint32_t bullet_max = 0;//判断是否是第一次发射，用于设置弹量上限是250还是500
uint16_t last_energy_point = 0;
uint8_t GameEnd = 0;//用于判断比赛是否结束，结束SD卡写入任务
/*-------------------------------- 与裁判系统相关外部变量定义结束-------------------------------- */

/*-------------------------------- 与算法相关外部变量定义开始----------------------------------- */
float Desire_Angle_Pitch;
float Desire_Angle_Yaw;           		/*Pitch与Yaw的目标角*/
float Jetson_Angle_Pitch = 0.0;				/*Pitch轴角度变量*/
float Jetson_Angle_Yaw = 0.0;					/*Yaw轴角度变量*/
float Jetson_Speed_Pitch = 0.0;				/*Pitch轴角速度变量*/
float Jetson_Speed_Yaw = 0.0;					/*Yaw轴角速度变量*/
float Jetson_Acceleration_Pitch = 0.0;/*Pitch轴角加速度变量*/
float Jetson_Acceleration_Yaw = 0.0;	/*Yaw轴角加速度变量*/
float Pre_Speed_Pitch = 0.0;					/*Pitch轴上次角速度变量*/
float Pre_Speed_Yaw = 0.0;			    	/*Yaw轴上次角度变量*/
/*卡尔曼滤波变量设置*/
kalman_filter_t KF_Gimbal_Pitch, KF_Gimbal_Yaw;
kalman_filter_init_t KF_Gimbal_Pitch_init, KF_Gimbal_Yaw_init;

JetsonFlag_Struct JetsonFlag[JETSONFLAG_LEN]; 								/*16个结构体用来克服延迟，用于记录数据*/
/*凯哥说,JetsonFlag的的作用如下:
算法计算并返回给我们的是当前角度与目标角度的差值,即Target_Angle
同时因为算法对图像的处理是有延迟的,当JetSon处理完图像之后,可能这时的云台已经指向另一个角度了
那如果还用JetSon结算出来的差值角度,显然是不对的,所以我们要将图像被处理时所对应的角度保存起来
等JetSon结算完成后,再加上当时记录的角度,就得到了最终想要的角度,再赋值给Desire_Angle*/
																			
JetsonToSTM_Struct DataRecFromJetson_Temp, DataRecFromJetson;               /*两个变量克服某些可能覆盖的错误*/
STMToJetson_Struct DataSendToJetson = {   			                		/*发送给Jetson的裁判系统数据*/
    .Seq = 0, 																/*seq是记录的第几个变量*/
    .SoF = JetsonCommSOF,													/*sof头帧*/
    .EoF = JetsonCommEOF};													/*eof尾帧*/

STMToJetson_Struct_Gyro DataSendToJetson_gyro = { 	                        /*发送给Jetson的陀螺仪数据*/
    .Seq = 0, 																/*seq是记录的第几个变量*/
    .SoF = JetsonCommSOF,													/*soq头帧*/
    .EoF = JetsonCommEOF};													/*eof尾帧*/

CommStatus_Struct CommStatus = {										    /*通信状态指示数据*/
    .CommSuccess = 0,
    .team = 0};
/*-------------------------------- 与算法相关外部变量定义结束-------------------------------- */

union Wild_Change Wild_Change_Angle_Pitch,Wild_Change_Angle_Yaw;

