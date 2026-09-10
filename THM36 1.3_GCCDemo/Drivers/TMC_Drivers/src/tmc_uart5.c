/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_uart5.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/
#include "tmc_uart5.h"
 
/**
  * @}
  */
/** @defgroup TMC_UART_Private_Functions TMC UART Private Functions
  * @{
  */
static TMC_StatusTypeDef UART5_WaitOnFlagUntilTimeout(UART5_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout);  
static TMC_StatusTypeDef UART5_Transmit_IT(UART5_HandleTypeDef *huart);
static TMC_StatusTypeDef UART5_Receive_IT(UART5_HandleTypeDef *huart);
static void UART5_EndTxTransfer(UART5_HandleTypeDef *huart);
static void UART5_EndRxTransfer(UART5_HandleTypeDef *huart);
static void UART5_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void UART5_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void UART5_DMAError(DMA_HandleTypeDef *hdma);
/**
  * @brief Initializes the UART5 struct according to the specified parameters in
  *		 the UART5_HandleTypeDef and create the associated handle.
  * @param[in] huart: pointer to a UART5_HandleTypeDef structure that contains
  *				the configuration information for the specified UART5 module.
  * @retval void
  */
void TMC_UART5_StructInit(UART5_HandleTypeDef *huart)
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
	huart->Mask = 0x00FF ;
}

