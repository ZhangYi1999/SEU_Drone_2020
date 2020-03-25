#ifndef _SYS_DEF_H_
#define _SYS_DEF_H_

#include "Sys_Config.h"
#include "stm32f4xx_hal.h"


typedef enum
{
	offline=0,
	online
}Link_Status;

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
   * 15   14   13   12   11   10   9   8   7   6     5     4   3   2   1
   *  V    C    X	  Z    G    F    R   E   Q  CTRL  SHIFT  D   A   S   W
***********************************************************************************/
    uint16_t key_code;
  }key_board;
}RCDecoding_type;


typedef enum{
	ButtonPressedOnce = 0,
	ButtonTaskFinish
} UserButtonStatus_Enum;

typedef enum
{
	FricStatus_Stop=0,
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
	ControlMode_Auto,
}ControlMode_Enum;

typedef enum
{
	HI219=0,
}GYRO_Model;

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
}Gimbal_Position;

typedef struct
{
	Gimbal_Position position;
	FricStatus_Enum fric_status;
	StirStatus_Enum stir_status;
	ShootStatus_Enum shoot_status;
	ControlMode_Enum control_mode;
}Gimbal_type;

typedef enum
{
	Can1=0,
	Can2
}Can_Channel;

typedef struct
{
	Can_Channel CANx;
	CAN_TxHeaderTypeDef SendCanHeader;
	uint8_t SendCanTxMsg[8];
}CanSend_type;

typedef struct
{
    uint8_t SoF;
    uint8_t Seq;
    uint16_t ShootMode;     //ٟыλעʤ׽ʽ ֍ыλעʤ̙׈ֈܶ  (0xFFFF-݇¼ձǰއ׈  0xEEEE-۬׽  0xDDDD-6׽  0xCCCC-ͨхݨb  0xBBBB-ȫȳ˽ߝע̍)
    float TargetPitchAngle; //PitchĿҪއ׈
    float TargetYawAngle;   //YawĿҪއ׈
    /*  ʚҸרԃ   */
    int16_t TargetSpeedOnRail; //ĿҪڬր̙׈èʚҸԃé
    uint8_t SentryGimbalMode;  //ʚҸՆ̨٥ܷģʽ
    uint8_t EoF;
} JetsonToSTM_Struct;

typedef struct
{
    //݇¼ׁͼʱքއ׈
    float CurAngle_Pitch;
    float CurAngle_Yaw;
    //ՋՎʏһՎ߸הއ׈
    float Velocity_Pitch;
    float Velocity_Yaw;
    //ˇر݇¼ڽއ׈
    uint8_t ChangeAngle_flag;
} JetsonFlag_Struct;

typedef struct
{
    uint8_t SoF;
    uint8_t Seq;
    uint8_t NeedMode;   //̹Ѩ࠘׆ģʽ
    uint8_t ShootSpeed; //ʤ̙
    /*  ʚҸרԃ   */
    uint8_t RailNum;      //̹ԦڬրҪۅ
    uint8_t ArmorType;    //ѻղܷװݗѥҪʶ
    uint16_t RemainHP;    //ʣԠѪ
    uint8_t Reserved[11]; //ѣ´ؖޚ
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


#define MEMORYRESET 2

#define Limit(value,range) value>range?range:(value<-1*range?-1*range:value) 
#define LimitIn360(angle)	 while(angle>360)angle-=360

extern Motor_type PitchMotor;
extern Motor_type YawMotor;
extern Motor_type FricMotor_Left;
extern Motor_type FricMotor_Right;
extern Motor_type StirMotor;

extern RCDecoding_type RC_ReceiveData,LastRC_ReceiveData;
extern uint8_t RCBuffer[2][RC_FRAME_LEN+RC_FRAME_LEN_BACK];
extern uint8_t RC_Rx_Mem;

extern uint8_t Judge_Receive_Buffer[130];

extern GYRO_type GYRO;

extern CanSend_type CAN_Tx_Msg;

extern Gimbal_type Gimbal;

extern JetsonFlag_Struct JetsonFlag[JETSONFLAG_LEN];
extern uint8_t Jetson_Seq;
extern STMToJetson_Struct_Gyro DataSendToJetson_gyro;
extern JetsonToSTM_Struct DataRecFromJetson,DataRecFromJetson_Temp;
extern STMToJetson_Struct DataSendToJetson;
extern CommStatus_Struct CommStatus;
extern float Pitch_Desire, Yaw_Desire;
extern float Pitch_otcAngle,Yaw_otcAngle;
extern float Jetson_AnglePitch;
extern float Jetson_AngleYaw;
extern float Jetson_VelocityPitch;
extern float Jetson_VelocityYaw;



extern GYRO_type PersonalGYRO,PersonalGYRO2;
extern uint8_t GYROBuffer[2][PersonalGYRO_rx_len],GYROBuffer2[2][PersonalGYRO_rx_len];
extern uint8_t GYRO_Rx_Mem,GYRO_Rx_Mem2;

void PID_Calculation(Motor_type* motor);

#endif
