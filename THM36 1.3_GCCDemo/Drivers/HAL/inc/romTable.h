/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: ROMTable.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 

/** @addtogroup 
  * @{
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ROMTABLE_H__
#define __ROMTABLE_H__
/* Exported struct ---------------------------------------------------------- */
typedef struct{
	unsigned char* p;
	unsigned short length;
}BigNum_t;

/* SHA */
typedef struct
{
	uint32_t blocks;
	uint8_t result[32];	
	uint32_t buffer[16];
	uint32_t length;	
}SHA_Context_t;
/* SHA end */

/* SHA1 */
//typedef struct
//{
//	uint16_t blocks;
//	uint8_t result[20];
//	uint32_t buffer[16];
//	uint16_t length;
//}SHA1_Context_t;
#define  SHA1_Context_t  SHA_Context_t
/* SHA1 end */
/* SHA384 */
typedef struct
{
	unsigned long blocks;
	unsigned char result[64];	
	unsigned long buffer[32];
	unsigned long length;	
}SHA384_Context_t;

/* SHA512 */
typedef struct
{
	unsigned long blocks;
	unsigned char result[64];	
	unsigned long buffer[32];
	unsigned long length;	
}SHA512_Context_t;

/* SHA256 */
//typedef struct
//{
//	uint16_t blocks;
//	uint8_t result[32];
//	uint32_t buffer[16];
//	uint16_t length;
//}SHA256_Context_t;
#define  SHA256_Context_t  SHA_Context_t
#define  SHA1_Context_t SHA_Context_t
#define  SHA224_Context_t  SHA_Context_t
/* SHA256 end */

/* RSA */
typedef struct
{
	BigNum_t N;
	BigNum_t E;
}RSAPubKey_t;

typedef struct
{
	BigNum_t N;
	BigNum_t D;
}RSAPrivKey_t;

typedef struct
{
	BigNum_t P;
	BigNum_t Q;
	BigNum_t DP;
	BigNum_t DQ;
	BigNum_t QInv;
}RSAPrivKeyCrt_t;

typedef struct {
	RSAPrivKey_t PriKey;
	RSAPrivKeyCrt_t PriCrtKey;
}RSAKeyGenOut_t;

/* PKE defined */
typedef struct{
	unsigned char* pucN;
	unsigned char* pucD;
	unsigned char* pucP;
	unsigned char* pucQ;
	unsigned char* pucDp;
	unsigned char* pucDq;
	unsigned char* pucIq;
}RSAKGOut_t;
/* RSA end */

/* ECC */
typedef struct{
	BigNum_t* pbnP;
	BigNum_t* pbnN;
	BigNum_t* pbnA;
	BigNum_t* pbnB;
	BigNum_t* pbnGx;
	BigNum_t* pbnGy;
}ECC_Domain_t;
/* ECC end */

/* Function pointer ---------------------------------------------------------*/
/* RSA */
typedef uint16_t (* ptmcRSAEncrypt)(BigNum_t *pResult, BigNum_t *pMessage,RSAPubKey_t *pPubKey);
typedef uint16_t (* ptmcRSADecrypt)(BigNum_t *pResult, BigNum_t *pMessage, RSAPrivKey_t *pPrivKey);
typedef uint16_t (* ptmcRSADecrypt_SU)(BigNum_t *pResult, BigNum_t *pMessage, RSAPrivKey_t *pPrivKey, BigNum_t *pE);
typedef uint16_t (* ptmcRSAKeyPairGen)(RSAKeyGenOut_t *pKey,BigNum_t *pE, uint16_t usBitLenKey, uint8_t mode);
typedef uint16_t (* ptmcRSADecryptCrt_SU)(BigNum_t *pResult,BigNum_t *pMessage, RSAPrivKeyCrt_t *pPrivKeyCrt,BigNum_t *pE);
typedef uint16_t (* ptmcRSACrtGetE)(BigNum_t *pE,  BigNum_t *pP,BigNum_t *pQ, BigNum_t *pDP, BigNum_t *pDQ);

