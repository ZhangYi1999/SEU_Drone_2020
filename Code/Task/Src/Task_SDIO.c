#include "Sys_Config.h"

/*------------------SD读写---变量定义------------------*/

FATFS SDFatFs;  /* 文件系统工作区，File system object for SD card logical drive */
FIL MyFile;     /* 文件对象结构的指针，File object */

FRESULT res;                                          /* 保存SD相关函数调用后返回的结果，FatFs function common result code */
uint32_t byteswritten, bytesread;                     /* 读写的字节数，File write/read counts */
char filename[] = "DroneData.txt";										/* SD卡里的文件名，在建立文本文件的时候临时使用，在比赛结束时将修改文件名，见下一行 */
char finalname[21] = "Game";													/* 下面五个变量用于最后对文件进行重命名，格式为“GameXXXXLineXXXX”，即第几场比赛，一共有多少行数据*/
char gamenum[4] = {0};
char linenum[4] = {0};
int game_num = 0;
int line_num = 0;
int game_num_temp = 12;//仅测试用

//char AllData[160];//用于暂存需要写入的所有数据，这样在SD写入时只需要调用一次f_write，DMA效率更高

char newline[8] = "      \r\n";												/* 向SD卡写入数据时用于分隔两次写入的数据，好像FATFs不支持写入换行符，姑且先试一下再说*/
char ctime[15] = {'[', '0', '0', ':', '0', '0', ':', '0', '0', ':', '0', ']', ' ', ' ', ' '};  //用于记录每次写入数据的时间，后面带空格，格式为“【小时：分钟：秒钟：百毫秒】   ”
char cpos_x[18] = {0};
char cpos_y[18] = {0};
char cpos_z[18] = {0};	//无人机xyz坐标
char cheight[18] = {0};	//TOF测得高度
char cpitch[18] = {0};	//pitch角度
char cyaw[18] = {0};		//yaw角度
char cfric[18] = {0};		//摩擦轮转速
char cspeed[18] = {0};	//子弹初速度

//以下变量用于对SD卡读写功能进行串口调试，正常工作的时候用不到
//char* debug_chars;																		/* 调试用，输出语句 */
//char wtext[] = "Testing STM32 working with FatFs";		/* 调试用，写缓存，File write buffer */
//char rtext[100];                                  	  /* 调试用，读缓存，File read buffer */

int sd = 0;
void Task_SDIO(void *parameters)
{
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();

	f_mount(&SDFatFs, "", 0);//将文件系统对象注册到FatFs模块。据说在fatfs外挂一个设备的时候，第二个参数为空""就可以访问；而在多个设备时，就得指定磁盘号。没验证过。
	
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime, 500);
		//Debug_LED;//调试看该任务是否正常运行
		sd++;//调试看该任务是否正常运行
    //if(ext_game_state.game_progress != 5)//比赛未结束
		if(line_num < 50)//测试时候没有裁判系统，临时用一下
		{
			DtatPrepareSD();
			
			f_open(&MyFile, filename, FA_OPEN_ALWAYS | FA_WRITE);//创建并打开具有写访问权限的新文本文件对象
			f_lseek(&MyFile, f_size(&MyFile));										 //从上次数据的结尾继续写入新数据
			
			f_write(&MyFile, ctime, sizeof(ctime), (void *)&byteswritten);//向文本文件写入时间
//			f_write(&MyFile, cpos_x, sizeof(cpos_x), (void *)&byteswritten);//向文本文件写入无人机坐标
//			f_write(&MyFile, cpos_y, sizeof(cpos_y), (void *)&byteswritten);
//			f_write(&MyFile, cpos_z, sizeof(cpos_z), (void *)&byteswritten);
//			f_write(&MyFile, cheight, sizeof(cheight), (void *)&byteswritten);//向文本文件写入无人机高度
//			f_write(&MyFile, cpitch, sizeof(cpitch), (void *)&byteswritten);//向文本文件写入pitch角度
//			f_write(&MyFile, cyaw, sizeof(cyaw), (void *)&byteswritten);//向文本文件写入yaw角度
//			f_write(&MyFile, cfric, sizeof(cfric), (void *)&byteswritten);//向文本文件写入摩擦轮转速
//			f_write(&MyFile, cspeed, sizeof(cspeed), (void *)&byteswritten);//向文本文件写入子弹初速度
			f_write(&MyFile, newline, sizeof(newline), (void *)&byteswritten);//换行
	
			f_close(&MyFile);//关闭文本文件
			
			line_num++;//更新数据行数
		}
		else//比赛结束，修改文件名
		{
			Int_to_Char(gamenum, game_num_temp, 4);
			Int_to_Char(linenum, line_num, 4);
			
			int i;
			for(i = 0; i < 4; i ++)
			{
				finalname[4 + i] = gamenum[i];
			}
			finalname[8] = 'L';
			finalname[9] = 'i';
			finalname[10] = 'n';
			finalname[11] = 'e';
			for(i = 0; i < 4; i ++)
			{
				finalname[12 + i] = linenum[i];
			}
			finalname[16] = '.';
			finalname[17] = 't';
			finalname[18] = 'x';
			finalname[19] = 't';
			finalname[20] = '\0';
			
			f_rename(filename, finalname);//将文件重命名
		}
	}
}

