/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart5.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/
#ifndef _UART5_H
#define _UART5_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "tmc_rcc.h"

typedef struct
{
	uint32_t ClockPrescaler;			/*!< Specifies the prescaler value used to divide the UART clock source.
										   This parameter can be a value of @ref UART_ClockPrescaler. */
	uint32_t BaudRate;			/*!< This member configures the UART communication baud rate.*/	
	uint32_t WordLength;		  /*!< Specifies the number of data bits transmitted or received in a frame.*/
	uint32_t StopBits;			/*!< Specifies the number of stop bits transmitted.*/
	uint32_t Parity;			  /*!< Specifies the parity mode.*/
	uint32_t Mode;				/*!< Specifies wether the Receive or Transmit mode is enabled or disabled.*/
	uint32_t HardwareFlowControl; /*!< Specifies wether the hardware flow control mode is enabled or disabled.*/	
	uint32_t FIFOMode;		 /*!< Specifies if the FIFO mode will be used. */
	uint32_t TXFIFOThreshold;  /*!< Specifies the TXFIFO threshold level.*/
	uint32_t RXFIFOThreshold;  /*!< Specifies the RXFIFO threshold level.*/
	uint32_t TxRx_Swap;		/*!< Specifies whether TX and RX pins are swapped.*/	
} UART5_InitTypeDef;

/**
  * @brief  HAL UART Error Code structure definition
  */
typedef enum
{
  UART5_ERROR_NONE	  = 0x00U,	/*!< No error			*/
  UART5_ERROR_PE		= 0x01U,	/*!< Parity error		*/
  UART5_ERROR_NE		= 0x02U,	/*!< Noise error		 */
  UART5_ERROR_FE		= 0x04U,	/*!< frame error		 */
  UART5_ERROR_ORE	   = 0x08U,	/*!< Overrun error	   */
  UART5_ERROR_DMA	   = 0x10U	 /*!< DMA transfer error  */
}UART5_ErrorTypeDef;

#define IS_UART5_PERIPH(PERIPH)  ((PERIPH) == UART5)
/** @defgroup UART_Stop_Bits 
  * @{
  */ 
  
#define UART5_StopBits_1					 (0x0U << 12) //((uint16_t)0x0000)
#define UART5_StopBits_2					 (0x2U << 12) //((uint16_t)0x2000)
#define CR2_STOP_CLEAR_Mask				  ((uint32_t)0xFFFFCFFF) /*Clear CR2 bit12、bit13*/
#define IS_UART5_STOPBITS(STOPBITS) (((STOPBITS) == UART5_StopBits_1) || \
									 ((STOPBITS) == UART5_StopBits_2))	 
/** @defgroup UART_Word_Length 
  * @{
  */ 
#define UART5_WordLength_7b				  ((0x1U << 28)|(0x0U << 12))
#define UART5_WordLength_8b				  ((0x0U << 28)|(0x0U << 12))
#define UART5_WordLength_9b				  ((0x0U << 28)|(0x1U << 12))
									
#define IS_UART5_WORD_LENGTH(LENGTH) (((LENGTH) == UART5_WordLength_7b) || \
									  ((LENGTH) == UART5_WordLength_8b) || \
									  ((LENGTH) == UART5_WordLength_9b))									 
/**
  * @}
  */ 

/** @defgroup UART_Parity 
  * @{
  */ 
  
#define UART5_Parity_None					((0x0U << 10)|(0x0U << 9))
#define UART5_Parity_Even					((0x1U << 10)|(0x0U << 9))
#define UART5_Parity_Odd					 ((0x1U << 10)|(0x1U << 9))
#define IS_UART5_PARITY(PARITY) (((PARITY) == UART5_Parity_None) || \
								 ((PARITY) == UART5_Parity_Even) || \
								 ((PARITY) == UART5_Parity_Odd))									 
									 
/**
  * @}
  */ 

/** @defgroup UART_Mode 
  * @{
  */ 
  
#define UART5_Mode_Rx						(0x1U << 2)
#define UART5_Mode_Tx						(0x1U << 3)
#define IS_UART5_MODE(MODE) ((((MODE) & (uint16_t)0xFFF3) == 0x00) && ((MODE) != (uint16_t)0x00))

