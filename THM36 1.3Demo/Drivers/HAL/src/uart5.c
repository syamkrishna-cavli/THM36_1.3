/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart5.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/   

#include "uart5.h"

/**
  * @brief  Deinitializes the UARTx peripheral registers to their default reset values.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values: 
  *	  UART5.
  * @retval None
  */
void UART5_DeInit(UART5_TypeDef* UARTx)
{
	/* Check the parameters */
	assert_param(IS_UART5_PERIPH(UARTx));

	RCC_APB3PeriphSoftReset(RCC_APB3Periph_UART5);
	while(RCC_APB3PeriphGetResetFlag(RCC_APB3Periph_UART5) == RESET);
}

/**
  * @brief  Initializes the UARTx peripheral according to the specified
  *		 parameters in the UART_InitStruct .
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   UART5.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *		 that contains the configuration information for the specified UART 
  *		 peripheral.
  * @retval None
  */
void UART5_Init(UART5_TypeDef* UARTx, UART5_InitTypeDef* UART_InitStruct)
{
	uint32_t tmpreg = 0x00;
	uint32_t uartckpres=0;
	uint32_t clocksource = 0;
	uint32_t uart_ker_ck_pres = 0x00000000U;
	
	/* Check the parameters */
	assert_param(IS_UART5_PERIPH(UARTx));
	assert_param(IS_UART5_PRESCALER(UART_InitStruct->ClockPrescaler));
	assert_param(IS_UART5_BAUDRATE(UART_InitStruct->BaudRate));  
	assert_param(IS_UART5_WORD_LENGTH(UART_InitStruct->WordLength));
	assert_param(IS_UART5_STOPBITS(UART_InitStruct->StopBits));
	assert_param(IS_UART5_PARITY(UART_InitStruct->Parity));
	assert_param(IS_UART5_MODE(UART_InitStruct->Mode));
	assert_param(IS_UART5_HARDWARE_FLOW_CONTROL(UART_InitStruct->HardwareFlowControl));
	assert_param(IS_UART5_FIFO_MODE_STATE(UART_InitStruct->FIFOMode));

	if (UART_InitStruct->FIFOMode == UART5_FIFOMODE_ENABLE)
	{
		assert_param(IS_UART5_TXFIFO_THRESHOLD(UART_InitStruct->TXFIFOThreshold));
		assert_param(IS_UART5_RXFIFO_THRESHOLD(UART_InitStruct->RXFIFOThreshold));
	}

	/*---------------------------- UART CR2 Configuration -----------------------*/
	tmpreg = UARTx->CR2;
	/* Clear STOP[13:12] bits */
	tmpreg &= CR2_STOP_CLEAR_Mask;
	/* Configure the UART Stop Bits, Clock, CPOL, CPHA and LastBit ------------*/
	/* Set STOP[13:12] bits according to UART_StopBits value */
	tmpreg |= (uint32_t)UART_InitStruct->StopBits | UART_InitStruct->TxRx_Swap;

	/* Write to UART CR2 */
	UARTx->CR2 = (uint16_t)tmpreg;

	/*---------------------------- UART CR1 Configuration -----------------------*/
	tmpreg = UARTx->CR1;
	/* Clear M, PCE, PS, TE and RE bits */
	tmpreg &= UART5_CR1_CLEAR_Mask;
	/* Configure the UART Word Length, Parity and mode ----------------------- */
	/* Set the M bits according to UART_WordLength value */
	/* Set PCE and PS bits according to UART_Parity value */
	/* Set TE and RE bits according to UART_Mode value */
	tmpreg |= (uint32_t)UART_InitStruct->WordLength | UART_InitStruct->Parity |
			UART_InitStruct->Mode;
	tmpreg |=  (uint32_t)UART_InitStruct->FIFOMode;
	/* Write to UART CR1 */
	UARTx->CR1 = tmpreg;

	/*---------------------------- UART CR3 Configuration -----------------------*/  
	tmpreg = UARTx->CR3;
	/* Clear CTSE and RTSE bits */
	tmpreg &= CR3_CLEAR_Mask;
	/* Configure the UART HFC -------------------------------------------------*/
	/* Set CTSE and RTSE bits according to UART_HardwareFlowControl value */
	tmpreg |= UART_InitStruct->HardwareFlowControl;

	if (UART_InitStruct->FIFOMode == UART5_FIFOMODE_ENABLE)
	{
		tmpreg |= ((uint32_t)UART_InitStruct->TXFIFOThreshold | (uint32_t)UART_InitStruct->RXFIFOThreshold);
	}

	/* Write to UART CR3 */
	UARTx->CR3 = tmpreg;
	/*---------------------------- UART BRR Configuration -----------------------*/
	/* Configure the UART Baud Rate -------------------------------------------*/
	UARTx->PRESC |= UART_InitStruct->ClockPrescaler;
	clocksource = RCC_GetUART5ClockSource();
	switch (clocksource)
	{
		case RCC_UART5_CLKSRC_PLL_L:
			uart_ker_ck_pres = RCC_GetPLL_L_Clock() / RCC_GetUART5ClockDivision();
			break;
		case RCC_UART5_CLKSRC_HSE:
			uart_ker_ck_pres = HSE_VALUE / RCC_GetUART5ClockDivision();
			break;
		case RCC_UART5_CLKSRC_HSI:
			uart_ker_ck_pres = HSI_VALUE / RCC_GetUART5ClockDivision();
			break;
		default:
			uart_ker_ck_pres = HSI_VALUE / RCC_GetUART5ClockDivision();
			break;
	}
	uartckpres = uart_ker_ck_pres / UART5_GET_DIV_FACTOR(UARTx->PRESC);
	tmpreg = (double)(256*((double)uartckpres)) / (double)(UART_InitStruct->BaudRate);
	UARTx->BRR = tmpreg;
	UART5_Cmd(UARTx,ENABLE);
}

