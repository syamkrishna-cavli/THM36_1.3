 /******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_iso7816Mx.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "TMC_DMA.h"
#include "TMC_ISO7816Mx.h"
#include "string.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_ISO7816Mx
  * @brief ISO7816Mx TMC modules driver 
  * @{
  */ 
/** @defgroup TMC_ISO7816Mx_private_defines TMC ISO7816Mx private defines
  * @{
  */
	
static void TMC_M7816x_RxISR(struct __M7816x_HandleTypeDef *h7816m);
static void TMC_M7816x_TxISR(struct __M7816x_HandleTypeDef *h7816m);
static void TMC_M7816x_DMAReceiveCplt(DMA_HandleTypeDef *hdma);
static void TMC_M7816x_DMAError(DMA_HandleTypeDef *hdma);
static void TMC_M7816x_DMATransmitCplt(DMA_HandleTypeDef *hdma);
/**
  * @}
  */
/** @defgroup TMC_M7816x_Exported_Function TMC M7816x Exported Function
  * @brief M7816x TMC modules driver
  * @{
  */ 





/**
  * @brief  Fills each M7816x_HandleTypeDef member with its default value.
  * @param[in] M7816x_HandleTypeDef: pointer to a M7816x_HandleTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
void TMC_M7816x_StructInit(M7816x_HandleTypeDef* h7816m)
{
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	h7816m->State = TMC_M7816x_STATE_RESET;
	h7816m->hdmarx = NULL;
	h7816m->hdmatx = NULL;
	h7816m->Instance = NULL;
	h7816m->Lock = HAL_UNLOCKED;
	h7816m->M7816x_RxISR = NULL;
	h7816m->M7816x_TxISR = NULL;
	h7816m->pRxBuffPtr = NULL;
	h7816m->RxXferSize = NULL;
	h7816m->RxXferCount = NULL;
	h7816m->pTxBuffPtr = NULL;
	h7816m->TxXferSize = NULL;
	h7816m->TxXferCount = NULL;
} 

/**
  * @brief  Initializes the M7816x Unit according to the specified
  *		 parameters in the M7816x_HandleTypeDef and create the associated handle.
  * @param[in]  h7816m: pointer to a M7816x_HandleTypeDef structure that contains
  *		 the configuration information for M7816x module.
  * @retval None
  */
TMC_StatusTypeDef TMC_M7816x_Init(M7816x_HandleTypeDef* h7816m) 
{

	/* check point of hspi is NULL */
	if(h7816m == NULL)
	{
		return TMC_ERROR;
	}
	/* check state of SPI is RESERT */
	if(h7816m->State == TMC_M7816x_STATE_RESET)
	{
	 /* Init the low level hardware : GPIO, CLOCK, NVIC... */	
		TMC_M7816x_MspInit(h7816m);
	}
	/* Disable M7816x */
	M7816x_Cmd(h7816m->Instance,DISABLE);
	/* Initializes M7816x */
	M7816x_Init(h7816m->Instance,h7816m->Init);
	
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	/* Set state of SPI is READY */
	h7816m->State = TMC_M7816x_STATE_READY;
	/* ENABLE M7816x */
	M7816x_Cmd(h7816m->Instance,ENABLE);
	/* DISABLE M7816x's DMA mode*/
	M7816x_ConfigDMA(h7816m->Instance,DISABLE);
	M7816x_ITConfigHMSK(h7816m->Instance,M7816x_HSTS_Mask_PRTL|M7816x_HSTS_Mask_PRL,DISABLE);
	M7816x_ITConfigMMSK(h7816m->Instance,M7816x_MSTS_Mask_BGT|M7816x_MSTS_Mask_RBF|M7816x_MSTS_Mask_TBE,DISABLE);
	M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3|M7816x_USTS_Mask_TO2|M7816x_USTS_Mask_TO1|M7816x_USTS_Mask_EA|M7816x_USTS_Mask_PE|
						M7816x_USTS_Mask_OVR|M7816x_USTS_Mask_FER|M7816x_USTS_Mask_TACT,DISABLE);
	return TMC_OK;
}

/**
  * @brief  DeInitializes the M7816x peripheral 
  * @param[in]  hspi pointer to a M7816x_HandleTypeDef structure that contains
  * 		the configuration information for M7816x module.
  * @retval TMC_StatusTypeDef status
  */