#define UART5_CR1_CLEAR_Mask			((uint16_t)0xEFFFE9F3)  /*Clear CR1 bit2、bit3、bit9、bit10、bit12、bit28*/
/**
  * @}
  */ 

/** @defgroup UART_Hardware_Flow_Control 
  * @{
  */ 
#define UART5_HardwareFlowControl_None	   ((uint16_t)0x0000)
#define UART5_HardwareFlowControl_RTS		((uint16_t)0x0100)
#define UART5_HardwareFlowControl_CTS		((uint16_t)0x0200)
#define UART5_HardwareFlowControl_RTS_CTS	((uint16_t)0x0300)
#define IS_UART5_HARDWARE_FLOW_CONTROL(CONTROL)\
							  (((CONTROL) == UART5_HardwareFlowControl_None) || \
							   ((CONTROL) == UART5_HardwareFlowControl_RTS) || \
							   ((CONTROL) == UART5_HardwareFlowControl_CTS) || \
							   ((CONTROL) == UART5_HardwareFlowControl_RTS_CTS))
							   
#define CR1_UE_Set_Uart5		  ((uint32_t)0x00000001)  /*!< UART Enable Mask */
#define CR1_UE_Reset_Uart5		((uint32_t)0xFFFFFFFE)  /*!< UART Disable Mask */							   
/**
  * @}
  */ 

/** @defgroup UART_Hardware_FIFO
  * @{
  */ 			
#define UART5_CR1_FIFOEN			  (0x1U << 29)					 /*FIFO mode enable */								  
#define UART5_CR1_TXFEIE			  (0x1U << 30)					/*TXFIFO empty interrupt enable */								   
#define UART5_CR1_RXFFIE			  (0x1U << 31)					 /* RXFIFO Full interrupt enable*/
#define UART5_CR1_PEIE				(0x1U << 8)					 /*PE interrupt enable*/  
#define UART5_CR1_TXEIE			   (0x1U << 7)					 /* Transmit data register empty/TXFIFO not full interrupt enable*/
#define UART5_CR1_TCIE				(0x1U << 6)					 /*TC interrupt enable*/	
#define UART5_CR1_RXNEIE			  (0x1U << 5)					 /*Receive data register not empty/RXFIFO not empty interrupt enable*/
#define UART5_CR1_IDLEIE			  (0x1U << 4)	                /*TMC(20240409)gsx:ILDE interrupt enable*/	
	
	  

#define UART5_CR3_EIE				 (0x1U << 0)					 /*Error interrupt enable*/
#define UART5_CR3_TXFTIE			  (0x1U << 23)					/*!< TXFIFO threshold interrupt enable !< 0x00800000 */											 
#define UART5_CR3_RXFTCFG			 (0x7U << 25)					/*!< RXFTCFG [2:0]Receive FIFO threshold configuration !< 0x0E000000 */
#define UART5_CR3_RXFTCFG_0		   (0x1U << 25)		  /*!< 0x02000000 */
#define UART5_CR3_RXFTCFG_1		   (0x2U << 25)		  /*!< 0x04000000 */
#define UART5_CR3_RXFTCFG_2		   (0x4U << 25)		  /*!< 0x08000000 */
						   
#define UART5_CR3_RXFTIE			  (0x1U << 28)					  /*!< RXFIFO threshold interrupt enable !< 0x10000000 */								  
#define UART5_CR3_TXFTCFG			 (0x7U << 29)					/*!< TXFIFO [2:0] threshold configuration !< 0xE0000000 */
#define UART5_CR3_TXFTCFG_0		   (0x1U << 29)		  /*!< 0x20000000 */
#define UART5_CR3_TXFTCFG_1		   (0x2U << 29)		  /*!< 0x40000000 */
#define UART5_CR3_TXFTCFG_2		   (0x4U << 29)		  /*!< 0x80000000 */

#define CR3_CLEAR_Mask			((uint32_t)0xFFFFFCFF)  /*!< UART CR3 Mask */
/** @defgroup UART5_FIFO_mode UART5 FIFO mode
  * @brief	UART5 FIFO  mode
  * @{
  */
