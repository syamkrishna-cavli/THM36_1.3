/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: gpio.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _GPIO_H
#define _GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"	 

/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */	
	 
/** @addtogroup GPIO
  * @{
  */ 
 
/* Exported types ------------------------------------------------------------*/
/** @defgroup GPIO_Exported_Types GPIO Exported Types
  * @{
  */ 
	 
/** 
  * @brief   GPIO Init structure definition  
  */	  
typedef struct
{
	volatile	uint32_t Pin;  /*!< Specifies the GPIO pins to be configured.This parameter can be any value of @ref GPIO_pins_define */ 
	volatile	uint32_t Mode; /*!< Specifies the operating mode for the selected pins.This parameter can be a value of @ref GPIO_mode_define */
	volatile	uint32_t Speed;/*!< Specifies the speed for the selected pins.This parameter can be a value of @ref GPIO_speed_define */
	volatile	uint32_t Pull; /*!< Specifies the operating Pull-up/Pull down for the selected pins.This parameter can be a value of @ref GPIO_pull_define */
	volatile	uint32_t Otype;/*!< Specifies the operating output type for the selected pins.This parameter can be a value of @ref GPIO_od_define */
	volatile	uint32_t SMIT; /*!< Specifies the operating input schmit type for the selected pins.This parameter can be a value of @ref GPIO_schmit_define */
} GPIO_InitTypeDef;	 

/** 
  * @brief  GPIO Bit SET and Bit RESET enumeration 
  */

typedef enum
{ Bit_RESET = 0,				
  Bit_SET
}BitAction;
/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/

/** @defgroup GPIO_Exported_Constants GPIO Exported Constants
  * @{
  */ 

/** @defgroup GPIO_mode_define GPIO mode define
  * @{
  */
#define GPIO_MODE_IN			(0x00000000UL)  
#define GPIO_MODE_OUT			(0x00000001UL)  
#define GPIO_MODE_AF			(0x00000002UL)  

/**
  * @}
  */
							
/** @defgroup GPIO_speed_define  GPIO Output Maximum speed define  
  * @{
  */ 
#define GPIO_LOW_SPEED			(0x00000000UL)  
#define GPIO_MEDIUM_SPEED		(0x00000001UL)  
#define GPIO_FAST_SPEED			(0x00000002UL)  
#define GPIO_HIGH_SPEED			(0x00000003UL)   

/**
  * @}
  */

/** @defgroup GPIO_pull_define GPIO Pull-Up or Pull-Down define
   * @{
  */ 
#define GPIO_NOPULL				(0x00000000UL)  
#define GPIO_PULLUP				(0x00000001UL)  
#define GPIO_PULLDOWN			(0x00000002UL)  

/**
  * @}
  */

/** @defgroup GPIO_od_define GPIO open drain define
  * @{
  */ 

#define GPIO_MODE_OUTPUT_PP		(0x00000000UL)  
#define GPIO_MODE_OUTPUT_OD		(0x00000001UL)
/**
  * @}
  */

/** @defgroup GPIO_schmit_define GPIO input schmit define
  * @{
  */ 
#define GPIO_INPUTSCHMIT_DISABLE (0x00000000UL)  
#define GPIO_INPUTSCHMIT_ENABLE  (0x00000001UL) 

/**
  * @}
  */


/** @defgroup GPIO_pins_define GPIO pins define
  * @{
  */
#define GPIO_PIN_0				((uint16_t)0x0001)  /* Pin 0 selected */
#define GPIO_PIN_1				((uint16_t)0x0002)  /* Pin 1 selected */
#define GPIO_PIN_2				((uint16_t)0x0004)  /* Pin 2 selected */
#define GPIO_PIN_3				((uint16_t)0x0008)  /* Pin 3 selected */
#define GPIO_PIN_4				((uint16_t)0x0010)  /* Pin 4 selected */
#define GPIO_PIN_5				((uint16_t)0x0020)  /* Pin 5 selected */
#define GPIO_PIN_6				((uint16_t)0x0040)  /* Pin 6 selected */
#define GPIO_PIN_7				((uint16_t)0x0080)  /* Pin 7 selected */
#define GPIO_PIN_8				((uint16_t)0x0100)  /* Pin 8 selected */
#define GPIO_PIN_9				((uint16_t)0x0200)  /* Pin 9 selected */
#define GPIO_PIN_10				((uint16_t)0x0400)  /* Pin 10 selected */
#define GPIO_PIN_11				((uint16_t)0x0800)  /* Pin 11 selected */
#define GPIO_PIN_12				((uint16_t)0x1000)  /* Pin 12 selected */
#define GPIO_PIN_13				((uint16_t)0x2000)  /* Pin 13 selected */
#define GPIO_PIN_14				((uint16_t)0x4000)  /* Pin 14 selected */
#define GPIO_PIN_15				((uint16_t)0x8000)  /* Pin 15 selected */
#define GPIO_PIN_All			((uint16_t)0xFFFF)  /* All pins selected */

