/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

/* Includes ------------------------------------------------------------------*/
#include "thm36_1.3.h"
#include "UART.h"
#include "RCC.h"

/** @addtogroup thm36_1.3
  * @{
  */

/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup UART UART
  * @brief UART HAL modules driver 
  * @{
  */ 

/** @defgroup UART_Private_Defines UART Private Defines
  * @{
  */

/* ---------------------- UART registers bit mask ------------------------ */

/* UARTCON1 register bit mask */

/* UARTCON2 register bit mask */
#define UART_BIT_WIDTH_Pos			(0U)
#define UART_BIT_WIDTH_Msk			(0x1UL << UART_BIT_WIDTH_Pos)
#define UART_BIT_WIDTH				UART_BIT_WIDTH_Msk
#define UART_PARITY_SET_Pos			(1U)
#define UART_PARITY_SET_Msk			(0x3UL << UART_PARITY_SET_Pos)
#define UART_PARITY_SET				UART_PARITY_SET_Msk
#define UART_BAUD_HIGH_Pos			(3U)
#define UART_BAUD_HIGH_Msk			(0x1FUL << UART_BAUD_HIGH_Pos)
#define UART_BAUD_HIGH				UART_BAUD_HIGH_Msk

/* UARTSTS register bit mask */
#define UART_STS_TXE_Pos			(0U)									 
#define UART_STS_TXE_Msk			(0x1U << UART_STS_TXE_Pos)				/*!< 0x00000001 */
#define UART_STS_TXE				UART_STS_TXE_Msk						/*!<Transmit Data Register Empty*/
#define UART_STS_RXF_Pos			(1U)									 
#define UART_STS_RXF_Msk			(0x1U << UART_STS_RXF_Pos)				/*!< 0x00000002 */
#define UART_STS_RXF				UART_STS_RXF_Msk						/*!<Receive Data Register Full	*/
#define UART_STS_PE_Pos				(2U)									 
#define UART_STS_PE_Msk				(0x1U << UART_STS_PE_Pos)				/*!< 0x00000004 */
#define UART_STS_PE					UART_STS_PE_Msk							/*!<Parity Error				*/
#define UART_STS_OFE_Pos			(3U)									 
#define UART_STS_OFE_Msk			(0x1U << UART_STS_OFE_Pos)				/*!< 0x00000008 */
#define UART_STS_OFE				UART_STS_OFE_Msk						/*!<Overflow Error				*/
#define UART_STS_FE_Pos				(4U)									 
#define UART_STS_FE_Msk				(0x1U << UART_STS_FE_Pos)				/*!< 0x00000010 */
#define UART_STS_FE					UART_STS_FE_Msk							/*!<Framing Error				*/

/* UARTCON3 register bit mask */
#define UART_BAUD_FRACTION_Pos		(0U)									 
#define UART_BAUD_FRACTION_Msk		(0x7U << UART_BAUD_FRACTION_Pos)		/*!< 0x00000007 */
#define UART_BAUD_FRACTION			UART_BAUD_FRACTION_Msk					/*!<Baud Fraction Value			*/
#define UART_BAUD_FRACTION_0000		0x00000000UL							/*!<Baud Value Add 0.000		*/
#define UART_BAUD_FRACTION_0125		0x00000001UL							/*!<Baud Value Add 0.125		*/
#define UART_BAUD_FRACTION_0250		0x00000002UL							/*!<Baud Value Add 0.25			*/
#define UART_BAUD_FRACTION_0375		0x00000003UL							/*!<Baud Value Add 0.375		*/
#define UART_BAUD_FRACTION_0500		0x00000004UL							/*!<Baud Value Add 0.5			*/
#define UART_BAUD_FRACTION_0625		0x00000005UL							/*!<Baud Value Add 0.625		*/
#define UART_BAUD_FRACTION_0750		0x00000006UL							/*!<Baud Value Add 0.75			*/
#define UART_BAUD_FRACTION_0875		0x00000007UL							/*!<Baud Value Add 0.875		*/

/* UARTMSK register bit mask */
#define UART_INT_TXEMSK_Pos			(0U)									 
#define UART_INT_TXEMSK_Msk			(0x1U << UART_INT_TXEMSK_Pos)			/*!< 0x00000001 */
#define UART_INT_TXEMSK				UART_INT_TXEMSK_Msk						/*!<Mask Transmit Data Register Empty Interrupt	*/
#define UART_INT_RXFMSK_Pos			(1U)									 
#define UART_INT_RXFMSK_Msk			(0x1U << UART_INT_RXFMSK_Pos)			/*!< 0x00000002 */
#define UART_INT_RXFMSK				UART_INT_RXFMSK_Msk						/*!<Mask Receive Data Register Full Interrupt	*/
#define UART_INT_PEMSK_Pos			(2U)									 
#define UART_INT_PEMSK_Msk			(0x1U << UART_INT_PEMSK_Pos)			/*!< 0x00000004 */
#define UART_INT_PEMSK				UART_INT_PEMSK_Msk						/*!<Mask Parity Error Interrupt					*/
#define UART_INT_OFEMSK_Pos			(3U)									 
#define UART_INT_OFEMSK_Msk			(0x1U << UART_INT_OFEMSK_Pos)			/*!< 0x00000008 */
#define UART_INT_OFEMSK				UART_INT_OFEMSK_Msk						/*!<Mask Overflow Error Interrupt				*/
#define UART_INT_FEMSK_Pos			(4U)									 
#define UART_INT_FEMSK_Msk			(0x1U << UART_INT_FEMSK_Pos)			/*!< 0x00000010 */
#define UART_INT_FEMSK				UART_INT_FEMSK_Msk						/*!<Mask Framing Error Interrupt				*/

/* UARTDMACON register bit mask */
#define UART_DMA_RXEN_Pos			(0U)									 
#define UART_DMA_RXEN_Msk			(0x1U << UART_DMA_RXEN_Pos)				/*!< 0x00000001 */
#define UART_DMA_RXEN				UART_DMA_RXEN_Msk						/*!<Receive DMA Enable				*/
#define UART_DMA_TXEN_Pos			(1U)									 
#define UART_DMA_TXEN_Msk			(0x1U << UART_DMA_TXEN_Pos)				/*!< 0x00000002 */
#define UART_DMA_TXEN				UART_DMA_TXEN_Msk						/*!<Transmit DMA Enable				*/


/**
  * @}
  */ 


/** @defgroup UART_Exported_Functions UART Exported Functions
  * @{
  */  


/**
  * @brief  Deinitializes the UARTx peripheral registers to their reset values.
  * @param[in]  UARTx: where x can be 1 to 3 to select the UART peripheral. 
  * @retval none
  * @note
  */
void UART_DeInit(UART_TypeDef * UARTx)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	if (UARTx == UART1)
	{
		RCC_APB2PeriphSoftReset(RCC_APB2Periph_UART1);
		while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_UART1) == RESET);
	}
	else if(UARTx == UART2)
	{
		RCC_APB3PeriphSoftReset(RCC_APB3Periph_UART2);
		while(RCC_APB3PeriphGetResetFlag(RCC_APB3Periph_UART2)==RESET);
	}
