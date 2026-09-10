/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwr.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "rcc.h"
#include "pwr.h"
extern void BAT_WriteEnable(FunctionalState NewState);
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup PWR
  * @brief PWR HAL modules driver 
  * @{
  */
/** @defgroup PWR_private_defines PWR private defines
  * @{
  */


/**
  * @}
  */
/** @defgroup PWR_exported_function PWR exported function
  * @{  
  */
/**
  * @brief  Deinitializes the PWR peripheral registers to their default reset values.	 
  * @param  none
  * @retval none
  * @note none
  */
void PWR_DeInit(void)
{
	RCC_Security1PeriphSoftReset(RCC_SEC1Periph_PWRC);
	while(RCC_Security1PeriphGetResetFlag(RCC_SEC1Periph_PWRC) == RESET);
}

/**
  * @brief Config the Wake up Source ,when chip into Low Power (standby mode)
  * @param[in] source: the Source of Wake up
  *		@arg IWDT_WAKE_UP
  *		@arg RTC_WAKE_UP
  * @retval none
  * @note by calling the function ,you can config the chip wake up source
  */ 
void PWR_ConfigStandbyWakeUpSource(uint32_t source)
{
	assert_param(IS_PWR_POWER_UP_MODE(source));
	PWR_VBAT->WKUPMODE = source;
}

/**
  * @brief when the Chip Wake Up form Standby,Get source of Wake Up
  * @param[in] flag: the flag of Wake Up source
  *		@arg PWR_STS_BAT_READY
  *		@arg PWR_STS_SAND_BY
  *		@arg PWR_STS_IWDT_ALM
  *		@arg PWR_STS_RTC_ALM
  * @retval  FlagStatus SET or RESET
  * @note by calling the function ,you can get the wake up source of chip
  */ 
FlagStatus PWR_GetStandbyStatus(uint32_t flag)
{
	assert_param(IS_PWR_POWER_UP(flag));
	if(PWR_VBAT->WKUPSTS&flag)
	{
		return SET;
	}
	return RESET;
}


/**
  * @brief Clear Standby Status
  * @param[in] flag: clear the flag of Wake Up source
  *		@arg PWR_STS_BAT_READY
  *		@arg PWR_STS_SAND_BY
  *		@arg PWR_STS_IWDT_ALM
  *		@arg PWR_STS_RTC_ALM
  * @retval none
  * @note Clear chip wake up source Flag 
  */ 
void PWR_ClearStandbyStatus(uint32_t flag)
{
	assert_param(IS_PWR_POWER_UP(flag));
	PWR_VBAT->WKUPSTS = flag;	
}

/**
  * @brief Config the Module State,before Chip into Stop Mode 
  * @param[in] mode: 
  *		@arg PWR_STOP_MODE_HSI_PWR_DOWN
  *		@arg PWR_STOP_MODE_FLASH_PWR_DOWN
  *		@arg PWR_STOP_MODE_LDO_PWR_DOWN
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval by calling the function ,you can config chip's HSI ClK,FLash,GPIO's LDO whether into Power down mode,when the chip into Stop mode
  * @note none
  */ 
void PWR_ConfigModuleStateInStopMode(uint32_t mode,FunctionalState NewState)
{
	assert_param(IS_PWR_STOP_MODE(mode));
	if(NewState == ENABLE)
	{
		if(mode&PWR_STOP_MODE_HSI_PWR_DOWN)
		{
			PWR->PWRC|=PWR_STOP_MODE_HSI_PWR_DOWN;
		}
		if(mode&PWR_STOP_MODE_FLASH_PWR_DOWN)
		{
			PWR->PWRC|=PWR_STOP_MODE_FLASH_PWR_DOWN;
		}	
		if(mode&PWR_STOP_MODE_LDO_PWR_DOWN)
		{
			PWR->PWRC|=PWR_STOP_MODE_LDO_PWR_DOWN;
		}	
	}
	else
	{
		if(mode&PWR_STOP_MODE_HSI_PWR_DOWN)
		{
			PWR->PWRC&=~PWR_STOP_MODE_HSI_PWR_DOWN;
		}
		if(mode&PWR_STOP_MODE_FLASH_PWR_DOWN)
		{
			PWR->PWRC&=~PWR_STOP_MODE_FLASH_PWR_DOWN;
		}	
		if(mode&PWR_STOP_MODE_LDO_PWR_DOWN)
		{
			PWR->PWRC&=~PWR_STOP_MODE_LDO_PWR_DOWN;
		}	
	}
}

/**
  * @brief  Enters STOP mode.  
  *	@note   the Function will Power Down HSI,Falsh and Not Under Driver
  * @param  PWR_STOPEntry: specifies if STOP mode in entered with WFI or WFE instruction.
  * This parameter can be one of the following values:
  *		@arg PWR_STOPEntry_WFI: enter STOP mode with WFI instruction
  *	 @arg PWR_STOPEntry_WFE: enter STOP mode with WFE instruction
  * @retval None
  */
void PWR_EnterStopMode(uint8_t StopEntry)
{
	assert_param(IS_PWR_STOP_ENTRY(StopEntry));
	EXTI->PR0 = 0xffffffff;
	EXTI->PR1 = 0xffffffff;
	EXTI->PR2 = 0xffffffff;
	EXTI->PR3 = 0xffffffff;
	/* Select Standby mode */
	PWR->PWRC &= PWOR_DWON_STOP_MODE;
	/* Set SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	if(StopEntry == PWR_STOPEntry_WFI)
	{
		__WFI();
	}
	else
	{
		/* Request Wait For Event */
		__SEV();
		__WFE();
		__WFE();
	}
	/* Reset SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR &= (uint32_t)~((uint32_t)SCB_SCR_SLEEPDEEP_Msk); 
}
/**
  * @brief  Enters STANDBY mode.
  * @note   The Wakeup flag (WUF) need to be cleared at application level before to call this function 
  * @param  None
  * @retval None
  */
void PWR_EnterStandbyMode(void)
{

	PWR->PWRC = PWOR_DWON_SANDBY_MODE;
	/* Set SLEEPDEEP bit of Cortex System Control Register */
	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;
	/* Request Wait For Interrupt */
	__WFI();
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


