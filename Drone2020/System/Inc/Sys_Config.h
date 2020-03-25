#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "Sys_Def.h"

#define CAN_ONE_USED
#define DMA_RC_USED
#define DMA_Judge_USED
#define DMA_JetsonTX2_USED
#define DMA_GYRO_USED

#define ONBOARDIMU_USED


#define FricMotor_MaximumOutput 								16000
#define FricMotor_SpeedPID_Kp   								8//8
#define FricMotor_SpeedPID_Ki   								0.1//0.1
#define FricMotor_SpeedPID_Kd   								0
#define FricMotor_SpeedPID_DeadBand   					0
#define FricMotor_SpeedPID_Maximum_Integration  10000
#define FricMotor_AnglePID_Kp   								0
#define FricMotor_AnglePID_Ki   								0
#define FricMotor_AnglePID_Kd   								0
#define FricMotor_AnglePID_DeadBand   					0
#define FricMotor_AnglePID_Maximum_Integration  0
#define FricMotor_WorkingSpeed									7520

#define StirMotor_MaximumOutput 								10000
#define StirMotor_SpeedPID_Kp   								8
#define StirMotor_SpeedPID_Ki   								0.02
#define StirMotor_SpeedPID_Kd   								0
#define StirMotor_SpeedPID_DeadBand   					0
#define StirMotor_SpeedPID_Maximum_Integration  5000
#define StirMotor_AnglePID_Kp   								5
#define StirMotor_AnglePID_Ki   								0.3
#define StirMotor_AnglePID_Kd   								0
#define StirMotor_AnglePID_DeadBand   					0
#define StirMotor_AnglePID_Maximum_Integration  10000
#define StirMotor_WorkingSpeed									-5500
#define StirMotor_WorkingAngle									60

#define StirMotor_Reduction_Ratio								36
#define PitchMotor_MaximumOutput 								29000
#define PitchMotor_SpeedPID_Kp   								130//95
#define PitchMotor_SpeedPID_Ki   								0.3//0.5
#define PitchMotor_SpeedPID_Kd   								0
#define PitchMotor_SpeedPID_DeadBand   					0
#define PitchMotor_SpeedPID_Maximum_Integration 5000
#define PitchMotor_AnglePID_Kp   								27//27
#define PitchMotor_AnglePID_Ki   								0
#define PitchMotor_AnglePID_Kd   								0.5//0.5
#define PitchMotor_AnglePID_DeadBand   					0
#define PitchMotor_AnglePID_Maximum_Integration 10000
#define PitchAngle_Uplimit											15
#define PitchAngle_Downlimit										-60


#define YawMotor_MaximumOutput 									20000
#define YawMotor_SpeedPID_Kp   									140//140
#define YawMotor_SpeedPID_Ki   									0.4//0.04
#define YawMotor_SpeedPID_Kd   									0
#define YawMotor_SpeedPID_DeadBand   						0
#define YawMotor_SpeedPID_Maximum_Integration  	5000
#define YawMotor_AnglePID_Kp   									30//30
#define YawMotor_AnglePID_Ki   									0
#define YawMotor_AnglePID_Kd   									0.1//0.1
#define YawMotor_AnglePID_DeadBand   						0
#define YawMotor_AnglePID_Maximum_Integration  	10000




#define PitchAngle_StepLength              			0.8
#define YawAngle_StepLength              				0.5

#define RC_CH_MAX_RELATIVE 660.0f

#define RC_FRAME_LEN        18U
#define RC_FRAME_LEN_BACK   7U 

#define REFEREE_DMA_SIZE 130

#define PersonalGYRO_rx_len 79
#define PersonalGYROFrameLength 50 

#define GYROFRAMEHEAD0 0X5A
#define GYROFRAMEHEAD1 0XA5


/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
#define RC_CH0    ((uint8_t)0)
#define RC_CH1    ((uint8_t)1)
#define RC_CH2    ((uint8_t)2)
#define RC_CH3    ((uint8_t)3)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP    ((uint16_t)1)
#define RC_SW_MID   ((uint16_t)3)
#define RC_SW_DOWN  ((uint16_t)2)
#define RC_SW_Right ((uint8_t)0)
#define RC_SW_Left  ((uint8_t)1)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)0x01<<8)
#define KEY_PRESSED_OFFSET_F ((uint16_t)0x01<<9)
#define KEY_PRESSED_OFFSET_G ((uint16_t)0x01<<10)
#define KEY_PRESSED_OFFSET_Z ((uint16_t)0x01<<11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)0x01<<12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)0x01<<13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)0x01<<14)
#define KEY_PRESSED_OFFSET_B ((uint16_t)0x01<<15)


#define KEY_W         ((uint8_t)0)
#define KEY_S         ((uint8_t)1)
#define KEY_A         ((uint8_t)2)
#define KEY_D         ((uint8_t)3)
#define KEY_SHIFT     ((uint8_t)4)
#define KEY_CTRL      ((uint8_t)5)
#define KEY_Q         ((uint8_t)6)
#define KEY_E         ((uint8_t)7)
#define KEY_R         ((uint8_t)8)
#define KEY_F         ((uint8_t)9)
#define KEY_G         ((uint8_t)10)
#define KEY_Z         ((uint8_t)11)
#define KEY_X         ((uint8_t)12)
#define KEY_C         ((uint8_t)13)
#define KEY_V         ((uint8_t)14)
#define KEY_B         ((uint8_t)15)
#define KEY_OFFSET    ((uint8_t)0)
/* ----------------------- PC Mouse Definition-------------------------------- */
#define MOUSE_X                 ((uint8_t)0)
#define MOUSE_Y                 ((uint8_t)1)
#define MOUSE_Z                 ((uint8_t)2)
#define MOUSE_SPEED_OFFSET      ((uint16_t)0)
#define MOUSE_LEFT              ((uint8_t)3)
#define MOUSE_RIGHT             ((uint8_t)4)
#define MOUSE_PRESSED_OFFSET    ((uint8_t)0)


#define JetsonCommReservedFrameLEN 5

#define JETSONFLAG_LEN 16

//֡ͷ֡β
#define JetsonCommSOF 0x66
#define JetsonCommEOF 0x88
#define CommSetUp (uint16_t)0xCCCC
#define RecordAngle (uint16_t)0xffff
#define RequestTrans (uint16_t)0xbbbb
//҈ɼ۬6׽
#define BlueTeam (uint16_t)0xDDDD
#define RedTeam (uint16_t)0xEEEE
//עʤ׽ʽ
#define NoFire (uint16_t)(0x00 << 8)     //һעʤ
#define SingleFire (uint16_t)(0x01 << 8) //֣ʤ
#define BurstFire (uint16_t)(0x02 << 8)  //lע
//עʤ̙׈èٟ̙֍̙é
#define HighBulletSpeed (uint16_t)(0x01)
#define LowBulletSpeed (uint16_t)(0x02)
//̹Ѩ࠘׆ģʽ
#define ManualMode (uint8_t)(0x00)     //˖֯࠘׆
#define SmallBuffMode (uint8_t)(0x01)  //Сػģʽ
#define BigBuffModeCCW (uint8_t)(0x02) //ճػģʽŦʱ֫
#define BigBuffModeCW (uint8_t)(0x03)  //ճػģʽ˳ʱ֫
#define AutoShootMode (uint8_t)(0x04)  //ؔ֯ʤܷ

#define RotatinPatrol (uint8_t)(0x01) 
#define PatrolArmor0 (uint8_t)(0x02)  
#define PatrolArmor1 (uint8_t)(0x03)  
#define ServoMode (uint8_t)(0x04)     

#endif