//	else if(UARTx == UART5)
//	{
//		RCC_APB3PeriphSoftReset(RCC_APB3Periph_UART5);
//		while(RCC_APB3PeriphGetResetFlag(RCC_APB3Periph_UART5) == RESET);
//	}

}

/**
  * @brief  Initializes the UARTx peripheral according to the specified
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral	 
  * @param[in]  UART_InitStruct: pointer to a UART_InitTypeDef structure
  * 		that contains the configuration information for the specified UART peripheral.
  * @retval none
  * @note
  */
void UART_Init(UART_TypeDef* UARTx, UART_InitTypeDef* UART_InitStruct)
{
	float BaudRate;
	uint32_t uart_clock = 0;

	 /* Check the parameters */
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	assert_param(IS_UART_DATAWIDTH(UART_InitStruct->DataWidth));
	//assert_param(IS_UART_PARITYSTATE(UART_InitStruct->ParityEn));
	assert_param(IS_UART_PARITYMODE(UART_InitStruct->ParityMode));
	
	uart_clock = RCC_GetSysClocksFreq()/2;
	
	/* Write to UART CON */
	BaudRate = uart_clock/(UART_InitStruct->BaudRate*16.0)-1;
	UARTx->CON1 = 0xFF & (uint16_t)BaudRate;
	
	/*Clear parity bits	*/
	UARTx->CON2 &= ~UART_PARITY_SET;
	
	/*set baudrate high bits, parity mode bits and data width	*/
	UARTx->CON2 = (((uint16_t)BaudRate>>5) & UART_BAUD_HIGH) 
					| UART_InitStruct->DataWidth 
					| UART_InitStruct->ParityMode;

	UARTx->CON3 = (uint8_t)((BaudRate-(uint16_t)BaudRate)*8 + 0.5);

}