TMC_StatusTypeDef TMC_M7816x_DeInit(M7816x_HandleTypeDef* h7816m) 
{
	/* check point of hspi is NULL */
	if(h7816m == NULL)
	{
		return TMC_ERROR;
	}

	/* Disable SPIx */
	M7816x_Cmd(h7816m->Instance,DISABLE);
	
	/* DeInit SPIx */
	M7816x_DeInit(h7816m->Instance);
	/* DeInit the low level hardware: GPIO, CLOCK, NVIC... */
	TMC_M7816x_MspDeInit(h7816m);
	/* Set ErrorCode is TMC_SPI_ERROR_NONE*/
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	/* Set state is TMC_SPI_STATE_RESET*/
	h7816m->State = TMC_M7816x_STATE_RESET;

	return TMC_OK;
} 
/**
  * @brief  Receive Data of ISO7816Mx with polling
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
*	@param[in]  TimeOut: the value of 7816Mx's Timer 
  * @param[in]  StopTOC: 7816Mx Timer Stop Mode
  *				@arg M7816x_TIM_STOP_TO1_TO2_TO3 :Stop TOR1+TOR2+TOR3
  *				@arg M7816x_TIM_STOP_TO2_TO3:Stop TOR2+TOR3,Not Stop TOR2+TOR3 
  * @param[in]  StartTOC: 7816Mx Timer Start Mode,value of @ref M7816_TOC_MODE
  * @retval TMC_StatusTypeDef status
  * @note : if want to start a new count of 7816Mx Timer,need to finish Timer(Stop Timer) 
  */
TMC_StatusTypeDef TMC_M7816x_Receive(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size) 
{
	if((h7816m == NULL)||(h7816m->State != TMC_M7816x_STATE_READY)||(pData == 0)||(Size == 0))
	{
		return TMC_ERROR;
	}
	h7816m->pRxBuffPtr = pData;
	h7816m->RxXferSize = Size;
	h7816m->RxXferCount = Size;
	h7816m->TxXferCount = 0;
	h7816m->TxXferSize = 0;
	h7816m->pTxBuffPtr = NULL;
	h7816m->State = TMC_M7816x_STATE_RX_BUSY;
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	
	M7816x_ClearUStatus(h7816m->Instance, M7816x_USTS_TO1 | M7816x_USTS_TO2 | M7816x_USTS_TO3);

	while(h7816m->RxXferCount)
	{
		while(M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_RBF) == RESET)
		{
			if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TO3)==SET)
			{
				M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TO3);
				h7816m->ErrorCode = TMC_M7816x_ERROR_TIME_OUT;
				h7816m->State = TMC_M7816x_STATE_TIMEOUT;
				return TMC_TIMEOUT;
			}
			if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_EA)==SET)
			{
				M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_EA);
				h7816m->ErrorCode = TMC_M7816x_ERROR_EA;
				h7816m->State = TMC_M7816x_STATE_ERROR;
				return TMC_ERROR;
			}
			if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_PE) == SET)
			{
				M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_PE);
				h7816m->ErrorCode = TMC_M7816x_ERROR_PARITY;
				h7816m->State = TMC_M7816x_STATE_ERROR;
			}
//			if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_OVR) == SET)
//			{
//				M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_OVR);
//				h7816m->ErrorCode = TMC_M7816x_ERROR_OVERFLOW;
//				h7816m->State = TMC_M7816x_STATE_ERROR;
//				TMC_M7816x_ErrorCallback(h7816m);
//			}
			if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_FER) == SET)
			{
				M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_FER);
				h7816m->ErrorCode = TMC_M7816x_ERROR_FER;
				h7816m->State = TMC_M7816x_STATE_ERROR;
				TMC_M7816x_ErrorCallback(h7816m);
			}
		}
		*((uint8_t*)h7816m->pRxBuffPtr) = M7816x_ReciveData(h7816m->Instance);
		h7816m->RxXferCount--;
		h7816m->pRxBuffPtr++;
	}
	h7816m->State = TMC_M7816x_STATE_READY;
	return TMC_OK;
}
/**
  * @brief  Transmit Datas of ISO7816Mx with polling
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be Transmited
  * @param[in]  StopTOC: 7816Mx Timer Stop Mode
  *				@arg M7816x_TIM_STOP_TO1_TO2_TO3 :Stop TOR1+TOR2+TOR3
  *				@arg M7816x_TIM_STOP_TO2_TO3:Stop TOR2+TOR3,Not Stop TOR2+TOR3 
  * @param[in]  StartTOC: 7816Mx Timer Start Mode,value of @ref M7816_TOC_MODE
  * @retval TMC_StatusTypeDef status
  * @note : if want to start a new count of 7816Mx Timer,need to finish Timer(Stop Timer) 
  *		 when the last byte transmit,set TOC of M7816x Timer,Set TimeOut 
  */
