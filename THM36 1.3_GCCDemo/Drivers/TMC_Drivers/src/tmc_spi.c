/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_spi.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
 
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "dma.h"
#include "tmc_dma.h"
#include "tmc_spi.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_SPI
  * @brief SPI TMC modules driver 
  * @{
  */

  
/** @defgroup TMC_SPI_private_defines TMC SPI private defines
  * @{
  */
#define ReceiveTimeOut			 	(0x00000FFFUL)
#define TramsmitTimeOut 			(0x00000FFFUL)

extern  uint32_t uwTick;
static TMC_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart);
//static void SPI_DMATransmitCplt(DMA_HandleTypeDef *hdma);
//static void SPI_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMATransmitReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAError(DMA_HandleTypeDef *hdma);
static void SPI_TxISR(struct __SPI_HandleTypeDef *hspi);
static void SPI_RxISR(struct __SPI_HandleTypeDef *hspi);
static void SPI_DMA_Transmit_handle(DMA_HandleTypeDef *hdma);
static void SPI_RxTxISR(struct __SPI_HandleTypeDef *hspi);
/**
  * @}
  */ 
  
/** @defgroup TMC_SPI_Exported_Function TMC SPI Exported Function
  * @brief SPI TMC modules driver
  * @{
  */
  
/**
  * @brief  Fills each SPI_HandleTypeDef member with its default value.
  * @param[in] SPI_HandleTypeDef: pointer to a SPI_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void TMC_SPI_StructInit(SPI_HandleTypeDef* hspi)
{
	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
	hspi->Lock = HAL_UNLOCKED;
	hspi->State = TMC_SPI_STATE_RESET;
	hspi->hdmarx = NULL;
	hspi->hdmatx = NULL;
	hspi->Instance = NULL;
	hspi->pRxBuffPtr = NULL;
	hspi->pTxBuffPtr = NULL;
	hspi->RxISR = NULL;
	hspi->RxXferCount = 0;
	hspi->RxXferSize = 0;
	hspi->TxISR = 0;
	hspi->TxXferCount = 0;
	hspi->TxXferSize = 0;
}
/**
  * @brief  Initializes the SPI Unit according to the specified
  *		 parameters in the SPI_HandleTypeDef and create the associated handle.
  * @param[in]  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *		 the configuration information for SPI module.
  * @retval None
  */
TMC_StatusTypeDef TMC_SPI_Init(SPI_HandleTypeDef* hspi) 
{

	/* check point of hspi is NULL */
	if(hspi == NULL)
	{
		return TMC_ERROR;
	}
	/* check state of SPI is RESERT */
	if(hspi->State == TMC_SPI_STATE_RESET)
	{
	 /* Init the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_SPI_MspInit(hspi);
	}
	/* Set state of SPI is BUSY */
	hspi->State = TMC_SPI_STATE_BUSY;
	/* Disable SPIx */
	SPIx_Cmd(hspi->Instance,DISABLE);
	/* Initializes SPIx */
	SPIx_Init(hspi->Instance,hspi->Init);
	
	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
	/* Set state of SPI is READY */
	hspi->State = TMC_SPI_STATE_READY;
	/* ENABLE SPIx */
	SPIx_Cmd(hspi->Instance,ENABLE);
	/* DISABLE SPIx's DMA mode*/
	SPIx_ConfigDMA(hspi->Instance,SPI_RX_DMA|SPI_TX_DMA,DISABLE);
	return TMC_OK;
}	
/**
  * @brief  DeInitializes the SPI peripheral 
  * @param[in]  hspi pointer to a SPI_HandleTypeDef structure that contains
  * 		the configuration information for SPI module.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_SPI_DeInit(SPI_HandleTypeDef* hspi) 
{
	/* check point of hspi is NULL */
	if(hspi == NULL)
	{
		return TMC_ERROR;
	}
	/* set the statse of hspi is BUSY */
	hspi->State = TMC_SPI_STATE_BUSY;
	/* Disable SPIx */
	SPIx_Cmd(hspi->Instance,DISABLE);
	
	/* DeInit SPIx */
	SPIx_DeInit(hspi->Instance);
	/* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
	TMC_SPI_MspDeInit(hspi);
	/* Set ErrorCode is TMC_SPI_ERROR_NONE*/
	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
	/* Set state is TMC_SPI_STATE_RESET*/
	hspi->State = TMC_SPI_STATE_RESET;

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
static TMC_StatusTypeDef SPI_WaitFlagStateUntilTimeout(SPI_HandleTypeDef *hspi, uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart)
{
	while((((hspi->Instance->STS & Flag) == (Flag)) ? SET : RESET) != State)
	{
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
			{
				/* Disable the SPI and reset the CRC: the CRC value should be cleared
				on both master and slave sides in order to resynchronize the master
				and slave for their respective CRC calculation */
				/* Disable TXE, RXNE and ERR interrupts for the interrupt process */
				SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT|SPI_TBE_IT,DISABLE);

				if(hspi->Init->Mode == SPI_MASTER_MODE)
				{
					/* Disable SPI peripheral */
					SPIx_Cmd(hspi->Instance,DISABLE);
				}
				hspi->State= TMC_SPI_STATE_READY;
				return TMC_TIMEOUT;
			}
		}
	}
	return TMC_OK;
}

