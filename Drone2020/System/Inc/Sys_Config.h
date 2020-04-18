#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "Sys_Def.h"

#define CAN_ONE_USED
#define DMA_RC_USED
#define DMA_Judge_USED
#define DMA_JetsonTX2_USED
#define DMA_GYRO_USED
#define ONBOARDIMU_USED

#define FricMotor_MaximumOutput 				16000
#define FricMotor_SpeedPID_Kp   				8//8
#define FricMotor_SpeedPID_Ki   				0.1//0.1
#define FricMotor_SpeedPID_Kd   				0
#define FricMotor_SpeedPID_DeadBand   			0
#define FricMotor_SpeedPID_Maximum_Integration  10000
#define FricMotor_AnglePID_Kp   				0
#define FricMotor_AnglePID_Ki   				0
#define FricMotor_AnglePID_Kd   				0
#define FricMotor_AnglePID_DeadBand   		    0
#define FricMotor_AnglePID_Maximum_Integration  0
#define FricMotor_WorkingSpeed					7520

#define StirMotor_MaximumOutput 				10000
#define StirMotor_SpeedPID_Kp   				8
#define StirMotor_SpeedPID_Ki   				0.02
#define StirMotor_SpeedPID_Kd   				0
#define StirMotor_SpeedPID_DeadBand   			0
#define StirMotor_SpeedPID_Maximum_Integration  5000
#define StirMotor_AnglePID_Kp   				5
#define StirMotor_AnglePID_Ki   				0.3
#define StirMotor_AnglePID_Kd   				0
#define StirMotor_AnglePID_DeadBand   		    0
#define StirMotor_AnglePID_Maximum_Integration  10000
#define StirMotor_WorkingSpeed					-5500
#define StirMotor_WorkingAngle					60

#define StirMotor_Reduction_Ratio				36
#define PitchMotor_MaximumOutput 				29000
#define PitchMotor_SpeedPID_Kp   				130//95
#define PitchMotor_SpeedPID_Ki   				0.3//0.5
#define PitchMotor_SpeedPID_Kd   				0
#define PitchMotor_SpeedPID_DeadBand   		    0
#define PitchMotor_SpeedPID_Maximum_Integration 5000
#define PitchMotor_AnglePID_Kp   				27//27
#define PitchMotor_AnglePID_Ki   	            0
#define PitchMotor_AnglePID_Kd   				0.5//0.5
#define PitchMotor_AnglePID_DeadBand   			0
#define PitchMotor_AnglePID_Maximum_Integration 10000
#define PitchAngle_Uplimit						15
#define PitchAngle_Downlimit				    -60


#define YawMotor_MaximumOutput 					20000
#define YawMotor_SpeedPID_Kp   					140//140
#define YawMotor_SpeedPID_Ki   					0.4//0.04
#define YawMotor_SpeedPID_Kd   				    0
#define YawMotor_SpeedPID_DeadBand   	        0
#define YawMotor_SpeedPID_Maximum_Integration   5000
#define YawMotor_AnglePID_Kp   					30//30
#define YawMotor_AnglePID_Ki   					0
#define YawMotor_AnglePID_Kd   				    0.1//0.1
#define YawMotor_AnglePID_DeadBand   		    0
#define YawMotor_AnglePID_Maximum_Integration   10000

/*前哨站和基地打击角度*/
#define Outpost_Angle_Pitch                     -19
#define Outpost_Angle_Yaw                       0

#define Base_Angle_Pitch                        0
#define Base_Angle_Yaw                          36
/*以上是我们主要调的参数,以下是一些名称定义*/

/*串口缓存区*/
#define MEMORYRESET 2

#define Limit(value,range) value>range?range:(value<-1*range?-1*range:value) 
#define LimitIn360(angle)	 while(angle>360)angle-=360

#define RC_FRAME_LEN        18U
#define RC_FRAME_LEN_BACK   7U 

#define REFEREE_DMA_SIZE 130

#define PersonalGYRO_rx_len 79
#define PersonalGYROFrameLength 50 

#define GYROFRAMEHEAD0 0X5A
#define GYROFRAMEHEAD1 0XA5

/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_OFFSET (uint16_t)1024//虽然只在RC.c中用到但还是放在了这里
#define RC_CH_MAX_RELATIVE  660.0f
#define RC_CH0      ((uint8_t)0)
#define RC_CH1      ((uint8_t)1)
#define RC_CH2      ((uint8_t)2)
#define RC_CH3      ((uint8_t)3)

/*--------------------------- 与裁判系统通信相关宏定义开始------------------------*/
//裁判系统的宏定义均只与自身头文件源文件有关,故不在sysconfig.h中写出

#define GRAPHIC_NUM (7)//UI画图图画数
#define LAYER_NUM   (9)//UI画图图层数

/*--------------------------- 与裁判系统通信相关宏定义结束------------------------------------------*/

/*--------------------------- 与算法通信相关宏定义开始------------------------------------------*/
//算法通信的宏定义均只与自身头文件源文件有关,故不在sysconfig.h中写出
#define JetsonCommReservedFrameLEN 5
//帧头帧尾
#define JetsonCommSOF 0x66 //头帧
#define JetsonCommEOF 0x88 //尾帧在def.c中有用到故放过来

#define JETSONFLAG_LEN 16

#define mat_init arm_mat_init_f32
#define mat_add arm_mat_add_f32
#define mat_sub arm_mat_sub_f32
#define mat_mult arm_mat_mult_f32
#define mat_trans arm_mat_trans_f32
#define mat_inv arm_mat_inverse_f32
#define mat arm_matrix_instance_f32
/*--------------------------- 与算法通信关宏定义结束------------------------------------------*/

#endif
