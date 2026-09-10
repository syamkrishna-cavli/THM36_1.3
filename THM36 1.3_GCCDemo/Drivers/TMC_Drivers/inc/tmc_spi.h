/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_spi.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef __thm36_13_TMC_SPI_H
#define __thm36_13_TMC_SPI_H

#ifdef __cplusplus
 extern "C" {
#endif
#include "tmc_dma.h"	 
#include "spi.h"
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

/** @defgroup TMC_SPI_exported_Types SPI exported types
  * @{
  */
	 
/** @defgroup SPI_Error_Code SPI Error Code
  * @{
  */
#define TMC_SPI_ERROR_NONE			  0x00000000U   /*!< No error			 */
#define TMC_SPI_ERROR_DMA			   0x00000010U   /*!< DMA transfer error   */
#define TMC_SPI_ERROR_FLAG			  0x00000020U   /*!< Flag: RXNE,TXE, BSY  */
/**
  * @}
  */


/** 
  * @brief  SPI State structures definition  
  */ 
typedef enum
{
  TMC_SPI_STATE_RESET	  = 0x00U,	/*!< Peripheral not Initialized						 */
  TMC_SPI_STATE_READY	  = 0x01U,	/*!< Peripheral Initialized and ready for use		   */
  TMC_SPI_STATE_BUSY	   = 0x02U,	/*!< an internal process is ongoing					 */
  TMC_SPI_STATE_BUSY_TX	= 0x03U,	/*!< Data Transmission process is ongoing			   */
  TMC_SPI_STATE_BUSY_RX	= 0x04U,	/*!< Data Reception process is ongoing				  */
  TMC_SPI_STATE_BUSY_TX_RX = 0x05U,	/*!< Data Transmission and Reception process is ongoing */
  TMC_SPI_STATE_ERROR	  = 0x06U	 /*!< SPI error state									*/
}TMC_SPI_StateTypeDef;

/** 
  * @brief  SPI structures definition  
  */ 

typedef struct __SPI_HandleTypeDef
{
	SPI_TypeDef				*Instance;	/* SPI registers base address */
	SPI_InitTypeDef			*Init;		 /* SPI communication parameters */
	uint8_t					*pTxBuffPtr;  /* Pointer to SPI Tx transfer Buffer */
	uint16_t				TxXferSize;   /* SPI Tx Transfer size */
	uint16_t				TxXferCount;  /* SPI Tx Transfer Counter */
	uint8_t					*pRxBuffPtr;  /* Pointer to SPI Rx transfer Buffer */
	uint16_t				RxXferSize;   /* SPI Rx Transfer size */
	uint16_t				RxXferCount;  /* SPI Rx Transfer Counter */
	void					(*RxISR)(struct __SPI_HandleTypeDef * hspi); /* function pointer on Rx ISR */
	void					(*TxISR)(struct __SPI_HandleTypeDef * hspi); /* function pointer on Tx ISR */
	void					(*RxTxISR)(struct __SPI_HandleTypeDef * hspi); /* function pointer on RxTx ISR */
	DMA_HandleTypeDef		*hdmatx;	  /* SPI Tx DMA Handle parameters   */
	DMA_HandleTypeDef		*hdmarx;	  /* SPI Rx DMA Handle parameters  */
	HAL_LockTypeDef		 Lock;		 /* Locking object*/ 
	volatile TMC_SPI_StateTypeDef	State;		/* SPI communication state */
	uint32_t				ErrorCode;	/* SPI Error code */
}SPI_HandleTypeDef;

/**
  * @}
  */
/** @defgroup TMC_SPI_transmit_mode TMC SPI transmit mode
  * @{
  */
#define SPI_ONLY_RECEIVE			0
#define SPI_ONLY_TRANSMIT			1
#define SPI_TRANSMIT_RECEIVE		2
/**
  * @}
  */
/** @addtogroup TMC_SPI_Exported_Function 
  * @{
  */
void TMC_SPI_StructInit(SPI_HandleTypeDef* hspi);
TMC_StatusTypeDef TMC_SPI_Init(SPI_HandleTypeDef* hspi);
TMC_StatusTypeDef TMC_SPI_DeInit(SPI_HandleTypeDef* hspi);

TMC_StatusTypeDef TMC_SPI_Receive(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_SPI_Transmit(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_SPI_TransmitReceive(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_SPI_Transmit_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData,uint16_t Size);
TMC_StatusTypeDef TMC_SPI_Receive_IT(SPI_HandleTypeDef *hspi, uint8_t *pRxData, uint16_t Size);
TMC_StatusTypeDef TMC_SPI_TransmitReceive_IT(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size);
TMC_StatusTypeDef TMC_SPI_Transmit_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint16_t Size);
TMC_StatusTypeDef TMC_SPI_Receive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pData, uint16_t Size);
TMC_StatusTypeDef TMC_SPI_TransmitReceive_DMA(SPI_HandleTypeDef *hspi, uint8_t *pTxData, uint8_t *pRxData, uint16_t Size,uint32_t Rx_LLI,uint32_t Tx_LLI);

void TMC_SPI_IRQHandler(SPI_HandleTypeDef *hspi);
TMC_SPI_StateTypeDef TMC_SPI_GetState(SPI_HandleTypeDef *hspi);
void TMC_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi);
void TMC_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void TMC_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi);
void TMC_SPI_ErrorCallback(SPI_HandleTypeDef *hspi);
void TMC_SPI_MspInit(SPI_HandleTypeDef *hspi);
void TMC_SPI_MspDeInit(SPI_HandleTypeDef *hspi);
void TMC_SPI_SSN_Enable(SPI_HandleTypeDef *hspi);
void TMC_SPI_SSN_Disable(SPI_HandleTypeDef *hspi);

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
	 
#ifdef __cplusplus
}
#endif

#endif /* __thm36_13_TMC_SPI_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

