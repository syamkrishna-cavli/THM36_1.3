/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: SPI_Controller.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SPI_CONTROLLER_H
#define _SPI_CONTROLLER_H

#ifdef __cplusplus
 extern "C" {
#endif
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
/** @defgroup SPI_Controller_Exported_Constants SPI_Controller Exported Constants
  * @{
  */
	 
/** 
  * @brief  SPI controller Init structure definition
  * @note   This sturcture is used with  SPI   
  */		 
typedef struct
{
	uint32_t			SPI_CLK_Mode;
	uint32_t			Mode;
	uint32_t			SPI_Controller_Div;
	FunctionalState		SPI_Controller_DMA;
	FunctionalState		SPI_Controller_INT;	
} SPI_Controller_InitTypeDef;

typedef struct
{
	
	uint8_t 			Instruction_Code;
	uint32_t 			Address_Length;
	uint8_t 			Dummy_Cycle;
	uint8_t				CS_Select;
	uint8_t 			Oper_Mode;
	uint8_t				Quad_Dual_IO_Mode_Enable;
	uint8_t				Quad_Dual_IO_Mode_Code;
}	SPI_Controller_CmdXIPDef;


typedef struct
{
	FunctionalState		Continuous_Read;
	uint32_t 			Instruction_length;
	uint8_t 			Dummy_Cycle;
	uint8_t 			Address_Length;	
}	SPI_Controller_CmdW1Def;

typedef struct
{
	uint8_t 			Instruction_Code;
	uint8_t				Continuous_Read_Code;
	uint8_t				CS_Select;
	uint8_t				Oper_Mode;
	FunctionalState		DTR_Mode;
	uint8_t				Read_Status_Mode;
	FunctionalState		Read_Status;
	FunctionalState		Write_Data;	
} SPI_Controller_CmdW3Def;

/** @defgroup SPI_Controller_INSLEN SPI_Controller INSLEN
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_2_BYTE_LEN	(1UL<<25)
#define SPI_CONTROLLER_1_BYTE_LEN	(1UL<<24)
#define SPI_CONTROLLER_0_BYTE_LEN	(0U)

/**
  * @}
  */



/** @defgroup SPI_Controller_ADDRESS_LENGTH SPI_Controller ADDRESS LENGTH
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_4_BYTE_ADDRESS_LEN	(4UL)
#define SPI_CONTROLLER_3_BYTE_ADDRESS_LEN	(3UL)
#define SPI_CONTROLLER_2_BYTE_ADDRESS_LEN	(2UL)
#define SPI_CONTROLLER_1_BYTE_ADDRESS_LEN	(1UL)
#define SPI_CONTROLLER_NONE_ADDRESS			(0L)
/**
  * @}
  */

/** @defgroup SPI_Controller_XIP_ADDRESS_LENGTH SPI_Controller XIP ADDRESS LENGTH
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_4_BYTE_XIP_ADDRESS_LEN	(1UL<<11)
#define SPI_CONTROLLER_3_BYTE_XIP_ADDRESS_LEN	(0UL)
/**
  * @}
  */

/** @defgroup SPI_Controller_CS_SELECT SPI_Controller CS SELECT
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_CS_0_SELECT	(0UL)
#define SPI_CONTROLLER_CS_1_SELECT	(1UL)
#define SPI_CONTROLLER_CS_2_SELECT	(2UL)
#define SPI_CONTROLLER_CS_3_SELECT	(3UL)

/**
  * @}
  */
/** @defgroup SPI_Controller_XIP_CS_SELECT SPI_Controller XIP CS SELECT
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_XIP_CS_0_SELECT	(0UL)
#define SPI_CONTROLLER_XIP_CS_1_SELECT	(1UL)
#define SPI_CONTROLLER_XIP_CS_2_SELECT	(2UL)
#define SPI_CONTROLLER_XIP_CS_3_SELECT	(3UL)

/**
  * @}
  */
/** @defgroup SPI_Controller_Mode SPI_Controller Mode
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_QUAD_IO_MODE	(4UL)
#define SPI_CONTROLLER_DUAL_IO_MODE	(3UL)
#define SPI_CONTROLLER_SQI_MODE		(2UL)
#define SPI_CONTROLLER_SDI_MODE		(1UL)
#define SPI_CONTROLLER_SPI_MODE		(0UL)
/**
  * @}
  */

/** @defgroup SPI_Controller_XIP_Mode SPI_Controller XIP Mode
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_XIP_QUAD_IO_MODE (4UL<<8)
#define SPI_CONTROLLER_XIP_DUAL_IO_MODE	(3UL<<8)
#define SPI_CONTROLLER_XIP_SQI_MODE		(2UL<<8)
#define SPI_CONTROLLER_XIP_SDI_MODE		(1UL<<8)
#define SPI_CONTROLLER_XIP_SPI_MODE		(0UL)
/**
  * @}
  */

/** @defgroup SPI_Controller_Get_Status_Mode SPI_Controller Get Status Mode 
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_SOFTWARE_MODE	(8UL)
#define SPI_CONTROLLER_HARDWARE_MODE	(0UL)

/**
  * @}
  */
  
/** @defgroup SPI_Controller_Mode SPI_Controller Mode 
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_XIP_MODE	(1<<20)
#define SPI_CONTROLLER_CMD_MODE	(0)

/**
  * @}
  */

/** @defgroup SPI_Controller_CPOL_Mode SPI_Controller CPOL Mode
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_CPOL_HIGH_LEVEL	(1<<4)
#define SPI_CONTROLLER_CPOL_LOW_LEVEL	(0)

/**
  * @}
  */

/** @defgroup SPI_Controller_Div SPI_Controller Div
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_DIV_8	3
#define SPI_CONTROLLER_DIV_6	2
#define SPI_CONTROLLER_DIV_4	1
#define SPI_CONTROLLER_DIV_2	0
/**
  * @}
  */

/** @defgroup SPI_Controller_FLAG SPI_Controller FLAG
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_STS_RXF	2
#define SPI_CONTROLLER_STS_TXE	1
/**
  * @}
  */

/** @defgroup SPI_Controller_Rx_FIFO_Thod SPI_Controller Rx FIFO Thod
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_RX_FIFO_24_WORD	2
#define SPI_CONTROLLER_RX_FIFO_16_WORD	1
#define SPI_CONTROLLER_RX_FIFO_8_WORD	0
/**
  * @}
  */

/** @defgroup SPI_Controller_Tx_FIFO_Thod SPI_Controller Tx FIFO Thod
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_TX_FIFO_24_WORD	2
#define SPI_CONTROLLER_TX_FIFO_16_WORD	1
#define SPI_CONTROLLER_TX_FIFO_8_WORD	0
/**
  * @}
  */

/** @defgroup SPI_Controller_SCKIN_MODE SPI_Controller SCKIN MODE
  * @brief   
  * @{
  */ 
#define SPI_CONTROLLER_SCKIN_DOUBLE_IO		(1UL<<4)
#define SPI_CONTROLLER_SCKIN_SINGLE_IO		0

/**
  * @}
  */

/** @defgroup SPI_Controller_private_macros SPI_Controller private macros
  * @{
  */ 
#define IS_SPI_CONTROLLER_CHECK_INSLEN(mode)		((mode == SPI_CONTROLLER_2_BYTE_LEN)||(mode == SPI_CONTROLLER_1_BYTE_LEN)||(mode == SPI_CONTROLLER_0_BYTE_LEN))	
#define IS_SPI_CONTROLLER_CHECK_DUMCYC(mode)		((mode)<33)  
#define IS_SPI_CONTROLLER_CHECK_ADDRESS_LEN(mode)	((mode == SPI_CONTROLLER_4_BYTE_ADDRESS_LEN)|| \
													 (mode == SPI_CONTROLLER_3_BYTE_ADDRESS_LEN)|| \
													 (mode == SPI_CONTROLLER_2_BYTE_ADDRESS_LEN)|| \
													 (mode == SPI_CONTROLLER_1_BYTE_ADDRESS_LEN)|| \
													 (mode == SPI_CONTROLLER_NONE_ADDRESS))
