/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: spi.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPI_H
#define _SPI_H

#ifdef __cplusplus
 extern "C" {
#endif
/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup SPI
  * @brief SPI HAL modules driver 
  * @{
  */ 
/** @defgroup SPI_Exported_Constants SPI Exported Constants
  * @{
  */
/** 
  * @brief SQI_IO_Out_Put_Mode SQI IO Out Put Mode
  * @note   This sturcture is used with  SPI   
  */
typedef enum {Input = 0, Output = !Input} SQI_IO_Mode;

/** 
  * @brief  SPI Init structure definition
  * @note   This sturcture is used with  SPI   
  */	 
typedef struct
{
  uint32_t Mode; 					/*!< SPIx Set Master or Slave mode,This parameter can be a value of @ref Master_Slave_Mode*/		  
  uint32_t Master_EGT;	
  uint32_t BaudRatePrescaler;		/*!< Specifies the Baud Rate prescaler value which will be
									used to configure the transmit and receive SCK clock.
									This parameter can be a value of @ref SPI_BaudRate_Prescaler
									@note The communication clock is derived from the master
									 clock. The slave clock does not need to be set. */
  uint32_t CLKPolarity;				/*!< Specifies the serial clock steady state.
									This parameter can be a value of @ref SPI_Clock_Polarity */
  uint32_t CLKPhase;				/*!< Specifies the clock active edge for the bit capture.
									This parameter can be a value of @ref SPI_Clock_Phase */
  uint32_t SQI_Select; 				/*!< the mode of SPIx,This parameter can be a value of @ref SPI_SQI_select*/	
} SPI_InitTypeDef;

/** @defgroup SPI_mode SPI mode
  * @brief   
  * @{
  */ 
#define SPI_MASTER_MODE							(0x00000040UL)		/*!< SPIx mode is Master */
#define SPI_SLAVE_MODE							(0x00000000UL)		/*!< SPIx mode is Slave  */ 

/**
  * @}
  */

/** @defgroup SPI_mode SPI mode
  * @brief   
  * @{
  */ 
#define SPI_RX_DMA								(0x00000001UL)		/*!< SPIx mode is Master */
#define SPI_TX_DMA								(0x00000002UL)		/*!< SPIx mode is Slave  */ 

/**
  * @}
  */
  
/** @defgroup SPI_BaudRate_Prescaler SPI BaudRate Prescaler
  * @{
  */
#define SPI_BAUDRATEPRESCALER_2		 (0x00000000UL)
#define SPI_BAUDRATEPRESCALER_4		 (0x00000004UL)
#define SPI_BAUDRATEPRESCALER_8		 (0x00000008UL)
#define SPI_BAUDRATEPRESCALER_16		(0x0000000CUL)
#define SPI_BAUDRATEPRESCALER_32		(0x00000010UL)
#define SPI_BAUDRATEPRESCALER_64		(0x00000014UL)
#define SPI_BAUDRATEPRESCALER_128	   (0x00000018UL)
#define SPI_BAUDRATEPRESCALER_256	   (0x0000001CUL)
/**
  * @}
  */

/** @defgroup SPI_Clock_Polarity SPI Clock Polarity
  * @{
  */
#define SPI_POLARITY_LOW				(0x00000000UL)
#define SPI_POLARITY_HIGH			   (0x00000001UL)
/**
  * @}
  */

/** @defgroup SPI_Clock_Phase SPI Clock Phase
  * @{
  */
#define SPI_PHASE_1EDGE				 (0x00000000UL)
#define SPI_PHASE_2EDGE				 (0x00000002UL)
/**
  * @}
  */

/** @defgroup SPI_SQI_select SPI SQI select
  * @brief   
  * @{
  */ 
#define SQI_MODE						(0x00000003UL)		/*!< SQIx mode */  
#define SPI_MODE						(0x00000000UL)		/*!< SPIx mode */  
/**
  * @}
  */
  
/** @defgroup SPI_ENABLE
  * @brief   
  * @{
  */ 
#define SPI_ENABLE						(0x00000080UL)		/*!< SPIx ENABLE */  
#define SPI_DISABLE						(~(0x00000080UL))	/*!< SPIx DISABLE */  
/**
  * @}
  */
  

/** @defgroup SPI_error_flag SPI error flag
  * @brief   
  * @{
  */ 
#define SPI_BOVER_FLAG							(0x00000001UL)
#define SPI_TBE_FLAG							(0x00000004UL)
/**
  * @}
  */ 
/** @defgroup SPI_IT_flag SPI IT flag
  * @brief   
  * @{
  */ 
#define SPI_BOVER_IT							(0x00000001UL)
#define SPI_TBE_IT								(0x00000004UL)
/**
  * @}
  */ 
/** @defgroup SPI_DMA_mode SPI DMA mode
  * @brief   
  * @{
  */ 
#define SPIx_ENABLE_TX_DMA						(0x00000002UL)
#define SPIx_DISABLE_TX_DMA						(~(0x00000002UL))
#define SPIx_ENABLE_RX_DMA						(0x00000001UL)
#define SPIx_DISABLE_RX_DMA						(~(0x00000001UL)) 
/**
  * @}
  */ 

/** @defgroup SPI_private_macros SPI private macros
  * @{
  */ 
#define IS_SPI_CHECK_MODE(mode)					((mode == SPI_MASTER_MODE)||(mode == SPI_SLAVE_MODE))	
#define IS_SPI_CHECK_EGT(EGT)					((EGT<=255)) 
#define IS_SPI_CHECK_OUT_CLOCK(CLK)				((CLK == SPI_BAUDRATEPRESCALER_2)||(CLK == SPI_BAUDRATEPRESCALER_4) || \
												(CLK == SPI_BAUDRATEPRESCALER_8)||(CLK == SPI_BAUDRATEPRESCALER_16) ||	\
												(CLK == SPI_BAUDRATEPRESCALER_32)||(CLK == SPI_BAUDRATEPRESCALER_64) || \
												(CLK == SPI_BAUDRATEPRESCALER_128)|| (CLK == SPI_BAUDRATEPRESCALER_256))
												
#define IS_SPI_CHECK_CPHA(CPHA)					((CPHA == SPI_PHASE_1EDGE) || (CPHA == SPI_PHASE_2EDGE))	
#define IS_SPI_CHECK_CPOL(CPOL)					((CPOL == SPI_POLARITY_LOW) || (CPOL == SPI_POLARITY_HIGH))
#define IS_CHECK_MODE(mode)						((mode == SQI_MODE) || (mode == SPI_MODE))

/**
  * @}
  */
/** @addtogroup SPI_Exported_Function 
  * @{  
  */   
void SPIx_StructInit(SPI_InitTypeDef* SPI_InitStruct);
void SPIx_Init(SPI_TypeDef* SPIx,SPI_InitTypeDef* SPI_InitStruct);
void SPIx_DeInit(SPI_TypeDef* SPIx);
void SPIx_SendData(SPI_TypeDef* SPIx,uint8_t Data);
uint8_t SPIx_ReceiveData(SPI_TypeDef* SPIx);
FlagStatus SPIx_GetStatus(SPI_TypeDef* SPIx,uint32_t Flag);
void SPIx_Cmd(SPI_TypeDef* SPIx,FunctionalState NewState);
void SPIx_ClearBoverStatus(SPI_TypeDef* SPIx);
void SPIx_ITConfig(SPI_TypeDef* SPIx,uint32_t Flag,FunctionalState NewState);
void SPIx_ConfigDMA(SPI_TypeDef* SPIx,uint8_t Mode,FunctionalState NewState);
FlagStatus SPIx_GetTxDMAStatus(SPI_TypeDef* SPIx);
FlagStatus SPIx_GetRxDMAStatus(SPI_TypeDef* SPIx);
void SPIx_ConfigSQI_IO(SPI_TypeDef* SPIx,SQI_IO_Mode mode);
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
#endif /* __TMC_SPI_H */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/

