/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_uart.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "TMC_UART.h"

/**
  * @}
  */
/** @defgroup TMC_UART_Private_Functions TMC UART Private Functions
  * @{
  */
static TMC_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);  
static TMC_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart);
static TMC_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart);
static void UART_EndTxTransfer(UART_HandleTypeDef *huart);
static void UART_EndRxTransfer(UART_HandleTypeDef *huart);
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void UART_DMAError(DMA_HandleTypeDef *hdma);

/**
  * @brief Initializes the UART struct according to the specified parameters in
  *		 the UART_HandleTypeDef and create the associated handle.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval void
  */
void TMC_UART_StructInit(UART_HandleTypeDef *huart)
{
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState = TMC_UART_STATE_RESET;
	huart->hdmarx = NULL;
	huart->hdmatx = NULL;
	huart->pRxBuffPtr = NULL;
	huart->pTxBuffPtr = NULL;
	huart->RxState = TMC_UART_STATE_RESET;
	huart->RxXferCount = NULL;
	huart->RxXferSize = NULL;
	huart->TxXferCount = NULL;
	huart->TxXferSize = NULL;
}

/**
  * @brief Initializes the UART mode according to the specified parameters in
  *		 the UART_InitTypeDef and create the associated handle.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Init(UART_HandleTypeDef *huart)
{
	/* Check the UART handle allocation */
	if(huart == NULL)
	{
		return TMC_ERROR;
	}

	if((huart->TxState == TMC_UART_STATE_RESET)||(huart->RxState == TMC_UART_STATE_RESET))
	{  
		/* Init the low level hardware */
		TMC_UART_MspInit(huart);
	}
	/* Set the UART Communication parameters */
	UART_Init(huart->Instance, huart->Init);
	
	/* Initialize the UART state */
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState= TMC_UART_STATE_READY;
	huart->RxState= TMC_UART_STATE_READY;
	
	return TMC_OK;
}

/**
  * @brief DeInitializes the UART peripheral. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_DeInit(UART_HandleTypeDef *huart)
{
	/* Check the UART handle allocation */
	if(huart == NULL)
	{
		return TMC_ERROR;
	}
	
	UART_DeInit(huart->Instance);
	TMC_UART_MspDeInit(huart);
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState = TMC_UART_STATE_RESET;
	huart->RxState = TMC_UART_STATE_RESET;
	return TMC_OK;
}

/**
  * @brief Sends an amount of data in query mode. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in]: pData: Pointer to data buffer
  * @param[in]: Size: Amount of data to be sent
  * @param[in]: Timeout: Timeout duration  
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t tickstart = 0;
	
	if((pData == NULL ) || (Size == 0)||(huart == 0)) 
	{
		return  TMC_ERROR;
	}
	if(huart->TxState != TMC_UART_STATE_READY) 
	{
		return  TMC_ERROR;
	}
	/* Process Locked */
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState = TMC_UART_STATE_BUSY;
	
	/* Init tickstart for timeout managment */
	tickstart = TMC_GetTick();

	huart->TxXferSize = Size;
	huart->TxXferCount = Size;
	while(huart->TxXferCount > 0U)
	{
		
		UART_SendData(huart->Instance, *pData);
		if(UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_TXE, RESET, tickstart, Timeout) != TMC_OK)
		{ 
			return TMC_TIMEOUT;
		}
		huart->TxXferCount--;
		pData++;
	}
	
	/* At end of Tx process, restore huart->gState to Ready */
	huart->TxState = TMC_UART_STATE_READY;


	
	return TMC_OK;
	
}

