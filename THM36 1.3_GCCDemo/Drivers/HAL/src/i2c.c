/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: i2c.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 


#include "thm36_1.3.h"
#include "thm36_1.3_hal_def.h"
#include "rcc.h"
#include "i2c.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup SPIx
  * @brief SPIx HAL modules driver 
  * @{
  */
/** @defgroup SPI_private_defines SPI private defines
  * @{
  */
#define CLEAR_STARTING_SIGNAL	~(1UL<<5)
#define CLEAR_STOP_SIGNAL		~(1UL<<4)
#define I2C_INTERRUPT_FLAG		(1UL<<3)

#define I2C_ENABLE				(1UL<<6)
#define I2C_DISABLE				~(1UL<<6)
/**
  * @}
  */
/** @defgroup SPI_exported_function FLASH exported function
  * @{  
  */

/**
  * @brief  Fills each I2C_InitTypeDef member with its default value.
  * @param[in] I2C_InitStruct: pointer to a SPI_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
__weak void I2C_StructInit(I2C_InitTypeDef* I2C_InitStruct)
{
	I2C_InitStruct->Clock_Divison = PRE_FREQUENCY_DIVISON_32;
	I2C_InitStruct->I2C_CLK_Divison = I2C_CLK_DIVISON_32;
	I2C_InitStruct->Salve_Address0 = 0;
}

/**
  * @brief  Initializes the I2C  peripheral according to 
  *   the specified parameters in the SPI_InitTypeDef.
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @param[in] I2C_InitStruct: pointer to a I2C_InitStruct structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
void I2C_Init(I2C_TypeDef* I2Cx,I2C_InitTypeDef* I2C_InitStruct)
{
	assert_param(IS_I2C_CHECK_DIVSION(I2C_InitStruct->Clock_Divison));
	assert_param(IS_CHECK_I2C_CLOCK_DIVSION(I2C_InitStruct->I2C_CLK_Divison));
	I2Cx->CK = 0;
	I2Cx->CK=I2C_InitStruct->Clock_Divison|I2C_InitStruct->I2C_CLK_Divison;
	I2Cx->ADDR0 = (I2C_InitStruct->Salve_Address0)<<1;
	I2Cx->MSK &= ~1U;
}

/**
  * @brief Deinitializes the I2C peripheral registers to their default reset values.
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval none
  * @note  
  */
void I2C_DeInit(I2C_TypeDef* I2Cx)
{
	assert_param(IS_I2C_CHECK(I2Cx));
	RCC_APB2PeriphSoftReset(RCC_APB2Periph_I2C);
	while(RCC_APB2PeriphGetResetFlag(RCC_APB2Periph_I2C) == RESET);
}

/**
  * @brief I2C Output Start Signal
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @param[in] mode: 
  *		@arg GENERATE_STARTING_SIGNAL
  *		@arg NONE_GENERATE_STARTING_SIGNAL
  * @retval none
  * @note  
  */
void I2C_OutputStartSignal(I2C_TypeDef* I2Cx,uint32_t mode)
{
	assert_param(IS_I2C_CHECK_START_SIGNAL(mode));
	I2Cx->CON&=CLEAR_STARTING_SIGNAL;
	I2Cx->CON&=CLEAR_STOP_SIGNAL;
	I2Cx->CON|=mode;
	I2Cx->MSK = 0;
}

/**
  * @brief I2C Output Stop Signal
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @param[in] mode: 
  *		@arg GENERATE_STOP_SIGNAL
  *		@arg NONE_GENERATE_STOP_SIGNAL
  * @retval none
  * @note  
  */
void I2C_OutputStopSignal(I2C_TypeDef* I2Cx,uint32_t mode)
{
	assert_param(IS_I2C_CHECK_STOP_SIGNAL(mode));
	I2Cx->CON|=mode;
}


/**
  * @brief Get I2C STS whether Change
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval FlagStatus
  *		@arg SET
  *		@arg RESET
  * @note  
  */
FlagStatus I2C_GetStatusChange(I2C_TypeDef* I2Cx)
{
	if(I2Cx->CON&I2C_INTERRUPT_FLAG)
	{
		return SET;
	}
	return RESET;
}

/**
  * @brief Clear I2C STS whether Change
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @note  
  */
void I2C_ClearStatusChange(I2C_TypeDef* I2Cx)
{
	I2Cx->CON&=~I2C_INTERRUPT_FLAG;
}
/**
  * @brief I2C Config Response
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @param[in] Response
  *		@arg I2C_CLK_ANSWER_ACK
  *		@arg I2C_CLK_ANSWER_NACK
  * @retval 
  * @note  
  */
void I2C_configResponse(I2C_TypeDef* I2Cx,uint32_t Response)
{
	if(Response  == I2C_CLK_ANSWER_ACK)
	{
		I2Cx->CON|=I2C_CLK_ANSWER_ACK;
	}
	else
	{
		I2Cx->CON&=~I2C_CLK_ANSWER_ACK;
	}
}

/**
  * @brief I2C Config Response
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @param[in] none
  * @retval uint8_t : the Status code
  * @note  
  */
uint8_t I2C_GetStatus(I2C_TypeDef* I2Cx)
{
	return (uint8_t)I2Cx->STS;
}


/**
  * @brief I2C Get I2C's DATA
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
* @retval uint8_t: I2C Data for Receive
  * @note  
  */
uint8_t I2C_GetData(I2C_TypeDef* I2Cx)
{
	return (uint8_t)I2Cx->DATA;
}


/**
  * @brief I2C Send DATA
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval Data: Data for Send
  * @note  
  */
void I2C_SendData(I2C_TypeDef* I2Cx,uint8_t Data)
{
	I2Cx->DATA = Data;
}


/**
  * @brief ENABLE or DISABLE I2C
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @note  
  */
void I2C_Cmd(I2C_TypeDef* I2Cx,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		I2Cx->CON |= I2C_ENABLE;
	}
	else
	{
		I2Cx->CON &= I2C_DISABLE;
	}
}


/**
  * @brief config I2C Salve Addr0
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval Addr0: Bit7 t0 Bit1 valid
  * @note  
  */
void I2C_ConfigSalveAdder0(I2C_TypeDef* I2Cx,uint8_t Addr0)
{
	
	I2Cx->ADDR0 = Addr0<<1;
}

/**
  * @brief config I2C Interrupt
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @note  
  */
void I2C_ITConfig(I2C_TypeDef* I2Cx,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		I2Cx->MSK = 0;
	}
	else
	{
		I2Cx->MSK = 1;
	}
}


/**
  * @brief config I2C Interrupt
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval NewState
  *		@arg ENABLE
  *		@arg DISABLE
  * @note  
  */
void I2C_BroadcastEnable(I2C_TypeDef* I2Cx,FunctionalState NewState)
{
	if(NewState == ENABLE)
	{
		I2Cx->ADDR0 |= 1;
	}
	else
	{
		I2Cx->ADDR0 &= ~(1UL);
	}
}


/**
  * @brief when I2C is slave mode,config I2C 's respose is ACK or NACK
  * @param[in] select the SPI1 I2C peripheral.
  *		@arg I2C
  * @retval mode
  *		@arg I2C_CLK_ANSWER_ACK
  *		@arg I2C_CLK_ANSWER_NACK
  * @note  
  */
void I2C_ConfigSlaveResponse(I2C_TypeDef* I2Cx,uint8_t mode)
{
	assert_param(IS_I2C_CHECK_RESPOSE(mode));
	I2Cx->CON&= ~I2C_CLK_ANSWER_ACK;
	I2Cx->CON|= mode;
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


