#include "Task_JetsonComm.h"


void Task_JetsonComm(void *parameters)
{
	while(1)
	{
		ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
	}
}

