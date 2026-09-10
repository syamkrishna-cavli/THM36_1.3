/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_mag.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_MAG_H
#define _TMC_MAG_H

/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_MAG
  * @{
  */ 
 
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "mag.h"
/** @defgroup TMC_ADC_Exported_Types TMC ADC Exported Types
  * @{
  */ 
/** 
  * @brief  ADC State structures definition  
  */ 
typedef enum
{
	TMC_MAG_STATE_RESET					= 0x00U,	/*!< Peripheral not Initialized							*/
	TMC_MAG_STATE_READY					= 0x01U,	/*!< Peripheral Initialized and ready for use			*/
	TMC_MAG_STATE_BUSY					= 0x02U,	/*!< an internal process is ongoing						*/
	TMC_MAG_STATE_ERROR					= 0x03U,	/*!< ADC error state									*/
	TMC_MAG_STATE_TIMEOUT				= 0x04U,
	TMC_MAG_
}TMC_MAG_StateTypeDef;	 


/** 
  * @brief  ADC structures definition  
  */ 
typedef struct __MAG_HandleTypeDef
{
	uint16_t					*pBuffPtr;  												
	uint32_t					XferSize;   											
	uint32_t					XferCount; 
	DMA_HandleTypeDef			*hdma0;	
	DMA_HandleTypeDef			*hdma1; 
	DMA_HandleTypeDef			*hdma2; 		   											
	TMC_MAG_StateTypeDef	 	State;													
	//uint32_t					ErrorCode;																								
}Mag_HandleTypeDef;	 

/**
  * @}
  */
/** @addtogroup TMC_ADC_Exported_Function 
  * @{
  */
TMC_StatusTypeDef TMC_Mag_Start_HDMA0_REQUEST_ADC1_IT(Mag_HandleTypeDef *hmag,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_Mag_Start_HDMA1_REQUEST_ADC2_IT(Mag_HandleTypeDef *hmag,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI);
TMC_StatusTypeDef TMC_Mag_Start_HDMA2_REQUEST_ADC3_IT(Mag_HandleTypeDef *hmag,uint32_t ADC_Channelx,uint16_t *pData, uint16_t Size,uint32_t LLI);
void TMC_Mag_HDMA0_RxCpltCallback(Mag_HandleTypeDef *hmag);
void TMC_Mag_HDMA0_ErrorCallback(Mag_HandleTypeDef *hmag);
void TMC_Mag_HDMA1_RxCpltCallback(Mag_HandleTypeDef *hmag);
void TMC_Mag_HDMA1_ErrorCallback(Mag_HandleTypeDef *hmag);
void TMC_Mag_HDMA2_RxCpltCallback(Mag_HandleTypeDef *hmag);
void TMC_Mag_HDMA2_ErrorCallback(Mag_HandleTypeDef *hmag);
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




