/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: dma.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DMA_H
#define _DMA_H

#ifdef __cplusplus
 extern "C" {
#endif
	 
/** @addtogroup thm36_1.3
  * @{
  */	 
	 
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @addtogroup DMA 
  * @{
  */ 
	 
/** @defgroup DMA_Exported_Types DMA Exported Types
  * @brief	DMA Exported Types 
  * @{
  */
   
/** 
  * @brief  DMA Configuration Structure definition
  */
typedef struct
{
	uint32_t RequestSelect;		/*!< Specifies the channel used for the specified stream. 
									  This parameter can be a value of @ref DMA_Request_selection					*/

	uint32_t Direction;			/*!< Specifies if the data will be transferred from memory to peripheral, 
									  from memory to memory or from peripheral to memory.
									  This parameter can be a value of @ref DMA_Data_transfer_direction				*/

	uint32_t DestInc;			  /*!< Specifies whether the Peripheral address register should be incremented or not.
									  This parameter can be a value of @ref DMA_Dest_incremented_mode				*/

	uint32_t SrcInc;			   /*!< Specifies whether the memory address register should be incremented or not.
									  This parameter can be a value of @ref DMA_Src_incremented_mode				*/

	uint32_t DestDataAlignment;	/*!< Specifies the Peripheral data width.
									  This parameter can be a value of @ref DMA_Dest_data_size						*/

	uint32_t SrcDataAlignment;	 /*!< Specifies the Memory data width.
									  This parameter can be a value of @ref DMA_Src_data_size						*/

	uint32_t DestBurst;			/*!< Specifies the destination Burst size. 
									  It specifies the amount of data to be transferred in a single non interruptible
									  transaction.
									  This parameter can be a value of @ref DMA_Dest_burst							*/

	uint32_t SrcBurst;			 /*!< Specifies the source Burst size. 
									  It specifies the amount of data to be transferred in a single non interruptible 
									  transaction. 
									  This parameter can be a value of @ref DMA_Src_burst							*/
	uint32_t IT_Enable;			 /*!< Set up Wether this Time DMA move make a interrupt£¬if DMA no LLI£¬this is not Last	time£¬set ENABLE¡£
										 if DMA have LLI£¬this is not Last time£¬set Disable
									*/
	uint32_t SrcAHBMx_Select;		/*!< SrcAHBMx Select  This parameter can be a value of @ref DMA_AHB_SELECT	
									*/
	uint32_t DecAHBMx_Select;		/*!< SrcAHBMx Select  This parameter can be a value of @ref DMA_AHB_SELECT	
									*/
}DMA_InitTypeDef;



/**
  * @}
  */


/* Exported constants --------------------------------------------------------*/

/** @defgroup DMA_Exported_Constants DMA Exported Constants
  * @brief	DMA Exported constants 
  * @{
  */

/** @defgroup DMA_IT_Enable DMA IT Enable
  * @{
  */ 
#define DMA_IT_DISABLE			0x00000000UL	/*!< this time DMA Move Over not make a interrupt*/
#define DMA_IT_ENABLE			(1UL << 31)		/*!< DMA is controller, Memory to peripheral direction */

/**
  * @}
  */

/** @defgroup DMA_AHB_SELECT DMA AHB SELECT
  * @{
  */ 
#define DMA_AHBM1_SELECT		0x00000000UL
#define DMA_AHBM2_SELECT		0x00000001UL	

/**
  * @}
  */
  
/** @defgroup DMA_Request_selection DMA Request selection
  * @{
  */ 

#define DMA_REQUEST_SPI1_RX					0x00000000UL 			/*!< DMA request: SPI1 receive  */
#define DMA_REQUEST_SPI1_TX					0x00000001UL			/*!< DMA request: SPI1 send  */
#define DMA_REQUEST_SPI3_RX 				0x00000002UL			/*!< DMA request: SPI3 receive  */
#define DMA_REQUEST_SPI3_TX					0x00000003UL			/*!< DMA request: SPI3 send  */
#define DMA_REQUEST_SPI2 					0x00000004UL			/*!< DMA request: SPI2  */
#define DMA_REQUEST_UART1_RX				0x00000005UL			/*!< DMA request: UART1 receive  */
#define DMA_REQUEST_UART1_TX				0x00000006UL			/*!< DMA request: UART1 send  */
#define DMA_REQUEST_UART2_RX				0x00000007UL			/*!< DMA request: UART2 receive  */
#define DMA_REQUEST_UART2_TX				0x00000008UL			/*!< DMA request: UART2 send  */
#define DMA_REQUEST_7816M1					0x0000000BUL			/*!< DMA request: ISO7816 master1 receive or send */
#define DMA_REQUEST_7816M2					0x0000000BUL			/*!< DMA request: ISO7816 master2 receive or send  */
#define DMA_REQUEST_7816M3					0x0000000BUL			/*!< DMA request: ISO7816 master3 receive or send  */
#define DMA_REQUEST_ADC1					0x0000000DUL			/*!< DMA request: ADC1 */
#define DMA_REQUEST_UART5_RX				0x0000000DUL			/*!< DMA request: UART5 receive  */
#define DMA_REQUEST_ADC2					0x0000000EUL			/*!< DMA request: ADC2  */
#define DMA_REQUEST_UART5_TX				0x0000000EUL			/*!< DMA request: UART5 receive  */
#define DMA_REQUEST_ADC3					0x0000000FUL			/*!< DMA request: ADC3  */

/**
  * @}
  */


/** @defgroup DMA_Data_transfer_direction DMA Data transfer direction
  * @{
  */ 
#define DMA_CTRL_MEMORY_TO_MEMORY			0x00000000UL				/*!< DMA is controller, Memory to memory direction */
#define DMA_CTRL_MEMORY_TO_PERIPH			(0x00000001UL << 11)		/*!< DMA is controller, Memory to peripheral direction */
#define DMA_CTRL_PERIPH_TO_MEMORY			(0x00000002UL << 11)		/*!< DMA is controller, Peripheral to memory direction */
#define PERIPH_CTRL_MEMORY_TO_PERIPH 		(0x00000005UL << 11)		/*!< Peripheral is controller, Memory to peripheral direction */
#define PERIPH_CTRL_PERIPH_TO_MEMORY 		(0x00000006UL << 11)		/*!< Peripheral is controller, Peripheral to memory direction */


/**
  * @}
  */

/** @defgroup DMA_Data_transfer_direction DMA Data transfer direction
  * @{
  */ 
#define DMA_CTRL_MEMORY_TO_MEMORY			0x00000000UL				/*!< DMA is controller, Memory to memory direction */
#define DMA_CTRL_MEMORY_TO_PERIPH			(0x00000001UL << 11)		/*!< DMA is controller, Memory to peripheral direction */
#define DMA_CTRL_PERIPH_TO_MEMORY			(0x00000002UL << 11)		/*!< DMA is controller, Peripheral to memory direction */
#define PERIPH_CTRL_MEMORY_TO_PERIPH 		(0x00000005UL << 11)		/*!< Peripheral is controller, Memory to peripheral direction */
#define PERIPH_CTRL_PERIPH_TO_MEMORY 		(0x00000006UL << 11)		/*!< Peripheral is controller, Peripheral to memory direction */


/**
  * @}
  */


/** @defgroup DMA_Dest_incremented_mode DMA destination incremented mode
  * @{
  */ 
#define DMA_DEST_INC_ENABLE					(0x00000001UL << 27)		 /*!< destination increment mode enable  */
#define DMA_DEST_INC_DISABLE				0x00000000UL				 /*!< destination increment mode disable */

/**
  * @}
  */ 


/** @defgroup DMA_Src_incremented_mode DMA source incremented mode
  * @{
  */ 
#define DMA_SRC_INC_ENABLE					(0x00000001UL << 26)		 /*!< source increment mode enable  */
#define DMA_SRC_INC_DISABLE					0x00000000UL				 /*!< source increment mode disable */

/**
  * @}
  */

/** @defgroup DMA_Endianness_transform_mode DMA Endianness transform mode
  * @{
  */
#define DMA_ENDIAN_TRANS_ENABLE				0x00000002UL		 /*!< DMA Endianness transform modeenable  */
#define DMA_ENDIAN_TRANS_DISABLE			0x00000000UL		 /*!< DMA Endianness transform mode disable */
  

/**
  * @}
  */

/** @defgroup DMA_Dest_data_size DMA Destination data size
  * @{
  */ 
#define DMA_DEST_ALIGN_BYTE					0x00000000UL				  /*!< Destination data alignment: Byte	 */
#define DMA_DEST_ALIGN_HALFWORD				(0x00000001UL << 21)		  /*!< Destination data alignment: HalfWord */
#define DMA_DEST_ALIGN_WORD					(0x00000002UL << 21)		  /*!< Destination data alignment: Word	 */
 
/**
  * @}
  */ 

/** @defgroup DMA_Src_data_size DMA source data size
  * @{ 
  */
#define DMA_SRC_ALIGN_BYTE					0x00000000UL				  /*!< source data alignment: Byte	 */
#define DMA_SRC_ALIGN_HALFWORD				(0x00000001UL << 18)		  /*!< source data alignment: HalfWord */
#define DMA_SRC_ALIGN_WORD					(0x00000002UL << 18)		  /*!< source data alignment: Word	 */

/**
  * @}
  */



/** @defgroup DMA_Dest_burst DMA destination burst
  * @{
  */ 
#define DMA_DEST_BURST_INC1					0x00000000UL
#define DMA_DEST_BURST_INC4					(0x00000001UL << 15)  
#define DMA_DEST_BURST_INC8					(0x00000002UL << 15)  
#define DMA_DEST_BURST_INC16				(0x00000003UL << 15)  
#define DMA_DEST_BURST_INC32				(0x00000004UL << 15) 
#define DMA_DEST_BURST_INC64				(0x00000005UL << 15) 
#define DMA_DEST_BURST_INC128				(0x00000006UL << 15) 
#define DMA_DEST_BURST_INC256				(0x00000007UL << 15) 

/**
  * @}
  */ 

/** @defgroup DMA_Src_burst DMA source burst
  * @{
  */ 
#define DMA_SRC_BURST_INC1					0x00000000UL
#define DMA_SRC_BURST_INC4					(0x00000001UL << 12)  
#define DMA_SRC_BURST_INC8					(0x00000002UL << 12)
#define DMA_SRC_BURST_INC16					(0x00000003UL << 12)
#define DMA_SRC_BURST_INC32					(0x00000004UL << 12) 
#define DMA_SRC_BURST_INC64					(0x00000005UL << 12) 
#define DMA_SRC_BURST_INC128				(0x00000006UL << 12) 
#define DMA_SRC_BURST_INC256				(0x00000007UL << 12) 
 

/**
  * @}
  */
  
  

/** @defgroup DMA_interrupts_definition  DMA interrupts definition   
  * @{
  */

#define DMA_IT_TC							(0x00000001UL << 15)
#define DMA_IT_ERR							(0x00000001UL << 14)

#define DMA_IT_TC0							(0x00000001UL)
#define DMA_IT_TC1							(0x00000002UL)
#define DMA_IT_TC2							(0x00000004UL)
#define DMA_IT_TC3							(0x00000008UL)
#define DMA_IT_TC4							(0x00000010UL)
#define DMA_IT_TC5							(0x00000020UL)
#define DMA_IT_TC6							(0x00000040UL)
#define DMA_IT_TC7							(0x00000080UL)
#define DMA_IT_ERR0							(0x00000100UL)
#define DMA_IT_ERR1							(0x00000200UL)
#define DMA_IT_ERR2							(0x00000400UL)
#define DMA_IT_ERR3							(0x00000800UL)
#define DMA_IT_ERR4							(0x00001000UL)
#define DMA_IT_ERR5							(0x00002000UL)
#define DMA_IT_ERR6							(0x00004000UL)
#define DMA_IT_ERR7							(0x00008000UL)
#define DMA_IT_GL0							(0x00010000UL)
#define DMA_IT_GL1							(0x00020000UL)
#define DMA_IT_GL2							(0x00040000UL)
#define DMA_IT_GL3							(0x00080000UL)
#define DMA_IT_GL4							(0x00100000UL)
#define DMA_IT_GL5							(0x00200000UL)
#define DMA_IT_GL6							(0x00400000UL)
#define DMA_IT_GL7							(0x00800000UL)
 
/**
  * @}
  */


/** @defgroup DMA_flags_definition DMA flags definition
  * @{
  */
#define DMA_FLAG_TC0						(0x00000001UL)
#define DMA_FLAG_TC1						(0x00000002UL)
#define DMA_FLAG_TC2						(0x00000004UL)
#define DMA_FLAG_TC3						(0x00000008UL)
#define DMA_FLAG_TC4						(0x00000010UL)
#define DMA_FLAG_TC5						(0x00000020UL)
#define DMA_FLAG_TC6						(0x00000040UL)
#define DMA_FLAG_TC7						(0x00000080UL)
#define DMA_FLAG_ERR0						(0x00000100UL)
#define DMA_FLAG_ERR1						(0x00000200UL)
#define DMA_FLAG_ERR2						(0x00000400UL)
#define DMA_FLAG_ERR3						(0x00000800UL)
#define DMA_FLAG_ERR4						(0x00001000UL)
#define DMA_FLAG_ERR5						(0x00002000UL)
#define DMA_FLAG_ERR6						(0x00004000UL)
#define DMA_FLAG_ERR7						(0x00008000UL)

/**
  * @}
  */										 
/**
  * @}
  */										 
/* Exported functions --------------------------------------------------------*/
/** @addtogroup DMA_Exported_Functions
  * @{
  */
void DMA_DeInit(DMA_Channel_TypeDef * DMA_Channelx);
void DMA_Init(DMA_Channel_TypeDef* DMA_Channelx, DMA_InitTypeDef* DMA_InitStruct);
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct);
void DMA_SetAddress(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DstAddress, uint32_t SrcAddress, uint32_t DataLength);
void DMA_SetLength(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DataLength);
void DMA_Cmd(DMA_Channel_TypeDef* DMA_Channelx, FunctionalState NewStatus);
void DMA_SetChainListAddress(DMA_Channel_TypeDef* DMA_Channelx, uint32_t LLIAddress);
void DMA_SetEndianTransform(uint32_t EndiannessTrans);
void DMA_ITConfig(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DMA_IT, FunctionalState NewState);
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMA_Channelx);
FlagStatus DMA_GetFlagStatus(uint32_t DMA_Flag);
void DMA_ClearFlag(uint32_t DMA_FLAG);
ITStatus DMA_GetITStatus(uint32_t DMA_IT);
void DMA_ClearITPendingBit(uint32_t DMA_IT);
FlagStatus DMA_GetCmdStatus(DMA_Channel_TypeDef* DMA_Channelx);
  
