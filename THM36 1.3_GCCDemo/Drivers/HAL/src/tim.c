/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tim.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "tim.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup TIMER
  * @brief TIM HAL modules driver 
  * @{
  */
/** @defgroup TIM_Private_Defines HAL TIM Private Defines
  * @{
  */
#define  TIM_CLR_PRESCALER_VALUE		(~(0x0000000CUL))
#define  TIM_CLR_SIZE_MODE_VALUE		(~(0x00000002UL))
#define  TIM_CLR_COUNTER_MODE_VALUE		(~(TIM_MODE_CONTINUITY_RELOAD|TIM_MODE_SINGEL))
#define  TIM_ENABLE_Pos					(7UL)
#define  TIM_ENABLE_Msk					(1<<TIM_ENABLE_Pos)
#define  TIM_DISABLE_Msk				(~(1<<TIM_ENABLE_Pos))
#define  TIM_IT_ENABLE_Pos				(5UL)
#define  TIM_IT_ENABLE_Msk				(1UL<<TIM_IT_ENABLE_Pos)
#define  TIM_IT_DISABLE_Msk				(~(1UL<<TIM_IT_ENABLE_Pos))
#define  TIM_FLAG_Pos					0
#define  TIM_FLAG_Msk					(1UL<<TIM_FLAG_Pos)
#define  TIM_IT_FLAG_Pos				0
#define  TIM_IT_FLAG_Msk				(1UL<<TIM_IT_FLAG_Pos)
#define  TIM_LOAD_DEFAULT				0
#define  TIM_CONTROL_DEFAULT			(0x20UL)
#define  TIM_BGLOAD_DEFAULT				0
#define  TIM_CLR_DEFAULT				(1UL)
/**
  * @}
  */
/** @defgroup TIM_Exported_Function HAL TIM Exported Function
  * @{  
  */  
/**
  * @brief  Deinitializes the TIMx peripheral registers to their default reset values.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @retval None
  */
void TIM_DeInit(TIM_TypeDef* TIMx)
{
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	if((TIMx == TIM1)||(TIM1 == TIM2))
	{
		RCC_APB2PeriphSoftReset(RCC_APB2Periph_TIMER1_2);
		while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_TIMER1_2) == RESET);
	}
	if((TIMx == TIM3)||(TIM1 == TIM4))
	{
		RCC_APB3PeriphSoftReset(RCC_APB3Periph_TIMER3_4);
		while(RCC_APB3PeriphGetResetFlag(RCC_APB3Periph_TIMER3_4) == RESET);
	}
	if((TIMx == TIM5)||(TIM1 == TIM6))
	{
		RCC_APB2PeriphSoftReset(RCC_APB2Periph_TIMER5_6);
		while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_TIMER5_6) == RESET);
	}
}

/**
  * @brief  Initializes the TIMx Time Base Unit peripheral according to 
  *   the specified parameters in the TIM_TimeBaseInitStruct.
  * @param[in]  TIMx: where x can be 1, 2, 3, 4, 5 or 6 to select the TIM peripheral.
  * @param[in]  TIM_TimeBaseInitStruct: pointer to a TIM_TimeBaseInitTypeDef
  *   structure that contains the configuration information for the specified TIM peripheral.
  * @retval None
  */
void TIM_TimeInit(TIM_TypeDef* TIMx, TIM_TimeInitTypeDef* TIM_TimeBaseInitStruct)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	assert_param(IS_TIM_PRE_DIV(TIM_TimeBaseInitStruct->TIM_Presclar)); 

	TIMx->CONTROL = 0;
	/* Set the Prescaler value  */
	TIMx->CONTROL |= TIM_TimeBaseInitStruct->TIM_Presclar;
	TIMx->CONTROL |= TIM_TimeBaseInitStruct->TIM_SizeMode;
	
	TIMx->CONTROL |=TIM_TimeBaseInitStruct->TIM_CounterMode;	
}


