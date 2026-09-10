/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_exti.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"	
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "pwr_demomain.h"
#include "tmc_exti.h"


/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_EXTI
  * @brief EXTI TMC modules driver 
  * @{
  */
/** @defgroup TMC_EXTI_Exported_Function
  * @brief EXTI TMC modules driver
  * @{
  */

/**
  * @brief GPIOA EXTI IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_GPIOA_IRQHandler(void)
{
	uint8_t num;
	EXTI_State_GPIO_A = 0;
	for(num=0;num<16;num++)
	{
		if(EXTI_GetPINxStatus(EXTI_GPIOA,1<<num)== SET)
		{
			EXTI_State_GPIO_A |= 1<<num;
			EXTI_ClearITPendingBit(EXTI_GPIOA,1<<num);
		}
	}
	TMC_EXTI_CpltCallback_GPIOA(EXTI_State_GPIO_A);
}


/**
  * @brief GPIOB EXTI IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_GPIOB_IRQHandler(void)
{
	uint8_t num;
	EXTI_State_GPIO_B = 0;
	for(num=0;num<16;num++)
	{
		if(EXTI_GetPINxStatus(EXTI_GPIOB,1<<num)== SET)
		{
			EXTI_State_GPIO_B |= 1<<num;
			EXTI_ClearITPendingBit(EXTI_GPIOB,1<<num);
		}
	}
	TMC_EXTI_CpltCallback_GPIOB(EXTI_State_GPIO_B);
}

/**
  * @brief GPIOC EXTI IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_GPIOC_IRQHandler(void)
{
	uint8_t num;
	EXTI_State_GPIO_C = 0;
	for(num=0;num<16;num++)
	{
		if(EXTI_GetPINxStatus(EXTI_GPIOC,1<<num)== SET)
		{
			EXTI_State_GPIO_C |= 1<<num;
			EXTI_ClearITPendingBit(EXTI_GPIOC,1<<num);
		}
	}
	TMC_EXTI_CpltCallback_GPIOC(EXTI_State_GPIO_C);
}

/**
  * @brief GPIOD EXTI IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_GPIOD_IRQHandler(void)
{
	uint8_t num;
	EXTI_State_GPIO_D = 0;
	for(num=0;num<16;num++)
	{
		if(EXTI_GetPINxStatus(EXTI_GPIOD,1<<num)== SET)
		{
			EXTI_State_GPIO_D |= 1<<num;
			EXTI_ClearITPendingBit(EXTI_GPIOD,1<<num);
		}
	}
	TMC_EXTI_CpltCallback_GPIOD(EXTI_State_GPIO_D);
}

/**
  * @brief GPIOE EXTI IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_GPIOE_IRQHandler(void)
{
	uint8_t num;
	EXTI_State_GPIO_E = 0;
	for(num=0;num<16;num++)
	{
		if(EXTI_GetPINxStatus(EXTI_GPIOE,1<<num)== SET)
		{
			EXTI_State_GPIO_E |= 1<<num;
			EXTI_ClearITPendingBit(EXTI_GPIOE,1<<num);
		}
	}
	TMC_EXTI_CpltCallback_GPIOE(EXTI_State_GPIO_E);
}

/**
  * @brief EXTI USB Resume  IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_USB_Resume_IRQHandler(void)
{
	EXTI_State_USB_Resume_Rsing = 1;
	EXTI_ClearOtherStatus(EXTI_USB_WAKE_UP);
	TMC_EXTI_CpltCallback_USB(EXTI_State_USB_Resume_Rsing);
}

/**
  * @brief EXTI RTC Alarm IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_RTC_Alarm_IRQHandler(void)
{
	EXTI_State_RTC_Alarm = 1;
	EXTI_ClearOtherStatus(EXTI_RTC_ALM_WAKE_UP);
	TMC_EXTI_CpltCallback_RTC_Alarm(EXTI_State_RTC_Alarm);
}


/**
  * @brief EXTI IWDT IRQ Handler
  * @param[in] EXTI_State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for the specified EXTI.
  * @retval  none
  */
void TMC_EXTI_IWDT_Alarm_IRQHandler(void)
{
	EXTI_State_IWDT_Alarm = 1;
	EXTI_ClearOtherStatus(EXTI_IWDT_ALM_WAKE_UP);
	TMC_EXTI_CpltCallback_IWDT_Alarm(EXTI_State_IWDT_Alarm);
}

/**
  * @brief EXTI callback.
  * @param  State pointer to a EXTI_StateTypeDef structure that contains
  *			   the configuration information for EXTI.
  * @retval None
  */
__weak void TMC_EXTI_CpltCallback_GPIOA(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}
__weak void TMC_EXTI_CpltCallback_GPIOB(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}
__weak void TMC_EXTI_CpltCallback_GPIOC(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}
__weak void TMC_EXTI_CpltCallback_GPIOD(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}
__weak void TMC_EXTI_CpltCallback_GPIOE(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}
__weak void TMC_EXTI_CpltCallback_USB(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}
__weak void TMC_EXTI_CpltCallback_RTC_Alarm(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
}

__weak void TMC_EXTI_CpltCallback_IWDT_Alarm(uint32_t uiState)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(uiState);
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
 