TMC_StatusTypeDef TMC_M7816x_Transmit(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size,uint8_t StopTOC,uint8_t StartTOC,uint32_t TimeOut) 
{

	/* check point of hspi is NULL */
	if((h7816m->State == TMC_M7816x_STATE_RX_BUSY)||(h7816m->State == TMC_M7816x_STATE_TX_BUSY))
	{
		return TMC_BUSY;
	}
	if((h7816m == NULL)||(h7816m->State != TMC_M7816x_STATE_READY)||(pData == 0)||(Size == 0)||(TimeOut == 0))
	{
		return TMC_ERROR;
	}
	h7816m->pRxBuffPtr = 0;
	h7816m->RxXferSize = 0;
	h7816m->RxXferCount = 0;
	h7816m->TxXferCount = Size;
	h7816m->TxXferSize = Size;
	h7816m->pTxBuffPtr = pData;
	h7816m->State = TMC_M7816x_STATE_TX_BUSY;
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	h7816m->TimerStartMode = StartTOC;
	h7816m->TimerStopMode = StopTOC;
	h7816m->TimerValue = TimeOut;
	M7816x_ConfigIODirection(h7816m->Instance,M7816x_SEND_MODE);
	if(h7816m->TxXferCount == 1)
	{
		M7816x_StopTOC(h7816m->Instance,h7816m->TimerStopMode);
		M7816x_ConfigTOR(h7816m->Instance,h7816m->TimerValue);
		M7816x_ConfigLCT(h7816m->Instance);
		M7816x_ConfigTOC(h7816m->Instance,h7816m->TimerStartMode);
		M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
			
		while(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TACT) == SET);
		while(M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_TBE) == RESET);
		M7816x_ConfigIODirection(h7816m->Instance,M7816x_RECEIVE_MODE);
		//h7816m->TxXferCount--;
		//h7816m->pTxBuffPtr++;
	}
	else
	{
		M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
	}
	h7816m->TxXferCount--;
	h7816m->pTxBuffPtr++;
	while(h7816m->TxXferCount)
	{
		while(M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_TBE) == RESET)
		{
			if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_PE) == SET)
			{
				M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_PE);
				h7816m->ErrorCode = TMC_M7816x_ERROR_PARITY;
				h7816m->State = TMC_M7816x_STATE_ERROR;
				return TMC_ERROR;
			}
		}
		if(h7816m->TxXferCount>1)
		{
			M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
			h7816m->TxXferCount--;
			h7816m->pTxBuffPtr++;
		}
		else
		{
			
			M7816x_StopTOC(h7816m->Instance,h7816m->TimerStopMode);
			M7816x_ConfigTOR(h7816m->Instance,h7816m->TimerValue);
			M7816x_ConfigLCT(h7816m->Instance);
			M7816x_ConfigTOC(h7816m->Instance,h7816m->TimerStartMode);
			M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
			
			while(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TACT) == SET);
			while(M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_TBE) == RESET);
			M7816x_ConfigIODirection(h7816m->Instance,M7816x_RECEIVE_MODE);
			h7816m->TxXferCount--;
			h7816m->pTxBuffPtr++;
		}
	}
	h7816m->State = TMC_M7816x_STATE_READY;
	
	return TMC_OK;
}
/**
  * @brief  Receive Data of ISO7816Mx with interrupt
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be received
*	@param[in]  TimeOut: the value of 7816Mx's Timer 
  * @param[in]  StopTOC: 7816Mx Timer Stop Mode
  *				@arg M7816x_TIM_STOP_TO1_TO2_TO3 :Stop TOR1+TOR2+TOR3
  *				@arg M7816x_TIM_STOP_TO2_TO3:Stop TOR2+TOR3,Not Stop TOR2+TOR3 
  * @param[in]  StartTOC: 7816Mx Timer Start Mode,value of @ref M7816_TOC_MODE
  * @retval TMC_StatusTypeDef status
  * @note : if want to start a new count of 7816Mx Timer,need to finish Timer(Stop Timer) 
  */
