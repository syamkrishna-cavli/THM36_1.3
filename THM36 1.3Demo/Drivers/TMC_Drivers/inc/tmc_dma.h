/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_dma.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_DMA_H
#define _TMC_DMA_H



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "DMA.h"
	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_DMA
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup TMC_DMA_Exported_Types TMC DMA Exported Types
  * @{
  */ 

/** 
  * @brief  TMC DMA State structures definition
  */
typedef enum
{
	TMC_DMA_STATE_RESET				= 0x00U,  /*!< DMA not yet initialized or disabled */
	TMC_DMA_STATE_READY				= 0x01U,  /*!< DMA initialized and ready for use   */
	TMC_DMA_STATE_BUSY				= 0x02U,  /*!< DMA process is ongoing			  */
	TMC_DMA_STATE_TIMEOUT			= 0x03U,  /*!< DMA timeout state				   */
	TMC_DMA_STATE_ERROR				= 0x04U,  /*!< DMA error state					 */
	TMC_DMA_STATE_ABORT				= 0x05U,  /*!< DMA Abort state					 */
}TMC_DMA_StateTypeDef;

/** 
  * @brief  HAL DMA Error Code structure definition
  */
typedef enum
{
	HAL_DMA_XFER_CPLT_CB_ID			= 0x00U,  /*!< Full transfer	 */
	HAL_DMA_XFER_ERROR_CB_ID		= 0x04U,  /*!< Error			 */
	HAL_DMA_XFER_ABORT_CB_ID		= 0x05U,  /*!< Abort			 */
	HAL_DMA_XFER_ALL_CB_ID			= 0x06U   /*!< All			   */
}TMC_DMA_CallbackIDTypeDef;

/** 
  * @brief  DMA handle Structure definition
  */
typedef struct __DMA_HandleTypeDef
{
	DMA_Channel_TypeDef			*Instance;															/*!< Register base address*/

	DMA_InitTypeDef				Init;																/*!< DMA communication parameters*/ 

	HAL_LockTypeDef				Lock;																/*!< DMA locking object*/  

	__IO TMC_DMA_StateTypeDef	State;																/*!< DMA transfer state*/

	void						*Parent;															/*!< Parent object state*/

	void						(* XferCpltCallback)( struct __DMA_HandleTypeDef * hdma);			/*!< DMA transfer complete callback*/

	void						(* XferErrorCallback)( struct __DMA_HandleTypeDef * hdma);			/*!< DMA transfer error callback*/
	
	void						(* XferAbortCallback)( struct __DMA_HandleTypeDef * hdma);			/*!< DMA transfer Abort callback*/  

	__IO uint32_t				ErrorCode;															/*!< DMA Error code*/

	uint32_t					StreamIndex;														/*!< DMA Stream Index*/
	
	uint32_t					LLI;																/*!< DMA LLI ADDRESS*/
 
}DMA_HandleTypeDef;

/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup TMC_DMA_Exported_Constants TMC DMA Exported Constants
  * @{
  */

/** @defgroup TMC_DMA_Error_Code TMC DMA Error Code
  * @{
  */ 
#define TMC_DMA_ERROR_NONE				0x00000000U		/*!< No error							   */
#define TMC_DMA_ERROR_TE				0x00000001U		/*!< Transfer error						 */
#define TMC_DMA_ERROR_TIMEOUT			0x00000020U		/*!< Timeout error						  */
#define TMC_DMA_ERROR_PARAM				0x00000040U		/*!< Parameter error						*/
#define TMC_DMA_ERROR_NO_XFER			0x00000080U		/*!< Abort requested with no Xfer ongoing   */
#define TMC_DMA_ERROR_NOT_SUPPORTED		0x00000100U		/*!< Not supported mode					 */
/**
  * @}
  */
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup TMC_DMA_Exported_Functions
  * @{
  */ 
TMC_StatusTypeDef TMC_DMA_Init(DMA_HandleTypeDef *hdma);
TMC_StatusTypeDef TMC_DMA_DeInit(DMA_HandleTypeDef *hdma);
TMC_StatusTypeDef TMC_DMA_Start(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
TMC_StatusTypeDef TMC_DMA_Start_IT(DMA_HandleTypeDef *hdma, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength);
TMC_StatusTypeDef TMC_DMA_Abort(DMA_HandleTypeDef *hdma);
TMC_StatusTypeDef TMC_DMA_Abort_IT(DMA_HandleTypeDef *hdma);
TMC_StatusTypeDef TMC_DMA_PollForTransfer(DMA_HandleTypeDef *hdma, uint32_t Timeout);
TMC_DMA_StateTypeDef TMC_DMA_GetState(DMA_HandleTypeDef *hdma);
uint32_t TMC_DMA_GetError(DMA_HandleTypeDef *hdma);
void TMC_DMA_IRQHandler(DMA_HandleTypeDef *hdma);		
TMC_StatusTypeDef TMC_DMA_RegisterCallback(DMA_HandleTypeDef *hdma, TMC_DMA_CallbackIDTypeDef CallbackID, void (* pCallback)(DMA_HandleTypeDef *_hdma));			 
TMC_StatusTypeDef TMC_DMA_UnRegisterCallback(DMA_HandleTypeDef *hdma, TMC_DMA_CallbackIDTypeDef CallbackID);  
/**
  * @}
  */ 
/* Private macros ------------------------------------------------------------*/
/** @defgroup TMC_DMA_Private_Macros TMC DMA Private Macros
  * @{
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

#endif 

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