/**
  * @brief  Fills each TIM_TimeBaseInitStruct member with its default value.
  * @param[in]  TIM_TimeBaseInitStruct : pointer to a TIM_TimeBaseInitTypeDef
  * @note   structure which will be initialized.
  * @retval None
  */
__weak void TIM_TimeStructInit(TIM_TimeInitTypeDef* TIM_TimeBaseInitStruct)
{
	/* Set the default configuration */
	//TIM_TimeBaseInitStruct->TIM_period = 0;
	TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_MODE_CONTINUITY_NO_RELOAD;
	TIM_TimeBaseInitStruct->TIM_Presclar = TIM_PRESCLAR_NOT_DIV;
	TIM_TimeBaseInitStruct->TIM_SizeMode = TIM_SIZE_32BIT;
}

/**
  * @brief  Enables or disables the specified TIM peripheral.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIMx peripheral.
  * @param[in]  NewState: new state of the TIMx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	if (NewState != DISABLE)
	{
		/* Enable the TIM Counter */
		TIMx->CONTROL |= TIM_ENABLE_Msk; 
	}
	else
	{
		/* Disable the TIM Counter */
		TIMx->CONTROL &= TIM_DISABLE_Msk; 
	}
}

/**
  * @brief  Enables or disables the specified TIM interrupts.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIMx peripheral.
  * @param[in]  NewState: new state of the TIM interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void TIM_ITConfig(TIM_TypeDef* TIMx,FunctionalState NewState)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));
	if (NewState != DISABLE)
	{
		/* Enable the Interrupt sources */
		TIMx->CONTROL |= TIM_IT_ENABLE_Msk;
	}
	else
	{
		/* Disable the Interrupt sources */
		TIMx->CONTROL &= TIM_IT_DISABLE_Msk;
	}
}

/**
  * @brief  Clears the TIMx's pending flags.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @retval None
  */
void TIM_ClearFlag(TIM_TypeDef* TIMx)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));

	/* Clear the flags */
	TIMx->CLR = TIM_FLAG_Msk;
}


/**
  * @brief  Get the TIMx's flags Status.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @retval FlagStatus
  */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	/* Clear the flags */
	if(TIMx->RIX == TIM_FLAG_Msk)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Get the TIMx's flags Status.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @retval FlagStatus
  */
FlagStatus TIM_GetITStatus(TIM_TypeDef* TIMx)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	/* Clear the flags */
	if((TIMx->RIX == TIM_FLAG_Msk)&&(TIMx->MIX == TIM_IT_FLAG_Msk))
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Clears the TIMx's pending flags.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @retval None
  */
void TIM_ClearITStatus(TIM_TypeDef* TIMx)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx));

	/* Clear the flags */
	TIMx->CLR = TIM_FLAG_Msk;

}

/**
  * @brief  Set the Load Value
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @param[in]  LoadValue: 	the LaodValue can Set form 1 to 0xFFFFFFFF,when TIM_SizeMode is 32Bits
				the LaodValue can Set form 1 to 0xFFFF,when TIM_SizeMode is 16Bits
  * @retval None
  */
void TIM_SetLoadValue(TIM_TypeDef* TIMx,uint32_t LoadValue)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	
	/* Set the TIMx Load Value */
	
	TIMx->LOAD = LoadValue;
	TIMx->BGLOAD = LoadValue;
}


/**
  * @brief  Get the TIMx's  Value.
  * @param[in]  TIMx: where x can be 1 to 6 to select the TIM peripheral.
  * @param[in]  LoadValue: 	the LaodValue can Set form 1 to 0xFFFFFFFF,when TIM_SizeMode is 32Bits
				the LaodValue can Set form 1 to 0xFFFF,when TIM_SizeMode is 16Bits
  * @retval None
  */
uint32_t TIM_GetValue(TIM_TypeDef* TIMx)
{  
	/* Check the parameters */
	assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
	
	/* Set the TIMx Load Value */
	return TIMx->VALUE;
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