/**
  * @}
  */
  
/* Private macros ------------------------------------------------------------*/
/** @defgroup DMA_Private_Macros DMA Private Macros
  * @{
  */
#define IS_DMA_GET_FLAG(FLAG)			(((FLAG) == DMA_FLAG_TC0)	|| \
										 ((FLAG) == DMA_FLAG_TC1)	|| \
										 ((FLAG) == DMA_FLAG_TC2)	|| \
										 ((FLAG) == DMA_FLAG_TC3)	|| \
										 ((FLAG) == DMA_FLAG_TC4)	|| \
										 ((FLAG) == DMA_FLAG_TC5)	|| \
										 ((FLAG) == DMA_FLAG_TC6)	|| \
										 ((FLAG) == DMA_FLAG_TC7)	|| \
										 ((FLAG) == DMA_FLAG_ERR0)	|| \
										 ((FLAG) == DMA_FLAG_ERR1)	|| \
										 ((FLAG) == DMA_FLAG_ERR2)	|| \
										 ((FLAG) == DMA_FLAG_ERR3)	|| \
										 ((FLAG) == DMA_FLAG_ERR4)	|| \
										 ((FLAG) == DMA_FLAG_ERR5)	|| \
										 ((FLAG) == DMA_FLAG_ERR6)	|| \
										 ((FLAG) == DMA_FLAG_ERR7))
										 
