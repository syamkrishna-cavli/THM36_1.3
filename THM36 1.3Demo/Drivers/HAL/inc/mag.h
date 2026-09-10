/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: mag.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
  
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _MAG_H
#define _MAG_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup MAG
  * @brief MAG HAL modules driver 
  * @{
  */ 
/** @defgroup MAG_Exported_Constants MAG Exported Constants
  * @{
  */
	 
/** @defgroup MAG_sts MAG stauts
  * @brief   
  * @{
  */ 
#define MAG_STS_DETECTED_SWIPE_ACTION		(1UL<<4)
#define MAG_STS_DATA_OVERFLOWER				(1UL<<3)
#define MAG_STS_WAITE_NOISE_MONITOR_OVER	(1UL<<2)
#define MAG_STS_SWIPE_CARD_COMPLETED		(1UL<<1)
#define MAG_STS_DATA_BUFFER_HAVE_DATA		(1UL)
/**
  * @}
  */

/** @defgroup MAG_IT MAG IT
  * @brief   
  * @{
  */ 
#define MAG_IT_DETECTED_SWIPE_ACTION		(1UL<<4)
#define MAG_IT_DATA_OVERFLOWER				(1UL<<3)
#define MAG_IT_WAITE_NOISE_MONITOR_OVER		(1UL<<2)
#define MAG_IT_SWIPE_CARD_COMPLETED			(1UL<<1)
#define MAG_IT_DATA_BUFFER_HAVE_DATA		(1UL)
/**
  * @}
  */

/** @defgroup MAG_DIGITAL_FILTER MAG DIGITAL FILTER
  * @brief   
  * @{
  */ 
#define MAG_FILTER_2_COUNS_DATA_AVERAGES	0
#define MAG_FILTER_3_COUNS_DATA_AVERAGES	1
#define MAG_FILTER_4_COUNS_DATA_AVERAGES	2
#define MAG_FILTER_5_COUNS_DATA_AVERAGES	3
/**
  * @}
  */

/** @defgroup MAG_CONFIG MAG CONFIG
  * @brief   
  * @{
  */ 
#define MAG_CONFIG_AUTO_THRESHOLD		(1UL<<4)
#define MAG_CONFIG_PEEL					(1UL<<3)
#define MAG_CONFIG_FILTER				(1UL<<2)
#define MAG_CONFIG_WITHE_NOSIE			(1UL<<1)
#define MAG_CONFIG_TOP_LOW_THRESHOLD	(1UL)
/**
  * @}
  */

/** @defgroup MAG_private_macros MAG private macros
  * @{
  */ 
#define IS_MAG_CHECK_STS(status)			((status & MAG_STS_DETECTED_SWIPE_ACTION)|| \
											 (status & MAG_STS_DATA_OVERFLOWER)|| \
											 (status & MAG_STS_WAITE_NOISE_MONITOR_OVER)|| \
											 (status & MAG_STS_SWIPE_CARD_COMPLETED)|| \
											 (status & MAG_STS_DATA_BUFFER_HAVE_DATA))  

												
#define IS_MAG_CHECK_IT(IT)					((IT&MAG_IT_DETECTED_SWIPE_ACTION)|| \
											 (IT&MAG_IT_DATA_OVERFLOWER)|| \
											 (IT&MAG_IT_WAITE_NOISE_MONITOR_OVER)|| \
											 (IT&MAG_IT_SWIPE_CARD_COMPLETED)|| \
											 (IT&MAG_IT_DATA_BUFFER_HAVE_DATA)) 
#define IS_MAG_BASE_ADDRESS(MAG)			((MAG == MAG1)|| \
											 (MAG == MAG2)|| \
											 (MAG == MAG3)) 
#define IS_MAG_CONFIG_VALUE(MODE)			((MODE & MAG_CONFIG_AUTO_THRESHOLD)|| \
											 (MODE & MAG_CONFIG_PEEL)|| \
											 (MODE & MAG_CONFIG_FILTER)|| \
											 (MODE & MAG_CONFIG_WITHE_NOSIE)|| \
											 (MODE & MAG_CONFIG_TOP_LOW_THRESHOLD)) 								 
/**
  * @}
  */

/**
  * @}
  */
/** @addtogroup MAG_Exported_Function 
  * @{  
  */   
FlagStatus MAG_GetStatus(MAG_TypeDef* Magx, uint32_t status);
void MAG_ClearStatus(MAG_TypeDef* Magx, uint32_t status);
void MAG_ITConfig(MAG_TypeDef* Magx, uint32_t Mask,FunctionalState NewState);
void MAG_GetCardSwipeValue(MAG_TypeDef* Magx,uint16_t* Voltage, uint16_t* time);
void MAG_GetWhiteNoise(MAG_TypeDef* Magx);
void MAG_ConfigCardSwipeVoltageThreshold(MAG_TypeDef* Magx,uint16_t HighVoltage,uint16_t LowVoltage);
void MAG_ConfigWitheNoiseCheckTimes(MAG_TypeDef* Magx,uint16_t times);
void MAG_ConfigDigitalFilter(MAG_TypeDef* Magx,uint16_t mode);
void MAG_ConfigTopAndLowThreshold(MAG_TypeDef* Magx,uint16_t TopVoltage,uint16_t LowVoltage);
void MAG_ConfigCardSwipeCheckTime(MAG_TypeDef* Magx,uint16_t times);
void MAG_Cmd(MAG_TypeDef* Magx,uint32_t mode,FunctionalState NewState);
uint32_t MAG_GetITMask(MAG_TypeDef* Magx);
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
#endif /* __TMC_MAG_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
