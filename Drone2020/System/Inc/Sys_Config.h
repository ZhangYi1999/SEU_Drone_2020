#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "Sys_Def.h"

#define CAN_ONE_USED
#define DMA_RC_USED
#define DMA_Judge_USED
#define DMA_JetsonTX2_USED
#define DMA_GYRO_USED
#define ONBOARDIMU_USED
#define DMA_TOF_USED

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

#define TOF_DMA_SIZE 19

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

/*--------------------------- 与Flash关宏定义开始------------------------------------------*/
/* Private define ------------------------------------------------------------*/
//我们只使用flash的最后一片区域以防止与代码区有冲突
#define FLASH_USER_START_ADDR   ADDR_FLASH_SECTOR_23   /* Start @ of user Flash area */
#define FLASH_USER_END_ADDR     ADDR_FLASH_SECTOR_23  +  GetSectorSize(ADDR_FLASH_SECTOR_23) -1 /* End @ of user Flash area : sector start address + sector size -1 */

/* Base address of the Flash sectors Bank 1 */
#define ADDR_FLASH_SECTOR_0     ((uint32_t)0x08000000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_1     ((uint32_t)0x08004000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_2     ((uint32_t)0x08008000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_3     ((uint32_t)0x0800C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_4     ((uint32_t)0x08010000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_5     ((uint32_t)0x08020000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_6     ((uint32_t)0x08040000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_7     ((uint32_t)0x08060000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_8     ((uint32_t)0x08080000) /* Base @ of Sector 8, 128 Kbytes */
#define ADDR_FLASH_SECTOR_9     ((uint32_t)0x080A0000) /* Base @ of Sector 9, 128 Kbytes */
#define ADDR_FLASH_SECTOR_10    ((uint32_t)0x080C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_11    ((uint32_t)0x080E0000) /* Base @ of Sector 11, 128 Kbytes */

/* Base address of the Flash sectors Bank 2 */
#define ADDR_FLASH_SECTOR_12     ((uint32_t)0x08100000) /* Base @ of Sector 0, 16 Kbytes */
#define ADDR_FLASH_SECTOR_13     ((uint32_t)0x08104000) /* Base @ of Sector 1, 16 Kbytes */
#define ADDR_FLASH_SECTOR_14     ((uint32_t)0x08108000) /* Base @ of Sector 2, 16 Kbytes */
#define ADDR_FLASH_SECTOR_15     ((uint32_t)0x0810C000) /* Base @ of Sector 3, 16 Kbytes */
#define ADDR_FLASH_SECTOR_16     ((uint32_t)0x08110000) /* Base @ of Sector 4, 64 Kbytes */
#define ADDR_FLASH_SECTOR_17     ((uint32_t)0x08120000) /* Base @ of Sector 5, 128 Kbytes */
#define ADDR_FLASH_SECTOR_18     ((uint32_t)0x08140000) /* Base @ of Sector 6, 128 Kbytes */
#define ADDR_FLASH_SECTOR_19     ((uint32_t)0x08160000) /* Base @ of Sector 7, 128 Kbytes */
#define ADDR_FLASH_SECTOR_20     ((uint32_t)0x08180000) /* Base @ of Sector 8, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_21     ((uint32_t)0x081A0000) /* Base @ of Sector 9, 128 Kbytes  */
#define ADDR_FLASH_SECTOR_22     ((uint32_t)0x081C0000) /* Base @ of Sector 10, 128 Kbytes */
#define ADDR_FLASH_SECTOR_23     ((uint32_t)0x081E0000) /* Base @ of Sector 11, 128 Kbytes */
/*--------------------------- 与Flash关宏定义结束------------------------------------------*/

#endif
