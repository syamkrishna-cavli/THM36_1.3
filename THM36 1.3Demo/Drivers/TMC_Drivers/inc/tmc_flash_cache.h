/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_flash_cache.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __TMC_FLASH_Cache_H
#define __TMC_FLASH_Cache_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "FLASH_CACHE.h"
	 
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
/** @defgroup TMC_FLASH_CACHE_exported_Types FLASH CACHE exported types
  * @{
  */
typedef enum
{
  TMC_CACHE_STATE_RESET  = 0x00U,   
  TMC_CACHE_STATE_NOT_RESET  = 0x01U,
}TMC_CACHE_REST_StateTypeDef;	

typedef enum
{
  TMC_CACHE_STATE_ENABLE  = 0x00U,   
  TMC_CACHE_STATE_DISABLE  = 0x01U,
}TMC_CACHE_BYPASS_StateTypeDef;
/** 
  * @brief  Flash_Cashe structures definition  
  */ 
typedef struct
{
	uint32_t StartAddress;
	uint32_t EndAddress;
	Flash_Cache_TypeDef *Instance; 
	Flash_Cache_InitTypeDef hcacheInit;
	TMC_CACHE_REST_StateTypeDef		CacheReatState;
	TMC_CACHE_BYPASS_StateTypeDef	CacheBypassState;
} Flash_CacheHandleDef; 
/** @addtogroup FLASH_CACHE_Exported_Function 
  * @{  
  */ 
TMC_StatusTypeDef TMC_FLASH_DCacheStructInit(Flash_CacheHandleDef* hcache);
TMC_StatusTypeDef TMC_FLASH_ICacheStructInit(Flash_CacheHandleDef* hcache);
TMC_StatusTypeDef TMC_FLASH_IcacheInit(Flash_CacheHandleDef* hcache);
TMC_StatusTypeDef TMC_FLASH_DcacheInit(Flash_CacheHandleDef* hcache);
TMC_StatusTypeDef TMC_FLASH_IcacheDeInit(Flash_CacheHandleDef* hcache);
TMC_StatusTypeDef TMC_FLASH_DcacheDeInit(Flash_CacheHandleDef* hcache);
TMC_StatusTypeDef TMC_ConfigIcache(Flash_CacheHandleDef* hcache,FunctionalState NewState);
TMC_StatusTypeDef TMC_ConfigDcache(Flash_CacheHandleDef* hcache,FunctionalState NewState);
TMC_StatusTypeDef TMC_FLASH_CacheWaitForOperation(Flash_CacheHandleDef* hcache,uint32_t Flag,uint32_t Timeout);
TMC_CACHE_REST_StateTypeDef TMC_FLASH_CacheGetRestStatus(Flash_CacheHandleDef* hcache);
TMC_CACHE_BYPASS_StateTypeDef TMC_FLASH_CacheGetBypassStatus(Flash_CacheHandleDef* hcache);
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
/**
  * @}
  */	 
#ifdef __cplusplus
}
#endif

#endif /* __TMC_FLASH_Cache_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