/**
  * @brief  Fills each UART_InitStruct member with its default value.
  * @param  UART_InitStruct: pointer to a UART_InitTypeDef structure
  *		 which will be initialized.
  * @retval None
  */
__weak void UART5_StructInit(UART5_InitTypeDef* UART_InitStruct)
{
	UART_InitStruct->ClockPrescaler = UART5_PRESCALER_DIV1;
	UART_InitStruct->BaudRate = 115200;
	UART_InitStruct->WordLength = UART5_WordLength_8b;
	UART_InitStruct->Parity = UART5_Parity_None;
	UART_InitStruct->StopBits = UART5_StopBits_1; 
	UART_InitStruct->Mode = UART5_Mode_Rx | UART5_Mode_Tx;
	UART_InitStruct->HardwareFlowControl = UART5_HardwareFlowControl_None;
	UART_InitStruct->FIFOMode = UART5_FIFOMODE_DISABLE;
	UART_InitStruct->TXFIFOThreshold = UART5_TXFIFO_THRESHOLD_1_8;
	UART_InitStruct->RXFIFOThreshold = UART5_RXFIFO_THRESHOLD_1_8;
	UART_InitStruct->TxRx_Swap = UART5_NSWAP_TXRX;
}
/**
  * @brief  Enables or disables the specified UART peripheral.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *		 This parameter can be one of the following values:
  *		   UART1, UART2, UART3, UART4 or UART5.
  * @param  NewState: new state of the UARTx peripheral.
  *		 This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void UART5_Cmd(UART5_TypeDef* UARTx, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART5_PERIPH(UARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
	/* Enable the selected UART by setting the UE bit in the CR1 register */
	UARTx->CR1 |= CR1_UE_Set_Uart5;//CR1_UE_Set;
  }
  else
  {
	/* Disable the selected UART by clearing the UE bit in the CR1 register */
	UARTx->CR1 &= CR1_UE_Reset_Uart5;//CR1_UE_Reset;
  }
}

