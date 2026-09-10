/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: flash.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "flash.h"
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
/** @defgroup FLASH_private_defines FLASH private defines
  * @{
  */

#define	FLASH_START_VALUE					(0xFFFFFFFFUL)
#define FLASH_ERASE_PAGE					(0x00000005UL)		/*!< FLASH Erase Mode is page erase*/
#define FLASH_ERASE_VERIFY_MODE_DOUBLEWORD	(0x30000000UL)		/*!< FLASH Erase Verify Mode is DoubleWord*///(((1UL)<<29)|((1UL)<<28))
#define FLASH_ERASE_VERIFY_MODE_PAGE		(0x10000000UL)		/*!< FLASH Erase Verify Mode is Page*/
#define FLASH_PROGRAM_DOUBLEWORD_MODE		(0x00000100UL)		/*!< FLASH Program Mode is double word */ 
#define FLASH_PROGRAM_BUFFER_MODE			(0x00000400UL)		/*!< FLASH Program Mode is Progbuffer word*/ 
/**
  * @}
  */
/** @defgroup FLASH_exported_function FLASH exported function
  * @{  
  */
  

/**
  * @brief  Program a double word (64-bit) at a specified address.
  * @note   If an erase and a program operations are requested simultaneously,	
  *		 the erase operation is performed before the program one. 
  * @param[in]  Address: specifies the address to be programmed.
  * @param[in]  Data: specifies the data to be programmed.
  * @param[in]  Flash_ProgramTypeDef:pointer to a Flash_ProgramTypeDef
				structure that contains the program Verify mode for the Flash.
  * @retval None
  */
void FLASH_ProgramDoubleWord(uint32_t Address, uint64_t Data,Flash_ProgramTypeDef* Program)
{
	/* Check the point of Program whether is NULL */ 
	assert_param(IS_POINT_NULL(Program));
	/* Check the Address of Program whether is out Flash or UserOTP range */ 
	assert_param(IS_FLASH_OR_UserOTP_ADDRESS(Address));
	/* Check the Address of Program whether is 64 bit alignment */ 
	assert_param(IS_FLASH_CHECK_U64_ADDRESS(Address));
	/* Check the Program->HardVerify whether is undefine */ 
	assert_param(IS_FLASH_PROGRAM_VERIFY(Program->HardVerify));
	/* Check the Program->ProgramVread whether is undefine */ 
	assert_param(IS_FLASH_PROGRAM_Vread(Program->ProgramVread));
	/* Set the program mode */ 
	Flash->FLCON = FLASH_PROGRAM_DOUBLEWORD_MODE|Program->HardVerify|Program->ProgramVread;
	/* Set the Flash operation is program and Start up */ 
	//Flash_WriteStart();
	Flash->FLSDP1 = 0xAA;
	Flash->FLSDP2 = 0x55;
	*((uint32_t*)(Address)) = (uint32_t)Data;
  *((uint32_t*)(Address + 4)) = (uint32_t)(Data >> 32);
}

/**
  * @brief  Erase one pager of Flash.
  * @note   If an erase and a program operations are requested simultaneously,	
  *		 the erase operation is performed before the program one. 
  * @param[in]  Address: specifies the address to be Erased.
  * @param[in]  Flash_EraseTypeDef:pointer to a Flash_EraseTypeDef
				structure that contains the Erase Verify mode for the Flash.
  * @retval None
  */ 
void Flash_ErasePage(uint32_t Adder,Flash_EraseTypeDef* Erase)
{
	/* Check the point of Erase whether is NULL */ 
	assert_param(IS_POINT_NULL(Erase));
	assert_param(IS_FLASH_ERASE_MODE(Erase->EraseMode));
	/* Check the Erase->HardVerify whether is undefine */ 
	assert_param(IS_FLASH_ERASE_VERIFY(Erase->HardVerify));
	/* Check the Erase->EraseVread whether is undefine */ 
	assert_param(IS_FLASH_ERASE_Vread(Erase->EraseVread));
	/* Set the Erase mode */ 
	Flash->FLCON = FLASH_ERASE_PAGE|Erase->EraseMode|Erase->EraseVread|Erase->HardVerify;
	/* Set the Flash operation is Erase and Start up */ 
	Flash->FLSDP1 = 0x55;
	Flash->FLSDP2 = 0xAA;
	*(uint32_t *)Adder = 0xFFFFFFFF;  
}

