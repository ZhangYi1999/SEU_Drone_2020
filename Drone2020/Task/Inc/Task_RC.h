#include "System.h"


void RC_InitConfig(void);
void RC_Data_Update(void);
int16_t Get_Channel_Val(RCDecoding_type *RC_ReceiveData,uint8_t channel_num);
uint8_t Get_Switch_Val(RCDecoding_type *RC_ReceiveData,uint8_t switch_num);
int16_t Get_Mouse_Speed(RCDecoding_type *RC_ReceiveData,uint8_t xyz);
uint8_t Get_Mouse_Pressed(RCDecoding_type *RC_ReceiveData,uint8_t button);
uint8_t Get_Keyboard_Val(RCDecoding_type *RC_ReceiveData,uint8_t key);