/**
  * @brief Initializes the UART mode according to the specified parameters in
  *		 the UART_InitTypeDef and create the associated handle.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART5_Init(UART5_HandleTypeDef *huart)
{
	/* Check the uart.handle allocation */
	if(huart == NULL)
	{
		return TMC_ERROR;
	}

	if((huart->TxState == TMC_UART_STATE_RESET)||(huart->RxState == TMC_UART_STATE_RESET))
	{  
		/* Init the low level hardware */
		TMC_UART5_MspInit(huart);
	}
	/* Set the UART Communication parameters */
	UART5_Init(huart->Instance, huart->Init);
	
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
TMC_StatusTypeDef TMC_UART5_DeInit(UART5_HandleTypeDef *huart)
{
	/* Check the uart.handle allocation */
	if(huart == NULL)
	{
		return TMC_ERROR;
	}
	
	UART5_DeInit(huart->Instance);
	TMC_UART5_MspDeInit(huart);
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
TMC_StatusTypeDef TMC_UART5_Transmit(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint32_t tickstart = 0;
	uint16_t* tmp;
	
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
		if ((huart->Init->WordLength == UART5_WordLength_9b) && (huart->Init->Parity == UART5_Parity_None))
		{
			tmp = (uint16_t*) pData;
			huart->Instance->TDR = (*tmp & (uint16_t)0x01FFU);
			pData += 2U;
		}
		else
		{
			huart->Instance->TDR = (*pData++ & (uint8_t)0xFFU);
		}
		if(UART5_WaitOnFlagUntilTimeout(huart, UART5_FLAG_TXE, RESET, tickstart, Timeout) != TMC_OK)
		{ 
			return TMC_TIMEOUT;
		}
		huart->TxXferCount--;
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
TMC_StatusTypeDef TMC_UART5_Receive(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout)
{
	uint16_t* tmp;	
	uint32_t tickstart = 0U;
	uint16_t uhMask;
	
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
	/* Computation of UART mask to apply to RDR register */
	TMC_UART5_Mask_Computation(huart);
	uhMask = huart->Mask;
	
	/* Check the remain data to be received */
	while(huart->RxXferCount > 0U)
	{
		if(UART5_WaitOnFlagUntilTimeout(huart, UART5_FLAG_RXNE, RESET, tickstart, Timeout) != TMC_OK)
		{ 
			return TMC_TIMEOUT;
		}
		if(UART5_GetFlagStatus(huart->Instance,UART5_FLAG_PE)==SET)
		{
			* huart->pRxBuffPtr  = UART5_ReceiveData(huart->Instance);
			huart->RxState = TMC_UART_STATE_ERROR;
			huart->ErrorCode = TMC_UART_ERROR_PE;
			return TMC_ERROR;
		}
		if(UART5_GetFlagStatus(huart->Instance,UART5_FLAG_ORE)==SET)
		{
			* huart->pRxBuffPtr  = UART5_ReceiveData(huart->Instance);
			huart->RxState = TMC_UART_STATE_ERROR;
			huart->ErrorCode = TMC_UART_ERROR_OFE;
			return TMC_ERROR;
		}
		if(UART5_GetFlagStatus(huart->Instance,UART5_FLAG_FE)==SET)
		{
			* huart->pRxBuffPtr  = UART5_ReceiveData(huart->Instance);
			huart->RxState = TMC_UART_STATE_ERROR;
			huart->ErrorCode = TMC_UART_ERROR_FE;
			return TMC_ERROR;
		}
//		* huart->pRxBuffPtr  = UART5_ReceiveData(huart->Instance);
		if ((huart->Init->WordLength == UART5_WordLength_9b) && (huart->Init->Parity == UART5_Parity_None))
		{
			tmp = (uint16_t*) pData ;
			*tmp = (uint16_t)(huart->Instance->RDR & uhMask);
			pData +=2U;
		}
		else
		{
			*pData++ = (uint8_t)(huart->Instance->RDR & (uint8_t)uhMask);
		}
		huart->RxXferCount--;
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
TMC_StatusTypeDef TMC_UART5_Transmit_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
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

	/* Enable the TX FIFO threshold interrupt (if FIFO mode is enabled) or
	   Transmit Data Register Empty interrupt (if FIFO mode is Disabled).
	*/
	if (READ_BIT(huart->Instance->CR1, UART5_CR1_FIFOEN) != RESET)
	{
		SET_BIT(huart->Instance->CR3, UART5_CR3_TXFTIE);
	}
	else
	{
		SET_BIT(huart->Instance->CR1, UART5_CR1_TXEIE);
	}
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
TMC_StatusTypeDef TMC_UART5_Receive_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
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

	/* Enable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	SET_BIT(huart->Instance->CR3, UART5_CR3_EIE);

	/* Enable the UART Parity Error interupt and RX FIFO Threshold interrupt
	   (if FIFO mode is enabled) or Data Register Not Empty interrupt
	   (if FIFO mode is disabled).
	*/
	if (READ_BIT(huart->Instance->CR1, UART5_CR1_FIFOEN) != RESET)
	{
		SET_BIT(huart->Instance->CR1, UART5_CR1_PEIE);
		SET_BIT(huart->Instance->CR3, UART5_CR3_RXFTIE);
         //TMC:gsx-20240430 IDLE interrupt can handle,don't need wait
//		while(!(huart->Instance->ISR & UART5_FLAG_IDLE));
//		SET_BIT(huart->Instance->ICR, UART5_FLAG_IDLE);
		//enable IDLE INT,receive data of arbitrary byte length
		SET_BIT(huart->Instance->CR1, UART5_CR1_IDLEIE);
	}
	else
	{
		SET_BIT(huart->Instance->CR1, UART5_CR1_PEIE | UART5_CR1_RXNEIE);
	}   
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
TMC_StatusTypeDef TMC_UART5_Transmit_DMA_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI)
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
	huart->hdmatx->XferCpltCallback = UART5_DMATransmitCplt;
	/* Set the DMA error callback */
	huart->hdmatx->XferErrorCallback = UART5_DMAError;
	/* Set the DMA abort callback */
	huart->hdmatx->XferAbortCallback = NULL;
	huart->hdmatx->Instance->LLI = LLI;
	/* Enable the UART transmit DMA Stream */
	TMC_DMA_Start_IT(huart->hdmatx, (uint32_t)pData, (uint32_t)&huart->Instance->TDR, Size);
	/* Enable the DMA transfer */
	UART5_DMACmd(huart->Instance, UART5_DMAReq_Tx, ENABLE);	
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
TMC_StatusTypeDef TMC_UART5_Receive_DMA_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI)
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
	huart->hdmarx->XferCpltCallback = UART5_DMAReceiveCplt;
	/* Set the DMA error callback */
	huart->hdmarx->XferErrorCallback = UART5_DMAError;
	/* Set the DMA abort callback */
	huart->hdmarx->XferAbortCallback = NULL;
	huart->hdmarx->Instance->LLI = LLI;
	/* Enable the DMA Stream */
	TMC_DMA_Start_IT(huart->hdmarx, (uint32_t)&huart->Instance->RDR, (uint32_t)pData, Size);
	/* Clear the overflow error flag */
	UART5_ClearFlag(huart->Instance, UART5_FLAG_ORE);	
	/* Enable the UART Parity Error, Frame Error, Overflow Error interrupt */
	SET_BIT(huart->Instance->CR1, UART5_CR1_PEIE);
	SET_BIT(huart->Instance->CR3, UART5_CR3_EIE);
    /*TMC(20240409)-gsx:DMA+IDLE wait IDLE status -enable IDLE INT*/
    //wait IDLE status
    while(!(huart->Instance->ISR & UART5_FLAG_IDLE));
    SET_BIT(huart->Instance->ICR, UART5_FLAG_IDLE);
	//enable IDLE INT
	SET_BIT(huart->Instance->CR1, UART5_CR1_IDLEIE);	
	/* Enable the DMA transfer for the receiver */
	UART5_DMACmd(huart->Instance, UART5_DMAReq_Rx, ENABLE);
	return TMC_OK;
}


/**
  * @brief Stops the DMA Transfer.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_UART5_DMAStop(UART5_HandleTypeDef *huart)
{
	/* The Lock is not implemented on this API to allow the user application
		to call the TMC UART API under callbacks TMC_UART_TxCpltCallback() / TMC_UART_RxCpltCallback():
		when calling TMC_DMA_Abort() API the DMA TX/RX Transfer complete interrupt is generated
		and the correspond call back is executed TMC_UART_TxCpltCallback() / TMC_UART_RxCpltCallback()
	*/
	
	/* Stop UART DMA Tx request if ongoing */
	if((huart->TxState == TMC_UART_STATE_BUSY) && UART5_GetDMAStatus(huart->Instance, UART5_DMAReq_Tx))
	{
		UART5_DMACmd(huart->Instance, UART5_DMAReq_Tx, DISABLE);

		/* Abort the UART DMA Tx channel */
		if(huart->hdmatx != NULL)
		{
			TMC_DMA_Abort(huart->hdmatx);
		}
		UART5_EndTxTransfer(huart);
	}

	/* Stop UART DMA Rx request if ongoing */
	if((huart->RxState == TMC_UART_STATE_BUSY) && UART5_GetDMAStatus(huart->Instance, UART5_DMAReq_Rx))
	{
		UART5_DMACmd(huart->Instance, UART5_DMAReq_Rx, DISABLE);

		/* Abort the UART DMA Rx channel */
		if(huart->hdmarx != NULL)
		{
			TMC_DMA_Abort(huart->hdmarx);
		}
		UART5_EndRxTransfer(huart);
	}

	return TMC_OK;
}