/**
  * @brief Receives an amount of data in query mode. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  * 								the configuration information for the specified UART module.
  * @param[in] pData: Pointer to data buffer
  * @param[in] Size: Amount of data to be received
  * @param[in] Timeout: Timeout duration
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{ 
	uint32_t tickstart = 0U;
	if((pData == NULL ) || (Size == 0)||(huart == 0)) 
	{
		return  TMC_ERROR;
	}
	if(huart->RxState != TMC_UART_STATE_READY) 
	{
		return  TMC_ERROR;
	}

	/* Process Locked */
	
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->RxState = TMC_UART_STATE_BUSY;
	
	/* Init tickstart for timeout managment */
	tickstart = TMC_GetTick();
	huart->pRxBuffPtr = pData;
	huart->RxXferSize = Size; 
	huart->RxXferCount = Size;

	/* Check the remain data to be received */
	while(huart->RxXferCount > 0U)
	{
		if(UART_WaitOnFlagUntilTimeout(huart, UART_FLAG_RXF, RESET, tickstart, Timeout) != TMC_OK)
		{ 
			return TMC_TIMEOUT;
		}
		if(UART_GetFlagStatus(huart->Instance,UART_FLAG_PE)==SET)
		{
			* huart->pRxBuffPtr  = UART_ReceiveData(huart->Instance);
			huart->RxState = TMC_UART_STATE_ERROR;
			huart->ErrorCode = TMC_UART_ERROR_PE;
			return TMC_ERROR;
		}
		if(UART_GetFlagStatus(huart->Instance,UART_FLAG_OF)==SET)
		{
			* huart->pRxBuffPtr  = UART_ReceiveData(huart->Instance);
			huart->RxState = TMC_UART_STATE_ERROR;
			huart->ErrorCode = TMC_UART_ERROR_OFE;
			return TMC_ERROR;
		}
		if(UART_GetFlagStatus(huart->Instance,UART_FLAG_FE)==SET)
		{
			* huart->pRxBuffPtr  = UART_ReceiveData(huart->Instance);
			huart->RxState = TMC_UART_STATE_ERROR;
			huart->ErrorCode = TMC_UART_ERROR_FE;
			return TMC_ERROR;
		}
		* huart->pRxBuffPtr  = UART_ReceiveData(huart->Instance);
		huart->RxXferCount--;
		huart->pRxBuffPtr ++;
	}
	/* At end of Rx process, restore huart->RxState to Ready */
	huart->RxState =TMC_UART_STATE_READY;
		
	return TMC_OK;
}

/**
  * @brief Sends an amount of data in interrupt mode.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in] pData: Pointer to data buffer
  * @param[in] Size: Amount of data to be sent
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
{
	if((pData == NULL ) || (Size == 0)||(huart == 0)) 
	{
		return  TMC_ERROR;
	}
	if(huart->TxState != TMC_UART_STATE_READY) 
	{
		return  TMC_ERROR;
	}	
	/* Process Locked */
	huart->pTxBuffPtr = pData;
	huart->TxXferSize = Size;
	huart->TxXferCount = Size;
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState = TMC_UART_STATE_BUSY;

	/* Enable the UART Transmit data register empty Interrupt */
	UART_ITConfig(huart->Instance,UART_FLAG_TXE, ENABLE);
	return TMC_OK;
	
}

/**
  * @brief Receives an amount of data in interrupt mode. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in] pData: Pointer to data buffer
  * @param[in] Size: Amount of data to be received
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
{
	if((pData == NULL ) || (Size == 0)||(huart == 0)) 
	{
		return  TMC_ERROR;
	}
	if(huart->RxState != TMC_UART_STATE_READY) 
	{
		return  TMC_ERROR;
	}
		
	/* Process Locked */
	
	huart->pRxBuffPtr = pData;
	huart->RxXferSize = Size;
	huart->RxXferCount = Size;
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->RxState = TMC_UART_STATE_BUSY;

	/* Enable the UART Receive buffer full interrupt and Error Interrupt: (Frame error, Parity error, Overflow error) */
	UART_ITConfig(huart->Instance,UART_FLAG_RXF|UART_FLAG_PE|UART_FLAG_OF|UART_FLAG_FE, ENABLE);
	return TMC_OK;
}

/**
  * @brief Sends an amount of data in DMA mode. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in] pData: Pointer to data buffer
  * @param[in] Size: Amount of data to be sent
  * @param[in] LLI: the address of LLI,if LLI is 0,the Uart Transmit not make LLI
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Transmit_DMA_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI)
{
	if((pData == NULL ) || (Size == 0)||(huart == 0)) 
	{
		return  TMC_ERROR;
	}
	if(huart->TxState != TMC_UART_STATE_READY) 
	{
		return  TMC_ERROR;
	}
	
	huart->pTxBuffPtr = pData;
	huart->TxXferSize = Size;
	huart->TxXferCount = Size;
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->TxState = TMC_UART_STATE_BUSY;
	
	huart->hdmatx->Parent = huart;
	/* Set the UART DMA transfer complete callback */
	huart->hdmatx->XferCpltCallback = UART_DMATransmitCplt;
	/* Set the DMA error callback */
	huart->hdmatx->XferErrorCallback = UART_DMAError;
	/* Set the DMA abort callback */
	huart->hdmatx->XferAbortCallback = NULL;
	huart->hdmatx->Instance->LLI = LLI;
	/* Enable the UART transmit DMA Stream */
	TMC_DMA_Start_IT(huart->hdmatx, (uint32_t)pData, (uint32_t)&huart->Instance->TxDATA, Size);
	/* Enable the DMA transfer */
	UART_DMACmd(huart->Instance, UART_DMA_TX, ENABLE);	
	return TMC_OK;
	
}