/**
  * @brief   Initializes the InitStruct of UARTx
  * @param[in]	UART_InitStruct: pointer to a UART_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  
  */
__weak void UART_StructInit(UART_InitTypeDef* UART_InitStruct)
{
	UART_InitStruct->BaudRate = 115200;
	UART_InitStruct->StopBits = UART_STOP_BIT_1;
	UART_InitStruct->DataWidth = UART_DATA_WIDTH_8;	
	UART_InitStruct->ParityMode = UART_PARITY_MODE_NONE;
}

/**
  * @brief  Read UART status
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral
  * @param[in]	Uart_Flag: specifies the UART status.
  *	 @arg UART_FLAG_TXE: Transmit Buffer Empty
  *	 @arg UART_FLAG_RXF: Receive Buffer Full
  *	 @arg UART_FLAG_PE: Parity Error
  *	 @arg UART_FLAG_OF: Overflow Error
  *	 @arg UART_FLAG_FE: Frame Error
  * @retval FlagStatus
  *		@arg SET : Flag Stutas is Set
  *	 @arg RESET : Flag Stutas is Reset
  * @note	none
  */ 
FlagStatus UART_GetFlagStatus(UART_TypeDef* UARTx, uint16_t Uart_Flag)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	assert_param(IS_UART_STATUS(Uart_Flag));
	
	if ((UARTx->STS & Uart_Flag) != RESET)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief  Clear UART Flag status
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral.
  * @param[in]	Uart_Flag: specifies the UART status.
  *	 @arg UART_FLAG_TXE: Transmit Buffer Empty
  *	 @arg UART_FLAG_RXF: Receive Buffer Full
  *	 @arg UART_FLAG_PE: Parity Error
  *	 @arg UART_FLAG_OF: Overflow Error
  *	 @arg UART_FLAG_FE: Frame Error
  * @retval none
  * @note	none
  */
void UART_ClearFlagStatus(UART_TypeDef* UARTx,uint16_t Uart_Flag)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	//read rxdata to clear flags;
	UARTx->RxDATA;
}
/**
  * @brief  Enable or disable UART interrupt
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral.
  * @param[in]	Uart_IT: specifies the UART interrupt status.
  *	 @arg UART_IT_TXE: Transmit Buffer Empty
  *	 @arg UART_IT_RXF: Receive Buffer Full
  *	 @arg UART_IT_PE: Parity Error
  *	 @arg UART_IT_OF: Overflow Error
  *	 @arg UART_IT_FE: Frame Error
  * @param[in]	NewState
  *		@arg DISABLE :Disable interrupt
  *	 @arg ENABLE  :Enable interrupt
  * @retval none
  * @note	none
  */ 
void UART_ITConfig(UART_TypeDef* UARTx, uint16_t Uart_IT, FunctionalState NewState)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	assert_param(IS_UART_STATUS(Uart_IT));
	
	if (NewState != DISABLE)
	{
		UARTx->MSK &=  ~Uart_IT;
	}
	else
	{
		UARTx->MSK |=  Uart_IT;
	}
}