/* ECC */	
typedef uint16_t (* ptmcECCParamCheck)(ECC_Domain_t *domain);
typedef uint16_t (* ptmcECCPointAdd)(BigNum_t *output, BigNum_t *point1, BigNum_t *point2, ECC_Domain_t *domain);
typedef uint16_t (* ptmcECCPointMul)(BigNum_t *output, BigNum_t *point, BigNum_t *K, ECC_Domain_t *domain);
typedef uint16_t (* ptmcECCKeyGen)(BigNum_t *publicKey, BigNum_t *privateKey, ECC_Domain_t *domain, uint8_t mode);
typedef uint16_t (* ptmcECCSign)(BigNum_t *output, BigNum_t* E, BigNum_t *privateKey, ECC_Domain_t *domain);
typedef uint16_t (* ptmcECCVerify)(BigNum_t *E, BigNum_t *publicKey, BigNum_t *signature, ECC_Domain_t *domain);

/* HASH */
typedef void 		(* ptmcSHA1Init)(SHA1_Context_t *c);
typedef void 		(* ptmcSHA1Update)(SHA1_Context_t *c,const void *data,uint32_t length);
typedef uint16_t 	(* ptmcSHA1Final)(uint8_t *output,SHA1_Context_t *c);
typedef void 		(* ptmcSHA256Init)(SHA256_Context_t *c);
typedef void 		(* ptmcSHA256Update)(SHA256_Context_t *c,const void *data,uint32_t length);
typedef uint16_t 	(* ptmcSHA256Final)(uint8_t *output,SHA256_Context_t *c);

typedef void		(* ptmcSHA224Init)(SHA256_Context_t *c);
typedef void		(* ptmcSHA224Update)(SHA256_Context_t *c,const void *data,uint32_t length);
typedef uint16_t	(* ptmcSHA224Final)(uint8_t *output,SHA256_Context_t *c);
typedef void 		(* ptmcSHA384Init)(SHA384_Context_t *c);
typedef void 		(* ptmcSHA384Update)(SHA384_Context_t *c,const void *data,uint32_t length);
typedef uint16_t 	(* ptmcSHA384Final)(uint8_t *output,SHA384_Context_t *c);
typedef void 		(* ptmcSHA512Init)(SHA512_Context_t *c);
typedef void 		(* ptmcSHA512Update)(SHA512_Context_t *c,const void *data,uint32_t length);
typedef uint16_t 	(* ptmcSHA512Final)(uint8_t *output,SHA512_Context_t *c);

/*-------------------------------------SEC DEPARTMENT API------------------------------*/

/* ECC */
typedef	void 	 (* pPkeECCSec_GetVer)(unsigned char* pucVer);
typedef	uint16_t (* pPkeECCSec_ParaChk)(ECC_Domain_t* pedDom);
typedef	uint16_t (* pPkeECCSec_KeyGen)(BigNum_t* pbnPubK, BigNum_t* pbnPriK, ECC_Domain_t* pedDom, unsigned char ucMode);
typedef	uint16_t (* pPkeECCSec_ECDSASign)(BigNum_t* pbnSign, BigNum_t* pbnHash, BigNum_t* pbnPriKey, ECC_Domain_t* pedDom);
typedef	uint16_t (* pPkeECCSec_ECDSAVerify)(BigNum_t* pbnSign, BigNum_t* pbnHash, BigNum_t* pbnPubKey, ECC_Domain_t* pedDom);
typedef	uint16_t (* pPkeECCSec_PtAdd)(BigNum_t* pbnPtOut, BigNum_t* pbnPt1, BigNum_t* pbnPt2, ECC_Domain_t* pedDom);
typedef	uint16_t (* pPkeECCSec_PtMul)(BigNum_t* pbnPtOut, BigNum_t* pbnPtIn, BigNum_t* pbnK, ECC_Domain_t* pedDom);

