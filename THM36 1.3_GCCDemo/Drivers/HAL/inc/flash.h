/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: flash.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

#include "stdint.h"
#ifdef __cplusplus
 extern "C" {
#endif

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup FLASH
  * @brief FLASH HAL modules driver 
  * @{
  */ 
/* Exported constants --------------------------------------------------------*/
/** @defgroup FLASH_Exported_Constants FLASH Exported Constants
  * @{
  */

/** 
  * @brief  Flash Program Init structure definition
  * @note   This sturcture is used with  Flash   
  */
typedef struct
{
	uint32_t HardVerify;		/*!< Flash HardVerify function,This parameter can be a value of @ref FLASH_program_Verify_mode*/
	uint32_t ProgramVread;		/*!< Flash ProgramVread function,This parameter can be a value of @ref FLASH_program_Vread_mode**/	 					
} Flash_ProgramTypeDef; 	

/** 
  * @brief  Flash Erase Init structure definition
  * @note   This sturcture is used with  Flash   
  */
typedef struct
{
	uint32_t EraseMode;			/*!< Flash Erase Mode is Fast Erase and Normal Erase,This parameter can be a value of @ref FLASH_erase_mode*/
	uint32_t HardVerify;		/*!< Flash Erase HardVerify function,This parameter can be a value of @ref FLASH_Erase_HardVerify_mode*/
	uint32_t EraseVread;		/*!< Flash Erase ProgramVread function,This parameter can be a value of @ref FLASH_Erase_Vready_mode*/	 
					
} Flash_EraseTypeDef; 
 
/** @defgroup FLASH_IT_flag FLASH IT flag
  * @brief   
  * @{
  */ 
#define FLASH_IT_MASK_OVER					(0x00000001UL)		/*!< IT of FLASH over of Operation flag */
#define FLASH_IT_MASK_HV_ERR	  			(0x00000010UL)		/*!< IT of FLASH Erase verify   */ 
#define FLASH_IT_MASK_PV_ERR	  			(0x00000008UL)		/*!< IT of FLASH Program Hardware Verify error flag*/ 
#define FLASH_IT_MASK_EV_ERR				(0x00000004UL)		/*!< IT of FLASH Erase Hardware Verify error flag */ 

/**
  * @}
  */
  
/** @defgroup FLASH_Flags 
  * @{
  */
#define FLASH_FLAG_OPERR					(0x00000002UL)  	/*!< FLASH SPD Error flag */
#define FLASH_FLAG_OVER						(0x00000001UL)  	/*!< FLASH over of Operation flag */
#define FLASH_FLAG_EVERR					(0x00000004UL)  	/*!< FLASH Erase verify*/
#define FLASH_FLAG_PVERR					(0x00000008UL)  	/*!< FLASH Program Hardware Verify error flag */
#define FLASH_FLAG_HVERR					(0x00000010UL)  	/*!< FLASH Erase Hardware Verify error flag ,if Erase mode HardVervify is FLASH_PROGRAM_NOT_VERIFY,this flag not be Set */

/**
  * @}
  */ 
/** @defgroup FLASH_program_Verify_mode  FLASH program Verify mode
  * @{
  */  
#define FLASH_PROGRAM_VERIFY				(0x00004000UL)		/*!< FLASH Program Verify Enable*/
#define FLASH_PROGRAM_NOT_VERIFY			(0x00000000UL)		/*!< FLASH Program Verify Disable*/

/**
  * @}
  */
/** @defgroup FLASH_program_Vread_mode  FLASH program Vread mode
  * @{
  */ 
#define FLASH_PROGRAM_VREAD					(0x00008000UL)		/*!< FLASH Program Verify Vread Enable*/
#define FLASH_PROGRAM_NOT_VREAD				(0x00000000UL)		/*!< FLASH Program Verify Vread Disable*/
/**
  * @}
  */
/** @defgroup FLASH_Operation_Mode
  * @{
  */  
#define FLASH_EXCUTE_CPU_IDLE				(0x00000001UL)		/*!< FLASH Excute mode is when CPU into Idle,Flash Erase or Program Start-up*/
#define FLASH_EXCUTE_CPU_NOT_IDLE			(0x00000000UL)		/*!< FLASH Excute mode is when CPU not into Idle,Flash Erase or Program Start-up*/

/**
  * @}
  */
/** @defgroup FLASH_erase_mode 
  * @{
  */  

#define FLASH_ERASE_FAST					(0x00000010UL)		/*!< FLASH Erase mode is Fast mode*/
#define FLASH_ERASE_NORMAL					(0x00000000UL)		/*!< FLASH Erase mode is Normal mode*/

/**
  * @}
  */
/** @defgroup FLASH_Erase_HardVerify_mode FLASH Erase HardVerify mode
  * @{
  */ 
#define FLASH_ERASE_VERIFY					(0x00000040UL)		/*!< FLASH Erase Verify Enable1UL<<6*/
#define FLASH_ERASE_NOT_VERIFY				(0x00000000UL)		/*!< FLASH Erase Verify Disable*/
/**
  * @}
  */
  
/** @defgroup FLASH_Erase_Vready_mode FLASH Erase Vready mode
  * @{
  */ 
#define FLASH_ERASE_VREAD					(0x00000080UL)		/*!< FLASH Erase Vread Enable*/
#define FLASH_ERASE_NOT_VREAD				(0x00000000UL)		/*!< FLASH Erase Vread Disable*/ 
/**
  * @}
  */
/** @defgroup  Flash_operation_mode  Flash operation mode
  * @{
  */  
#define FLASH_ERASE_TRY_0					(~(((1UL)<<21)|((1UL)<<20)))	/*!< FLASH Erase mode is Fast Mode,when Erase fail,hardware retry 0 time*/
#define FLASH_ERASE_TRY_1					((1UL)<<20)						/*!< FLASH Erase mode is Fast Mode,when Erase fail,hardware retry 1 time*/
#define FLASH_ERASE_TRY_2					((1UL)<<21)						/*!< FLASH Erase mode is Fast Mode,when Erase fail,hardware retry 2 time*/
#define FLASH_ERASE_TRY_3					(((1UL)<<20)|((1UL)<<21))		/*!< FLASH Erase mode is Fast Mode,when Erase fail,hardware retry 3 time*/
/**
  * @}
  */
/** @defgroup FLASH_Address_Range
  * @{
  */
#define FLASH_STRAT_ADDRESS					(0x10000000UL)		/*!< FLASH Start Address*/
#define FLASH_END_ADDRESS					(0x1007FFFFUL)		/*!< FLASH End Address*/

/**
  * @}
  */
/** @defgroup User OTP_Address_Range
  * @{
  */
#define UserOTP_STRAT_ADDRESS				(0x1F000300UL)		/*!< User OTP Start Address*/
#define UserOTP_END_ADDRESS					(0x1F0003FFUL)		/*!< User OTP End Address*/

/**
  * @}
  */
/** @defgroup Chip ID
  * @{
  */
#define CHIP_ID_ADDRESS				        (0x1F000008UL)		/*!< Chip ID Address*/

/**
  * @}
  */
  
/** @defgroup FLASH_private_macros FLASH private macros
  * @{
  */
#define IS_ADDRESS_ALIGNMENT(ADDRESS)		(!(ADDRESS%0x100))
//#define IS_POINT_NULL(POINT)				(((uint32_t)POINT)!=NULL) 
#define IS_FLASH_PROGRAM_LENGTH(LENGTH)		((LENGTH>1)||(LENGTH<=64))  
#define IS_FLASH_ERROR(ERROR)				((ERROR&HAL_FLASH_ERROR_NONE)||(ERROR&HAL_FLASH_ERROR_RD)|| \
											(ERROR&HAL_FLASH_ERROR_PGS)||(ERROR&HAL_FLASH_ERROR_PGP)|| \
											(ERROR&HAL_FLASH_ERROR_PGA)||(ERROR&HAL_FLASH_ERROR_WRP)|| \
											(ERROR&HAL_FLASH_ERROR_OPERATION))
											
#define IS_FLASH_IT(IT)						((IT&FLASH_IT_MASK_OVER)||(IT&FLASH_IT_MASK_HV_ERR)|| \
											(IT&FLASH_IT_MASK_PV_ERR)||(IT&FLASH_IT_MASK_EV_ERR))
											
