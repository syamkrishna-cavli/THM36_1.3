/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_spi_controller.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "TMC_DMA.h"
#include "TMC_SPI_Controller.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_SPI_CONTROLLER
  * @brief SPI CONTROLLER TMC modules driver 
  * @{
  */

/** @defgroup TMC_SPI_CONTROLLER_private_defines TMC SPI CONTROLLER private defines
  * @{
  */
static TMC_StatusTypeDef SPI_Controller_WaitFlagStateUntilTimeout(uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart);  
static void SPI_Controller_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void SPI_Controller_DMATransmitCplt(DMA_HandleTypeDef *hdma);
static void SPI_DMAError(DMA_HandleTypeDef *hdma);
/**
  * @}
  */ 
  
  
/** @defgroup TMC_SPI_CONTROLLER_Exported_Function TMC SPI CONTROLLER Exported Function
  * @brief SPI TMC modules driver
  * @{
  */
  

/**
  * @brief  Fills each SPI_Controller_HandleTypeDef member with its default value.
  * @param[in] SPI_Controller_HandleTypeDef: pointer to a SPI_Controller_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
void TMC_TMC_SPI_Controller_StructInit(SPI_Controller_HandleTypeDef *hqspi)
{
	hqspi->ErrorCode = 0;
	hqspi->hdma = NULL;
	hqspi->Init = NULL;
	hqspi->pRxBuffPtr = NULL;
	hqspi->pTxBuffPtr = NULL;
	hqspi->RxXferCount = NULL;
	hqspi->RxXferSize = NULL;
	hqspi->State = TMC_SPI_CONTROLLER_STATE_RESET;
	hqspi->TxXferCount = NULL;
	hqspi->TxXferSize = NULL;
}

/**
  * @brief Initializes the QSPI mode according to the specified parameters
  *		in the QSPI_InitTypeDef and creates the associated handle.
  * @param hqspi qspi handle
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_SPI_Controller_Init(SPI_Controller_HandleTypeDef *hqspi)
{
	if(hqspi == NULL)
	{
		return TMC_ERROR;
	}
	if(hqspi->State == TMC_SPI_CONTROLLER_STATE_RESET)
	{
		TMC_SPI_Controller_MspInit(hqspi);
	}
    SPI_Controller_StructInit(hqspi->Init);
	SPI_Controller_Init(hqspi->Init);
	hqspi->State = TMC_SPI_CONTROLLER_STATE_READY;
	hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	return TMC_OK;
}

/**
  * @brief Initializes the QSPI mode according to the specified parameters
  *		in the QSPI_InitTypeDef and creates the associated handle.
  * @param hqspi qspi handle
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_SPI_Controller_DeInit(SPI_Controller_HandleTypeDef *hqspi)
{
	if(hqspi == NULL)
	{
		return TMC_ERROR;
	}
	TMC_SPI_Controller_MspDeInit(hqspi);
	SPI_Controller_DeInit();
	hqspi->State = TMC_SPI_CONTROLLER_STATE_RESET;
	hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	return TMC_OK;
}


/**
  * @brief Initializes the SPI_Controller mode according to the specified parameters
  *		in the SPI_Controller_HandleTypeDef and creates the associated handle.
  * @param  SPI_Controller_HandleTypeDef hqspi
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_SPI_Controller_Command(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd)
{
	if((hqspi == NULL)||(cmd == NULL))
	{
		return TMC_ERROR;
	}
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	if((cmd->CmdW3->DTR_Mode == ENABLE)&&((hqspi->Init->SPI_Controller_Div == SPI_CONTROLLER_DIV_6)||(hqspi->Init->SPI_Controller_Div == SPI_CONTROLLER_DIV_2)))
	{
		return TMC_ERROR;
	}
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_COMMAND;
	hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	while(SPI_Controller_GetInterruptrStatus()== RESET);
	SPI_Controller_ClearInterruptrStatus();
	hqspi->State = TMC_SPI_CONTROLLER_STATE_READY;
	return TMC_OK;
}


/**
  * @brief  
  * @param
  * @param 
  * @param 
  * @note   
  * @retval 
  */
TMC_StatusTypeDef TMC_SPI_Controller_Receive(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd, uint32_t* buf,uint32_t Timeout)
{
	uint32_t tickstart = 0U;

	/* Init tickstart for timeout management*/
	tickstart = TMC_GetTick();
	
	if((hqspi == NULL)||(cmd == NULL)||(Timeout == 0))
	{
		return TMC_ERROR;
	}
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length*4);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	
	hqspi->pRxBuffPtr = buf;
	hqspi->RxXferSize = cmd->Data_Length;
	hqspi->RxXferCount = 0;
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_RX;
	
	while(hqspi->RxXferCount<hqspi->RxXferSize)
	{
		if(SPI_Controller_WaitFlagStateUntilTimeout(SPI_CONTROLLER_STS_RXF,SET,Timeout,tickstart) == TMC_TIMEOUT)
		{
			hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_TIMEOUT;
			return TMC_TIMEOUT;
		}
		*hqspi->pRxBuffPtr = SPI_Controller_GetdData();
		hqspi->pRxBuffPtr++;
		hqspi->RxXferCount++;
	}
	while(SPI_Controller_GetInterruptrStatus()== RESET);
	SPI_Controller_ClearInterruptrStatus();
	hqspi->State = TMC_SPI_CONTROLLER_STATE_READY;
	hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	return TMC_OK;
}

