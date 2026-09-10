/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: TMC_ISO7816Mx.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_ISO7816Mx_H
#define _TMC_ISO7816Mx_H

/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup ISO7816M
  * @{
  */ 
 
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "iso7816M.h"
#include "tmc_dma.h"
/* Exported types ------------------------------------------------------------*/
/** @defgroup ISO7816M_Exported_Types ISO7816M Exported Types
  * @{
  */ 
/** 
  * @brief  SPI State structures definition  
  */ 
typedef enum
{
	TMC_M7816x_STATE_RESET			= 0x00U,	/*!< Peripheral not Initialized							*/
	TMC_M7816x_STATE_READY			= 0x01U,	/*!< Peripheral Initialized and ready for use			*/
	TMC_M7816x_STATE_RX_BUSY		= 0x02U,	/*!< an internal process is ongoing						*/
	TMC_M7816x_STATE_TX_BUSY		= 0x03U,	/*!< an internal process is ongoing						*/
	TMC_M7816x_STATE_ERROR			= 0x04U,	/*!< M7816x error state									*/
	TMC_M7816x_STATE_TIMEOUT		= 0x05U		/*!< M7816x Time Out									*/
}TMC_M7816x_StateTypeDef;

/** 
  * @brief  SPI structures definition  
  */ 

typedef struct __M7816x_HandleTypeDef
{
	M7816_TypeDef				*Instance;													/* M7816x registers base address */
	M7816x_InitTypeDef			*Init;		 												/* M7816x communication parameters */
	uint8_t						*pTxBuffPtr;  												/* Pointer to SPI Tx transfer Buffer */
	uint16_t					TxXferSize;   												/* M7816x Tx Transfer size */
	uint16_t					TxXferCount;  												/* M7816x Tx Transfer Counter */
	uint8_t						*pRxBuffPtr;  												/* Pointer to M7816x Rx transfer Buffer */
	uint16_t					RxXferSize;   												/* M7816x Rx Transfer size */
	uint16_t					RxXferCount;  												/* M7816x Rx Transfer Counter */
	void						(*M7816x_RxISR)(struct __M7816x_HandleTypeDef * h7816m); 	/* function pointer on Rx ISR */
	void						(*M7816x_TxISR)(struct __M7816x_HandleTypeDef * h7816m); 	/* function pointer on Rx ISR */
	DMA_HandleTypeDef			*hdmatx;	  												/* M7816x Tx DMA Handle parameters   */
	DMA_HandleTypeDef			*hdmarx;	  												/* M7816x Rx DMA Handle parameters  */
	HAL_LockTypeDef		 	Lock;		 												/* Locking object*/ 
	volatile TMC_M7816x_StateTypeDef	 State;														/* M7816x state */
	uint32_t					ErrorCode;													/* M7816x Error code */
	uint8_t						TimerStopMode;												/* M7816x's Timer Stop Mode */
	uint8_t						TimerStartMode;												/* M7816x's Timer Start Mode */
	uint32_t					TimerValue;													/* M7816x's Timer Value */
}M7816x_HandleTypeDef;

/**
  * @}
  */	 
/** @defgroup M7816x_Error_Code M7816x Error Code
  * @{
  */
#define TMC_M7816x_ERROR_NONE				0x00000000U   /*!< No error			 */
#define TMC_M7816x_ERROR_PARAMER			0x00000007U   /*!<			  */
#define TMC_M7816x_ERROR_DMA				0x00000001U   /*!< DMA transfer error   */
#define TMC_M7816x_ERROR_SHORT				0x00000002U   /*!< 接口短路  */
#define TMC_M7816x_ERROR_EA					0x00000004U   /*!< RST拉高后起始位发送过早错误 */
#define TMC_M7816x_ERROR_PARITY				0x00000005U   /*!< 奇偶校验错误 */
#define TMC_M7816x_ERROR_OVERFLOW			0x00000003U   /*!< 奇偶校验错误 */
#define TMC_M7816x_ERROR_FER				0x00000006U   /*!< 帧错误 */
#define TMC_M7816x_ERROR_TIME_OUT			0x00000008U
/**
  * @}
  */
/** @addtogroup TMC_M7816x_Exported_Function 
  * @{
  */
void TMC_M7816x_StructInit(M7816x_HandleTypeDef* h7816m);
TMC_StatusTypeDef TMC_M7816x_Init(M7816x_HandleTypeDef* h7816m);
TMC_StatusTypeDef TMC_M7816x_DeInit(M7816x_HandleTypeDef* h7816m);
TMC_StatusTypeDef TMC_M7816x_Receive(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size);
TMC_StatusTypeDef TMC_M7816x_Transmit(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size,uint8_t StopTOC,uint8_t StartTOC,uint32_t TimeOut);
TMC_StatusTypeDef TMC_M7816x_Receive_IT(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size);
TMC_StatusTypeDef TMC_M7816x_Transmit_IT(M7816x_HandleTypeDef* h7816m,uint8_t * pData,uint16_t Size,uint8_t StopTOC,uint8_t StartTOC,uint32_t TimeOut);
TMC_StatusTypeDef TMC_M7816x_ConfigTimerIT(M7816x_HandleTypeDef* h7816m,uint8_t TOC);
void TMC_M7816x_IRQHandler(M7816x_HandleTypeDef *h7816m);
TMC_StatusTypeDef TMC_M7816x_Receive_DMA(M7816x_HandleTypeDef *h7816m, uint8_t *pData, uint16_t Size);
TMC_StatusTypeDef TMC_M7816x_Transmit_DMA(M7816x_HandleTypeDef *h7816m, uint8_t *pTxData, uint16_t Size);
void TMC_M7816x_MspInit(M7816x_HandleTypeDef* h7816m);
void TMC_M7816x_MspDeInit(M7816x_HandleTypeDef* h7816m);
void TMC_M7816x_ErrorCallback(M7816x_HandleTypeDef *h7816m);
void TMC_M7816x_RxCpltCallback(M7816x_HandleTypeDef *h7816m);
void TMC_M7816x_TxCpltCallback(M7816x_HandleTypeDef *h7816m);
TMC_StatusTypeDef TMC_M7816x_PPS_Config(M7816x_HandleTypeDef *h7816m, uint8_t BaudRate);

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

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

