/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: tim.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

#ifndef __TIM_H
#define __TIM_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup TIM
  * @brief TIM HAL modules driver 
  * @{
  */ 

/** @defgroup TIM_Exported_Types HAL TIM Exported Types
  * @{
  */  
	 
/** 
  * @brief  TIM Time Base Init structure definition
  * @note   This sturcture is used with all TIMx   
  */
typedef struct
{
//	uint32_t TIM_period;		/*!< Specifies the period value to be loaded into the active
//									Auto-Reload Register at the next update event. 
//									if TIM_SizeMode is 16Bits,This parameter must be a number between 0x0000 and 0xFFFF
//									if TIM_SizeMode is 32Bits,This parameter must be a number between 0x00000000 and 0xFFFFFFFF	*/
	uint16_t TIM_Presclar;		/*!< Specifies the clock division.
									This parameter can be a value of @ref TIM_presclar											*/  
	uint8_t TIM_CounterMode;	/*!< Specifies the counter mode.This parameter can be a value of @ref TIM_counter_mode			*/  		
	uint8_t TIM_SizeMode;		/*!< Specifies the TIM Size mode.This parameter can be a value of @ref TIM_size_mode			*/  				
} TIM_TimeInitTypeDef; 			

/** @defgroup TIM_counter_mode TIM counter mode
  * @{
  */
#define TIM_MODE_CONTINUITY_NO_RELOAD	(0x00UL)	/*!< The Counter warps after reaching its zero value,and continues to count down from the maximum value. this is the default mode.*/ 
#define TIM_MODE_CONTINUITY_RELOAD		(0x40UL)		/*!< The Counter generates an interrupt at a constant interval,relaoding the original value after wrapping past zero*/ 
#define TIM_MODE_SINGEL					(0x01UL)	/*!< The Counter generates an interrupt once,when the counter reaches 0,it halts until you reprogram it,you can do this using one of follower:
														 Clear the one shot count bit in the contral register,in which case the count proceeds according to the selection of Free-running or periodic mode
														 Write a new value to the Load value register*/							  
/**
  * @}
  */ 

/** @defgroup TIM_presclar TIM presclar
  * @{
  */

#define TIM_PRESCLAR_NOT_DIV				(0UL)		/*!< The 0 stages of prescale,clock is divided 1,default*/ 
#define TIM_PRESCLAR_DIV_16				(0x04UL)	/*!< The 4 stages of prescale,clock is divided 16,default*/ 
#define TIM_PRESCLAR_DIV_256			(0x08UL)	/*!<The 8 stages of prescale,clock is divided 256,default*/

/**
  * @}
  */ 

/** @defgroup TIM_size_mode TIM size mode
  * @{
  */
#define TIM_SIZE_16BIT					(0UL)	/*!< Selects 16 bit Counter default*/
#define TIM_SIZE_32BIT					(0x02UL) 	/*!< Selects 32 bit Counter */ 
/**
  * @}
  */ 

/** @defgroup TIM_private_macros TIM private macros
  * @{
  */
/*!< Check the Parameter of TIM_MODE*/
#define IS_COUNTER_TIM_MODE(MODE)				(((MODE) == TIM_MODE_CONTINUITY_NO_RELOAD) || \
												((MODE) == TIM_MODE_CONTINUITY_RELOAD) 	  || \
												((MODE) == TIM_MODE_SINGEL_RELOAD)) 
#define IS_TIM_ALL_PERIPH(PERIPH)				(((PERIPH) == TIM1) || \
												((PERIPH) == TIM2) || \
												((PERIPH) == TIM3) || \
												((PERIPH) == TIM4) || \
												((PERIPH) == TIM5) || \
												((PERIPH) == TIM6))
/*!< Check the Parameter of TIM_SIZE_Mode*/
#define IS_TIM_SIZE(MODE)						(((MODE) == TIM_MODE_16BIT_SIZE) || \
												((MODE) == TIM_MODE_32BIT_SIZE))
/*!<Check the Parameter of TIM_Presclar*/
#define IS_TIM_PRE_DIV(MODE)					(((MODE) == TIM_PRESCLAR_NOT_DIV)  || \
												((MODE) == TIM_PRESCLAR_DIV_16) || \
												((MODE) == TIM_PRESCLAR_DIV_256))	
/**
  * @}
  */ 
/** @addtogroup TIM_Exported_Function 
  * @{  
  */   
void TIM_DeInit(TIM_TypeDef* TIMx);
void TIM_TimeInit(TIM_TypeDef* TIMx, TIM_TimeInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_TimeStructInit(TIM_TimeInitTypeDef* TIM_TimeBaseInitStruct);
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState);
void TIM_ITConfig(TIM_TypeDef* TIMx,FunctionalState NewState);
void TIM_ClearFlag(TIM_TypeDef* TIMx);
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx);
FlagStatus TIM_GetITStatus(TIM_TypeDef* TIMx);
void TIM_ClearITStatus(TIM_TypeDef* TIMx);
void TIM_SetLoadValue(TIM_TypeDef* TIMx,uint32_t LoadValue); 
uint32_t TIM_GetValue(TIM_TypeDef* TIMx);
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
/**
  * @}
  */  
  
									


#ifdef __cplusplus
}
#endif

#endif   
  

  
  



