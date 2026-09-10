/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: dma.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

  
#include "thm36_1.3.h"
#include "dma.h"

/** @addtogroup thm36_1.3
  * @{
  */
  
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */

/** @defgroup DMA 
  * @brief DMA HAL modules driver 
  * @{
  */ 
  
/** @defgroup DMA_Private_Defines DMA Private Defines
  * @{
  */

/* DMA Channel reset value */
#define DMA_Channel_CFG_RSTVAL			(0x00000000UL)
#define DMA_Channel_CON_RSTVAL			(0x00000000UL)
#define DMA_Channel_SRC_RSTVAL			(0x00000000UL)
#define DMA_Channel_DEST_RSTVAL			(0x00000000UL)
#define DMA_Channel_LLI_RSTVAL			(0x00000000UL)


/* DMA Channelx interrupt pending bit masks */
#define DMA_Channel0_FLAG_Clr			(0x00000001UL)
#define DMA_Channel1_FLAG_Clr			(0x00000002UL)
#define DMA_Channel2_FLAG_Clr			(0x00000004UL)
#define DMA_Channel3_FLAG_Clr			(0x00000008UL)
#define DMA_Channel4_FLAG_Clr			(0x00000010UL)
#define DMA_Channel5_FLAG_Clr			(0x00000020UL)
#define DMA_Channel6_FLAG_Clr			(0x00000040UL)
#define DMA_Channel7_FLAG_Clr			(0x00000080UL)

/* DMA registers Masks */
#define DMA_LLI_COMPLETE_IT_Mask		(0x80000000UL)
#define DMA_Channel_CON_CLEAR_Mask		(0x80000FFFUL)
#define DMA_Channel_CFG_CLEAR_Mask		(0xFFFFC001UL)
#define DMA_CFG_CLEAR_Mask				(0xFFFFFFF9UL)
#define DMA_LENGTH_Mask					(0x00000FFFUL)
#define DMA_ENABLE_Mask					(0x00000001UL)

#define DMA_CONTROLLER_ENABLE			(0x00000001UL)
#define DMA_SRC_MASTER2_ENABLE			(0x01000000UL)
#define DMA_CHANNEL_ENABLE				(0x00000001UL)

#define DMA_MEMORY_TO_PERIPH_OFFS		(0x00000006UL)
#define DMA_PERIPH_TO_MEMORY_OFFS		(0x00000001UL)


/**
  * @}
  */
  
 
/** @defgroup DMA_Exported_Functions DMA Exported Functions 
  * @{
  */
  
  
/**
  * @brief	  Deinitializes the DMA Channelx registers to their default reset values.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @retval None
  */
void DMA_DeInit(DMA_Channel_TypeDef * DMA_Channelx)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	
	/* Reset DMA Channelx configuration register */
	DMA_Channelx->CONFIGURATION = DMA_Channel_CFG_RSTVAL;
	
	/* Reset DMA Channelx control register */
	DMA_Channelx->CONTROL = DMA_Channel_CON_RSTVAL;
	
	/* Reset DMA Channelx destination register */
	DMA_Channelx->DESTADDR = DMA_Channel_DEST_RSTVAL;
	
	/* Reset DMA Channelx source register */
	DMA_Channelx->SRCADDR = DMA_Channel_SRC_RSTVAL;
	
	/* Reset DMA Channelx linked list item register */
	DMA_Channelx->LLI = DMA_Channel_LLI_RSTVAL;
	
	if (DMA_Channelx == DMA_Channel0)
	{
		/* Reset interrupt pending bits for DMA Channel0 */
		DMA->INTTCCLR = DMA_Channel0_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel0_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel0_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel1)
	{
		/* Reset interrupt pending bits for DMA Channel1 */
		DMA->INTTCCLR = DMA_Channel1_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel1_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel1_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel2)
	{
		/* Reset interrupt pending bits for DMA Channel2 */
		DMA->INTTCCLR = DMA_Channel2_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel2_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel2_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel3)
	{
		/* Reset interrupt pending bits for DMA Channel3 */
		DMA->INTTCCLR = DMA_Channel3_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel3_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel3_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel4)
	{
		/* Reset interrupt pending bits for DMA Channel4 */
		DMA->INTTCCLR = DMA_Channel4_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel4_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel4_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel5)
	{
		/* Reset interrupt pending bits for DMA Channel5 */
		DMA->INTTCCLR = DMA_Channel5_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel5_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel5_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel6)
	{
		/* Reset interrupt pending bits for DMA Channel6 */
		DMA->INTTCCLR = DMA_Channel6_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel6_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel6_FLAG_Clr;
	}
	else if (DMA_Channelx == DMA_Channel7)
	{
		/* Reset interrupt pending bits for DMA Channel7 */
		DMA->INTTCCLR = DMA_Channel7_FLAG_Clr;
		DMA->INTERRCLR = DMA_Channel7_FLAG_Clr;
		DMA->ENBLDCHNS &= ~DMA_Channel7_FLAG_Clr;
	}
}

