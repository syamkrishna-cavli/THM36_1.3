/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: crypto_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

#ifndef __CRTPTO_DEMOMAIN_H_
#define __CRTPTO_DEMOMAIN_H_


#define AES_MODE_128_KEY_DECRYPT   0xA5
#define AES_MODE_128_KEY_ENCRYPT   0xAA
#define AES_MODE_192_KEY_DECRYPT   0x55
#define AES_MODE_192_KEY_ENCRYPT   0x5A
#define AES_MODE_256_KEY_DECRYPT   0x35
#define AES_MODE_256_KEY_ENCRYPT   0x3A

#define AES_CRYPT_DATA_LEN	0x10


#define XDES_MODE_KEY_DECRYPT	0xA5
#define XDES_MODE_KEY_ENCRYPT	0xAA
#define XDES_MODE_2KEY_DECRYPT   0x55
#define XDES_MODE_2KEY_ENCRYPT   0x5A
#define XDES_MODE_3KEY_DECRYPT   0x35
#define XDES_MODE_3KEY_ENCRYPT   0x3A


#define XDES_CRYPT_DATA_LEN	  0x08


#define RSA_GEN_KEY_MODE_VERIFY	   (1<<2)
#define RSA_GEN_KEY_MODE_MSB_BIT_1	0x02	
#define RSA_GEN_KEY_MODE_MSB_BITS_11  0x03 


#define ECC_GEN_KEY_MODE_INPTUT_PRI_KEY  0x00 
#define ECC_GEN_KEY_MODE_GEN_PRI_KEY	 0x01 



extern void CRYPTO_DemoMain(void);
#endif 

/************************ (C) COPYRIGHT TMC *****END OF FILE****/
