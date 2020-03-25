#include "System.h"

typedef union
{
	float SrcData;
	uint8_t Data[4];
}Encode_type;

extern uint8_t Head[2];
extern uint8_t Tail[2];
