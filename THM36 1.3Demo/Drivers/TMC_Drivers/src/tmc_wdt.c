/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_wdt.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "TMC_WDT.h"

/** @addtogroup thm36_1.3
  * @{
  */
  
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */

/** @defgroup TMC_WDT
  * @brief WDT TMC modules driver
  * @{
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup TMC_WDT_Exported_Functions TMC WDT Exported Functions
  * @{
  */

/**
  * @brief  Initializes the UART mode according to the specified parameters in
  *		 the UART_InitTypeDef and create the associated handle.
  * @param[in]  huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_WDT_Init(WDT_HandleTypeDef *hwdt)
{
	/* Check the WDT handle allocation */
	if(hwdt == NULL)
	{
		return TMC_ERROR;
	}
	hwdt->WDT = WWDT;
	if(hwdt->State == TMC_WDT_STATE_RESET)
	{
		/* Init the low level hardware */
		TMC_WDT_MspInit(hwdt);	
	}
	WDT_Init(hwdt->WDT, hwdt->Init);
	
	WDT_Cmd(hwdt->WDT, ENABLE);
	
	hwdt->State = TMC_WDT_STATE_BUSY;
	
	return TMC_OK;
}

/**
  * @brief  DeInitializes the UART peripheral. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_WDT_DeInit(WDT_HandleTypeDef *hwdt)
{
	/* Check the WDT handle allocation */
	if(hwdt == NULL)
	{
		return TMC_ERROR;
	}
  	
	WDT_DeInit();
	TMC_WDT_MspDeInit(hwdt);
	hwdt->State = TMC_WDT_STATE_RESET;
	return TMC_OK;
}

/**
  * @brief  Refresh the WDT.
  * @param  hwdt  pointer to a WDT_HandleTypeDef structure that contains
  *				the configuration information for the specified WDT module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_WDT_Refresh(WDT_HandleTypeDef *hwdt)
{
	/* Write to WWDG CR the WWDG Counter value to refresh with */
	WDT_FeedDog(hwdt->WDT, (hwdt->Init));
	
	return TMC_OK;
}

/**
  * @brief  This function handles WDT interrupt request.
  * @param[in]  huart: pointer to a TMC_WDT_HandleTypeDef structure that contains
  *		 the configuration information for the specified UART module.
  * @retval None
  */
void TMC_WDT_IRQHandler(WDT_HandleTypeDef *hwdt)
{
	/* Check if Early Wakeup Interrupt is enable */
	if(WDT_GetITStatus(hwdt->WDT) != RESET)
	{
		/* Clear the WWDG Early Wakeup flag */
		WDT_ClearFlag(hwdt->WDT);
		//TMC_WDT_Refresh(hwdt);
		hwdt->State = TMC_WDT_STATE_OVERFLOW;
		
		/* Wakeup callback */ 
		//TMC_WDT_Callback(hwdt); 
	}
}


/**
  * @brief  Initialize the WDT MSP.
  * @param  hwwdg  pointer to a WDT_HandleTypeDef structure that contains
  *				the configuration information for the specified WDT module.
  * @note   When rewriting this function in user file, mechanism may be added
  *		 to avoid multiple initialize when TMC_WDT_Init function is called
  *		 again to change parameters.
  * @retval None
  */
__weak void TMC_WDT_MspInit(WDT_HandleTypeDef *hwdt)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hwdt);

	/* NOTE: This function should not be modified, when the callback is needed,
			the HAL_WWDG_MspInit could be implemented in the user file
	*/
}

/**
  * @brief  DeInitialize the WDT MSP.
  * @param  hwwdg  pointer to a WDT_HandleTypeDef structure that contains
  *				the configuration information for the specified WDT module.
  * @note   When rewriting this function in user file, mechanism may be added
  *		 to avoid multiple initialize when TMC_WDT_Init function is called
  *		 again to change parameters.
  * @retval None
  */
__weak void TMC_WDT_MspDeInit(WDT_HandleTypeDef *hwdt)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hwdt);

	/* NOTE: This function should not be modified, when the callback is needed,
			the HAL_WWDG_MspInit could be implemented in the user file
	*/
}

/**
  * @brief  WDT callback.
  * @param  hwdt: pointer to a WDT_HandleTypeDef structure that contains
  *			   the configuration information for the specified WDT module.
  * @retval None
  */
__weak void TMC_WDT_Callback(WDT_HandleTypeDef *hwdt)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hwdt);

	/* NOTE: This function should not be modified, when the callback is needed,
		   the HAL_WDT_Callback could be implemented in the user file
	
	*/ 
}

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
/************************ (C) COPYRIGHT TMC *****END OF FILE****/