TMC_StatusTypeDef TMC_M7816x_Receive_IT(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size) 
{
	/* check point of hspi is NULL */
	if((h7816m == NULL)||(h7816m->State != TMC_M7816x_STATE_READY)||(pData == 0)||(Size == 0))
	{
		return TMC_ERROR;
	}

	h7816m->pRxBuffPtr = pData;
	h7816m->RxXferSize = Size;
	h7816m->RxXferCount = Size;
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	h7816m->M7816x_RxISR = TMC_M7816x_RxISR;	
	return TMC_OK;
}

/**
  * @brief  Transmit Datas of ISO7816Mx with interrupt
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @param[in]  pData: pointer to data buffer
  * @param[in]  Size: amount of data to be Transmited
  * @param[in]  StopTOC: 7816Mx Timer Stop Mode
  *				@arg M7816x_TIM_STOP_TO1_TO2_TO3 :Stop TOR1+TOR2+TOR3
  *				@arg M7816x_TIM_STOP_TO2_TO3:Stop TOR2+TOR3,Not Stop TOR2+TOR3 
  * @param[in]  StartTOC: 7816Mx Timer Start Mode,value of @ref M7816_TOC_MODE
  * @retval TMC_StatusTypeDef status
  * @note : if want to start a new count of 7816Mx Timer,need to finish Timer(Stop Timer) 
  */
TMC_StatusTypeDef TMC_M7816x_Transmit_IT(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size,uint8_t StopTOC,uint8_t StartTOC,uint32_t TimeOut) 
{
	/* check point of hspi is NULL */
	if(h7816m == NULL)
	{
		return TMC_ERROR;
	}
	if((h7816m->State != TMC_M7816x_STATE_READY)||(pData == 0)||(Size == 0)||(TimeOut == 0))
	{
		return TMC_ERROR;
	}
	
	h7816m->TxXferCount = Size;
	h7816m->TxXferSize = Size;
	h7816m->pTxBuffPtr = pData;
	h7816m->State = TMC_M7816x_STATE_TX_BUSY;
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	h7816m->M7816x_TxISR = TMC_M7816x_TxISR;
	h7816m->TimerStartMode = StartTOC;
	h7816m->TimerStopMode = StopTOC;
	h7816m->TimerValue = TimeOut;
	M7816x_ConfigIODirection(h7816m->Instance,M7816x_SEND_MODE);
	if(h7816m->TxXferCount == 1)
	{
		M7816x_StopTOC(h7816m->Instance,h7816m->TimerStopMode);
		M7816x_ConfigTOR(h7816m->Instance,h7816m->TimerValue);
		M7816x_ConfigLCT(h7816m->Instance);
		M7816x_ConfigTOC(h7816m->Instance,h7816m->TimerStartMode);
	}
	M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
	h7816m->TxXferCount--;
	h7816m->pTxBuffPtr++;
	M7816x_ITConfigMMSK(h7816m->Instance,M7816x_MSTS_Mask_TBE,ENABLE);
	M7816x_ITConfigMMSK(h7816m->Instance,M7816x_MSTS_Mask_RBF,DISABLE);
	M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_PE,ENABLE);
	return TMC_OK;
}
/**
  * @brief Set the 7816Mx's Interrupt based on value of TOC
  * @param[in]  h7816m: pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.  
  * @param[in] TOC: M7816x's Timer Mode 
  * @retval TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_M7816x_ConfigTimerIT(M7816x_HandleTypeDef* h7816m,uint8_t TOC)
{
	/* check point of hspi is NULL */
	if(h7816m == NULL)
	{
		return TMC_ERROR;
	}
	switch(TOC)
	{
		case M7816x_TIM_MODE2:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO1,ENABLE);break;							
		case M7816x_TIM_MODE3:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);break;
		case M7816x_TIM_MODE4:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO1,ENABLE);
							  M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);
							  break;
		case M7816x_TIM_MODE5:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);break;
		case M7816x_TIM_MODE6:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);break;
		case M7816x_TIM_MODE7:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);
							  M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO1,ENABLE);					  
							  break;
		case M7816x_TIM_MODE8:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);break;
		case M7816x_TIM_MODE9:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO1,ENABLE);break;	
		case M7816x_TIM_MODE10:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);
							   M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO1,ENABLE);
							   break;
		case M7816x_TIM_MODE11:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);break;
		case M7816x_TIM_MODE12:M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO3,ENABLE);
							   M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_TO1,ENABLE);
							   break;
		default: return TMC_ERROR;
	}
	return TMC_OK; 
}
/**
  * @brief  Handle M7816x interrupt request.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for the specified 7816Mx module.
  * @retval None
  */
