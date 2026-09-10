/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: crc.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  


#include "thm36_1.3.h"
#include "rcc.h"
#include "crc.h"

/** @addtogroup thm36_1.3
  * @{
  */
/** @addtogroup thm36_1.3_HAL_Driver
  * @{
  */
/** @defgroup CRC
  * @brief CRC HAL modules driver 
  * @{
  */
/** @defgroup CRC_private_defines CRC private defines
  * @{
  */


/**
  * @}
  */
/** @defgroup CRC_exported_function CRC exported function
  * @{  
  */


/**
  * @brief  Fills each CRC_InitStruct member with its default value.
  * @param[in] CRC_InitStruct: pointer to a CRC_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  none
  */
__weak void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct)
{
	assert_param(IS_CRC_MODE(CRC_InitStruct->CRC_Mode));
	assert_param(IS_CRC_READ(CRC_InitStruct->CRC_Read));
	CRC_InitStruct->CRC_Mode = CRC_MODE_CCITT;
	CRC_InitStruct->CRC_Read = CRC_LITTLE_ENDIAN;
	CRC_InitStruct->CRC_Result_Reverberate = DISABLE;
	CRC_InitStruct->CRC_Data_Reverberate = DISABLE;
}

/**
  * @brief  Initializes the SPIx  peripheral according to 
  *   the specified parameters in the SPI_InitTypeDef.
  * @param[in] select the SPI1 or SPI3 peripheral.
  * @param[in] SPI_InitStruct: pointer to a SPI_InitTypeDef structure,which will be initialized.
  * @retval none
  * @note  The default value is as following
  */
void CRC_Init(CRC_InitTypeDef *CRC_InitStruct)
{
	assert_param(IS_CRC_MODE(CRC_InitStruct->CRC_Mode));
	assert_param(IS_CRC_READ(CRC_InitStruct->CRC_Read));
	if(CRC_InitStruct->CRC_Mode == CRC_MODE_CCITT)
	{
		CRC->CON1&= CRC_MODE_CCITT;
	}
	else
	{
		CRC->CON1|= CRC_MODE_32;
	}
	if(CRC_InitStruct->CRC_Read == CRC_LITTLE_ENDIAN)
	{
		CRC->CON1&= CRC_LITTLE_ENDIAN;
	}
	else
	{
		CRC->CON1|= CRC_BIG_ENDIAN;
	}
	if(CRC_InitStruct->CRC_Result_Reverberate == DISABLE)
	{
		CRC->CON1&= ~(0x04UL);
	}
	else
	{
		CRC->CON1|= (0x04UL);
	}
	if(CRC_InitStruct->CRC_Data_Reverberate == DISABLE)
	{
		CRC->CON1&= ~(0x02UL);
	}
	else
	{
		CRC->CON1|= (0x02UL);
	}
}

/**
  * @brief  Calculate CRC32
  * @param[in] initval : the initial value of CRC32
  * @param[in] *input: the data of input 
  * @param[in] *len: the len of input 
  * @param[in] *output: the Result of crc32
  * @retval 
  * @note  
  */
ErrorStatus CRC_32_Calculate(uint32_t initval,uint32_t *input,uint32_t len,uint32_t *output)
{
	uint32_t i;
	if(initval == 0)
	{
		return ERROR;
	}
	CRC->CON = initval;
	for(i=0;i<len;i++)
	{
		CRC->DAT = *input;
		input++;
	}
	*output = (uint32_t)CRC->DAT;
	return SUCCESS;
}

/**
  * @brief  Calculate CRC16
  * @param[in] initval : the initial value of CRC16
  * @param[in] *input: the data of input 
  * @param[in] *len: the len of input 
  * @param[in] *output: the Result of crc16
  * @retval 
  * @note  
  */
ErrorStatus CRC_16_Calculate(uint16_t initval,uint8_t *input,uint32_t len,uint16_t *output)
{
	uint32_t i;
	volatile uint32_t temp;
	volatile uint16_t temp1;
	if(initval == 0)
	{
		return ERROR;
	}
	CRC->CON = initval;
	for(i=0;i<len;i++)
	{
		CRC->DAT = *input;
		input++;
	}
	temp = CRC->DAT;
	temp1 = (uint16_t)(temp<<8);
	temp = CRC->DAT;
	temp1 |= (uint16_t)(temp);
	*output = temp1;
	return SUCCESS;
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

