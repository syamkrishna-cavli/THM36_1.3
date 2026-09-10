/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: xdes.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _XDES_H_
#define _XDES_H_

#ifdef __cplusplus
 extern "C" {
#endif



/**
  * @brief None Secure Xdes function  
  * @param[in] bmode 
  *    @arg 0x05 Decrypt
  *    @arg 0x0A Encrypt
  *    @arg 0x50 2 Des
  *    @arg 0xA0 1 Des
  *    @arg 0x30 3 Des
  * @param[in] pbDataIn: input data    
  * @param[in] pbDataOut: Output data  
  * @retval none
  * @note none			
  */
void XDES_ECB(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t * pbDataOut, uint32_t bmode);

/**
  * @brief Security ECB Xdes function  
  * @param[in] bmode 
  *    @arg 0x05 Decrypt
  *    @arg 0x0A Encrypt
  *    @arg 0x50 2 Des
  *    @arg 0xA0 1 Des
  *    @arg 0x30 3 Des
  * @param[in] pbDataIn: input data    
  * @param[in] pbDataOut: Output data  
  * @retval 0xAAAA :sucess
  *			0x5555 :error
  * @note none			
  */
uint16_t XDES_SecurityECB(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t *pbDataOut, uint32_t bmode); 
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

