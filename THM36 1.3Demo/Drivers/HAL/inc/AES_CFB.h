/**
  ******************************************************************************
  * @file    THM36V1.3/AES/AES_CFB.h
  * @author  Shang Ziyun
  * @version V1.0
  * @date    01/04/2024
  * @brief   
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, TMC SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2024 TMC</center></h2>
  */

/** @addtogroup THM36V1.3
  * @{
  */
#include <stdint.h>
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _AES_CFB_H_
#define _AES_CFB_H_


#ifdef __cplusplus
 extern "C" {
#endif
     
/********************************************************************************
  *@brief	使用AES ECB方式的API实现的AES CFB算法。
  *@param  	pbKey:	密钥,分别对应AES算法128bit,192bit,256bit的16字节，24字节，32字节秘钥
  *@param  	iv:	16字节初始化向量
  *@param  	pbDataInput:	明文(加密)/密文(解密)
  *@param  	pbDataOutput:	密文(加密)/明文(解密)
  *@param  	length:	明文(加密)/密文(解密)数据长度
  *@param  	bmode:	加/解密方式，	bit0~bit3
										0xA：加密
										0x5：解密
									bit4~bit7
										0xA：AES128
										0x5：AES192
										0x3：AES256
  *@retval	AES_SUCCESS		加/解密成功
			PARAERR			参数错误
			AES_FAILED		计算错误
  *@note 	使用romTable.h指向的接口，加解密使用同一函数。
 *******************************************************************************/
extern uint16_t AES_CFB(const uint8_t *pbKey, const uint8_t *iv, const uint8_t *pbDataInput, uint8_t *pbDataOutput, uint32_t length, uint8_t bmode);

#ifdef __cplusplus
}
#endif

#endif /* _AES_CFB_H_ */
/**
  * @}
  */
/**
  * @}
  */
/**
  * @}
  */