/**
  * @brief  Program a double word (64-bit) at a specified address.
  * @note   If an erase and a program operations are requested simultaneously	
  *		 the erase operation is performed before the program one. 
  * @param[in]  DestAddress: Dest the address to be programmed.
  * @param[in]  SourAddress: Sour the address of program.
  * @param[in]	Length: the length of program,Min:1 Max:64(Unit: double Word)
  * @param[in]  Flash_ProgramTypeDef:pointer to a Flash_ProgramTypeDef
				structure that contains the program Verify mode for the Flash.
  * @retval None
  */
void FLASH_ProgramHalfPage(uint32_t DestAddress, uint32_t SourAddress,uint8_t Length,Flash_ProgramTypeDef* Program)
{
	uint32_t i;
	/* Check the point of Program whether is NULL */ 
	assert_param(IS_POINT_NULL(Program));
	/* Check the DestAddress whether is start Address of HalfPage */ 
	assert_param(IS_ADDRESS_ALIGNMENT(DestAddress));
	/* Check the DestAddress whether is out of Flash or UserOTP range */ 
	assert_param(IS_FLASH_OR_UserOTP_ADDRESS(DestAddress));
	/* Check the Length is between 1 and 64 */ 
	assert_param(IS_FLASH_PROGRAM_LENGTH(Length));
	assert_param(IS_FLASH_PROGRAM_VERIFY(Program->HardVerify));
	assert_param(IS_FLASH_PROGRAM_Vread(Program->ProgramVread));
	/* Set the Flash operation is program Buffer */ 
	Flash->FLSDP1 = 0x33;
	Flash->FLSDP2 = 0xCC;
	for(i=0;i<Length;i++)
	{
			*((uint32_t *)(DestAddress + (i * 8))) = *((uint32_t *)(SourAddress + (i * 8)));
			*((uint32_t *)(DestAddress + 4 + (i * 8))) = *((uint32_t *)(SourAddress + 4 + (i * 8)));
	}
	/* Set the program mode*/ 
	Flash->FLCON = FLASH_PROGRAM_BUFFER_MODE|Program->HardVerify|Program->ProgramVread;
	
	/* Set the Flash operation is program and Start up */ 
	Flash->FLSDP1 = 0xAA;
	Flash->FLSDP2 = 0x55;
	*((uint32_t*)(DestAddress)) = FLASH_START_VALUE;
}

/**
  * @brief  Verify the Pager whether is Erased
  * @note   when chip is Erasing,the chip Power off. and it need to enable Vread to determine whether flash erase success
  * @param[in]  Address:  address to be Verified.
  * @param[in]  VerifyMode:  Flash Pager verify mode
  *			@arg FLASH_ERASE_VERIFY_MODE_DOUBLEWORD
  *			@arg FLASH_ERASE_VERIFY_MODE_PAGE
  * @param[in]  Vread:  Erase Vread mode
  *			@arg FLASH_ERASE_VREAD
  *			@arg FLASH_ERASE_NOT_VREAD
  * @retval None
  */
void FLASH_HardVerify(uint32_t Address,uint32_t VerifyMode,uint32_t Vread)
{
	/* Check the Address of hardwareVerify whether is out Flash or UserOTP range */
	assert_param(IS_FLASH_OR_UserOTP_ADDRESS(Address));
	/* Check the Erase->HardVerify whether is undefine */ 
	assert_param(IS_FLASH_ERASE_VERIFY_MODE(VerifyMode));
	/* Check the Erase->EraseVread whether is undefine */ 
	assert_param(IS_FLASH_ERASE_Vread(Vread));
	Flash->FLCON = VerifyMode|Vread;
	/* Set the Flash operation is hardverify and Start up */ 
	Flash->FLSDP1 = 0xAA;
	Flash->FLSDP2 = 0xAA;
	*((uint32_t*)(Address)) = FLASH_START_VALUE; 
}

