/**
  ******************************************************************************
  * @file    XDES.h
  * @author  TMC Terminal Team
  * @version V1.0.0
  * @date    04/23/2023
  * @brief   This file provides all the XDES functions.
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
  * <h2><center>&copy; COPYRIGHT 2023 TMC</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _XDES_H_
#define _XDES_H_

#ifdef __cplusplus
 extern "C" {
#endif

/**
* @brief  Get the version of XDES Library.
* @param[in] Version: Two byte buffer.
* @retval none
* @note	none
*/  
void _GetXDESLibVersion(unsigned char *Version);

/**
  * @brief Security ECB Xdes function  
  * @param[in] bmode 
  *    @arg 0x05 Decrypt
  *    @arg 0x0A Encrypt
  *    @arg 0x50 2 Des
  *    @arg 0xA0 1 Des
  *    @arg 0x30 3 Des
  *	   @arg	0x08 Key not change
  * @param[in] pbDataIn: input data    
  * @param[in] pbDataOut: Output data  
  * @retval 0xAAAA :sucess
  *			0x5555 :error
  * @note none			
  */
uint16_t XDES_SecurityECB(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t *pbDataOut, uint8_t bmode); 

/**
  * @brief Security CBC Xdes function  
  * @param[in] bmode 
  *    @arg 0x05 Decrypt
  *    @arg 0x0A Encrypt
  *    @arg 0x50 2 Des
  *    @arg 0xA0 1 Des
  *    @arg 0x30 3 Des
  * @param[in] pbDataIn: input data    
  * @param[in] pbDataOut: Output data  
  * @param[in] pbIv: initial value 
  * @retval 0xAAAA :sucess
  *            0x5555 :error
  * @note none            
  */
uint16_t XDES_SecurityCBC(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t *pbDataOut, uint8_t *pbIv, uint8_t bmode);

#ifdef __cplusplus
}
#endif
#endif /* _XDES_H_ */
/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */

