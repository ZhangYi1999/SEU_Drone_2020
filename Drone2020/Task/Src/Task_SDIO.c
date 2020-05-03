#include "Task_SDIO.h"

//�ο�΢ѩ����http://www.waveshare.net/study/article-657-1.html
/*------------------SD��д---��������------------------*/

FATFS SDFatFs;  /* �ļ�ϵͳ��������File system object for SD card logical drive */
FIL MyFile;     /* �ļ�����ṹ��ָ�룬File object */
//char SDPath[4]; /* SD card logical drive path */

FRESULT res;                                          /* ����SD��غ������ú󷵻صĽ����FatFs function common result code */
uint32_t byteswritten, bytesread;                     /* ��д���ֽ�����File write/read counts */
char filename[] = "DroneData.txt";										/* SD������ļ��� */
char finalname[17] = "Game";													/* ���������ļ���������������ʽΪ��GameXXXXLineXXXX�������ڼ���������һ���ж���������*/
char gamenum[4] = {0};
char linenum[4] = {0};
int game_num = 0;
int line_num = 0;

//char interval[3] = "   ";											 	 	  /* ��SD��д������ʱ���ڷָ�һ��д��Ĳ�ͬ����*/
char newline[8] = "      \r\n";												/* ��SD��д������ʱ���ڷָ�����д������ݣ�����FATFs��֧��д�뻻�з�����������һ����˵*/
char ctime[] = {'[', '0', '0', ':', '0', '0', ':', '0', '0', ':', '0', ']', ' ', ' ', ' '};  //���ڼ�¼ÿ��д�����ݵ�ʱ�䣬������ո񣬸�ʽΪ����Сʱ�����ӣ����ӣ��ٺ��롿   ��
char cpos_x[18] = {0};
char cpos_y[18] = {0};
char cpos_z[18] = {0};	//���˻�xyz����
char cheight[18] = {0};	//TOF��ø߶�
char cpitch[18] = {0};	//pitch�Ƕ�
char cyaw[18] = {0};		//yaw�Ƕ�
char cfric[18] = {0};		//Ħ����ת��
char cspeed[18] = {0};	//�ӵ����ٶ�

//���±������ڶ�SD����д���ܽ��д��ڵ��ԣ�����������ʱ���ò���
char* debug_chars;																		/* �����ã������� */
char wtext[] = "Testing STM32 working with FatFs";		/* �����ã�д���棬File write buffer */
char rtext[100];                                  	  /* �����ã������棬File read buffer */

/*------------------SD��д---��������------------------*/

void Task_SDIO(void *parameters)
{
	f_mount(&SDFatFs, "", 0);//���ļ�ϵͳ����ע�ᵽFatFsģ��
	
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime, 100/portTICK_RATE_MS);
		
		if(GameEnd == 0)//����δ����
		{
			DtatPrepareSD();
			
			f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);//�������򿪾���д����Ȩ�޵����ı��ļ�����
			
			f_write(&MyFile, ctime, 15, (void *)&byteswritten);//���ı��ļ�д��ʱ��
			f_write(&MyFile, cpos_x, 18, (void *)&byteswritten);
			f_write(&MyFile, cpos_y, 18, (void *)&byteswritten);
			f_write(&MyFile, cpos_z, 18, (void *)&byteswritten);//���ı��ļ�д�����˻�����
			f_write(&MyFile, cheight, 18, (void *)&byteswritten);//���ı��ļ�д�����˻��߶�
			f_write(&MyFile, cpitch, 18, (void *)&byteswritten);//���ı��ļ�д��pitch�Ƕ�
			f_write(&MyFile, cyaw, 18, (void *)&byteswritten);//���ı��ļ�д��yaw�Ƕ�
			f_write(&MyFile, cfric, 18, (void *)&byteswritten);//���ı��ļ�д��Ħ����ת��
			f_write(&MyFile, cspeed, 18, (void *)&byteswritten);//���ı��ļ�д���ӵ����ٶ�
			f_write(&MyFile, newline, 8, (void *)&byteswritten);//����
			
			f_close(&MyFile);//�ر��ı��ļ�
			
			line_num++;//������������
		}
		else//�����������޸��ļ���
		{
			Int_to_Char(gamenum, game_num, 4);
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
			finalname[16] = '\0';
			
			f_rename(filename, finalname);//���ļ�������
		}
	}
}

/**
  * @brief  ����׼����Ҫд��SD������
  * @param  None
  * @retval None
  * @note	  None
  */
void DtatPrepareSD(void)
{
	Float_to_Char(cpos_x, ext_game_robot_pos.x, 8, 5);
	AppendBlank(cpos_x, 15, 3);
	
	Float_to_Char(cpos_y, ext_game_robot_pos.y, 8, 5);
	AppendBlank(cpos_y, 15, 3);
	
	Float_to_Char(cpos_z, ext_game_robot_pos.z, 8, 5);
	AppendBlank(cpos_z, 15, 3);
	
	Float_to_Char(cheight, Gimbal.position.Height, 8, 5);
	AppendBlank(cheight, 15, 3);
	
	Float_to_Char(cpitch, Gimbal.position.PitchAngle, 8, 5);
	AppendBlank(cpitch, 15, 3);
	
	Float_to_Char(cyaw, Gimbal.position.YawAngle, 8, 5);
	AppendBlank(cyaw, 15, 3);
	
	Float_to_Char(cfric,FricMotor_Left.RealSpeed, 8, 5);
	AppendBlank(cfric, 15, 3);
	
	Float_to_Char(cspeed, ext_shoot_data.bullet_speed, 8, 5);
	AppendBlank(cspeed, 15, 3);
}

/**
  * @brief  �������ַ�������ӿո������������
  * @param  str��   	Ҫд����ַ���
  * @param  pre_len�� �ӿո�֮ǰ�ַ����ж೤�����ַ������ݼ��Ͻ�β��\0���ĳ��ȣ�
  * @param  blank�� 	Ҫ�ӵĿո���
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
  * @brief  ���ݶ�ʱ��TIM5����д��ĳ�����ݵ�ʱ��
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

/*���º������ڶ�SD����д���ܽ��д��ڵ��ԣ����õ�ʱ��ע�͵�*/

/*
void Task_SDIO(void *parameters)
{
	//##-1- ���ļ�ϵͳ����ע�ᵽFatFsģ�飬Register the file system object to the FatFs module ##############
	res = f_mount(&SDFatFs, "", 0);
	if(res)			//��������ʱ��������ֵFR_OK = 0,��res��Ϊ0��˵�������쳣
	{
			Error_Handler();
	}
	else
	{
			debug_chars = "Mount success\r\n";
			HAL_UART_Transmit(&huart7, (uint8_t*)debug_chars ,15,100);
	}
	 
	//##-2- �������򿪾���д����Ȩ�޵����ı��ļ�����Create and Open new text file objects with write access ######
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
	 
	//##-3- ���ı��ļ�д�����ݣ�Write data to the text files ###############################
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
	 
	//##-4- �ر��ı��ļ���Close the open text files ################################
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
	 
	//##-5- �򿪾��ж�����Ȩ�޵��ı��ļ�����Open the text files object with read access ##############
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
	 
	//##-6- ���ı��ļ��������ݣ�Read data from the text files ##########################
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
	 
	//##-7- �ر��ı��ļ���Close the open text files ############################
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