/**
  * @}
  */

/** @defgroup GPIO_pin_sources GPIO pins sources
  * @{
  */ 
#define GPIO_PIN_Source0			((uint32_t)0x00)
#define GPIO_PIN_Source1			((uint32_t)0x01)
#define GPIO_PIN_Source2			((uint32_t)0x02)
#define GPIO_PIN_Source3			((uint32_t)0x03)
#define GPIO_PIN_Source4			((uint32_t)0x04)
#define GPIO_PIN_Source5			((uint32_t)0x05)
#define GPIO_PIN_Source6			((uint32_t)0x06)
#define GPIO_PIN_Source7			((uint32_t)0x07)
#define GPIO_PIN_Source8			((uint32_t)0x08)
#define GPIO_PIN_Source9			((uint32_t)0x09)
#define GPIO_PIN_Source10		   ((uint32_t)0x0A)
#define GPIO_PIN_Source11		   ((uint32_t)0x0B)
#define GPIO_PIN_Source12		   ((uint32_t)0x0C)
#define GPIO_PIN_Source13		   ((uint32_t)0x0D)
#define GPIO_PIN_Source14		   ((uint32_t)0x0E)
#define GPIO_PIN_Source15		   ((uint32_t)0x0F)

/**
  * @}
  */

/** @defgroup GPIO_alternat_function_selection_define GPIO Alternate Function Selection
  * @{
  */ 

/** 
  * @brief   AF 1 selection  
  */ 
#define GPIO_AF1_SPI1		  ((uint8_t)0x00)  /* SPI11 Alternate Function mapping */
#define GPIO_AF1_SPI2		  ((uint8_t)0x00)  /* SPI2 Alternate Function mapping */
#define GPIO_AF1_SPI3		  ((uint8_t)0x00)  /* SPI3 Alternate Function mapping */
#define GPIO_AF1_SPI2_CLK_IN   ((uint8_t)0x00)  /* SPI2_CLK_IN Alternate Function mapping */

#define GPIO_AF1_7816M1		((uint8_t)0x00)  /* 7816M1 Alternate Function mapping */
#define GPIO_AF1_7816M2		((uint8_t)0x00)  /* 7816M2 Alternate Function mapping */
#define GPIO_AF1_7816M3		((uint8_t)0x00)  /* 7816M3 Alternate Function mapping */

#define GPIO_AF1_UART1		((uint8_t)0x00)  /* UART1 Alternate Function mapping */
#define GPIO_AF1_UART2		((uint8_t)0x00)  /* UART2 Alternate Function mapping */
#define GPIO_AF1_UART5		((uint8_t)0x00)  /* UART5 Alternate Function mapping */

#define GPIO_AF1_I2C		   ((uint8_t)0x00)  /* I2C Alternate Function mapping */

#define GPIO_AF1_PWM1		  ((uint8_t)0x00)  /* PWM1 Alternate Function mapping */
#define GPIO_AF1_PWM2		  ((uint8_t)0x00)  /* PWM2 Alternate Function mapping */

#define GPIO_AF1_MONITOR	   ((uint8_t)0x00)  /* MONITOR Alternate Function mapping */

/** 
  * @brief   AF 2 selection  
  */ 
#define GPIO_AF2_SPI1		  ((uint8_t)0x01)  /* SPI1 Alternate Function mapping */
#define GPIO_AF2_SPI2		  ((uint8_t)0x01)  /* SPI2 Alternate Function mapping */ 
#define GPIO_AF2_SPI3		  ((uint8_t)0x01)  /* SPI3 Alternate Function mapping */
 
#define GPIO_AF2_7816M1		((uint8_t)0x01)  /* 7816M1 Alternate Function mapping */
#define GPIO_AF2_7816M3		((uint8_t)0x01)  /* 7816M3 Alternate Function mapping */

 
/** 
  * @brief   AF 3 selection  
  */ 
#define GPIO_AF3_MONITOR	   ((uint8_t)0x02)  /* MONITOR Alternate Function mapping */
#define GPIO_AF3_SWD		   ((uint8_t)0x02)  /* SWD Alternate Function mapping */ 
#define GPIO_AF3_PWM3		  ((uint8_t)0x02)  /* PWM3 Alternate Function mapping */
#define GPIO_AF3_PWM4		  ((uint8_t)0x02)  /* PWM3 Alternate Function mapping */