/**
  * @brief  Receive an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be received
  * @param  Timeout Timeout duration
  * @retval TMC status
  */
TMC_StatusTypeDef TMC_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint32_t tickstart = 0U;

	/* Init tickstart for timeout management*/
	tickstart = TMC_GetTick();
	/* check State whether is TMC_SPI_STATE_READY,check pData is NULL,check Size is 0*/
	if((hspi->State !=TMC_SPI_STATE_READY)||(pData == NULL )||(Size == 0))
	{
		return TMC_ERROR;

	}
	/* Set the transaction information */
	hspi->State	   = TMC_SPI_STATE_BUSY_RX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pData;
	hspi->RxXferSize  = Size;
	hspi->RxXferCount = Size;

	/*Init field not used in handle to zero */
	hspi->pTxBuffPtr  = (uint8_t *)NULL;
	hspi->TxXferSize  = 0U;
	hspi->TxXferCount = 0U;
	if(hspi->Init->SQI_Select == SQI_MODE)
	{
		SPIx_ConfigSQI_IO(hspi->Instance,Input);
	}
	/* Check if the SPI is already enabled */
	if(!(hspi->Instance->CON2&SPI_ENABLE))
	{
		SPIx_Cmd(hspi->Instance,ENABLE);
	}
	while(hspi->RxXferCount > 0U)
	{
		if(hspi->Init->Mode == SPI_MASTER_MODE)
		{
			SPIx_SendData(hspi->Instance,0);
		}
		if(SPI_WaitFlagStateUntilTimeout(hspi,SPI_BOVER_FLAG,SET,Timeout,tickstart) == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		else
		{
			* pData= SPIx_ReceiveData(hspi->Instance);
			pData++;
			hspi->RxXferCount--;
		}
			
	}
	/* Check the end of the transaction */
	if(hspi->Init->Mode == SPI_MASTER_MODE)
	{
		/* Disable SPI peripheral */
		SPIx_Cmd(hspi->Instance,DISABLE);
	}

	hspi->State = TMC_SPI_STATE_READY;
	if(hspi->Init->SQI_Select == SQI_MODE)
	{
		SPIx_ConfigSQI_IO(hspi->Instance,Output);
	}
	return TMC_OK;
}


/**
  * @brief  Transmit an amount of data in blocking mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @param  Timeout Timeout duration
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	uint32_t tickstart = 0U;
	/* Init tickstart for timeout management*/
	tickstart = TMC_GetTick();
	/* check the state is not TMC_SPI_STATE_READY,check pData is NULL,check Size is zero*/
	if((hspi->State != TMC_SPI_STATE_READY)||(pData == NULL )||(Size == 0))
	{
		return TMC_ERROR;
	}

	/* Set the transaction information */
	hspi->State	   = TMC_SPI_STATE_BUSY_TX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pData;
	hspi->TxXferSize  = Size;
	hspi->TxXferCount = Size;

	/*Init field not used in handle to zero */
	hspi->pRxBuffPtr  = (uint8_t *)NULL;
	hspi->RxXferSize  = 0U;
	hspi->RxXferCount = 0U;

	/* Check if the SPI is already enabled */
	if(!(hspi->Instance->CON2&SPI_ENABLE))
	{
		SPIx_Cmd(hspi->Instance,ENABLE);
	}
	while(hspi->TxXferCount>0U)
	{		
		if(SPI_WaitFlagStateUntilTimeout(hspi,SPI_TBE_FLAG,SET,Timeout,tickstart) == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		SPIx_SendData(hspi->Instance,*pData);
		pData++;
		hspi->TxXferCount--;
		SPIx_ClearBoverStatus(hspi->Instance);
	}
//	if(SPI_WaitFlagStateUntilTimeout(hspi,SPI_BOVER_FLAG,SET,Timeout,tickstart) == TMC_TIMEOUT)
//	{
//		return TMC_TIMEOUT;
//	}
//	SPIx_ClearBoverStatus(hspi->Instance);
	hspi->State = TMC_SPI_STATE_READY;
	return TMC_OK;
}


