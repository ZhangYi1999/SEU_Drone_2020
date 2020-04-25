#include "Task_SDIO.h"

//�ο�΢ѩ����http://www.waveshare.net/study/article-657-1.html
/*------------------SD��д---��������------------------*/

FATFS SDFatFs;  /* �ļ�ϵͳ��������File system object for SD card logical drive */
FIL MyFile;     /* �ļ�����ṹ��ָ�룬File object */
//char SDPath[4]; /* SD card logical drive path */

FRESULT res;                                          /* �����ã�����SD��غ������ú󷵻صĽ����FatFs function common result code */
uint32_t byteswritten, bytesread;                     /* ��д���ֽ�����File write/read counts */
uint8_t wtext[] = "Testing STM32 working with FatFs"; /* �����ã�д���棬File write buffer */
uint8_t rtext[100];                                   /* �����ã������棬File read buffer */
char filename[] = "DroneData.txt";										/* SD������ļ��� */
uint8_t interval[3] = "   ";											 	  /* ��SD��д������ʱ���ڷָ�һ��д��Ĳ�ͬ����*/
uint8_t newline[8] = "      \r\n";										/* ��SD��д������ʱ���ڷָ�����д������ݣ�����FATFs��֧��д�뻻�з�����������һ����˵*/

/*------------------SD��д---��������------------------*/

void Task_SDIO(void *parameters)
{
	f_mount(&SDFatFs, "", 0);//���ļ�ϵͳ����ע�ᵽFatFsģ��
	
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,2);//����ϵͳ��10Hz�������˻�λ����Ϣ���˴����λ����Ϣ�Ƿ����
		
		if(pos_update == 1)//λ����Ϣ�Ѹ���
		{
			f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);//�������򿪾���д����Ȩ�޵����ı��ļ�����
			
			f_write(&MyFile, &robot_pos[0], 4, (void *)&byteswritten);//���ı��ļ�д�����˻�����
			f_write(&MyFile, interval, 3, (void *)&byteswritten);
			f_write(&MyFile, &robot_pos[1], 4, (void *)&byteswritten);
			f_write(&MyFile, interval, 3, (void *)&byteswritten);
			f_write(&MyFile, &robot_pos[2], 4, (void *)&byteswritten);
			f_write(&MyFile, newline, 8, (void *)&byteswritten);
			
			f_close(&MyFile);//�ر��ı��ļ�
		}
	}
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
			printf(" mount sucess!!! \r\n");//���ڵ���ʱ�������״̬
	 
	//##-2- �������򿪾���д����Ȩ�޵����ı��ļ�����Create and Open new text file objects with write access ######
	res = f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
	if(res)
			printf(" open file error : %d\r\n",res);
	else
			printf(" open file sucess!!! \r\n");
	 
	//##-3- ���ı��ļ�д�����ݣ�Write data to the text files ###############################
	res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
	if(res)
			printf(" write file error : %d\r\n",res);
	else
	{
			printf(" write file sucess!!! \r\n");
			printf(" write Data : %s\r\n",wtext);
	}
	 
	//##-4- �ر��ı��ļ���Close the open text files ################################
	res = f_close(&MyFile);
	if(res)
			printf(" close error : %d\r\n",res);
	else
			printf(" close sucess!!! \r\n");
	 
	//##-5- �򿪾��ж�����Ȩ�޵��ı��ļ�����Open the text files object with read access ##############
	res = f_open(&MyFile, filename, FA_READ);
	if(res)
			printf(" open file error : %d\r\n",res);
	else
			printf(" open file sucess!!! \r\n");
	 
	//##-6- ���ı��ļ��������ݣ�Read data from the text files ##########################
	res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
	if(res)
			printf(" read error!!! %d\r\n",res);
	else
	{
			printf(" read sucess!!! \r\n");
			printf(" read Data : %s\r\n",rtext);
	}
	 
	//##-7- �ر��ı��ļ���Close the open text files ############################
	res = f_close(&MyFile);
	if(res)  
			printf(" close error!!! %d\r\n",res);
	else
			printf(" close sucess!!! \r\n");
	 
	//##-8- Compare read data with the expected data ############
	if(bytesread == byteswritten)
	{ 
			printf(" FatFs is working well!!!\r\n");
	}
}

static void Error_Handler(void)
{
  printf("something wrong ....\r\n");
     //User may add here some code to deal with this error 
  while(1)
  {
  }
}
*/
