/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: SPI2_XFLASH.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#ifndef __SPI2_XFLASH_H
#define __SPI2_XFLASH_H


#define W25Q256				1
#define TH25Q_80HB 			2
#define TH25Q_16HB 			3
#define TH25Q_32HA 			4

#define XFLASH_MODEL W25Q256
#define FLASH_ERASE_VERIFY_MODE_DOUBLEWORD	(0x30000000UL)		/*!< FLASH Erase Verify Mode is DoubleWord*///(((1UL)<<29)|((1UL)<<28))


/***********************************************************************
*	芯片内部XFLASH地址映射范围
************************************************************************/
#define XFLASH_MEMORY_ADDR_BEGIN  			0x10080000
#define XFLASH_MEMORY_ADDR_END  			0x1807FFFF
#define XFLASH_MEMORY_ADDR_REMAP_BEGIN  	0x00080000
#define XFLASH_MEMORY_ADDR_REMAP_END  		0x0807FFFF
#define XFLASH_MEMORY_RANGE			  		0x08080000



/***********************************************************************
*	片上FLASH地址
************************************************************************/
#define FLASH_ADDR_START2 	(0x10000000)
#define FLASH_ADDR_END2		(0x1007FFFF)
#define	FLASH_WRITE_MAX_NUM	(1024)
#define MOD_BY_512(X)	(((uint32_t)X) & 0x000001FF)
#define DIV_BY_512(X)	(((uint32_t)X) >> 9)
#define MUL_BY_512(X)	(((uint32_t)X) << 9)
#define GET_PAGE_ADDR(X)  (((uint32_t)X) & 0xFFFFFE00)
#define MOD_BY_8(X)	(((uint32_t)X) & 0x00000007)
#define DIV_BY_8(X)	(((uint32_t)X) >> 3)
#define MUL_BY_8(X)	(((uint32_t)X) << 3)
#define GET_DW_ADDR(X)  (((uint32_t)X) & 0xFFFFFFF8)


/***********************************************************************
*	SPI NORFLASH
************************************************************************/
#if (XFLASH_MODEL == W25Q256)
#define INS_CODE_WRITE_ENABLE  			0x06
#define INS_CODE_WRITE_DISABLE 			0x04
#define INS_CODE_4_BYTES_ADDRESS		0xB7
#define	INS_CODE_WRITE_REG_2_STS   		0x31
#define	INS_CODE_READ_REG_1_STS			0x05
#define	INS_CODE_READ_REG_2_STS			0x35
#define INS_CODE_WRITE_REGS_STS			0x01
#define INS_CODE_ERASE_SECTOR_4K   		0x20
#define INS_CODE_PAGE_PROGRAM			0x02
#define INS_CODE_QUAD_PAGE_PROGRAM		0x32
#define INS_CODE_QUAD_FAST_READ			0x6B
#define INS_CODE_FAST_READ				0x0B
#define INS_CODE_READ					0x03
#define INS_CODE_READ_UNIQUE_ID			0x4B

#define R1_STS_S0_BUSY					(1UL)
#define R1_STS_S1_WEL					(1UL<<1)
#define R1_STS_S2_BP0					(1UL<<2)
#define R1_STS_S3_BP1					(1UL<<3)
#define R1_STS_S4_BP2					(1UL<<4)
#define R1_STS_S5_BP3					(1UL<<5)
#define R1_STS_S6_TB					(1UL<<6)
#define R1_STS_S7_SRP					(1UL<<7)

#define R2_STS_S0_SRL					(1UL)
#define R2_STS_S1_QE					(1UL<<1)
#define R2_STS_S2_RVD					(1UL<<2)
#define R2_STS_S3_LB1					(1UL<<3)
#define R2_STS_S4_LB2					(1UL<<4)
#define R2_STS_S5_LB3					(1UL<<5)
#define R2_STS_S6_CMP					(1UL<<6)
#define R2_STS_S7_SUS					(1UL<<7)

#define XFLAHS_SECTOR_SIZE				4096
#define XFLAHS_PAGE_SIZE				256
#define XFLASH_CAPACITY					(0x02000000)
#elif (XFLASH_MODEL == TH25Q_32HB)
#define INS_CODE_WRITE_ENABLE  			0x06
#define INS_CODE_WRITE_DISABLE 			0x04
//#define INS_CODE_4_BYTES_ADDRESS		0xB7
//#define INS_CODE_WRITE_REG_2_STS   	0x31
#define	INS_CODE_READ_REG_1_STS			0x05
#define	INS_CODE_READ_REG_2_STS			0x35
#define INS_CODE_WRITE_REGS_STS			0x01
#define INS_CODE_ERASE_SECTOR_4K   		0x20
#define INS_CODE_PAGE_PROGRAM			0x02
#define INS_CODE_QUAD_PAGE_PROGRAM		0x32
#define INS_CODE_QUAD_FAST_READ			0x6B
#define INS_CODE_FAST_READ				0x0B
#define INS_CODE_READ					0x03
#define INS_CODE_READ_UNIQUE_ID			0x4B