/**
  * @brief  Transmit and Receive an amount of data in blocking mode.
  * @param[in]  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param[in]  pTxData: pointer to transmission data buffer
  * @param[in]  pRxData: pointer to reception data buffer
  * @param[in]  Size: amount of data to be sent and received
  * @param[in]  Timeout: Timeout duration
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout)
{	
	uint32_t tickstart = 0U;
	/* check State is not TMC_SPI_STATE_READY,check pTxData is NULL,check pRxData is NULL,check Size is 0*/
	if((hspi->State != TMC_SPI_STATE_READY)||(pTxData == NULL)||(pRxData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	/* Init tickstart for timeout management*/
	tickstart = TMC_GetTick();
	/* Set the transaction information */
	hspi->State	   = TMC_SPI_STATE_BUSY_TX_RX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pRxBuffPtr  = (uint8_t *)pRxData;
	hspi->RxXferCount = Size;
	hspi->RxXferSize  = Size;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferCount = Size;
	hspi->TxXferSize  = Size;
	SPIx_Cmd(hspi->Instance,ENABLE);
	while((hspi->TxXferCount>0U)||(hspi->RxXferCount>0U))
	{
		SPIx_SendData(hspi->Instance,*pTxData);
		if(SPI_WaitFlagStateUntilTimeout(hspi,SPI_BOVER_FLAG,SET,ReceiveTimeOut,tickstart) == TMC_TIMEOUT)
		{
			return TMC_TIMEOUT;
		}
		else
		{
			hspi->TxXferCount--;
			pTxData++;
			*pRxData = SPIx_ReceiveData(hspi->Instance);
			pRxData++;
			hspi->RxXferCount--;
		}
	}
	hspi->State = TMC_SPI_STATE_READY;
	return TMC_OK;
}
/**
  * @brief  Transmit  Size of data in blocking mode with interrupt.
  * @param[in]  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param[in]  pTxData: pointer to transmission data buffer
  * @param[in]  Size: amount of data to be sent and received
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData,uint16_t Size)
{
	if((hspi->State != TMC_SPI_STATE_READY)||(pTxData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	/* Set the transaction information */
	hspi->State	   = TMC_SPI_STATE_BUSY_TX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
	hspi->TxXferSize  = Size;
	hspi->TxXferCount = Size;
	/* Init field not used in handle to zero */
	hspi->pRxBuffPtr  = (uint8_t *)NULL;
	hspi->RxXferSize  = 0U;
	hspi->RxXferCount = 0U;
	hspi->TxISR = SPI_TxISR;
	/* Init interrupt of SPI_TBE_IT */
	SPIx_ITConfig(hspi->Instance,SPI_TBE_IT,ENABLE);
	/* Check if the SPI is already enabled */
	if(!(hspi->Instance->CON2&SPI_ENABLE))
	{
		SPIx_Cmd(hspi->Instance,ENABLE);
	}
	return TMC_OK;
}

