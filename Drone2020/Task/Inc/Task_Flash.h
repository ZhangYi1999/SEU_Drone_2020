#ifndef _TASK_FLASH_H_
#define _TASK_FLASH_H_

#include "System.h"

void FlashWrite(void);

uint32_t GetSector(uint32_t Address);
uint32_t GetSectorSize(uint32_t Sector);


#endif