#define GPIO_AF3_I2C		   ((uint8_t)0x02)  /* I2C Alternate Function mapping */
#define GPIO_AF3_7816M2		((uint8_t)0x02)  /* 7816M2 Alternate Function mapping */

#define GPIO_AF3_UART1		 ((uint8_t)0x02)  /* UART1 Alternate Function mapping */ 
#define GPIO_AF3_UART2		 ((uint8_t)0x02)  /* UART2 Alternate Function mapping */ 
#define GPIO_AF3_UART5		 ((uint8_t)0x02)  /* UART5 Alternate Function mapping */

#define GPIO_AF3_SPI1		  ((uint8_t)0x02)  /* SPI1 Alternate Function mapping */ 
/** 
  * @brief   AF 4 selection  
  */ 
#define GPIO_AF4_7816M1		((uint8_t)0x03)  /* 7816M1 Alternate Function mapping */

#define GPIO_AF4_UART1		 ((uint8_t)0x03)  /* UART1 Alternate Function mapping */ 
#define GPIO_AF4_UART2		 ((uint8_t)0x03)  /* UART2 Alternate Function mapping */ 
#define GPIO_AF4_UART5		 ((uint8_t)0x03)  /* UART5 Alternate Function mapping */

#define GPIO_AF4_I2C		   ((uint8_t)0x03)  /* I2C Alternate Function mapping */ 

/**
  * @}
  */
/** @defgroup Clock_Select_Define,
  * @{  
  */

/**
  * External Clock of BAT (32K),Not Be System Clock
  */
#define OSC_SOURCE_CLOCK_LSE	7
/**
  * Internal Clock of BAT (32K),Not Be System Clock
  */
#define OSC_SOURCE_CLOCK_LSI	6
/**
  * PLL_L Clock,Not Be System Clock 
  */
#define OSC_SOURCE_CLOCK_PLL_L  5

/**
  * USBPHY Clock  
  */
#define OSC_SOURCE_CLOCK_USBPHY 3
/**
  * PLL_H Clock  
  */
#define OSC_SOURCE_CLOCK_PLL_H  2
/**
  * XTAL Clock 
  */
#define OSC_SOURCE_CLOCK_HSE	1
/**
  * OSC Clock 
  */
#define OSC_SOURCE_CLOCK_HSI	0
/**
  * @}
  */  


/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/
/** @addtogroup GPIO_Exported_Functions
  * @{
  */

void GPIO_DeInit(GPIO_TypeDef* GPIOx);
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct);
void GPIO_DirectionConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t GPIO_Mode);
void GPIO_OdenConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState);
void GPIO_OutSpeedConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Mode);
void GPIO_PUPDConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Mode);
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_PinAFConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Mode);
void GPIO_InterruptModeConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, uint32_t Mode);
void GPIO_InterruptMskConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, FunctionalState NewState);
FlagStatus GPIO_GetITStatus(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);	
void GPIO_MonitorClkOut(uint8_t num,uint8_t source,uint8_t division);
/**
  * @}
  */ 
  
/**
  * @}
  */ 
  
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup GPIO_Private_Constants GPIO Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/** @defgroup GPIO_Private_Macros GPIO Private Macros
  * @{
  */
#define IS_GPIO_ALL_PERIPH(PERIPH)  (((PERIPH) == GPIOA) || \
									((PERIPH) == GPIOB) || \
									((PERIPH) == GPIOC) || \
									((PERIPH) == GPIOD) || \
									((PERIPH) == GPIOE))
									
#define IS_GPIO_MODE(MODE)		  (((MODE) == GPIO_MODE_IN)  || ((MODE) == GPIO_MODE_OUT) || \
									((MODE) == GPIO_MODE_AF))
									
  
#define IS_GPIO_SPEED(SPEED)		  (((SPEED) == GPIO_LOW_SPEED) || ((SPEED) == GPIO_MEDIUM_SPEED) || \
									((SPEED) == GPIO_FAST_SPEED)||  ((SPEED) == GPIO_HIGH_SPEED)) 
									
#define IS_GPIO_PUPD(PUPD)			 (((PUPD) == GPIO_NOPULL) || ((PUPD) == GPIO_PULLUP)|| ((PUPD) == GPIO_PULLDOWN))
#define IS_GPIO_OTYPE(OTYPE)			(((OTYPE) == GPIO_MODE_OUTPUT_PP) || ((OTYPE) == GPIO_MODE_OUTPUT_OD))
#define IS_GPIO_SMIT(SMIT)			(((SMIT) == GPIO_INPUTSCHMIT_DISABLE) || ((SMIT) == GPIO_INPUTSCHMIT_ENABLE))
 