void TMC_M7816x_IRQHandler(M7816x_HandleTypeDef *h7816m)
{
	if(h7816m->State == TMC_M7816x_STATE_RX_BUSY)
	{
		h7816m->M7816x_RxISR(h7816m);
	}
	if(h7816m->State == TMC_M7816x_STATE_TX_BUSY)
	{
		h7816m->M7816x_TxISR(h7816m);
	}
}
/**
  * @brief  Rx  handler for  Receive in Interrupt mode.
  * @param  h7816m pointer to a __M7816x_HandleTypeDef structure that contains
  *			   the configuration information for ISO 7816Mx module.
  * @retval None
  */
static void TMC_M7816x_RxISR(M7816x_HandleTypeDef *h7816m)
{
	if(M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_RBF) == SET)
	{
		*(h7816m->pRxBuffPtr) = M7816x_ReciveData(h7816m->Instance);
		h7816m->RxXferCount--;
		h7816m->pRxBuffPtr++;
		if(h7816m->RxXferCount == 0)
		{
			h7816m->State = TMC_M7816x_STATE_READY;
			h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
			M7816x_ITConfigMMSK(h7816m->Instance,M7816x_MSTS_Mask_RBF,DISABLE);
			TMC_M7816x_RxCpltCallback(h7816m);
		}
		return;
	}
	if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_EA) == SET)
	{
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_EA);
		h7816m->ErrorCode = TMC_M7816x_ERROR_EA;
		h7816m->State = TMC_M7816x_STATE_ERROR;
		TMC_M7816x_ErrorCallback(h7816m);
		return;
	}
	if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_PE) == SET)
	{
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_PE);
		h7816m->ErrorCode = TMC_M7816x_ERROR_PARITY;
		h7816m->State = TMC_M7816x_STATE_ERROR;
		TMC_M7816x_ErrorCallback(h7816m);
		return;
	}
	if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_OVR) == SET)
	{
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_OVR);
		h7816m->ErrorCode = TMC_M7816x_ERROR_OVERFLOW;
		h7816m->State = TMC_M7816x_STATE_ERROR;
		TMC_M7816x_ErrorCallback(h7816m);
		return;
	}
	if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_FER) == SET)
	{
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_FER);
		h7816m->ErrorCode = TMC_M7816x_ERROR_FER;
		h7816m->State = TMC_M7816x_STATE_ERROR;
		TMC_M7816x_ErrorCallback(h7816m);
		return;
	}
	if(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TO3|M7816x_USTS_TO3) == SET)
	{
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TO1|M7816x_USTS_TO3);
		h7816m->ErrorCode = TMC_M7816x_ERROR_TIME_OUT;
		h7816m->State = TMC_M7816x_STATE_TIMEOUT;
		TMC_M7816x_ErrorCallback(h7816m);
		return;
	}
	
}


/**
  * @brief  Tx  handler for  Transmit in Interrupt mode.
  * @param  h7816m pointer to a __M7816x_HandleTypeDef structure that contains
  *			   the configuration information for ISO 7816Mx module.
  * @retval None
  */
