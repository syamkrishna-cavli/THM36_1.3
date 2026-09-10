/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: thm36_1.3_hal_conf.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __thm36_13_HAL_CONF_H
#define __thm36_13_HAL_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

//#define EMULATION 0
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* ########################## HSE/HSI Values adaptation ##################### */
/**
  * @brief Adjust the value of External High Speed oscillator (HSE) used in your application.
  *		This value is used by the RCC HAL module to compute the system frequency
  *		(when HSE is used as system clock source, directly or through the PLL).  
  */
#ifdef EMULATION

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE	((uint32_t)12000000U) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT	((uint32_t)100U)   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *		This value is used by the RCC HAL module to compute the system frequency
  *		(when HSI is used as system clock source, directly or through the PLL). 
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE	((uint32_t)48000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

#else

#if !defined  (HSE_VALUE) 
  #define HSE_VALUE	((uint32_t)12000000U) /*!< Value of the External oscillator in Hz */
#endif /* HSE_VALUE */

#if !defined  (HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT	((uint32_t)100U)   /*!< Time out for HSE start up, in ms */
#endif /* HSE_STARTUP_TIMEOUT */

/**
  * @brief Internal High Speed oscillator (HSI) value.
  *		This value is used by the RCC HAL module to compute the system frequency
  *		(when HSI is used as system clock source, directly or through the PLL). 
  */
#if !defined  (HSI_VALUE)
  #define HSI_VALUE	((uint32_t)12000000U) /*!< Value of the Internal oscillator in Hz*/
#endif /* HSI_VALUE */

/**
  * @brief Internal Low Speed oscillator (LSI) value.
  */
#if !defined  (LSI_VALUE) 
 #define LSI_VALUE  ((uint32_t)32000U)	   /*!< LSI Typical Value in Hz*/
#endif /* LSI_VALUE */					  /*!< Value of the Internal Low Speed oscillator in Hz
											 The real value may vary depending on the variations
											 in voltage and temperature.*/
/**
  * @brief External Low Speed oscillator (LSE) value.
  */
#if !defined  (LSE_VALUE)
 #define LSE_VALUE  ((uint32_t)32768U)	/*!< Value of the External Low Speed oscillator in Hz */
#endif /* LSE_VALUE */


#endif

#if !defined  (LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT	((uint32_t)5000U)   /*!< Time out for LSE start up, in ms */
#endif /* LSE_STARTUP_TIMEOUT */

/**
  * @brief External clock source for I2S peripheral
  *		This value is used by the I2S HAL module to compute the I2S clock source 
  *		frequency, this source is inserted directly through I2S_CKIN pad. 
  */
#if !defined  (EXTERNAL_CLOCK_VALUE)
  #define EXTERNAL_CLOCK_VALUE	((uint32_t)12288000U) /*!< Value of the External audio frequency in Hz*/
#endif /* EXTERNAL_CLOCK_VALUE */

/* Tip: To avoid modifying this file each time you need to use different HSE,
   ===  you can define the HSE value in your toolchain compiler preprocessor. */
   
/* ########################### System Configuration ######################### */
/**
  * @brief This is the HAL system configuration section
  */		  
#define  TICK_INT_PRIORITY			((uint32_t)0U)   /*!< tick interrupt priority */			


#ifdef __cplusplus
}
#endif

#endif /* __thm36_13_HAL_CONF_H */
 

/************************ (C) COPYRIGHT TongXinMicroelectronics *****END OF FILE****/
