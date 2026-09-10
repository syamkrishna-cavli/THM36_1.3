/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: wdt.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WDT_H
#define __WDT_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup WDT
  * @brief WDT HAL modules driver 
  * @{
  */ 

/** @defgroup WDT_Exported_Types HAL WDT Exported Types
  * @{
  */
	 
/** 
  * @brief  WDT Init structure definition
  */
typedef struct
{
	uint32_t			Loadvalue;	   /*!< Specifies the WDT counter value.
										This parameter must be a number between Min_Data = 0x01 and Max_Data = 0xFFFFFFFF */
	
	uint32_t			Mode;	   	/*!< Specifies the WDT Mode.
										This parameter can be a value of @ref WDT_set_mode							 */
}WDT_InitTypeDef;

/**
  * @}
  */

/** @defgroup WDT_Exported_Constants WDT Exported Constants
  * @{
  */

/** @defgroup WDT_set_mode WDT set mode 
  * @{
  */

#define WDT_MODE_ALARM				0x00000002UL
#define WDT_MODE_INT				0x00000000UL


/**
  * @}
  */
/**
  * @}
  */

/** @addtogroup WDT_Exported_Function 
  * @{  
  */   
void WDT_DeInit(void);
void WDT_Init(WWDT_TypeDef *WDT, WDT_InitTypeDef * init);
void WDT_Cmd(WWDT_TypeDef *WDT, FunctionalState NewState);
FlagStatus WDT_GetFlagStatus(WWDT_TypeDef *WDT);
FlagStatus WDT_GetITStatus(WWDT_TypeDef *WDT);
void WDT_ClearFlag(WWDT_TypeDef *WDT);
uint32_t WDT_ReadCurrentValue(WWDT_TypeDef *WDT);
void WDT_FeedDog(WWDT_TypeDef *WDT, WDT_InitTypeDef * init);
void WDT_StructInit(WDT_InitTypeDef * init);
/**
  * @}
  */

/** @defgroup WDT_private_macros WDT private macros
  * @{
  */

#define IS_WDT_LOADVALUE(b) ((b>0)&&(b<=0xFFFFFFFF))
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

#endif   
  

  
  



