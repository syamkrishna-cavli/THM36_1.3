/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_dma.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
 
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_dma.h"

/** @addtogroup thm36_1.3
  * @{
  */
  
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */

/** @defgroup TMC_DMA 
  * @brief DMA TMC modules driver
  * @{
  */

/** @defgroup TMC_DMA_Private_Defines TMC DMA Private Defines
  * @{
  */
#define DMA_IT_TC_OFFS			0x00000001UL
#define DMA_IT_ERR_OFFS			0x00000100UL

/**
  * @}
  */

/** @addtogroup DMA_Private_Constants
 * @{
 */
 #define TMC_TIMEOUT_DMA_ABORT	5U  /* 5 ms */
/**
  * @}
  */

/** @addtogroup TMC_DMA_Private_Functions
  * @{
  */

static uint32_t DMA_CalcBitShift(DMA_HandleTypeDef *hdma);
	
/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup TMC_DMA_Exported_Functions TMC DMA Exported Functions
  * @{
  */


/**
  * @brief  Initialize the DMA according to the specified
  *		 parameters in the DMA_InitTypeDef and create the associated handle.
  * @param[in]  hdma: Pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA Stream.  
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_Init(DMA_HandleTypeDef *hdma)
{
	uint32_t tickstart = TMC_GetTick();
	
	/* Check the DMA peripheral state */
	if(hdma == NULL)
	{
		return TMC_ERROR;
	}
  

	/* Change DMA peripheral state */
	hdma->State = TMC_DMA_STATE_BUSY;
  
	/* Disable the peripheral */
	DMA_Cmd(hdma->Instance, DISABLE);
	
	/* Check if the DMA channel is effectively disabled */
	while(DMA_GetCmdStatus(hdma->Instance) != RESET)
	{
		/* Check for the Timeout */
		if((TMC_GetTick() - tickstart ) > TMC_TIMEOUT_DMA_ABORT)
		{
			/* Update error code */
			hdma->ErrorCode = TMC_DMA_ERROR_TIMEOUT;


			/* Change the DMA state */
			hdma->State = TMC_DMA_STATE_TIMEOUT;

			return TMC_TIMEOUT;
		}
	}
	
	/* init the DMA channel */
	DMA_Init(hdma->Instance, &hdma->Init);
	
	/* Initialize StreamIndex parameters to be used */
	hdma->StreamIndex = DMA_CalcBitShift(hdma);
	
	/* Clear all interrupt flags */
	DMA_ClearITPendingBit(DMA_IT_TC_OFFS << hdma->StreamIndex);
	DMA_ClearITPendingBit(DMA_IT_ERR_OFFS << hdma->StreamIndex);
	
	/* Initialize the error code */
	hdma->ErrorCode = TMC_DMA_ERROR_NONE;
																				 
	/* Initialize the DMA state */
	hdma->State = TMC_DMA_STATE_READY;

	return TMC_OK;
	
}

/**
  * @brief  DeInitializes the DMA peripheral 
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA Stream.  
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_DeInit(DMA_HandleTypeDef *hdma)
{
	/* Check the DMA peripheral state */
	if(hdma == NULL)
	{
		return TMC_ERROR;
	}

	/* Check the DMA peripheral state */
	if(hdma->State == TMC_DMA_STATE_BUSY)
	{
		/* Return error status */
		return TMC_BUSY;
	}

	/* Reset DMA channel register */
	DMA_DeInit(hdma->Instance);
	
	/* Initialize the error code */
	hdma->ErrorCode = TMC_DMA_ERROR_NONE;

	/* Initialize the DMA state */
	hdma->State = TMC_DMA_STATE_RESET;



	return TMC_OK;
}

/**
  * @brief  Starts the DMA Transfer.
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *				   the configuration information for the specified DMA Stream.
  * @param[in]  SrcAddress: The source memory Buffer address
  * @param[in]  DstAddress: The destination memory Buffer address
  * @param[in]  DataLength: The length of data to be transferred from source to destination
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	TMC_StatusTypeDef status = TMC_OK;



	if(TMC_DMA_STATE_READY == hdma->State)
	{
		/* Change DMA peripheral state */
		hdma->State = TMC_DMA_STATE_BUSY;

		/* Initialize the error code */
		hdma->ErrorCode = TMC_DMA_ERROR_NONE;

		/* Configure the source, destination address and the data length */
		DMA_SetAddress(hdma->Instance, SrcAddress, DstAddress, DataLength);

		/* Enable the Peripheral */
		DMA_Cmd(hdma->Instance,ENABLE);
	}
	else
	{


		/* Return error status */
		status = TMC_BUSY;
	} 
	return status; 
}

