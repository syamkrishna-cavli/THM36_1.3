/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_adc.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "TMC_DMA.h"
#include "TMC_ADC.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_ADC
  * @brief ADC TMC modules driver 
  * @{
  */

  
/** @defgroup TMC_ADC_private_defines TMC ADC private defines
  * @{
  */
static TMC_StatusTypeDef ADC_WaitFlagStateUntilTimeout(ADC_HandleTypeDef* hadc, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart);
static void TMC_ADC_ISR(struct __ADC_HandleTypeDef *hadc);
static void TMC_ADC_HDMA0_ReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_ADC_HDMA0_Error(DMA_HandleTypeDef *hdma);
static void TMC_ADC_HDMA1_ReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_ADC_HDMA1_Error(DMA_HandleTypeDef *hdma);
static void TMC_ADC_HDMA2_ReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_ADC_HDMA2_Error(DMA_HandleTypeDef *hdma);
/**
  * @}
  */ 
  
/** @defgroup TMC_ADC_Exported_Function TMC ADC Exported Function
  * @brief ADC TMC modules driver
  * @{
  */
/**
  * @brief  Fills each ADC_HandleTypeDef member with its default value.
  * @param[in] ADC_HandleTypeDef: pointer to a ADC_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_ADC_StructInit(ADC_HandleTypeDef* hadc)
{
	hadc->ADC_ISR = NULL;
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	hadc->State = TMC_ADC_STATE_RESET;
	
	hadc->hdma0 = NULL;
	hadc->hdma1 = NULL;
	hadc->hdma2 = NULL;
	hadc->pBuffPtr = NULL;
	hadc->XferCount = 0;
	hadc->XferSize = 0;

}
/**
  * @brief  Initializes the ADC Unit according to the specified
  *		 parameters in the ADC_HandleTypeDef and create the associated handle.
  * @param[in]  hspi: pointer to a ADC_HandleTypeDef structure that contains
  *		 the configuration information for ADC module.
  * @retval None
  */
TMC_StatusTypeDef TMC_ADC_Init(ADC_HandleTypeDef* hadc) 
{
	/* check point of hadc is NULL */
	if(hadc == NULL)
	{
		return TMC_ERROR;
	}

	
	/* check state of SPI is RESERT */
	if(hadc->State == TMC_ADC_STATE_RESET)
	{

	 /* Init the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_ADC_MspInit(hadc);
	}
	ADC_Cmd(DISABLE);
	ADC_Initial(hadc->Init);
	ADC_AMP_Initial(hadc->AMP_Init);
	 /* DISABHLE ADC DMA Config */	
	//ADC_ConfigDMA(ADC_DMA_CHANNEL0,ADC_CONFIG_DMA_DISABLE,DISABLE);
	//ADC_ConfigDMA(ADC_DMA_CHANNEL1,ADC_CONFIG_DMA_DISABLE,DISABLE);
	//ADC_ConfigDMA(ADC_DMA_CHANNEL2,ADC_CONFIG_DMA_DISABLE,DISABLE);
	ADC_DMA_Cmd(ADC_DMA_CHANNEL1,DISABLE);
	ADC_DMA_Cmd(ADC_DMA_CHANNEL2,DISABLE);
	ADC_DMA_Cmd(ADC_DMA_CHANNEL3,DISABLE);

	hadc->State = TMC_ADC_STATE_READY;
	return TMC_OK;
}

/**
  * @brief  DeInitializes the ADC peripheral 
  * @param[in]  hadc pointer to a ADC_HandleTypeDef structure that contains
  * 		the configuration information for ADC module.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_ADC_DeInit(ADC_HandleTypeDef* hadc) 
{
	/* check point of hadc is NULL */
	if(hadc == NULL)
	{
		return TMC_ERROR;
	}

	
	ADC_Cmd(DISABLE);
	
	ADC_DeInitial();
	/* check state of SPI is RESERT */
	if(hadc->State != TMC_ADC_STATE_RESET)
	{

	 /* deInit the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_ADC_MspDeInit(hadc);
	}
	hadc->State = TMC_ADC_STATE_RESET;
	return TMC_OK;
}


/**
  * @brief Handle SPI Communication Timeout.
  * @param hspi pointer to a SPI_HandleTypeDef structure that contains
  *			  the configuration information for SPI module.
  * @param Flag SPI flag to check
  * @param State flag state to check
  * @param Timeout Timeout duration
  * @param Tickstart tick start value
  * @retval TMC status
  */
static TMC_StatusTypeDef ADC_WaitFlagStateUntilTimeout(ADC_HandleTypeDef* hadc, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart)
{
	while((((ADC->ADCSTS& Flag) == (Flag)) ? SET : RESET) != State)
	{
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
			{
				hadc->State= TMC_ADC_STATE_TIMEOUT;

				return TMC_TIMEOUT;
			}
		}
	}
	return TMC_OK;
}

/**
  * @brief  Enables ADC and starts conversion of the regular channels.
  * @param[in]  hadc: pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
  * @param[in]  Timeout: the value of ADC's Timer 
  * @retval TMC_StatusTypeDef status
  * @note 
  */
TMC_StatusTypeDef TMC_ADC_Start(ADC_HandleTypeDef* hadc,uint16_t * pData,uint32_t Size,uint32_t Timeout)
{
	uint32_t tickstart = 0U;
	if((hadc->State!= TMC_ADC_STATE_READY)||(hadc == NULL)||(pData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	hadc->State = TMC_ADC_STATE_BUSY;
	hadc->pBuffPtr = pData;
	hadc->XferCount = Size;
	hadc->XferSize = Size;
	/* Clear the ADC Status */
	ADC_ClearStatus(ADC_STS_OVER_FLOW|ADC_STS_RBF);
	/* ENABLE the ADC  */
	ADC_Cmd(ENABLE);
	while(hadc->XferCount)
	{
        tickstart = TMC_GetTick();
		while(ADC_WaitFlagStateUntilTimeout(hadc,ADC_STS_RBF,SET,Timeout,tickstart))
		{
			if(ADC_GetStatus(ADC_STS_OVER_FLOW)== SET)
			{
				ADC_ClearStatus(ADC_STS_OVER_FLOW);
				hadc->State = TMC_ADC_STATE_ERROR;
				hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
				return TMC_ERROR;
			}
			else
			{
				hadc->State = TMC_ADC_STATE_TIMEOUT;
				hadc->ErrorCode = TMC_ADC_ERROR_NONE;
				return TMC_TIMEOUT;
			}
		}
		*(uint16_t*)hadc->pBuffPtr = ADC_GetData();
		hadc->pBuffPtr++;
		hadc->XferCount--;
	}

	hadc->State= TMC_ADC_STATE_READY;
	return TMC_OK;
}

/**
  * @brief  Enables ADC and starts conversion of the regular channels.
			This function 
  * @param[in]  hadc: pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
* @param[in]  CurrentChannel: Which channel is Select
  * @param[in]  Timeout: the value of ADC's Timer 
  * @retval TMC_StatusTypeDef status
  * @note 
  */
TMC_StatusTypeDef TMC_ADC_SingleChannelStart(ADC_HandleTypeDef* hadc, uint16_t * pData, uint32_t Size, \
										     ADC_ConvertingChannelx ChannelSelected, uint32_t Timeout)
{
	uint32_t tickstart = 0U;
	volatile uint8_t CurrentConvertingChannel = 0;
	if((hadc->State!= TMC_ADC_STATE_READY)||(hadc == NULL)||(pData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	hadc->State = TMC_ADC_STATE_BUSY;
	hadc->pBuffPtr = pData;
	hadc->XferCount = Size;
	hadc->XferSize = Size;
	/* Clear the ADC Status */
	ADC_ClearStatus(ADC_STS_OVER_FLOW|ADC_STS_RBF);
	/* ENABLE the ADC  */
	ADC_Cmd(ENABLE);
	while(hadc->XferCount)
	{
		CurrentConvertingChannel = ADC->ADCCVTIO;
        tickstart = TMC_GetTick();
		while(ADC_WaitFlagStateUntilTimeout(hadc,ADC_STS_RBF,SET,Timeout,tickstart))
		{
			if(ADC_GetStatus(ADC_STS_OVER_FLOW)== SET)
			{
				ADC_ClearStatus(ADC_STS_OVER_FLOW);
				hadc->State = TMC_ADC_STATE_ERROR;
				hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
				return TMC_ERROR;
			}
			else
			{
				hadc->State = TMC_ADC_STATE_TIMEOUT;
				hadc->ErrorCode = TMC_ADC_ERROR_NONE;
				return TMC_TIMEOUT;
			}
		}
		
		if (CurrentConvertingChannel == ChannelSelected)
		{
			*((uint16_t*)hadc->pBuffPtr) = ADC_GetData();
			hadc->pBuffPtr++;
			hadc->XferCount--;
		}

		else  //Other channel only read,do not care the data.
		{
			ADC_GetData();
		}
	}

	hadc->State = TMC_ADC_STATE_READY;
	return TMC_OK;
}


/**
  * @brief  Disable ADC and Stop conversion of the regular channels.
  * @param[in]  hadc: pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval TMC_StatusTypeDef status
  * @note 
  */
TMC_StatusTypeDef TMC_ADC_Stop(ADC_HandleTypeDef* hadc)
{

	/* Clear the ADC Status */
	ADC_ClearStatus(ADC_STS_OVER_FLOW|ADC_STS_RBF);
	/* DISABLE the ADC  */
	ADC_Cmd(DISABLE);
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	hadc->State = TMC_ADC_STATE_READY;
	return TMC_OK;
}

/**
  * @brief  Enables ADC and starts conversion of the regular channels with Interrupt
  * @param[in]  hadc: pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
  * @retval TMC_StatusTypeDef status
  * @note 
  */
TMC_StatusTypeDef TMC_ADC_Start_IT(ADC_HandleTypeDef* hadc,uint16_t * pData,uint32_t Size)
{
	if((hadc->State!= TMC_ADC_STATE_READY)||(hadc == NULL)||(pData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	if(hadc->State == TMC_ADC_STATE_BUSY)
	{
		return TMC_BUSY;
	}
	hadc->State = TMC_ADC_STATE_BUSY;

	hadc->pBuffPtr = pData;
	hadc->XferCount = Size;
	hadc->XferSize = Size;
	hadc->ADC_ISR = TMC_ADC_ISR;
	ADC_ConfigIT(ADC_IT_RBF|ADC_IT_OVER_FLOW,ENABLE);
	ADC_Cmd(ENABLE);
	return TMC_OK;
}

/**
  * @brief  Enables ADC and starts conversion of the regular channels with Interrupt
  * @param[in]  hadc: pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
  * @retval TMC_StatusTypeDef status
  * @note 
  */
TMC_StatusTypeDef TMC_ADC_Stop_IT(ADC_HandleTypeDef* hadc)
{
	if(hadc == NULL)
	{
		return TMC_ERROR;
	}
	ADC_Cmd(DISABLE);
	ADC_ConfigIT(ADC_IT_RBF|ADC_IT_OVER_FLOW,DISABLE);
	hadc->State = TMC_ADC_STATE_READY;
	return TMC_OK;
}

/**
  * @brief  Handle ADC interrupt request.
  * @param  hspi pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for the specified ADC module.
  * @retval None
  */
void TMC_ADC_IRQHandler(ADC_HandleTypeDef *hadc)
{
	if(hadc->State == TMC_ADC_STATE_BUSY)
	{
		hadc->ADC_ISR(hadc);
	}
	else
	{
		ADC_ClearStatus(ADC_STS_RBF|ADC_STS_OVER_FLOW);
		ADC_ConfigIT(ADC_IT_RBF|ADC_IT_OVER_FLOW,DISABLE);
		hadc->State = TMC_ADC_STATE_ERROR;
		TMC_ADCx_ErrorCallback(hadc);
	}
}

/**
  * @brief  ADC  handler for  completed in Interrupt mode.
  * @param  hadc pointer to a __ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
static void TMC_ADC_ISR(struct __ADC_HandleTypeDef *hadc)
{
	if(ADC_GetStatus(ADC_STS_RBF) == SET)
	{
		ADC_ClearStatus(ADC_STS_RBF);
		*hadc->pBuffPtr = ADC_GetData();
		hadc->pBuffPtr++;
		hadc->XferCount--;
		if(hadc->XferCount==0)
		{
			hadc->ErrorCode = TMC_ADC_ERROR_NONE;
			hadc->State = TMC_ADC_STATE_READY;
			TMC_ADCx_RxCpltCallback(hadc);
		}
	}
	if(ADC_GetStatus(ADC_STS_OVER_FLOW) == SET)
	{
		ADC_ClearStatus(ADC_STS_OVER_FLOW);
		hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
		hadc->State = TMC_ADC_STATE_ERROR;
		TMC_ADCx_ErrorCallback(hadc);
	}
}

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param  ADC_Channelx: 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6 
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  LLI  the address of LLI array,if LLI is 0 ,ADC is Single acquisition
  * @retval  TMC_StatusTypeDef
  * @note one DMA's channel can only correspond to one ADC's channel
  */
TMC_StatusTypeDef TMC_ADC_Start_HDMA0_REQUEST_ADC1_IT(ADC_HandleTypeDef *hadc,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI)
{
	if(hadc == NULL)
	{
		return TMC_ERROR;
	}
	if(pData == NULL)
	{
		return TMC_ERROR;
	}
	if(hadc->hdma0->Init.RequestSelect != DMA_REQUEST_ADC1)
	{
		return TMC_ERROR;
	}
	if(hadc->hdma0->State == TMC_DMA_STATE_BUSY)
	{
		return TMC_ERROR;
	}
	
	hadc->State	   = TMC_ADC_STATE_READY;//TMC_ADC_STATE_BUSY;
	hadc->ErrorCode   = TMC_ADC_ERROR_NONE;
	hadc->hdma0->Instance = DMA_Channel0;
	/* Set the SPI Rx DMA transfer complete callback */
	hadc->hdma0->XferCpltCallback = TMC_ADC_HDMA0_ReceiveCplt;
	/* Set the DMA error callback */
	hadc->hdma0->XferErrorCallback = TMC_ADC_HDMA0_Error;
	hadc->hdma0->Parent = hadc;
	hadc->hdma0->Instance->LLI = LLI;
	ADC_DMA_Cmd(ADC_DMA_CHANNEL1,ENABLE);
	ADC_Config_DMA_Channel1(ADC_Channelx);
	
	/* Check if the SPI is already enabled */
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(hadc->hdma0, (uint32_t)&ADC->ADCDAT, (uint32_t)pData, Size);
	
	
	/* Process Unlocked */
	//__HAL_UNLOCK(hadc);
	
	return TMC_OK;
}

/**
  * @brief  Stop DMA Channel0
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval  TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_ADC_Stop_HDMA0_REQUEST_ADC1_IT(ADC_HandleTypeDef *hadc)
{

	/* DISABLE the Rx DMA Stream */
	TMC_DMA_Abort_IT(hadc->hdma0);
	
	ADC_DMA_Cmd(ADC_DMA_CHANNEL1,DISABLE);

	/* Check if the SPI is already enabled */
	ADC_Cmd(DISABLE);
	/* Process Unlocked */
	//__HAL_UNLOCK(hadc);
	hadc->State	   = TMC_ADC_STATE_READY;
	hadc->ErrorCode   = TMC_ADC_ERROR_NONE;
	return TMC_OK;
}

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param  ADC_Channelx: 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6 
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  LLI  the address of LLI array,if LLI is 0 ,ADC is Single acquisition
  * @retval  TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_ADC_Start_HDMA1_REQUEST_ADC2_IT(ADC_HandleTypeDef *hadc,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI)
{
	if(hadc == NULL)
	{
		return TMC_ERROR;
	}
	if(pData == NULL)
	{
		return TMC_ERROR;
	}
	if(hadc->hdma1->Init.RequestSelect != DMA_REQUEST_ADC2)
	{
		return TMC_ERROR;
	}
	if(hadc->hdma1->State == TMC_DMA_STATE_BUSY)
	{
		return TMC_ERROR;
	}

	hadc->State	   = TMC_ADC_STATE_READY;
	hadc->ErrorCode   = TMC_ADC_ERROR_NONE;
	hadc->hdma1->Instance = DMA_Channel1;
	/* Set the SPI Rx DMA transfer complete callback */
	hadc->hdma1->XferCpltCallback = TMC_ADC_HDMA1_ReceiveCplt;
	/* Set the DMA error callback */
	hadc->hdma1->XferErrorCallback = TMC_ADC_HDMA1_Error;
	hadc->hdma1->Parent = hadc;
	
	hadc->hdma1->Instance->LLI = LLI;
	ADC_DMA_Cmd(ADC_DMA_CHANNEL2,ENABLE);
	ADC_Config_DMA_Channel2(ADC_Channelx);
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(hadc->hdma1, (uint32_t)&ADC->ADCDAT, (uint32_t)pData, Size);
	
	/* Check if the SPI is already enabled */
	//ADC_Cmd(ENABLE);
	/* Process Unlocked */
	//__HAL_UNLOCK(hadc);
	
	return TMC_OK;
}

/**
  * @brief  Stop DMA Channel1
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval  TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_ADC_Stop_HDMA1_REQUEST_ADC2_IT(ADC_HandleTypeDef *hadc)
{
	

	TMC_DMA_Abort_IT(hadc->hdma1);	
	
	ADC_DMA_Cmd(ADC_DMA_CHANNEL2,DISABLE);
	
	/* Check if the SPI is already enabled */
	ADC_Cmd(DISABLE);
	/* Process Unlocked */
//	__HAL_UNLOCK(hadc);
	hadc->State	   = TMC_ADC_STATE_READY;
	hadc->ErrorCode   = TMC_ADC_ERROR_NONE;
	return TMC_OK;
}
/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param  ADC_Channelx: 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL2 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6 
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  LLI  the address of LLI array,if LLI is 0 ,ADC is Single acquisition
  * @retval  TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_ADC_Start_HDMA2_REQUEST_ADC3_IT(ADC_HandleTypeDef *hadc,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI)
{
	if(hadc == NULL)
	{
		return TMC_ERROR;
	}
	if(pData == NULL)
	{
		return TMC_ERROR;
	}
	if(hadc->hdma2->Init.RequestSelect != DMA_REQUEST_ADC3)
	{
		return TMC_ERROR;
	}
	if(hadc->hdma2->State == TMC_DMA_STATE_BUSY)
	{
		return TMC_ERROR;
	}

	hadc->State	   = TMC_ADC_STATE_READY;
	hadc->ErrorCode   = TMC_ADC_ERROR_NONE;

	hadc->hdma2->Instance = DMA_Channel2;
	/* Set the SPI Rx DMA transfer complete callback */
	hadc->hdma2->XferCpltCallback = TMC_ADC_HDMA2_ReceiveCplt;

	/* Set the DMA error callback */
	hadc->hdma2->XferErrorCallback = TMC_ADC_HDMA2_Error;
	hadc->hdma2->Parent = hadc;
	hadc->hdma2->Instance->LLI = LLI;
	
	
	ADC_DMA_Cmd(ADC_DMA_CHANNEL3,ENABLE);
	ADC_Config_DMA_Channel3(ADC_Channelx);
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(hadc->hdma2, (uint32_t)&ADC->ADCDAT, (uint32_t)pData, Size);
	/* Check if the SPI is already enabled */
	//ADC_Cmd(ENABLE);

	
	return TMC_OK;
}

/**
  * @brief  Stop DMA Channel2
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval  TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_ADC_Stop_HDMA2_REQUEST_ADC3_IT(ADC_HandleTypeDef *hadc)
{

	TMC_DMA_Abort_IT(hadc->hdma2);	

	ADC_DMA_Cmd(ADC_DMA_CHANNEL3,DISABLE);
	/* Check if the SPI is already enabled */
	ADC_Cmd(DISABLE);

	hadc->State	   = TMC_ADC_STATE_READY;
	hadc->ErrorCode   = TMC_ADC_ERROR_NONE;
	return TMC_OK;
}

/**
  * @brief  DMA ADC0 half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_ADC_HDMA0_ReceiveCplt(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc= ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_READY;
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	TMC_ADC_HDMA0_RxCpltCallback(hadc);
}

/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_ADC_HDMA0_Error(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_ERROR;
	hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
	TMC_ADC_HDMA0_ErrorCallback(hadc);
}

/**
  * @brief  DMA ADC0 half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_ADC_HDMA1_ReceiveCplt(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc= ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_READY;
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	TMC_ADC_HDMA1_RxCpltCallback(hadc);
}

/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_ADC_HDMA1_Error(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_ERROR;
	hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
	ADC_Cmd(DISABLE);
	TMC_ADC_HDMA1_ErrorCallback(hadc);
}

/**
  * @brief  DMA ADC0 half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_ADC_HDMA2_ReceiveCplt(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc= ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_READY;
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	ADC_Cmd(DISABLE);
	TMC_ADC_HDMA2_RxCpltCallback(hadc);
}

/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_ADC_HDMA2_Error(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_ERROR;
	hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
	ADC_Cmd(DISABLE);
	TMC_ADC_HDMA2_ErrorCallback(hadc);
}
/**
  * @brief  Initialize the ADC MSP.
  * @param  hspi pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
__weak void TMC_ADC_MspInit(ADC_HandleTypeDef* hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  DeInitialize the ADC MSP.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
__weak void TMC_ADC_MspDeInit(ADC_HandleTypeDef* hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}


/**
  * @brief  TMC ADC completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
__weak void TMC_ADCx_RxCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_RxCpltCallback should be implemented in the user file
	*/
}


/**
  * @brief TMC ADC Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
__weak void TMC_ADCx_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_ErrorCallback should be implemented in the user file
	*/
}

/**
  * @brief  TMC ADC0 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
__weak void TMC_ADC_HDMA0_RxCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_RxCpltCallback should be implemented in the user file
	*/
}


/**
  * @brief TMC ADC0 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
__weak void TMC_ADC_HDMA0_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_ErrorCallback should be implemented in the user file
	*/
}

/**
  * @brief  TMC ADC1 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
__weak void TMC_ADC_HDMA1_RxCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_RxCpltCallback should be implemented in the user file
	*/
}


/**
  * @brief TMC ADC1 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
__weak void TMC_ADC_HDMA1_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_ErrorCallback should be implemented in the user file
	*/
}


/**
  * @brief  TMC ADC2 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
__weak void TMC_ADC_HDMA2_RxCpltCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_RxCpltCallback should be implemented in the user file
	*/
}


/**
  * @brief TMC ADC2 Error callback.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @retval None
  */
__weak void TMC_ADC_HDMA2_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hadc);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_ErrorCallback should be implemented in the user file
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
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