#define IS_FLASH_GET_FLAG(FLAG)				(((FLAG) & FLASH_FLAG_OPERR) || ((FLAG) & FLASH_FLAG_OVER) || \
											((FLAG) & FLASH_FLAG_EVERR) || ((FLAG) & FLASH_FLAG_PVERR) || \
											((FLAG) & FLASH_FLAG_HVERR))
											
#define IS_FLASH_ERASE_MODE(FLAG)			(((FLAG) == FLASH_ERASE_FAST) || ((FLAG) == FLASH_ERASE_NORMAL))
#define IS_FLASH_ERASE_VERIFY(FLAG)			(((FLAG) == FLASH_ERASE_VERIFY) || ((FLAG) == FLASH_ERASE_NOT_VERIFY))

#define IS_FLASH_ERASE_Vread(FLAG)			(((FLAG) == FLASH_ERASE_VREAD) || ((FLAG) == FLASH_ERASE_NOT_VREAD))

#define IS_FLASH_PROGRAM_VERIFY(FLAG)		(((FLAG) == FLASH_PROGRAM_VERIFY) || ((FLAG) == FLASH_PROGRAM_NOT_VERIFY))

#define IS_FLASH_PROGRAM_Vread(FLAG)		(((FLAG) == FLASH_PROGRAM_VREAD) || ((FLAG) == FLASH_PROGRAM_NOT_VREAD))

