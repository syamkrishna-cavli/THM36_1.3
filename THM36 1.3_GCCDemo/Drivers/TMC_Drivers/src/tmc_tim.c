/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_tim.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "tim.h"
#include "tmc_tim.h"
#include "tmc_gpio.h"
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_TIMER
  * @brief TIM TMC modules driver 
  * @{
  */
/** @defgroup TMC_TIM_Exported_Function TMC TIM Exported Function
  * @brief TIM TMC modules driver
  * @{
  */
/**
  * @brief  Fills each TIM_HandleTypeDef member with its default value.
  * @param[in] TIM_HandleTypeDef: pointer to a TIM_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_TIM_StructInit(TIM_HandleTypeDef* htim)
{
	htim->Instance = NULL;
	htim->State = TIM_STATE_RESET;
}
/**
  * @brief  Initializes the TIM Time base Unit according to the specified
  *		 parameters in the TIM_HandleTypeDef and create the associated handle.
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  *		 the configuration information for TIM module.
  * @retval StatusTypeDef status
  */
TMC_StatusTypeDef TMC_TIM_Init(TIM_HandleTypeDef *htim)
{ 
	/* Check the tim.handle allocation */
	if(htim == NULL)
	{
		return TMC_ERROR;
	}
	
	if(htim->State == TIM_STATE_RESET)
	{  
		/* Init the low level hardware : GPIO, CLOCK, NVIC */
		TMC_TIM_MspInit(htim);
  }
  
	/* Set the TIM state */
	htim->State= TIM_STATE_BUSY; 
	/* Set the Time Base configuration */
	TIM_TimeInit(htim->Instance,htim->Init);
	/* Initialize the TIM state*/
	htim->State= TIM_STATE_READY;
	return TMC_OK;
}

/**
  * @brief  DeInitializes the TIM Base peripheral 
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  * 		the configuration information for TIM module.
  * @retval StatusTypeDef status
  */
TMC_StatusTypeDef TMC_TIM_DeInit(TIM_HandleTypeDef *htim)
{  
	/* Check the tim.handle allocation */
	if(htim == NULL)
	{
		return TMC_ERROR;
	}
	/* Disable the TIMx */
	TIM_DeInit(htim->Instance);
	/* DeInit the low level hardware: CLOCK, NVIC */
	TMC_TIM_MspDeInit(htim);
	/* Change TIM state */  
	htim->State = TIM_STATE_RESET; 
	return TMC_OK;
}

/**
  * @brief  Initializes the TIM Base MSP.
  * @param  htim pointer to a TIM_HandleTypeDef structure that contains
  *				the configuration information for TIM module.
  * @retval None
  */
__weak void TMC_TIM_MspInit(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(htim);
	/* NOTE : This function Should not be modified, when the callback is needed,
				the HAL_TIM_Base_MspInit could be implemented in the user file
	*/
}

/**
  * @brief  DeInitializes TIM Base MSP.
  * @param  htim pointer to a TIM_HandleTypeDef structure that contains
  *				the configuration information for TIM module.
  * @retval None
  */
__weak void TMC_TIM_MspDeInit(TIM_HandleTypeDef *htim)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(htim);
	/* NOTE : This function Should not be modified, when the callback is needed,
				the HAL_TIM_Base_MspDeInit could be implemented in the user file
	*/
}

/**
  * @brief  Starts the TIM Base generation.
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  *		 	the configuration information for TIM module.
  * @param[in]  ReloadValue is TIM's Reload Value,if htim->Init->TIM_SizeMode is TIM_SIZE_16BIT, the Range of Value is 0x0000 to 0xFFFF
  *				if htim->Init->TIM_SizeMode is TIM_SIZE_32BIT, the Range of Value is 0x00000000 to 0xFFFFFFFF
  * @retval StatusTypeDef status
  */
