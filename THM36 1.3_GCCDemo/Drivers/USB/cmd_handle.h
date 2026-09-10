/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: cmd_handle.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 

#ifndef __cmdhandle_H_
#define __cmdhandle_H_

#define SUCCESS	0
#define	IVDINS	2		//	Invalid INS,6D00
#define	P3ERROR	4		//	P3 error,6C00
#define	WRERROR	6		//	Flash write error,6501
#define	RDERROR	8		//	Verify error,6504
#define	P1P2ERR	10		//	P1 and/or P2 error,6A00
#define	SW6A88		12		//	
#define SW6W00		14		//	6E00 CLA ERR
#define SW6581		16		//	6581
#define SW9240		18		//	9240

#define SETBASE		0x50	//	Change base
#define	ALGOPER		0x56	//	DES operation: Encryption/Decrytion/Read result
#define	WRFLASH		0x58	//	Write flash
#define RDFLASH		0x5A	//	Read flash
#define	UDFLASH		0x5C	//	Update flash
#define	ERFLASH		0x2E	//	Erase flash
#define FERFLASH	0x3E	//	Fast erase flash
#define GETRDMN		0x84	//	get random number
#define CHECKCRC	0x30
#define RESERVE	 0x57

extern const uint8_t SW[];
extern const uint8_t ATR[];
extern union W2BUNION busmask;

extern uint8_t g_abComBuf[];
extern __attribute__ ((aligned (4))) uint8_t IOBuf[];

extern uint32_t Foffset;
extern uint8_t SWptr;


#define CLA	g_abComBuf[0]
#define INS	g_abComBuf[1]
#define P1	g_abComBuf[2]
#define P2	g_abComBuf[3]
#define P3	g_abComBuf[4]

void CMD_Handle(void);

void RSA_Operation(uint8_t mode,uint8_t oper,uint8_t * inputdata,uint8_t *outputdata,uint16_t len);
void CRYPTO_Operation(uint8_t mode,uint8_t oper,uint8_t * desdata,uint16_t len);
void ECC_Operation(uint8_t mode,uint8_t oper,uint8_t *inputdata,uint8_t *outputdata,uint16_t len);
void SHA256_Operation(uint8_t mode,uint8_t oper,uint8_t *inputdata,uint8_t *outputdata,uint16_t len);
void SHA1_Operation(uint8_t mode,uint8_t oper,uint8_t *inputdata,uint8_t *outputdata,uint16_t len);	
void AES_Operation(uint8_t mode,uint8_t oper,uint8_t * inputdata,uint8_t * outputdata,uint16_t len);
//static void MemcpyTrans( uint8_t *pbDest, uint8_t *pbSrc, uint16_t wLen, uint8_t trans );
void DES_Operation(uint8_t mode,uint8_t oper,uint8_t * inputdata,uint8_t * outputdata,uint16_t len);
void SetBase(void);

#endif 


