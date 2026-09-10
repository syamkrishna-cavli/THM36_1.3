
#ifndef __TMCECCENCDECPTCHK_H__
#define __TMCECCENCDECPTCHK_H__

#include "romTable.h"

/*
* 函数原型 void tmcECCEncDecPtchkGetVer(unsigned char *version)
* 功能说明 tmcSecECC版本号读取，需要2字节空间
* 参数说明
*     [OUT]   version        输出版本号地址
* version[0]: major version
* version[1]: minor version
* Such as V1.00, version[2] = {0x01, 0x00}
* 返值说明: -
*/
void tmcECCEncDecPtchkGetVer(unsigned char * version);


/*
* 函数原型 unsigned short tmcECCEncrypt(BigNum_t * pbnCpr, BigNum_t * pbnMsg, BigNum_t * pbnPubK, BigNum_t * pbnShareData1,
                                            BigNum_t * pbnShareData2, ECC_Domain_t * domain, unsigned short usMode)
* 功能说明 ECC加密
* 参数说明
*     [OUT]   pbnCpr		输出密文
*     [IN]    pbnMsg		输入明文
*     [IN]    pbnPubK		输入公钥
*     [IN]    pbnShareData1		输入共享值1   
*     [IN]    pbnShareData2		输入共享值2
*     [IN]    pedDom		输入椭圆曲线参数组
*     [IN]    usMode		输入 模式选择   

* 
* 返值说明 -  0xAAAA	计算成功
*			  其他		计算失败，详见说明文档
*/	
unsigned short tmcECCEncrypt(BigNum_t * pbnCpr, BigNum_t * pbnMsg, BigNum_t * pbnPubK, BigNum_t * pbnShareData1,
                                            BigNum_t * pbnShareData2, ECC_Domain_t * domain, unsigned short usMode);


/*
* 函数原型 unsigned short tmcECCDecrypt(BigNum_t * pbnMsg, BigNum_t * pbnCpr, BigNum_t * pbnPriK, BigNum_t * pbnShareData1, 
	                                        BigNum_t * pbnShareData2, unsigned char* pucMask, ECC_Domain_t * domain, unsigned short usMode)
* 功能说明 ECC解密
* 参数说明
*     [OUT]   pbnMsg		输出明文
*     [IN]    pbnCpr		输入密文
*     [IN]    pbnPubK		输入私钥
*     [IN]    pbnShareData1		输入共享值1   
*     [IN]    pbnShareData2		输入共享值2
*     [IN]    ulMask			总线加密秘钥	
*     [IN]    pedDom		输入椭圆曲线参数组
*     [IN]    usMode		输入 模式选择   

* 
* 返值说明 -  0xAAAA	计算成功
*			  其他		计算失败，详见说明文档
*/
unsigned short tmcECCDecrypt(BigNum_t * pbnMsg, BigNum_t * pbnCpr, BigNum_t * pbnPriK, BigNum_t * pbnShareData1, 
	                             BigNum_t * pbnShareData2, unsigned char* pucMask, ECC_Domain_t * domain, unsigned short usMode);
		
		
/*
* 函数原型 unsigned short tmcECCPtChk(BigNum_t * bnpPoint, ECC_Domain_t * domain)
* 功能说明 验证点是否在曲线上
* 参数说明
*     [IN]    bnpPoint	输入坐标
*     [IN]    pedDom	输入椭圆曲线参数组
* 
* 返值说明 -  0xAAAA	计算成功
*			  其他		计算失败，详见说明文档
*/
unsigned short tmcECCPtChk(BigNum_t * bnpPoint, ECC_Domain_t * domain);

#endif