/**
  * @brief  
  * @param
  * @param 
  * @param 
  * @note   
  * @retval 
  */
TMC_StatusTypeDef TMC_SPI_Controller_Transmit(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf,uint32_t Timeout)
{
	uint32_t tickstart = 0U;

	tickstart = TMC_GetTick();
	
	if((hqspi == NULL)||(cmd == NULL)||(Timeout == 0))
	{
		return TMC_ERROR;
	}
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length*4);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	
	hqspi->pTxBuffPtr = buf;
	hqspi->TxXferSize = cmd->Data_Length;
	hqspi->TxXferCount = 0;
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_TX;
	
	while(hqspi->TxXferCount<hqspi->TxXferSize)
	{
		if(SPI_Controller_WaitFlagStateUntilTimeout(SPI_CONTROLLER_STS_TXE,SET,Timeout,tickstart) == TMC_TIMEOUT)
		{
			hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_TIMEOUT;
			return TMC_TIMEOUT;
		}
		SPI_Controller_WritedData(*hqspi->pTxBuffPtr);
		hqspi->pTxBuffPtr++;
		hqspi->TxXferCount++;
	}
	while(SPI_Controller_GetInterruptrStatus()== RESET);
	SPI_Controller_ClearInterruptrStatus();
	hqspi->State = TMC_SPI_CONTROLLER_STATE_READY;
	hqspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	return TMC_OK;
}


/**
  * @brief  
  * @param
  * @param 
  * @param 
  * @note   
  * @retval 
  */
TMC_StatusTypeDef TMC_SPI_Controller_Receive_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf)
{

	if((hqspi == NULL)||(cmd == NULL))
	{
		return TMC_ERROR;
	}
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	hqspi->pRxBuffPtr = buf;
	hqspi->RxXferSize = cmd->Data_Length;
	hqspi->RxXferCount = 0;
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_RX;
	
	SPI_Controller_ConfigRX_FIFO(SPI_CONTROLLER_RX_FIFO_24_WORD);
	
	SPI_Controller_ConfigCommandOverEnable(ENABLE);
	return TMC_OK;
}



/**
  * @brief  
  * @param
  * @param 
  * @param 
  * @note   
  * @retval 
  */
TMC_StatusTypeDef TMC_SPI_Controller_Transmit_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf)
{
	
	if((hqspi == NULL)||(cmd == NULL))
	{
		return TMC_ERROR;
	}
	/* Wether Command had be Send*/
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	
	hqspi->pTxBuffPtr = buf;
	hqspi->TxXferSize = cmd->Data_Length;
	hqspi->TxXferCount = 0;
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_TX;
	
	SPI_Controller_ConfigTX_FIFO(SPI_CONTROLLER_TX_FIFO_24_WORD);
	SPI_Controller_ConfigCommandOverEnable(ENABLE);
	return TMC_OK;
}

/**
  * @brief  Handle SPI Controller interrupt request.
  * @param  hspi pointer to a SPI_Controller_HandleTypeDef structure that contains
  *			   the configuration information for the specified SPI module.
  * @retval None
  */
void TMC_SPI_Controller_IRQHandler(SPI_Controller_HandleTypeDef  *hspi)
{

}


/**
  * @brief  
  * @param
  * @param 
  * @param 
  * @note   
  * @retval 
  */
