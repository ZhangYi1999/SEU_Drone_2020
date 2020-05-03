#ifndef _SYS_DEF_H_
#define _SYS_DEF_H_

#include "Sys_Config.h"
#include "stm32f4xx_hal.h"

#include <arm_math.h>

/*-------------------------------- 枚举变量定义开始-------------------------------- */
typedef enum
{
	offline=0,
	online
}Link_Status;

typedef enum
{
	Can1=0,
	Can2
}Can_Channel;

typedef enum
{
	GM6020=0,
	GM3508,
	GM2006,
	Snail
}Motor_Model;

typedef enum
{
	Can=0,
	PWM
}Motor_CommMethod;

typedef enum
{
	single_loop=0,
	double_loop,
	open_loop
}Motor_ControlMethod;

typedef enum{
	ButtonPressedOnce = 0,
	ButtonTaskFinish
}UserButtonStatus_Enum;

typedef enum
{
	FricStatus_Stop = 0,
	FricStatus_Working
}FricStatus_Enum;

typedef enum
{
	StirStatus_Stop=0,
	StirStatus_Working
}StirStatus_Enum;

typedef enum
{
	ShootStatus_Stop=0,
	ShootStatus_SingleRound,
	ShootStatus_MultiRound,
}ShootStatus_Enum;

typedef enum
{
	ControlMode_Protect=0,            
	ControlMode_Manual,
	ControlMode_PC,
}ControlMode_Enum;

typedef enum
{
	AimMode_Protect=0,            
	AimMode_Manual,
	AimMode_Auto,
    AimMode_Outpost,
    AimMode_Base,
}AimMode_Enum;

typedef enum
{
	HI219=0,
}GYRO_Model;
/*-------------------------------- 枚举变量定义结束-------------------------------- */

/*-------------------------------- 基本变量结构体定义开始-------------------------------- */
typedef struct
{
	Link_Status link_status;
	uint16_t FrameCounter;
}Comm_Status;

typedef struct
{
	float Kp;
	float Ki;
	float Kd;
	float Current_Error;
	float Last_Error;
	float Sum_Integration;
	float Maximum_Integration;
	float Dead_Band;
	float Output;
}PID_type;

typedef struct
{
	GYRO_Model model;
	Comm_Status comm_status;
	uint32_t comm_id;
	float PitchAngle;
	float YawAngle;
	float RollAngle;
	float Gyro_X;
	float Gyro_Y;
	float Gyro_Z;
} GYRO_type;

typedef struct
{
	float PitchAngle;
	float YawAngle;
	float RollAngle;
	float PitchSpeed;
	float YawSpeed;
	float RollSpeed;
    float x;
    float y;
    float z;
    float Height;
}Gimbal_Position;

typedef struct
{
	Gimbal_Position position;
	FricStatus_Enum fric_status;
	StirStatus_Enum stir_status;
	ShootStatus_Enum shoot_status;
	ControlMode_Enum control_mode;
    AimMode_Enum aim_mode;
}Gimbal_type;

typedef struct
{
	Can_Channel CANx;
	CAN_TxHeaderTypeDef SendCanHeader;
	uint8_t SendCanTxMsg[8];
}CanSend_type;

typedef struct
{
	Motor_Model model;
	Motor_CommMethod communication;
	Motor_ControlMethod control;
	Comm_Status comm_status;
	int comm_id;
	
	float RealSpeed;
	float TargetSpeed;
	float RealAngle;
	float TargetAngle;
	uint16_t Last_MechanicalAngle;
	uint16_t MechanicalAngle;
	int16_t	RotationalSpeed;
	int16_t RealTorque;
	int16_t RealCurrent;
	
	int16_t NeedOutput;
	int16_t MaximumOutput;
	PID_type SpeedPID;
	PID_type AnglePID;
}Motor_type;