#define IS_DMA_CLEAR_IT(IT)				(((IT) == DMA_IT_TC0) || ((IT) == DMA_IT_ERR0) || \
										 ((IT) == DMA_IT_TC1) || ((IT) == DMA_IT_ERR1) || \
										 ((IT) == DMA_IT_TC2) || ((IT) == DMA_IT_ERR2) || \
										 ((IT) == DMA_IT_TC3) || ((IT) == DMA_IT_ERR3) || \
										 ((IT) == DMA_IT_TC4) || ((IT) == DMA_IT_ERR4) || \
										 ((IT) == DMA_IT_TC5) || ((IT) == DMA_IT_ERR5) || \
										 ((IT) == DMA_IT_TC6) || ((IT) == DMA_IT_ERR6) || \
										 ((IT) == DMA_IT_TC7) || ((IT) == DMA_IT_ERR7))

#define IS_DMA_GET_IT(IT)				(((IT) == DMA_IT_GL0) || ((IT) == DMA_IT_TC0) || ((IT) == DMA_IT_ERR0) || \
										 ((IT) == DMA_IT_GL1) || ((IT) == DMA_IT_TC1) || ((IT) == DMA_IT_ERR1) || \
										 ((IT) == DMA_IT_GL2) || ((IT) == DMA_IT_TC2) || ((IT) == DMA_IT_ERR2) || \
										 ((IT) == DMA_IT_GL3) || ((IT) == DMA_IT_TC3) || ((IT) == DMA_IT_ERR3) || \
										 ((IT) == DMA_IT_GL4) || ((IT) == DMA_IT_TC4) || ((IT) == DMA_IT_ERR4) || \
										 ((IT) == DMA_IT_GL5) || ((IT) == DMA_IT_TC5) || ((IT) == DMA_IT_ERR5) || \
										 ((IT) == DMA_IT_GL6) || ((IT) == DMA_IT_TC6) || ((IT) == DMA_IT_ERR6) || \
										 ((IT) == DMA_IT_GL7) || ((IT) == DMA_IT_TC7) || ((IT) == DMA_IT_ERR7))
						   
