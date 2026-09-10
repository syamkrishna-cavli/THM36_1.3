/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_flash_cache.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "tmc_rcc.h"
#include "flash_cache.h"
#include "tmc_flash_cache.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_FLASH_CACHE
  * @brief FLASH CACHE TMC modules driver 
  * @{
  */
/** @defgroup TMC_FLASH_CACHE_private_defines TMC FLASH CACHE private defines
  * @{
  */
#define IDLE_TimeOut		   	(0x00000100UL) 
  
/**
  * @}
  */  
/**
  * @brief  Fills each Flash_CacheHandleTypeDef member with its default value.
  * @param[in] Flash_Cache: pointer to a SPI_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak TMC_StatusTypeDef TMC_FLASH_ICacheStructInit(Flash_CacheHandleDef* hcache)
{
	hcache->EndAddress = 0x0007FFFFUL;
	hcache->StartAddress = 0UL;
	hcache->Instance = Flash_ACC;
	hcache->CacheBypassState = TMC_CACHE_STATE_ENABLE;
	hcache->CacheReatState = TMC_CACHE_STATE_NOT_RESET;
	Flash_IcacheStructInit(&hcache->hcacheInit);
	return TMC_OK;
}
/**
  * @brief  Fills each Flash_CacheHandleTypeDef member with its default value.
  * @param[in] Flash_Cache: pointer to a Flash_CacheHandleDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak TMC_StatusTypeDef TMC_FLASH_DCacheStructInit(Flash_CacheHandleDef* hcache)
{
	hcache->EndAddress = 0x0007FFFFUL;
	hcache->StartAddress = 0UL;
	hcache->Instance = Flash_ACC;
	hcache->CacheBypassState = TMC_CACHE_STATE_DISABLE;
	hcache->CacheReatState = TMC_CACHE_STATE_RESET;
	Flash_DcacheStructInit(&hcache->hcacheInit);
	return TMC_OK;
}
/**
  * @brief  Initializes the cache Unit according to the specified
  *		 parameters in the Flash_CacheTypeDef and create the associated handle.
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  *		 the configuration information for Cache module.
  * @retval None
  */
TMC_StatusTypeDef TMC_FLASH_IcacheInit(Flash_CacheHandleDef* hcache) 
{
	volatile uint32_t temp_clock,temp_division;
	RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
	temp_clock = RCC_GetSystermClkSource();
	temp_division = RCC_GetSystermClkDiv();
	if(hcache == NULL)
	{
		return TMC_ERROR;
	}
	if(Flash_GetIcacheEnable(hcache->Instance) == SET)/*Icahce已经打开时*/
	{
		if(TMC_ConfigIcache(hcache,DISABLE) == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
	}
	/* before enable Icache must be reduced frequency*/
	RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
	RCC_SysclkInitStruct.SYSCLKDIV = 0;
	TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
	
	Flash_IcacheRest(hcache->Instance,ENABLE);
	Flash_IcacheConfigAddress(hcache->Instance,hcache->StartAddress,hcache->EndAddress);
	Flash_IcacheInit(hcache->Instance,&hcache->hcacheInit);
	Flash_IcacheRest(hcache->Instance,DISABLE);
	Flash_IcacheCmd(hcache->Instance,ENABLE);
	/*enable Icache, recovery frequency*/
	RCC_SysclkInitStruct.SYSCLKSource = temp_clock;
	RCC_SysclkInitStruct.SYSCLKDIV = temp_division;
	TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
	
	hcache->CacheReatState = TMC_CACHE_STATE_NOT_RESET;
	hcache->CacheBypassState = TMC_CACHE_STATE_ENABLE;
	return TMC_OK;
}

/**
  * @brief  Initializes the cache Unit according to the specified
  *		 parameters in the Flash_CacheTypeDef and create the associated handle.
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  *		 the configuration information for Cache module.
  * @retval None
  */
TMC_StatusTypeDef TMC_FLASH_DcacheInit(Flash_CacheHandleDef* hcache) 
{
	volatile uint32_t temp_clock,temp_division;
	RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
	temp_clock = RCC_GetSystermClkSource();
	temp_division = RCC_GetSystermClkDiv();
	if(hcache == NULL)
	{
		return TMC_ERROR;
	}
	if(Flash_GetDcacheEnable(hcache->Instance) == SET)/*Dcahce已经打开时*/
	{
		if(TMC_ConfigDcache(hcache,DISABLE) == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
	}
	/* before enable Icache must be reduced frequency*/
	RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
	RCC_SysclkInitStruct.SYSCLKDIV = 0;
	TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
	
	Flash_DcacheRest(hcache->Instance,ENABLE);
	Flash_DcacheConfigAddress(hcache->Instance,hcache->StartAddress,hcache->EndAddress);
	Flash_DcacheInit(hcache->Instance,&hcache->hcacheInit);
	Flash_DcacheRest(hcache->Instance,DISABLE);
	Flash_DcacheCmd(hcache->Instance,ENABLE);
	/*enable Icache, recovery frequency*/
	RCC_SysclkInitStruct.SYSCLKSource = temp_clock;
	RCC_SysclkInitStruct.SYSCLKDIV = temp_division;
	TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
	
	hcache->CacheReatState = TMC_CACHE_STATE_NOT_RESET;
	hcache->CacheBypassState = TMC_CACHE_STATE_ENABLE;
	return TMC_OK;
}


/**
  * @brief  DeInitializes the FLASH_Cache peripheral 
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  * 		the configuration information for FLASH Cache module.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_FLASH_IcacheDeInit(Flash_CacheHandleDef* hcache) 
{
	if(hcache == NULL)
	{
		return TMC_ERROR;
	}
	Flash_IcacheStructInit(&hcache->hcacheInit);
	Flash_IcacheDeInit(hcache->Instance);
	hcache->CacheReatState = TMC_CACHE_STATE_RESET;
	hcache->CacheBypassState = TMC_CACHE_STATE_DISABLE;
	return TMC_OK;
} 

/**
  * @brief  DeInitializes the FLASH_Cache peripheral 
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  * 		the configuration information for FLASH Cache module.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_FLASH_DcacheDeInit(Flash_CacheHandleDef* hcache) 
{
	if(hcache == NULL)
	{
		return TMC_ERROR;
	}
	Flash_DcacheStructInit(&hcache->hcacheInit);
	Flash_DcacheDeInit(hcache->Instance);
	hcache->CacheReatState = TMC_CACHE_STATE_RESET;
	hcache->CacheBypassState = TMC_CACHE_STATE_DISABLE;
	return TMC_OK;
} 
/**
  * @brief  enable or disable Icache
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  * 		the configuration information for FLASH Cache module.
  * @param[in] NewState: NewState: new state of the Icache peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_ConfigIcache(Flash_CacheHandleDef* hcache,FunctionalState NewState)
{
	volatile uint32_t temp_clock,temp_division;
	RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
	temp_clock = RCC_GetSystermClkSource();
	temp_division = RCC_GetSystermClkDiv();
	if(hcache == NULL)
	{
		return TMC_ERROR;
	}
	if(NewState == ENABLE)
	{
		/*如果Icache已经打开，不需要再次启动*/
		if((hcache->CacheBypassState == TMC_CACHE_STATE_DISABLE)||(Flash_GetIcacheEnable(hcache->Instance) == RESET))
		{
			/* before enable Icache must be reduced frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
			RCC_SysclkInitStruct.SYSCLKDIV = 0;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			Flash_IcacheRest(hcache->Instance,DISABLE);
			Flash_IcacheCmd(hcache->Instance,ENABLE);
			
			/*enable Icache, recovery frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = temp_clock;
			RCC_SysclkInitStruct.SYSCLKDIV = temp_division;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			hcache->CacheReatState = TMC_CACHE_STATE_NOT_RESET;
			hcache->CacheBypassState = TMC_CACHE_STATE_ENABLE;
		}
	}
	else
	{	/*如果Icache已经关闭*/
		if((hcache->CacheBypassState == TMC_CACHE_STATE_ENABLE)||(Flash_GetIcacheEnable(hcache->Instance) == SET))
		{
			if(TMC_FLASH_CacheWaitForOperation(hcache,FLASH_CACHE_Flag_ICACHE,IDLE_TimeOut)== TMC_TIMEOUT)
			{
				return TMC_TIMEOUT;
			}
			/* before enable Icache must be reduced frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
			RCC_SysclkInitStruct.SYSCLKDIV = 0;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			Flash_IcacheCmd(hcache->Instance,DISABLE);
			Flash_IcacheRest(hcache->Instance,ENABLE);
			
			/*enable Icache, recovery frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = temp_clock;
			RCC_SysclkInitStruct.SYSCLKDIV = temp_division;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			hcache->CacheReatState = TMC_CACHE_STATE_RESET;
			hcache->CacheBypassState = TMC_CACHE_STATE_DISABLE;
		}
	}
	return TMC_OK;
}

/**
  * @brief  enable or disable Dcache
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  * 		the configuration information for FLASH Cache module.
  * @param[in] NewState: NewState: new state of the Dcache peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_ConfigDcache(Flash_CacheHandleDef* hcache,FunctionalState NewState)
{
	
	volatile uint32_t temp_clock,temp_division;
	RCC_SysclkInitTypeDef RCC_SysclkInitStruct;
	temp_clock = RCC_GetSystermClkSource();
	temp_division = RCC_GetSystermClkDiv();
	if(hcache == NULL)
	{
		return TMC_ERROR;
	}
	if(NewState == ENABLE)
	{
		if((Flash_CacheGetStatus(hcache->Instance,FLASH_CACHE_Flag_DCACHE) == IDLE)||(Flash_GetDcacheEnable(hcache->Instance) == RESET))
		{
			/* before enable Icache must be reduced frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
			RCC_SysclkInitStruct.SYSCLKDIV = 0;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			Flash_DcacheRest(hcache->Instance,DISABLE);
			Flash_DcacheCmd(hcache->Instance,ENABLE);
			/*enable Icache, recovery frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = temp_clock;
			RCC_SysclkInitStruct.SYSCLKDIV = temp_division;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			hcache->CacheReatState = TMC_CACHE_STATE_NOT_RESET;
			hcache->CacheBypassState = TMC_CACHE_STATE_ENABLE;
		}
	}
	else
	{
		if((Flash_CacheGetStatus(hcache->Instance,FLASH_CACHE_Flag_DCACHE) == BUSY))
		{
			
			if(TMC_FLASH_CacheWaitForOperation(hcache,FLASH_CACHE_Flag_DCACHE,IDLE_TimeOut)== TMC_TIMEOUT)
			{
				return TMC_TIMEOUT;
			}
			/* before enable Icache must be reduced frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = RCC_SYS_CLKSRC_HSI;
			RCC_SysclkInitStruct.SYSCLKDIV = 0;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			Flash_DcacheCmd(hcache->Instance,DISABLE);
			Flash_DcacheRest(hcache->Instance,ENABLE);
			
			/*enable Icache, recovery frequency*/
			RCC_SysclkInitStruct.SYSCLKSource = temp_clock;
			RCC_SysclkInitStruct.SYSCLKDIV = temp_division;
			TMC_RCC_SystemClockConfig(&RCC_SysclkInitStruct);
			
			hcache->CacheReatState = TMC_CACHE_STATE_RESET;
			hcache->CacheBypassState = TMC_CACHE_STATE_DISABLE;
		}
	}
	return TMC_OK;
}

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @param[in]  Flag: the Flag of Flash Cache
  *			@arg FLASH_CACHE_Flag_DCACHE
  *			@arg FLASH_CACHE_Flag_ICACHE
  * @param[in]  Timeout: FLASH progamming Timeout
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_CacheWaitForOperation(Flash_CacheHandleDef* hcache,uint32_t Flag,uint32_t Timeout)
{ 
	uint32_t temp;
	temp = TMC_GetTick();
	while(TMC_GetTick()<(temp+Timeout))
	{
		if(Flash_CacheGetStatus(hcache->Instance,Flag) == IDLE)
		{
			return TMC_OK;
		}
	}
	return TMC_TIMEOUT;
}

/**
  * @brief  Get Cache Status of Rest
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  * 		the configuration information for FLASH Cache module.
  * @retval TMC_CACHE_REST_StateTypeDef status
  *			@arg TMC_CACHE_STATE_RESET
  *			@arg TMC_CACHE_STATE_NOT_RESET
  */
TMC_CACHE_REST_StateTypeDef TMC_FLASH_CacheGetRestStatus(Flash_CacheHandleDef* hcache)
{ 
	return hcache->CacheReatState;
}

/**
  * @brief  Get Cache Status of ByPass
  * @param[in]  hcache: pointer to a Flash_CacheHandleDef structure that contains
  * 		the configuration information for FLASH Cache module.
  * @retval TMC_CACHE_REST_StateTypeDef status
  *			@arg TMC_CACHE_STATE_RESET
  *			@arg TMC_CACHE_STATE_NOT_RESET
  */
TMC_CACHE_BYPASS_StateTypeDef TMC_FLASH_CacheGetBypassStatus(Flash_CacheHandleDef* hcache)
{ 
	return hcache->CacheBypassState;
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

