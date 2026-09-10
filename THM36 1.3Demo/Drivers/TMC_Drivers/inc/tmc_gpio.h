/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_gpio.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_GPIO_H
#define _TMC_GPIO_H


#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "GPIO.h"
	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup GPIO
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Types GPIO Exported Types
  * @{
  */ 
	 
/** 
  * @brief  TMC GPIO Bit SET and Bit RESET enumeration 
  */
typedef enum
{
  GPIO_PIN_RESET = 0,
  GPIO_PIN_SET = 1
}GPIO_PinState;

/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/



/* Exported functions --------------------------------------------------------*/
/** @addtogroup TMC_GPIO_Exported_Functions
  * @{
  */

/** @addtogroup TMC_GPIO_Exported_Functions_Group1
  * @{
  */
void TMC_GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIOx_Init);
void TMC_GPIO_DeInit(GPIO_TypeDef* GPIOx);
void TMC_GPIO_StructInit(GPIO_InitTypeDef* GPIOx_Init);

/**
  * @}
  */
  
 /** @addtogroup TMC_GPIO_Exported_Functions_Group2
  * @{
  */ 
extern GPIO_PinState TMC_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
extern void TMC_GPIO_WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState Pin_State);

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

#endif  /* _TMC_GPIO_H */ 

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