typedef struct
{ 
  Comm_Status comm_status;       

  uint16_t ch0;
  uint16_t ch1;
  uint16_t ch2;
  uint16_t ch3;
  uint8_t  Switch_Left;
  uint8_t  Switch_Right;

  struct
  {
    int16_t x;
    int16_t y;
    int16_t z;
    uint8_t press_left;
    uint8_t press_right;
  }mouse;

  struct
  {
/**********************************************************************************
   *  15   14   13   12   11   10   9   8   7   6     5     4   3   2   1
   *  V    C    X	 Z    G    F    R   E   Q  CTRL  SHIFT  D   A   S   W
***********************************************************************************/
    uint16_t key_code;
  }key_board;

}RCDecoding_type;


/*-------------------------------- 基本变量结构体定义结束-------------------------------- */

/*-------------------------------- 基本变量外部声明开始-------------------------------- */
extern Motor_type PitchMotor;
extern Motor_type YawMotor;
extern Motor_type FricMotor_Left;
extern Motor_type FricMotor_Right;
extern Motor_type StirMotor;

extern RCDecoding_type RC_ReceiveData,LastRC_ReceiveData;
extern uint8_t RCBuffer[2][RC_FRAME_LEN+RC_FRAME_LEN_BACK];
extern uint8_t RC_Rx_Mem;

extern CanSend_type CAN_Tx_Msg;

extern Gimbal_type Gimbal;

extern GYRO_type PersonalGYRO,PersonalGYRO2;
extern GYRO_type GYRO;
extern uint8_t GYROBuffer[2][PersonalGYRO_rx_len],GYROBuffer2[2][PersonalGYRO_rx_len];
extern uint8_t GYRO_Rx_Mem,GYRO_Rx_Mem2;

extern uint8_t TOFBuffer[TOF_DMA_SIZE];
extern uint32_t height_data_temp[4];
/*-------------------------------- 基本变量外部声明结束-------------------------------- */

/*-------------------------------- 与裁判系统相关枚举定义开始-------------------------------- */
typedef enum
{
  OPERATE_NULL = 0,
  OPERATE_ADD,
  OPERATE_CHANGE,
  OPERATE_DELETE,
}OPERATE_TYPE;

//图形类型，英文能看懂
typedef enum
{
  GRAPHIC_LINE = 0,
  GRAPHIC_RECT,
  GRAPHIC_CIRCLE,
  GRAPHIC_ELLIPSE,
  GRAPHIC_ARC,//圆弧，我翻译搜的，你懂就行
  GRAPHIC_FLOAT,
  GRAPHIC_INT,
  GRAPHIC_CHAR,
}GRAPHIC_TYPE;

typedef enum
{
  COLOR_TEAM = 0,
  COLOR_YELLOW,
  COLOR_GREEN,
  COLOR_ORANGE,
  COLOR_AMARANTH, //紫红色
  COLOR_PINK,
  COLOR_CYAN,
  COLOR_BLACK,
  COLOR_WHITE,
  COLOR_NUM,
}COLOR_TYPE;

/*-------------------------------- 与裁判系统相关结构体定义开始-------------------------------- */
typedef __packed struct 
{
    uint16_t data_cmd_id;
    uint16_t sender_ID;
    uint16_t receiver_ID;
}ext_student_interactive_header_data_t; //交互数据帧头

typedef __packed struct
{ 
    uint8_t* data;
}robot_interactive_data_t;//交互数据

typedef __packed struct 
{
    uint8_t graphic_name[3];
    uint32_t operate_tpye:3;
    uint32_t graphic_tpye:3;
    uint32_t layer:4;
    uint32_t color:4;
    uint32_t start_angle:9;
    uint32_t end_angle:9;
    uint32_t width:10;
    uint32_t start_x:11;
    uint32_t start_y:11;
    uint32_t radius:10;
    uint32_t end_x:11;
    uint32_t end_y:11;
}graphic_data_struct_t;

typedef __packed struct
{ 
	  graphic_data_struct_t graphic_data_struct;
    uint8_t data[30]; 
} ext_client_custom_character_t;

typedef __packed struct 
{   
  graphic_data_struct_t  grapic_data_struct; 
} ext_client_custom_graphic_single_t;

typedef __packed struct 
{ 
  graphic_data_struct_t  grapic_data_struct[2]; 
} ext_client_custom_graphic_double_t; 