TMC_StatusTypeDef TMC_SPI_Controller_DMA_Receive_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf)
{	
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length*4);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	
	hqspi->pRxBuffPtr = buf;
	hqspi->RxXferSize = cmd->Data_Length;
	hqspi->RxXferCount = 0;
	hqspi->hdma->XferCpltCallback = SPI_Controller_DMAReceiveCplt;
	hqspi->hdma->XferErrorCallback =  SPI_DMAError;
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_RX;

	TMC_DMA_Start_IT(hqspi->hdma, (uint32_t)&SPI2->DAT, (uint32_t)hqspi->pRxBuffPtr,hqspi->RxXferSize);
	return TMC_OK;
}


/**
  * @brief  
  * @param
  * @param 
  * @param 
  * @note   
  * @retval 
  */
TMC_StatusTypeDef TMC_SPI_Controller_DMA_Transmit_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf)
{	
	if(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY)
	{
		return TMC_ERROR;
	}
	
	hqspi->pTxBuffPtr = buf;
	hqspi->TxXferSize = cmd->Data_Length;
	hqspi->TxXferCount = 0;

	/* Set the DMA abort callback */
	hqspi->hdma->XferAbortCallback = NULL;
	hqspi->hdma->XferCpltCallback = SPI_Controller_DMATransmitCplt;
	hqspi->hdma->XferErrorCallback =  SPI_DMAError;
	
	hqspi->State = TMC_SPI_CONTROLLER_STATE_BUSY_TX;
	
	SPI_Controller_ConfigFlashAddress(cmd->Flash_Addr);
	SPI_Controller_Write_CWD1(cmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(cmd->Data_Length*4);
	SPI_Controller_Write_CWD3(cmd->CmdW3);
	
	TMC_DMA_Start_IT(hqspi->hdma, (uint32_t)hqspi->pTxBuffPtr,(uint32_t)&SPI2->DAT,hqspi->TxXferSize );
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
static TMC_StatusTypeDef SPI_Controller_WaitFlagStateUntilTimeout(uint32_t Flag, uint32_t State, uint32_t Timeout, uint32_t Tickstart)
{
	while(((((SPI2->STS) & Flag) == (Flag)) ? SET : RESET) != State)
	{
		if(Timeout != HAL_MAX_DELAY)
		{
			if((Timeout == 0U) || ((TMC_GetTick()-Tickstart) >= Timeout))
			{
				SPI_Controller_ConfigAbort();
				while(SPI_Controller_GetAbortStatus() == SET);
				return TMC_TIMEOUT;
			}
		}
	}
	return TMC_OK;
}
/**
  * @brief  DMA SPI half transmit receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_Controller_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
	SPI_Controller_HandleTypeDef* hspi = ( SPI_Controller_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hspi->State = TMC_SPI_CONTROLLER_STATE_READY;
	hspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	while(SPI_Controller_GetInterruptrStatus()== RESET);
	SPI_Controller_ClearInterruptrStatus();
	TMC_SPI_Controller_RxCpltCallback(hspi);
}


/**
  * @brief  DMA SPI half transmit receive process complete callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_Controller_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
	SPI_Controller_HandleTypeDef* hspi = ( SPI_Controller_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hspi->State = TMC_SPI_CONTROLLER_STATE_READY;
	hspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	while(SPI_Controller_GetInterruptrStatus()== RESET);
	SPI_Controller_ClearInterruptrStatus();
	TMC_SPI_Controller_TxCpltCallback(hspi);
}

/**
  * @brief  DMA SPI communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void SPI_DMAError(DMA_HandleTypeDef *hdma)
{
	SPI_Controller_HandleTypeDef* hspi = (SPI_Controller_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	hspi->State = TMC_SPI_CONTROLLER_STATE_ERROR;
	hspi->ErrorCode = TMC_SPI_CONTROLLER_ERROR_DMA;
//	SPIx_Cmd(hspi->Instance,DISABLE);
//	TMC_SPI_ErrorCallback(hspi);
}


/**
  * @brief Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_Controller_RxCpltCallback(SPI_Controller_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_RxCpltCallback should be implemented in the user file
	*/
}


/**
  * @brief Rx Transfer completed callback.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_Controller_TxCpltCallback(SPI_Controller_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_RxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the SPI Controller MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_Controller_MspInit(SPI_Controller_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the SPI Controller MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for SPI module.
  * @retval None
  */
__weak void TMC_SPI_Controller_MspDeInit(SPI_Controller_HandleTypeDef *hspi)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hspi);
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
/**
  * @}
  */ 
  
  
  
  