/**
  * @brief This function handles UART interrupt request.
  * @param[in]  huart: pointer to a UART_HandleTypeDef structure that contains
  *		 				the configuration information for the specified UART module.
  * @retval None
  */
void TMC_UART5_IRQHandler(UART5_HandleTypeDef *huart)
{
    uint32_t isrflags   = READ_REG(huart->Instance->ISR);
    uint32_t cr1its	 = READ_REG(huart->Instance->CR1);
    uint32_t cr3its	 = READ_REG(huart->Instance->CR3);
    uint32_t errorflags;
    uint8_t rxfifothreshold = 0;
    uint8_t i = 0;
    
    /* If no error occurs */
    errorflags = (isrflags & (uint32_t)(UART5_FLAG_PE | UART5_FLAG_FE | UART5_FLAG_ORE | UART5_FLAG_NE));
   /*TMC£ºgsx-20240430-solution Receive error problem*/
    if (errorflags == RESET)
    {
		if((cr3its & UART5_DMAReq_Rx) == RESET)
		{
            //enable rxfifothreshold and UART5_FLAG_RXFT = 1
			if(((cr3its & UART5_CR3_RXFTIE) != RESET) && (isrflags & UART5_FLAG_RXFT))
			{
				rxfifothreshold = TMC_Check_Uart5_RxFIFO_Threshold(huart->Init->RXFIFOThreshold);
                if(rxfifothreshold > 0)
                {
                    for(i = 0; i < rxfifothreshold;i++)
					{
						UART5_Receive_IT(huart);				
					}				
                }
			}
            //enable IDLE and UART5_FLAG_IDLE = 1
			if((cr1its & UART5_CR1_IDLEIE) != RESET && (isrflags & UART5_FLAG_IDLE))
			{
			    //read date untill UART5_FLAG_RXNE = 0
				while((READ_REG(huart->Instance->ISR) & UART5_FLAG_RXNE) != RESET)
				{
					UART5_Receive_IT(huart);
					
				}
                //clear UART5_FLAG_IDLE
				SET_BIT(huart->Instance->ICR, UART5_FLAG_IDLE);
				
			}

		}
    }
    
    /* If some errors occur */
    if(   (errorflags != RESET)
    && (   ((cr3its & (UART5_CR3_RXFTIE | UART5_CR3_EIE)) != RESET)
    || ((cr1its & (UART5_CR1_RXNEIE | UART5_CR1_PEIE)) != RESET)))
    {
        /* UART parity error interrupt occurred -------------------------------------*/
        if(((isrflags & UART5_FLAG_PE) != RESET) && ((cr1its & UART5_CR1_PEIE) != RESET))
        {
            __UART5_CLEAR_IT(huart, UART5_CLEAR_PECF);
            huart->ErrorCode |= UART5_ERROR_PE;
        }
    
        /* UART frame error interrupt occurred --------------------------------------*/
        if(((isrflags & UART5_FLAG_FE) != RESET) && ((cr3its & UART5_CR3_EIE) != RESET))
        {
            __UART5_CLEAR_IT(huart, UART5_CLEAR_FECF);
            huart->ErrorCode |= UART5_ERROR_FE;
        }
        
        /* UART noise error interrupt occurred --------------------------------------*/
        if(((isrflags & UART5_FLAG_NE) != RESET) && ((cr3its & UART5_CR3_EIE) != RESET))
        {
            __UART5_CLEAR_IT(huart, UART5_CLEAR_NECF | UART5_CLEAR_IDLECF);
            huart->ErrorCode |= UART5_ERROR_NE;
        }
        
        /* UART Over-Run interrupt occurred -----------------------------------------*/
        if(   ((isrflags & UART5_FLAG_ORE) != RESET)
        &&(  ((cr1its & UART5_CR1_RXNEIE) != RESET) ||
            ((cr3its & UART5_CR3_RXFTIE) != RESET) ||
            ((cr3its & UART5_CR3_EIE) != RESET)) )
        {
            __UART5_CLEAR_IT(huart, UART5_CLEAR_ORECF);
            huart->ErrorCode |= UART5_ERROR_ORE;
        }
        
        /* Call UART Error Call back function if need be --------------------------*/
        if(huart->ErrorCode != UART5_ERROR_NONE)
        {
            /* UART in mode Receiver ---------------------------------------------------*/
            if(((isrflags & UART5_FLAG_RXNE) != RESET)
            && (   ((cr1its & UART5_CR1_RXNEIE) != RESET)
                || ((cr3its & UART5_CR3_RXFTIE) != RESET))&& ((cr3its & UART5_DMAReq_Rx) == RESET) )/*TMC(20240409)-gsx:DMA+IDLE add cr3its & UART5_DMAReq_Rx*/
            {
                UART5_Receive_IT(huart);
            }
            
            /* Non Blocking error : transfer could go on.
            Error is notified to user through user error callback */
            TMC_UART5_ErrorCallback(huart);
            huart->ErrorCode = UART5_ERROR_NONE;
        }
        return;
    
    } /* End if some error occurs */
    
    
    /* UART in mode Transmitter ------------------------------------------------*/
    if(((isrflags & UART5_FLAG_TXE) != RESET)
    && (   ((cr1its & UART5_CR1_TXEIE) != RESET)
        || ((cr3its & UART5_CR3_TXFTIE) != RESET)) && ((cr3its & UART5_DMAReq_Tx) == RESET))/*TMC(20240409)-gsx:DMA+IDLE add cr3its & UART5_DMAReq_Tx*/
    {
        UART5_Transmit_IT(huart);
        return;
    }
    
    /* UART in mode Transmitter (transmission end) -----------------------------*/
    if(((isrflags & UART5_FLAG_TC) != RESET) && ((cr1its & UART5_CR1_TCIE) != RESET))
    {
        UART5_EndRxTransfer(huart);
        return;
    }
    
    /* UART TX FIFO Empty  -----------------------------------------------------*/
    if(((isrflags & UART5_FLAG_TXFE) != RESET) && ((cr1its & UART5_CR1_TXFEIE) != RESET))
    {
        CLEAR_BIT(huart->Instance->CR1, UART5_CR1_TXFEIE);
    }
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
static TMC_StatusTypeDef UART5_Transmit_IT(UART5_HandleTypeDef *huart)
{
	uint16_t* tmp;

	if ((huart->Init->WordLength == UART5_WordLength_9b) && (huart->Init->Parity == UART5_Parity_None))
	{
		tmp = (uint16_t*) huart->pTxBuffPtr;
		huart->Instance->TDR = (*tmp & (uint16_t)0x01FFU);
		huart->pTxBuffPtr += 2U;
	}
	else
	{
		huart->Instance->TDR = (uint8_t)(*huart->pTxBuffPtr++ & (uint8_t)0xFFU);
	}
	huart->TxXferCount--;
	if(huart->TxXferCount == 0U)
	{
		/* Disable the UART Transmit Buffer Empty Interrupt */
		if (READ_BIT(huart->Instance->CR1, UART5_CR1_FIFOEN) != RESET)
		{
			CLEAR_BIT(huart->Instance->CR3, UART5_CR3_TXFTIE);
		}
		else
		{
			CLEAR_BIT(huart->Instance->CR1, UART5_CR1_TXEIE);
		}

		/* Tx process is ended, restore huart->gState to Ready */
		huart->TxState = TMC_UART_STATE_READY;
		TMC_UART5_TxCpltCallback(huart);
	}
	return TMC_OK;
	
}
/**
  * @brief  Receive an amount of data in interrupt mode.
  * @note   Function is called under interruption only, once
  *		 interruptions have been enabled by UART5_Receive_IT()
  * @param  huart: uart.handle.
  * @retval TMC status
  */
static TMC_StatusTypeDef UART5_Receive_IT(UART5_HandleTypeDef *huart)
{
	uint16_t* tmp;
	uint16_t  uhMask = huart->Mask;
	uint16_t  uhdata;

	uhdata = (uint16_t) READ_REG(huart->Instance->RDR);
	if ((huart->Init->WordLength == UART5_WordLength_9b) && (huart->Init->Parity == UART5_Parity_None))
	{
	  tmp = (uint16_t*) huart->pRxBuffPtr ;
	  *tmp = (uint16_t)(uhdata & uhMask);
	  huart->pRxBuffPtr +=2;
	}
	else
	{
	  *huart->pRxBuffPtr++ = (uint8_t)(uhdata & (uint8_t)0x1FF);
	}

	if(--huart->RxXferCount == 0U)
	{
        if (READ_BIT(huart->Instance->CR1, UART5_CR1_FIFOEN) != RESET)
        {
            /* Disable the UART Parity Error Interrupt and RXFTIE interrupt*/
            CLEAR_BIT(huart->Instance->CR3, (UART5_CR3_RXFTIE | UART5_CR1_PEIE));
        }
        else
        {
            /* Disable the UART Parity Error Interrupt and RXNE interrupt*/
            CLEAR_BIT(huart->Instance->CR1, (UART5_CR1_RXNEIE | UART5_CR1_PEIE));
        }

	  /* Disable the UART Error Interrupt: (Frame error, noise error, overrun error) */
	  CLEAR_BIT(huart->Instance->CR3, UART5_CR3_EIE);

	  /* Rx process is completed, restore huart->RxState to Ready */
	  huart->RxState = TMC_UART_STATE_READY;

	  TMC_UART5_RxCpltCallback(huart);;
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
static TMC_StatusTypeDef UART5_WaitOnFlagUntilTimeout(UART5_HandleTypeDef *huart, uint32_t Flag, FlagStatus Status, uint32_t Tickstart, uint32_t Timeout)
{
	/* Wait until flag is set */
	while((UART5_GetFlagStatus(huart->Instance, Flag) ? SET : RESET) == Status) 
	{
		/* Check for the Timeout */
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U)||((TMC_GetTick() - Tickstart ) > Timeout))
			{
				/* Disable TXE, RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts for the interrupt process */
				CLEAR_BIT(huart->Instance->CR1, (UART5_CR1_RXNEIE | UART5_CR1_PEIE | UART5_CR1_TXEIE));
				CLEAR_BIT(huart->Instance->CR3, UART5_CR3_EIE);
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
  * @param[in] huart: uart.handle.
  * @retval None
  */
static void UART5_EndTxTransfer(UART5_HandleTypeDef *huart)
{
	/* Disable the UART Transmit Complete Interrupt */
	CLEAR_BIT(huart->Instance->CR1, UART5_CR1_TCIE);

	/* Tx process is ended, restore huart->gState to Ready */
	huart->TxState = TMC_UART_STATE_READY;
}
/**
  * @brief End ongoing Rx transfer on UART peripheral (following error detection or Reception completion).
  * @param[in] huart: uart.handle.
  * @retval None
  */
static void UART5_EndRxTransfer(UART5_HandleTypeDef *huart)
{
	/* Disable RXNE, PE and ERR (Frame error, noise error, overrun error) interrupts */
	CLEAR_BIT(huart->Instance->CR1, (UART5_CR1_RXNEIE | UART5_CR1_PEIE));
	CLEAR_BIT(huart->Instance->CR3, UART5_CR3_EIE);

	/* At end of Rx process, restore huart->RxState to Ready */
	huart->RxState = TMC_UART_STATE_READY;
}

/**
  * @brief DMA UART transmit process complete callback. 
  * @param[in] hdma: dma.handle
  * @retval None
  */
static void UART5_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
	UART5_HandleTypeDef* huart = ( UART5_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

	huart->TxXferCount = 0U;

	/* Disable the DMA transfer for transmit request */
	UART5_DMACmd(huart->Instance, UART5_DMAReq_Tx, DISABLE);
	
	/* Disable the transmit empty interrupt */
	//UART_ITConfig(huart->Instance, UART_FLAG_TXE, DISABLE);
	
	/* Restore huart->gState to Ready */
	huart->TxState = TMC_UART_STATE_READY;
	TMC_UART5_TxCpltCallback(huart);

}

/**
  * @brief DMA UART receive process complete callback. 
  * @param[in] hdma: dma.handle
  * @retval None
  */
static void UART5_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
	UART5_HandleTypeDef* huart = ( UART5_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
 
	huart->RxXferCount = 0U;
  
	/* Disable the DMA receive for transmit request */
	UART5_DMACmd(huart->Instance, UART5_DMAReq_Rx, DISABLE);
	
	/* Disable the UART Parity Error Interrupt and RXNE interrupt*/
	/* Disable PE and ERR (Frame error, noise error, overrun error) interrupts */
	CLEAR_BIT(huart->Instance->CR1, UART5_CR1_PEIE);
	CLEAR_BIT(huart->Instance->CR3, UART5_CR3_EIE);
	/* Tx process is ended, restore huart->gState to Ready */
	huart->RxState = TMC_UART_STATE_READY;
	TMC_UART5_RxCpltCallback(huart);

}

/**
  * @brief DMA UART communication error callback.
  * @param[in] hdma: dma.handle
  * @retval None
  */
static void UART5_DMAError(DMA_HandleTypeDef *hdma)
{
	UART5_HandleTypeDef* huart = ( UART5_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;

	/* Stop UART DMA Tx request if ongoing */
	if((huart->TxState == TMC_UART_STATE_BUSY) && (UART5_GetDMAStatus(huart->Instance, UART5_DMAReq_Tx) != RESET))
	{
		huart->TxXferCount = 0U;
		UART5_EndTxTransfer(huart);
	}

	/* Stop UART DMA Rx request if ongoing */
	if((huart->RxState == TMC_UART_STATE_BUSY) && (UART5_GetDMAStatus(huart->Instance, UART5_DMAReq_Rx) != RESET))
	{
		huart->RxXferCount = 0U;
		UART5_EndRxTransfer(huart);
	}

	huart->ErrorCode |= TMC_UART_ERROR_DMA;
	TMC_UART5_ErrorCallback(huart);
}

/** @brief  Report the UART mask to apply to retrieve the received data
  *		 according to the word length and to the parity bits activation.
  * @note   If PCE = 1, the parity bit is not included in the data extracted
  *		 by the reception API().
  *		 This masking operation is not carried out in the case of
  *		 DMA transfers.
  * @param  __HANDLE__: specifies the UART Handle.
  * @retval None, the mask to apply to UART RDR register is stored in huart->Mask field.
  */
void TMC_UART5_Mask_Computation(UART5_HandleTypeDef *huart)							 
{															   
	if (huart->Init->WordLength == UART5_WordLength_9b)		   
	{															 
		if (huart->Init->Parity == UART5_Parity_None)			
		{													   
			huart->Mask = 0x01FF ;							   
		}													   
		else													
		{													   
			huart->Mask = 0x00FF ;							   
		}													   
	}														  
	else if (huart->Init->WordLength == UART5_WordLength_8b)	
	{														  
		if (huart->Init->Parity == UART5_Parity_None)			
		{													   
			huart->Mask = 0x00FF ;							   
		}													   
		else													
		{													   
			huart->Mask = 0x007F ;							   
		}													   
	}														  
	else if (huart->Init->WordLength == (uint16_t)UART5_WordLength_7b)	
	{														  
		if (huart->Init->Parity == UART5_Parity_None)			
		{													   
			huart->Mask = 0x007F ;							   
		}													   
		else													
		{													   
			huart->Mask = 0x003F ;							   
		}													   
	}														  
}
/**
  * @brief  Check the TMC uart5  RxFIFO_Threshold
  * @param[in]  none
  * @retval RxFIFO Bytes
  */
int8_t TMC_Check_Uart5_RxFIFO_Threshold(uint32_t rx_fifo_threshold)
{
	if(rx_fifo_threshold==UART5_RXFIFO_THRESHOLD_1_8)
		return 2;
	else if(rx_fifo_threshold==UART5_RXFIFO_THRESHOLD_1_4)
		return 4;				  
	else if(rx_fifo_threshold==UART5_RXFIFO_THRESHOLD_1_2)
		return 8;				  
	else if(rx_fifo_threshold==UART5_RXFIFO_THRESHOLD_3_4)
		return 12;				 
	else if(rx_fifo_threshold==UART5_RXFIFO_THRESHOLD_7_8)
		return 14;				 
	else if(rx_fifo_threshold==UART5_RXFIFO_THRESHOLD_8_8)
		return 16;
	else 
		return -1;
}
/**
  * @brief UART MSP Init.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART5_MspInit(UART5_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
					the UART5_MspInit could be implemented in the user file
	*/ 
}

/**
  * @brief UART MSP DeInit.
  * @param[in] huart: pointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART5_MspDeInit(UART5_HandleTypeDef *huart)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(huart);
	/* NOTE: This function Should not be modified, when the callback is needed,
					the UART5_MspDeInit could be implemented in the user file
	*/ 
}

/**
  * @brief Tx Transfer completed callbacks.
  * @param[in] huart£ºpointer to a UART_HandleTypeDef structure that contains
  *				the configuration information for the specified UART module.
  * @retval None
  */
 __weak void TMC_UART5_TxCpltCallback(UART5_HandleTypeDef *huart)
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
__weak void TMC_UART5_RxCpltCallback(UART5_HandleTypeDef *huart)
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
 __weak void TMC_UART5_ErrorCallback(UART5_HandleTypeDef *huart)
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
/**
  * @}
  */
/**
  * @}
  */

/**
  * @}
  */