/**
  * @brief  用于准备需要写入SD的数据
  * @param  None
  * @retval None
  * @note	  None
  */
void DtatPrepareSD(void)
{
//	Float_to_Char(cpos_x, ext_game_robot_pos.x, 8, 5);
//	AppendBlank(cpos_x, 15, 3);
//	
//	Float_to_Char(cpos_y, ext_game_robot_pos.y, 8, 5);
//	AppendBlank(cpos_y, 15, 3);
//	
//	Float_to_Char(cpos_z, ext_game_robot_pos.z, 8, 5);
//	AppendBlank(cpos_z, 15, 3);
//	
//	Float_to_Char(cheight, Gimbal.position.Height, 8, 5);
//	AppendBlank(cheight, 15, 3);
//	
//	Float_to_Char(cpitch, Gimbal.position.PitchAngle, 8, 5);
//	AppendBlank(cpitch, 15, 3);
//	
//	Float_to_Char(cyaw, Gimbal.position.YawAngle, 8, 5);
//	AppendBlank(cyaw, 15, 3);
//	
//	Float_to_Char(cfric,FricMotor_Left.RealSpeed, 8, 5);
//	AppendBlank(cfric, 15, 3);
//	
//	Float_to_Char(cspeed, ext_shoot_data.bullet_speed, 8, 5);
//	AppendBlank(cspeed, 15, 3);
}

/**
  * @brief  用于在字符串后面加空格便于区分数据
  * @param  str：   	要写入的字符串
  * @param  pre_len： 加空格之前字符串有多长（即字符串内容加上结尾‘\0’的长度）
  * @param  blank： 	要加的空格数
  * @retval None
  */
void AppendBlank(char *str, int pre_len, int blank)
{
    uint8_t i;
		
		for(i = 0; i < blank; i ++)
		{
			*(str + pre_len + i - 1) = ' ';
		}

    *(str + pre_len + i - 1) = '\0';
}

/**
  * @brief  因为FreeRTOS的任务可能会受到其他任务的影响而不能按照预定时间周期唤醒，所以这里根据定时器TIM5计算写入某行数据的时间。每次增加100ms，按照进位的逻辑计算
  * @param  None
  * @retval None
  * @note	  None
  */
