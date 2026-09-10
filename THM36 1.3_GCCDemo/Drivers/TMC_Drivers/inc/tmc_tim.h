/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_tim.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMC_TIM_H
#define __TMC_TIM_H

#ifdef __cplusplus
 extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "tim.h"
	 
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_TIM
  * @brief TIM TMC modules driver 
  * @{
  */
/** @defgroup TMC_TIM_exported_Types TIM exported types
  * @{
  */
/** 
  * @brief  TIM State structures definition  
  */ 
typedef enum
{
	TIM_STATE_RESET				= 0x00U,	/*!< Peripheral not yet initialized or disabled  */
	TIM_STATE_READY				= 0x01U,	/*!< Peripheral Initialized and ready for use	*/
	TIM_STATE_BUSY				= 0x02U,	/*!< An internal process is ongoing			  */
	TIM_STATE_OVER_FLOW			= 0x03U,	/*!< Timeout state							   */
}TIM_StateTypeDef;

/** 
  * @brief  TIM Time Base Handle Structure definition  
  */ 
typedef struct
{
	TIM_TypeDef *Instance;			/*!< Register base address			  */
	TIM_TimeInitTypeDef *Init;		/*!< TIM Time Base required parameters  */
	__IO TIM_StateTypeDef State;	/*!< TIM operation state				*/
}TIM_HandleTypeDef;	 
/**
  * @}
  */
/** @addtogroup TMC_TIM_Exported_Function
  * @{
  */
TMC_StatusTypeDef TMC_TIM_Init(TIM_HandleTypeDef *htim);
TMC_StatusTypeDef TMC_TIM_DeInit(TIM_HandleTypeDef *htim);
void TMC_TIM_MspInit(TIM_HandleTypeDef *htim);
void TMC_TIM_MspDeInit(TIM_HandleTypeDef *htim);
void TMC_TIM_StructInit(TIM_HandleTypeDef *htim);
TMC_StatusTypeDef TMC_TIM_Start(TIM_HandleTypeDef *htim,uint32_t ReloadValue);
TMC_StatusTypeDef TMC_TIM_Stop(TIM_HandleTypeDef *htim);
TMC_StatusTypeDef TMC_TIM_StartIT(TIM_HandleTypeDef *htim,uint32_t ReloadValue);
TMC_StatusTypeDef TMC_TIM_StopIT(TIM_HandleTypeDef *htim);
void TMC_TIM_IRQHandler(TIM_HandleTypeDef *htim);
void TMC_TIM_Callback(TIM_HandleTypeDef *htim);
TIM_StateTypeDef TMC_TIM_GetState(TIM_HandleTypeDef *htim);

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

#endif /* __thm36_13_TMC_TIM_H */