/**
  * @brief  Enables or disables the specified UART interrupts.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   UART5.
  * @param  UART_IT: specifies the UART interrupt sources to be enabled or disabled.
  *   This parameter can be one of the following values:
  *	 @arg UART_IT_CTS:  CTS change interrupt (not available for UART4 and UART5)
  *	 @arg UART_IT_TXE:  Transmit Data Register empty interrupt
  *	 @arg UART_IT_TC:   Transmission complete interrupt
  *	 @arg UART_IT_RXNE: Receive Data register not empty interrupt
  *	 @arg UART_IT_IDLE: Idle line detection interrupt
  *	 @arg UART_IT_PE:   Parity Error interrupt
  *	 @arg UART_IT_ERR:  Error interrupt(Frame error, noise error, overrun error)
  * @param  NewState: new state of the specified UARTx interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
//void UART5_ITConfig(UART5_TypeDef* UARTx, uint16_t UART_IT, FunctionalState NewState)
//{
//  uint32_t UARTreg = 0x00, itmask = 0x00;
//  volatile uint32_t itpos = 0x00;
//  uint32_t UARTxbase = 0x00;
//  /* Check the parameters */
//  assert_param(IS_UART5_PERIPH(UARTx));
//  assert_param(IS_UART5_CONFIG_IT(UART_IT));
//  assert_param(IS_FUNCTIONAL_STATE(NewState));
//  /* The CTS interrupt is not available for UART4 and UART5 */
//  if (UART_IT == UART5_IT_CTS)
//  {
//	assert_param(IS_UART5_PERIPH(UARTx));
//  }   
//  
//  UARTxbase = (uint32_t)UARTx;

//  /* Get the UART register index */
//  UARTreg = (((uint8_t)UART_IT) >> 0x05);

//  /* Get the interrupt position */
//  itpos = UART_IT & IT_Mask;
//  itmask = (((uint32_t)0x01) << itpos);
//	
//  if (UARTreg == 0x01) /* The IT is in CR1 register */
//  {
//	UARTxbase += 0x00;
//  }
//  else if (UARTreg == 0x02) /* The IT is in CR2 register */
//  {
//	UARTxbase += 0x04;
//  }
//  else /* The IT is in CR3 register */
//  {
//	UARTxbase += 0x08; 
//  }
//  if (NewState != DISABLE)
//  {
//	*(volatile uint32_t*)UARTxbase  |= itmask;
//  }
//  else
//  {
//	*(volatile uint32_t*)UARTxbase &= ~itmask;
//  }
//}
/**
  * @brief  Enables or disables the UART’s DMA interface.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *	UART5.
  * @param  UART_DMAReq: specifies the DMA request.
  *   This parameter can be any combination of the following values:
  *	 @arg UART_DMAReq_Tx: UART DMA transmit request
  *	 @arg UART_DMAReq_Rx: UART DMA receive request
  * @param  NewState: new state of the DMA Request sources.
  *   This parameter can be: ENABLE or DISABLE.
  * @note The DMA mode is not available for UART5 except in the STM32
  *	   High density value line devices(STM32F10X_HD_VL).  
  * @retval None
  */