/* RSA */
typedef	void 	 (* pPkeRSASec_GetVer)(unsigned char* pucVer);
typedef	uint16_t (* pPkeRSASec_Enc)(BigNum_t* pbnC, BigNum_t* pbnM, BigNum_t* pbnN, BigNum_t*pbnE);
typedef	uint16_t (* pPkeRSASec_Dec)(BigNum_t* pbnM, BigNum_t* pbnC, BigNum_t* pbnN, BigNum_t*pbnD);
typedef	uint16_t (* pPkeRSASec_DecCRT)(BigNum_t* pbnM, BigNum_t* pbnC, BigNum_t* pbnP, BigNum_t*pbnQ, BigNum_t* pbnDp, BigNum_t* pbnDq, BigNum_t* pbnIq, BigNum_t* pbnE);
typedef	uint16_t (* pPkeRSASec_DecSTD)(BigNum_t* pbnM, BigNum_t* pbnC, BigNum_t* pbnN, BigNum_t*pbnD, BigNum_t* pbnE);
typedef uint16_t (* pPkeRSASec_Keygen)(RSAKGOut_t* prkOut, BigNum_t* pbnE, uint16_t usCLenP, uint16_t usCLenQ, uint8_t ucMode);
typedef	uint16_t (* pPkeRSASec_GetE)(BigNum_t* pbnE, BigNum_t* pbnP, BigNum_t* pbnQ, BigNum_t* pbnDp, BigNum_t* pbnDq);


/* HASH */
typedef	void 	 (* pPkeHash_GetVer)(unsigned char* pucVer);
typedef	void 	 (* pPkeSHA1_Update)(unsigned char* pucOut, unsigned char* pucIn,unsigned char* pucLastBlk, unsigned char ucInit);
typedef	uint16_t (* pPkeSHA1_Final)(unsigned char* pucOut, unsigned char* pucIn, unsigned char* pucLastBlk, uint16_t usBlkDone, uint16_t usBitLen);
typedef	uint16_t (* pPkeSHA1_Compute)(unsigned char* pucOut, unsigned char* pucIn, uint16_t usBitLen);
typedef	void 	 (* pPkeSHA256_Update)(unsigned char* pucOut, unsigned char* pucIn,unsigned char* pucLastBlk, unsigned char ucInit);
typedef	uint16_t (* pPkeSHA256_Final)(unsigned char* pucOut, unsigned char* pucIn, unsigned char* pucLastBlk, uint16_t usBlkDone, uint16_t usBitLen);
typedef	uint16_t (* pPkeSHA256_Compute)(unsigned char* pucOut, unsigned char* pucIn, uint16_t usBitLen);

/* Get version */
typedef void (* ptmcPkeGetVer)(uint8_t* pucVer);

/* PKE basic api */
typedef uint8_t (* pPkeGetRandomNum)(uint8_t *buf,uint16_t len);
typedef void (* pGetVer)(uint8_t* pucVer);
typedef void (*	pPkeCfg)(uint8_t eccA,uint8_t eccZ1,uint8_t mulCfg);
typedef void (*pInputRamMaskHandler)(unsigned char * pucMask);
typedef void (*pInputFlashWriteMaskHandler)(unsigned long StartAdder,unsigned long EndAdder,unsigned char * pucMask);
typedef void (*pEnableFlashWriteMaskHandler)(unsigned char Mode);
typedef void (*pInputFlashReadMaskHandler)(unsigned long StartAdder,unsigned long EndAdder,unsigned char * pucMask);
typedef void (*pEnableFlashReadMaskHandler)(unsigned char Mode);
typedef void (*pInputRamWriteMaskHandler)(unsigned long StartAdder,unsigned long EndAdder,unsigned char * pucMask);
typedef void (*pEnableRamWriteMaskHandler)(unsigned char Mode);
typedef void (*pInputRamReadMaskHandler)(unsigned long StartAdder,unsigned long EndAdder,unsigned char * pucMask);
typedef void (*pEnableRamReadMaskHandler)(unsigned char Mode);