/**
  * @brief  Initializes the DMA Channelx according to the specified
  *   parameters in the DMA_InitStruct.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @param[in]  DMA_InitStruct: pointer to a DMA_InitTypeDef structure that
  *   contains the configuration information for the specified DMA Channel.
  * @retval None
  */
void DMA_Init(DMA_Channel_TypeDef* DMA_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
	uint32_t tmpreg = 0;
	
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	assert_param(IS_DMA_ALL_REQUEST(DMA_InitStruct->RequestSelect));
	assert_param(IS_DMA_DIR(DMA_InitStruct->Direction));
	assert_param(IS_DMA_DEST_INC_STATE(DMA_InitStruct->DestInc));
	assert_param(IS_DMA_SRC_INC_STATE(DMA_InitStruct->SrcInc)); 
	//assert_param(IS_DMA_ENDIAN_TRANS_STATE(DMA_InitStruct->EndiannessTrans));
	assert_param(IS_DMA_DEST_ALIGN(DMA_InitStruct->DestDataAlignment));
	assert_param(IS_DMA_SRC_ALIGN(DMA_InitStruct->SrcDataAlignment));
	assert_param(IS_DMA_DEST_BURST(DMA_InitStruct->DestBurst));
	assert_param(IS_DMA_SRC_BURST(DMA_InitStruct->SrcBurst));
	assert_param(IS_DMA_IT_MASK(DMA_InitStruct->IT_Enable));
	
/*--------------------------- DMA Channelx CONTROL Configuration -----------------*/
	/* Get the DMA_Channelx Control value */	
	tmpreg = DMA_Channelx->CONTROL;
	
	/* Clear all bits except interrupt enable bit and length bits*/
	tmpreg &= DMA_Channel_CON_CLEAR_Mask;
	
	/* Set DMA destination incremented mode */
	/* Set DMA source incremented mode */
	/* Set DMA Destination data size */
	/* Set DMA source data size */
	/* Set DMA destination burst */
	/* Set DMA source burst */
	/* Set DMA select master2 for source controller */
	tmpreg |= DMA_InitStruct->DestInc | DMA_InitStruct->SrcInc | 
			  DMA_InitStruct->DestDataAlignment | DMA_InitStruct->SrcDataAlignment | 
			  DMA_InitStruct->DestBurst | DMA_InitStruct->SrcBurst |
			  DMA_SRC_MASTER2_ENABLE | DMA_InitStruct->IT_Enable;

	/* Set the DMA_Channelx control register */
	DMA_Channelx->CONTROL = tmpreg;

	
/*--------------------------- DMA Channelx cfg Configuration -----------------*/
	/* Get the DMA_Channelx Control value */									
	tmpreg = DMA_Channelx->CONFIGURATION; 
	tmpreg &= DMA_Channel_CFG_CLEAR_Mask;
	
	/* Set the DMA_Channelx configuration  direction*/ 
	tmpreg |= DMA_InitStruct->Direction;
	
	/* Set the DMA_Channelx configuration  request*/
	if((DMA_InitStruct->Direction == DMA_CTRL_MEMORY_TO_PERIPH)||(DMA_InitStruct->Direction == PERIPH_CTRL_MEMORY_TO_PERIPH))
		tmpreg |= DMA_InitStruct->RequestSelect << DMA_MEMORY_TO_PERIPH_OFFS;
	else if((DMA_InitStruct->Direction == DMA_CTRL_PERIPH_TO_MEMORY)||(DMA_InitStruct->Direction == PERIPH_CTRL_PERIPH_TO_MEMORY))
		tmpreg |= DMA_InitStruct->RequestSelect << DMA_PERIPH_TO_MEMORY_OFFS;
	
	/* Set the DMA_Channelx configuration register */
	DMA_Channelx->CONFIGURATION = tmpreg;
	if(DMA_InitStruct->DecAHBMx_Select&DMA_AHBM2_SELECT)
	{
		DMA_Channelx->CONTROL|=DMA_AHBM2_SELECT<<25;
	}
	else
	{
		DMA_Channelx->CONTROL&=~(DMA_AHBM2_SELECT<<25);
	}
	if(DMA_InitStruct->SrcAHBMx_Select&DMA_AHBM2_SELECT)
	{
		DMA_Channelx->CONTROL|=DMA_AHBM2_SELECT<<24;
	}
	else
	{
		DMA_Channelx->CONTROL&=~(DMA_AHBM2_SELECT<<24);
	}
/*--------------------------- DMA cfg Configuration -----------------*/
	/* enable DMA controller */	   
	DMA->CONFIG |= DMA_CONTROLLER_ENABLE; 
	
	
}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param[in]  DMA_InitStruct : pointer to a DMA_InitTypeDef structure which will
  *   be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
	DMA_InitStruct->DestBurst = DMA_DEST_BURST_INC1;
	DMA_InitStruct->DestDataAlignment = DMA_DEST_ALIGN_BYTE;
	DMA_InitStruct->DestInc = DMA_DEST_INC_ENABLE;
	
	DMA_InitStruct->Direction = DMA_CTRL_MEMORY_TO_MEMORY;
	DMA_InitStruct->RequestSelect = 0;
	
	DMA_InitStruct->SrcBurst = DMA_SRC_BURST_INC1;
	DMA_InitStruct->SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
	DMA_InitStruct->SrcInc = DMA_SRC_INC_ENABLE;
}