#define IS_SPI_CONTROLLER_CHECK_CS(mode) 			((mode == SPI_CONTROLLER_CS_0_SELECT)|| \
													 (mode == SPI_CONTROLLER_CS_1_SELECT)|| \
													 (mode == SPI_CONTROLLER_CS_2_SELECT)|| \
													 (mode == SPI_CONTROLLER_CS_3_SELECT))
													 
#define IS_SPI_CONTROLLER_CHECK_MODE(mode) 			((mode == SPI_CONTROLLER_SQI_MODE)|| \
													 (mode == SPI_CONTROLLER_SDI_MODE)|| \
													 (mode == SPI_CONTROLLER_SPI_MODE)|| \
													 (mode == SPI_CONTROLLER_DUAL_IO_MODE)|| \
													 (mode == SPI_CONTROLLER_QUAD_IO_MODE))
#define IS_SPI_CONTROLLER_CHECK_SOFTWARE_MODE(mode) ((mode == SPI_CONTROLLER_SOFTWARE_MODE)|| \
													 (mode == SPI_CONTROLLER_HARDWARE_MODE))	
#define IS_SPI_CONTROLLER_CHECK_CMD_MODE(mode) 		((mode == SPI_CONTROLLER_XIP_MODE)|| \
													 (mode == SPI_CONTROLLER_CMD_MODE))	
#define IS_SPI_CONTROLLER_CHECK_CPOL(mode) 			((mode == SPI_CONTROLLER_CPOL_HIGH_LEVEL)|| \
													 (mode == SPI_CONTROLLER_CPOL_LOW_LEVEL))	
