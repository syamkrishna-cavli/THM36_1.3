/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: pwm.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "pwm.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup PWM
  * @brief PWM HAL modules driver 
  * @{
  */
/** @defgroup PWM_private_defines PWM private defines
  * @{
  */
#define PWM_MODE_CONFIG			~(1UL)
#define PWM_MODE_NOT_CONFIG		(1UL)

/**
  * @}
  */
/** @defgroup PWM_exported_function PWM exported function
  * @{  
  */


/**
  * @brief  Fills each PWM_InitStruct member with its default value.
  * @param[in] PWM_InitStruct: pointer to a PWM_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  the OutPut frequency = PWM source frequency / PWM_Div/ PWM_PSC/PWM_PER
  *			Duty cycle = PWM_PER/PWM_DTY;
  */
__weak void PWM_StructInit(PWM_InitTypeDef *PWM_InitStruct)
{
	PWM_InitStruct->PWM_Start_Level = START_LEVEL_LOW;
	PWM_InitStruct->PWM_Stop_Level = STOP_LEVEL_IS_NOW;
	PWM_InitStruct->PWM_Div = PWM_CLK_DIV1;
	PWM_InitStruct->PWM_PSC = 12;
	PWM_InitStruct->PWM_PER = 1000;
	PWM_InitStruct->PWM_DTY = 1000>>1;
}

/**
  * @brief Initializes the PWM peripheral according to 
  *   the specified parameters in the PWM_InitTypeDef.
  * @param[in] PWM_InitStruct: pointer to a PWM_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note none
  */  

void PWM_Initial(PWM_TypeDef* PWMx,PWM_InitTypeDef *PWM_InitStruct) 
{
	assert_param(IS_PWM_START_LEVEL(PWM_InitStruct->PWM_Start_Level)); 
	assert_param(IS_PWM_STOP_LEVEL(PWM_InitStruct->PWM_Stop_Level));
	assert_param(IS_PWM_DIV(PWM_InitStruct->PWM_Div));
	assert_param(IS_PWM_PSC(PWM_InitStruct->PWM_PSC));
	assert_param(IS_PWM_DTY(PWM_InitStruct->PWM_DTY));
	assert_param(IS_PWM_PER(PWM_InitStruct->PWM_PER));
	PWMx->CON&=~PWM_MODE_CONFIG;
	PWM_ConfigStartLevel(PWMx,PWM_InitStruct->PWM_Start_Level);
	PWM_ConfigStopLevel(PWMx,PWM_InitStruct->PWM_Stop_Level);
	PWMx->CLK = PWM_InitStruct->PWM_Div;
	PWMx->PSC = PWM_InitStruct->PWM_PSC;
	PWMx->DTY = PWM_InitStruct->PWM_DTY;
	PWMx->PER = PWM_InitStruct->PWM_PER;
	PWM_Cmd(PWMx,ENABLE);
} 

/**
  * @brief Deinitializes the ADC peripheral registers to their default reset values.
  * @param[in] none
  * @retval none
  * @note  
  */ 
void PWM_DeInitial(PWM_TypeDef* PWMx)
{
	PWM_Cmd(PWMx,DISABLE);
	if(PWMx == PWM1)
	{
		RCC_APB2PeriphSoftReset(RCC_APB2Periph_PWM1);
		while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_PWM1) == RESET);
	}
	else
	{
		RCC_APB3PeriphSoftReset(RCC_APB3Periph_PWM2);
		while(RCC_APB3PeriphGetResetFlag(RCC_APB3Periph_PWM2) == RESET);
	}
}

/**
  * @brief  Config PWMx's Start Level
  * @param[in] PWMx:select the PWM1 or PWM2 peripheral.
  * @param[in] Level:select the Level of PWMx
  *			@arg START_LEVEL_HIGH
  *			@arg START_LEVEL_LOW
  * @retval none
  * @note none
  */
void PWM_ConfigStartLevel(PWM_TypeDef* PWMx,uint32_t Level)
{
	if(Level == START_LEVEL_LOW)
	{
		PWMx->CFG &= Level;
	}
	else
	{
		PWMx->CFG |= Level;
	}
}

/**
  * @brief  Config PWMx's Stop Level
  * @param[in] PWMx:select the PWM1 or PWM2 peripheral.
  * @param[in] Level:select the Level of PWMx
  *			@arg STOP_LEVEL_IS_NOW
  *			@arg STOP_LEVEL_IS_START
  * @retval none
  * @note none
  */
void PWM_ConfigStopLevel(PWM_TypeDef* PWMx,uint32_t Level)
{
	if(Level == STOP_LEVEL_IS_START)
	{
		PWMx->CFG |= Level;
	}
	else
	{
		PWMx->CFG &= Level;
	}
}

/**
  * @brief  Enable or Disable PWM 
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
void PWM_Cmd(PWM_TypeDef* PWMx,FunctionalState NewState) 
{
	if(NewState == ENABLE)
	{
		PWMx->CON |= 1UL;
	}
	else
	{
		PWMx->CON &= ~(1UL);
	}
}

/**
  * @brief  Enable or Disable PWM's interrupt 
  * @param[in] Flag the interrupt of PWMx
  *		@arg PWM_STS_DTY_FINISH
  *		@arg PWM_STS_PER_FINISH
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
void PWM_ConfigIT(PWM_TypeDef* PWMx,uint32_t Flag,FunctionalState NewState) 
{
	if(NewState == ENABLE)
	{
		PWMx->MSK&= ~Flag;
	}
	else
	{
		PWMx->MSK |= Flag;
	}
}


/**
  * @brief  Get PWM Status 
  * @param[in] Flag the Status of PWMx
  *		@arg PWM_OUT_HIGH
  *		@arg PWM_STS_DTY_FINISH
  *		@arg PWM_STS_PER_FINISH
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
FlagStatus PWM_GetStatus(PWM_TypeDef* PWMx,uint32_t Flag) 
{
	if(PWMx->STS&Flag)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Enable or Disable PWM's interrupt 
  * @param[in] Flag the Status of PWMx
  *		@arg PWM_OUT_HIGH
  *		@arg PWM_STS_DTY_FINISH
  *		@arg PWM_STS_PER_FINISH
  * @param[in] NewState: NewState: new state of the SPIx peripheral.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note none
  */  
void PWM_ClearStatus(PWM_TypeDef* PWMx,uint32_t Flag) 
{
	PWMx->STS = Flag;
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