/**
  * @brief  Set the DMA Transfer address and length.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @param[in]  SrcAddress: The source memory Buffer address
  * @param[in]  DstAddress: The destination memory Buffer address
  * @param[in]  DataLength: The length of data to be transferred from source to destination
  * @retval None
  */
void DMA_SetAddress(DMA_Channel_TypeDef* DMA_Channelx, uint32_t SrcAddress, uint32_t DstAddress, uint32_t DataLength)
{
	volatile uint32_t temp;
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	
	DMA_Channelx->DESTADDR = DstAddress;
	DMA_Channelx->SRCADDR = SrcAddress;
	temp = DMA_Channelx->CONTROL;
	temp &= ~DMA_LENGTH_Mask;
	temp |= (DataLength & DMA_LENGTH_Mask);
	DMA_Channelx->CONTROL = temp;
}

/**
  * @brief  Set the DMA Transfer length.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @param[in]  SrcAddress: The source memory Buffer address
  * @param[in]  DstAddress: The destination memory Buffer address
  * @param[in]  DataLength: The length of data to be transferred from source to destination
  * @retval None
  */
void DMA_SetLength(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DataLength)
{
	volatile uint32_t temp;
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	
	temp = DMA_Channelx->CONTROL;
	temp &= ~DMA_LENGTH_Mask;
	temp |= (DataLength & DMA_LENGTH_Mask);
	DMA_Channelx->CONTROL = temp;
}
/**
  * @brief  Enables or disables the specified DMA Channelx.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @param[in]  NewState: new state of the DMA Channelx. 
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMA_Channelx, FunctionalState NewStatus)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	assert_param(IS_FUNCTIONAL_STATE(NewStatus));
	
	if(NewStatus != DISABLE)
	{
		/* Enable the selected DMA channelx */
		DMA_Channelx->CONFIGURATION |= DMA_CHANNEL_ENABLE;
	}
	else
	{
		/* Enable the selected DMA channelx */
		DMA_Channelx->CONFIGURATION &= (~DMA_CHANNEL_ENABLE);
	}
}

/**
  * @brief  Set the DMA Transfer address and length.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @param[in]  LLIAddress: Specifies the linked list item address of chain operation mode.
	   if this value is 0 indicate that there is no chain.
  * @retval the Address's Bit1 and Bit0 must be 0
  */