#define IS_DMA_CONFIG_IT(IT)			(((IT) == DMA_IT_TC) || \
										 ((IT) == DMA_IT_ERR))

#define IS_DMA_SRC_BURST(STATE)			(((STATE) == 0)	|| \
										 ((STATE) == DMA_SRC_BURST_INC4)	|| \
										 ((STATE) == DMA_SRC_BURST_INC8)	|| \
										 ((STATE) == DMA_SRC_BURST_INC16)	|| \
										 ((STATE) == DMA_SRC_BURST_INC32)	|| \
										 ((STATE) == DMA_SRC_BURST_INC64)	|| \
										 ((STATE) == DMA_SRC_BURST_INC128)	|| \
										 ((STATE) == DMA_SRC_BURST_INC256)) 
									   
#define IS_DMA_DEST_BURST(STATE)		(((STATE) == 0)	|| \
										 ((STATE) == DMA_DEST_BURST_INC4)	|| \
										 ((STATE) == DMA_DEST_BURST_INC8)	|| \
										 ((STATE) == DMA_DEST_BURST_INC16)	|| \
										 ((STATE) == DMA_DEST_BURST_INC32)	|| \
										 ((STATE) == DMA_DEST_BURST_INC64)	|| \
										 ((STATE) == DMA_DEST_BURST_INC128)	|| \
										 ((STATE) == DMA_DEST_BURST_INC256))  									   
						   