/**
  * @brief Receives an amount of data in DMA mode. 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in] pData: Pointer to data buffer
  * @param[in] Size: Amount of data to be received
  * @param[in] LLI: the address of LLI,if LLI is 0,the Uart Transmit not make LLI
  * @note   When the UART parity is enabled (PCE = 1) the data received contain the parity bit.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_Receive_DMA_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI)
{
	/* Check that a Rx process is not already ongoing */
	if((pData == NULL ) || (Size == 0)||(huart == 0)) 
	{
		return  TMC_ERROR;
	}
	if(huart->RxState != TMC_UART_STATE_READY) 
	{
		return  TMC_ERROR;
	}

	
	huart->pRxBuffPtr = pData;
	huart->RxXferSize = Size;
	huart->ErrorCode = TMC_UART_ERROR_NONE;
	huart->RxState = TMC_UART_STATE_BUSY;
	/* Set the UART DMA transfer complete callback */
	huart->hdmarx->Parent = huart;
	huart->hdmarx->XferCpltCallback = UART_DMAReceiveCplt;
	/* Set the DMA error callback */
	huart->hdmarx->XferErrorCallback = UART_DMAError;
	/* Set the DMA abort callback */
	huart->hdmarx->XferAbortCallback = NULL;
	huart->hdmatx->Instance->LLI = LLI;
	/* Enable the DMA Stream */
	TMC_DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->RxDATA, (uint32_t)pData, Size);
	/* Clear the overflow error flag */
	UART_ClearFlagStatus(huart->Instance, UART_FLAG_OF);	
	/* Enable the UART Parity Error, Frame Error, Overflow Error interrupt */
	UART_ITConfig(huart->Instance, UART_FLAG_PE|UART_FLAG_OF|UART_FLAG_FE, ENABLE);
	/* Enable the DMA transfer for the receiver */
	UART_DMACmd(huart->Instance, UART_DMA_RX, ENABLE);
	return TMC_OK;
}


/**
  * @brief Stops the DMA Transfer.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART_DMAStop(UART_HandleTypeDef *huart)
{
	/* The Lock is not implemented on this API to allow the user application
		to call the TMC UART API under callbacks TMC_UART_TxCpltCallback() / TMC_UART_RxCpltCallback():
		when calling TMC_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
		and the correspond call back is executed TMC_UART_TxCpltCallback() / TMC_UART_RxCpltCallback()
	*/
	
	/* Stop UART DMA Tx request if ongoing */
	if((huart->TxState == TMC_UART_STATE_BUSY) && UART_GetDMAStatus(huart->Instance, UART_DMA_TX))
	{
		UART_DMACmd(huart->Instance, UART_DMA_TX, DISABLE);

		/* Abort the UART DMA Tx channel */
		if(huart->hdmatx != NULL)
		{
			TMC_DMA_Abort(huart->hdmatx);
		}
		UART_EndTxTransfer(huart);
	}

	/* Stop UART DMA Rx request if ongoing */
	if((huart->RxState == TMC_UART_STATE_BUSY) && UART_GetDMAStatus(huart->Instance, UART_DMA_RX))
	{
		UART_DMACmd(huart->Instance, UART_DMA_RX, DISABLE);

		/* Abort the UART DMA Rx channel */
		if(huart->hdmarx != NULL)
		{
			TMC_DMA_Abort(huart->hdmarx);
		}
		UART_EndRxTransfer(huart);
	}

	return TMC_OK;
}


/**
  * @brief This function handles UART interrupt request.
  * @param[in]  huart: pointer to a UART_HandleTypeDef structure that contains
  *		 				the configuration information for the specified UART module.
  * @retval None
  */