#define UART5_FIFOMODE_DISABLE		((uint32_t)0x00000000U)	   /*!< FIFO mode disable */
#define UART5_FIFOMODE_ENABLE		 ((uint32_t)UART5_CR1_FIFOEN)  /*!< FIFO mode enable  */
			
#define UART5_DMATX_DISABLE		((uint32_t)0x00000000U)	   /*!< DMA TX disable */
#define UART5_DMATX_ENABLE		 ((uint32_t)(1<<7))  /*!< DMA TX enable  */
			
#define UART5_DMARX_DISABLE		((uint32_t)0x00000000U)	   /*!< DMA RX disable */
#define UART5_DMARX_ENABLE		 ((uint32_t)(1<<6))  /*!< DMA RX enable  */
/**
  * @}
  */

/** @defgroup UART5_TXFIFO_threshold_level UART TXFIFO threshold level
  * @brief	UART5 TXFIFO level
  * @{
  */
#define UART5_TXFIFO_THRESHOLD_1_8	((uint32_t)0x00000000U)							   /*!< TXFIFO reaches 1/8 of its depth */
#define UART5_TXFIFO_THRESHOLD_1_4	((uint32_t)UART5_CR3_TXFTCFG_0)					   /*!< TXFIFO reaches 1/4 of its depth */
#define UART5_TXFIFO_THRESHOLD_1_2	((uint32_t)(UART5_CR3_TXFTCFG_1))					   /*!< TXFIFO reaches 1/2 of its depth */
#define UART5_TXFIFO_THRESHOLD_3_4	((uint32_t)(UART5_CR3_TXFTCFG_0|UART5_CR3_TXFTCFG_1)) /*!< TXFIFO reaches 3/4 of its depth */
#define UART5_TXFIFO_THRESHOLD_7_8	((uint32_t)UART5_CR3_TXFTCFG_2)					   /*!< TXFIFO reaches 7/8 of its depth */
#define UART5_TXFIFO_THRESHOLD_8_8	((uint32_t)(UART5_CR3_TXFTCFG_2|UART5_CR3_TXFTCFG_0)) /*!< TXFIFO becomes empty			*/
/**
  * @}
  */

/** @defgroup UART5_RXFIFO_threshold_level UART RXFIFO threshold level
  * @brief	UART5 RXFIFO level
  * @{
  */
#define UART5_RXFIFO_THRESHOLD_1_8	((uint32_t)0x00000000U)							   /*!< RXFIFO reaches 1/8 of its depth */
#define UART5_RXFIFO_THRESHOLD_1_4	((uint32_t)UART5_CR3_RXFTCFG_0)					   /*!< RXFIFO reaches 1/4 of its depth */
#define UART5_RXFIFO_THRESHOLD_1_2	((uint32_t)UART5_CR3_RXFTCFG_1)					   /*!< RXFIFO reaches 1/2 of its depth */
#define UART5_RXFIFO_THRESHOLD_3_4	((uint32_t)(UART5_CR3_RXFTCFG_0|UART5_CR3_RXFTCFG_1)) /*!< RXFIFO reaches 3/4 of its depth */
#define UART5_RXFIFO_THRESHOLD_7_8	((uint32_t)UART5_CR3_RXFTCFG_2)					   /*!< RXFIFO reaches 7/8 of its depth */
#define UART5_RXFIFO_THRESHOLD_8_8	((uint32_t)(UART5_CR3_RXFTCFG_2|UART5_CR3_RXFTCFG_0)) /*!< RXFIFO becomes full			 */

/**
  * @brief Ensure that UART5 FIFO mode is valid.
  * @param __STATE__: UART5 FIFO mode.
  * @retval SET (__STATE__ is valid) or RESET (__STATE__ is invalid)
  */
#define IS_UART5_FIFO_MODE_STATE(__STATE__) (((__STATE__) == UART5_FIFOMODE_DISABLE ) || \
											 ((__STATE__) == UART5_FIFOMODE_ENABLE))

/**
  * @brief Ensure that UART5 TXFIFO threshold level is valid.
  * @param __THRESHOLD__: UART5 TXFIFO threshold level.
  * @retval SET (__THRESHOLD__ is valid) or RESET (__THRESHOLD__ is invalid)
  */
