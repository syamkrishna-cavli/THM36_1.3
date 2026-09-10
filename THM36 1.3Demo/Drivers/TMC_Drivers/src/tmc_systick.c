/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_systick.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "rcc.h"
#include "TMC_SYSTICK.h"


/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */

/** @defgroup TMC_SYSTICK 
  * @brief SYSTICK TMC modules driver
  * @{
  */

/** @addtogroup TMC_SYSTICK_Private_Variables
  * @{
  */
__IO uint32_t uwTick;
uint32_t SystemCoreClock;
uint32_t SystemCoreClock_MHz;
extern uint32_t uwTickPrio; /* Invalid PRIO */
extern TMC_TickFreqTypeDef uwTickFreq;  /* 1KHz */


uint32_t uwTickPrio   = (1UL << __NVIC_PRIO_BITS); /* Invalid PRIO */
TMC_TickFreqTypeDef uwTickFreq = TMC_TICK_FREQ_DEFAULT;  /* 1KHz */
/**
  * @}
  */


/* Exported functions ---------------------------------------------------------*/
/** @defgroup TMC_RCC_Exported_Functions TMC RCC Exported Functions
  * @{
  */

/**
  * @brief This function configures the source of the time base.
  *		The time source is configured to have 1ms time base with a dedicated 
  *		Tick interrupt priority.
  * @note In the default implementation, SysTick timer is the source of time base. 
  *	   It is used to generate interrupts at regular time intervals. 
  *	   Care must be taken if TMC_Delay() is called from a peripheral ISR process, 
  *	   The SysTick interrupt must have higher priority (numerically lower)
  *	   than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
  *	   The function is declared as __weak  to be overwritten  in case of other
  *	   implementation in user file.
  * @param TickPriority Tick interrupt priority.
  * @retval HAL status
  */
  
__weak TMC_StatusTypeDef TMC_InitTick(uint32_t TickPriority)
{
	volatile uint32_t temp;
	temp = RCC_GetSystemClockSource();
	switch(temp)
	{
		case RCC_SYS_CLKSRC_HSI: SystemCoreClock = HSI_VALUE/RCC_GetSystemClockDivision();break;
		case RCC_SYS_CLKSRC_PLL_H:SystemCoreClock = RCC_GetPLL_H_Clock()/RCC_GetSystemClockDivision();break;
		case RCC_SYS_CLKSRC_HSE:SystemCoreClock = HSE_VALUE/RCC_GetSystemClockDivision();break;
		default: SystemCoreClock = HSI_VALUE/RCC_GetSystemClockDivision();break;
	
	}
	SystemCoreClock_MHz=SystemCoreClock/1000000;
    
    /* Configure the SysTick to have interrupt in 1ms time basis*/
	if (TMC_SYSTICK_Config(SystemCoreClock / (1000U / uwTickFreq)) > 0U)
	{
		return TMC_ERROR;
	}

	/* Configure the SysTick IRQ priority */
	if (TickPriority < (1UL << __NVIC_PRIO_BITS))
	{
		NVIC_SetPriority(SysTick_IRQn, TickPriority);
		uwTickPrio = TickPriority;
	}
	else
	{
		return TMC_ERROR;
	}

	/* Return function status */
	return TMC_OK;
}

/**
  * @brief  Initializes the System Timer and its interrupt, and starts the System Tick Timer.
  *		 Counter is in free running mode to generate periodic interrupts.
  * @param  TicksNumb Specifies the ticks Number of ticks between two interrupts.
  * @retval status:  - 0  Function succeeded.
  *				  - 1  Function failed.
  */
uint32_t TMC_SYSTICK_Config(uint32_t TicksNumb)
{
	return SysTick_Config(TicksNumb);
}

/**
  * @brief This function is called to increment a global variable "uwTick"
  *		used as application time base.
  * @note In the default implementation, this variable is incremented each 1ms
  *	   in SysTick ISR.
 * @note This function is declared as __weak to be overwritten in case of other 
  *	  implementations in user file.
  * @retval None
  */
__weak void TMC_IncTick(void)
{
	uwTick += uwTickFreq;
}