static void TMC_M7816x_TxISR(struct __M7816x_HandleTypeDef *h7816m)
{
	if((M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_PE) == SET)&&(M7816x_GetIT_UMSK(h7816m->Instance,M7816x_USTS_Mask_PE) == RESET))
	{
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_PE);
		h7816m->ErrorCode = TMC_M7816x_ERROR_PARITY;
		h7816m->State = TMC_M7816x_STATE_ERROR;
		M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_PE,DISABLE);
		TMC_M7816x_ErrorCallback(h7816m);
		return;
	}
	if((M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TACT) == SET)&&(M7816x_GetIT_UMSK(h7816m->Instance,M7816x_USTS_Mask_TACT) == RESET))
	{
	
		//h7816m->State = TMC_M7816x_STATE_READY;
		M7816x_ClearUStatus(h7816m->Instance,M7816x_USTS_TACT);
		M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_TACT,DISABLE);
		M7816x_ConfigIODirection(h7816m->Instance,M7816x_RECEIVE_MODE);
		M7816x_ITConfigMMSK(h7816m->Instance,M7816x_MSTS_Mask_RBF,ENABLE);
		M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_Mask_EA|M7816x_USTS_Mask_PE|M7816x_USTS_Mask_OVR|M7816x_USTS_Mask_FER,ENABLE);
		h7816m->State = TMC_M7816x_STATE_RX_BUSY;
		M7816x_StopTOC(h7816m->Instance,h7816m->TimerStopMode);
		TMC_M7816x_TxCpltCallback(h7816m);
		return;
	}
	if((M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_TBE) == SET)&&(M7816x_GetIT_MMSK(h7816m->Instance,M7816x_MSTS_Mask_TBE) == RESET))
	{
		if(h7816m->TxXferCount == 0)
		{
			M7816x_ITConfigMMSK(h7816m->Instance,M7816x_MSTS_TBE,DISABLE);
			M7816x_ITConfigUMSK(h7816m->Instance,M7816x_USTS_TACT,ENABLE);
		}
		if(h7816m->TxXferCount>1)
		{
			M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
			h7816m->TxXferCount--;
			h7816m->pTxBuffPtr++;
		}
		else
		{
			M7816x_StopTOC(h7816m->Instance,h7816m->TimerStopMode);
			M7816x_ConfigTOR(h7816m->Instance,h7816m->TimerValue);
			M7816x_ConfigLCT(h7816m->Instance);
			M7816x_ConfigTOC(h7816m->Instance,h7816m->TimerStartMode);
			M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);
			
			h7816m->TxXferCount--;
			h7816m->pTxBuffPtr++;
		}
		return;
	}
}

/**
  * @brief  Receive an amount of data in non-blocking mode with DMA.
  * @param  h7816m pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for 7816Mx module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval  TMC_StatusTypeDef
  */