/**
  * @brief  Start the DMA Transfer with interrupt enabled.
  * @param  hdma	   pointer to a DMA_HandleTypeDef structure that contains
  *					 the configuration information for the specified DMA Stream.  
  * @param  SrcAddress The source memory Buffer address
  * @param  DstAddress The destination memory Buffer address
  * @param  DataLength The length of data to be transferred from source to destination
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	TMC_StatusTypeDef status = TMC_OK;



	if(TMC_DMA_STATE_READY == hdma->State)
	{
		/* Change DMA peripheral state */
		hdma->State = TMC_DMA_STATE_BUSY;

		/* Initialize the error code */
		hdma->ErrorCode = TMC_DMA_ERROR_NONE;
		
		/* Configure the source, destination address and the data length */
		DMA_SetAddress(hdma->Instance, SrcAddress, DstAddress, DataLength);

		/* Clear all interrupt flags at correct offset within the register */
		DMA_ClearITPendingBit(DMA_IT_TC_OFFS << hdma->StreamIndex);
		DMA_ClearITPendingBit(DMA_IT_ERR_OFFS << hdma->StreamIndex);

		/* Enable Common interrupts*/
		DMA_ITConfig(hdma->Instance, DMA_IT_TC, ENABLE);
		DMA_ITConfig(hdma->Instance, DMA_IT_ERR, ENABLE);

		/* Enable the Peripheral */
		DMA_Cmd(hdma->Instance,ENABLE);
	}
	else
	{


		/* Return error status */
		status = TMC_BUSY;
	}

	return status;
}


/**
  * @brief  Aborts the DMA Transfer.
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *				 the configuration information for the specified DMA channel.
  *				   
  * @note  After disabling a DMA channel, a check for wait until the DMA channel is 
  *		effectively disabled is added. If a Stream is disabled 
  *		while a data transfer is ongoing, the current data will be transferred
  *		and the channel will be effectively disabled only after the transfer of
  *		this single data is finished.  
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_Abort(DMA_HandleTypeDef *hdma)
{
	uint32_t tickstart = TMC_GetTick();

	if(hdma->State != TMC_DMA_STATE_BUSY)
	{
		hdma->ErrorCode = TMC_DMA_ERROR_NO_XFER;


		return TMC_ERROR;
	}
	else
	{
		/* Disable all the transfer interrupts */
		DMA_ITConfig(hdma->Instance, DMA_IT_TC, DISABLE);
		DMA_ITConfig(hdma->Instance, DMA_IT_ERR, DISABLE);

		/* Disable the channel */
		DMA_Cmd(hdma->Instance,DISABLE);

		/* Check if the DMA channel is effectively disabled */
		while(DMA_GetCmdStatus(hdma->Instance) != RESET)
		{
			/* Check for the Timeout */
			if((TMC_GetTick() - tickstart ) > TMC_TIMEOUT_DMA_ABORT)
			{
				/* Update error code */
				hdma->ErrorCode = TMC_DMA_ERROR_TIMEOUT;


				/* Change the DMA state */
				hdma->State = TMC_DMA_STATE_TIMEOUT;

				return TMC_TIMEOUT;
			}
		}

		/* Clear all interrupt flags at correct offset within the register */
		DMA_ClearITPendingBit(DMA_IT_TC_OFFS << hdma->StreamIndex);
		DMA_ClearITPendingBit(DMA_IT_ERR_OFFS << hdma->StreamIndex);



		/* Change the DMA state*/
		hdma->State = TMC_DMA_STATE_READY;
	}
	return TMC_OK;
}

/**
  * @brief  Aborts the DMA Transfer in Interrupt mode.
  * @param  hdma   pointer to a DMA_HandleTypeDef structure that contains
  *				 the configuration information for the specified DMA Stream.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_Abort_IT(DMA_HandleTypeDef *hdma)
{
	if(hdma->State != TMC_DMA_STATE_BUSY)
	{
		hdma->ErrorCode = TMC_DMA_ERROR_NO_XFER;
		return TMC_ERROR;
	}
	else
	{
		/* Set Abort State  */
		hdma->State = TMC_DMA_STATE_ABORT;

		/* Disable the stream */
		DMA_Cmd(hdma->Instance,DISABLE);
	}
	return TMC_OK;
}

