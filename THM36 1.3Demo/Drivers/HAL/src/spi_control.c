/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: spi_controll.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Includes ------------------------------------------------------------------*/

#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "RCC.h"
#include "SPI_Controller.h"


/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup SPI_Controller
  * @brief SPI_Controller HAL modules driver 
  * @{
  */
/** @defgroup SPI_Controller_private_defines SPI_Controller  private defines
  * @{
  */
#define CREADEN_ENABLE			(1<<28)
#define CREADEN_DISABLE			~(1<<28)
#define CLEAR_CYCLE				(0xFF0000)
#define CLEAR_ADDRESS_LEN		(7UL)
#define CLEAR_INSCODE			(0xFF000000)
#define CLEAR_CREAD_CODE		(0xFF0000)
#define CLEAR_CS_SELECT			(0x300)
#define CLEAR_XIP_CS_SELECT		(0x60000000)
#define CLEAR_OPER_MODE			(0xE0)
#define CLEAR_GET_SATUS_MODE	(0x08)
#define CLEAR_MODE				(1UL<<20)
#define CLEAR_CPOL				(1UL<<4)
#define CLEAR_DIV				(3UL)
#define CLEAR_CS_DELAY			(0x0F)
#define CLEAR_RX_FIFO_THOD		(0x1800)
#define CLEAR_TX_FIFO_THOD		(0x300)
#define CLEAR_XIP_INSCODE		(0xFF<<12)
#define CLEAR_XIP_MODE			(0x700)
#define CLEAR_SCK_DELAY			(0x07)
#define CLEAR_XIP_CYCLE			(0xFF)
/**
  * @}
  */
/** @defgroup SPI_Controller_exported_function SPI_Controller exported function
  * @{  
  */

/**
  * @brief  config SPI_Controller CMD0 Flash Address
  * @param[in] Address: Address of Flash
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigFlashAddress(uint32_t Address)
{
	SPI2->CMDW0 = Address;
}


/**
  * @brief  config Length of Read or Write Data 
  * @param[in] Len:range 0 to 0xFFFFFFFF
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigReadOrWriteLength(uint32_t Len)
{
	SPI2->CMDW2 = Len;
}



/**
  * @brief  Config SPI_CONTROLLER Mode
  * @param[in] Mode:
  *		@arg SPI_CONTROLLER_XIP_MODE	
  *		@arg SPI_CONTROLLER_CMD_MODE
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigMode(uint32_t mode)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_CMD_MODE(mode));
	
	SPI2->CON&=~CLEAR_MODE;
	if(mode == SPI_CONTROLLER_XIP_MODE)
	{
		SPI2->CON|=mode;
	}
	while((SPI2->CON&(1UL<<8)));
}

/**
  * @brief  Config SPI_CONTROLLER Abort
  * @param[in] none
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigAbort(void)
{
	SPI2->CON|= 1UL<<8;
}

/**
  * @brief  Get SPI Controller  Abort Status
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note  
  */
FlagStatus SPI_Controller_GetAbortStatus(void)
{
	if(SPI2->CON&(1UL<<8))
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Config SPI_CONTROLLER CPOL
  * @param[in] mode
  *		@arg SPI_CONTROLLER_CPOL_HIGH_LEVEL
  *		@arg SPI_CONTROLLER_CPOL_LOW_LEVEL
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigCpol(uint32_t mode)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_CPOL(mode));
	SPI2->CON&= ~CLEAR_CPOL;
	SPI2->CON|= mode;
}

/**
  * @brief  Config SPI_CONTROLLER Divsion
  * @param[in] mode
  *		@arg SPI_CONTROLLER_DIV_8
  *		@arg SPI_CONTROLLER_DIV_6
  *		@arg SPI_CONTROLLER_DIV_4
  *		@arg SPI_CONTROLLER_DIV_2
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigDiv(uint32_t div)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_DIV(div));
	SPI2->CON&= ~CLEAR_DIV;
	SPI2->CON|= div;
}


/**
  * @brief  Config SPI_CONTROLLER CS Delay
  * @param[in] cycle:range 0 to 15
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigCS_Dely(uint32_t cycle)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_CS_DELAY(cycle));
	SPI2->CSDLY = cycle;
}

/**
  * @brief  Config SPI_CONTROLLER Get Status
  * @param[in] Flag
  *		@arg SPI_CONTROLLER_STS_RXF
  *		@arg SPI_CONTROLLER_STS_TXE
  * @retval none
  * @note  
  */