void DMA_SetChainListAddress(DMA_Channel_TypeDef* DMA_Channelx, uint32_t LLIAddress)
{	
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	
	DMA_Channelx->LLI = LLIAddress;
}	

/**
  * @brief  Perform endian conversion when DMA transfer.
  * @param[in]  EndiannessTrans : enable or disable endian conversion.
  * This parameter can be one of the following values:
  *	@arg DMA_ENDIAN_TRANS_ENABLE
  *	@arg DMA_ENDIAN_TRANS_DISABLE
  * @retval None
  * @note The EndiannessTrans is effective to all DMA channels. 
  */
void DMA_SetEndianTransform(uint32_t EndiannessTrans)
{	
	/* Check the parameters */
	assert_param(IS_DMA_ENDIAN_TRANS_STATE(EndiannessTrans));
	
	/* Write to CONFIG register */
	DMA->CONFIG &= DMA_CFG_CLEAR_Mask;
	DMA->CONFIG |= EndiannessTrans;			 								  
}

/**
  * @brief  Enables or disables the specified DMA Channelx interrupts.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @param[in]  DMA_IT: specifies the DMA interrupts sources to be enabled or disabled. 
  *   This parameter can be any combination of the following values:
  *	 @arg DMA_IT_TC:  Transfer complete interrupt 
  *	 @arg DMA_IT_ERR:  Transfer error interrupt 
  * @param  NewState: new state of the specified DMA interrupts.
  *   This parameter can be: ENABLE or DISABLE.
  * @retval None
  */
void DMA_ITConfig(DMA_Channel_TypeDef* DMA_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	assert_param(IS_DMA_CONFIG_IT(DMA_IT));
	assert_param(IS_FUNCTIONAL_STATE(NewState));

	if (NewState != DISABLE)
	{
		/* Enable the selected DMA interrupts */
		DMA_Channelx->CONFIGURATION |= DMA_IT;
	}
	else
	{
		/* Disable the selected DMA interrupts */
		DMA_Channelx->CONFIGURATION &= ~DMA_IT;
	}
}


/**
  * @brief  Returns the number of remaining data units in the current
  *   DMA Channelx transfer.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @retval The number of remaining data units in the current DMA Channelx transfer.
  */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMA_Channelx)
{
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	
	/* Return the number of remaining data units for DMA Channelx */
	return ((uint16_t)(DMA_Channelx->CONTROL & DMA_LENGTH_Mask));
}

/**
  * @brief  Checks whether the specified DMA Channelx flag is set or not.
  * @param[in]  DMA_FLAG: specifies the flag to check.
  *   This parameter can be one of the following values:
  *	 @arg DMA_FLAG_TC0: DMA Channel0 transfer complete flag.
  *	 @arg DMA_FLAG_TC1: DMA Channel1 transfer complete flag.
  *	 @arg DMA_FLAG_TC2: DMA Channel2 transfer complete flag.
  *	 @arg DMA_FLAG_TC3: DMA Channel3 transfer complete flag.
  *	 @arg DMA_FLAG_TC4: DMA Channel4 transfer complete flag.
  *	 @arg DMA_FLAG_TC5: DMA Channel5 transfer complete flag.
  *	 @arg DMA_FLAG_TC6: DMA Channel6 transfer complete flag.
  *	 @arg DMA_FLAG_TC7: DMA Channel7 transfer complete flag.
  *	 @arg DMA_FLAG_ERR0: DMA Channel0 transfer error flag.
  *	 @arg DMA_FLAG_ERR1: DMA Channel1 transfer error flag.
  *	 @arg DMA_FLAG_ERR2: DMA Channel2 transfer error flag.
  *	 @arg DMA_FLAG_ERR3: DMA Channel3 transfer error flag.
  *	 @arg DMA_FLAG_ERR4: DMA Channel4 transfer error flag.
  *	 @arg DMA_FLAG_ERR5: DMA Channel5 transfer error flag.
  *	 @arg DMA_FLAG_ERR6: DMA Channel6 transfer error flag.
  *	 @arg DMA_FLAG_ERR7: DMA Channel7 transfer error flag.
  * @retval The new state of DMA_FLAG (SET or RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_Flag)
{
	FlagStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_DMA_GET_FLAG(DMA_Flag));

	/* Check the error status of the specified DMA flag */
	if(DMA_Flag >= DMA_FLAG_ERR0)
	{
		DMA_Flag >>= 8;
		if ((DMA->RAWINTERRSTS & DMA_Flag) != (uint32_t)RESET)
		{
			/* DMA_FLAG is set */
			bitstatus = SET;
		}
		else
		{
			/* DMA_FLAG is reset */
			bitstatus = RESET;
		}
	}
	/* Check the correct status of the specified DMA flag */
	else
	{
		if ((DMA->RAWINTTCSTS & DMA_Flag) != (uint32_t)RESET)
		{
			/* DMA_FLAG is set */
			bitstatus = SET;
		}
		else
		{
			/* DMA_FLAG is reset */
			bitstatus = RESET;
		}
	}

	/* Return the DMA_FLAG status */
	return  bitstatus;
}

