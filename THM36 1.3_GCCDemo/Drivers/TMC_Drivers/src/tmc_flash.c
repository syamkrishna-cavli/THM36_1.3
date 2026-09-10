/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_flash.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "tmc_flash.h"
#include "flash_cache.h"
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_FLASH
  * @brief FLASH TMC modules driver 
  * @{
  */
/** @defgroup TMC_FLASH_Exported_Function TMC FLASH Exported Function
  * @brief FLASH TMC modules driver
  * @{
  */
  
/** @defgroup TMC_FLASH_private_defines TMC FLASH private defines
  * @{
  */

#define EraseTimeOut			 	(0x00000FFFUL)
#define ProgramTimeOut		   		(0x00000FFFUL)
#define VerfiyTimeOut				(0x00000FFFUL)
#define PageSize					512
extern  uint32_t uwTick;
/**
  * @}
  */  
 
/**
  * @brief  Fills each Flash_EraseTypeDef member with its default value.
  * @param[in] Flash_EraseTypeDef: pointer to a Flash_EraseTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_Flash_EraseStructInit(Flash_EraseTypeDef *EraseMode)
{
	EraseMode->EraseVread = FLASH_ERASE_NOT_VREAD;
	EraseMode->HardVerify = FLASH_ERASE_NOT_VERIFY;
}

/**
  * @brief  Fills each Flash_EraseTypeDef member with its default value.
  * @param[in] Flash_EraseTypeDef: pointer to a Flash_EraseTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_Flash_ProgramStructInit(Flash_ProgramTypeDef *ProgramMode)
{
	ProgramMode->HardVerify = FLASH_PROGRAM_NOT_VERIFY;
	ProgramMode->ProgramVread = FLASH_PROGRAM_NOT_VREAD;

}

/**
  * @brief Flash Intial 
  * @param[in]  none
  * @retval none
  */
void TMC_FLASH_Msp_Init(void)
{
	while(RCC_Security2PeriphGetResetFlag(RCC_SEC2Periph_FLASH) == RESET);
	FLASH_ClearFlag(FLASH_FLAG_OPERR|FLASH_FLAG_OVER|FLASH_FLAG_EVERR|FLASH_FLAG_PVERR|FLASH_FLAG_HVERR);
}

/**
  * @brief  Waits for a Flash operation to complete or a TIMEOUT to occur.
  * @param[in]  Timeout: FLASH progamming Timeout
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_WaitForOperation(uint32_t Timeout)
{ 
//	uint32_t temp;
//	uint32_t t = 0x10000000;
//	temp = TMC_GetTick();
//	while(uwTick<(temp+Timeout))
//	{
//		if(FLASH_GetFlagStatus(FLASH_FLAG_OVER|FLASH_FLAG_EVERR|FLASH_FLAG_PVERR|FLASH_FLAG_HVERR) == SET)
//		{			
//			return TMC_OK;
//		}
//	}
//	return TMC_TIMEOUT;
	while((!(FLASH_GetFlagStatus(FLASH_FLAG_OVER|FLASH_FLAG_EVERR|FLASH_FLAG_PVERR|FLASH_FLAG_HVERR)== SET))&&(Timeout--));
	if(!(Timeout == 0))
		{			
			return TMC_OK;
		}
	return TMC_TIMEOUT;
}


/**
  * @brief  Erases FLASH pages.
  * @param[in]  Address: The Start page address to be erased.
  * @param[in]  Nbpage: The numbers of pages to be erased.
  * @param[in]  EraseMode: pointer to a Flash_EraseTypeDef structure that contains the Erase Verify mode for the Flash.
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_ErasePage(uint32_t Address,uint32_t Nbpage,Flash_EraseTypeDef *EraseMode,uint32_t TimeOut)
{
	TMC_StatusTypeDef status;
	uint32_t TryNum = 0;
	uint32_t i;
	/* Start Erase page */ 
	for(i=0;i<Nbpage;i++)
	{
		EraseMode->EraseVread = FLASH_ERASE_VREAD;
		EraseMode->EraseMode = FLASH_ERASE_FAST;
		Flash_ErasePage((Address+(i*512)),EraseMode);
		/* Get Erase OutTime Status */ 
		status = TMC_FLASH_WaitForOperation(TimeOut);
		if(status == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		/* Get Erase Error Status */ 
		while(FLASH_GetFlagStatus(FLASH_FLAG_HVERR|FLASH_FLAG_EVERR) == SET)
		{
			TryNum++;
			if(TryNum>3)
			{
				return TMC_ERROR;
			}
			FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_HVERR|FLASH_FLAG_EVERR);
			/* set TryNum Value*/ 
			Flash_ConfigFastEraseTryTimes(TryNum);
			/* Start Erase page */ 
			if(TryNum == 3)
			{
				EraseMode->EraseMode = FLASH_ERASE_NORMAL;
				EraseMode->EraseVread = FLASH_ERASE_NOT_VREAD;
			}
			Flash_ErasePage((Address+(i*512)),EraseMode);
		}
		/* Clear Fast Erase TryTimes */
		FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_HVERR|FLASH_FLAG_EVERR);
		Flash_ConfigFastEraseTryTimes(FLASH_ERASE_TRY_0);
	
	}
	TMC_FLASH_Msp_Init();
	return TMC_OK;
}