#define IS_SPI_CONTROLLER_CHECK_DIV(div) 			((div == SPI_CONTROLLER_DIV_8)|| \
													 (div == SPI_CONTROLLER_DIV_6)|| \
													 (div == SPI_CONTROLLER_DIV_4)|| \
													 (div == SPI_CONTROLLER_DIV_2))	
#define IS_SPI_CONTROLLER_CHECK_CS_DELAY(cycle) 	(cycle<16)	
#define IS_SPI_CONTROLLER_CHECK_RX_FIFO(thod) 		((thod == SPI_CONTROLLER_RX_FIFO_24_WORD)|| \
													 (thod == SPI_CONTROLLER_RX_FIFO_16_WORD)|| \
					 								 (thod == SPI_CONTROLLER_RX_FIFO_8_WORD))	

#define IS_SPI_CONTROLLER_CHECK_TX_FIFO(thod) 		((thod == SPI_CONTROLLER_TX_FIFO_24_WORD)|| \
													 (thod == SPI_CONTROLLER_TX_FIFO_16_WORD)|| \
					 								 (thod == SPI_CONTROLLER_TX_FIFO_8_WORD))	

#define IS_SPI_CONTROLLER_CHECK_XIP_ADDRESS_LENGTH(len)  ((len == SPI_CONTROLLER_4_BYTE_XIP_ADDRESS_LEN)|| \
														  (len == SPI_CONTROLLER_3_BYTE_XIP_ADDRESS_LEN))

#define IS_SPI_CONTROLLER_CHECK_XIP_MODE(mode) 		((mode == SPI_CONTROLLER_XIP_SQI_MODE)|| \
													 (mode == SPI_CONTROLLER_XIP_SDI_MODE)|| \
													 (mode == SPI_CONTROLLER_XIP_SPI_MODE)|| \
													 (mode == SPI_CONTROLLER_XIP_DUAL_IO_MODE)|| \
													 (mode == SPI_CONTROLLER_XIP_QUAD_IO_MODE))
#define IS_SPI_CONTROLLER_CHECK_SCKIN_MODE(mode) 	((mode == SPI_CONTROLLER_SCKIN_DOUBLE_IO)|| \
													 (mode == SPI_CONTROLLER_SCKIN_SINGLE_IO))	
													 
#define IS_SPI_CONTROLLER_CHECK_SCKIN_DELAY(time) 	(time<8)	
/**
  * @}
  */
  
/** @addtogroup SPI_Exported_Function 
  * @{  
  */  
void SPI_Controller_ConfigFlashAddress(uint32_t Address);
void SPI_Controller_ConfigReadOrWriteLength(uint32_t Len);
void SPI_Controller_ConfigMode(uint32_t mode);
void SPI_Controller_ConfigAbort(void);
FlagStatus SPI_Controller_GetAbortStatus(void);
void SPI_Controller_ConfigCpol(uint32_t mode);
void SPI_Controller_ConfigDiv(uint32_t div);
void SPI_Controller_ConfigCS_Dely(uint32_t cycle);
FlagStatus SPI_Controller_GetStatus(uint32_t Flag);
void SPI_Controller_ClearStatus(uint32_t Flag);
void SPI_Controller_ConfigRX_FIFO(uint32_t Thod);
void SPI_Controller_ConfigTX_FIFO(uint32_t Thod);
void SPI_Controller_ConfigCommandOverEnable(FunctionalState NewState);
void SPI_Controller_ConfigDMA_Enable(FunctionalState NewState);
FlagStatus SPI_Controller_GetInterruptrStatus(void);
void SPI_Controller_ClearInterruptrStatus(void);
uint8_t SPI_Controller_GetFlashStatus(void);
uint32_t SPI_Controller_GetdData(void);
void SPI_Controller_WritedData(uint32_t Data);
void SPI_Controller_ConfigSCKIN_Mode(uint32_t SckIn);
void SPI_Controller_ConfigSCKIN_Delay(uint8_t SCK_Delay_time);
void SPI_Controller_SCKIN_DelayCMD(FunctionalState NewState);
void SPI_Controller_StructInit(SPI_Controller_InitTypeDef* SPI_InitStruct);
void SPI_Controller_Init(SPI_Controller_InitTypeDef* SPI_InitStruct);
void SPI_Controller_DeInit(void);
void SPI_Controller_Write_CWD1(SPI_Controller_CmdW1Def* SPI_InitStruct);
void SPI_Controller_Write_CWD3(SPI_Controller_CmdW3Def* SPI_InitStruct);
void SPI_Controller_XIP_Init(SPI_Controller_CmdXIPDef* SPI_InitStruct);
void SPI_Controller_ConfigTX_FIFO(uint32_t Thod);

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

