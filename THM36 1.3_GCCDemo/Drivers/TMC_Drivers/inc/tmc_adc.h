/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_adc.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_ADC_H
#define _TMC_ADC_H

/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_ADC
  * @{
  */ 
 
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "adc.h"
#include "tmc_dma.h"
#include "stdint.h"
/* Exported types ------------------------------------------------------------*/
/** @defgroup TMC_ADC_Exported_Types TMC ADC Exported Types
  * @{
  */ 
/** 
  * @brief  ADC State structures definition  
  */ 
typedef enum
{
	TMC_ADC_STATE_RESET					= 0x00U,	/*!< Peripheral not Initialized							*/
	TMC_ADC_STATE_READY					= 0x01U,	/*!< Peripheral Initialized and ready for use			*/
	TMC_ADC_STATE_BUSY					= 0x02U,	/*!< an internal process is ongoing						*/
	TMC_ADC_STATE_ERROR					= 0x03U,	/*!< ADC error state									*/
	TMC_ADC_STATE_TIMEOUT				= 0x04U
}TMC_ADC_StateTypeDef;	 


/** 
  * @brief  ADC structures definition  
  */ 
typedef struct __ADC_HandleTypeDef
{
	ADC_InitTypeDef				*Init; 
	uint16_t					*pBuffPtr;  												
	uint32_t					XferSize;   											
	uint32_t					XferCount; 
	void						(*ADC_ISR)(struct __ADC_HandleTypeDef * hadc); 
	ADC_AMP_InitTypeDef			*AMP_Init;
	DMA_HandleTypeDef			*hdma0;	
	DMA_HandleTypeDef			*hdma1; 
	DMA_HandleTypeDef			*hdma2; 	
	HAL_LockTypeDef		 	Lock;		 											
	TMC_ADC_StateTypeDef	 	State;													
	uint32_t					ErrorCode;																								
}ADC_HandleTypeDef;

/** @defgroup ADC_Error_Code ADC Error Code
  * @{
  */
#define TMC_ADC_ERROR_NONE				0x00000000U   /*!< No error			 */
#define TMC_ADC_ERROR_OVER_FLOW			0x00000001U   /*!<			  */

/**
  * @}
  */


/**
  * @}
  */
/** @addtogroup TMC_ADC_Exported_Function 
  * @{
  */

void TMC_ADC_StructInit(ADC_HandleTypeDef* hadc);
TMC_StatusTypeDef TMC_ADC_Init(ADC_HandleTypeDef* hadc);
TMC_StatusTypeDef TMC_ADC_DeInit(ADC_HandleTypeDef* hadc);
TMC_StatusTypeDef TMC_ADC_Start(ADC_HandleTypeDef* hadc,uint16_t * pData,uint32_t Size,uint32_t Timeout);
TMC_StatusTypeDef TMC_ADC_SingleChannelStart(ADC_HandleTypeDef* hadc, uint16_t * pData, uint32_t Size, ADC_ConvertingChannelx ChannelSelected, uint32_t Timeout);
TMC_StatusTypeDef TMC_ADC_Stop(ADC_HandleTypeDef* hadc);
TMC_StatusTypeDef TMC_ADC_Start_IT(ADC_HandleTypeDef* hadc,uint16_t * pData,uint32_t Size);
TMC_StatusTypeDef TMC_ADC_Stop_IT(ADC_HandleTypeDef* hadc);
void TMC_ADC_IRQHandler(ADC_HandleTypeDef *hadc);
TMC_StatusTypeDef TMC_ADC_Start_HDMA0_REQUEST_ADC1_IT(ADC_HandleTypeDef *hadc,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_ADC_Stop_HDMA0_REQUEST_ADC1_IT(ADC_HandleTypeDef *hadc);
TMC_StatusTypeDef TMC_ADC_Start_HDMA1_REQUEST_ADC2_IT(ADC_HandleTypeDef *hadc,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_ADC_Stop_HDMA1_REQUEST_ADC2_IT(ADC_HandleTypeDef *hadc);
TMC_StatusTypeDef TMC_ADC_Start_HDMA2_REQUEST_ADC3_IT(ADC_HandleTypeDef *hadc,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_ADC_Stop_HDMA2_REQUEST_ADC3_IT(ADC_HandleTypeDef *hadc);
void TMC_ADC_MspInit(ADC_HandleTypeDef* hadc);
void TMC_ADC_MspDeInit(ADC_HandleTypeDef* hadc);
void TMC_ADCx_RxCpltCallback(ADC_HandleTypeDef *hadc);
void TMC_ADCx_ErrorCallback(ADC_HandleTypeDef *hadc);
void TMC_ADC_HDMA0_RxCpltCallback(ADC_HandleTypeDef *hadc);
void TMC_ADC_HDMA0_ErrorCallback(ADC_HandleTypeDef *hadc);
void TMC_ADC_HDMA1_RxCpltCallback(ADC_HandleTypeDef *hadc);
void TMC_ADC_HDMA1_ErrorCallback(ADC_HandleTypeDef *hadc);
void TMC_ADC_HDMA2_RxCpltCallback(ADC_HandleTypeDef *hadc);
void TMC_ADC_HDMA2_ErrorCallback(ADC_HandleTypeDef *hadc);
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

