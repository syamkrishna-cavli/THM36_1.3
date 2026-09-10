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
#include "RCC.h"
#include "TMC_Flash.h"
#include "Flash_Cache.h"
#include "flash.h"
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

/* ÉùÃ÷ */
TMC_StatusTypeDef TMC_FLASH_ProgramInOnePage(uint32_t destAddr, uint8_t * pbData, uint32_t wDataLen);
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
  * @brief  	Reset FLASH Dcache
  * @param[in]  void
  * @retval 	void
  *
  */
void TMC_Reset_Flash_Dcache(void)
{
	Flash_DcacheCmd(Flash_ACC,DISABLE);
	while(Flash_CacheGetStatus(Flash_ACC,FLASH_CACHE_Flag_DCACHE) == BUSY);
	Flash_DcacheRest(Flash_ACC,ENABLE);
	Flash_DcacheRest(Flash_ACC,DISABLE);
	Flash_DcacheCmd(Flash_ACC,ENABLE);
}

/**
  * @brief  	Flash Programs  Data at a specified Dest address(data of  length no greater than 1024 bytes)
  * @note 	If Program data to Dest Address,the page of  DestAddress need to be Erased.   (Double Word == 8 bytes)
  * @param[in]  destAddr: Flash target physical address
  * @param[in]  pbData: 	Pointer to the programming Data
  * @param[in]  wDataLen: 	the length of programming Data(no greater than 1024 bytes)
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_ProgramByte(uint32_t destAddr, uint8_t * pbData, uint32_t wDataLen)
{
	uint32_t i;
	uint8_t pageNum = 0;			
	TMC_StatusTypeDef funcRet;		
	
	uint32_t	totalLen = 0;			
	uint32_t	offset = 0;			
	uint32_t	progAddr = destAddr;
	uint32_t	remainLen = wDataLen;
	uint32_t	progLen = 0;
	
	/* Check whether the address space, parameters, etc. conform to the specifications */
	if(	(destAddr < FLASH_ADDR_START2) || 
		(destAddr + wDataLen > FLASH_ADDR_END2) || 
		(pbData == NULL) ||
		(wDataLen > FLASH_WRITE_MAX_NUM))
	{
		return TMC_ERROR;
	}

	/* Judge the data position and length */
	totalLen = MOD_BY_512(destAddr) + wDataLen;
	pageNum = DIV_BY_512(totalLen + 511);
	
	for(i = 0; i < pageNum; i++)
	{
		progLen = (remainLen >= (512 - MOD_BY_512(progAddr)))? (512 - MOD_BY_512(progAddr)) : remainLen;
		funcRet = TMC_FLASH_ProgramInOnePage(progAddr, pbData + offset, progLen);
		offset += progLen;
		remainLen -= progLen;
		progAddr += progLen;
		if(funcRet != TMC_OK)
		{
			return funcRet;
		}
	}
	return TMC_OK;
}