/* SYM ALG */
typedef void 		(* ptmcAES_ECB)(uint8_t *pbKey, uint8_t *pbDataInput, uint8_t *pbDataOutput, uint32_t bmode);
typedef void 		(* ptmcAES_CBC)(uint8_t *pbKey, uint8_t *pbDataInput, uint8_t *pbDataOutput,uint8_t *iv, uint32_t bmode);
//typedef uint16_t 	(* ptmcAES_SecurityECB)(uint8_t *pbKey, uint8_t *pbDataInput,uint8_t *pbDataOutput, uint32_t bmode);
//typedef uint16_t 	(* ptmcAES_SecurityCBC)(uint8_t *pbKey, uint8_t *pbDataInput,uint8_t *pbDataOutput, uint8_t *iv,uint32_t bmode);
typedef void 		(* ptmcXDES_ECB)(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t * pbDataOut, uint8_t bmode);
typedef void 		(* ptmcXDES_CBC)(uint8_t *pbKey, uint8_t *pbDataInput,uint8_t *pbDataOutput,uint8_t *iv, uint8_t bmode);
//typedef uint16_t 	(* ptmcXDES_SecurityECB)(uint8_t *pbKey, uint8_t *pbDataIn, uint8_t *pbDataOut, uint8_t bmode);
//typedef uint16_t 	(* ptmcXDES_SecurityCBC)(uint8_t *pbKey, uint8_t *pbDataInput,uint8_t *pbDataOutput, uint8_t *iv,uint8_t bmode);




////for flash test
//#define RSAFUNCADDR				0x00030000
//#define	ECCFUNCADDR				0x00030038
//#define	HASHFUNCADDR			0x00030050
//#define PKERSAFUNCADDR			0x00030074

/* Address */
//#if 1
#define ROM_START_ADDRESS		0x1A000000

#define RSAFUNCADDR				0x1A000000
#define	ECCFUNCADDR				0x1A000038
#define	HASHFUNCADDR			0x1A000050
#define PKERSAFUNCADDR			0x1A000074
#define PKEECCFUNCADDR			0x1A0000b8
#define PKEHASHFUNCADDR			0x1A0000d4
#define PKEBASEAPIADDR			0x1A000100
#define HASHFUNC2ADDR		   0x1A000134
#define SYMFUNCADDR			 0x1A000158

//#else
//#define RSAFUNCADDR				0x10077000
//#define	ECCFUNCADDR				0x10077038
//#define	HASHFUNCADDR			0x10077050
//#define PKERSAFUNCADDR			0x10077074
//#define PKEECCFUNCADDR			0x100770b8
//#define PKEHASHFUNCADDR			0x100770d4
//#define PKEBASEAPIADDR			0x10077100


#endif
/* RSA */
#define		tmcRSAEncrypt 				((ptmcRSAEncrypt)(*(uint32_t *)(RSAFUNCADDR + 0)))
#define		tmcRSADecrypt 				((ptmcRSADecrypt)(*(uint32_t *)(RSAFUNCADDR + 4)))
#define		tmcRSADecrypt_SU 			((ptmcRSADecrypt_SU)(*(uint32_t *)(RSAFUNCADDR + 8)))
#define		tmcRSAKeyPairGen 			((ptmcRSAKeyPairGen)(*(uint32_t *)(RSAFUNCADDR + 0xC)))
#define		tmcRSADecryptCrt_SU 		((ptmcRSADecryptCrt_SU)(*(uint32_t *)(RSAFUNCADDR + 0x10)))
#define		tmcRSACrtGetE 				((ptmcRSACrtGetE)(*(uint32_t *)(RSAFUNCADDR + 0x14)))