/**
  * @brief  Receive Data of SPIx with interrupt
  * @param[in]  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pRxData, uint16_t Size)
{
	if((hspi->State != TMC_SPI_STATE_READY)||(pRxData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	/* Set the transaction information */
	hspi->State	   = TMC_SPI_STATE_BUSY_RX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = NULL;
	hspi->TxXferSize  = 0U;
	hspi->TxXferCount = 0U;
	hspi->TxISR = NULL;
	/* Init field not used in handle to zero */
	hspi->pRxBuffPtr  = pRxData;
	hspi->RxXferSize  = Size;
	hspi->RxXferCount = Size;
	hspi->RxISR = SPI_RxISR;
	SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT,ENABLE);
	if(hspi->Init->SQI_Select == SQI_MODE)
	{
		SPIx_ConfigSQI_IO(hspi->Instance,Input);
	}
	/* Check if the SPI is already enabled */
	if(!(hspi->Instance->CON2&SPI_ENABLE))
	{
		SPIx_Cmd(hspi->Instance,ENABLE);
	}
	if(hspi->Init->Mode == SPI_MASTER_MODE)
	{
		SPIx_SendData(hspi->Instance,0);
	}
	return TMC_OK;
}
/**
  * @brief  Transmit and Receive an amount of data in blocking mode.
  * @param[in]  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param[in]  pTxData: pointer to transmission data buffer
  * @param[in]  pRxData: pointer to reception data buffer
  * @param[in]  Size: amount of data to be sent and received
  * @param[in]  Timeout: Timeout duration
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size)
{
	if((hspi->State != TMC_SPI_STATE_READY)||(pRxData == NULL)||(Size == 0)||(pTxData == NULL))
	{
		return TMC_ERROR;
	}
	hspi->State	   = TMC_SPI_STATE_BUSY_TX_RX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = pTxData;
	hspi->TxXferSize  = Size;
	hspi->TxXferCount = Size;
	/* Init field not used in handle to zero */
	hspi->pRxBuffPtr  = pRxData;
	hspi->RxXferSize  = Size;
	hspi->RxXferCount = Size;
	hspi->RxTxISR = SPI_RxTxISR;
//	hspi->TxISR = SPI_TxISR;
//	hspi->RxISR = SPI_RxISR;
	/* set interrupt of SPI_TBE_IT or SPI_BOVER_IT */
	SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT,ENABLE);
	SPIx_Cmd(hspi->Instance,ENABLE);
	SPIx_SendData(hspi->Instance,*hspi->pTxBuffPtr);
	hspi->pTxBuffPtr++;  
	hspi->TxXferCount--;
	return TMC_OK;
}

///**
//  * @brief  Transmit an amount of data in non-blocking mode with DMA.
//  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
//  *			   the configuration information for SPI module.
//  * @param  pData pointer to data buffer
//  * @param  Size amount of data to be sent
//  * @retval HAL status
//  */
//TMC_StatusTypeDef TMC_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint16_t Size)
//{
//	/* Process Locked */
//	if((hspi->State == TMC_SPI_STATE_BUSY_TX_RX)||(hspi->State == TMC_SPI_STATE_BUSY_TX)||(hspi->State == TMC_SPI_STATE_BUSY_RX))
//	{
//		return TMC_BUSY;
//	}
//	if((hspi->State != TMC_SPI_STATE_READY)||(pTxData == NULL)||(Size == 0)||(pTxData == NULL))
//	{
//		return TMC_ERROR;
//	}
//	SPIx_Cmd(hspi->Instance,DISABLE);
//	/* Set the transaction information */
//	hspi->State	   = TMC_SPI_STATE_BUSY_TX;
//	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
//	hspi->pTxBuffPtr  = (uint8_t *)pTxData;
//	hspi->TxXferSize  = Size;
//	hspi->TxXferCount = Size;
//	/* Init field not used in handle to zero */
//	hspi->pRxBuffPtr  = (uint8_t *)NULL;
//	hspi->RxXferSize  = 0U;
//	hspi->RxXferCount = 0U;
//	/* Set the SPI TxDMA Half transfer complete callback */
//	hspi->hdmatx->XferCpltCallback = SPI_DMATransmitCplt;
//	/* Set the DMA error callback */
//	hspi->hdmatx->XferErrorCallback = SPI_DMAError;
//	/* Set the DMA AbortCpltCallback */
//	hspi->hdmatx->XferAbortCallback = NULL;
//	/* Disable interrupt of SPI_BOVER_IT or SPI_TBE_IT*/
//	SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT|SPI_TBE_IT,DISABLE);
//	/* Enable the Tx DMA Stream */
//	SPIx_Cmd(hspi->Instance,ENABLE);
//	TMC_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)(&hspi->Instance->DAT), hspi->TxXferSize);
//	
//	/* Check if the SPI is already enabled */
//	

//	return TMC_OK;
//}