#define R1_STS_S0_BUSY					(1UL)
#define R1_STS_S1_WEL					(1UL<<1)
#define R1_STS_S2_BP0					(1UL<<2)
#define R1_STS_S3_BP1					(1UL<<3)
#define R1_STS_S4_BP2					(1UL<<4)
#define R1_STS_S5_BP3					(1UL<<5)
#define R1_STS_S6_TB					(1UL<<6)
#define R1_STS_S7_SRP					(1UL<<7)

#define R2_STS_S0_SRL					(1UL)
#define R2_STS_S1_QE					(1UL<<1)
#define R2_STS_S2_RVD					(1UL<<2)
#define R2_STS_S3_LB1					(1UL<<3)
#define R2_STS_S4_LB2					(1UL<<4)
#define R2_STS_S5_LB3					(1UL<<5)
#define R2_STS_S6_CMP					(1UL<<6)
#define R2_STS_S7_SUS					(1UL<<7)

#define XFLAHS_SECTOR_SIZE				4096
#define XFLAHS_PAGE_SIZE				256
#define XFLASH_CAPACITY					(0x00400000)
#elif (XFLASH_MODEL == TH25Q_16HB)
#define INS_CODE_WRITE_ENABLE  			0x06
#define INS_CODE_WRITE_DISABLE 			0x04
//#define INS_CODE_4_BYTES_ADDRESS		0xB7
//#define INS_CODE_WRITE_REG_2_STS   	0x31
#define	INS_CODE_READ_REG_1_STS			0x05
#define	INS_CODE_READ_REG_2_STS			0x35
#define INS_CODE_WRITE_REGS_STS			0x01
#define INS_CODE_ERASE_SECTOR_4K   		0x20
#define INS_CODE_PAGE_PROGRAM			0x02
#define INS_CODE_QUAD_PAGE_PROGRAM		0x32
#define INS_CODE_QUAD_FAST_READ			0x6B
#define INS_CODE_FAST_READ				0x0B
#define INS_CODE_READ					0x03
#define INS_CODE_READ_UNIQUE_ID			0x4B

#define R1_STS_S0_BUSY					(1UL)
#define R1_STS_S1_WEL					(1UL<<1)
#define R1_STS_S2_BP0					(1UL<<2)
#define R1_STS_S3_BP1					(1UL<<3)
#define R1_STS_S4_BP2					(1UL<<4)
#define R1_STS_S5_BP3					(1UL<<5)
#define R1_STS_S6_TB					(1UL<<6)
#define R1_STS_S7_SRP					(1UL<<7)

#define R2_STS_S0_SRL					(1UL)
#define R2_STS_S1_QE					(1UL<<1)
#define R2_STS_S2_RVD					(1UL<<2)
#define R2_STS_S3_LB1					(1UL<<3)
#define R2_STS_S4_LB2					(1UL<<4)
#define R2_STS_S5_LB3					(1UL<<5)
#define R2_STS_S6_CMP					(1UL<<6)
#define R2_STS_S7_SUS					(1UL<<7)

#define XFLAHS_SECTOR_SIZE				4096
#define XFLAHS_PAGE_SIZE				256
#define XFLASH_CAPACITY					(0x00200000)
#elif (XFLASH_MODEL == TH25Q_80HB)
#define INS_CODE_WRITE_ENABLE  			0x06
#define INS_CODE_WRITE_DISABLE 			0x04
// #define INS_CODE_4_BYTES_ADDRESS		0xB7
#define INS_CODE_WRITE_REG_STS			0x01
// #define	INS_CODE_WRITE_REG_2_STS   	0x31
#define	INS_CODE_READ_REG_1_STS			0x05
#define	INS_CODE_READ_REG_2_STS			0x35
#define INS_CODE_WRITE_REGS_STS			0x01
#define INS_CODE_ERASE_SECTOR_1K   		0x8B
#define INS_CODE_ERASE_SECTOR_4K   		0x20
#define INS_CODE_PAGE_PROGRAM			0x02
#define INS_CODE_QUAD_PAGE_PROGRAM		0x32
#define INS_CODE_QUAD_FAST_READ			0x6B
#define INS_CODE_FAST_READ				0x0B
#define INS_CODE_READ					0x03

#define R1_STS_S0_WIP					(1UL)
#define R1_STS_S1_WEL					(1UL<<1)
#define R1_STS_S2_BP0					(1UL<<2)
#define R1_STS_S3_BP1					(1UL<<3)
#define R1_STS_S4_BP2					(1UL<<4)
#define R1_STS_S5_BP3					(1UL<<5)
#define R1_STS_S6_BP4					(1UL<<6)
#define R1_STS_S7_SRP0					(1UL<<7)

