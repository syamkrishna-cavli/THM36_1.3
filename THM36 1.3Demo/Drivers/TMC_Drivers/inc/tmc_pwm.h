/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_pwm.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_PWM_H
#define _TMC_PWM_H

/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_PWM
  * @{
  */ 
 
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "PWM.h"
/** @defgroup TMC_ADC_Exported_Types TMC ADC Exported Types
  * @{
  */ 
typedef enum
{
	TMC_PWM_STATE_RESET					= 0x00U,	/*!< Peripheral not Initialized							*/
	TMC_PWM_STATE_READY					= 0x01U,	/*!< Peripheral Initialized and ready for use			*/
	TMC_PWM_STATE_BUSY					= 0x02U,	/*!< an internal process is ongoing						*/
}TMC_PWM_StateTypeDef;

/** 
  * @brief  PWM structures definition  
  */ 
typedef struct __PWM_HandleTypeDef
{
	PWM_InitTypeDef	*Init; 
	PWM_TypeDef		*Instance;
	uint32_t 		XferSize; 
	uint32_t 		XferCount;
	HAL_LockTypeDef  Lock; 
	volatile TMC_PWM_StateTypeDef State;	  																																	
}PWM_HandleTypeDef;

/**
  * @}
  */
/** @addtogroup TMC_PWM_Exported_Function TMC PWM Exported Function
  * @{ 
  */
void TMC_PWM_StructInit(PWM_HandleTypeDef* hpwm);
TMC_StatusTypeDef TMC_PWM_Init(PWM_HandleTypeDef* hpwm);
TMC_StatusTypeDef TMC_PWM_DeInit(PWM_HandleTypeDef* hpwm);
TMC_StatusTypeDef TMC_PWM_StartOutputWaveforms(PWM_HandleTypeDef* hpwm);
TMC_StatusTypeDef TMC_PWM_StopOutputWaveforms(PWM_HandleTypeDef* hpwm);
TMC_StatusTypeDef TMC_PWM_OutputWaveforms(PWM_HandleTypeDef* hpwm,uint32_t Size,uint32_t Timeout);
TMC_StatusTypeDef TMC_PWM_OutputWaveforms_IT(PWM_HandleTypeDef* hpwm,uint32_t Size);
void TMC_PWM_IRQHandler(PWM_HandleTypeDef* hpwm);
void TMC_PWM_CpltCallback(PWM_HandleTypeDef* hpwm);
void TMC_PWM_MspInit(PWM_HandleTypeDef* hpwm);
void TMC_PWM_MspDeInit(PWM_HandleTypeDef* hpwm);
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