typedef __packed struct 
{ 
  graphic_data_struct_t  grapic_data_struct[5];
} ext_client_custom_graphic_five_t;

typedef __packed struct 
{ 
  graphic_data_struct_t  grapic_data_struct[7]; 
} ext_client_custom_graphic_seven_t;

/*
这里是要发送数据的帧头				|帧格式
															|frame_header (5-byte) 
															|cmd_id (2-byte) 
															|data (n-byte) 
															|frame_tail (2-byte，CRC16，整包校验)

															|Frame header 格式:
															|SOF    data_length seq     CRC8 
															|1-byte 2-byte      1-byte  1-byte 
*/
typedef __packed struct 
{
	uint8_t SOF;
	__packed union 
	{
		uint16_t Frame;
		uint8_t FrameBuffer[2];
	}data_length;
	uint8_t seq;
	uint8_t CRC8;
}frame_header_t;

/*
这里是把帧头,cmd_id与数据合起来
数据长度为121
但是还没有帧尾
*/
typedef __packed struct
{
	__packed union
	{
		frame_header_t Frame;
		uint8_t FrameBuffer[5];
	}frame_header;

	__packed union
	{
		uint16_t Frame;
		uint8_t FrameBuffer[2];
	}cmd_id;

	uint8_t data[121];
}UiFrame_t;

/*
一共有帧头 5+cmdid 2+数据 121 一共长128

|SOF|data_length|seq|CRC8|
|frame_header------------|cmd_id|data|
|UiFrame_t(UiFrameBuffer[128])-------|

这是裁判系统协议联合体，可能因结构体字节对齐影响出现问题！！需后期测试
*/

//1.	比赛机器人状态(0x0001)
typedef __packed struct
{
    /*
    0-3 bit：比赛类型
    ? 1：RoboMaster 机甲大师赛；
    ? 2：RoboMaster 机甲大师单项赛；
    ? 3：ICRA RoboMaster 人工智能挑战赛
    */
    uint8_t game_type : 4;

    /*
    4-7 bit：当前比赛阶段
    ? 0：未开始比赛；
    ? 1：准备阶段；
    ? 2：自检阶段；
    ? 3：5s 倒计时；
    ? 4：对战中；
    ? 5：比赛结算中
    */
    uint8_t game_progress : 4;

    /*
    当前阶段剩余时间，单位 s
    */
    uint16_t stage_remain_time;
} ext_game_state_t;

//2.比赛结果数据：0x0002。发送频率：比赛结束后发送
typedef __packed struct
{
    /*0 平局 1 红方胜利 2 蓝方胜利*/
    uint8_t winner;
} ext_game_result_t;

//3. 机器人血量数据：0x0003。发送频率：1Hz
typedef __packed struct
{
    uint16_t red_1_robot_HP;    
    uint16_t red_2_robot_HP;    
    uint16_t red_3_robot_HP;    
    uint16_t red_4_robot_HP;    
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;    
    uint16_t red_outpost_HP;   
    uint16_t red_base_HP;    

    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;    
    uint16_t blue_3_robot_HP;    
    uint16_t blue_4_robot_HP;    
    uint16_t blue_5_robot_HP;    
    uint16_t blue_7_robot_HP;    
    uint16_t blue_outpost_HP;   
    uint16_t blue_base_HP; 
} ext_game_robot_HP_t;

//4. 飞镖发射状态：0x0004。发送频率：飞镖发射后发送，发送范围：所有机器人。 
typedef __packed struct 
{   
    uint8_t dart_belong;    
    uint16_t stage_remaining_time;  
} ext_dart_status_t; 

//6.场地事件数据：0x0101。发送频率：事件改变后发送
typedef __packed struct
{
    /*
    bit 0-1：己方停机坪占领状态
    ? 0 为无机器人占领；
    ? 1 为空中机器人已占领但未停桨；
    ? 2 为空中机器人已占领并停桨

    bit 2-3：己方能量机关状态： 
    bit 2 为小能量机关激活状态，1 为已激活； 
    bit 3 为大能量机关激活状态，1 为已激活； 

    bit 4：己方基地虚拟护盾状态 
    1 为基地有虚拟护盾血量； 
    0 为基地无虚拟护盾血量；

    bit 5 -31: 保留 
    */

    uint32_t event_type;
} ext_event_data_t;

