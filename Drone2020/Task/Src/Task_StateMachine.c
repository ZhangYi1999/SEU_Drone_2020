#include "Task_StateMachine.h"
#include "Task_JetsonComm.h"
#include "Task_RC.h"
#include "Task_Flash.h"
void Task_StateMachine(void *parameters)
{
	StatusMachine_Init();
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
		StatusMachine_Update();
	}
}

/**
  * @brief  状态机初始化
  * @param  void
  * @retval void
  */
void StatusMachine_Init(void)
{
	Gimbal.control_mode = ControlMode_Protect;
	Gimbal.fric_status = FricStatus_Stop;
	Gimbal.stir_status = StirStatus_Stop;
	Gimbal.shoot_status = ShootStatus_Stop;
	Gimbal.aim_mode = AimMode_Protect;
}

void StatusMachine_Update(void)
{
    switch (Get_Switch_Val(&RC_ReceiveData, RC_SW_Right))
    {
			case RC_SW_MID:
			{
				Gimbal.control_mode = ControlMode_Manual;
				break;
			}
			case RC_SW_DOWN:
			{
				Gimbal.control_mode = ControlMode_PC;
				break;
			}
			default:
			{
				Gimbal.control_mode = ControlMode_Protect;
				break;
			}
    }
	
	/*按键切换变量，使得每个按键动作执行一次*/
	static uint8_t FricHighChange = 1;
	static uint8_t AutoShootChange = 1;
	static uint8_t HaveWildChange = 0;
	switch (Gimbal.control_mode)
  {
		case ControlMode_Manual:
		{
			if (Get_Switch_Val(&RC_ReceiveData, RC_SW_Left) == RC_SW_UP)
			{
				if(FricHighChange)
				{
					FricHighChange = 0;
					
					if(Gimbal.shoot_status == ShootStatus_Stop)
						Gimbal.shoot_status = ShootStatus_SingleRound;
					else if(Gimbal.shoot_status == ShootStatus_SingleRound)
						Gimbal.shoot_status = ShootStatus_MultiRound;
					else
						Gimbal.shoot_status = ShootStatus_Stop;
					
					if(Gimbal.shoot_status != ShootStatus_Stop)
						Gimbal.fric_status = FricStatus_Working;
					else
						Gimbal.fric_status = FricStatus_Stop;				
				}
			}
			else if(Get_Switch_Val(&RC_ReceiveData, RC_SW_Left) == RC_SW_DOWN)
			{
				if(Gimbal.fric_status != FricStatus_Stop)
					Gimbal.stir_status = StirStatus_Working;
				else
					Gimbal.stir_status = StirStatus_Stop;				
			}
			else//回中间位时置一
			{
				FricHighChange = 1;
				Gimbal.stir_status = StirStatus_Stop;
			}
			break;
		} 
		case ControlMode_PC:
		{
			if (Get_Switch_Val(&RC_ReceiveData, RC_SW_Left) == RC_SW_DOWN)
			{
			    /*************************************开关摩擦轮******************************************/
				if(KEY_FRICHIGH)
				{
					if(FricHighChange)
					{
						FricHighChange = 0;
						if(Gimbal.shoot_status != ShootStatus_Stop)
							Gimbal.fric_status = FricStatus_Working;
						else
							Gimbal.fric_status = FricStatus_Stop;
					}
				}
				else
					FricHighChange = 1;/*按键抬起后才能在进行下一次判断*/

			    /*************************************开拨盘*******************************************/
				if (KEY_SHOOT)
				{
					Gimbal.stir_status = StirStatus_Working;
				}
				else
					Gimbal.stir_status = StirStatus_Stop;

			    /*************************************自瞄模式切换************************************/
				//有一说一,这段代码我挺迷的,我不知道是不是凯哥写的有问题,反正我觉得有点问题,还需要找别人商讨一下
				//原代码如下:

				/*
				if (!KEY_AUTO_SHOOT)
					AutoShootChange = 1;
				if (KEY_AUTO_SHOOT)	
				{
					ShootStatus = AutoShootMode;
					while (AutoShootChange)
					{
						Pitch_Desire = Pitch.TargetAngle;
						Yaw_Desire = Yaw.TargetAngle;
						Version_Init();
						AutoShootChange--;
					}
				}
				else if (ShootStatus == AutoShootMode)
					ShootStatus = ManualMode;
				*/

				/*-------------------------------自瞄模式切换-----------------------------------*/
				if (!KEY_AUTO_SHOOT)						/*判断鼠标右键松开*/
				{
					AutoShootChange = 1;
					Gimbal.aim_mode = AimMode_Manual;		/*松开右键，进入手瞄*/
				}
					
				if (KEY_AUTO_SHOOT)							/*按下鼠标右键，进入自瞄*/
				{
					Gimbal.aim_mode = AimMode_Auto;			/*鼠标右键时永远只有自瞄*/
					while (AutoShootChange)					/*按下右键时只进入一次卡尔曼初始化*/
					{
						Version_Init();						/*卡尔曼滤波初始化，防止爆炸*/
						AutoShootChange = 0;				/*只进入一次*/
					}
					if(KEY_WILD_CHANGE)//必须要在按着shift键的情况下才能打开微调模式,担心会误触
					{
						HaveWildChange = 1;
						if(KEY_WILD_CHANGE_LEFT) Wild_Change_Angle_Yaw.FLOAT -= (float)0.5;
						else if(KEY_WILD_CHANGE_RIGHT) Wild_Change_Angle_Yaw.FLOAT += (float)0.5;
						else if(KEY_WILD_CHANGE_UP) Wild_Change_Angle_Pitch.FLOAT -= (float)0.5;
						else if(KEY_WILD_CHANGE_DOWN) Wild_Change_Angle_Pitch.FLOAT += (float)0.5;
					}
					else//此时没有按着shift
					{
						if(HaveWildChange)//此时已经微调完了
						{
							HaveWildChange = 0;
							FlashWrite();//Flash写入条件:首先是要没有按着Shift,其次是要松开了Shift,这个时候 HaveWildChange 置1,
										//才可以写入,写入后接着置0,防止下次循环的时候再次写入
						}
					}
				}
				/*-------------------------------打前哨站切换-----------------------------------*/
				if (KEY_SHOOT_OUTPOST && !KEY_AUTO_SHOOT)   /*此时打前哨站键按下,但是不能处于自瞄状态*/
				{
					Gimbal.aim_mode = AimMode_Outpost;		/*按x键,枪口立即指向前哨站方向,Yaw机头方向,Pitch-19°*/
				}
				/*-------------------------------打基地切换-----------------------------------*/
				if (KEY_SHOOT_BASE && !KEY_AUTO_SHOOT)      /*此时打基地键按下,但是不能处于自瞄状态*/
				{
					Gimbal.aim_mode = AimMode_Base;		    /*按c键,枪口立即指向基地方向,Yaw+36°,Pitch水平*/
				}

			}
			break;
		}
		
		case ControlMode_Protect:
		{
			Gimbal.fric_status = FricStatus_Stop;
			Gimbal.shoot_status = ShootStatus_Stop;
			break;
		}
  }
}