/* Pke defined */
/* ECC */
#define		tmcECCParamCheck 			((ptmcECCParamCheck)(*(uint32_t *)(ECCFUNCADDR + 0)))
#define		tmcECCPointAdd 				((ptmcECCPointAdd)(*(uint32_t *)(ECCFUNCADDR + 4)))
#define		tmcECCPointMul 				((ptmcECCPointMul)(*(uint32_t *)(ECCFUNCADDR + 8)))
#define		tmcECCKeyGen 				((ptmcECCKeyGen)(*(uint32_t *)(ECCFUNCADDR + 0x0C)))
#define		tmcECCSign 					((ptmcECCSign)(*(uint32_t *)(ECCFUNCADDR + 0x10)))
#define		tmcECCVerify 				((ptmcECCVerify)(*(uint32_t *)(ECCFUNCADDR + 0x14)))

/* Hash */
#define		tmcSHA1Init 				((ptmcSHA1Init)(*(uint32_t *)(HASHFUNCADDR + 0xC)))
#define		tmcSHA1Update 				((ptmcSHA1Update)(*(uint32_t *)(HASHFUNCADDR + 0x10)))
#define		tmcSHA1Final 				((ptmcSHA1Final)(*(uint32_t *)(HASHFUNCADDR + 0x14))) 
#define		tmcSHA256Init 				((ptmcSHA256Init)(*(uint32_t *)(HASHFUNCADDR + 0x18)))
#define		tmcSHA256Update 			((ptmcSHA256Update)(*(uint32_t *)(HASHFUNCADDR + 0x1C)))
#define		tmcSHA256Final 				((ptmcSHA256Final)(*(uint32_t *)(HASHFUNCADDR + 0x20)))

/*-------------------------------------SEC DEPARTMENT API------------------------------*/

#define		PkeECCSec_ParaChk  			((pPkeECCSec_ParaChk)(*(uint32_t *)(PKEECCFUNCADDR + 0)))
#define		PkeECCSec_PtAdd  			((pPkeECCSec_PtAdd)(*(uint32_t *)(PKEECCFUNCADDR + 4)))
#define		PkeECCSec_PtMul				((pPkeECCSec_PtMul)(*(uint32_t *)(PKEECCFUNCADDR + 8)))
#define		PkeECCSec_KeyGen			((pPkeECCSec_KeyGen)(*(uint32_t *)(PKEECCFUNCADDR + 0x0c)))
#define		PkeECCSec_ECDSASign			((pPkeECCSec_ECDSASign)(*(uint32_t *)(PKEECCFUNCADDR + 0x10)))
#define		PkeECCSec_ECDSAVerify		((pPkeECCSec_ECDSAVerify)(*(uint32_t *)(PKEECCFUNCADDR + 0x14)))
#define		PkeECCSec_GetVer			((pPkeECCSec_GetVer)(*(uint32_t *)(PKEECCFUNCADDR + 0x18)))

#define		PkeRSASec_Enc  				((pPkeRSASec_Enc)(*(uint32_t *)(PKERSAFUNCADDR + 0)))
#define		PkeRSASec_Dec  				((pPkeRSASec_Dec)(*(uint32_t *)(PKERSAFUNCADDR + 4)))
#define		PkeRSASec_DecSTD			((pPkeRSASec_DecSTD)(*(uint32_t *)(PKERSAFUNCADDR + 8)))
#define		PkeRSASec_Keygen			((pPkeRSASec_Keygen)(*(uint32_t *)(PKERSAFUNCADDR + 0x0C)))
#define		PkeRSASec_DecCRT			((pPkeRSASec_DecCRT)(*(uint32_t *)(PKERSAFUNCADDR + 0x0C)))
#define 	PkeRSASec_GetE				((pPkeRSASec_GetE)(*(uint32_t *)(PKERSAFUNCADDR + 0x10)))
#define		PkeRSASec_GetVer			((pPkeRSASec_GetVer)(*(uint32_t *)(PKERSAFUNCADDR + 0x14)))