#define IS_UART5_TXFIFO_THRESHOLD(__THRESHOLD__) ((((__THRESHOLD__) == UART5_TXFIFO_THRESHOLD_1_8 ) || \
												   ((__THRESHOLD__) == UART5_TXFIFO_THRESHOLD_1_4 ) || \
												   ((__THRESHOLD__) == UART5_TXFIFO_THRESHOLD_1_2)  || \
												   ((__THRESHOLD__) == UART5_TXFIFO_THRESHOLD_3_4)  || \
												   ((__THRESHOLD__) == UART5_TXFIFO_THRESHOLD_7_8)) || \
												   ((__THRESHOLD__) == UART5_TXFIFO_THRESHOLD_8_8))

/**
  * @brief Ensure that UART5 RXFIFO threshold level is valid.
  * @param __THRESHOLD__: UART5 RXFIFO threshold level.
  * @retval SET (__THRESHOLD__ is valid) or RESET (__THRESHOLD__ is invalid)
  */
#define IS_UART5_RXFIFO_THRESHOLD(__THRESHOLD__) ((((__THRESHOLD__) == UART5_RXFIFO_THRESHOLD_1_8 ) || \
												   ((__THRESHOLD__) == UART5_RXFIFO_THRESHOLD_1_4 ) || \
												   ((__THRESHOLD__) == UART5_RXFIFO_THRESHOLD_1_2)  || \
												   ((__THRESHOLD__) == UART5_RXFIFO_THRESHOLD_3_4)  || \
												   ((__THRESHOLD__) == UART5_RXFIFO_THRESHOLD_7_8)) || \
												   ((__THRESHOLD__) == UART5_RXFIFO_THRESHOLD_8_8))
												  
#define  UART5_SWAP_TXRX   ((uint32_t)(1<<15))
#define  UART5_NSWAP_TXRX ((uint32_t)0x00000000U)

/**
  * @}
  */

/** @defgroup UART5_DMA_Requests 
  * @{
  */

#define UART5_DMAReq_Tx					  ((uint16_t)0x0080)
#define UART5_DMAReq_Rx					  ((uint16_t)0x0040)
#define IS_UART5_DMAREQ(DMAREQ) ((((DMAREQ) & (uint16_t)0xFF3F) == 0x00) && ((DMAREQ) != (uint16_t)0x00))

/**
  * @}
  */

/** @defgroup UART5_Flags 
  * @{
  */
#define UART5_FLAG_TXFT		((uint32_t)(1<<27))  //TXFIFO threshold flag
#define UART5_FLAG_RXFT		((uint32_t)(1<<26))  //RXFIFO threshold flag
#define UART5_FLAG_RXFF		((uint32_t)(1<<24))  //RXFIFO Full flag
#define UART5_FLAG_TXFE		((uint32_t)(1<<23))  //TXFIFO Emptyflag
#define UART5_FLAG_BUSY		((uint32_t)(1<<16))  //Busy flag
#define UART5_FLAG_CTS		 ((uint32_t)(1<<10))  //CTS flag		  
#define UART5_FLAG_CTSIF	   ((uint32_t)(1<<9))   //CTS interrupt flag				   
#define UART5_FLAG_TXE		 ((uint32_t)(1<<7))   //Transmit data register empty/TXFIFO not full flag				 
#define UART5_FLAG_TC		  ((uint32_t)(1<<6))   //Transmission complete flag		 
#define UART5_FLAG_RXNE		((uint32_t)(1<<5))   //Read data register not empty/RXFIFO not empty flag	   
#define UART5_FLAG_IDLE		((uint32_t)(1<<4))   //IDLE line detected flag		
#define UART5_FLAG_ORE		 ((uint32_t)(1<<3))   //Overrun error flag		  
#define UART5_FLAG_NE		  ((uint32_t)(1<<2))   //START bit Noise detection flag			 
#define UART5_FLAG_FE		  ((uint32_t)(1<<1))   //Framing error flag		 
#define UART5_FLAG_PE		  ((uint32_t)(1<<0))   //Parity error flag		  
#define IS_UART5_FLAG(FLAG)	((((FLAG) & (~(UART5_FLAG_PE   | UART5_FLAG_TXE   | \
										   UART5_FLAG_TC	| UART5_FLAG_RXNE  | \
										   UART5_FLAG_IDLE  |  UART5_FLAG_BUSY | \
										   UART5_FLAG_CTS   | UART5_FLAG_CTSIF | \
										   UART5_FLAG_ORE   | UART5_FLAG_TXFT  | \
										   UART5_FLAG_RXFT  |  UART5_FLAG_RXFF | \
										   UART5_FLAG_TXFE  |  UART5_FLAG_NE   |  UART5_FLAG_FE))) == 0) && ((FLAG) != (uint32_t)0x00))
							 
