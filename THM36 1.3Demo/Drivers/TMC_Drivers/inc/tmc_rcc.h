/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_rcc.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _TMC_RCC_H
#define _TMC_RCC_H



#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include "RCC.h"	 
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */	
	 
/** @addtogroup TMC_RCC
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup TMC_RCC_Exported_Types TMC RCC Exported Types
  * @{
  */ 

/**
  * @brief  RCC System clock configuration structure definition
  */
typedef struct
{
	uint32_t SYSCLKSource;		  /*!< The clock source (SYSCLKS) used as system clock.
									   This parameter can be a value of @ref System_Source_Clock_Define	*/

	uint32_t SYSCLKDIV;		 	/*!< The system clock divider. This clock is derived from the system clock (SYSCLK).
									   This parameter can be 0x00 to 0x1F	   */

}RCC_SysclkInitTypeDef;

/**
  * @brief  RCC Internal/External Oscillator (HSE, HSI, LSE and LSI) configuration structure definition
  */
typedef struct
{
	uint32_t ClockSourceType;			/*!< The clock to be configured.
											 This parameter can be a value of @ref TMC_RCC_clocksource_type	*/
	FunctionalState LSEState;			/*!< The new state of the LSE.										*/

	FunctionalState HSIState;			/*!< The new state of the HSI.										*/

	FunctionalState LSIState;			/*!< The new state of the LSI.										*/
	
	FunctionalState PLLState;			/*!< The new state of the PLL.										*/
	
	
	RCC_LSEConfigTypeDef LSEConfig;		/*!< LSE configure structure parameters.							*/
	
	RCC_PLL_InitTypeDef PLLConfig;		/*!< PLL configure structure parameters.						*/
	
	
}RCC_OscInitTypeDef;
/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/
/** @defgroup TMC_RCC_Exported_Constants TMC RCC Exported Constants
  * @{
  */
/** @defgroup TMC_RCC_clocksource_type	 TMC clock source type
  * @{
  */


#define RCC_ClockSource_HSI			 0x00000002UL
#define RCC_ClockSource_LSE			 0x00000004UL
#define RCC_ClockSource_LSI			 0x00000008UL
#define RCC_ClockSource_PLL			 0x00000010UL


/**
  * @}
  */
/**
  * @}
  */
/* Exported functions --------------------------------------------------------*/
/** @addtogroup TMC_RCC_Exported_Functions
  * @{
  */ 
TMC_StatusTypeDef TMC_RCC_OscConfig(RCC_OscInitTypeDef  *RCC_OscInitStruct);
TMC_StatusTypeDef TMC_RCC_SystemClockConfig(RCC_SysclkInitTypeDef *RCC_SysclkInitStruct);
  
/**
  * @}
  */ 
/* Private macros ------------------------------------------------------------*/
/** @defgroup TMC_RCC_Private_Macros RCC Private Macros
  * @{
  */

#define IS_RCC_CLOCKSOURCE(SOURCE)					(((SOURCE) <= (RCC_ClockSource_HSI|RCC_ClockSource_LSE|RCC_ClockSource_LSI|RCC_ClockSource_PLL)))

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