#define R2_STS_S0_SRP1					(1UL)
#define R2_STS_S1_QE					(1UL<<1)
#define R2_STS_S2_SUS2					(1UL<<2)
#define R2_STS_S3_LB1					(1UL<<3)
#define R2_STS_S4_LB2					(1UL<<4)
#define R2_STS_S5_LB3					(1UL<<5)
#define R2_STS_S6_CMP					(1UL<<6)
#define R2_STS_S7_SUS					(1UL<<7)
#define XFLAHS_SECTOR_SIZE				4096
#define XFLAHS_PAGE_SIZE				256
#define XFLASH_CAPACITY					(0x00100000)
#endif



typedef enum 
{
  TMC_XFLASH_OK	   		 = 0x00U,
  TMC_XFLASH_ADDR_ERR	 = 0x01U,
  TMC_XFLASH_MODE_ERR	 = 0x02U,
  TMC_XFLASH_EXEC_ERR    = 0x03U
} TMC_XFLASH_FuncStatus;

typedef enum 
{
	SQI2_Demo_Polling	= 0,
	SPI2_Demo_DMA		  = 1,
	SPI2_Demo_Polling	= 2,
	SQI2_Demo_DMA 		= 3,
	XIP_Demo		  = 4,
    SPI2_XFLASH_Demo     = 5,
    IFLASH_PROG_BY_BYTE = 6
} SPI2_Demo_TypefDef;


typedef enum
{
	SPI2_CTRL_MODE_CMD = 0,
	SPI2_CTRL_MODE_XIP = 1

}XFLASH_MODE;

void Set_SPI2_XIPCMD(void);
inline void Set_SPI2_XIPCMD(void)
{
    SPI2->XIPCMD &= 0xFFF007FF;
	SPI2->XIPCMD |= 0x6B << 12;
}

ErrorStatus SPI2_WriteEnable(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_WriteDisable(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_Enable4BytesAddrMode(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_WriteRegister2Status(uint32_t buf,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
uint8_t SPI2_Read_R1Status(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus  SPI2_SectorErase(uint32_t Address,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_PageProgram(uint32_t Address,uint32_t *buf,uint32_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SQI2_PageProgram(uint32_t Address,uint32_t *buf,uint32_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_FastRead(uint32_t Address, uint32_t * buf, uint16_t Length, SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_Read(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_QuadFastRead(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
ErrorStatus SPI2_DMA_QuadFastRead(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);

void SPI2_Controller_W25Q256_Test(void);
void SPI2_DMA_Handle(void);
void SPI2_Demo_Main(void);

/**
  * @brief  获取 SPI 控制器工作模式
  * @param[in]  void
  * @retval XFLASH_MODE status
  */
XFLASH_MODE TMC_Get_SPI_Controller_Mode(void);

/**
  * @brief  将 MCU 空间的地址转换为 外部 FLASH 的地址 
  * @param[in]  addr MCU 地址	 单位 byte
  * @param[in]	target_addr  外部 FLASH 地址
  * @retval TMC_xflash_func_status status
  */
extern TMC_XFLASH_FuncStatus TMC_MCU_Addr_to_XFLASH_Addr(uint32_t source_addr, uint32_t * target_addr);


/**
  * @brief  将指定内容写入到指定地址
  * @param[in]  addr 需要擦除的MCU地址（芯片存贮器映射后外部FLASH地址空间）
  * @param[in]	len  需要擦除的数据长度
  * @param[in]	write_buffer  需要擦除的数据内容
  * @retval TMC_xflash_func_status status
  */
extern TMC_XFLASH_FuncStatus TMC_XFLASH_Erase(uint32_t addr, uint32_t len);


/**
  * @brief  将指定内容写入到指定地址
  * @param[in]  addr 需要擦除的MCU地址（芯片存贮器映射后外部FLASH地址空间）
  * @param[in]	len  需要擦除的数据长度
  * @param[in]	write_buffer  需要擦除的数据内容
  * @retval TMC_xflash_func_status status
  */
extern TMC_XFLASH_FuncStatus TMC_XFLASH_Program(uint32_t addr, uint32_t len, uint8_t * buffer);


/**
  * @brief  设置SPI 控制器工作模式 XIP, CMD
  * @param[in]  mode SPI2(SPI控制器)工作模式
  * @retval TMC_xflash_func_status status
  */
extern TMC_XFLASH_FuncStatus TMC_SPI_Controller_Set_Mode(XFLASH_MODE mode);


/**
  * @prototype  resetFlashDcache
  * @brief  	复位FLASH Dcache
  * @param[in]  void
  * @retval 	void
  * @note:	
  *
  */
extern void TMC_Reset_Flash_Dcache(void);

/**
  * @prototype  hwWriteFlash
  * @brief  	支持将不大于 1024 字节长度的数据写入到 flash 地址
  * @param[in]  destAddr: Flash 目标物理地址
  * @param[in]  pbData: 	需要写入的数据指针
  * @param[in]  wDataLen: 	需要写入的数据长度(不超过 1024)
  * @retval TMC_StatusTypeDef
  * @note:	双字 == 8 字节
  */
extern TMC_StatusTypeDef TMC_FLASH_Program(uint32_t destAddr, uint8_t * pbData, uint32_t wDataLen);


#define XFLASH_DEBUG


void SPI2_DemoMain(void);


#endif