#define IS_DMA_SRC_ALIGN(STATE)			(((STATE) == DMA_SRC_ALIGN_BYTE)		|| \
										 ((STATE) == DMA_SRC_ALIGN_HALFWORD)	|| \
										 ((STATE) == DMA_SRC_ALIGN_WORD))  

#define IS_DMA_DEST_ALIGN(STATE)		(((STATE) == DMA_DEST_ALIGN_BYTE)		|| \
										 ((STATE) == DMA_DEST_ALIGN_HALFWORD)	|| \
										 ((STATE) == DMA_DEST_ALIGN_WORD))

#define IS_DMA_ENDIAN_TRANS_STATE(STATE)	(((STATE) == DMA_ENDIAN_TRANS_ENABLE)	|| \
											 ((STATE) == DMA_ENDIAN_TRANS_DISABLE))
											   											   
#define IS_DMA_SRC_INC_STATE(STATE)		(((STATE) == DMA_SRC_INC_ENABLE) || \
										 ((STATE) == DMA_SRC_INC_DISABLE))											   
											   
#define IS_DMA_DEST_INC_STATE(STATE)	(((STATE) == DMA_DEST_INC_ENABLE) || \
										 ((STATE) == DMA_DEST_INC_DISABLE))											   
											   
#define IS_DMA_DIR(DIR)					(((DIR) == DMA_CTRL_MEMORY_TO_MEMORY)		|| \
										 ((DIR) == DMA_CTRL_MEMORY_TO_PERIPH)		|| \
										 ((DIR) == DMA_CTRL_PERIPH_TO_MEMORY)		|| \
										 ((DIR) == PERIPH_CTRL_MEMORY_TO_PERIPH)	|| \
										 ((DIR) == PERIPH_CTRL_PERIPH_TO_MEMORY))

