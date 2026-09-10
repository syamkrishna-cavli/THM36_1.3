/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: flash_cache.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "flash_cache.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup FLASH_CACHE
  * @brief TIM HAL modules driver 
  * @{
  */
/** @defgroup FLASH_CACHE_Private_Defines HAL FLASH CACHE Private Defines
  * @{
  */
#define CACHE_ICON_DEFAULT		(0x00000002UL)
#define CACHE_ICFG_DEFAULT		(0x00000210UL)
#define CACHE_DCON_DEFAULT		(0x00000002UL)
#define CACHE_DCFG_DEFAULT		(0x00000210UL)
#define CACHE_ISADDR_DEFAULT	(0x00000000UL)
#define CACHE_IEADDR_DEFAULT	(0x0007FFFFUL)
#define CACHE_DSADDR_DEFAULT	(0x00000000UL)
#define CACHE_DEADDR_DEFAULT	(0x0007FFFFUL)
#define CACHE_ENABLE			(~(0x00000002UL))
#define CACHE_DISABLE			(0x00000002UL)
#define CACHE_REST				(~(0x00000001UL))
#define CACHE_NOT_REST			(0x00000001UL)
/**
  * @}
  */
/** @defgroup FLASH_CACHE_Exported_Function FLASH CACHE Exported Function
  * @{  
  */ 
  
/**
  * @brief  Fills Icache Struct member with its default value.
  * @param[in] hdcache: pointer to a Flash_Cache_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following FLASH_CACHE_ASSOCCFG_4_WAY/FLASH_CACHE_LINESIZE_16_BYTE/FLASH_CACHE_CRAM_SIZE_4K
  *
  */
void Flash_IcacheStructInit(Flash_Cache_InitTypeDef* hicache)
{
	hicache->ASSOCCFG = FLASH_CACHE_ASSOCCFG_4_WAY;
	hicache->CRAMSIZE = FLASH_CACHE_LINESIZE_16_BYTE;
	hicache->LINESIZE = FLASH_CACHE_CRAM_SIZE_4K;
}

/**
  * @brief  Fills Dcache Struct member with its default value.
  * @param[in] hdcache: pointer to a Flash_Cache_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following FLASH_CACHE_ASSOCCFG_4_WAY/FLASH_CACHE_LINESIZE_16_BYTE/FLASH_CACHE_CRAM_SIZE_4K
  *
  */
void Flash_DcacheStructInit(Flash_Cache_InitTypeDef* hdcache)
{
	hdcache->ASSOCCFG = FLASH_CACHE_ASSOCCFG_4_WAY;
	hdcache->CRAMSIZE = FLASH_CACHE_LINESIZE_16_BYTE;
	hdcache->LINESIZE = FLASH_CACHE_CRAM_SIZE_4K;
}

/**
  * @brief  Initializes the Icache  peripheral according to 
  *   the specified parameters in the Flash_Cache_InitTypeDef.
  * @param[in] hicache: where x can be 0 to 2 to select the SPI peripheral.
  * @param[in] hdcache: pointer to a SPI_InitTypeDef structure,which will be initialized.
  * @retval none
  */
void Flash_IcacheInit(Flash_Cache_TypeDef *Flash_Cache, Flash_Cache_InitTypeDef *hicache)
{
	Flash_Cache->ICFG = hicache->ASSOCCFG|hicache->CRAMSIZE|hicache->LINESIZE;
}

/**
  * @brief  Initializes the Dcache  peripheral according to 
  *   the specified parameters in the Flash_Cache_InitTypeDef.
  * @param[in] hicache: where x can be 0 to 2 to select the SPI peripheral.
  * @param[in] hdcache: pointer to a SPI_InitTypeDef structure,which will be initialized.
  * @retval none
  */
void Flash_DcacheInit(Flash_Cache_TypeDef *Flash_Cache,Flash_Cache_InitTypeDef* hdcache)
{
	Flash_Cache->DCFG = hdcache->ASSOCCFG|hdcache->CRAMSIZE|hdcache->LINESIZE;
}
/**
  * @brief Deinitializes the Icache peripheral registers to their default reset values.
  * @param[in] none
  * @retval none
  * @note  
  */
void Flash_IcacheDeInit(Flash_Cache_TypeDef *Flash_Cache)
{
	Flash_IcacheCmd(Flash_Cache,DISABLE);
	Flash_IcacheRest(Flash_Cache,ENABLE);
	Flash_Cache->ICON = CACHE_ICON_DEFAULT;
	Flash_Cache->ICFG = CACHE_ICFG_DEFAULT;
	Flash_Cache->ISADDR = CACHE_ISADDR_DEFAULT;
	Flash_Cache->IEADDR = CACHE_IEADDR_DEFAULT;
}

/**
  * @brief Deinitializes the Dcache peripheral registers to their default reset values.
  * @param[in] none
  * @retval none
  * @note  
  */
void Flash_DcacheDeInit(Flash_Cache_TypeDef *Flash_Cache)
{
	Flash_DcacheCmd(Flash_Cache,DISABLE);
	Flash_DcacheRest(Flash_Cache,ENABLE);
	Flash_Cache->DCON = CACHE_DCON_DEFAULT;
	Flash_Cache->DCFG = CACHE_DCFG_DEFAULT;
	Flash_Cache->DSADDR = CACHE_DSADDR_DEFAULT;
	Flash_Cache->DEADDR = CACHE_DEADDR_DEFAULT;

}
/**
  * @brief Reset Icache and Dcache all Register
  * @param[in] none
  * @retval none
  * @note  none
  */
void Flash_CacheRegisterReset(void)
{
	RCC_AHBPeriphSoftReset(RCC_AHBPeriph_FLASHCACHE);
	while(RCC_AHBPeriphGetResetFlag(RCC_AHBPeriph_FLASHCACHE)==RESET);
}

/**
  * @brief Get the Icache wether Enable
  * @param[in] none
  * @retval none
  * @note  none
  */

FlagStatus Flash_GetIcacheEnable(Flash_Cache_TypeDef *Flash_Cache)
{
	if(Flash_Cache->ICON&0x02)
	{
		return RESET;
	}
	else
	{
		return SET;
	}
}

/**
  * @brief Get the Icache wether Enable
  * @param[in] none
  * @retval none
  * @note  none
  */
FlagStatus Flash_GetDcacheEnable(Flash_Cache_TypeDef *Flash_Cache)
{
	if(Flash_Cache->DCON&0x02)
	{
		return RESET;
	}
	else
	{
		return SET;
	}
}

/**
  * @brief Enable or Disable ICache
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note  
  */
void Flash_IcacheCmd(Flash_Cache_TypeDef *Flash_Cache,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		Flash_Cache->ICON&=CACHE_ENABLE;
	}
	else
	{
		Flash_Cache->ICON|=CACHE_DISABLE;
	}
}

