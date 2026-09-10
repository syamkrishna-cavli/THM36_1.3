/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: aes.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef _TMC_AES_H_
#define _TMC_AES_H_

/** @addtogroup TMC_Driver
  * @{
  */
/** @defgroup TMC_AES
  * @{
  */

/**
* @brief  AES ECB 128/192/256 encryption/decryption
* @param[in] pbKey: key for encryption/decryption
* @param[in] pbDataInput: encryption/decryption data
* @param[in] bmode
* 			bmode if bit0:1 bit2:1 AES decryption
*			bmode if bit1:1 bit3:1 AES encryption
*			bmode if bit5:1 bit7:1 AES KEY is 128Bit
*			bmode if bit4:1 bit6:1 AES KEY is 192Bit
*			bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @retval none
* @note	none
*/ 
void AES_ECB(uint8_t *pbKey, uint8_t *pbDataInput,uint32_t *pbDataOutput, uint32_t bmode);


/**
* @brief  AES ECB 128/192/256 encryption/decryption
* @param[in] pbKey: key for encryption/decryption
* @param[in] pbDataInput: encryption/decryption data
* @param[in] bmode
* 			bmode if bit0:1 bit2:1 AES decryption
*			bmode if bit1:1 bit3:1 AES encryption
*			bmode if bit5:1 bit7:1 AES KEY is 128Bit
*			bmode if bit4:1 bit6:1 AES KEY is 192Bit
*			bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @retval none
* @note	none
*/ 
uint16_t AES_SecurityECB(uint8_t *pbKey, uint8_t *pbDataInput,uint32_t *pbDataOutput, uint32_t bmode);

#endif  //_TMC_AES_H_
/**
  * @}
  */
/**
  * @}
  */