/**
  * @brief  Clears the DMA Channelx's pending flags.
  * @param[in]  DMA_FLAG: specifies the flag to clear.
  * @retval None
  */
void DMA_ClearFlag(uint32_t DMA_FLAG)
{
	/* There is nothing in this function */
}

/**
  * @brief  Checks whether the specified DMA Channelx interrupt has occurred or not.
  * @param[in]  DMA_IT: specifies the DMA interrupt source to check. 
  *   This parameter can be one of the following values:
  *	 @arg DMA_IT_TC0: DMA Channel0 transfer complete interrupt.
  *	 @arg DMA_IT_TC1: DMA Channel1 transfer complete interrupt.
  *	 @arg DMA_IT_TC2: DMA Channel2 transfer complete interrupt.
  *	 @arg DMA_IT_TC3: DMA Channel3 transfer complete interrupt.
  *	 @arg DMA_IT_TC4: DMA Channel4 transfer complete interrupt.
  *	 @arg DMA_IT_TC5: DMA Channel5 transfer complete interrupt.
  *	 @arg DMA_IT_TC6: DMA Channel6 transfer complete interrupt.
  *	 @arg DMA_IT_TC7: DMA Channel7 transfer complete interrupt.
  *	 @arg DMA_IT_ERR0: DMA Channel0 transfer error interrupt.
  *	 @arg DMA_IT_ERR1: DMA Channel1 transfer error interrupt.
  *	 @arg DMA_IT_ERR2: DMA Channel2 transfer error interrupt.
  *	 @arg DMA_IT_ERR3: DMA Channel3 transfer error interrupt.
  *	 @arg DMA_IT_ERR4: DMA Channel4 transfer error interrupt.
  *	 @arg DMA_IT_ERR5: DMA Channel5 transfer error interrupt.
  *	 @arg DMA_IT_ERR6: DMA Channel6 transfer error interrupt.
  *	 @arg DMA_IT_ERR7: DMA Channel7 transfer error interrupt.
  *	 @arg DMA_IT_GL0: DMA Channel0 global interrupt.
  *	 @arg DMA_IT_GL1: DMA Channel1 global interrupt.
  *	 @arg DMA_IT_GL2: DMA Channel2 global interrupt.
  *	 @arg DMA_IT_GL3: DMA Channel3 global interrupt.
  *	 @arg DMA_IT_GL4: DMA Channel4 global interrupt.
  *	 @arg DMA_IT_GL5: DMA Channel5 global interrupt.
  *	 @arg DMA_IT_GL6: DMA Channel6 global interrupt.
  *	 @arg DMA_IT_GL7: DMA Channel7 global interrupt.  
  * @retval The new state of DMA_IT (SET or RESET).
  */
