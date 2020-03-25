#include "System.h"

extern float test_speed;
extern float test_angle;
extern int16_t test_output;

void Gimbal_Init(void);
void Gimbal_SetPosition(void);
void Gimbal_Control(void);
void Gimbal_Protect(void);
int Gimbal_StartMove(void);