//7. 补给站动作标识：0x0102。发送频率：动作改变后发送
typedef __packed struct
{
    /*
    补给站口 ID：
    1：1 号补给口；
    2：2 号补给口
    */
    uint8_t supply_projectile_id;

    /*
    补弹机器人 ID：0 为当前无机器人补弹，1 为红方英雄机器人补弹，2 为红方工程
    机器人补弹，3/4/5 为红方步兵机器人补弹，101 为蓝方英雄机器人补弹，102 为蓝方
    工程机器人补弹，103/104/105 为蓝方步兵机器人补弹
    */
    uint8_t supply_robot_id;

    /*
    出弹口开闭状态：0 为关闭，1 为子弹准备中，2 为子弹下落
    */
    uint8_t supply_projectile_step;

    /*
    补弹数量：
    50：50 颗子弹；
    100：100 颗子弹；
    150：150 颗子弹；
    200：200 颗子弹。
    */
    uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

//8. 裁判警告信息：cmd_id (0x0104)。发送频率：警告发生后发送，发送范围：己方机器人。 
typedef __packed struct 
{   
    uint8_t level;          // 警告等级： 
    /*犯规机器人 ID： 
    1 级以及 5 级警告时，机器人 ID 为 0 
    二三四级警告时，机器人 ID 为犯规机器人 ID  
    */
    uint8_t foul_robot_id;  
} ext_referee_warning_t; 

//9. 飞镖发射口倒计时：cmd_id (0x0105)。发送频率：1Hz 周期发送，发送范围：己方机器人。 
typedef __packed struct 
{   
    uint8_t dart_remaining_time;   //15s 倒计时 
} ext_dart_remaining_time_t; 

//10.比赛机器人状态：0x0201。发送频率：10Hz
typedef __packed struct
{
    /*
    机器人 ID：
    1：红方英雄机器人；
    2：红方工程机器人；
    3/4/5：红方步兵机器人；
    6：红方空中机器人；
    7：红方哨兵机器人；
    8：红方飞镖机器人；
    9：红方雷达站；  
    101：蓝方英雄机器人；
    102：蓝方工程机器人；
    103/104/105：蓝方步兵机器人；
    106：蓝方空中机器人；
    107：蓝方哨兵机器人。
    108：蓝方飞镖机器人； 
    109：蓝方雷达站。 
    */
    uint8_t robot_id;

    /*
    机器人等级：
    1：一级；2：二级；3：三级。
    */
    uint8_t robot_level;

    /*
    机器人剩余血量
    */
    uint16_t remain_HP;

    /*
    机器人上限血量
    */
    uint16_t max_HP;

    /*
    机器人 17mm 枪口每秒冷却值
    */
    uint16_t shooter_heat0_cooling_rate;

    /*
    机器人 17mm 枪口热量上限
    */
    uint16_t shooter_heat0_cooling_limit;

    /*
    机器人 42mm 枪口每秒冷却值
    */
    uint16_t shooter_heat1_cooling_rate;

    /*
    机器人 42mm 枪口热量上限
     */
    uint16_t shooter_heat1_cooling_limit;

    /*
    机器人 17mm 枪口上限速度 单位 m/s
    */
    uint8_t shooter_heat0_speed_limit; 

    /*
    机器人 42mm 枪口上限速度 单位 m/s
    */
    uint8_t shooter_heat1_speed_limit;   

    /*
    机器人最大底盘功率， 单位 w 
    */
    uint8_t max_chassis_power;

    /*
    主控电源输出情况：
    0 bit：gimbal 口输出： 1 为有 24V 输出，0 为无 24v 输出；
    1 bit：chassis 口输出：1 为有 24V 输出，0 为无 24v 输出；
    2 bit：shooter 口输出：1 为有 24V 输出，0 为无 24v 输出；
    */
    uint8_t mains_power_gimbal_output : 1;
    uint8_t mains_power_chassis_output : 1;
    uint8_t mains_power_shooter_output : 1;
} ext_game_robot_state_t;

//11.实时功率热量数据：0x0202。发送频率：50Hz
typedef __packed struct
{
    //底盘输出电压 单位 毫伏
    uint16_t chassis_volt;
    //底盘输出电流 单位 毫安
    uint16_t chassis_current;
    //底盘输出功率 单位 W 瓦
    float chassis_power;
    //底盘功率缓冲 单位 J 焦耳
    uint16_t chassis_power_buffer;
    //17mm 枪口热量
    uint16_t shooter_heat0;
    //42mm 枪口热量
    uint16_t shooter_heat1;
    //机动 17 mm 枪口热量 
    uint16_t mobile_shooter_heat2; 
} ext_power_heat_data_t;

//12.机器人位置：0x0203。发送频率：10Hz
typedef __packed struct
{
    float x;   //位置 x 坐标，单位 m
    float y;   //位置 y 坐标，单位 m
    float z;   //位置 z 坐标，单位 m
    float yaw; //位置枪口，单位度
} ext_game_robot_pos_t;

//13. 机器人增益：0x0204。发送频率：状态改变后发送
typedef __packed struct
{
    /*
    bit 0：机器人血量补血状态
    bit 1：枪口热量冷却加速
    bit 2：机器人防御加成
    bit 3：机器人攻击加成
    其他 bit 保留
    */
    uint8_t power_rune_buff;
} ext_buff_musk_t;

//14. 空中机器人能量状态：0x0205。发送频率：10Hz
typedef __packed struct
{
    uint8_t energy_point; //积累的能量点
    uint8_t attack_time;  //可攻击时间 单位 s。50s 递减至 0
} aerial_robot_energy_t;

//15. 伤害状态：0x0206。发送频率：伤害发生后发送
typedef __packed struct
{
    /*
    bit 0-3：当血量变化类型为装甲伤害，代表装甲 ID，其中数值为 0-4 号代表机器人
    的五个装甲片，其他血量变化类型，该变量数值为 0。
    */
    uint8_t armor_id : 4;
    /*bit 4-7：血量变化类型
    0x0 装甲伤害扣血；
    0x1 模块掉线扣血；
    0x2 超枪口热量扣血；
    0x3 超底盘功率扣血。
    */
    uint8_t hurt_type : 4;
} ext_robot_hurt_t;

//16. 实时射击信息：0x0207。发送频率：射击后发送
typedef __packed struct
{
    uint8_t bullet_type; //子弹类型: 1：17mm 弹丸 2：42mm 弹丸
    uint8_t bullet_freq; //子弹射频 单位 Hz
    float bullet_speed;  //子弹射速 单位 m/s
} ext_shoot_data_t;

//17. 子弹剩余发射数：0x0208。发送频率：1Hz 周期发送，空中机器人，哨兵机器人以及 ICRA 机器人
//主控发送，发送范围：单一机器人。 
typedef __packed struct 
{   
    uint16_t bullet_remaining_num;   
} ext_bullet_remaining_t;

//18. 机器人 RFID 状态：0x0209。发送频率：1Hz，发送范围：单一机器人。 
typedef __packed struct 
{   
    /*
    bit 0：基地增益点 RFID 状态； 
    bit 1：高地增益点 RFID 状态； 
    bit 2：能量机关激活点 RFID 状态； 
    bit 3：飞坡增益点 RFID 状态； 
    bit 4：前哨岗增益点 RFID 状态； 
    bit 5：资源岛增益点 RFID 状态； 
    bit 6：补血点增益点 RFID 状态； 
    bit 7：工程机器人补血卡 RFID 状态； 
    bit 8-25：保留 
    bit 26-31：人工智能挑战赛 F1-F6 RFID 状态；  
    RFID 状态不完全代表对应的增益或处罚状态，例如敌方已占领的高地增益点，不
    能获取对应的增益效果
    */
    uint32_t rfid_status;
} ext_rfid_status_t;
/*-------------------------------- 与裁判系统相关定义结束-------------------------------- */

/*-------------------------------- 与裁判系统相关外部变量声明开始-------------------------------- */
extern uint8_t Judge_Receive_Buffer[REFEREE_DMA_SIZE];
extern ext_student_interactive_header_data_t header_data;
extern robot_interactive_data_t robot_interactive_data;
extern ext_client_custom_character_t custom_character;
extern graphic_data_struct_t graphic_data[GRAPHIC_NUM];
extern ext_game_state_t ext_game_state;
extern ext_game_result_t ext_game_result;
extern ext_game_robot_HP_t ext_game_robot_HP;
extern ext_dart_status_t ext_dart_status;
extern ext_event_data_t ext_event_data;
extern ext_supply_projectile_action_t ext_supply_projectile_action;
extern ext_referee_warning_t ext_referee_warning;
extern ext_dart_remaining_time_t ext_dart_remaining_time;
extern ext_game_robot_state_t ext_game_robot_state;
extern ext_power_heat_data_t ext_power_heat_data;
extern ext_game_robot_pos_t ext_game_robot_pos;
extern ext_buff_musk_t ext_buff_musk;
extern aerial_robot_energy_t aerial_robot_energy;
extern ext_robot_hurt_t ext_robot_hurt;
extern ext_shoot_data_t ext_shoot_data;
extern ext_bullet_remaining_t ext_bullet_remaining;
extern ext_rfid_status_t ext_rfid_status;

extern uint32_t bullet_max;
extern uint8_t GameEnd;
extern uint16_t bullet_remaining_num;
extern uint16_t last_energy_point;
/*-------------------------------- 与裁判系统相关外部变量声明结束-------------------------------- */

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

extern float Desire_Angle_Pitch, Desire_Angle_Yaw; /*Pitch轴与Yaw轴目标角*/
extern float Jetson_Angle_Pitch;			   /*Pitch轴角度变量*/
extern float Jetson_Angle_Yaw;			   /*Yaw轴角度变量*/
extern float Jetson_Speed_Pitch;		       /*Pitch轴角速度变量*/
extern float Jetson_Speed_Yaw;			   /*Yaw轴角速度变量*/
extern float Jetson_Acceleration_Pitch;	   /*Pitch轴角加速度变量*/
extern float Jetson_Acceleration_Yaw;		   /*Yaw轴角加速度变量*/
extern float Pre_Speed_Pitch;			       /*Pitch轴上次角速度变量*/
extern float Pre_Speed_Yaw;			       /*Yaw轴上次角度变量*/
/*卡尔曼滤波变量设置*/
extern kalman_filter_t KF_Gimbal_Pitch, KF_Gimbal_Yaw;
extern kalman_filter_init_t KF_Gimbal_Pitch_init, KF_Gimbal_Yaw_init;

extern JetsonFlag_Struct JetsonFlag[JETSONFLAG_LEN]; 								/*16个结构体用来克服延迟，用于记录数据*/
																			/*凯哥说,JetsonFlag的的作用如下:
																			算法计算并返回给我们的是当前角度与目标角度的差值,即Target_Angle
																			同时因为算法对图像的处理是有延迟的,当JetSon处理完图像之后,可能这时的云台已经指向另一个角度了
																			那如果还用JetSon结算出来的差值角度,显然是不对的,所以我们要将图像被处理时所对应的角度保存起来
																			等JetSon结算完成后,再加上当时记录的角度,就得到了最终想要的角度,再赋值给Desire_Angle*/
																			
extern JetsonToSTM_Struct DataRecFromJetson_Temp, DataRecFromJetson;               /*两个变量克服某些可能覆盖的错误*/
extern STMToJetson_Struct DataSendToJetson;
extern STMToJetson_Struct_Gyro DataSendToJetson_gyro;
extern CommStatus_Struct CommStatus;
/*-------------------------------- 与算法的通信所需外部变量结束-------------------------------- */

union Wild_Change{float FLOAT;uint32_t INT;};

extern union Wild_Change Wild_Change_Angle_Pitch,Wild_Change_Angle_Yaw;

#endif