FlagStatus SPI_Controller_GetStatus(uint32_t Flag)
{
	if(Flag&SPI2->STS)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  Config SPI_CONTROLLER Clear Status
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note  
  */
void SPI_Controller_ClearStatus(uint32_t Flag)
{
	SPI2->STS = Flag;
}

/**
  * @brief  Config SPI_CONTROLLER RX FIFO
  * @param[in] Thod
  *		@arg SPI_CONTROLLER_RX_FIFO_24_WORD
  *		@arg SPI_CONTROLLER_RX_FIFO_16_WORD
  *		@arg SPI_CONTROLLER_RX_FIFO_8_WORD
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigRX_FIFO(uint32_t Thod)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_RX_FIFO(Thod));
	SPI2->INTCON&= ~CLEAR_RX_FIFO_THOD;
	SPI2->INTCON|= Thod<<12;
}


///**
//  * @brief  return RX FIFO Status
//  * @param[in] none
//  * @retval uint8_t
//  *		@arg SPI_CONTROLLER_RX_FIFO_24_WORD 
//  *		@arg SPI_CONTROLLER_RX_FIFO_16_WORD 
//  *		@arg SPI_CONTROLLER_RX_FIFO_8_WORD 
//  * @note  
//  */
//uint8_t SPI_Controller_GetRX_FIFO_Status(void)
//{
//	volatile uint32_t temp;
//	temp = (SPI2->INTCON&(3UL<<12));
//	temp = temp>>12;
//	return(uint8_t)temp;
//}

/**
  * @brief  Config SPI_CONTROLLER TX FIFO
  * @param[in] Thod
  *		@arg SPI_CONTROLLER_TX_FIFO_24_WORD
  *		@arg SPI_CONTROLLER_TX_FIFO_16_WORD
  *		@arg SPI_CONTROLLER_TX_FIFO_8_WORD
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigTX_FIFO(uint32_t Thod)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_TX_FIFO(Thod));
	SPI2->INTCON&= ~CLEAR_TX_FIFO_THOD;
	SPI2->INTCON|= Thod<<8;
}

/**
  * @brief  return TX FIFO Status
  * @param[in] none
  * @retval uint8_t
  *		@arg SPI_CONTROLLER_TX_FIFO_24_WORD 
  *		@arg SPI_CONTROLLER_TX_FIFO_16_WORD 
  *		@arg SPI_CONTROLLER_TX_FIFO_8_WORD 
  * @note  
  */
uint8_t SPI_Controller_GetTX_FIFO_Status(void)
{
	volatile uint32_t temp;
	temp = (SPI2->INTCON&(3UL<<8));
	temp = temp>>8;
	return(uint8_t)temp;
}

/**
  * @brief  ENABLE or DISABLE Command OVER Interrupt
  * @param[in] NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigCommandOverEnable(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		SPI2->INTCON |= 0x02;
	}
	else
	{
		SPI2->INTCON &= ~0x02;
	}
}

/**
  * @brief  ENABLE or DISABLE DMA
  * @param[in] NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigDMA_Enable(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		SPI2->INTCON |= 0x01;
	}
	else
	{
		SPI2->INTCON &= ~0x01;
	}
}

/**
  * @brief  SPI_Controller Get Interruptr Status
  * @param[in] none
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note  
  */
FlagStatus SPI_Controller_GetInterruptrStatus(void)
{
	if(SPI2->INTSTS)
	{
		return SET;
	}
	else
	{
		return RESET;
	}
}

/**
  * @brief  SPI_Controller Clear Interruptr Status
  * @param[in] none
  * @retval none
  * @note  
  */
void SPI_Controller_ClearInterruptrStatus(void)
{
	SPI2->INTSTS = 1;
}

/**
  * @brief  SPI_Controller return Flash Status
  * @param[in] none
  * @retval the status code of Flash
  * @note  
  */
uint8_t SPI_Controller_GetFlashStatus(void)
{
	return ((uint8_t)SPI2->RSTS);
}

/**
  * @brief  SPI_Controller Get Data
  * @param[in] none
  * @retval uint32_t the value of Dat Register
  * @note  
  */
uint32_t SPI_Controller_GetdData(void)
{
	return SPI2->DAT;
}

/**
  * @brief  SPI_Controller Write Data
  * @param[in] none
  * @retval uint32_t the value of Dat Register
  * @note  none
  */
void SPI_Controller_WritedData(uint32_t Data)
{
	SPI2->DAT = Data;
}

/**
  * @brief   SPI_Controller Config the SCKIN Delay 
  * @param[in] SCK_Delay_time: SCKIN delay time (unit ns),mix 0 max 7
  * @retval none
  * @note  
  */
void SPI_Controller_ConfigSCKIN_Delay(uint8_t SCK_Delay_time)
{
	assert_param(IS_SPI_CONTROLLER_CHECK_SCKIN_DELAY(SCK_Delay_time));
	SPI2->SCKDLYCON&=~CLEAR_SCK_DELAY;
	SPI2->SCKDLYCON|= SCK_Delay_time;
}

/**
  * @brief   SPI_Controller ENABLE or DISABLE the SCKIN Delay 
  * @param[in] NewState
  *		@arg ENABLE
  *		@ARG DISABLE
  * @retval none
  * @note  
  */
void SPI_Controller_SCKIN_DelayCMD(FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		SPI2->SCKDLYCON|=1<<3;
	}
	else
	{
		SPI2->SCKDLYCON&=~(1<<3);
	}
}

/**
  * @brief  Fills each SPI_controller_InitTypeDef member with its default value.
  * @param[in] SPI_InitStruct: pointer to a SPI_controller_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
__weak void SPI_Controller_StructInit(SPI_Controller_InitTypeDef* SPI_InitStruct)
{
	SPI_InitStruct->SPI_CLK_Mode = SPI_CONTROLLER_CPOL_LOW_LEVEL;
	SPI_InitStruct->SPI_Controller_Div = SPI_CONTROLLER_DIV_8;
	SPI_InitStruct->SPI_Controller_DMA = ENABLE;
	SPI_InitStruct->SPI_Controller_INT = DISABLE;
	SPI_InitStruct->Mode = SPI_CONTROLLER_CMD_MODE;

}


/**
  * @brief   spi controller Inital 
  * @param[in]  SPI_SetCommand
  * that contains the configuration information for the specified SPI peripheral.
  * @retval none
  */
void SPI_Controller_Init(SPI_Controller_InitTypeDef* SPI_InitStruct)
{
	
	SPI_Controller_ConfigCpol(SPI_InitStruct->SPI_CLK_Mode);
	//SPI_Controller_ConfigTX_FIFO(SPI_CONTROLLER_TX_FIFO_24_WORD);
	
	SPI_Controller_ConfigMode(SPI_InitStruct->Mode);
	SPI_Controller_ConfigDiv(SPI_InitStruct->SPI_Controller_Div);
	SPI_Controller_ConfigDMA_Enable(SPI_InitStruct->SPI_Controller_DMA);
	SPI_Controller_ConfigCommandOverEnable(SPI_InitStruct->SPI_Controller_INT);
}

/**
  * @brief   spi controller DeInital 
  * @param[in]  SPI_SetCommand
  * that contains the configuration information for the specified SPI peripheral.
  * @retval none
  */
void SPI_Controller_DeInit(void)
{
	RCC_AHBPeriphSoftReset(RCC_AHBPeriph_SPI2);
	while(RCC_AHBPeriphGetResetFlag(RCC_AHBPeriph_SPI2) == RESET);
}