/**
  * @brief  Polling for transfer complete.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA Stream.
  * @param  Timeout	   Timeout duration.
  * @param  CompleteLevel Specifies the DMA level complete.
  * @note   The polling mode is kept in this version for legacy. it is recommanded to use the IT model instead.
  *		 This model could be used for debug purpose.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t Timeout)
{
	uint32_t complete_sts,error_sts;
	uint32_t tickstart = TMC_GetTick(); 

	if(TMC_DMA_STATE_BUSY != hdma->State)
	{
		/* No transfer ongoing */
		hdma->ErrorCode = TMC_DMA_ERROR_NO_XFER;
		return TMC_ERROR;
	}

//	/* Polling mode not supported in circular mode and double buffering mode */
//	if ((hdma->Instance->LLI ) != RESET)
//	{
//		hdma->ErrorCode = TMC_DMA_ERROR_NOT_SUPPORTED;
//		return TMC_ERROR;
//	}
  
	/* calc the transfer complete flag and error flag bits*/
	complete_sts = DMA_IT_TC_OFFS << hdma->StreamIndex;
	error_sts = DMA_IT_ERR_OFFS << hdma->StreamIndex;

	while(((DMA_GetFlagStatus(complete_sts)) == RESET) && ((hdma->ErrorCode & TMC_DMA_ERROR_TE) == RESET))
	{
		/* Check for the Timeout (Not applicable in circular mode)*/
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U)||((TMC_GetTick() - tickstart ) > Timeout))
			{
				/* Update error code */
				hdma->ErrorCode = TMC_DMA_ERROR_TIMEOUT;

				/* Change the DMA state */
				hdma->State = TMC_DMA_STATE_READY;

				return TMC_TIMEOUT;
			}
		}

		if(DMA_GetFlagStatus(error_sts) != RESET)
		{
			/* Update error code */
			hdma->ErrorCode |= TMC_DMA_ERROR_TE;
		}
	}
  
	if(hdma->ErrorCode != TMC_DMA_ERROR_NONE)
	{
		if((hdma->ErrorCode & TMC_DMA_ERROR_TE) != RESET)
		{
			TMC_DMA_Abort(hdma);

			/* Change the DMA state */
			hdma->State= TMC_DMA_STATE_READY;

			return TMC_ERROR;
		}
	}

	hdma->State = TMC_DMA_STATE_READY;

	return TMC_OK;
}

/**
  * @brief  Handles DMA interrupt request.
  * @param  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA Stream.  
  * @retval None
  */
void TMC_DMA_IRQHandler(DMA_HandleTypeDef *hdma)
{
	uint32_t complete_sts,error_sts;
	__IO uint32_t count = 0U;
	uint32_t timeout = 20000;

	/* calculate DMA base and stream number */
	complete_sts = DMA_IT_TC_OFFS << hdma->StreamIndex;
	error_sts = DMA_IT_ERR_OFFS << hdma->StreamIndex;

	/* Transfer Error Interrupt management ***************************************/
	if (DMA_GetITStatus(error_sts) != RESET)
	{
		if(DMA_GetFlagStatus(error_sts) != RESET)
		{
			/* Disable the transfer error interrupt */
			DMA_ITConfig(hdma->Instance, DMA_IT_ERR, DISABLE);

			/* Clear the transfer error flag */
			DMA_ClearITPendingBit(error_sts);

			/* Update error code */
			hdma->ErrorCode |= TMC_DMA_ERROR_TE;
		}
	}

	/* Transfer Complete Interrupt management ***********************************/
	if (DMA_GetITStatus(complete_sts) != RESET)
	{
		if(DMA_GetFlagStatus(complete_sts) != RESET)
		{
			/* Clear the transfer complete flag */
			DMA_ClearITPendingBit(complete_sts);

			if(TMC_DMA_STATE_ABORT == hdma->State)
			{
				/* Disable all the transfer interrupts */
				DMA_ITConfig(hdma->Instance, DMA_IT_TC, DISABLE);
				DMA_ITConfig(hdma->Instance, DMA_IT_ERR, DISABLE);

				/* Clear all interrupt flags at correct offset within the register */
				DMA_ClearITPendingBit(complete_sts);
				DMA_ClearITPendingBit(error_sts);


				/* Change the DMA state */
				hdma->State = TMC_DMA_STATE_READY;

				if(hdma->XferAbortCallback != NULL)
				{
					hdma->XferAbortCallback(hdma);
				}
				return;
			}

			/* Disable the transfer complete interrupt if the DMA mode is not CIRCULAR */
			/* Disable the transfer complete interrupt */
			DMA_ITConfig(hdma->Instance, DMA_IT_TC, DISABLE);

			/* Change the DMA state */
			hdma->State = TMC_DMA_STATE_READY;

			if(hdma->XferCpltCallback != NULL)
			{
				/* Transfer complete callback */
				hdma->XferCpltCallback(hdma);
			}
		}
	}
  
	/* manage error case */
	if(hdma->ErrorCode != TMC_DMA_ERROR_NONE)
	{
		if((hdma->ErrorCode & TMC_DMA_ERROR_TE) != RESET)
		{
			hdma->State = TMC_DMA_STATE_ABORT;

			/* Disable the stream */
			DMA_Cmd(hdma->Instance,DISABLE);

			do
			{
				if (++count > timeout)
				{
					break;
				}
			}
			while(DMA_GetCmdStatus(hdma->Instance) != RESET);

			/* Change the DMA state */
			hdma->State = TMC_DMA_STATE_READY;
		}

		if(hdma->XferErrorCallback != NULL)
		{
			/* Transfer error callback */
			hdma->XferErrorCallback(hdma);
		}
	}
}

