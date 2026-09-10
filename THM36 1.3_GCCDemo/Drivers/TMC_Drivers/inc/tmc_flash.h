/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_flash.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef _TMC_FLASH_H
#define _TMC_FLASH_H

#ifdef __cplusplus
 extern "C" {
#endif
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
/** @defgroup TMC_FLASH_exported_Types FLASH exported types
  * @{
  */
///** @defgroup TMC_FLASH_state TMC FLASH state
//  * @{
//  */
//	 
//typedef enum
//{ 
//  FLASH_STATE_ERROR_NONE	   	   =  0,	/*!< Flash No error */	 
//  FLASH_STATE_ERROR_VERIFY		   = -1,	/*!< Flash Erase Verify error */ 
//  FLASH_STATE_ERROR_OPERATION	   = -2,	/*!< Flash Operation Flag error */ 
//  FLASH_STATE_ERROR_Vread		   = -3,	/*!< Flash Vread error */
//  FLASH_STATE_ERROR_ADDRESS_OUT	= -4,	/*!< Flash Address error */
//  FLASH_STATE_OTHER_ERR			   = -5,	/*!< Flash other error */
//}Flash_StateTypeDef;
///**
//  * @}
//  */

///** 
//  * @brief  FLASH handle Structure definition  
//  */
//typedef struct
//{ 

//	uint8_t BusyStatus;
//	Flash_StateTypeDef  ErroStatus;
//	uint8_t *pEraseAddress;
//	uint16_t ErasePageNum;
//	uint16_t EraseCount; 
//	uint8_t *pWriteSrcBuffPtr;
//	uint8_t *pWriteDestBuffPtr;
//	uint32_t WriteLength; 
//	uint32_t WriteCount;   
//}Flash_HandleTypeDef;

///**
//  * @}
//  */

/** @defgroup TMC_flash_busy_state
  * @{
  */
#define FLASH_NO_BUSY		0
#define FLASH_BUSY			1
/**
  * @}
  */
/** @addtogroup TMC_FLASH_Exported_Function 
  * @brief FLASH TMC modules driver
  * @{
  */
void TMC_FLASH_Msp_Init(void);
TMC_StatusTypeDef TMC_FLASH_ErasePage(uint32_t Address,uint32_t Nbpage,Flash_EraseTypeDef *EraseMode,uint32_t TimeOut);
TMC_StatusTypeDef TMC_FLASH_ProgramDoubleWord(uint32_t DestAddress,uint32_t SourAddress,uint32_t Length,Flash_ProgramTypeDef *Program,uint32_t TimeOut);
TMC_StatusTypeDef TMC_FLASH_ProgramHalfPage(uint32_t DestAddress,uint32_t SourAddress,uint32_t Length,Flash_ProgramTypeDef *Program,uint32_t TimeOut);
TMC_StatusTypeDef TMC_FLASH_HardwareVerify(uint32_t Address,uint32_t VerifyMode,uint32_t Vread,uint32_t TimeOut);
void TMC_Flash_EraseStructInit(Flash_EraseTypeDef *EraseMode);
void TMC_Flash_ProgramStructInit(Flash_ProgramTypeDef *ProgramMode);

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

#endif /* _TMC_FLASH_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/



