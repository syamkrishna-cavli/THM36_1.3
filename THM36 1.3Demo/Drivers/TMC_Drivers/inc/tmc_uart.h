/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: TMC_UART.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_UART_H
#define _TMC_UART_H



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "UART.h"
#include "TMC_DMA.h"	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup UART
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup UART_Exported_Types UART Exported Types
  * @{
  */ 
 

/** 
  * @brief	UART handle structure definition  
  */
typedef enum
{
	TMC_UART_STATE_RESET			= 0x00U,			/*!< Peripheral is not yet Initialized 
															Value is allowed for gState and RxState */
	TMC_UART_STATE_READY			= 0x01U,			/*!< Peripheral Initialized and ready for use
															Value is allowed for gState and RxState */
	TMC_UART_STATE_BUSY				= 0x02U,			/*!< an internal process is ongoing
															Value is allowed for gState only */
	TMC_UART_STATE_TIMEOUT			= 0x04U,			/*!< Timeout state	
															Value is allowed for gState only */
	TMC_UART_STATE_ERROR			= 0x05U				/*!< Error   
															Value is allowed for gState only */
} UART_StateTypeDef;

typedef struct
{
	UART_TypeDef					*Instance;				/*!< UART registers base address		*/
	
	UART_InitTypeDef				*Init;				/*!< UART communication parameters		*/
	
	HAL_LockTypeDef					Lock;				/*!< UART locking object					*/

	uint8_t							*pTxBuffPtr;		/*!< Pointer to UART Tx transfer Buffer */

	uint32_t						TxXferSize;			/*!< UART Tx Transfer size				*/

	__IO uint32_t					TxXferCount;		/*!< UART Tx Transfer Counter			*/

	uint8_t							*pRxBuffPtr;		/*!< Pointer to UART Rx transfer Buffer	*/

	uint32_t						RxXferSize;			/*!< UART Rx Transfer size				*/

	__IO uint32_t					RxXferCount;		/*!< UART Rx Transfer Counter			*/  

	DMA_HandleTypeDef				*hdmatx;			/*!< UART Tx DMA Handle parameters		*/

	DMA_HandleTypeDef				*hdmarx;			/*!< UART Rx DMA Handle parameters		*/
	
	__IO UART_StateTypeDef			TxState;		   	/*!< UART state information related to global Handle management 
															and also related to Tx operations.
															This parameter can be a value of @ref UART_StateTypeDef */
  
	__IO UART_StateTypeDef			RxState;			/*!< UART state information related to Rx operations.
															This parameter can be a value of @ref UART_StateTypeDef */
 
	__IO uint32_t					ErrorCode;			/*!< UART Error code
															This parameter can be a value of @ref UART_error_status	*/

} UART_HandleTypeDef;



/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup UART_Exported_Constants UART Exported Constants
  * @{
  */
/** @defgroup UART_error_status UART error status
  * @{
  */
#define TMC_UART_ERROR_NONE		 0x00000000U   /*!< No error			*/
#define TMC_UART_ERROR_PE		   0x00000001U   /*!< Parity error		*/
#define TMC_UART_ERROR_FE		   0x00000002U   /*!< Frame error		 */
#define TMC_UART_ERROR_OFE		  0x00000004U   /*!< Overflow error	   */
#define TMC_UART_ERROR_DMA		  0x00000008U   /*!< DMA transfer error  */

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
TMC_StatusTypeDef TMC_UART_Init(UART_HandleTypeDef *huart);
TMC_StatusTypeDef TMC_UART_DeInit(UART_HandleTypeDef *huart);
TMC_StatusTypeDef TMC_UART_Transmit(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_UART_Receive(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size, uint32_t Timeout);
TMC_StatusTypeDef TMC_UART_Transmit_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size);
TMC_StatusTypeDef TMC_UART_Receive_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size);
TMC_StatusTypeDef TMC_UART_Transmit_DMA_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_UART_Receive_DMA_IT(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size,uint32_t LLI);

TMC_StatusTypeDef TMC_UART_DMAStop(UART_HandleTypeDef *huart);

void TMC_UART_IRQHandler(UART_HandleTypeDef *huart);
void TMC_UART_MspInit(UART_HandleTypeDef *huart);	
void TMC_UART_MspDeInit(UART_HandleTypeDef *huart);
void TMC_UART_TxCpltCallback(UART_HandleTypeDef *huart);
void TMC_UART_RxCpltCallback(UART_HandleTypeDef *huart);
void TMC_UART_ErrorCallback(UART_HandleTypeDef *huart);
void TMC_UART_StructInit(UART_HandleTypeDef *huart);
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