TMC_StatusTypeDef TMC_TIM_Start(TIM_HandleTypeDef *htim,uint32_t ReloadValue)
{
	/* Check the tim.handle allocation */
	if(htim == NULL)
	{
		return TMC_ERROR;
	}
	/* Set the TIM state */
	htim->State= TIM_STATE_BUSY;
	/* Set the TIM reload value */
	TIM_SetLoadValue(htim->Instance,ReloadValue);

	/* Enable the Peripheral */
	//TMC_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_RESET);
	TIM_Cmd(htim->Instance,ENABLE);
	while(TIM_GetFlagStatus(htim->Instance)== RESET);
	//TMC_GPIO_WritePin(GPIOB,GPIO_PIN_13,GPIO_PIN_SET);
	TIM_ClearFlag(htim->Instance);
	htim->State= TIM_STATE_READY;
	/* Return function status */
	return TMC_OK;
}


/**
  * @brief  Stops the TIM Base generation.
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  *		 the configuration information for TIM module.
  * @retval StatusTypeDef status
  */
TMC_StatusTypeDef TMC_TIM_Stop(TIM_HandleTypeDef *htim)
{
	/* Check the tim.handle allocation */
	if(htim == NULL)
	{
		return TMC_ERROR;
	}
	/* Disable the Peripheral */
	TIM_Cmd(htim->Instance,DISABLE);

	/* Change the TIM state*/
	htim->State= TIM_STATE_READY;

	/* Return function status */
	return TMC_OK;
}

/**
  * @brief  Starts the TIM Base generation in interrupt mode.
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  *			 the configuration information for TIM module.
  * @retval StatusTypeDef status
  */
TMC_StatusTypeDef TMC_TIM_StartIT(TIM_HandleTypeDef *htim,uint32_t ReloadValue)
{
	/* Check the tim.handle allocation */
	if(htim == NULL)
	{
		return TMC_ERROR;
	}
	htim->State= TIM_STATE_BUSY;
	/* Enable the TIM  interrupt */
	TIM_ITConfig(htim->Instance,ENABLE);
	 
	/* Set the TIM reload value */
	TIM_SetLoadValue(htim->Instance,ReloadValue); 
	/* Enable the Peripheral */
	TIM_Cmd(htim->Instance,ENABLE);
	/* Return function status */
	return TMC_OK;
}

/**
  * @brief  Stops the TIM Base generation in interrupt mode.
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  *			 the configuration information for TIM module.
  * @retval StatusTypeDef status
  */
TMC_StatusTypeDef TMC_TIM_StopIT(TIM_HandleTypeDef *htim)
{
	/* Check the tim.handle allocation */
	if(htim == NULL)
	{
		return TMC_ERROR;
	}
	/* Disable the TIM  interrupt */
	TIM_ITConfig(htim->Instance,ENABLE);  
	/* Disable the Peripheral */
	TIM_Cmd(htim->Instance,DISABLE);
	/* Return function status */
	return TMC_OK;
}
/**
  * @brief  This function handles Tmx interrupt request.
  * @param  huart pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
void TMC_TIM_IRQHandler(TIM_HandleTypeDef *htim)
{
	TIM_ClearITStatus(htim->Instance);
	htim->State = TIM_STATE_READY;
	TMC_TIM_Callback(htim);
}

/**
  * @brief  Input Capture callback in non blocking mode 
  * @param[in] htim pointer to a TIM_HandleTypeDef structure that contains
  *			the configuration information for TIM module.
  * @retval None
  */
__weak void TMC_TIM_Callback(TIM_HandleTypeDef *htim)
{
	UNUSED(htim);
	/* NOTE : This function Should not be modified, when the callback is needed,
		the TMC_TIM_Callback could be implemented in the user file
	*/
}

/**
  * @brief  Return the TIM state
  * @param[in]  htim pointer to a TIM_HandleTypeDef structure that contains
  *			 the configuration information for TIM module.
  * @retval TIM_StateTypeDef state
  */
TIM_StateTypeDef TMC_TIM_GetState(TIM_HandleTypeDef *htim)
{
  return htim->State;
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

