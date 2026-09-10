/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: thm36_1.3_hal_def.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __thm36_13_HAL_DEF
#define __thm36_13_HAL_DEF

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "core_sc300.h"
	 
/* Exported types ------------------------------------------------------------*/

/** 
  * @brief  Standard Peripheral Library old types 
  */ 
typedef int32_t  s32;
typedef int16_t s16;
typedef int8_t  s8;

typedef const int32_t sc32;  /*!< Read Only */
typedef const int16_t sc16;  /*!< Read Only */
typedef const int8_t sc8;   /*!< Read Only */

typedef __IO int32_t  vs32;
typedef __IO int16_t  vs16;
typedef __IO int8_t   vs8;

typedef __I int32_t vsc32;  /*!< Read Only */
typedef __I int16_t vsc16;  /*!< Read Only */
typedef __I int8_t vsc8;   /*!< Read Only */

typedef uint32_t  uint32_t;
typedef uint16_t uint16_t;
typedef uint8_t  uint8_t;

typedef const uint32_t uc32;  /*!< Read Only */
typedef const uint16_t uc16;  /*!< Read Only */
typedef const uint8_t uc8;   /*!< Read Only */

typedef __IO uint32_t  vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t  vu8;

typedef __I uint32_t vuc32;  /*!< Read Only */
typedef __I uint16_t vuc16;  /*!< Read Only */
typedef __I uint8_t vuc8;   /*!< Read Only */

#ifndef __cplusplus
typedef enum {FALSE = 0, TRUE = !FALSE} bool;
#endif

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;

#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

typedef enum {Busy = 0, NoBusy = !Busy} BusyStatus;

#define IS_POINT_NULL(x)				(x!=0)
#define USE_FULL_ASSERT
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function
  *   which reports the name of the source file and the source
  *   line number of the call that failed. 
  *   If expr is true, it returns no value.
  * @retval None
  */
#define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
void assert_failed(uint8_t* file, uint32_t line);
#else
#define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

typedef enum 
{
  STATUS_OK	   = 0x00U,
  STATUS_ERROR	= 0x01U,
  STATUS_BUSY	 = 0x02U,
  STATUS_TIMEOUT  = 0x03U
} StatusTypeDef;
/** 
  * @brief  HAL Lock structures definition  
  */
typedef enum 
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U  
} HAL_LockTypeDef;

typedef enum 
{
  TMC_OK	   = 0x00U,
  TMC_ERROR	= 0x01U,
  TMC_BUSY	 = 0x02U,
  TMC_TIMEOUT  = 0x03U
} TMC_StatusTypeDef;

/* Exported macro ------------------------------------------------------------*/

#define UNUSED(X) (void)X	  /* To avoid gcc/g++ warnings */

#define HAL_MAX_DELAY	  0xFFFFFFFEU

