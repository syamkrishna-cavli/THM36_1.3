/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: TMC_SPI_Controller.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef _TMC_SPI_CONTROLLER_H_
#define _TMC_SPI_CONTROLLER_H_

#ifdef __cplusplus
 extern "C" {
#endif  

#include "TMC_DMA.h"	 
#include "SPI_Controller.h"
  
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

/** @defgroup TMC_SPI_CONTROLLER_exported_Types SPI_CONTROLLER exported types
  * @{
  */

/** 
  * @brief HAL QSPI State structures definition  
  */ 
typedef enum
{
	TMC_SPI_CONTROLLER_STATE_RESET		   			= 0x00U,	/*!< Peripheral not initialized							*/
	TMC_SPI_CONTROLLER_STATE_READY						= 0x01U,	/*!< Peripheral initialized and ready for use			  */
	TMC_SPI_CONTROLLER_STATE_BUSY_COMMAND				= 0x02U,	/*!< Peripheral in indirect mode and busy				  */
	TMC_SPI_CONTROLLER_STATE_BUSY_TX  					= 0x12U,	/*!< Peripheral in indirect mode with transmission ongoing */
	TMC_SPI_CONTROLLER_STATE_BUSY_RX  					= 0x22U,	/*!< Peripheral in indirect mode with reception ongoing	*/
	TMC_SPI_CONTROLLER_STATE_ABORT			 			= 0x08U,	/*!< Peripheral with abort request ongoing				 */
	TMC_SPI_CONTROLLER_STATE_ERROR			 			= 0x04U	 /*!< Peripheral in error								   */
}SPI_CONTROLLER_StateTypeDef;	 

/** 
  * @brief  QSPI Handle Structure definition  
  */  
typedef struct
{
  SPI_Controller_InitTypeDef			*Init;				/* SPI_controller communication parameters	  */
  uint32_t								*pTxBuffPtr;	  	/* Pointer to SPI_controller Tx transfer Buffer */
  uint32_t			  				TxXferSize;	   	/* SPI_controller Tx Transfer size			  */
  uint32_t			  				TxXferCount;	  	/* SPI_controller Tx Transfer Counter		   */
  uint32_t								*pRxBuffPtr;	  	/* Pointer to SPI_controller Rx transfer Buffer */
  uint32_t			  				RxXferSize;	   	/* SPI_controller Rx Transfer size			  */
  uint32_t			  				RxXferCount;	  	/* SPI_controller Rx Transfer Counter		   */
  DMA_HandleTypeDef		  			*hdma;				/* SPI_controller Rx/Tx DMA Handle parameters   */				   
  SPI_CONTROLLER_StateTypeDef 			State;				/* SPI_controller communication state		   */
  uint32_t			  				ErrorCode;			/* SPI_controller Error code					*/

}SPI_Controller_HandleTypeDef;

/** 
  * @brief  QSPI Command structure definition  
  */
typedef struct
{
	uint32_t Flash_Addr;
	SPI_Controller_CmdW1Def *CmdW1;
	uint32_t Data_Length;
	SPI_Controller_CmdW3Def *CmdW3;
	uint32_t *Data;
	//uint8_t	 Rstatus;
}SPI_Controller_CommandTypeDef;

/** @defgroup QSPI_ErrorCode QSPI Error Code
  * @{
  */ 
#define TMC_SPI_CONTROLLER_ERROR_NONE			0x00000000U /*!< No error		   */
#define TMC_SPI_CONTROLLER_ERROR_TIMEOUT		 0x00000001U /*!< Timeout error	  */
#define TMC_SPI_CONTROLLER_ERROR_TRANSFER		0x00000002U /*!< Transfer error	 */
#define TMC_SPI_CONTROLLER_ERROR_DMA			 0x00000004U /*!< DMA transfer error */
#define TMC_SPI_CONTROLLER_ERROR_INVALID_PARAM   0x00000008U /*!< Invalid parameters error */
/**
  * @}
  */ 

/** @addtogroup TMC_SPI_Exported_Function 
  * @{
  */

void TMC_TMC_SPI_Controller_StructInit(SPI_Controller_HandleTypeDef *hqspi);

TMC_StatusTypeDef TMC_SPI_Controller_Init(SPI_Controller_HandleTypeDef *hqspi);
TMC_StatusTypeDef TMC_SPI_Controller_DeInit(SPI_Controller_HandleTypeDef *hqspi);
TMC_StatusTypeDef TMC_SPI_Controller_Command(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd);
TMC_StatusTypeDef TMC_SPI_Controller_Receive(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd, uint32_t* buf,uint32_t Timeout);
TMC_StatusTypeDef TMC_SPI_Controller_Transmit(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf,uint32_t Timeout);
TMC_StatusTypeDef TMC_SPI_Controller_Receive_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf);
TMC_StatusTypeDef TMC_SPI_Controller_Transmit_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf);
TMC_StatusTypeDef TMC_SPI_Controller_DMA_Receive_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf);
TMC_StatusTypeDef TMC_SPI_Controller_DMA_Transmit_IT(SPI_Controller_HandleTypeDef *hqspi, SPI_Controller_CommandTypeDef *cmd,uint32_t* buf);

void TMC_SPI_Controller_MspInit(SPI_Controller_HandleTypeDef *hspi);
void TMC_SPI_Controller_MspDeInit(SPI_Controller_HandleTypeDef *hspi);
void TMC_SPI_Controller_RxCpltCallback(SPI_Controller_HandleTypeDef *hspi);
void TMC_SPI_Controller_TxCpltCallback(SPI_Controller_HandleTypeDef *hspi);
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

#endif /* _TMC_SPI_CONTROLLER_H_ */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/  
  
  
 