///**
//  * @brief  Receive an amount of data in non-blocking mode with DMA.
//  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
//  *			   the configuration information for SPI module.
//  * @param  pData pointer to data buffer
//  * @note   When the CRC feature is enabled the pData Length must be Size + 1.
//  * @param  Size amount of data to be sent
//  * @retval HAL status
//  */
//TMC_StatusTypeDef TMC_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size)
//{
//	
//	if((hspi->State == TMC_SPI_STATE_BUSY_TX_RX)||(hspi->State == TMC_SPI_STATE_BUSY_TX)||(hspi->State == TMC_SPI_STATE_BUSY_RX))
//	{
//		return TMC_BUSY;
//	}
//	if((hspi->State != TMC_SPI_STATE_READY)||(pData == NULL)||(Size == 0))
//	{
//		return TMC_ERROR;
//	}
//	if(hspi->Init->Mode == SPI_MASTER_MODE)
//	{
//	/* the SPI mode is SPI_MASTER_MODE, TMC_SPI_Receive_DMA is unavailable*/
//		return TMC_ERROR;
//	}
//	/* Set the transaction information */
//	hspi->State	   = TMC_SPI_STATE_BUSY_RX;
//	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
//	hspi->pRxBuffPtr  = (uint8_t *)pData;
//	hspi->RxXferSize  = Size;
//	hspi->RxXferCount = Size;
//	/*Init field not used in handle to zero */
//	hspi->pTxBuffPtr  = (uint8_t *)NULL;
//	hspi->TxXferSize  = 0U;
//	hspi->TxXferCount = 0U;

//	/* Set the SPI Rx DMA transfer complete callback */
//	hspi->hdmarx->XferCpltCallback = SPI_DMAReceiveCplt;

//	/* Set the DMA error callback */
//	hspi->hdmarx->XferErrorCallback = SPI_DMAError;

//	/* Set the DMA AbortCpltCallback */
//	hspi->hdmarx->XferAbortCallback = NULL;
//	SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT|SPI_TBE_IT,DISABLE);
//	/* Enable the Rx DMA Stream */
//	TMC_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DAT, (uint32_t)hspi->pRxBuffPtr, hspi->RxXferSize);
//	if(hspi->Init->SQI_Select == SQI_MODE)
//	{
//		SPIx_ConfigSQI_IO(hspi->Instance,Input);
//	}
//	
//	/* Check if the SPI is already enabled */
//	if(!(hspi->Instance->CON2&SPI_ENABLE))
//	{
//		SPIx_Cmd(hspi->Instance,ENABLE);
//	}	
//	return TMC_OK;
//}

/**
  * @brief  Transmit and Receive an amount of data in non-blocking mode with DMA.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @param  pTxData pointer to transmission data buffer
  * @param  pRxData pointer to reception data buffer
  * @note   When the CRC feature is enabled the pRxData Length must be Size + 1
  * @param  Size amount of data to be sent
  * @param  Rx_LLI the address of Rx LLI,if Rx_LLI is 0,the Rx not make a LLI 
  * @param  Tx_LLI the address of Tx LLI,if Tx_LLI is 0,the Tx not make a LLI 
  * @retval HAL status
  * @note Rx_LLI's receive data 's length must be equal to Tx_LLI's transfer data's length
  */
TMC_StatusTypeDef TMC_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,uint32_t Rx_LLI,uint32_t Tx_LLI)
{
	if((hspi->State != TMC_SPI_STATE_READY)||(pTxData == NULL)||(Size == 0)||(pRxData == NULL))
	{
		return TMC_ERROR;
	}
	DMA->CONFIG = 0; 
	SPIx_Cmd(hspi->Instance,DISABLE);
	/* Set the transaction information */
	hspi->State = TMC_SPI_STATE_BUSY_TX_RX;
	hspi->ErrorCode   = TMC_SPI_ERROR_NONE;
	hspi->pTxBuffPtr  = pTxData;
	hspi->TxXferSize  = Size;
	hspi->TxXferCount = 0;
	hspi->pRxBuffPtr  = pRxData;
	hspi->RxXferSize  = Size;
	hspi->RxXferCount = 0;		
	//DMA->CONFIG=0;
	SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT|SPI_TBE_IT,DISABLE);
    SPIx_ConfigDMA(hspi->Instance,SPI_RX_DMA|SPI_TX_DMA,ENABLE);
	SPIx_Cmd(hspi->Instance,ENABLE);
	hspi->hdmarx->XferCpltCallback	= SPI_DMATransmitReceiveCplt;
	hspi->hdmarx->XferErrorCallback = SPI_DMAError;
	hspi->hdmarx->XferAbortCallback = NULL;
	hspi->hdmatx->XferCpltCallback	 = SPI_DMA_Transmit_handle;
	hspi->hdmatx->XferErrorCallback	= SPI_DMAError;
	hspi->hdmatx->XferAbortCallback	= NULL;
	hspi->hdmatx->Instance->LLI = Tx_LLI;
	hspi->hdmarx->Instance->LLI = Rx_LLI;
	/* Enable the Rx DMA Stream */
	
	TMC_DMA_Start_IT(hspi->hdmarx, (uint32_t)&hspi->Instance->DAT, (uint32_t)hspi->pRxBuffPtr,hspi->RxXferSize);
	TMC_DMA_Start_IT(hspi->hdmatx, (uint32_t)hspi->pTxBuffPtr, (uint32_t)&hspi->Instance->DAT, hspi->TxXferSize);
	
	DMA->CONFIG = 1; 
	return TMC_OK;
}

