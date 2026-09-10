/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: crc.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CRC_H_
#define _CRC_H_

#ifdef __cplusplus
 extern "C" {
#endif
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
/** @defgroup CRC_Exported_Constants CRC Exported Constants
  * @{
  */
/** 
  * @brief  CRC Init structure definition
  * @note   This sturcture is used with  PWM   
  */	 
typedef struct
{
	uint32_t	CRC_Mode;
	uint32_t	CRC_Read;
	FunctionalState	CRC_Result_Reverberate;
	FunctionalState	CRC_Data_Reverberate;
} CRC_InitTypeDef;


/** @defgroup CRC_Mode CRC Mode
  * @{
  */
#define  CRC_MODE_32		0x30UL
#define  CRC_MODE_CCITT		~(0x30UL)

/**
  * @}
  */

/** @defgroup CRC_Read CRC Read
  * @{
  */
#define  CRC_BIG_ENDIAN		1UL<<3
#define  CRC_LITTLE_ENDIAN	~(1UL<<3)

/**
  * @}
  */


/** @defgroup CRC_Private_Macros CRC Private Macros
  * @{
  */
#define  IS_CRC_MODE(x)	((x == CRC_MODE_32)||(x == CRC_MODE_CCITT))
#define  IS_CRC_READ(x)	((x == CRC_BIG_ENDIAN)||(x == CRC_LITTLE_ENDIAN))

/**
  * @}
  */

  
/** @addtogroup CRC_Exported_Function 
  * @{  
  */   
void CRC_StructInit(CRC_InitTypeDef *CRC_InitStruct);
void CRC_Init(CRC_InitTypeDef *CRC_InitStruct);
ErrorStatus CRC_32_Calculate(uint32_t initval,uint32_t *input,uint32_t len,uint32_t *output);
ErrorStatus CRC_16_Calculate(uint16_t initval,uint8_t *input,uint32_t len,uint16_t *output);
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
#endif /* _CRC_H_ */

/************************ (C) COPYRIGHT TMC *****END OF FILE****/


