/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_wdt.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_WDT_H
#define _TMC_WDT_H



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "wdt.h"	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_WDT
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup TMC_WDT_Exported_Types TMC WDT Exported Types
  * @{
  */ 

/**
  * @brief  WDT  structure definition
  */
/** 
  * @brief	UART handle structure definition  
  */
typedef enum
{
	TMC_WDT_STATE_RESET			= 0x00U,			
	TMC_WDT_STATE_READY			= 0x01U,			
	TMC_WDT_STATE_BUSY			= 0x02U,
	TMC_WDT_STATE_OVERFLOW		= 0x03U,
} WWDT_StateTypeDef;
/**
  * @brief  WDT handle Structure definition
  */
typedef struct
{
	WWDT_TypeDef			*WDT;  			/*!< Register base address	*/
	WDT_InitTypeDef			*Init;	   	/*!< WWDG required parameters */
	WWDT_StateTypeDef	   State;
}WDT_HandleTypeDef;

/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup TMC_WDT_Exported_Constants TMC WDT Exported Constants
  * @{
  */

/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup TMC_WDT_Exported_Functions
  * @{
  */ 
TMC_StatusTypeDef TMC_WDT_Init(WDT_HandleTypeDef *hwdt);
TMC_StatusTypeDef TMC_WDT_DeInit(WDT_HandleTypeDef *hwdt);
TMC_StatusTypeDef TMC_WDT_Refresh(WDT_HandleTypeDef *hwdt);
void TMC_WDT_IRQHandler(WDT_HandleTypeDef *hwdt);
void TMC_WDT_MspInit(WDT_HandleTypeDef *hwdt);
void TMC_WDT_Callback(WDT_HandleTypeDef *hwdt);
void TMC_WDT_MspDeInit(WDT_HandleTypeDef *hwdt);
/**
  * @}
  */ 
/* Private macros ------------------------------------------------------------*/
/** @defgroup TMC_WDT_Private_Macros TMC WDT Private Macros
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
