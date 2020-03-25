#include "System.h"

void PositionInit(void);
void Personal_GYRO_Receive(GYRO_type *gyro);
void Gimbal_PositionSet(void);
float CharsToFloat(uint8_t* s);
float LowFilter(float last_value,float current_value);
float AverageFilter(float current_value);