/**
  * @brief   spi controller Inital CWD1 byte
  * @param[in]  SPI_InitStruct
  * that contains the configuration information for the specified SPI peripheral.
  * @retval none
  */
void SPI_Controller_Write_CWD1(SPI_Controller_CmdW1Def* SPI_InitStruct)
{
	uint32_t temp =0;
	assert_param(IS_SPI_CONTROLLER_CHECK_INSLEN(SPI_InitStruct->Instruction_length));
	assert_param(IS_SPI_CONTROLLER_CHECK_DUMCYC(SPI_InitStruct->Dummy_Cycle));
	assert_param(IS_SPI_CONTROLLER_CHECK_ADDRESS_LEN(SPI_InitStruct->Address_Length));
	if(SPI_InitStruct->Continuous_Read == ENABLE)
	{
		temp|= CREADEN_ENABLE;
	}
	temp|= SPI_InitStruct->Instruction_length;	
	temp|=SPI_InitStruct->Dummy_Cycle<<16;	
	temp|=SPI_InitStruct->Address_Length;
	SPI2->CMDW1 = temp;
}

/**
  * @brief   spi controller Inital CWD1 byte
  * @param[in]  SPI_InitStruct
  * that contains the configuration information for the specified SPI peripheral.
  * @retval none
  */
void SPI_Controller_Write_CWD3(SPI_Controller_CmdW3Def* SPI_InitStruct)
{
	uint32_t temp = 0;	
	assert_param(IS_SPI_CONTROLLER_CHECK_CS(SPI_InitStruct->CS_Select));
	assert_param(IS_SPI_CONTROLLER_CHECK_MODE(SPI_InitStruct->Oper_Mode));
	assert_param(IS_SPI_CONTROLLER_CHECK_SOFTWARE_MODE(SPI_InitStruct->Read_Status_Mode));
	temp|= SPI_InitStruct->Instruction_Code<<24;
	temp|= SPI_InitStruct->Continuous_Read_Code<<16;
	
	temp|=SPI_InitStruct->CS_Select<<8;
	temp|= SPI_InitStruct->Oper_Mode<<5;
	if(SPI_InitStruct->DTR_Mode == ENABLE)
	{
		temp|= 1<<4;
	}
	
	temp|= SPI_InitStruct->Read_Status_Mode;
	if(SPI_InitStruct->Read_Status == ENABLE)
	{
		temp|=1<<2;
	}
	if(SPI_InitStruct->Write_Data == ENABLE)
	{
		temp|=2;
	}
	SPI2->CMDW3 = temp;
}

/**
  * @brief  spi XIP controller Inital 
  * @param[in]  SPI_InitStruct
  * that contains the configuration information for the specified SPI peripheral.
  * @retval none
  */
void SPI_Controller_XIP_Init(SPI_Controller_CmdXIPDef* SPI_InitStruct)
{
	uint32_t temp = 0;
	assert_param(IS_SPI_CONTROLLER_CHECK_DUMCYC(SPI_InitStruct->Dummy_Cycle));
	assert_param(IS_SPI_CONTROLLER_CHECK_XIP_ADDRESS_LENGTH(SPI_InitStruct->Address_Length));
	assert_param(IS_SPI_CONTROLLER_CHECK_CS(SPI_InitStruct->CS_Select));
	assert_param(IS_SPI_CONTROLLER_CHECK_MODE(SPI_InitStruct->Oper_Mode));
	
	temp |= SPI_InitStruct->Dummy_Cycle;
	temp |= SPI_InitStruct->Address_Length;
	temp |= SPI_InitStruct->Instruction_Code << 12;
	temp |= SPI_InitStruct->CS_Select << 29;
	temp |= SPI_InitStruct->Oper_Mode << 8;
	if(SPI_InitStruct->Quad_Dual_IO_Mode_Enable == ENABLE)
	{
		temp |= 1<<28;
	}
	temp |= SPI_InitStruct->Quad_Dual_IO_Mode_Code << 20;
	SPI2->XIPCMD = temp;
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


