/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: exti.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef _EXTI_H
#define _EXTI_H
#ifdef __cplusplus
 extern "C" {
#endif
	 
/* Includes ------------------------------------------------------------------*/

/** @addtogroup thm36_1.3
  * @{
  */	 
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
	 
/** @defgroup EXTI
	* @brief EXTI HAL modules driver
  * @{
  */
/** 
  * @brief  EXTI structures definition  
  */
typedef enum 
{
	EXTI_GPIOA = 0,
	EXTI_GPIOB = 1,
	EXTI_GPIOC = 2,
	EXTI_GPIOD = 3,
	EXTI_GPIOE = 4
} EXTI_Groupx;

/** @defgroup EXTI_pins_define EXTI pins define
  * @{
  */
#define EXTI_PIN_0				((uint32_t)0x0001)  /* Pin 0 selected */
#define EXTI_PIN_1				((uint32_t)0x0002)  /* Pin 1 selected */
#define EXTI_PIN_2				((uint32_t)0x0004)  /* Pin 2 selected */
#define EXTI_PIN_3				((uint32_t)0x0008)  /* Pin 3 selected */
#define EXTI_PIN_4				((uint32_t)0x0010)  /* Pin 4 selected */
#define EXTI_PIN_5				((uint32_t)0x0020)  /* Pin 5 selected */
#define EXTI_PIN_6				((uint32_t)0x0040)  /* Pin 6 selected */
#define EXTI_PIN_7				((uint32_t)0x0080)  /* Pin 7 selected */
#define EXTI_PIN_8				((uint32_t)0x0100)  /* Pin 8 selected */
#define EXTI_PIN_9				((uint32_t)0x0200)  /* Pin 9 selected */
#define EXTI_PIN_10				((uint32_t)0x0400)  /* Pin 10 selected */
#define EXTI_PIN_11				((uint32_t)0x0800)  /* Pin 11 selected */
#define EXTI_PIN_12				((uint32_t)0x1000)  /* Pin 12 selected */
#define EXTI_PIN_13				((uint32_t)0x2000)  /* Pin 13 selected */
#define EXTI_PIN_14				((uint32_t)0x4000)  /* Pin 14 selected */
#define EXTI_PIN_15				((uint32_t)0x8000)  /* Pin 15 selected */
#define EXTI_PIN_All			((uint32_t)0xFFFF)  /* All pins selected */

/**
  * @}
  */

/** @defgroup EXTI_Other_Mode_define EXTI Other Mode define
  * @{
  */
#define EXTI_USB_WAKE_UP					((uint32_t)0x0001)  
#define EXTI_RTC_ALM_WAKE_UP				((uint32_t)0x0002) 
#define EXTI_IWDT_ALM_WAKE_UP				((uint32_t)0x0004)  
 

#define EXTI_BAT_PIN0_WAKE_UP				((uint32_t)0x0010)
#define EXTI_BAT_PIN1_WAKE_UP				((uint32_t)0x0020)

#define EXTI_BAT_PIN0_RSING_WAKE_UP			((uint32_t)0x0040)  
#define EXTI_BAT_PIN1_RSING_WAKE_UP			((uint32_t)0x0080)

#define EXTI_BAT_PIN0_FALLING_WAKE_UP		((uint32_t)0x0100)  
#define EXTI_BAT_PIN1_FALLING_WAKE_UP		((uint32_t)0x0200)

#define EXTI_BAT_PIN0_HIGH_LEVEL_WAKE_UP	((uint32_t)0x0400)  
#define EXTI_BAT_PIN1_HIGH_LEVEL_WAKE_UP	((uint32_t)0x0800)

#define EXTI_BAT_PIN0_LOW_LEVEL_WAKE_UP		((uint32_t)0x1000)  
#define EXTI_BAT_PIN1_LOW_LEVEL_WAKE_UP		((uint32_t)0x2000)
/**
  * @}
  */
///** @defgroup EXTI_80_TO_EXTI_85 EXTI 80 TO EXTI 85
//  * @{
//  */
//#define EXTI_80_USB				1U
//#define EXTI_81_RTC_ALARM		(1U<<1)
//#define EXTI_82_IWDT_ALARM		(1U<<2)
//#define EXTI_83_WAKE_UP_PIN_0	(1U<<3)
//#define EXTI_84_WAKE_UP_PIN_1	(1U<<4)
///**
//  * @}
//  */
  
/** @defgroup EXTI_Private_Macros EXTI Private Macros
  * @{
  */
#define IS_EXTI_PIN(x) ((x == EXTI_PIN_0)||(x == EXTI_PIN_1)||(x == EXTI_PIN_2)||(x == EXTI_PIN_3)||(x == EXTI_PIN_4)|| \
					   (x == EXTI_PIN_5)||(x == EXTI_PIN_6)||(x == EXTI_PIN_7)||(x == EXTI_PIN_8)||(x == EXTI_PIN_9)|| \
					   (x == EXTI_PIN_10)||(x == EXTI_PIN_11)||(x == EXTI_PIN_12)||(x == EXTI_PIN_13)||(x == EXTI_PIN_14)||(x == EXTI_PIN_15)||  \
					   (x == EXTI_PIN_All))

#define IS_EXTI_OTHER(x) ((x == EXTI_USB_WAKE_UP)||(x == EXTI_RTC_ALM_WAKE_UP)||(x == EXTI_IWDT_ALM_WAKE_UP)|| \
						  (x == EXTI_BAT_PIN0_RSING_WAKE_UP)||(x==EXTI_BAT_PIN1_RSING_WAKE_UP)|| \
						  (x == EXTI_BAT_PIN0_FALLING_WAKE_UP)||(x == EXTI_BAT_PIN1_FALLING_WAKE_UP)||(x == EXTI_BAT_PIN0_HIGH_LEVEL_WAKE_UP)|| \
						  (x == EXTI_BAT_PIN1_HIGH_LEVEL_WAKE_UP)||(x == EXTI_BAT_PIN0_LOW_LEVEL_WAKE_UP)||(x == EXTI_BAT_PIN1_LOW_LEVEL_WAKE_UP))
/**
  * @}
  */

/** @addtogroup EXTI_Exported_Types
  * @{
  */
void EXTI_Reset(void);
void EXTI_ConfigRising(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus);
void EXTI_ConfigFalling(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus);
void EXTI_ConfigHighLevel(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus);
void EXTI_ConfigLowLevel(EXTI_Groupx EXTIx,uint32_t EXTI_Line,FunctionalState NewStatus);
void EXTI_ConfigOtherMode(uint32_t Mode,FunctionalState NewStatus);
void EXTI_GenerateSWInterrupt(EXTI_Groupx EXTIx,uint32_t EXTI_Line);
FlagStatus EXTI_GetPINxStatus(EXTI_Groupx EXTIx,uint32_t EXTI_Line);
FlagStatus EXTI_GetOtherStatus(uint32_t Flag);
void EXTI_ClearOtherStatus(uint32_t Flag);
void EXTI_ClearITPendingBit(EXTI_Groupx EXTIx,uint32_t EXTI_Line);
void EXTI_ConfigInterruptMode(IRQn_Type IRQn,FunctionalState NewStatus);
void EXTI_ConfigEventMode(IRQn_Type IRQn,FunctionalState NewStatus);

  



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

