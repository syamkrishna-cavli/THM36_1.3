/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_mag.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_dma.h"
#include "tmc_adc.h"
#include "tmc_mag.h"
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_MAG
  * @brief MAG TMC modules driver 
  * @{
  */

  
/** @defgroup TMC_MAG_private_defines TMC MAG private defines
  * @{
  */
static void TMC_Mag_HDMA0_ReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_Mag_HDMA0_Error(DMA_HandleTypeDef *hdma);
static void TMC_Mag_HDMA1_ReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_Mag_HDMA1_Error(DMA_HandleTypeDef *hdma);
static void TMC_Mag_HDMA2_ReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_Mag_HDMA2_Error(DMA_HandleTypeDef *hdma);
/**
  * @}
  */ 
  
/** @defgroup TMC_MAG_Exported_Function TMC MAG Exported Function
  * @brief MAG TMC modules driver
  * @{
  */

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  hadc pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for ADC module.
  * @param  ADC_Channelx: 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL5 
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL6
  *		@arg	ADC_DMA_CONFIG_ADC_CHANNEL7 
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  LLI  the address of LLI array,if LLI is 0 ,ADC is Single acquisition
  * @retval  TMC_StatusTypeDef
  * @note one DMA's channel can only correspond to one ADC's channel
  */
TMC_StatusTypeDef TMC_Mag_Start_HDMA0_REQUEST_ADC1_IT(Mag_HandleTypeDef *hmag,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI)
{
	if(hmag == NULL)
	{
		return TMC_ERROR;
	}
	if(pData == NULL)
	{
		return TMC_ERROR;
	}
	if(hmag->hdma0->Init.RequestSelect != DMA_REQUEST_ADC1)
	{
		return TMC_ERROR;
	}
	if(hmag->hdma0->State == TMC_DMA_STATE_BUSY)
	{
		return TMC_ERROR;
	}
	
	hmag->State  = TMC_MAG_STATE_BUSY;

	/* Set the SPI Rx DMA transfer complete callback */
	hmag->hdma0->XferCpltCallback = TMC_Mag_HDMA0_ReceiveCplt;
	/* Set the DMA error callback */
	hmag->hdma0->XferErrorCallback = TMC_Mag_HDMA0_Error;
	hmag->hdma0->Parent = hmag;
	hmag->hdma0->Instance->LLI = LLI;
	ADC_DMA_Cmd(ADC_DMA_CHANNEL1,ENABLE);
	ADC_Config_DMA_Channel1(ADC_Channelx);
	
	/* Check if the SPI is already enabled */
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(hmag->hdma0, (uint32_t)&MAG1->MAGDAT, (uint32_t)pData, Size);	
	return TMC_OK;
}

/**
  * @brief  DMA ADC0 half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_Mag_HDMA0_ReceiveCplt(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc= ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_READY;
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	ADC_Cmd(DISABLE);
	TMC_ADC_HDMA0_RxCpltCallback(hadc);
}

/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_Mag_HDMA0_Error(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc = ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_ERROR;
	hadc->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
	ADC_Cmd(DISABLE);
	TMC_ADC_HDMA0_ErrorCallback(hadc);
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
TMC_StatusTypeDef TMC_Mag_Start_HDMA1_REQUEST_ADC2_IT(Mag_HandleTypeDef *hmag,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI)
{
	if(hmag == NULL)
	{
		return TMC_ERROR;
	}
	if(pData == NULL)
	{
		return TMC_ERROR;
	}
	if(hmag->hdma1->Init.RequestSelect != DMA_REQUEST_ADC2)
	{
		return TMC_ERROR;
	}
	if(hmag->hdma1->State == TMC_DMA_STATE_BUSY)
	{
		return TMC_ERROR;
	}

	hmag->State  = TMC_MAG_STATE_BUSY;
	/* Set the SPI Rx DMA transfer complete callback */
	hmag->hdma1->XferCpltCallback = TMC_Mag_HDMA1_ReceiveCplt;
	/* Set the DMA error callback */
	hmag->hdma1->XferErrorCallback = TMC_Mag_HDMA1_Error;
	hmag->hdma1->Parent = hmag;
	
	hmag->hdma1->Instance->LLI = LLI;
	ADC_DMA_Cmd(ADC_DMA_CHANNEL2,ENABLE);
	ADC_Config_DMA_Channel2(ADC_Channelx);
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(hmag->hdma1, (uint32_t)&MAG2->MAGDAT, (uint32_t)pData, Size);
	
	/* Check if the SPI is already enabled */
	//ADC_Cmd(ENABLE);
	/* Process Unlocked */
	//__HAL_UNLOCK(hadc);
	
	return TMC_OK;
}