/**
  * @brief  Checks whether the specified FLASH flag is set or not.
  * @param  FLASH_FLAG: specifies the FLASH flag to check.
  *   This parameter can be one of the following values:
  *	 @arg FLASH_FLAG_OPERR: 	the Value of SDP1 or SDP2 is Error		  
  *	 @arg FLASH_FLAG_OVER:  	the over Flag of Flash ,SET is FLash operation is finish, Reset is FLash operation is not finish
  *	 @arg FLASH_FLAG_EVERR: 	Erase Err Flag of Flash	  
  *	 @arg FLASH_FLAG_PVERR: 	Program Verify Err Flag of Flash		 
  *	 @arg FLASH_FLAG_HVERR:  Erase Verify Err Flag of Flash	 
  * @retval The new state of FLASH_FLAG (SET or RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint32_t FLASH_FLAG)
{
	/* Check the parameters */ 
	assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG));
	if((Flash->FLSTS)&FLASH_FLAG)
	{
		return SET;
	}
	else if((Flash->FLSTS1)&FLASH_FLAG)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clears the FLASH flags.
  * @param  FLASH_FLAG: specifies the FLASH flags to clear.
  *   This parameter can be any combination of the following values:		 
  *	 @arg FLASH_FLAG_OPERR: 	the Value of SDP1 or SDP2 is Error		  
  *	 @arg FLASH_FLAG_OVER:  	the over Flag of Flash ,SET is FLash operation is finish, Reset is FLash operation is not finish
  *	 @arg FLASH_FLAG_EVERR: 	Erase Err Flag of Flash	  
  *	 @arg FLASH_FLAG_PVERR: 	Program Verify Err Flag of Flash		 
  *	 @arg FLASH_FLAG_HVERR:  Erase Verify Err Flag of Flash			 
  * @retval None
  */
void FLASH_ClearFlag(uint32_t FlashFlag)
{
	/* Check the parameters */
	assert_param(IS_FLASH_GET_FLAG(FlashFlag));
	/* Clear the flags */
	
	Flash->FLSTS |= ((FlashFlag & FLASH_FLAG_OVER) | (FlashFlag & FLASH_FLAG_OPERR));
	Flash->FLSTS1 |= ((FlashFlag & FLASH_FLAG_EVERR) | (FlashFlag & FLASH_FLAG_PVERR) | (FlashFlag & FLASH_FLAG_HVERR));
}

/**
  * @brief  Enables or disables the specified FLASH interrupts.
  * @param  FLASH_IT: specifies the FLASH interrupt sources to be enabled or disabled.
  *   This parameter can be any combination of the following values:
  *	 @arg FLASH_IT_MASK_OVER: FLASH end of operation Interrupt
  *	 @arg FLASH_IT_MASK_HV_ERR: FLASH Erase Error Interrupt
  *	 @arg FLASH_IT_MASK_PV_ERR: FLASH after program,Verify Error Interrupt 
  *	 @arg FLASH_IT_MASK_EV_ERR: FLASH after Erase,Verify Error Interrupt 
  * @param  NewState: new state of the specified Flash interrupts.
  *   This parameter can be: ENABLE or DISABLE.	  
  * @retval None 
  */

void Flash_ConfigIT(uint32_t FLASH_IT,FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_FLASH_IT(FLASH_IT));
	
	if(NewState == DISABLE)
	{
		Flash->FLMSK = FLASH_IT;
		Flash->FLMSK1 = FLASH_IT;
	}
	else
	{
		Flash->FLMSK &= (~FLASH_IT);
		Flash->FLMSK1 &= (~FLASH_IT);
	}
}




/**
  * @brief when the Flash Fast Erase mode,config the Erase Try number
  * @param[in] TryTimes:the Number of Fast Erase try times ,the value is 0,1,2,3		
  * @retval None
  * @note None
  */ 
void Flash_ConfigFastEraseTryTimes(uint32_t TryTimes)
{
	/* Check the parameters */
	assert_param(IS_FLASH_ERASE_TRY(TryTimes));
	
	Flash->FLCON&=FLASH_ERASE_TRY_0;
	if(TryTimes == 1)
	{
		Flash->FLCON|=FLASH_ERASE_TRY_1;
	}
	else if(TryTimes == 2)
	{
		Flash->FLCON|=FLASH_ERASE_TRY_2;
	}
	else if(TryTimes == 3)
	{
		Flash->FLCON|=FLASH_ERASE_TRY_3;
	}
}

/**
  * @brief Config Flash operation mode
  * @param[in] operation: Flash operation mode
  *   This parameter can be any combination of the following values: 
  *	 @arg  FLASH_EXCUTE_CPU_IDLE	   
  *	 @arg  FLASH_EXCUTE_CPU_NOT_IDLE
  * @retval None
  * @note None
  */ 
void Flash_ConfigFlashOperation(uint32_t operation)
{
	/* Check the parameters */
	assert_param(IS_FLASH_ERASE_TRY(operation));
	Flash->FLEPMODE = operation;
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
