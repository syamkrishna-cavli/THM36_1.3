/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tmc_exti.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TMC_EXTI_H
#define __TMC_EXTI_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include "EXTI.h"
	 
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_TMC_Driver
  * @{
  */
/** @defgroup TMC_EXTI
  * @brief TMC EXTI modules driver 
  * @{
  */
/** @defgroup TMC_EXTI_Exported_Types 
  * @{
  */


/** 
  * @brief  EXTI State Structure definition  
  */ 
//typedef struct
//{
//	uint32_t GPIO_A;				/*!< Bit0-Bit15 indicates GPIOA IO0-IO15, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t GPIO_B;				/*!< Bit0-Bit15 indicates GPIOB IO0-IO15, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t GPIO_C;				/*!< Bit0-Bit15 indicates GPIOC IO0-IO15, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t GPIO_D;				/*!< Bit0-Bit15 indicates GPIOD IO0-IO15, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t GPIO_E;				/*!< Bit0-Bit15 indicates GPIOE IO1-IO9, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t USB_Resume_Rsing;		/*!< USB_Resume, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t RTC_Alarm;				/*!< RTC_Alarm, 1:EXTI Linex has generated event 0:not have generated event*/ 
//	uint32_t IWDT_Alarm;			/*!< IWDT_Alarm, 1:EXTI Linex has generated event 0:not have generated event*/ 
//}EXTI_StateTypeDef;	 
		
void TMC_EXTI_GPIOA_IRQHandler(void);
void TMC_EXTI_GPIOB_IRQHandler(void);
void TMC_EXTI_GPIOC_IRQHandler(void);
void TMC_EXTI_GPIOD_IRQHandler(void);
void TMC_EXTI_GPIOE_IRQHandler(void);
void TMC_EXTI_USB_Resume_IRQHandler(void);
void TMC_EXTI_RTC_Alarm_IRQHandler(void);
void TMC_EXTI_WKUP_Pin0_IRQHandler(void);
void TMC_EXTI_WKUP_Pin1_IRQHandler(void);
void TMC_EXTI_IWDT_Alarm_IRQHandler(void);

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

#endif /* __TMC_EXTI_H */