void TMC_UART_IRQHandler(UART_HandleTypeDef *huart)
{
	
	if(UART_GetITStatus(huart->Instance,UART_IT_PE|UART_IT_OF|UART_IT_FE)==SET)
	{
		huart->RxState = TMC_UART_STATE_ERROR;
		if(UART_GetITStatus(huart->Instance,UART_IT_PE)==SET)
		{
			huart->ErrorCode = TMC_UART_ERROR_PE;
		}
		if(UART_GetITStatus(huart->Instance,UART_IT_OF)==SET)
		{
			huart->ErrorCode = TMC_UART_ERROR_OFE;
		}
		if(UART_GetITStatus(huart->Instance,UART_IT_FE)==SET)
		{
			huart->ErrorCode = TMC_UART_ERROR_FE;
		}
		UART_ITConfig(huart->Instance,UART_IT_TXE|UART_IT_RXF|UART_IT_PE|UART_IT_OF|UART_IT_FE,DISABLE);
		UART_ClearITStatus(huart->Instance,UART_IT_PE|UART_IT_OF|UART_IT_FE);
		UART_DMACmd(huart->Instance, UART_DMA_RX, DISABLE);
		TMC_UART_ErrorCallback(huart);
		return;
		
	}
	if(UART_GetITStatus(huart->Instance,UART_FLAG_TXE) == SET)
	{
		UART_Transmit_IT(huart);
	}
	if(UART_GetITStatus(huart->Instance,UART_FLAG_RXF) == SET)
	{
		UART_Receive_IT(huart);
	}
}

/**
  * @brief UART MSP Init.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART_MspInit(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
					the HAL_UART_MspInit could be implemented in the user file
	*/ 
}

/**
  * @brief UART MSP DeInit.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART_MspDeInit(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
					the HAL_UART_MspDeInit could be implemented in the user file
	*/ 
}

/**
  * @brief Tx Transfer completed callbacks.
  * @param[in] huart£ºpointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
					the TMC_UART_TxCpltCallback could be implemented in the user file
	*/ 
}

/**
  * @brief Rx Transfer completed callbacks.
  * @param[in] huart£ºpointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
__weak void TMC_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
					the TMC_UART_TxCpltCallback could be implemented in the user file
	*/
}
/**
  * @brief UART error callbacks.
  * @param[in] huart£ºpointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart); 
	/* NOTE: This function Should not be modified, when the callback is needed,
					the TMC_UART_ErrorCallback could be implemented in the user file
	*/ 
}


/**
  * @}
  */ 

/** @addtogroup TMC_UART_Private_Functions TMC UART Private Functions
  * @{
  */

/**
  * @brief Sends an amount of data in non query mode.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in] pData: Pointer to data buffer
  * @param[in] Size: Amount of data to be sent
  * @retval TMCstatus
  */
static TMC_StatusTypeDef UART_Transmit_IT(UART_HandleTypeDef *huart)
{
	huart->Instance->TxDATA = *(uint8_t*) huart->pTxBuffPtr;
	huart->pTxBuffPtr ++;
	huart->TxXferCount--;
	if(huart->TxXferCount == 0U)
	{
		/* Disable the UART Transmit Buffer Empty Interrupt */
		UART_ITConfig(huart->Instance,UART_FLAG_TXE, DISABLE);
		/* Tx process is ended, restore huart->gState to Ready */
		huart->TxState = TMC_UART_STATE_READY;
		TMC_UART_TxCpltCallback(huart);
	}
	return TMC_OK;
	
}

/**
  * @brief Receives an amount of data in non query mode 
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
static TMC_StatusTypeDef UART_Receive_IT(UART_HandleTypeDef *huart)
{
	*huart->pRxBuffPtr = (huart->Instance->RxDATA);
	huart->pRxBuffPtr++;
	huart->RxXferCount--;
	if(huart->RxXferCount == 0U)
	{
		UART_ITConfig(huart->Instance,UART_FLAG_RXF|UART_FLAG_PE|UART_FLAG_OF|UART_FLAG_FE, DISABLE);
		huart->RxState = TMC_UART_STATE_READY;

		TMC_UART_RxCpltCallback(huart);

	}
	return TMC_OK;	
}

/**
  * @brief This function handles UART Communication Timeout.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @param[in] Flag: specifies the UART flag to check.
  * @param[in] Status: The new Flag status (SET or RESET).
  * @param[in] Tickstart£» Tick start value
  * @param[in] Timeout£º Timeout duration
  * @retval TMC status
  */