/**
  * @brief Enable or Disable DCache
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note  
  */
void Flash_DcacheCmd(Flash_Cache_TypeDef *Flash_Cache,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		Flash_Cache->DCON&=CACHE_ENABLE;
	}
	else
	{
		Flash_Cache->DCON|=CACHE_DISABLE;
	}
}

/**
  * @brief Enable or Disable Rest ICache buffer data
  * @param[in] NewState: NewState: new state of Flash Icache
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note when rest cache,the buffer data of cache will be clear 
  */
void Flash_IcacheRest(Flash_Cache_TypeDef *Flash_Cache,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		Flash_Cache->ICON&= CACHE_REST;
	}
	else
	{
		Flash_Cache->ICON|= CACHE_NOT_REST;
	}
}

/**
  * @brief Enable or Disable Rest DCache buffer data
  * @param[in] NewState: NewState: new state of Flash Dcache
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note when rest cache,the buffer data of cache will be clear 
  */
void Flash_DcacheRest(Flash_Cache_TypeDef *Flash_Cache,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		Flash_Cache->DCON&= CACHE_REST;
	}
	else
	{
		Flash_Cache->DCON|= CACHE_NOT_REST;
	}
}

/**
  * @brief config the Dcache's Start Address and End Address
  * @param[in] StartAddress: the Dcache start address
  * @param[in] EndAddress: the Dcache end address
  * @retval none
  * @note none
  */
void Flash_DcacheConfigAddress(Flash_Cache_TypeDef *Flash_Cache,uint32_t StartAddress,uint32_t EndAddress)
{
	Flash_Cache->DSADDR = StartAddress;
	Flash_Cache->DEADDR = EndAddress;
}

/**
  * @brief config the Icache's Start Address and End Address
  * @param[in] StartAddress: the Icache start address
  * @param[in] EndAddress: the Icache end address
  * @retval none
  * @note none
  */
void Flash_IcacheConfigAddress(Flash_Cache_TypeDef *Flash_Cache,uint32_t StartAddress,uint32_t EndAddress)
{
	Flash_Cache->ISADDR = StartAddress;
	Flash_Cache->IEADDR = EndAddress;
}
/**
  * @brief Get Flash Cache Status
  * @param[in] CacheFlag: the Flag of CacheFlag
  *		@arg FLASH_CACHE_Flag_ICACHE
  *		@arg FLASH_CACHE_Flag_DCACHE
  * @param[in] EndAddress: the Icache end address
  * @retval none
  * @note none
  */
CacheStatus Flash_CacheGetStatus(Flash_Cache_TypeDef *Flash_Cache,uint32_t CacheFlag)
{
	if(Flash_Cache->STS&CacheFlag)
	{
		return BUSY;
	}
	return IDLE;
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

