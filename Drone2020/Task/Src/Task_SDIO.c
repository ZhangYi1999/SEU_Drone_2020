#include "Task_SDIO.h"

//参考微雪电子http://www.waveshare.net/study/article-657-1.html
/*------------------SD读写---变量定义------------------*/

FATFS SDFatFs;  /* 文件系统工作区，File system object for SD card logical drive */
FIL MyFile;     /* 文件对象结构的指针，File object */
//char SDPath[4]; /* SD card logical drive path */

FRESULT res;                                          /* 调试用，保存SD相关函数调用后返回的结果，FatFs function common result code */
uint32_t byteswritten, bytesread;                     /* 读写的字节数，File write/read counts */
uint8_t wtext[] = "Testing STM32 working with FatFs"; /* 调试用，写缓存，File write buffer */
uint8_t rtext[100];                                   /* 调试用，读缓存，File read buffer */
char filename[] = "DroneData.txt";										/* SD卡里的文件名 */
uint8_t interval[3] = "   ";											 	  /* 向SD卡写入数据时用于分隔一次写入的不同数据*/
uint8_t newline[8] = "      \r\n";										/* 向SD卡写入数据时用于分隔两次写入的数据，好像FATFs不支持写入换行符，姑且先试一下再说*/

/*------------------SD读写---函数定义------------------*/

void Task_SDIO(void *parameters)
{
	f_mount(&SDFatFs, "", 0);//将文件系统对象注册到FatFs模块
	
	TickType_t xLastWakeUpTime;
	xLastWakeUpTime = xTaskGetTickCount();
	
	while(1)
	{
		vTaskDelayUntil(&xLastWakeUpTime,2);//裁判系统以10Hz发送无人机位置信息，此处检查位置信息是否更新
		
		if(pos_update == 1)//位置信息已更新
		{
			f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);//创建并打开具有写访问权限的新文本文件对象
			
			f_write(&MyFile, &robot_pos[0], 4, (void *)&byteswritten);//向文本文件写入无人机坐标
			f_write(&MyFile, interval, 3, (void *)&byteswritten);
			f_write(&MyFile, &robot_pos[1], 4, (void *)&byteswritten);
			f_write(&MyFile, interval, 3, (void *)&byteswritten);
			f_write(&MyFile, &robot_pos[2], 4, (void *)&byteswritten);
			f_write(&MyFile, newline, 8, (void *)&byteswritten);
			
			f_close(&MyFile);//关闭文本文件
		}
	}
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
			printf(" mount sucess!!! \r\n");//用于调试时串口输出状态
	 
	//##-2- 创建并打开具有写访问权限的新文本文件对象，Create and Open new text file objects with write access ######
	res = f_open(&MyFile, filename, FA_CREATE_ALWAYS | FA_WRITE);
	if(res)
			printf(" open file error : %d\r\n",res);
	else
			printf(" open file sucess!!! \r\n");
	 
	//##-3- 向文本文件写入数据，Write data to the text files ###############################
	res = f_write(&MyFile, wtext, sizeof(wtext), (void *)&byteswritten);
	if(res)
			printf(" write file error : %d\r\n",res);
	else
	{
			printf(" write file sucess!!! \r\n");
			printf(" write Data : %s\r\n",wtext);
	}
	 
	//##-4- 关闭文本文件，Close the open text files ################################
	res = f_close(&MyFile);
	if(res)
			printf(" close error : %d\r\n",res);
	else
			printf(" close sucess!!! \r\n");
	 
	//##-5- 打开具有读访问权限的文本文件对象，Open the text files object with read access ##############
	res = f_open(&MyFile, filename, FA_READ);
	if(res)
			printf(" open file error : %d\r\n",res);
	else
			printf(" open file sucess!!! \r\n");
	 
	//##-6- 从文本文件读入数据，Read data from the text files ##########################
	res = f_read(&MyFile, rtext, sizeof(rtext), (UINT*)&bytesread);
	if(res)
			printf(" read error!!! %d\r\n",res);
	else
	{
			printf(" read sucess!!! \r\n");
			printf(" read Data : %s\r\n",rtext);
	}
	 
	//##-7- 关闭文本文件，Close the open text files ############################
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
