/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: wdt.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_conf.h"
#include "RCC.h"
#include "WDT.h"

/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup WDT WDT
  * @brief WDT HAL modules driver 
  * @{
  */ 
/** @defgroup WDT_Private_Macros WDT Private Macros
  * @{
  */
/** @brief  Lock WDT
  * @param  __TYPE__: specifies the WDT Type.
  * @retval none
  */	
#define __WDT_LOCK(__TYPE__)				((__TYPE__)->LOCK &= 0x00000001)

/** @brief  Unlock WDT
  * @param  __TYPE__: specifies the WDT Type.
  * @retval none
  */
#define __WDT_UNLOCK(__TYPE__)				((__TYPE__)->LOCK |= 0x1ACCE551)

/** @brief  Lock Status
  * @param  __TYPE__: specifies the WDT Type.
  * @retval status
  * 		SET: WDT has been locked
  * 		RESET: WDT is not locked
  */	
#define __WDT_LOCKSTS(__TYPE__)				(((__TYPE__)->LOCK & 0x01) != RESET)
/**
  * @}
  */ 

/** @defgroup WDT_Private_Defines WDT Private Defines
  * @{
  */

/* ---------------------- WDT registers bit mask ------------------------ */

/* WDTCONTROL register bit mask */
#define WDT_COUNTER_EN_Pos			(0U)									 
#define WDT_COUNTER_EN_Msk			(0x1UL << WDT_COUNTER_EN_Pos)		/*!< 0x00000001 */
#define WDT_COUNTER_EN				WDT_COUNTER_EN_Msk					/*!<Enable WDT counter and interrupt*/

#define WDT_SET_MODE_Pos			(1U)									 
#define WDT_SET_MODE_Msk			(0x1UL << WDT_SET_MODE_Pos)				/*!< 0x00000002 */
#define WDT_SET_MODE				WDT_SET_MODE_Msk						/*!<Enable output reset single		*/

/* WDTRIS register bit mask */
#define WDT_COUNTER_STS_Pos			(0U)									 
#define WDT_COUNTER_STS_Msk			(0x1UL << WDT_COUNTER_STS_Pos)			/*!< 0x00000001 */
#define WDT_COUNTER_STS				WDT_COUNTER_STS_Pos						/*!<Status of the counter			*/

/* WDTMIS register bit mask */
#define WDT_INT_STS_Pos				(0U)									 
#define WDT_INT_STS_Msk				(0x1UL << WDT_INT_STS_Pos)				/*!< 0x00000001 */
#define WDT_INT_STS					WDT_INT_STS_Msk							/*!<Status of the interrupt			*/

/**
  * @}
  */ 


/** @defgroup WDT_Exported_Functions WDT Exported Functions
  * @{
  */  

/**
  * @brief   Initializes the InitStruct of UARTx
  * @param[in]	UART_InitStruct: pointer to a UART_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
__weak void WDT_StructInit(WDT_InitTypeDef * init)
{
	init->Loadvalue = 0xFFFFFFFF;
	init->Mode = WDT_MODE_INT;
}
/**
  * @brief  Deinitializes the WDT registers to their reset values.
  * @param none
  * @retval none
  * @note
  */
void WDT_DeInit(void)
{
	RCC_APB2PeriphSoftReset(RCC_APB2Periph_WDT);
	while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_WDT)== RESET);
}

/**
  * @brief  Initializes the WDT according to the specified	 
  * @param[in]  Load_Value: WDT initial value
  * @retval none
  * @note
  */
void WDT_Init(WWDT_TypeDef *WDT, WDT_InitTypeDef * init)
{
	/* Check the parameters */
	assert_param(IS_WDT_LOADVALUE(init->Loadvalue));
	
	__WDT_UNLOCK(WWDT);

	/* Set mode 		*/
	WDT->CONTROL &= ~WDT_SET_MODE;
	
	WDT->CONTROL |= init->Mode;
	
	/* Load initial value */
	WDT->LOAD = init->Loadvalue;
	
	__WDT_LOCK(WWDT);
}


/**
  * @brief  Enable or disable WDT counter.
  * @param[in] NewState: new state of the specified WDT
  * 	This parameter can be one of the following values: 
  *		@arg DISABLE :disable WDT
  *	 @arg ENABLE  :enable WDT
  * @retval none
  * @note	none
  */
void WDT_Cmd(WWDT_TypeDef *WDT, FunctionalState NewState)
{
	__WDT_UNLOCK(WWDT);
	if (NewState != DISABLE)
	{
		WDT->CONTROL |= WDT_COUNTER_EN;
	}
	else
	{
		WDT->CONTROL &= ~WDT_COUNTER_EN;
	}
	__WDT_LOCK(WWDT);
}

/**
  * @brief  Read WDT counter status
  * @param	none
  * @retval FlagStatus
  *		@arg SET : Flag Stutas is Set
  *	 @arg RESET : Flag Stutas is Reset
  * @note	none
  */ 
FlagStatus WDT_GetFlagStatus(WWDT_TypeDef *WDT)
{	
	if ((WDT->RIS & WDT_COUNTER_STS) != RESET)
	{
		return SET;
	}
	return RESET;
}


/**
  * @brief  Read UART interrupt status
  * @param	none
  * @retval FlagStatus
  *		@arg SET : IT Stutas is Set
  *	 @arg RESET : IT Stutas is Reset
  * @note	none
  */ 
FlagStatus WDT_GetITStatus(WWDT_TypeDef *WDT)
{
	if ((WDT->MIS & WDT_INT_STS) != RESET)
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Clear WDT Flag
  * @param	none
  * @retval none
  * @note	none
  */
void WDT_ClearFlag(WWDT_TypeDef *WDT)
{
	__WDT_UNLOCK(WWDT);
	WDT->INTCLR = 0x01;
	__WDT_LOCK(WWDT);
}

/**
  * @brief  Clear WDT IT Flag
  * @param	none
  * @retval none
  * @note	none
  */
void WDT_ClearITFlag(WWDT_TypeDef *WDT)
{
	__WDT_UNLOCK(WWDT);
	WDT->INTCLR = 0x01;
	__WDT_LOCK(WWDT);
}
/**
  * @brief  Read current WDT value
  * @param	none
  * @retval Value: current counter value
  * @note	none
  */
uint32_t WDT_ReadCurrentValue(WWDT_TypeDef *WDT)
{
	return (uint32_t)WDT->VALUE;
}

/**
  * @brief  Feed watch dog
  * @param[in]  Load_Value: WDT initial value
  * @retval none
  * @note	none
  */
void WDT_FeedDog(WWDT_TypeDef *WDT, WDT_InitTypeDef * init)
{
	__WDT_UNLOCK(WWDT);
	WDT->LOAD = init->Loadvalue;
	__WDT_LOCK(WWDT);
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
