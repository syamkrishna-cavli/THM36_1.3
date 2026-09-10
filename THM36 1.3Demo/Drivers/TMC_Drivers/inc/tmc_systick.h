/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_systick.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_SYSTICK_H
#define _TMC_SYSTICK_H



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "core_sc300.h"
	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_SYSTICK
  * @{
  */ 


/* Exported constants --------------------------------------------------------*/
/** @defgroup TMC_SYSTICK_Exported_Constants TMC SYSTICK Exported Constants
  * @{
  */
/** @defgroup HAL_TICK_FREQ Tick Frequency
  * @{
  */
typedef enum
{
	TMC_TICK_FREQ_10HZ			= 100U,
	TMC_TICK_FREQ_100HZ			= 10U,
	TMC_TICK_FREQ_1KHZ			= 1U,
	TMC_TICK_FREQ_DEFAULT		= TMC_TICK_FREQ_1KHZ
} TMC_TickFreqTypeDef;
/**
  * @}
*/

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup TMC_SYSTICK_Exported_Functions
  * @{
  */
TMC_StatusTypeDef TMC_InitTick(uint32_t TickPriority);
uint32_t TMC_SYSTICK_Config(uint32_t TicksNumb);
void TMC_IncTick(void);
void TMC_Delay(uint32_t Delay);
uint32_t TMC_GetTick(void);
uint32_t TMC_GetTickPrio(void);
TMC_StatusTypeDef TMC_SetTickFreq(TMC_TickFreqTypeDef Freq);
TMC_TickFreqTypeDef TMC_GetTickFreq(void);
void TMC_SuspendTick(void);
void TMC_ResumeTick(void);
void TMC_SYSTICK_IRQHandler(void);
void TMC_SYSTICK_Callback(void);
void delay_us(uint32_t nus);
void delay_ms(uint32_t nms);
  
/**
  * @}
  */ 
/* Private macros ------------------------------------------------------------*/
/** @defgroup TMC_SYSTICK_Private_Macros TMC SYSTICK Private Macros
  * @{
  */

//#define IS_TICKFREQ(FREQ) (((FREQ) == HAL_TICK_FREQ_10HZ)  || \
//						   ((FREQ) == HAL_TICK_FREQ_100HZ) || \
//						   ((FREQ) == HAL_TICK_FREQ_1KHZ))
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