#define IS_FLASH_ERASE_TRY(FLAG)			(((FLAG) == FLASH_ERASE_TRY_0)||((FLAG) == FLASH_ERASE_TRY_1) || \
											((FLAG) == FLASH_ERASE_TRY_2)||((FLAG) == FLASH_ERASE_TRY_3))
											
#define IS_FLASH_ERASE_VERIFY_MODE(FLAG)  	(((FLAG) == FLASH_ERASE_VERIFY_MODE_DOUBLEWORD) || ((FLAG) == FLASH_ERASE_VERIFY_MODE_PAGE))

#define IS_FLASH_ERASE_VREAD(FLAG)  		(((FLAG) == FLASH_ERASE_VREAD) || ((FLAG) == FLASH_ERASE_NOT_VREAD))

#define IS_FLASH_OR_UserOTP_ADDRESS(ADRESS)			(((ADRESS>=FLASH_STRAT_ADDRESS)&&(ADRESS<=FLASH_END_ADDRESS)) || ((ADRESS>=UserOTP_STRAT_ADDRESS)&&(ADRESS<=UserOTP_END_ADDRESS))) 

#define IS_FLASH_CHECK_U32_ADDRESS(ADRESS)		(!(ADRESS&0x03)) 

#define IS_FLASH_CHECK_U64_ADDRESS(ADRESS)		(!(ADRESS&0x07))

#define IS_UserOTP_ADDRESS(ADRESS)			

#define CHIP_IS_THM36V13			        0x45

#define BootLoderVersion 					(*(volatile unsigned char*)(0x1007FE24UL))
	
#define CHIPID 								(*(volatile unsigned char*)(0x1F000008UL))
	
/**
  * @}
  */
/** @addtogroup FLASH_exported_function
  * @{
  */
void FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data,Flash_ProgramTypeDef* Program);
void Flash_ErasePage(uint32_t Adder,Flash_EraseTypeDef* Erase);
void FLASH_ProgramHalfPage(uint32_t DestAddress, uint32_t SourAddress,uint8_t Length,Flash_ProgramTypeDef* Program);
void FLASH_HardVerify(uint32_t Address,uint32_t VerifyMode,uint32_t Vread);
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG);
void FLASH_ClearFlag(uint32_t FLASH_FLAG);
void Flash_ConfigIT(uint32_t FLASH_IT,FunctionalState NewState);
void Flash_ConfigFastEraseTryTimes(uint32_t TryTimes);
void Flash_ConfigFlashOperation(uint32_t operation);
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

#endif   

