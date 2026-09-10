/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_uart5.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TMC_UART5_H
#define __TMC_UART5_H
#include "thm36_1.3.h"
#include "TMC_RCC.h"
#include "RCC.h"
#include "UART.h"
#include "DMA.h"
#include "stdint.h"
#include "TMC_DMA.h"
#include "TMC_UART.h"
#include "GPIO.h"
#include "uart5.h"
/** @addtogroup TMC_Driver
  * @{
  */
/** @defgroup TMC_UART
  * @{
  */ 
typedef struct
{
	UART5_TypeDef					*Instance;				/*!< UART registers base address		*/
	
	UART5_InitTypeDef				*Init;				/*!< UART communication parameters		*/
	
	HAL_LockTypeDef					Lock;				/*!< UART locking object					*/

	uint8_t							*pTxBuffPtr;		/*!< Pointer to UART Tx transfer Buffer */

	uint32_t						TxXferSize;			/*!< UART Tx Transfer size				*/

	__IO uint32_t					TxXferCount;		/*!< UART Tx Transfer Counter			*/

	uint8_t							*pRxBuffPtr;		/*!< Pointer to UART Rx transfer Buffer	*/

	uint32_t						RxXferSize;			/*!< UART Rx Transfer size				*/

	__IO uint32_t					RxXferCount;		/*!< UART Rx Transfer Counter			*/ 
	
	uint16_t						Mask;			 /*!< UART Rx RDR register mask		  */

	DMA_HandleTypeDef				*hdmatx;			/*!< UART Tx DMA Handle parameters		*/

	DMA_HandleTypeDef				*hdmarx;			/*!< UART Rx DMA Handle parameters		*/
	
	__IO UART_StateTypeDef			TxState;		   	/*!< UART state information related to global Handle management 
															and also related to Tx operations.
															This parameter can be a value of @ref UART_StateTypeDef */
  
	__IO UART_StateTypeDef			RxState;			/*!< UART state information related to Rx operations.
															This parameter can be a value of @ref UART_StateTypeDef */
 
	__IO uint32_t					ErrorCode;			/*!< UART Error code
															This parameter can be a value of @ref UART_error_status	*/

} UART5_HandleTypeDef;

typedef struct
{
    uint32_t source_addr;
    uint32_t dest_addr;
    uint32_t lli_addr;
    uint32_t ctrl;
}LLI_StructTypeDef;

extern uint32_t tx_fifo_threshold;
extern uint32_t rx_fifo_threshold;
/**
  * @}
  */
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup UART_Exported_Functions
  * @{
  */ 
TMC_StatusTypeDef TMC_UART5_Init(UART5_HandleTypeDef *huart);
TMC_StatusTypeDef TMC_UART5_DeInit(UART5_HandleTypeDef *huart);
TMC_StatusTypeDef TMC_UART5_Transmit(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_UART5_Receive(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_UART5_Transmit_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size);
TMC_StatusTypeDef TMC_UART5_Receive_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size);
TMC_StatusTypeDef TMC_UART5_Transmit_DMA_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_UART5_Receive_DMA_IT(UART5_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI);

TMC_StatusTypeDef TMC_UART5_DMAStop(UART5_HandleTypeDef *huart);

void TMC_UART5_IRQHandler(UART5_HandleTypeDef *huart);
void TMC_UART5_MspInit(UART5_HandleTypeDef *huart);	
void TMC_UART5_MspDeInit(UART5_HandleTypeDef *huart);
void TMC_UART5_TxCpltCallback(UART5_HandleTypeDef *huart);
void TMC_UART5_RxCpltCallback(UART5_HandleTypeDef *huart);
void TMC_UART5_ErrorCallback(UART5_HandleTypeDef *huart);
void TMC_UART5_StructInit(UART5_HandleTypeDef *huart);
void TMC_UART5_Mask_Computation(UART5_HandleTypeDef *huart);
int8_t TMC_Check_Uart5_RxFIFO_Threshold(uint32_t rx_fifo_threshold);
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

#endif

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