/**
  * @brief  DMA ADC0 half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_Mag_HDMA1_ReceiveCplt(DMA_HandleTypeDef *hdma)
{
	ADC_HandleTypeDef *hadc= ( ADC_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hadc->State = TMC_ADC_STATE_READY;
	hadc->ErrorCode = TMC_ADC_ERROR_NONE;
	ADC_Cmd(DISABLE);
	TMC_ADC_HDMA1_RxCpltCallback(hadc);
}

/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_Mag_HDMA1_Error(DMA_HandleTypeDef *hdma)
{
	Mag_HandleTypeDef *hmag= ( Mag_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hmag->State = TMC_MAG_STATE_ERROR;
	//hmag->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
	ADC_Cmd(DISABLE);
	TMC_Mag_HDMA1_ErrorCallback(hmag);
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
TMC_StatusTypeDef TMC_Mag_Start_HDMA2_REQUEST_ADC3_IT(Mag_HandleTypeDef *hmag,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI)
{
	if(hmag == NULL)
	{
		return TMC_ERROR;
	}
	if(pData == NULL)
	{
		return TMC_ERROR;
	}
	if(hmag->hdma2->Init.RequestSelect != DMA_REQUEST_ADC3)
	{
		return TMC_ERROR;
	}
	if(hmag->hdma2->State == TMC_DMA_STATE_BUSY)
	{
		return TMC_ERROR;
	}

	hmag->State = TMC_MAG_STATE_BUSY;


	//hadc->hdma2->Instance = DMA_Channel2;
	/* Set the SPI Rx DMA transfer complete callback */
	hmag->hdma2->XferCpltCallback = TMC_Mag_HDMA2_ReceiveCplt;

	/* Set the DMA error callback */
	hmag->hdma2->XferErrorCallback = TMC_Mag_HDMA2_Error;
	hmag->hdma2->Parent = hmag;
	hmag->hdma2->Instance->LLI = LLI;
	
	
	//ADC_DMA_Cmd(ADC_DMA_CHANNEL3,ENABLE);
	ADC_Config_DMA_Channel3(ADC_Channelx);
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(hmag->hdma2, (uint32_t)&MAG3->MAGDAT, (uint32_t)pData, Size);
	/* Check if the SPI is already enabled */
	//ADC_Cmd(ENABLE);

	
	return TMC_OK;
}


/**
  * @brief  DMA ADC0 half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_Mag_HDMA2_ReceiveCplt(DMA_HandleTypeDef *hdma)
{
	Mag_HandleTypeDef *hmag= ( Mag_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hmag->State = TMC_MAG_STATE_READY;
	//hmag->ErrorCode = TMC_ADC_ERROR_NONE;
	ADC_Cmd(DISABLE);
	TMC_Mag_HDMA2_RxCpltCallback(hmag);
}

/**
  * @brief  DMA ADC communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_Mag_HDMA2_Error(DMA_HandleTypeDef *hdma)
{
	Mag_HandleTypeDef *hmag= ( Mag_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hmag->State = TMC_MAG_STATE_ERROR;
	//hmag->ErrorCode = TMC_ADC_ERROR_OVER_FLOW;
	ADC_Cmd(DISABLE);
	TMC_Mag_HDMA2_ErrorCallback(hmag);
}

/**
  * @brief  TMC ADC0 completed callback.
  * @param  ADC pointer to a ADC_HandleTypeDef structure that contains
  *			   the configuration information for  ADC module.
  * @retval None
  */
__weak void TMC_Mag_HDMA0_RxCpltCallback(Mag_HandleTypeDef *hmag)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hmag);
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
__weak void TMC_Mag_HDMA0_ErrorCallback(Mag_HandleTypeDef *hmag)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hmag);
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
__weak void TMC_Mag_HDMA1_RxCpltCallback(Mag_HandleTypeDef *hmag)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hmag);
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
__weak void TMC_Mag_HDMA1_ErrorCallback(Mag_HandleTypeDef *hmag)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hmag);
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
__weak void TMC_Mag_HDMA2_RxCpltCallback(Mag_HandleTypeDef *hmag)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hmag);
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
__weak void TMC_Mag_HDMA2_ErrorCallback(Mag_HandleTypeDef *hmag)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hmag);
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