#define		PkeSHA1_Update				((pPkeSHA1_Update)(*(uint32_t *)(PKEHASHFUNCADDR + 0x0C)))
#define		PkeSHA1_Final				((pPkeSHA1_Final)(*(uint32_t *)(PKEHASHFUNCADDR + 0x10)))
#define		PkeSHA1_Compute				((pPkeSHA1_Compute)(*(uint32_t *)(PKEHASHFUNCADDR + 0x14)))
#define		PkeSHA256_Update			((pPkeSHA256_Update)(*(uint32_t *)(PKEHASHFUNCADDR + 0x18)))
#define		PkeSHA256_Final				((pPkeSHA256_Final)(*(uint32_t *)(PKEHASHFUNCADDR + 0x1C)))
#define		PkeSHA256_Compute			((pPkeSHA256_Compute)(*(uint32_t *)(PKEHASHFUNCADDR + 0x20)))
#define		PkeHash_GetVer				((pPkeHash_GetVer)(*(uint32_t *)(PKEHASHFUNCADDR + 0x28)))


#define		pkeConfig						((pPkeCfg)(*(uint32_t *)(PKEBASEAPIADDR + 0)))
#define 	pkeGetRandomNum					((pPkeGetRandomNum)(*(uint32_t *)(PKEBASEAPIADDR + 0x04)))
#define 	pkeGetVer						((pGetVer)(*(uint32_t *)(PKEBASEAPIADDR + 0x08)))
#define		pkeInputRamMaskHandler			((pInputRamMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x0C)))
#define  	pkeInputFlashWriteMask			((pInputFlashWriteMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x10)))
#define  	pkeEnableFlashWriteMask			((pEnableFlashWriteMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x14)))
#define  	pkeInputFlashReadMask			((pInputFlashReadMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x18)))
#define  	pkeEnableFlashReadMask			((pEnableFlashReadMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x1C)))
#define		PkeInputRamWriteMaskHandler 	((pInputRamWriteMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x20)))
#define		pkeEnableRamWriteMaskHandler	((pEnableRamWriteMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x24)))
#define		pkeInputRamReadMaskHandler		((pInputRamReadMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x28)))
#define 	pkeEnableRamReadMaskHandler		((pEnableRamReadMaskHandler)(*(uint32_t *)(PKEBASEAPIADDR + 0x2C)))

#define		tmcSHA224Init			    ((ptmcSHA224Init)(*(uint32_t *)(HASHFUNC2ADDR + 0x0)))
#define		tmcSHA224Update			    ((ptmcSHA224Update)(*(uint32_t *)(HASHFUNC2ADDR + 0x04)))
#define		tmcSHA224Final			    ((ptmcSHA224Final)(*(uint32_t *)(HASHFUNC2ADDR + 0x08)))
#define		tmcSHA384Init 				((ptmcSHA384Init)(*(uint32_t *)(HASHFUNC2ADDR + 0x0C)))
#define		tmcSHA384Update 			((ptmcSHA384Update)(*(uint32_t *)(HASHFUNC2ADDR + 0x10)))
#define		tmcSHA384Final 				((ptmcSHA384Final)(*(uint32_t *)(HASHFUNC2ADDR + 0x14)))
#define		tmcSHA512Init 				((ptmcSHA512Init)(*(uint32_t *)(HASHFUNC2ADDR + 0x18)))
#define		tmcSHA512Update 			((ptmcSHA512Update)(*(uint32_t *)(HASHFUNC2ADDR + 0x1C)))
#define		tmcSHA512Final 				((ptmcSHA512Final)(*(uint32_t *)(HASHFUNC2ADDR + 0x20)))


#define		 AES_ECB					    ((ptmcAES_ECB)(*(uint32_t *)(SYMFUNCADDR + 0x0)))
#define		 AES_CBC					    ((ptmcAES_CBC)(*(uint32_t *)(SYMFUNCADDR + 0x04)))
#define		 XDES_ECB					    ((ptmcXDES_ECB)(*(uint32_t *)(SYMFUNCADDR + 0x08)))
#define		 XDES_CBC 					    ((ptmcXDES_CBC)(*(uint32_t *)(SYMFUNCADDR + 0x0C)))

//#endif