/**
  * @brief Stop the DMA Transfer.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for the specified SPI module.
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_SPI_DMAStop(SPI_HandleTypeDef *hspi)
{
	if(hspi == NULL)
	{
		return TMC_ERROR;
	}
	/* Abort the SPI DMA tx Stream */
	if(hspi->hdmatx != NULL)
	{
		TMC_DMA_Abort(hspi->hdmatx);
	}
	/* Abort the SPI DMA rx Stream */
	if(hspi->hdmarx != NULL)
	{
		TMC_DMA_Abort(hspi->hdmarx);
	}
	/* Disable the SPI DMA Tx & Rx requests */
	hspi->State = TMC_SPI_STATE_READY;
	return TMC_OK;
}


/**
  * @brief  Handle SPI interrupt request.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for the specified SPI module.
  * @retval None
  */
void TMC_SPI_IRQHandler(SPI_HandleTypeDef *hspi)
{
	if(hspi->State == TMC_SPI_STATE_BUSY_TX)
	{
		hspi->TxISR(hspi);
	}
	if(hspi->State == TMC_SPI_STATE_BUSY_RX)
	{
		hspi->RxISR(hspi);
	}
	if(hspi->State == TMC_SPI_STATE_BUSY_TX_RX)
	{
		hspi->RxTxISR(hspi);
	}
}



/**
  * @brief  Return the TMC SPI  state.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval TMC_SPI_StateTypeDef  state
  */
TMC_SPI_StateTypeDef TMC_SPI_GetState(SPI_HandleTypeDef *hspi)
{
	/* Return SPI handle state */
	return hspi->State;
}

///**
//  * @brief  DMA SPI half transmit process complete callback.
//  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
//  *			   the configuration information for the specified DMA module.
//  * @retval None
//  */
//static void SPI_DMATransmitCplt(DMA_HandleTypeDef *hdma)
//{
//	SPI_HandleTypeDef* hspi = (SPI_HandleTypeDef*)((DMA_HandleTypeDef*)hdma)->Parent;
//	hspi->State = TMC_SPI_STATE_READY;
//	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
//	//SPIx_Cmd(hspi->Instance,DISABLE);
//	TMC_SPI_TxCpltCallback(hspi);
//}

///**
//  * @brief  DMA SPI half receive process complete callback
//  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
//  *			   the configuration information for the specified DMA module.
//  * @retval None
//  */
//static void SPI_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
//{
//	SPI_HandleTypeDef* hspi = ( SPI_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
//	
//	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
//	hspi->State = TMC_SPI_STATE_READY;
//	SPIx_Cmd(hspi->Instance,DISABLE);
//	if(hspi->Init->SQI_Select == SQI_MODE)
//	{
//		SPIx_ConfigSQI_IO(hspi->Instance,Output);
//	}
//	TMC_SPI_RxCpltCallback(hspi);
//}

