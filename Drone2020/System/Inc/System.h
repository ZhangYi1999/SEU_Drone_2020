#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "Sys_Def.h"
#include "Sys_Config.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include <string.h>

#include "main.h"
#include "can.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"
#include "tim.h"
#include "spi.h"
#include "freertos.h"
#include "cmsis_os.h"
#include "stm32f4xx_it.h"

extern QueueHandle_t Queue_CANSend;
extern TaskHandle_t TaskProtect_Handle;
extern TaskHandle_t TaskCanComm_Handle;
extern TaskHandle_t TaskPosition_Handle;
extern TaskHandle_t TaskJudge_Handle;
extern TaskHandle_t TaskJetsonComm_Handle;
extern TaskHandle_t TaskRC_Handle;
extern TaskHandle_t TaskStateMachine_Handle;
extern TaskHandle_t TaskGimbal_Handle;
extern TaskHandle_t TaskShoot_Handle;
extern TaskHandle_t TaskDebug_Handle;
extern TaskHandle_t TaskLED_Handle;
extern TaskHandle_t TaskUi_Handle;
extern TaskHandle_t TaskSDIO_Handle;
extern TaskHandle_t TaskTOF_Handle;

void Task_Protect(void *parameters);
void Task_CanComm(void *parameters);
void Task_Position(void *parameters);
void Task_Judge(void *parameters);
void Task_JetsonComm(void *parameters);
void Task_RC(void *parameters);
void Task_StateMachine(void *parameters);
void Task_Gimbal(void *parameters);
void Task_Shoot(void *parameters);
void Task_Debug(void *parameters);
void Task_LED(void *parameters);
void Task_Ui(void *parameters);
void Task_SDIO(void *parameters);
void Task_TOF(void *parameters);

void Motor_SetOutput(Motor_type* motor,int16_t output);
void Motor_CanSend(Motor_type* motor);
void Motor_CanSend_Gimbal(Motor_type* Pitch,Motor_type* Yaw);
void Motor_CanSend_Shoot(Motor_type* FricMotor_Left,Motor_type* FricMotor_Right,Motor_type* StirMotor);

void PID_Calculation(Motor_type* motor);

float SpliceFloat(uint8_t* s);
uint32_t CharToInt(uint8_t ch);
void Float_to_Char(char *str, float number, uint8_t bef_len, uint8_t aft_len);
void Int_to_Char(char* str, int number, int len);

uint8_t WhichTeam(void);

/*--------------------------------------------------У�麯��--------------------------------------------------*/
unsigned char Get_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength, unsigned char ucCRC8);
unsigned int Verify_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
void Append_CRC8_Check_Sum(unsigned char *pchMessage, unsigned int dwLength);
uint16_t Get_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength, uint16_t wCRC);
uint32_t Verify_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);
void Append_CRC16_Check_Sum(uint8_t *pchMessage, uint32_t dwLength);

/*-------------------------------------------CRCУ��---------------------------------------------------*/
/**
  * @brief  ����ϵͳ����У��
  * @param  __RECEIVEBUFFER__��  ���յ��Ĳ���ϵͳ����ͷ֡���ڵ�ַ
  * @param  __DATALENGTH__��     һ֡�����ڵ�������/Bytes�����ݣ�
  * @retval 1��                  У����ȷ
  * @retval 0��                  У�����
  * @note	None
  */
#define Verify_CRC_Check_Sum(__RECEIVEBUFFER__, __DATALENGTH__) (Verify_CRC8_Check_Sum(__RECEIVEBUFFER__, JUDGE_CRC8_OFFSET + 1) \
                                                            && Verify_CRC16_Check_Sum(__RECEIVEBUFFER__, __DATALENGTH__ + JUDGE_DATA_LENGTH(0)))

/**
  * @brief  ����ϵͳ���У��
  * @param  __TRANSMITBUFFER__�� ���͵�����ϵͳ��������ͷ֡���ڵ�ַ
  * @param  __DATALENGTH__��     һ֡�����ڵ�������/Bytes�����ݣ�
  * @retval None
  * @note	None
  */
#define Append_CRC_Check_Sum(__TRANSMITBUFFER__, __DATALENGTH__)                       \
do                                                                                     \
{                                                                                      \
    Append_CRC8_Check_Sum(__TRANSMITBUFFER__, JUDGE_CRC8_OFFSET + 1);                  \
    Append_CRC16_Check_Sum(__TRANSMITBUFFER__, __DATALENGTH__ + JUDGE_DATA_LENGTH(0)); \
} while (0U)


#endif