void UART5_DMACmd(UART5_TypeDef* UARTx, uint16_t UART_DMAReq, FunctionalState NewState)
{
  /* Check the parameters */
  assert_param(IS_UART5_PERIPH(UARTx));
  assert_param(IS_UART5_DMAREQ(UART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  if (NewState != DISABLE)
  {
	/* Enable the DMA transfer for selected requests by setting the DMAT and/or
	   DMAR bits in the UART CR3 register */
	UARTx->CR3 |= UART_DMAReq;
  }
  else
  {
	/* Disable the DMA transfer for selected requests by clearing the DMAT and/or
	   DMAR bits in the UART CR3 register */
	UARTx->CR3 &= (uint16_t)~UART_DMAReq;
  }
}
/**
  * @brief  Get DMA status
  * @param[in]  UARTx: where x can be 5 to select the UART peripheral.
  * @param[in]	DMA_Mode: specifies receive or transmit.
  *	 @arg UART5_DMAReq_Tx
  *	 @arg UART5_DMAReq_Rx
  * @retval FlagStatus
  *		@arg SET : DMA has been enabled
  *	 @arg RESET : DMA has been closed
  * @note	none
  */
FlagStatus UART5_GetDMAStatus(UART5_TypeDef* UARTx, uint32_t DMA_Mode)
{
	assert_param(IS_UART5_PERIPH(UARTx));
	assert_param(IS_UART5_DMAREQ(DMA_Mode));
	
	if (UARTx->CR3 & DMA_Mode)
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
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   UART1, UART2, UART3, UART4 or UART5.
  * @param  Data: the data to transmit.
  * @retval None
  */
void UART5_SendData(UART5_TypeDef* UARTx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_UART5_PERIPH(UARTx));
  assert_param(IS_UART5_DATA(Data)); 
	
  /* Transmit Data */
  UARTx->TDR = (Data & (uint16_t)0x01FF);
}

/**
  * @brief  Returns the most recent received data by the UARTx peripheral.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   UART1, UART2, UART3, UART4 or UART5.
  * @retval The received data.
  */
uint16_t UART5_ReceiveData(UART5_TypeDef* UARTx)
{
  /* Check the parameters */
  assert_param(IS_UART5_PERIPH(UARTx));
  
  /* Receive Data */
  return (uint16_t)(UARTx->RDR & (uint16_t)0x01FF);
}
/**
  * @brief  Checks whether the specified UART flag is set or not.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   UART5.
  * @param  UART_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *	 @arg UART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5)
  *	 @arg UART_FLAG_TXE:  Transmit data register empty flag
  *	 @arg UART_FLAG_TC:   Transmission Complete flag
  *	 @arg UART_FLAG_RXNE: Receive data register not empty flag
  *	 @arg UART_FLAG_IDLE: Idle Line detection flag
  *	 @arg UART_FLAG_ORE:  OverRun Error flag
  *	 @arg UART_FLAG_NE:   Noise Error flag
  *	 @arg UART_FLAG_FE:   Framing Error flag
  *	 @arg UART_FLAG_PE:   Parity Error flag
  * @retval The new state of UART_FLAG (SET or RESET).
  */
FlagStatus UART5_GetFlagStatus(UART5_TypeDef* UARTx, uint32_t UART_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* Check the parameters */
  assert_param(IS_UART5_PERIPH(UARTx));
  assert_param(IS_UART5_FLAG(UART_FLAG));
  /* The CTS flag is not available for UART4 and UART5 */
  if (UART_FLAG == UART5_FLAG_CTS)
  {
	assert_param(IS_UART5_PERIPH(UARTx));
  }  
  
  if ((UARTx->ISR & UART_FLAG) != (uint32_t)RESET)
  {
	bitstatus = SET;
  }
  else
  {
	bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @brief  Clears the UARTx's pending flags.
  * @param  UARTx: Select the UART or the UART peripheral. 
  *   This parameter can be one of the following values:
  *   UART1, UART2, UART3, UART4 or UART5.
  * @param  UART_FLAG: specifies the flag to clear.
  *   This parameter can be any combination of the following values:
  *	 @arg UART_FLAG_CTS:  CTS Change flag (not available for UART4 and UART5).
  *	 @arg UART_FLAG_LBD:  LIN Break detection flag.
  *	 @arg UART_FLAG_TC:   Transmission Complete flag.
  *	 @arg UART_FLAG_RXNE: Receive data register not empty flag.
  *   
  * @note
  *   - PE (Parity error), FE (Framing error), NE (Noise error), ORE (OverRun 
  *	 error) and IDLE (Idle line detected) flags are cleared by software 
  *	 sequence: a read operation to UART_SR register (UART_GetFlagStatus()) 
  *	 followed by a read operation to UART_DR register (UART_ReceiveData()).
  *   - RXNE flag can be also cleared by a read to the UART_DR register 
  *	 (UART_ReceiveData()).
  *   - TC flag can be also cleared by software sequence: a read operation to 
  *	 UART_SR register (UART_GetFlagStatus()) followed by a write operation
  *	 to UART_DR register (UART_SendData()).
  *   - TXE flag is cleared only by a write to the UART_DR register 
  *	 (UART_SendData()).
  * @retval None
  */
void UART5_ClearFlag(UART5_TypeDef* UARTx, uint32_t UART_FLAG)
{
  /* Check the parameters */
  assert_param(IS_UART5_PERIPH(UARTx));
  assert_param(IS_UART5_CLEAR_FLAG(UART_FLAG));
  /* The CTS flag is not available for UART4 and UART5 */
  if ((UART_FLAG & UART5_FLAG_CTS) == UART5_FLAG_CTS)
  {
	assert_param(IS_UART5_PERIPH(UARTx));
  } 
   
  UARTx->ISR = ~UART_FLAG;
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

/************************END OF FILE**************************/
