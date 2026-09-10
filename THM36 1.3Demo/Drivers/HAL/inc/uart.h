/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: UART.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/
/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _UART_H
#define _UART_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @addtogroup UART 
  * @brief UART driver modules
  * @{
  */ 
/** @defgroup UART_Exported_Types UART Exported Types 
  * @{
  */
	 
/** 
  * @brief  UART Init Structure definition  
  */
   
typedef struct
{
 
	uint32_t BaudRate;			/*!< This member configures the UART communication baud rate.
									This parameter can be one of the following values:
									9600,14400,38400,56000,57600,115200,128000,256000,etc			*/		  
										  										   
	uint8_t StopBits;		 	/*!< Specifies the number of stop bits transmitted
									This parameter can be a value of @ref UART_stop_bits			*/		  
											
	uint32_t DataWidth;			/*!< This member configures the width of data.
									This parameter can be a value of @ref UART_data_width			*/		  
										  										   
	uint8_t ParityMode;			/*!< Specifies the parity mode.
									This parameter can be a value of @ref UART_parity_mode			*/			  		
} UART_InitTypeDef;	 


/**
  * @}
  */

/** @defgroup UART_Exported_Constants UART Exported Constants
  * @{
  */

/** @defgroup UART_stop_bits UART stop bits 
  * @{
  */

#define UART_STOP_BIT_1			0x00U

/**
  * @}
  */

/** @defgroup UART_data_width UART data width
  * @{
  */

#define UART_DATA_WIDTH_7			0x00000000UL
#define UART_DATA_WIDTH_8			0x00000001UL

/**
  * @}
  */

/** @defgroup UART_parity_mode UART parity mode
  * @{
  */
#define UART_PARITY_MODE_NONE		0x00000000UL 
#define UART_PARITY_MODE_EVEN		0x00000002UL
#define UART_PARITY_MODE_ODD		0x00000006UL

/**
  * @}
  */

/** @defgroup UART_flag_define RCC flag
  * @{  
  */
#define UART_FLAG_TXE				0x00000001UL
#define UART_FLAG_RXF				0x00000002UL
#define UART_FLAG_PE				0x00000004UL
#define UART_FLAG_OF				0x00000008UL
#define UART_FLAG_FE				0x00000010UL
/**
  * @}
  */

/** @defgroup UART_IT_define RCC IT
  * @{  
  */
#define UART_IT_TXE					0x00000001UL
#define UART_IT_RXF					0x00000002UL
#define UART_IT_PE					0x00000004UL
#define UART_IT_OF					0x00000008UL
#define UART_IT_FE					0x00000010UL
/**
  * @}
  */
  
/** @defgroup UART_DMA_control UART DMA control
  * @{  
  */
  
#define UART_DMA_RX					0x00000001UL
#define UART_DMA_TX					0x00000002UL

/**
  * @}
  */

/**
  * @}
  */

/** @addtogroup UART_Exported_Functions
  * @{  
  */

void UART_DeInit(UART_TypeDef * UARTx);
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct);
void UART_StructInit(UART_InitTypeDef* UART_InitStruct);
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint16_t Uart_Flag);
void UART_ClearFlagStatus(UART_TypeDef* UARTx,uint16_t Uart_Flag);
void UART_ITConfig(UART_TypeDef* UARTx, uint16_t Uart_Flag, FunctionalState NewState);
FlagStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t Uart_Flag);
void UART_ClearITStatus(UART_TypeDef* UARTx,uint16_t Uart_IT);
void UART_DMACmd(UART_TypeDef* UARTx, uint32_t DMA_Mode, FunctionalState NewState);
FlagStatus UART_GetDMAStatus(UART_TypeDef* UARTx, uint32_t DMA_Mode);
void UART_SendData(UART_TypeDef* UARTx, uint8_t Data);
uint8_t UART_ReceiveData(UART_TypeDef* UARTx);

  /**
  * @}
  */

/** @defgroup UART_Private_Macros UART Private Macros
  * @{
  */
								
#define IS_UART_Periph(PERIPH) 						(((PERIPH) == UART1_BASE_ADDR)|| \
													 ((PERIPH) == UART2_BASE_ADDR))
													 
#define IS_UART_STOPBIT(BIT) 						(((BIT) == UART_STOP_BIT_1)			|| \
													 ((BIT) == UART_STOP_BIT_1))
													 
#define IS_UART_DATAWIDTH(WIDTH) 					(((WIDTH) == UART_DATA_WIDTH_7)		|| \
													 ((WIDTH) == UART_DATA_WIDTH_8))
													 
#define IS_UART_PARITYMODE(MODE) 					(((MODE) == UART_PARITY_MODE_EVEN)	|| \
													 ((MODE) == UART_PARITY_MODE_NONE)  || \
													 ((MODE) == UART_PARITY_MODE_ODD))

#define IS_UART_STATUS(STATUS) 						((STATUS & (~(UART_FLAG_TXE | UART_FLAG_RXF | UART_FLAG_PE | UART_FLAG_OF | UART_FLAG_FE))) == 0)

#define IS_UART_DAM_MODE(MODE) 						(((MODE) == UART_DMA_RX)		|| \
													 ((MODE) == UART_DMA_TX))

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