ITStatus DMA_GetITStatus(uint32_t DMA_IT)
{
	ITStatus bitstatus = RESET;

	/* Check the parameters */
	assert_param(IS_DMA_GET_IT(DMA_IT));

	/* Check the global interrupt status of the specified DMA flag */
	if(DMA_IT >= DMA_IT_GL0)
	{
		DMA_IT >>= 16;
		if((DMA->INTSTS & DMA_IT) != (uint32_t)RESET)
		{
			/* DMA_IT is set */
			bitstatus = SET;
		}
		else
		{
			/* DMA_IT is reset */
			bitstatus = RESET;
		}	
	}
	/* Check the error interrupt status of the specified DMA flag */
	else if(DMA_IT >= DMA_IT_ERR0)
	{
		DMA_IT >>= 8;
		if((DMA->INTERRSTS & DMA_IT) != (uint32_t)RESET)
		{
			/* DMA_IT is set */
			bitstatus = SET;
		}
		else
		{
			/* DMA_IT is reset */
			bitstatus = RESET;
		}
	}
	/* Check the correct interrupt status of the specified DMA flag */
	else
	{
		/* Check the status of the specified DMA interrupt */
		if ((DMA->INTTCSTS & DMA_IT) != (uint32_t)RESET)
		{
			/* DMA_IT is set */
			bitstatus = SET;
		}
		else
		{
			/* DMA_IT is reset */
			bitstatus = RESET;
		}
	}

	/* Return the DMA_IT status */
	return  bitstatus;
}

/**
  * @brief  Clears the DMA Channelx interrupt pending bits.
  * @param[in]  DMA_IT: specifies the DMA interrupt pending bit to clear.
  *   This parameter can be any combination (for the same DMA) of the following values:
  *	 @arg DMA_IT_TC0: DMA Channel0 transfer complete interrupt.
  *	 @arg DMA_IT_TC1: DMA Channel1 transfer complete interrupt.
  *	 @arg DMA_IT_TC2: DMA Channel2 transfer complete interrupt.
  *	 @arg DMA_IT_TC3: DMA Channel3 transfer complete interrupt.
  *	 @arg DMA_IT_TC4: DMA Channel4 transfer complete interrupt.
  *	 @arg DMA_IT_TC5: DMA Channel5 transfer complete interrupt.
  *	 @arg DMA_IT_TC6: DMA Channel6 transfer complete interrupt.
  *	 @arg DMA_IT_TC7: DMA Channel7 transfer complete interrupt.
  *	 @arg DMA_IT_ERR0: DMA Channel0 transfer error interrupt.
  *	 @arg DMA_IT_ERR1: DMA Channel1 transfer error interrupt.
  *	 @arg DMA_IT_ERR2: DMA Channel2 transfer error interrupt.
  *	 @arg DMA_IT_ERR3: DMA Channel3 transfer error interrupt.
  *	 @arg DMA_IT_ERR4: DMA Channel4 transfer error interrupt.
  *	 @arg DMA_IT_ERR5: DMA Channel5 transfer error interrupt.
  *	 @arg DMA_IT_ERR6: DMA Channel6 transfer error interrupt.
  *	 @arg DMA_IT_ERR7: DMA Channel7 transfer error interrupt.
  * @retval None
  */
void DMA_ClearITPendingBit(uint32_t DMA_IT)
{
	/* Check the parameters */
	assert_param(IS_DMA_CLEAR_IT(DMA_IT));

	if(DMA_IT >= DMA_IT_ERR0)
	{
		DMA_IT >>= 8;
		/* Clear the selected DMA interrupt pending bits */
		DMA->INTERRCLR = DMA_IT;
	}
	else
	{
		/* Clear the selected DMA interrupt pending bits */
		DMA->INTTCCLR = DMA_IT;
	}
}

/**
  * @brief  Checks whether the specified DMA Channelx has been enabled or not.
  * @param[in]  DMA_Channelx: where x can be 0 to 7 to select the DMA Channel.
  * @retval The new state of DMA_FLAG (SET or RESET).
  */
FlagStatus DMA_GetCmdStatus(DMA_Channel_TypeDef* DMA_Channelx)
{
	FlagStatus bitstatus = RESET;
	
	/* Check the parameters */
	assert_param(IS_DMA_ALL_PERIPH(DMA_Channelx));
	
	/* Return the number of remaining data units for DMA Channelx */
	if((DMA_Channelx->CONFIGURATION & DMA_ENABLE_Mask) != DMA_ENABLE_Mask)
		bitstatus = RESET;
	else
		bitstatus = SET;
	
	return bitstatus;
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

/************************ (C) COPYRIGHT TMC *****END OF FILE***********************/
