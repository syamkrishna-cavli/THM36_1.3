/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: flash_cachemain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

#include "thm36_1.3.h"
#include "TMC_RCC.h"
#include "TMC_Flash_Cache.h"
Flash_CacheHandleDef hicache;
Flash_CacheHandleDef hdcache;
void Flash_CacheDemomain(void);

TMC_StatusTypeDef TMC_FLASH_ICacheStructInit(Flash_CacheHandleDef* hcache)
{
	hcache->EndAddress = 0x00001000UL;
	hcache->StartAddress = 0UL;
	hcache->Instance = Flash_ACC;
	hcache->CacheBypassState = TMC_CACHE_STATE_ENABLE;
	hcache->CacheReatState = TMC_CACHE_STATE_NOT_RESET;
	Flash_IcacheStructInit(&hcache->hcacheInit);
	return TMC_OK;
}
TMC_StatusTypeDef TMC_FLASH_DCacheStructInit(Flash_CacheHandleDef* hcache)
{
	hcache->EndAddress = 0x0007FFFFUL;
	hcache->StartAddress = 0UL;
	hcache->Instance = Flash_ACC;
	hcache->CacheBypassState = TMC_CACHE_STATE_DISABLE;
	hcache->CacheReatState = TMC_CACHE_STATE_RESET;
	Flash_DcacheStructInit(&hcache->hcacheInit);
	return TMC_OK;
}
void Flash_CacheDemomain(void)
{
	/*ICache Configuration.*/
	TMC_FLASH_ICacheStructInit(&hicache);
	TMC_FLASH_IcacheInit(&hicache);
	/*DCache Configuration.*/
	TMC_FLASH_DCacheStructInit(&hdcache);
	TMC_FLASH_DcacheInit(&hdcache); 
	/*Close DCache and ICache.*/
	TMC_FLASH_IcacheDeInit(&hicache);
	TMC_FLASH_DcacheDeInit(&hdcache);
	
}


