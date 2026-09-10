/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_pwm.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_pwm.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_PWM
  * @brief PWM TMC modules driver 
  * @{
  */
static TMC_StatusTypeDef PWM_WaitFlagStateUntilTimeout(PWM_HandleTypeDef* hpwm, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart);  
/**
  * @brief  Fills each PWM_HandleTypeDef member with its default value.
  * @param[in] PWM_HandleTypeDef: pointer to a PWM_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_PWM_StructInit(PWM_HandleTypeDef* hpwm)
{
	hpwm->Init = NULL;
	hpwm->Instance = NULL;
	hpwm->State = TMC_PWM_STATE_RESET;
	hpwm->XferSize = NULL;
	hpwm->XferCount = NULL;
}
/**
  * @brief  Initializes the PWM Unit according to the specified
  * @param[in]  hspi: pointer to a PWM_HandleTypeDef structure that contains
  *		 the configuration information for PWM module.
  * @retval None
  */
TMC_StatusTypeDef TMC_PWM_Init(PWM_HandleTypeDef* hpwm) 
{
	/* check point of hadc is NULL */
	if(hpwm == NULL)
	{
		return TMC_ERROR;
	}
	
	/* check state of SPI is RESERT */
	if(hpwm->State == TMC_PWM_STATE_RESET)
	{

	 /* Init the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_PWM_MspInit(hpwm);
	}
	PWM_Cmd(hpwm->Instance,DISABLE);
	PWM_Initial(hpwm->Instance,hpwm->Init);


	hpwm->State = TMC_PWM_STATE_READY;
	return TMC_OK;
}  

/**
  * @brief  Initializes the PWM Unit according to the specified
  * @param[in]  hspi: pointer to a PWM_HandleTypeDef structure that contains
  *		 the configuration information for PWM module.
  * @retval None
  */
TMC_StatusTypeDef TMC_PWM_DeInit(PWM_HandleTypeDef* hpwm) 
{
	/* check point of hadc is NULL */
	if(hpwm == NULL)
	{
		return TMC_ERROR;
	}	
	
	PWM_Cmd(hpwm->Instance,DISABLE);
	PWM_DeInitial(hpwm->Instance);
	/* Init the low level hardware : GPIO, CLOCK, NVIC... */	
	TMC_PWM_MspDeInit(hpwm);
	hpwm->State = TMC_PWM_STATE_RESET;
	return TMC_OK;
}

///**
//  * @brief  Start PWM always Out waveform,  
//  * @param[in]  hpwm: pointer to a PWM_HandleTypeDef structure that contains
//  *		 the configuration information for PWM module.
//  * @retval Call TMC_PWM_Stop Function to Stop Output waveform
//  */
//TMC_StatusTypeDef TMC_PWM_StartOutputWaveforms(PWM_HandleTypeDef* hpwm) 
//{
//	/* check point of hadc is NULL */
//	if(hpwm == NULL)
//	{
//		return TMC_ERROR;
//	}
//	hpwm->State = TMC_PWM_STATE_BUSY;
//	PWM_Cmd(hpwm->Instance,ENABLE);
//	return TMC_OK;
//}

///**
//  * @brief  Stop PWM Out 
//  * @param[in]  hpwm: pointer to a PWM_HandleTypeDef structure that contains
//  *		 the configuration information for PWM module.
//  * @retval none
//  */
//TMC_StatusTypeDef TMC_PWM_StopOutputWaveforms(PWM_HandleTypeDef* hpwm) 
//{
//	/* check point of hadc is NULL */
//	if(hpwm == NULL)
//	{
//		return TMC_ERROR;
//	}
//	hpwm->State = TMC_PWM_STATE_READY;
//	PWM_Cmd(hpwm->Instance,DISABLE);
//	return TMC_OK;
//}

/**
  * @brief Handle PWM Communication Timeout.
  * @param hspi pointer to a PWM_HandleTypeDef structure that contains
  *			  the configuration information for PWM module.
  * @param Flag PWM flag to check
  * @param State flag state to check
  * @param Timeout Timeout duration
  * @param Tickstart tick start value
  * @retval TMC status
  */
static TMC_StatusTypeDef PWM_WaitFlagStateUntilTimeout(PWM_HandleTypeDef* hpwm, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart)
{
	while((((hpwm->Instance->STS & Flag) == (Flag)) ? SET : RESET) != State)
	{
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
			{
				return TMC_TIMEOUT;
			}
		}
	}
	return TMC_OK;
}
/**
  * @brief  PWM OutPut numbers of waveforms
  * @param[in]  hpwm: pointer to a PWM_HandleTypeDef structure that contains
  *		 the configuration information for PWM module.
  * @retval none
  */
TMC_StatusTypeDef TMC_PWM_OutputWaveforms(PWM_HandleTypeDef* hpwm,uint32_t Size,uint32_t Timeout) 
{
	uint32_t tickstart = 0U;
	tickstart = TMC_GetTick();
	if(hpwm == NULL)
	{
		return TMC_ERROR;
	}
	hpwm->State = TMC_PWM_STATE_BUSY;
	hpwm->XferSize = Size;
	hpwm->XferCount = 0;
	PWM_ClearStatus(hpwm->Instance,PWM_STS_DTY_FINISH|PWM_STS_PER_FINISH);
	PWM_Cmd(hpwm->Instance,ENABLE);
	while(hpwm->XferCount<hpwm->XferSize)
	{
		if(PWM_WaitFlagStateUntilTimeout(hpwm,PWM_STS_PER_FINISH,SET,Timeout,tickstart) == TMC_TIMEOUT)
		{
			PWM_Cmd(hpwm->Instance,DISABLE);
			return TMC_TIMEOUT;
		}
		else
		{
			PWM_ClearStatus(hpwm->Instance,PWM_STS_DTY_FINISH|PWM_STS_PER_FINISH);
			hpwm->XferCount++;
		}
	}
	PWM_Cmd(hpwm->Instance,DISABLE);
	hpwm->State = TMC_PWM_STATE_READY;
	return TMC_OK;
}

/**
  * @brief  PWM OutPut numbers of waveforms
  * @param[in]  hpwm: pointer to a PWM_HandleTypeDef structure that contains
  *		 the configuration information for PWM module.
  * @retval none
  */
TMC_StatusTypeDef TMC_PWM_OutputWaveforms_IT(PWM_HandleTypeDef* hpwm,uint32_t Size) 
{
	if(hpwm == NULL)
	{
		return TMC_ERROR;
	}
	hpwm->State = TMC_PWM_STATE_BUSY;
	hpwm->XferSize = Size;
	hpwm->XferCount = 0;
	PWM_ClearStatus(hpwm->Instance,PWM_STS_DTY_FINISH|PWM_STS_PER_FINISH);
	PWM_ConfigIT(hpwm->Instance,PWM_STS_PER_FINISH,ENABLE);
	PWM_Cmd(hpwm->Instance,ENABLE); 
	return TMC_OK;
}

/**
  * @brief  Handle PWM interrupt request.
  * @param  hspi pointer to a PWM_HandleTypeDef structure that contains
  *			   the configuration information for the specified PWM module.
  * @retval None
  */
void TMC_PWM_IRQHandler(PWM_HandleTypeDef* hpwm)
{
	PWM_ClearStatus(hpwm->Instance,PWM_STS_DTY_FINISH|PWM_STS_PER_FINISH);
	hpwm->XferCount++;
	if(hpwm->XferCount == hpwm->XferSize)
	{
		hpwm->State = TMC_PWM_STATE_READY;
		PWM_ConfigIT(hpwm->Instance,PWM_STS_PER_FINISH,DISABLE);
		PWM_Cmd(hpwm->Instance,DISABLE);
		TMC_PWM_CpltCallback(hpwm);
	}
}


/**
  * @brief  TMC PWM completed callback.
  * @param  hpwm pointer to a PWM_HandleTypeDef structure that contains
  *			   the configuration information for  PWM module.
  * @retval None
  */
__weak void TMC_PWM_CpltCallback(PWM_HandleTypeDef* hpwm)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hpwm);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_PWM_CpltCallback should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the PWM MSP.
  * @param  hspi pointer to a PWM_HandleTypeDef structure that contains
  *			   the configuration information for PWM module.
  * @retval None
  */
__weak void TMC_PWM_MspInit(PWM_HandleTypeDef* hpwm)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hpwm);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_PWM_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  DeInitialize the PWM MSP.
  * @param  hpwm pointer to a PWM_HandleTypeDef structure that contains
  *			   the configuration information for PWM module.
  * @retval None
  */
__weak void TMC_PWM_MspDeInit(PWM_HandleTypeDef* hpwm)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hpwm);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
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
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

