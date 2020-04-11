#ifndef __TASK_JETSONCOMM_H
#define __TASK_JETSONCOMM_H

#include "System.h"

/*阵营选择*/
#define WeAreRedTeam     !HAL_GPIO_ReadPin(CampInput_GPIO_Port,CampInput_Pin)      /*红方阵营选择*/
#define WeAreBlueTeam    HAL_GPIO_ReadPin(CampInput_GPIO_Port,CampInput_Pin)       /*蓝方阵营选择*/

void JetsonCommUart_Config(UART_HandleTypeDef *huart);
void JetsonCommUart_ReConfig_In_IRQHandler(UART_HandleTypeDef *huart);
void JetsonComm_Control(UART_HandleTypeDef *huart);

void kalman_filter_init(kalman_filter_t *F, kalman_filter_init_t *I);

float *amended_kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2, float signal3);
float *kalman_filter_calc(kalman_filter_t *F, float signal1, float signal2, float signal3);

void KF_Init(void);
void KF_Cal_Desire(void);
extern void Version_Init(void);


#endif