/**
  * @brief  Register callbacks
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *				 the configuration information for the specified DMA Stream.
  * @param[in]  CallbackID: User Callback identifer
  *					   a DMA_HandleTypeDef structure as parameter.
  * @param[in]  pCallback: pointer to private callbacsk function which has pointer to 
  *					  a DMA_HandleTypeDef structure as parameter.
  * @retval TMC status
  */					  
TMC_StatusTypeDef TMC_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, TMC_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)(DMA_HandleTypeDef *_hdma))
{
	TMC_StatusTypeDef status = TMC_OK;



	if(TMC_DMA_STATE_READY == hdma->State)
	{
		switch (CallbackID)
		{
			case  HAL_DMA_XFER_CPLT_CB_ID:
				hdma->XferCpltCallback = pCallback;
				break;

			case  HAL_DMA_XFER_ERROR_CB_ID:
				hdma->XferErrorCallback = pCallback;
				break;

			case  HAL_DMA_XFER_ABORT_CB_ID:
				hdma->XferAbortCallback = pCallback;
				break;

			default:
				break;
		}
	}
	else
	{
		/* Return error status */
		status =  TMC_ERROR;
	}



	return status;
}

/**
  * @brief  UnRegister callbacks
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *				   the configuration information for the specified DMA Stream.
  * @param[in]  CallbackID: User Callback identifer
  *						 a HAL_DMA_CallbackIDTypeDef ENUM as parameter.
  * @retval TMC status
  */			  
TMC_StatusTypeDef TMC_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, TMC_DMA_CallbackIDTypeDef CallbackID)
{
	TMC_StatusTypeDef status = TMC_OK;



	if(TMC_DMA_STATE_READY == hdma->State)
	{
		switch (CallbackID)
		{
			case  HAL_DMA_XFER_CPLT_CB_ID:
				hdma->XferCpltCallback = NULL;
				break;

			case  HAL_DMA_XFER_ERROR_CB_ID:
				hdma->XferErrorCallback = NULL;
				break;

			case  HAL_DMA_XFER_ABORT_CB_ID:
				hdma->XferAbortCallback = NULL;
				break; 

			case   HAL_DMA_XFER_ALL_CB_ID:
				hdma->XferCpltCallback = NULL;
				hdma->XferErrorCallback = NULL;
				hdma->XferAbortCallback = NULL;
				break; 

			default:
				status = TMC_ERROR;
				break;
		}
	}
	else
	{
		status = TMC_ERROR;
	}



	return status;
}


/**
  * @brief  Returns the DMA state.
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *				  the configuration information for the specified DMA Stream.
  * @retval TMC state
  */
TMC_DMA_StateTypeDef TMC_DMA_GetState(DMA_HandleTypeDef *hdma)
{
	return hdma->State;
}

/**
  * @brief  Return the DMA error code
  * @param[in]  hdma:  pointer to a DMA_HandleTypeDef structure that contains
  *			  the configuration information for the specified DMA Stream.
  * @retval DMA Error Code
  */
uint32_t TMC_DMA_GetError(DMA_HandleTypeDef *hdma)
{
	return hdma->ErrorCode;
}


/** @addtogroup TMC_DMA_Private_Functions TMC DMA Private Functions
  * @{
  */

/**
  * @brief  Returns the DMA channel number
  * @param[in]  hdma: pointer to a DMA_HandleTypeDef structure that contains
  *					 the configuration information for the specified DMA Stream. 
  * @retval DMA channel number
  */
static uint32_t DMA_CalcBitShift(DMA_HandleTypeDef *hdma)
{
  uint32_t channel_number = (((uint32_t)hdma->Instance & 0xFFU)) / 32U;
  
  return channel_number;
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

/**
  * @}
  */ 
/************************ (C) COPYRIGHT TMC *****END OF FILE****/