/**
  * @brief  DMA SPI half transmit receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMATransmitReceiveCplt(DMA_HandleTypeDef *hdma)
{
	SPI_HandleTypeDef* hspi = ( SPI_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hspi->State = TMC_SPI_STATE_READY;
	hspi->ErrorCode = TMC_SPI_ERROR_NONE;
	//SPIx_Cmd(hspi->Instance,DISABLE);
	TMC_SPI_TxRxCpltCallback(hspi);
}

/**
  * @brief  DMA SPI communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAError(DMA_HandleTypeDef *hdma)
{
	SPI_HandleTypeDef* hspi = (SPI_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hspi->State = TMC_SPI_STATE_ERROR;
	hspi->ErrorCode = TMC_SPI_ERROR_DMA;
	SPIx_Cmd(hspi->Instance,DISABLE);
	TMC_SPI_ErrorCallback(hspi);
}


static void SPI_DMA_Transmit_handle(DMA_HandleTypeDef *hdma)
{
	return;
}
/**
  * @brief Tx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_RxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief Tx and Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxRxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief Tx and Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_ErrorCallback(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_TxRxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the SPI MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_MspInit(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the SPI MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_MspDeInit(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}


/**
  * @brief  when the SPI mode is SPI_MASTER_MODE, SSN is configed with Low level
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_SSN_Enable(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  when the SPI mode is SPI_MASTER_MODE, SSN is configed with High level
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_SSN_Disable(SPI_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  Tx  handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
static void SPI_TxISR(struct __SPI_HandleTypeDef *hspi)
{
	
	/* check the end of the transmission */
	if((hspi->TxXferCount == 0U)&&(hspi->State == TMC_SPI_STATE_BUSY_TX))
	{
		/* Disable TXE interrupt */
		SPIx_ITConfig(hspi->Instance,SPI_TBE_IT,DISABLE);
		hspi->ErrorCode = TMC_SPI_ERROR_NONE;
		hspi->State = TMC_SPI_STATE_READY;
		TMC_SPI_TxCpltCallback(hspi);
		return;
	}
	SPIx_SendData(hspi->Instance,*hspi->pTxBuffPtr);
	hspi->pTxBuffPtr++;
	hspi->TxXferCount--;
	
}

/**
  * @brief  Rx  handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
static void SPI_RxISR(struct __SPI_HandleTypeDef *hspi)
{
	/* Receive data in 16 Bit mode */
	*((uint8_t*)hspi->pRxBuffPtr) = SPIx_ReceiveData(hspi->Instance);
	hspi->RxXferCount--;
	hspi->pRxBuffPtr++;
	if((hspi->RxXferCount == 0U)&&(hspi->TxXferCount == 0U))
	{
		if(hspi->State == TMC_SPI_STATE_BUSY_RX)
		{
			/* Disable RXNE interrupt */
			SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT,DISABLE);
			hspi->ErrorCode = TMC_SPI_ERROR_NONE;
			hspi->State = TMC_SPI_STATE_READY;
			if(hspi->Init->SQI_Select == SQI_MODE)
			{
				SPIx_ConfigSQI_IO(hspi->Instance,Output);
			}
			TMC_SPI_RxCpltCallback(hspi);
		}
		if(hspi->State == TMC_SPI_STATE_BUSY_TX_RX)
		{
			/* Disable RXNE interrupt */
			SPIx_ITConfig(hspi->Instance,SPI_BOVER_IT,DISABLE);
			hspi->ErrorCode = TMC_SPI_ERROR_NONE;
			hspi->State = TMC_SPI_STATE_READY;
			TMC_SPI_TxRxCpltCallback(hspi);
		}
		return;
	}
	if(hspi->Init->Mode == SPI_MASTER_MODE)
	{
		SPIx_SendData(hspi->Instance,0);
	}
}
/**
  * @brief  Tx  handler for Transmit and Receive in Interrupt mode.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
static void SPI_RxTxISR(struct __SPI_HandleTypeDef *hspi)
{
	
	*((uint8_t*)hspi->pRxBuffPtr) = SPIx_ReceiveData(hspi->Instance);
	hspi->RxXferCount--;
	hspi->pRxBuffPtr++;
	if((hspi->RxXferCount ==0)&&(hspi->TxXferCount==0))
	{
		SPIx_ITConfig(hspi->Instance,SPI_TBE_IT|SPI_BOVER_IT,DISABLE);
		SPIx_Cmd(hspi->Instance,DISABLE);
		hspi->ErrorCode = TMC_SPI_ERROR_NONE;
		hspi->State = TMC_SPI_STATE_READY;
		TMC_SPI_TxRxCpltCallback(hspi);
	}
	SPIx_SendData(hspi->Instance,*hspi->pTxBuffPtr);
	hspi->pTxBuffPtr++;  
	hspi->TxXferCount--;
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