/** @defgroup UART5_Flags 
  * @{
  */		
#define UART5_CLEAR_CTSCF	   ((uint32_t)(1<<9))   //CTS interrupt flag								   
#define UART5_CLEAR_TCCF		((uint32_t)(1<<6))   //Transmission complete flag			 
#define UART5_CLEAR_IDLECF	  ((uint32_t)(1<<4))   //IDLE line detected flag		
#define UART5_CLEAR_ORECF	   ((uint32_t)(1<<3))   //Overrun error flag		  
#define UART5_CLEAR_NECF		((uint32_t)(1<<2))   //START bit Noise detection flag			 
#define UART5_CLEAR_FECF		((uint32_t)(1<<1))   //Framing error flag		 
#define UART5_CLEAR_PECF		((uint32_t)(1<<0))   //Parity error flag		  
#define IS_UART5_CLEAR_FLAG(FLAG)	((((FLAG) & (~(UART5_CLEAR_CTSCF   | UART5_CLEAR_TCCF   | \
										   UART5_CLEAR_IDLECF	| UART5_CLEAR_ORECF  | \
										   UART5_CLEAR_NECF  |  UART5_CLEAR_FECF | \
										   UART5_CLEAR_PECF ))) == 0) && ((FLAG) != (uint32_t)0x00))
										   

#define IS_UART5_PERIPH_FLAG(PERIPH, UART_FLAG) ((((*(uint32_t*)&(PERIPH)) != UART5_BASE) &&\
												  || ((UART_FLAG) != UART_FLAG_CTS)) 
//#define IS_UART5_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x0044AA21))
 /*TMC(20240409)-gsx:UART5 Maximum support��6M*/
#define IS_UART5_BAUDRATE(BAUDRATE) (((BAUDRATE) > 0) && ((BAUDRATE) < 0x005B8D81))
#define IS_UART5_ADDRESS(ADDRESS) ((ADDRESS) <= 0xF)
#define IS_UART5_DATA(DATA) ((DATA) <= 0x1FF)

/** @brief  Clear the specified UART ISR flag, in setting the proper ICR register flag.
  * @param  __HANDLE__: specifies the UART Handle.
  * @param  __IT_CLEAR__: specifies the interrupt clear register flag that needs to be set
  *					   to clear the corresponding interrupt
  *		  This parameter can be one of the following values:
  *			@arg UART5_CLEAR_PEF: Parity Error Clear Flag
  *			@arg UART5_CLEAR_FEF: Framing Error Clear Flag
  *			@arg UART5_CLEAR_NEF: Noise detected Clear Flag
  *			@arg UART5_CLEAR_OREF: OverRun Error Clear Flag
  *			@arg UART5_CLEAR_IDLEF: IDLE line detected Clear Flag
  *			@arg UART5_CLEAR_TCF: Transmission Complete Clear Flag
  *			@arg UART5_CLEAR_LBDF: LIN Break Detection Clear Flag
  *			@arg UART5_CLEAR_CTSF: CTS Interrupt Clear Flag
  *			@arg UART5_CLEAR_RTOF: Receiver Time Out Clear Flag
  *			@arg UART5_CLEAR_CMF: Character Match Clear Flag
  *			@arg.UART5_CLEAR_WUF:  Wake Up from stop mode Clear Flag
  *			@arg UART5_CLEAR_TXFECF: TXFIFO empty Clear Flag
  * @retval None
  */