#define HAL_IS_BIT_SET(REG, BIT)		 (((REG) & (BIT)) != RESET)
#define HAL_IS_BIT_CLR(REG, BIT)		 (((REG) & (BIT)) == RESET)

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)			   \
						do{													  \
							  (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
							  (__DMA_HANDLE__).Parent = (__HANDLE__);			 \
						  } while(0U)

/** @brief Reset the Handle's State field.
  * @param __HANDLE__ specifies the Peripheral Handle.
  * @note  This macro can be used for the following purpose: 
  *		  - When the Handle is declared as local variable; before passing it as parameter
  *			to HAL_PPP_Init() for the first time, it is mandatory to use this macro 
  *			to set to 0 the Handle's "State" field.
  *			Otherwise, "State" field may have any random value and the first time the function 
  *			HAL_PPP_Init() is called, the low level hardware initialization will be missed
  *			(i.e. HAL_PPP_MspInit() will not be executed).
  *		  - When there is a need to reconfigure the low level hardware: instead of calling
  *			HAL_PPP_DeInit() then HAL_PPP_Init(), user can make a call to this macro then HAL_PPP_Init().
  *			In this later function, when the Handle's "State" field is set to 0, it will execute the function
  *			HAL_PPP_MspInit() which will reconfigure the low level hardware.
  * @retval None
  */
#define __HAL_RESET_HANDLE_STATE(__HANDLE__) ((__HANDLE__)->State = 0U)

#if (USE_RTOS == 1U)
  /* Reserved for future use */
  #error "USE_RTOS should be 0 in the current HAL release"
#else
  #define __HAL_LOCK(__HANDLE__)										   \
								do{										\
									if((__HANDLE__)->Lock == HAL_LOCKED)   \
									{									  \
									   return TMC_BUSY;					\
									}									  \
									else								   \
									{									  \
									   (__HANDLE__)->Lock = HAL_LOCKED;	\
									}									  \
								  }while (0U)

  #define __HAL_UNLOCK(__HANDLE__)										  \
								  do{									   \
									  (__HANDLE__)->Lock = HAL_UNLOCKED;	\
									}while (0U)
#endif /* USE_RTOS */

#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __weak
	#define __weak   __attribute__((weak))
  #endif /* __weak */
  #ifndef __packed
	#define __packed __attribute__((__packed__))
  #endif /* __packed */
#endif /* __GNUC__ */


/* Macro to get variable aligned on 4-bytes, for __ICCARM__ the directive "#pragma data_alignment=4" must be used instead */
#if defined ( __GNUC__ ) && !defined (__CC_ARM) /* GNU Compiler */
  #ifndef __ALIGN_END
#define __ALIGN_END	__attribute__ ((aligned (4)))
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN  
	#define __ALIGN_BEGIN
  #endif /* __ALIGN_BEGIN */
#else
  #ifndef __ALIGN_END
	#define __ALIGN_END
  #endif /* __ALIGN_END */
  #ifndef __ALIGN_BEGIN	  
	#if defined   (__CC_ARM)	  /* ARM Compiler */
#define __ALIGN_BEGIN	__align(4)
	#elif defined (__ICCARM__)	/* IAR Compiler */
	  #define __ALIGN_BEGIN 
	#endif /* __CC_ARM */
  #endif /* __ALIGN_BEGIN */
#endif /* __GNUC__ */


/** 
  * @brief  __RAM_FUNC definition
  */ 
#if defined ( __CC_ARM   )
/* ARM Compiler
   ------------
   RAM functions are defined using the toolchain options. 
   Functions that are executed in RAM should reside in a separate source module.
   Using the 'Options for File' dialog you can simply change the 'Code / Const' 
   area of a module to a memory space in physical RAM.
   Available memory areas are declared in the 'Target' tab of the 'Options for Target'
   dialog. 
*/
#define __RAM_FUNC

#elif defined ( __ICCARM__ )
/* ICCARM Compiler
   ---------------
   RAM functions are defined using a specific toolchain keyword "__ramfunc". 
*/
#define __RAM_FUNC __ramfunc

#elif defined   (  __GNUC__  )
/* GNU Compiler
   ------------
  RAM functions are defined using a specific toolchain attribute 
   "__attribute__((section(".RamFunc")))".
*/
#define __RAM_FUNC __attribute__((section(".RamFunc")))

#endif

/** 
  * @brief  __NOINLINE definition
  */ 
#if defined ( __CC_ARM   ) || defined   (  __GNUC__  )
/* ARM & GNUCompiler 
   ---------------- 
*/
#define __NOINLINE __attribute__ ( (noinline) )

#elif defined ( __ICCARM__ )
/* ICCARM Compiler
   ---------------
*/
#define __NOINLINE _Pragma("optimize = no_inline")

#endif

#ifdef __cplusplus
}
#endif

#endif /* ___thm36_13_HAL_DEF */

/************************ (C) COPYRIGHT TongXinMicroelectronics *****END OF FILE****/