#define IS_GPIO_Interrupt(Mode)		(((Mode) == GPIO_Low_Speed) || ((Mode) == GPIO_Medium_Speed) || \
									((Mode) == GPIO_Fast_Speed)||  ((Mode) == GPIO_High_Speed))
									
#define IS_GPIO_OUT(MODE)		   (((MODE) == GPIO_OUT_HIGH) || ((MODE) == GPIO_OUT_LOW))  
									
#define GPIO_PIN_MASK				  ((uint32_t)0x0000FFFF) 

#define IS_GPIO_PIN_SOURCE(PIN)	  (((PIN) == GPIO_PIN_Source0) || \
									((PIN) == GPIO_PIN_Source1)  || \
									((PIN) == GPIO_PIN_Source2)  || \
									((PIN) == GPIO_PIN_Source3)  || \
									((PIN) == GPIO_PIN_Source4)  || \
									((PIN) == GPIO_PIN_Source5)  || \
									((PIN) == GPIO_PIN_Source6)  || \
									((PIN) == GPIO_PIN_Source7)  || \
									((PIN) == GPIO_PIN_Source8)  || \
									((PIN) == GPIO_PIN_Source9)  || \
									((PIN) == GPIO_PIN_Source10) || \
									((PIN) == GPIO_PIN_Source11) || \
									((PIN) == GPIO_PIN_Source12) || \
									((PIN) == GPIO_PIN_Source13) || \
									((PIN) == GPIO_PIN_Source14) || \
									((PIN) == GPIO_PIN_Source15))

#define IS_GPIO_PIN(PIN)		 (((PIN) == GPIO_PIN_0) || \
									((PIN) == GPIO_PIN_1)  || \
									((PIN) == GPIO_PIN_2)  || \
									((PIN) == GPIO_PIN_3)  || \
									((PIN) == GPIO_PIN_4)  || \
									((PIN) == GPIO_PIN_5)  || \
									((PIN) == GPIO_PIN_6)  || \
									((PIN) == GPIO_PIN_7)  || \
									((PIN) == GPIO_PIN_8)  || \
									((PIN) == GPIO_PIN_9)  || \
									((PIN) == GPIO_PIN_10) || \
									((PIN) == GPIO_PIN_11) || \
									((PIN) == GPIO_PIN_12) || \
									((PIN) == GPIO_PIN_13) || \
									((PIN) == GPIO_PIN_14) || \
									((PIN) == GPIO_PIN_15))
									

									
#define IS_GPIO_AF(AF)			  (((AF) == GPIO_AF1_SPI1)	|| ((AF) == GPIO_AF1_SPI2)		 || \
									((AF) == GPIO_AF1_SPI3)	 || ((AF) == GPIO_AF1_SPI2_CLK_IN)  || \
									((AF) == GPIO_AF1_7816M1)   || ((AF) == GPIO_AF1_7816M2)	   || \
									((AF) == GPIO_AF1_7816M3)   || ((AF) == GPIO_AF1_UART1)		|| \
									((AF) == GPIO_AF1_UART2)	||									\
									((AF) == GPIO_AF1_I2C)	  ||									\
									((AF) == GPIO_AF1_PWM1)	 || ((AF) == GPIO_AF1_PWM2)		 || \
									((AF) == GPIO_AF1_MONITOR)										\
									((AF) == GPIO_AF2_SPI1)	 || ((AF) == GPIO_AF2_SPI2)		 || \
									((AF) == GPIO_AF2_SPI3)	 || ((AF) == GPIO_AF2_7816M1)	   || \
									((AF) == GPIO_AF2_7816M3)   ||									\
									((AF) == GPIO_AF3_MONITOR)  || ((AF) == GPIO_AF3_SPI1)								   \
									((AF) == GPIO_AF3_SWD)	  || ((AF) == GPIO_AF3_PWM3)			\
									((AF) == GPIO_AF3_I2C)	  || ((AF) == GPIO_AF3_PWM4)			\
									((AF) == GPIO_AF3_UART1)	|| ((AF) == GPIO_AF3_UART2)		|| \
									((AF) == GPIO_AF3_UART5))   ||									\
									((AF) == GPIO_AF4_7816M1)   ||									\
									((AF) == GPIO_AF4_UART1)	|| ((AF) == GPIO_AF4_UART2)		|| \
									((AF) == GPIO_AF4_UART5)	|| ((AF) == GPIO_AF4_I2C))
									
/**
  * @}
  */

/* Private functions ---------------------------------------------------------*/
/** @defgroup GPIO_Private_Functions GPIO Private Functions
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

#endif  /* _GPIO_H */ 

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

