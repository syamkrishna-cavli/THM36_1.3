/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_rcc.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "rcc.h"
#include "tmc_rcc.h"
#include "thm36_1.3_hal_conf.h"
#include "system_thm36_1.3.h"
#include "pll_enable.h"
/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */

/** @defgroup TMC_RCC 
  * @brief RCC TMC modules driver
  * @{
  */

/** @defgroup TMC_RCC_Private_Defines TMC RCC Private Defines
  * @{
  */

#define RCC_LSE_TIMEOUT_VALUE	  LSE_STARTUP_TIMEOUT
#define HSI_TIMEOUT_VALUE		  2U  /* 2 ms */
#define LSI_TIMEOUT_VALUE		  2U  /* 2 ms */
#define CLOCKSWITCH_TIMEOUT_VALUE  5000U /* 5 s */
#define PLL_TIMEOUT_VALUE		  2U  /* 2 ms */
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup TMC_RCC_Exported_Functions TMC RCC Exported Functions
  * @{
  */


/**
  * @brief  Initializes the RCC Oscillators according to the specified parameters in the
  *		 RCC_OscInitTypeDef.
  * @param[in]  RCC_OscInitStruct pointer to an RCC_OscInitTypeDef structure that
  *		 contains the configuration information for the RCC Oscillators.
  * @note   The PLL is not disabled when used as system clock.
  * @note   Transitions LSE Bypass to LSE On and LSE On to LSE Bypass are not
  *		 supported by this API. User should request a transition to LSE Off
  *		 first and then LSE On or LSE Bypass.
  * @note   Transition HSE Bypass to HSE On and HSE On to HSE Bypass are not
  *		 supported by this API. User should request a transition to HSE Off
  *		 first and then HSE On or HSE Bypass.
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct)
{
	uint32_t tickstart;

	/* Check Null pointer */
	if(RCC_OscInitStruct == NULL)
	{
		return TMC_ERROR;
	}

	/* Check the parameters */
	assert_param(IS_RCC_CLOCKSOURCE(RCC_OscInitStruct->ClockSourceType));
	/*----------------------------- HSI Configuration --------------------------*/
	if(((RCC_OscInitStruct->ClockSourceType) & RCC_ClockSource_HSI) == RCC_ClockSource_HSI)
	{
		/* Check if HSI is used as system clock or as PLL source when PLL is selected as system clock */
		if((RCC_GetSystemClockSource() == RCC_SYS_CLKSRC_HSI) ||\
		((RCC_GetSystemClockSource() == RCC_SYS_CLKSRC_PLL_H) && (RCC_GetPLLClockSource() == RCC_PLLC_PLLSRC_HSI)))
		{
			/* When HSI is used as system clock it will not disabled */
			if((RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == SET) && (RCC_OscInitStruct->HSIState == DISABLE))
			{
				return TMC_ERROR;
			}
		}
		else
		{
			/* Set the new HSI configuration ---------------------------------------*/
			RCC_HSICmd(RCC_OscInitStruct->HSIState);
			/* Check the HSI State */
			if((RCC_OscInitStruct->HSIState)!= DISABLE)
			{
				/* Get Start Tick*/
				tickstart = TMC_GetTick();

				/* Wait till HSI is ready */
				while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
				{
					if((TMC_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)
					{
						return TMC_TIMEOUT;
					}
				}
			}
			else
			{
				/* Get Start Tick*/
				tickstart = TMC_GetTick();

				/* Wait till HSI is ready */
				while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) != RESET)
				{
					if((TMC_GetTick() - tickstart ) > HSI_TIMEOUT_VALUE)
					{
						return TMC_TIMEOUT;
					}
				}
			}
		}
	}
	/*------------------------------ LSI Configuration -------------------------*/
	if(((RCC_OscInitStruct->ClockSourceType) & RCC_ClockSource_LSI) == RCC_ClockSource_LSI)
	{
		RCC_LSICmd(RCC_OscInitStruct->LSIState);
		/* Check the LSI State */
		if((RCC_OscInitStruct->LSIState)!= DISABLE)
		{
			/* Get Start Tick*/
			tickstart = TMC_GetTick();

			/* Wait till LSI is ready */
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
			{
				if((TMC_GetTick() - tickstart ) > LSI_TIMEOUT_VALUE)
				{
					return TMC_TIMEOUT;
				}
			}
		}
		else
		{
			/* Get Start Tick */
			tickstart = TMC_GetTick();

			/* Wait till LSI is ready */
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) != RESET)
			{
				if((TMC_GetTick() - tickstart ) > LSI_TIMEOUT_VALUE)
				{
					return TMC_TIMEOUT;
				}
			}
		}
	}
	/*------------------------------ LSE Configuration -------------------------*/
	if(((RCC_OscInitStruct->ClockSourceType) & RCC_ClockSource_LSE) == RCC_ClockSource_LSE)
	{
		/* Set the new LSE configuration -----------------------------------------*/
		
		RCC_LSECmd(RCC_OscInitStruct->LSEState,RCC_OscInitStruct->LSEConfig.FeedbackResEanble);
		/* Check the LSE State */
		if((RCC_OscInitStruct->LSEState) != DISABLE)
		{
			/* Get Start Tick*/
			tickstart = TMC_GetTick();

			/* Wait till LSE is ready */
			while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
			{
				if((TMC_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
				{
					return TMC_TIMEOUT;
				}
			}
		}
		else
		{
			/* Get Start Tick */
			tickstart = TMC_GetTick();

			/* Wait till LSE is ready */
			while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) != RESET)
			{
				if((TMC_GetTick() - tickstart ) > RCC_LSE_TIMEOUT_VALUE)
				{
					return TMC_TIMEOUT;
				}
			}
		}
	}
	/*-------------------------------- PLL Configuration -----------------------*/
	if(((RCC_OscInitStruct->ClockSourceType) & RCC_ClockSource_PLL) == RCC_ClockSource_PLL)
	{
		/* Check if the PLL is used as system clock or not */
		if(RCC_GetSystemClockSource() != RCC_SYS_CLKSRC_PLL_H)
		{
			if((RCC_OscInitStruct->PLLState) != DISABLE)
			{
				/* Check the parameters */
				assert_param(IS_RCC_PLL_CLKSOURCE(RCC_OscInitStruct->PLLConfig.PLL_Input));

				if(RCC_OscInitStruct->PLLConfig.PLL_Input == RCC_PLLC_PLLSRC_HSI)
				{
					RCC_OscInitStruct->PLLConfig.SourceClockVaule = HSI_VALUE;
				}
				else if(RCC_OscInitStruct->PLLConfig.PLL_Input == RCC_PLLC_PLLSRC_HSE)
				{
					RCC_OscInitStruct->PLLConfig.SourceClockVaule = HSE_VALUE;
				}
				else
				{
					RCC_OscInitStruct->PLLConfig.SourceClockVaule = EXTERNAL_CLOCK_VALUE;
				}

				/* Disable the main PLL. */
				RCC_PLLCmd(DISABLE);

				/* Get Start Tick */
				tickstart = TMC_GetTick();

				/* Wait till PLL is ready */
				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
				{
					if((TMC_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
					{
						return TMC_TIMEOUT;
					}
				}
				/* Configure the main PLL clock source, multiplication and division factors. */
				RCC_PLLConfig(&(RCC_OscInitStruct->PLLConfig));
				RCC_Enable();
//				/* Enable the main PLL. */
//				RCC_PLLCmd(ENABLE);

//				/* Get Start Tick */
//				tickstart = TMC_GetTick();

//				/* Wait till PLL is ready */
//				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
//				{
//					if((TMC_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
//					{
//						return TMC_TIMEOUT;
//					}
//				}
			}
			else
			{
				/* Disable the main PLL. */
				RCC_PLLCmd(DISABLE);
#ifndef EMULATION
				/* Get Start Tick */
				tickstart = TMC_GetTick();

				/* Wait till PLL is ready */
				while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) != RESET)
				{
					if((TMC_GetTick() - tickstart ) > PLL_TIMEOUT_VALUE)
					{
						return TMC_TIMEOUT;
					}
				}
#endif
			}
		}
		else
		{
			return TMC_ERROR;
		}
	}
	return TMC_OK;
}



