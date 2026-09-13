/**
  ******************************************************************************
  * @file    AES.h
  * @author  TMC Terminal Team
  * @version V1.1.0
  * @date    01/21/2016
  * @brief   This file provides all the AES functions.
  * History:
  *          2016-01-13 Original version
  ******************************************************************************  
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, TMC SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2016 TMC</center></h2>
  ******************************************************************************
  */

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
* @brief  Get the version of AES Library.
* @param[in] Version: Two bytes buffer.
* @retval none
* @note    none
*/
void _GetAESLibVersion(unsigned char *Version);

/**
* @brief  AES ECB 128/192/256 encryption/decryption
* @param[in] pbKey: key for encryption/decryption
* @param[in] pbDataInput: encryption/decryption data
* @param[in] pbIv: Initial value
* @param[in] bmode
* 			    bmode if bit0:1 bit2:1 AES decryption
*			      bmode if bit1:1 bit3:1 AES encryption
*			      bmode if bit5:1 bit7:1 AES KEY is 128Bit
*			      bmode if bit4:1 bit6:1 AES KEY is 192Bit
*			      bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @param[out] pbDataOutput: encryption/decryption result
* @retval   other: error
*           0xAAAA: success
* @note	none
*/ 
uint16_t _AES_SecurityCBC(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t *pbDataOut, uint8_t *pbIv, uint32_t bmode);

/**
* @brief  AES ECB 128/192/256 encryption/decryption
* @param[in] pbKey: key for encryption/decryption
* @param[in] pbDataInput: encryption/decryption data
* @param[in] bmode
* 			    bmode if bit0:1 bit2:1 AES decryption
*			      bmode if bit1:1 bit3:1 AES encryption
*			      bmode if bit5:1 bit7:1 AES KEY is 128Bit
*			      bmode if bit4:1 bit6:1 AES KEY is 192Bit
*			      bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @param[out] pbDataOutput: encryption/decryption result
* @retval   other: error
*           0xAAAA: success
* @note	none
*/ 
uint16_t _AES_SecurityECB(uint8_t *pbKey, uint8_t *pbDataInput, uint8_t *pbDataOutput, uint32_t bmode);

/**
* @brief  AES ECB 128/192/256 nBytes encryption/decryption
* @param[in] pbKey: key for encryption/decryption
* @param[in] pbDataInput: encryption/decryption data
* @param[in] bmode
*            bmode if bit0:1 bit2:1 AES decryption
*            bmode if bit1:1 bit3:1 AES encryption
*            bmode if bit5:1 bit7:1 AES KEY is 128Bit
*            bmode if bit4:1 bit6:1 AES KEY is 192Bit
*            bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @param[in] len: Data length in bytes.
*			 	  The length should be a multiple of 16 bytes.
* @param[out] pbDataOutput: encryption/decryption result
* @retval   other: error
*           0xAAAA: success
* @note     none
*/

uint16_t _AES_SecurityECBnBytes(uint8_t *pbKey, uint8_t *pbDataInput, uint8_t *pbDataOutput, uint32_t bmode, uint32_t len);

/**
* @brief  AES CBC 128/192/256 nBytes encryption/decryption
* @param[in] pbKey: key for encryption/decryption
* @param[in] pbDataInput: encryption/decryption data
* @param[in] pbIv: Initial value
* @param[in] bmode
*            bmode if bit0:1 bit2:1 AES decryption
*            bmode if bit1:1 bit3:1 AES encryption
*            bmode if bit5:1 bit7:1 AES KEY is 128Bit
*            bmode if bit4:1 bit6:1 AES KEY is 192Bit
*            bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @param[in] len: Data length in bytes.
*			 	  The length should be a multiple of 16 bytes.
* @param[out] pbDataOutput: encryption/decryption result
* @retval   other: error
*           0xAAAA: success
* @note    none
*/ 
uint16_t _AES_SecurityCBCnBytes(uint8_t *pbKey, uint8_t *pbDataInput, uint8_t *pbDataOutput, uint8_t *pbIv, uint32_t bmode, uint32_t len);

#endif  //_TMC_AES_H_
/**
  * @}
  */
/**
  * @}
  */

