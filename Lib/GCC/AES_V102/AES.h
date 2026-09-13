/**
  ******************************************************************************
  * @file    AES.h
  * @author  TMC Terminal Team
  * @version V1.0.0
  * @date    04/23/2023
  * @brief   This file provides the AES functions of THM36 1.3 chip.
  * History:
  *          2023-04-23 Original version
  ******************************************************************************  
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, TMC SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  *       &copy; COPYRIGHT 2023 TMC
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __TMC_AES_H_
#define __TMC_AES_H_

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
* 			bmode if bit0:1 bit2:1 AES decryption
*			bmode if bit1:1 bit3:1 AES encryption
*			bmode if bit5:1 bit7:1 AES KEY is 128Bit
*			bmode if bit4:1 bit6:1 AES KEY is 192Bit
*			bmode if bit4:1 bit5:1 AES KEY is 256Bit
* @retval none
* @note    none
*/ 
uint16_t _AES_SecurityCBC(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t *pbDataOut, uint8_t *pbIv, uint32_t bmode);

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
* @note    none
*/ 
uint16_t _AES_SecurityECB(uint8_t *pbKey, uint8_t *pbDataInput, uint8_t *pbDataOutput, uint32_t bmode);

#endif   //__TMC_AES_H_
/**
  * @}
  */
/**
  * @}
  */