/**
  * @brief  Read UART interrupt status
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral
  * @param[in]	Uart_IT: specifies the UART interrupt status.
  *	 @arg UART_IT_TXE: Transmit Buffer Empty
  *	 @arg UART_IT_RXF: Receive Buffer Full
  *	 @arg UART_IT_PE: Parity Error
  *	 @arg UART_IT_OF: Overflow Error
  *	 @arg UART_IT_FE: Frame Error
  * @retval FlagStatus
  *		@arg SET : IT Stutas is Set
  *	 @arg RESET : IT Stutas is Reset
  * @note	none
  */ 
FlagStatus UART_GetITStatus(UART_TypeDef* UARTx, uint16_t Uart_IT)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	assert_param(IS_UART_STATUS(Uart_IT));
	
	if (((UARTx->STS & Uart_IT) != RESET) && ((UARTx->MSK & Uart_IT) == RESET))
	{
		return SET;
	}
	return RESET;
}
/**
  * @brief  Clear UART IT status
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral.
  * @param[in]	Uart_IT: specifies the UART interrupt status.
  *	 @arg UART_IT_TXE: Transmit Buffer Empty
  *	 @arg UART_IT_RXF: Receive Buffer Full
  *	 @arg UART_IT_PE: Parity Error
  *	 @arg UART_IT_OF: Overflow Error
  *	 @arg UART_IT_FE: Frame Error
  * @retval none
  * @note	none
  */
void UART_ClearITStatus(UART_TypeDef* UARTx,uint16_t Uart_IT)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	//read rxdata to clear flags;
	UARTx->RxDATA;
}

/**
  * @brief  Enable or disable DMA
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral.
  * @param[in]	DMA_Mode: specifies receive or transmit.
  *	 @arg UART_DMA_RX
  *	 @arg UART_DMA_TX
  * @param[in]	NewState: new state of the UART DMA.
  *   		This parameter can be: ENABLE or DISABLE.
  * @retval none
  * @note	none
  */
void UART_DMACmd(UART_TypeDef* UARTx, uint32_t DMA_Mode, FunctionalState NewState)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	assert_param(IS_UART_DAM_MODE(DMA_Mode));
	
	if (NewState != DISABLE)
	{
		UARTx->DMACON |= DMA_Mode;
	}
	else
	{
		UARTx->DMACON &= ~DMA_Mode;
	}
}

/**
  * @brief  Get DMA status
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral.
  * @param[in]	DMA_Mode: specifies receive or transmit.
  *	 @arg UART_DMA_RX
  *	 @arg UART_DMA_TX
  * @retval FlagStatus
  *		@arg SET : DMA has been enabled
  *	 @arg RESET : DMA has been closed
  * @note	none
  */
FlagStatus UART_GetDMAStatus(UART_TypeDef* UARTx, uint32_t DMA_Mode)
{
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	assert_param(IS_UART_DAM_MODE(DMA_Mode));
	
	if (UARTx->DMACON & DMA_Mode)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Transmits single data through the UARTx peripheral.
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral. 
  * @param[in]  Data: the data to transmit.
  * @retval None
  */
void UART_SendData(UART_TypeDef* UARTx, uint8_t Data)
{
	/* Check the parameters */
	assert_param(IS_UART_Periph((uint32_t)UARTx));
	/* Transmit Data */
	UARTx->TxDATA = Data;
}

/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param[in]  UARTx: where x can be 1 to 4 to select the UART peripheral.
  * @retval The received data.
  */
uint8_t UART_ReceiveData(UART_TypeDef* UARTx)
{
	/* Check the parameters */
	assert_param(IS_UART_Periph((uint32_t)UARTx));
  
	/* Receive Data */
	return (uint8_t)UARTx->RxDATA;
}
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
/************************ (C) COPYRIGHT TMC *****END OF FILE****/