TMC_StatusTypeDef TMC_M7816x_Receive_DMA(M7816x_HandleTypeDef *h7816m, uint8_t *pData, uint16_t Size)
{
	if((h7816m->State != TMC_M7816x_STATE_READY)||(pData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}

	/* Set the transaction information */
	h7816m->State	   = TMC_M7816x_STATE_RX_BUSY;
	h7816m->ErrorCode   = TMC_M7816x_ERROR_NONE;
	h7816m->pRxBuffPtr  = (uint8_t *)pData;
	h7816m->RxXferSize  = Size;
	h7816m->RxXferCount = Size;
	/*Init field not used in handle to zero */
	/* Set the SPI Rx DMA transfer complete callback */
	h7816m->hdmarx->XferCpltCallback = TMC_M7816x_DMAReceiveCplt;

	/* Set the DMA error callback */
	h7816m->hdmarx->XferErrorCallback = TMC_M7816x_DMAError;

	/* Set the DMA AbortCpltCallback */
	h7816m->hdmarx->XferAbortCallback = NULL;
	
	M7816x_ConfigIODirection(h7816m->Instance,M7816x_RECEIVE_MODE);
	
	/* Enable the Rx DMA Stream */
	TMC_DMA_Start_IT(h7816m->hdmarx, (uint32_t)&h7816m->Instance->RBUF, (uint32_t)h7816m->pRxBuffPtr, h7816m->RxXferSize);

	M7816x_ConfigDMA(h7816m->Instance,ENABLE);
	/* Check if the SPI is already enabled */
	M7816x_Cmd(h7816m->Instance,ENABLE);

	
	return TMC_OK;
}

/**
  * @brief  Transmit an amount of data in non-blocking mode with DMA.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for 7816Mx module.
  * @param  pData pointer to data buffer
  * @param  Size amount of data to be sent
  * @retval HAL status
  */
TMC_StatusTypeDef TMC_M7816x_Transmit_DMA(M7816x_HandleTypeDef *h7816m, uint8_t *pTxData, uint16_t Size)
{
	/* Process Locked */
	if((h7816m->State == TMC_M7816x_STATE_RX_BUSY)||(h7816m->State == TMC_M7816x_STATE_TX_BUSY))
	{
		return TMC_BUSY;
	}
	if((h7816m->State != TMC_M7816x_STATE_READY)||(pTxData == NULL)||(Size == 0))
	{
		return TMC_ERROR;
	}
	/* Set the transaction information */
	h7816m->State	   = TMC_M7816x_STATE_TX_BUSY;
	h7816m->ErrorCode   = TMC_M7816x_ERROR_NONE;
	h7816m->pTxBuffPtr  = (uint8_t *)pTxData;
	h7816m->TxXferSize  = Size-1; /* the Last byte need */
	h7816m->TxXferCount = Size;
	/* Set the 7816M Half transfer complete callback */
	h7816m->hdmatx->XferCpltCallback = TMC_M7816x_DMATransmitCplt;
	/* Set the DMA error callback */
	h7816m->hdmatx->XferErrorCallback = TMC_M7816x_DMAError;
	/* Set the DMA AbortCpltCallback */
	h7816m->hdmatx->XferAbortCallback = NULL;
	/* Enable the Tx DMA Stream */
	M7816x_ConfigDMA(h7816m->Instance,ENABLE);
	
	M7816x_ConfigIODirection(h7816m->Instance,M7816x_SEND_MODE);
	TMC_DMA_Start_IT(h7816m->hdmatx, (uint32_t)h7816m->pTxBuffPtr, (uint32_t)(&h7816m->Instance->TBUF), h7816m->TxXferSize);

	/* Check if the 7816M is already enabled */
//	M7816x_Cmd(h7816m->Instance,ENABLE);
	return TMC_OK;
}

TMC_StatusTypeDef TMC_M7816x_PPS_Config(M7816x_HandleTypeDef *h7816m, uint8_t BaudRate)
{
	uint8_t PPSTxbuf[4],PPSRxbuf[4];
	
	PPSTxbuf[0] = 0xFF;
	PPSTxbuf[1] = 0x10;
	PPSTxbuf[2] = BaudRate;
	PPSTxbuf[3] = ~(PPSTxbuf[1] ^ PPSTxbuf[2]);
	
	TMC_M7816x_Transmit(h7816m,PPSTxbuf,4,M7816x_TIM_STOP_TO2_TO3,M7816x_TIM_MODE4,0x276A00);
	M7816x_StopTOC(h7816m->Instance,M7816x_TIM_STOP_TO1_TO2_TO3);
	M7816x_ConfigTOR(h7816m->Instance,0xFCFA<<8);	
	M7816x_ConfigTOC(h7816m->Instance,M7816x_TIM_MODE4);
	TMC_M7816x_Receive(h7816m,PPSRxbuf,4);
	
	if(memcmp(PPSTxbuf, PPSRxbuf, 4) ==0x00)
	{
		if(BaudRate != 0x96)
		{
			M7816x_ConfigBaudRate(h7816m->Instance,BaudRate);
		}
		else
		{
			M7816x_ConfigBaudRate(h7816m->Instance,0x95);
			M7816x_SetDoubleBaudRate(h7816m->Instance,ENABLE);
		}
		return TMC_OK;
	}
	return TMC_ERROR;
	
	
}

/**
  * @brief  DMA SPI half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_M7816x_DMAReceiveCplt(DMA_HandleTypeDef *hdma)
{
	M7816x_HandleTypeDef* h7816m = ( M7816x_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	h7816m->State = TMC_M7816x_STATE_READY;
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	M7816x_Cmd(h7816m->Instance,DISABLE);
	TMC_M7816x_RxCpltCallback(h7816m);
}

/**
  * @brief  DMA SPI half receive process complete callback
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_M7816x_DMATransmitCplt(DMA_HandleTypeDef *hdma)
{
	M7816x_HandleTypeDef* h7816m = ( M7816x_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	h7816m->State = TMC_M7816x_STATE_READY;
	h7816m->ErrorCode = TMC_M7816x_ERROR_NONE;
	
	M7816x_StopTOC(h7816m->Instance,h7816m->TimerStopMode);
	M7816x_ConfigTOR(h7816m->Instance,h7816m->TimerValue);
	M7816x_ConfigLCT(h7816m->Instance);
	M7816x_ConfigTOC(h7816m->Instance,h7816m->TimerStartMode);

    h7816m->pTxBuffPtr += h7816m->TxXferSize;
	while(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TACT) == SET);
	M7816x_SendData(h7816m->Instance,*h7816m->pTxBuffPtr);/*DMA SRC is config auto increase */
    
	while(M7816x_GetUSTS(h7816m->Instance,M7816x_USTS_TACT) == SET);
	while(M7816x_GetMSTS(h7816m->Instance,M7816x_MSTS_TBE) == RESET);	

	M7816x_ConfigIODirection(h7816m->Instance,M7816x_RECEIVE_MODE);
	TMC_M7816x_TxCpltCallback(h7816m);
}
/**
  * @brief  DMA SPI communication error callback.
  * @param  hdma pointer to a DMA_HandleTypeDef structure that contains
  *			   the configuration information for the specified DMA module.
  * @retval None
  */
