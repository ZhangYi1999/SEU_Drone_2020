#include "Task_Flash.h"

/*
首先在Task_Init.c里调用了FlashInit函数在一开机时就读取两个微调值,然后当进入自瞄状态时,
若按住shift键就可以更改微调值,wasd对应云台微调方向,当松开shift键后,就会将微调之后的值通过调用
flashwrite函数来写进flash里
*/


void FlashWrite(void)
{
	
	uint32_t FirstSector = 0, NbOfSectors = 0, Address = 0;
	uint32_t SectorError = 0;
	__IO uint32_t data32 = 0 , MemoryProgramStatus = 0;

	/*Variable used for Erase procedure*/
	static FLASH_EraseInitTypeDef EraseInitStruct;
	/* Unlock the Flash to enable the flash control register access *************/ 
  HAL_FLASH_Unlock();

  /* Erase the user Flash area(area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  /* Get the 1st sector to erase */
  FirstSector = GetSector(FLASH_USER_START_ADDR);
  /* Get the number of sector to erase from 1st sector*/
  NbOfSectors = GetSector(FLASH_USER_END_ADDR) - FirstSector + 1;

  /* Fill EraseInit structure*/
  EraseInitStruct.TypeErase = FLASH_TYPEERASE_SECTORS;
  EraseInitStruct.VoltageRange = FLASH_VOLTAGE_RANGE_3;
  EraseInitStruct.Sector = FirstSector;
  EraseInitStruct.NbSectors = NbOfSectors;
	
  if(HAL_FLASHEx_Erase(&EraseInitStruct, &SectorError) != HAL_OK)
  { 
    /* 
      Error occurred while sector erase. 
      User can add here some code to deal with this error. 
      SectorError will contain the faulty sector and then to know the code error on this sector,
      user can call function 'HAL_FLASH_GetError()'
    */
		/*
			FLASH_ErrorTypeDef errorcode = HAL_FLASH_GetError();
		*/
    //Error_Handler();/*虽然我们的错误处理函数里面什么也没有,但是这里放上可以以后方便调试的时候使用*/
  }
/* Program the user Flash area word by word
    (area defined by FLASH_USER_START_ADDR and FLASH_USER_END_ADDR) ***********/

  Address = FLASH_USER_START_ADDR;
  
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address,Wild_Change_Angle_Pitch.INT);/*注意!!!!!!!在这里写入数据啦,并且这里写入的是32位的数据,
																																									故使用的是FLASH_TYPEPROGRAM_WORD,还有FLASH_TYPEPROGRAM_HALFWORD等*/
  Address = Address + 4;
	
  HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, Address,Wild_Change_Angle_Yaw.INT);
	
  /* Lock the Flash to disable the flash control register access (recommended
     to protect the FLASH memory against possible unwanted operation) *********/
  HAL_FLASH_Lock(); 

  /* Check if the programmed data is OK 
      MemoryProgramStatus = 0: data programmed correctly
      MemoryProgramStatus != 0: number of words not programmed correctly ******/
  Address = FLASH_USER_START_ADDR;
  MemoryProgramStatus = 0x0;

	data32 = *(__IO uint32_t*)Address;

	if (data32 != Wild_Change_Angle_Pitch.INT)
	{
		MemoryProgramStatus++;  
	}
	Address = Address + 4;
	
	if (data32 != Wild_Change_Angle_Yaw.INT)
	{
		MemoryProgramStatus++;  
	}		

  /* Check if there is an issue to program data */
  if (MemoryProgramStatus == 1)
  {
    /* No error detected. Switch on LED3 */
    //Error_Handler();
  }
  
}
/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
static uint32_t GetSector(uint32_t Address)
{
  uint32_t sector = 0;
  
  if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
  {
    sector = FLASH_SECTOR_0;  
  }
  else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
  {
    sector = FLASH_SECTOR_1;  
  }
  else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
  {
    sector = FLASH_SECTOR_2;  
  }
  else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
  {
    sector = FLASH_SECTOR_3;  
  }
  else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
  {
    sector = FLASH_SECTOR_4;  
  }
  else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
  {
    sector = FLASH_SECTOR_5;  
  }
  else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
  {
    sector = FLASH_SECTOR_6;  
  }
  else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
  {
    sector = FLASH_SECTOR_7;  
  }
  else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
  {
    sector = FLASH_SECTOR_8;  
  }
  else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
  {
    sector = FLASH_SECTOR_9;  
  }
  else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
  {
    sector = FLASH_SECTOR_10;  
  }
  else if((Address < ADDR_FLASH_SECTOR_12) && (Address >= ADDR_FLASH_SECTOR_11))
  {
    sector = FLASH_SECTOR_11;  
  }
  else if((Address < ADDR_FLASH_SECTOR_13) && (Address >= ADDR_FLASH_SECTOR_12))
  {
    sector = FLASH_SECTOR_12;  
  }
  else if((Address < ADDR_FLASH_SECTOR_14) && (Address >= ADDR_FLASH_SECTOR_13))
  {
    sector = FLASH_SECTOR_13;  
  }
  else if((Address < ADDR_FLASH_SECTOR_15) && (Address >= ADDR_FLASH_SECTOR_14))
  {
    sector = FLASH_SECTOR_14;  
  }
  else if((Address < ADDR_FLASH_SECTOR_16) && (Address >= ADDR_FLASH_SECTOR_15))
  {
    sector = FLASH_SECTOR_15;  
  }
  else if((Address < ADDR_FLASH_SECTOR_17) && (Address >= ADDR_FLASH_SECTOR_16))
  {
    sector = FLASH_SECTOR_16;  
  }
  else if((Address < ADDR_FLASH_SECTOR_18) && (Address >= ADDR_FLASH_SECTOR_17))
  {
    sector = FLASH_SECTOR_17;  
  }
  else if((Address < ADDR_FLASH_SECTOR_19) && (Address >= ADDR_FLASH_SECTOR_18))
  {
    sector = FLASH_SECTOR_18;  
  }
  else if((Address < ADDR_FLASH_SECTOR_20) && (Address >= ADDR_FLASH_SECTOR_19))
  {
    sector = FLASH_SECTOR_19;  
  }
  else if((Address < ADDR_FLASH_SECTOR_21) && (Address >= ADDR_FLASH_SECTOR_20))
  {
    sector = FLASH_SECTOR_20;  
  } 
  else if((Address < ADDR_FLASH_SECTOR_22) && (Address >= ADDR_FLASH_SECTOR_21))
  {
    sector = FLASH_SECTOR_21;  
  }
  else if((Address < ADDR_FLASH_SECTOR_23) && (Address >= ADDR_FLASH_SECTOR_22))
  {
    sector = FLASH_SECTOR_22;  
  }
  else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_23))*/
  {
    sector = FLASH_SECTOR_23;  
  }
  return sector;
}

/**
  * @brief  Gets sector Size
  * @param  None
  * @retval The size of a given sector
  */
static uint32_t GetSectorSize(uint32_t Sector)
{
  uint32_t sectorsize = 0x00;
  if((Sector == FLASH_SECTOR_0) || (Sector == FLASH_SECTOR_1) || (Sector == FLASH_SECTOR_2) ||\
     (Sector == FLASH_SECTOR_3) || (Sector == FLASH_SECTOR_12) || (Sector == FLASH_SECTOR_13) ||\
     (Sector == FLASH_SECTOR_14) || (Sector == FLASH_SECTOR_15))
  {
    sectorsize = 16 * 1024;
  }
  else if((Sector == FLASH_SECTOR_4) || (Sector == FLASH_SECTOR_16))
  {
    sectorsize = 64 * 1024;
  }
  else
  {
    sectorsize = 128 * 1024;
  }  
  return sectorsize;
}