/**
  * @brief  Flash Programs  Data at a specified Dest address(Each programming double Word).
  * @note 	If Program data to Dest Address,the page of  DestAddress need to be Erased		
  * @param[in]  DestAddress: specifies the Dest address to be programmed.
  * @param[in]  SourAddress: specifies the source address to be programmed.
  * @param[in]  Length: the length of programming Data (Unit: double Word)
  * @param[in]  Program: pointer to a Flash_ProgramTypeDef structure that contains the program Verify mode for the Flash.
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_ProgramDoubleWord(uint32_t DestAddress,uint32_t SourAddress,uint32_t Length,Flash_ProgramTypeDef *Program,uint32_t TimeOut)
{
	uint32_t num;
	for(num=0;num<Length;num++)
	{
		/* Start Program DoubleWord */ 
		FLASH_ProgramDoubleWord(DestAddress+(num*8),*((uint64_t*)(SourAddress+(num*8))),Program);
		/* Check Flash Program is TimeOut*/  
		if(TMC_FLASH_WaitForOperation(TimeOut) == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		/* Check Flash HardwareVerify is Error*/
		if(FLASH_GetFlagStatus(FLASH_FLAG_PVERR) == SET)
		{
			FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_PVERR);
			return TMC_ERROR;
		}
		FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_PVERR);
	}
	TMC_FLASH_Msp_Init();
	return 	TMC_OK;
}

/**
  * @brief  Flash Programs  Data at a specified Dest address(Each programming max numbers is half page,min numbers is double word).
  * @note 	If Program data to Dest Address,the page of  DestAddress need to be Erased 
  * @param[in]  DestAddress: specifies the Dest address to be programmed.
  * @param[in]  SourAddress: specifies the source address to be programmed.
  * @param[in]  Length: the length of programming Data (Unit: double Word)
  * @param[in]  Program: pointer to a Flash_ProgramTypeDef structure that contains the program Verify mode for the Flash.
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_ProgramHalfPage(uint32_t DestAddress,uint32_t SourAddress,uint32_t Length,Flash_ProgramTypeDef *Program,uint32_t TimeOut)
{
	/* Start Program DoubleWord */ 
	FLASH_ProgramHalfPage(DestAddress,SourAddress,Length,Program);
	/* Check Flash Program is TimeOut*/  
	if(TMC_FLASH_WaitForOperation(TimeOut) == TMC_TIMEOUT)
	{
		return TMC_TIMEOUT;
	}
	/* Check Flash HardwareVerify is Error*/
	if(FLASH_GetFlagStatus(FLASH_FLAG_PVERR) == SET)
	{
		FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_PVERR);
		TMC_FLASH_Msp_Init();
		return TMC_ERROR;
	}
	FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_PVERR);
	TMC_FLASH_Msp_Init();
	return 	TMC_OK;
}

/**
  * @brief  Flash Erase hardware Verify 
  * @param[in]  Address: specifies the Address to hardverify
  * @param[in]  VerifyMode: the hardwarewary mode
  *				@arg FLASH_ERASE_VERIFY_MODE_DOUBLEWORD
  *				@arg FLASH_ERASE_VERIFY_MODE_PAGE
  * @param[in]  Vread: enable or disable Erase Vread
  *				@arg FLASH_ERASE_VREAD
  *				@arg FLASH_ERASE_NOT_VREAD
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_HardwareVerify(uint32_t Address,uint32_t VerifyMode,uint32_t Vread,uint32_t TimeOut)
{
	FLASH_HardVerify(Address,VerifyMode,Vread);
	/* Check Flash Program is TimeOut*/  
	if(TMC_FLASH_WaitForOperation(TimeOut) == TMC_TIMEOUT)
	{
		return TMC_TIMEOUT;
	}
	/* Check Flash HardwareVerify is Error*/
	if(FLASH_GetFlagStatus(FLASH_FLAG_HVERR) == SET)
	{
		FLASH_ClearFlag(FLASH_FLAG_OVER|FLASH_FLAG_HVERR);
		return TMC_ERROR;
	}
	TMC_FLASH_Msp_Init();
	return 	TMC_OK;
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