#define __UART5_CLEAR_IT(__HANDLE__, __IT_CLEAR__) ((__HANDLE__)->Instance->ICR = (uint32_t)(__IT_CLEAR__))

/**
  * @}
  */

/** @defgroup USART_ClockPrescaler  USART Clock Prescaler
  * @{
  */
#define UART5_PRESCALER_DIV1	0x00000000U  /*!< fclk_pres = fclk	 */
#define UART5_PRESCALER_DIV2	0x00000001U  /*!< fclk_pres = fclk/2   */
#define UART5_PRESCALER_DIV4	0x00000002U  /*!< fclk_pres = fclk/4   */
#define UART5_PRESCALER_DIV6	0x00000003U  /*!< fclk_pres = fclk/6   */
#define UART5_PRESCALER_DIV8	0x00000004U  /*!< fclk_pres = fclk/8   */
#define UART5_PRESCALER_DIV10   0x00000005U  /*!< fclk_pres = fclk/10  */
#define UART5_PRESCALER_DIV12   0x00000006U  /*!< fclk_pres = fclk/12  */
#define UART5_PRESCALER_DIV16   0x00000007U  /*!< fclk_pres = fclk/16  */
#define UART5_PRESCALER_DIV32   0x00000008U  /*!< fclk_pres = fclk/32  */
#define UART5_PRESCALER_DIV64   0x00000009U  /*!< fclk_pres = fclk/64  */
#define UART5_PRESCALER_DIV128  0x0000000AU  /*!< fclk_pres = fclk/128 */
#define UART5_PRESCALER_DIV256  0x0000000BU  /*!< fclk_pres = fclk/256 */
/**
  * @brief Ensure that USART Prescaler is valid.
  * @param __CLOCKPRESCALER__ USART Prescaler value.
  * @retval SET (__CLOCKPRESCALER__ is valid) or RESET (__CLOCKPRESCALER__ is invalid)
  */
#define IS_UART5_PRESCALER(__CLOCKPRESCALER__) (((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV1) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV2) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV4) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV6) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV8) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV10) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV12) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV16) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV32) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV64) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV128) || \
												((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV256))
/** @brief  Get UART clok division factor from clock prescaler value.
  * @param  __CLOCKPRESCALER__ UART prescaler value.
  * @retval UART clock division factor
  */												
#define UART5_GET_DIV_FACTOR(__CLOCKPRESCALER__) \
  (((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV1)   ? 1U :	   \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV2)   ? 2U :	   \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV4)   ? 4U :	   \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV6)   ? 6U :	   \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV8)   ? 8U :	   \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV10)  ? 10U :	  \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV12)  ? 12U :	  \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV16)  ? 16U :	  \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV32)  ? 32U :	  \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV64)  ? 64U :	  \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV128) ? 128U :	 \
   ((__CLOCKPRESCALER__) == UART5_PRESCALER_DIV256) ? 256U : 1U)												
/** @defgroup UART5_Exported_Functions
  * @{
  */
void UART5_DeInit(UART5_TypeDef* UARTx);
void UART5_Init(UART5_TypeDef* UARTx, UART5_InitTypeDef* UART_InitStruct);
__weak void UART5_StructInit(UART5_InitTypeDef* UART_InitStruct);
void UART5_Cmd(UART5_TypeDef* UARTx, FunctionalState NewState);
void UART5_ITConfig(UART5_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState);
void UART5_DMACmd(UART5_TypeDef* UARTx, uint16_t UART_DMAReq, FunctionalState NewState);
void UART5_SendData(UART5_TypeDef* UARTx, uint16_t Data);
FlagStatus UART5_GetDMAStatus(UART5_TypeDef* UARTx, uint32_t DMA_Mode);
uint16_t UART5_ReceiveData(UART5_TypeDef* UARTx);
FlagStatus UART5_GetFlagStatus(UART5_TypeDef* UARTx, uint32_t UART_FLAG);
void UART5_ClearFlag(UART5_TypeDef* UARTx, uint32_t UART_FLAG);

#ifdef __cplusplus
}
#endif
#endif /* __UART5_H */