static TMC_StatusTypeDef UART_WaitOnFlagUntilTimeout(UART_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)
{
	/* Wait until flag is set */
	while((UART_GetFlagStatus(huart->Instance, Flag) ? SET : RESET) == Status) 
	{
		/* Check for the Timeout */
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U)||((TMC_GetTick() - Tickstart ) > Timeout))
			{
				UART_ITConfig(huart->Instance, UART_FLAG_PE|UART_FLAG_TXE|UART_FLAG_RXF|UART_FLAG_OF|UART_FLAG_FE, DISABLE);
				
				huart->TxState  = TMC_UART_STATE_READY;
				huart->RxState = TMC_UART_STATE_READY;

				return TMC_TIMEOUT;
			}
		}
	}
	return TMC_OK;
}

/**
  * @brief End ongoing Tx transfer on UART peripheral (following error detection or Transmit completion).
  * @param[in] huart: UART handle.
  * @retval None
  */
static void UART_EndTxTransfer(UART_HandleTypeDef *huart)
{
	/* Disable TXE interrupts */
	UART_ITConfig(huart->Instance,UART_FLAG_TXE, DISABLE);

	/* At end of Tx process, restore huart->gState to Ready */
	huart->TxState = TMC_UART_STATE_READY;
}

/**
  * @brief End ongoing Rx transfer on UART peripheral (following error detection or Reception completion).
  * @param[in] huart: UART handle.
  * @retval None
  */
static void UART_EndRxTransfer(UART_HandleTypeDef *huart)
{
	/* Enable the UART Receive buffer full interrupt and Error Interrupt: (Frame error, Parity error, Overflow error) */
	UART_ITConfig(huart->Instance,UART_FLAG_RXF|UART_FLAG_PE|UART_FLAG_OF|UART_FLAG_FE, DISABLE);

	/* At end of Rx process, restore huart->RxState to Ready */
	huart->RxState = TMC_UART_STATE_READY;
}



/**
  * @brief DMA UART transmit process complete callback. 
  * @param[in] hdma: DMA handle
  * @retval None
  */
static void UART_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
	UART_HandleTypeDef* huart = ( UART_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

	huart->TxXferCount = 0U;

	/* Disable the DMA transfer for transmit request */
	UART_DMACmd(huart->Instance, UART_DMA_TX, DISABLE);
	
	/* Disable the transmit empty interrupt */
	//UART_ITConfig(huart->Instance, UART_FLAG_TXE, DISABLE);
	
	/* Restore huart->gState to Ready */
	huart->TxState = TMC_UART_STATE_READY;
	TMC_UART_TxCpltCallback(huart);

}

/**
  * @brief DMA UART receive process complete callback. 
  * @param[in] hdma: DMA handle
  * @retval None
  */
static void UART_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
	UART_HandleTypeDef* huart = ( UART_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
 
	huart->RxXferCount = 0U;
  
	/* Disable the DMA receive for transmit request */
	UART_DMACmd(huart->Instance, UART_DMA_RX, DISABLE);
	
	/* Disable the UART Parity Error Interrupt and RXNE interrupt*/
	UART_ITConfig(huart->Instance,UART_FLAG_RXF|UART_FLAG_PE|UART_FLAG_OF|UART_FLAG_FE, DISABLE);
	/* Tx process is ended, restore huart->gState to Ready */
	huart->RxState = TMC_UART_STATE_READY;
	TMC_UART_RxCpltCallback(huart);

}

/**
  * @brief DMA UART communication error callback.
  * @param[in] hdma: DMA handle
  * @retval None
  */
static void UART_DMAError(DMA_HandleTypeDef *hdma)
{
	UART_HandleTypeDef* huart = ( UART_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

	/* Stop UART DMA Tx request if ongoing */
	if((huart->TxState == TMC_UART_STATE_BUSY) && (UART_GetDMAStatus(huart->Instance, UART_DMA_TX) != RESET))
	{
		huart->TxXferCount = 0U;
		UART_EndTxTransfer(huart);
	}

	/* Stop UART DMA Rx request if ongoing */
	if((huart->RxState == TMC_UART_STATE_BUSY) && (UART_GetDMAStatus(huart->Instance, UART_DMA_RX) != RESET))
	{
		huart->RxXferCount = 0U;
		UART_EndRxTransfer(huart);
	}

	huart->ErrorCode |= TMC_UART_ERROR_DMA;
	TMC_UART_ErrorCallback(huart);
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