void Update_SDtime(void)
{
	if(ctime[10] == '9')
	{
		if(ctime[8] == '9')
		{
			if(ctime[7] == '5')
			{
				if(ctime[5] == '9')
				{
					if(ctime[4] == '5')
					{
						if(ctime[2] == '9')
						{
							if(ctime[1] == '9')
							{
								ctime[1] = '0';   ctime[2] = '0';   ctime[4] = '0';   ctime[5] = '0';   ctime[7] = '0';   ctime[8] = '0';   ctime[10] = '0';
							}
							else
							{
								ctime[1]++;   ctime[2] = '0';   ctime[4] = '0';   ctime[5] = '0';   ctime[7] = '0';   ctime[8] = '0';   ctime[10] = '0';
							}
						}
						else
						{
							ctime[2]++;   ctime[4] = '0';   ctime[5] = '0';   ctime[7] = '0';   ctime[8] = '0';   ctime[10] = '0';
						}
					}
					else
					{
						ctime[4]++;   ctime[5] = '0';   ctime[7] = '0';   ctime[8] = '0';   ctime[10] = '0';
					}
				}
				else
				{
					ctime[5]++;   ctime[7] = '0';   ctime[8] = '0';   ctime[10] = '0';
				}
			}
			else
			{
				ctime[7]++;   ctime[8] = '0';   ctime[10] = '0';
			}
		}
		else
		{
			ctime[8]++;   ctime[10] = '0';
		}
	}
	else
		ctime[10]++;
}

/*以下函数用于对SD卡读写功能进行串口调试，不用的时候注释掉*/
/*
void Task_SDIO(void *parameters)
{
	//##-1- 将文件系统对象注册到FatFs模块，Register the file system object to the FatFs module ##############
	res = f_mount(&SDFatFs, "", 0);
	if(res)			//运行正常时函数返回值FR_OK = 0,若res不为0则说明出现异常
	{
			Error_Handler();
	}
	else
	{
			debug_chars = "Mount success\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,15,100);
	}
	 
	//##-2- 创建并打开具有写访问权限的新文本文件对象，Create and Open new text file objects with write access ######
	res = f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
	if(res)
	{
			debug_chars = "Open fail\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,11,100);
	}
	else
	{
			debug_chars = "Open success\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,14,100);
	}
	 
	//##-3- 向文本文件写入数据，Write data to the text files ###############################
	res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
	if(res)
	{
			debug_chars = "Write fail  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,12,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&res ,sizeof(res),100);
		  debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	else
	{
			debug_chars = "Write success  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,15,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&wtext ,sizeof(wtext),100);
			debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	 
	//##-4- 关闭文本文件，Close the open text files ################################
	res = f_close(&MyFile);
	if(res)
	{
			debug_chars = "Close fail  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,12,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&res ,sizeof(res),100);
		  debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	else
	{
			debug_chars = "Close success\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,15,100);
	}
	 
	//##-5- 打开具有读访问权限的文本文件对象，Open the text files object with read access ##############
	res = f_open(&MyFile, filename, FA_READ);
	if(res)
	{
			debug_chars = "Open fail  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,11,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&res ,sizeof(res),100);
		  debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	else
	{
			debug_chars = "Open success\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,14,100);
	}
	 
	//##-6- 从文本文件读入数据，Read data from the text files ##########################
	res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
	if(res)
	{
			debug_chars = "Read fail  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,11,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&res ,sizeof(res),100);
		  debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	else
	{
			debug_chars = "Read success  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,14,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&rtext ,sizeof(rtext),100);
			debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	 
	//##-7- 关闭文本文件，Close the open text files ############################
	res = f_close(&MyFile);
	if(res)  
	{
			debug_chars = "Close fail  ";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,12,100);
			HAL_UART_Transmit(&huart7, (uint8_t*)&res ,sizeof(res),100);
		  debug_chars = "\r\n";
		  HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,2,100);
	}
	else
	{
			debug_chars = "Close success\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,15,100);
	}
	 
	//##-8- Compare read data with the expected data ############
	if(bytesread == byteswritten)
	{
			debug_chars = "FatFs success...\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,18,100);
	}
}

static void Error_Handler(void)
{
	debug_chars = "Error...\r\n";
	HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,10,100);
     //User may add here some code to deal with this error 
  while(1)
  {
  }
}
*/