static void TMC_M7816x_DMAError(DMA_HandleTypeDef *hdma)
{
	M7816x_HandleTypeDef* h7816m = ( M7816x_HandleTypeDef* )((DMA_HandleTypeDef* )hdma)->Parent;
	h7816m->State = TMC_M7816x_STATE_ERROR;
	h7816m->ErrorCode = TMC_M7816x_ERROR_DMA;
	M7816x_Cmd(h7816m->Instance,DISABLE);
	TMC_M7816x_ErrorCallback(h7816m);
}
/**
  * @brief  Initialize the M7816x MSP.
  * @param  hspi pointer to a SPI_HandleTypeDef structure that contains
  *			   the configuration information for 7816Mx module.
  * @retval None
  */
__weak void TMC_M7816x_MspInit(M7816x_HandleTypeDef* h7816m)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(h7816m);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief  Initialize the M7816x MSP.
  * @param  hspi pointer to a M7816x_HandleTypeDef structure that contains
  *			   the configuration information for M7816x module.
  * @retval None
  */
__weak void TMC_M7816x_MspDeInit(M7816x_HandleTypeDef* h7816m)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(h7816m);
	/* NOTE : This function should not be modified, when the callback is needed,
			the HAL_SPI_MspInit should be implemented in the user file
	*/
}

/**
  * @brief TMC 7816Mx Error callback.
  * @param  h7816m pointer to a M7816x_HandleTypeDef structure that contains
  *			   the configuration information for ISO7816Mx module.
  * @retval None
  */
__weak void TMC_M7816x_ErrorCallback(M7816x_HandleTypeDef *h7816m)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(h7816m);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_ErrorCallback should be implemented in the user file
	*/
}

/**
  * @brief  TMC 7816Mx Rx Receive completed callback.
  * @param  h7816m pointer to a M7816x_HandleTypeDef structure that contains
  *			   the configuration information for ISO 7816Mx module.
  * @retval None
  */
__weak void TMC_M7816x_RxCpltCallback(M7816x_HandleTypeDef *h7816m)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(h7816m);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_RxCpltCallback should be implemented in the user file
	*/
}

/**
  * @brief  TMC 7816Mx Tx Transfer completed callback.
  * @param  h7816m pointer to a M7816x_HandleTypeDef structure that contains
  *			   the configuration information for ISO 7816Mx module.
  * @retval None
  */
__weak void TMC_M7816x_TxCpltCallback(M7816x_HandleTypeDef *h7816m)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(h7816m);
	/* NOTE : This function should not be modified, when the callback is needed,
			the TMC_M7816x_RxCpltCallback should be implemented in the user file
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