/**
  * @brief  	Programming n-byte data to a specific page of the flash (the address + length cannot exceed the range of one page)
  * @note:	Double Word == 8 bytes
  * @param[in]  destAddr: Flash target physical address
  * @param[in]  pbData: 	Pointer to the programming Data
  * @param[in]  wDataLen: 	the length of programming Data
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_FLASH_ProgramInOnePage(uint32_t destAddr, uint8_t * pbData, uint32_t wDataLen)
{
	Flash_EraseTypeDef eraseMode;
	Flash_ProgramTypeDef programType;
	uint32_t dwDestAddr;	
	uint32_t dwDataLen;		
	uint32_t dwNum;			
	uint32_t headLen;		
	uint32_t tailLen;		
	
	uint32_t 	i = 0;
	uint32_t	pageStartAddr = 0;		
	uint8_t 	isPageNeedErase = 0;	
	TMC_StatusTypeDef funcRet;			
	uint8_t		readbackContent[512 + 1] = {0};
	uint32_t	dataInPageAddr = destAddr;
	uint32_t	dataInPageLen = wDataLen;
	
	/* Check whether the address space, parameters, etc. conform to the specifications */
	if((destAddr < FLASH_ADDR_START2) || (destAddr + wDataLen > FLASH_ADDR_END2) || (pbData == NULL))
	{
		return TMC_ERROR;
	}

	/* Calculate the starting address after double-word alignment and the length after programming by double words*/
	headLen = MOD_BY_8(destAddr);
	dwDestAddr = GET_DW_ADDR(destAddr);
	tailLen = (((7 + destAddr + wDataLen) / 8) * 8);
	tailLen = tailLen - (destAddr + wDataLen);
	dwDataLen = wDataLen + headLen + tailLen;
	dwNum = DIV_BY_8(dwDataLen);

	/* Check the hardware erase flag by double words. */
	for(i = 0; i < dwNum; i++)
	{
		funcRet = TMC_FLASH_HardwareVerify(	dwDestAddr + (i << 3), 
											FLASH_ERASE_VERIFY_MODE_DOUBLEWORD,
											FLASH_ERASE_NOT_VREAD,
											0x10000000);
		if(funcRet != TMC_OK)
		{
			isPageNeedErase = 1;
			break;
		}
	}

	pageStartAddr = GET_PAGE_ADDR(destAddr);
	/* Read the page content */
	memcpy(	&(readbackContent[0]),(uint8_t *)pageStartAddr, 512);
	/* Data merge */
	memcpy(	&(readbackContent[MOD_BY_512(dataInPageAddr)]),	pbData, dataInPageLen);

	if(isPageNeedErase == 1)
	{
		/* Erase flash */
		eraseMode.HardVerify = FLASH_ERASE_NOT_VERIFY;
		funcRet = TMC_FLASH_ErasePage(pageStartAddr,1,&eraseMode,0x10000000);
		if(funcRet != TMC_OK)
		{
			return funcRet;
		}
		/* Reset DCache */
		TMC_Reset_Flash_Dcache();
			
		/* Program the data by half-page */
		programType.HardVerify = FLASH_PROGRAM_NOT_VERIFY;
		programType.ProgramVread = FLASH_PROGRAM_NOT_VREAD;
		funcRet = TMC_FLASH_ProgramHalfPage(pageStartAddr,(uint32_t)readbackContent,32,&programType,0x10000000);
		if(funcRet != TMC_OK)
		{		
			return funcRet;
		}

		funcRet = TMC_FLASH_ProgramHalfPage((pageStartAddr + 256),(uint32_t)(readbackContent+256),32,&programType,0x10000000);
		if(funcRet != TMC_OK)
		{
			return funcRet;
		}
		/* Reset DCache */
		TMC_Reset_Flash_Dcache();
		return TMC_OK;
	}
	else
	{
		/*Check the first half-page programming: The starting address must be the first address and the data length must be greater than half a page.*/
		if((dwDestAddr == pageStartAddr) && (dwDataLen >= 256))
		{
			programType.HardVerify = FLASH_PROGRAM_NOT_VERIFY;
			programType.ProgramVread = FLASH_PROGRAM_NOT_VREAD;
			funcRet = TMC_FLASH_ProgramHalfPage(			pageStartAddr,
												(uint32_t)	readbackContent,
															32,
															&programType,
															0x10000000);
			if(funcRet != TMC_OK)
			{
				return funcRet;
			}
			dwDestAddr 	+= 256;
			dwDataLen	-= 256;
		}
		
		/* Check the second half-page programming: The address needs to be greater than the starting point of half a page and the data length just reaches the end */
		if((dwDestAddr <= pageStartAddr + 256) && (dwDataLen + dwDestAddr == 512))
		{
			programType.HardVerify = FLASH_PROGRAM_NOT_VERIFY;
			programType.ProgramVread = FLASH_PROGRAM_NOT_VREAD;	
			funcRet = TMC_FLASH_ProgramHalfPage(			pageStartAddr + 256, 
												(uint32_t)(	readbackContent + 256),
															32,
															&programType,
															0x10000000);
			if(funcRet != TMC_OK)
			{	
				return funcRet;
			}
			dwDataLen	-= 256;
		}
		
		/* Program Flash by double words */
		if(dwDataLen == 0)
		{
			TMC_Reset_Flash_Dcache();
			funcRet = TMC_OK;
		}
		else
		{
			programType.HardVerify = FLASH_PROGRAM_NOT_VERIFY;
			programType.ProgramVread = FLASH_PROGRAM_NOT_VREAD;				
			funcRet = TMC_FLASH_ProgramDoubleWord(	dwDestAddr, 
										(uint32_t)(	readbackContent + dwDestAddr - pageStartAddr),
													(dwDataLen / 8),
													&programType,
													0x10000000);
			TMC_Reset_Flash_Dcache();
		}
		
		if(funcRet == TMC_OK)
		{
			return TMC_OK;
		}
		else
		{
			return funcRet;
		}
	}
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

