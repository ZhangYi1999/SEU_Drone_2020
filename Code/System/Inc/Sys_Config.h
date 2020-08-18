#ifndef _SYS_CONFIG_H_
#define _SYS_CONFIG_H_

#include "System.h"
//
#define CAN_ONE_USED
#define DMA_RC_USED
#define DMA_Judge_USED
#define DMA_JetsonTX2_USED
#define DMA_GYRO_USED
#define ONBOARDIMU_USED
#define DMA_TOF_USED

#define FricMotor_MaximumOutput                 16000
#define FricMotor_SpeedPID_Kp                   8//8
#define FricMotor_SpeedPID_Ki                   0.1//0.1
#define FricMotor_SpeedPID_Kd                   0
#define FricMotor_SpeedPID_DeadBand             0
#define FricMotor_SpeedPID_Maximum_Integration  10000
#define FricMotor_AnglePID_Kp                   0
#define FricMotor_AnglePID_Ki                   0
#define FricMotor_AnglePID_Kd                   0
#define FricMotor_AnglePID_DeadBand             0
#define FricMotor_AnglePID_Maximum_Integration  0
#define FricMotor_WorkingSpeed                  7520

#define StirMotor_MaximumOutput                 10000
#define StirMotor_SpeedPID_Kp                   8
#define StirMotor_SpeedPID_Ki                   0.02
#define StirMotor_SpeedPID_Kd                   0
#define StirMotor_SpeedPID_DeadBand             0
#define StirMotor_SpeedPID_Maximum_Integration  5000
#define StirMotor_AnglePID_Kp                   5
#define StirMotor_AnglePID_Ki                   0.3
#define StirMotor_AnglePID_Kd                   0
#define StirMotor_AnglePID_DeadBand             0
#define StirMotor_AnglePID_Maximum_Integration  10000
#define StirMotor_WorkingSpeed                  -5500
#define StirMotor_WorkingAngle                  60

#define StirMotor_Reduction_Ratio               36
#define PitchMotor_MaximumOutput                29000
#define PitchMotor_SpeedPID_Kp                  130//95
#define PitchMotor_SpeedPID_Ki                  0.3//0.5
#define PitchMotor_SpeedPID_Kd                  0
#define PitchMotor_SpeedPID_DeadBand            0
#define PitchMotor_SpeedPID_Maximum_Integration 5000
#define PitchMotor_AnglePID_Kp                  27//27
#define PitchMotor_AnglePID_Ki                  0
#define PitchMotor_AnglePID_Kd                  0.5//0.5
#define PitchMotor_AnglePID_DeadBand            0
#define PitchMotor_AnglePID_Maximum_Integration 10000
#define PitchAngle_Uplimit                      15
#define PitchAngle_Downlimit                    -60

#define YawMotor_MaximumOutput                  20000
#define YawMotor_SpeedPID_Kp                    140//140
#define YawMotor_SpeedPID_Ki                    0.4//0.04
#define YawMotor_SpeedPID_Kd                    0
#define YawMotor_SpeedPID_DeadBand              0
#define YawMotor_SpeedPID_Maximum_Integration   5000
#define YawMotor_AnglePID_Kp                    30//30
#define YawMotor_AnglePID_Ki                    0
#define YawMotor_AnglePID_Kd                    0.1//0.1
#define YawMotor_AnglePID_DeadBand              0
#define YawMotor_AnglePID_Maximum_Integration   10000

/*前哨站和基地打击角度*/
#define Outpost_Angle_Pitch                     -19
#define Outpost_Angle_Yaw                       0

#define Base_Angle_Pitch                        0
#define Base_Angle_Yaw                          36

#endif