/**
  * @brief  Initializes the CPU clocks according to the specified
  *		 parameters in the RCC_ClkInitStruct.
  * @param[in]  RCC_ClkInitStruct pointer to an RCC_OscInitTypeDef structure that
  *		 contains the configuration information for the RCC peripheral.
  * @note   A switch from one clock source to another occurs only if the target
  *		 clock source is ready (clock stable after startup delay).
  *		 If a clock source which is not yet ready is selected, the switch will
  *		 occur when the clock source will be ready.
  * @note   If the USBPHY is selected as the system clock source, it must be configured first. 
  * @retval None
  */
TMC_StatusTypeDef TMC_RCC_SystemClockConfig(RCC_SysclkInitTypeDef *RCC_SysclkInitStruct)
{
	/* Check Null pointer */
	if(RCC_SysclkInitStruct == NULL)
	{
		return TMC_ERROR;
	}
	/* HSI is selected as System Clock Source */
	if(RCC_SysclkInitStruct->SYSCLKSource == RCC_SYS_CLKSRC_HSI)
	{
		/* Check the HSI ready flag */
		if(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET)
		{
			return TMC_ERROR;
		}
	}
	/* HSE is selected as System Clock Source */
	else if(RCC_SysclkInitStruct->SYSCLKSource == RCC_SYS_CLKSRC_HSE)
	{
		/* Check the HSE ready flag */
		if(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
		{
			return TMC_ERROR;
		}
	}
	/* PLL is selected as System Clock Source */
	else if(RCC_SysclkInitStruct->SYSCLKSource == RCC_SYS_CLKSRC_PLL_H)
	{
		/* Check the PLL ready flag */
		if(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
			return TMC_ERROR;
		}
	}
	/* USBPHY is selected as System Clock Source */
	else if(RCC_SysclkInitStruct->SYSCLKSource == RCC_SYS_CLKSRC_USBPHY)
	{
		/* Check the USBPHY ready flag */
		
	}
	RCC_SystemClockConfig(RCC_SysclkInitStruct->SYSCLKSource, RCC_SysclkInitStruct->SYSCLKDIV);
	//SystemCoreClock = RCC_GetSysClocksFreq();
	
	return TMC_OK;
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