#define IS_DMA_ALL_REQUEST(PERIPH)		(((PERIPH) == DMA_REQUEST_SPI1_RX)	|| \
										 ((PERIPH) == DMA_REQUEST_SPI1_TX)	|| \
										 ((PERIPH) == DMA_REQUEST_SPI3_RX)	|| \
										 ((PERIPH) == DMA_REQUEST_SPI3_TX)	|| \
										 ((PERIPH) == DMA_REQUEST_SPI2)		|| \
										 ((PERIPH) == DMA_REQUEST_UART1_RX)	|| \
										 ((PERIPH) == DMA_REQUEST_UART1_TX)	|| \
										 ((PERIPH) == DMA_REQUEST_UART2_RX)	|| \
										 ((PERIPH) == DMA_REQUEST_UART2_TX)	|| \
										 ((PERIPH) == DMA_REQUEST_7816M1)	|| \
										 ((PERIPH) == DMA_REQUEST_7816M2)	|| \
										 ((PERIPH) == DMA_REQUEST_7816M3)	|| \
										 ((PERIPH) == DMA_REQUEST_ADC1)		|| \
										 ((PERIPH) == DMA_REQUEST_UART5_RX)	|| \
										 ((PERIPH) == DMA_REQUEST_ADC2)		|| \
										 ((PERIPH) == DMA_REQUEST_UART5_TX)	|| \
										 ((PERIPH) == DMA_REQUEST_ADC3))



#define IS_DMA_ALL_PERIPH(PERIPH)		(((PERIPH) == DMA_Channel1)	|| \
										 ((PERIPH) == DMA_Channel2)	|| \
										 ((PERIPH) == DMA_Channel3)	|| \
										 ((PERIPH) == DMA_Channel4)	|| \
										 ((PERIPH) == DMA_Channel5)	|| \
										 ((PERIPH) == DMA_Channel6)	|| \
										 ((PERIPH) == DMA_Channel7)	|| \
										 ((PERIPH) == DMA_Channel0))

#define IS_DMA_IT_MASK(PERIPH)			(((PERIPH) == DMA_IT_DISABLE)|| \
										 ((PERIPH) == DMA_IT_ENABLE))
										 


void DMA_DemoMain(void);


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
#endif 

/************************ (C) COPYRIGHT TMC *****END OF FILE***********************/