/**
  * @brief Provides a tick value in millisecond.
  * @note This function is declared as __weak to be overwritten in case of other 
  *	   implementations in user file.
  * @retval tick value
  */
__weak uint32_t TMC_GetTick(void)
{
	return uwTick;
}

/**
  * @brief This function returns a tick priority.
  * @retval tick priority
  */
uint32_t TMC_GetTickPrio(void)
{
	return uwTickPrio;
}

/**
  * @brief Set new tick Freq.
  * @retval Status
  */
TMC_StatusTypeDef TMC_SetTickFreq(TMC_TickFreqTypeDef Freq)
{
	TMC_StatusTypeDef status  = TMC_OK;
//	assert_param(IS_TICKFREQ(Freq));

	if (uwTickFreq != Freq)
	{
		uwTickFreq = Freq;

		/* Apply the new tick Freq  */
		status = TMC_InitTick(uwTickPrio);
	}

	return status;
}

/**
  * @brief Return tick frequency.
  * @retval tick period in Hz
  */
TMC_TickFreqTypeDef TMC_GetTickFreq(void)
{
	return uwTickFreq;
}

/**
  * @brief This function provides minimum delay (in milliseconds) based 
  *		on variable incremented.
  * @note In the default implementation , SysTick timer is the source of time base.
  *	   It is used to generate interrupts at regular time intervals where uwTick
  *	   is incremented.
  * @note This function is declared as __weak to be overwritten in case of other
  *	   implementations in user file.
  * @param Delay specifies the delay time length, in milliseconds.
  * @retval None
  */
__weak void TMC_Delay(uint32_t Delay)
{
	uint32_t tickstart = TMC_GetTick();
	uint32_t wait = Delay;

	/* Add a freq to guarantee minimum wait */
	if (wait < HAL_MAX_DELAY)
	{
		wait += (uint32_t)(uwTickFreq);
	}

	while((TMC_GetTick() - tickstart) < wait);
}

void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD的值			 
	ticks=nus*SystemCoreClock_MHz; 						//需要的节拍数 
	told=SysTick->VAL;						//刚进入时的计数器值
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{		
			if(tnow<told)tcnt+=told-tnow;	//这里注意一下SYSTICK是一个递减的计数器就可以了.
			else tcnt+=reload-tnow+told;		
			told=tnow;
			if(tcnt>=ticks)break;			//时间超过/等于要延迟的时间,则退出.
		}  
	};
}

void delay_ms(uint32_t nms)
{
	uint32_t i;
	for(i=0;i<nms;i++)
		delay_us(1000);
}
/**
  * @brief Suspend Tick increment.
  * @note In the default implementation , SysTick timer is the source of time base. It is
  *	   used to generate interrupts at regular time intervals. Once HAL_SuspendTick()
  *	   is called, the SysTick interrupt will be disabled and so Tick increment 
  *	   is suspended.
  * @note This function is declared as __weak to be overwritten in case of other
  *	   implementations in user file.
  * @retval None
  */
__weak void TMC_SuspendTick(void)
{
	/* Disable SysTick Interrupt */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
  * @brief Resume Tick increment.
  * @note In the default implementation , SysTick timer is the source of time base. It is
  *	   used to generate interrupts at regular time intervals. Once HAL_ResumeTick()
  *	   is called, the SysTick interrupt will be enabled and so Tick increment 
  *	   is resumed.
  * @note This function is declared as __weak to be overwritten in case of other
  *	   implementations in user file.
  * @retval None
  */
__weak void TMC_ResumeTick(void)
{
	/* Enable SysTick Interrupt */
	SysTick->CTRL  |= SysTick_CTRL_TICKINT_Msk;
}

/**
  * @brief  This function handles SYSTICK interrupt request.
  * @retval None
  */
void TMC_SYSTICK_IRQHandler(void)
{
	TMC_SYSTICK_Callback();
}

/**
  * @brief  SYSTICK callback.
  * @retval None
  */
__weak void TMC_SYSTICK_Callback(void)
{
	/* NOTE : This function Should not be modified, when the callback is needed,
			the HAL_SYSTICK_Callback could be implemented in the user file
	*/
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


