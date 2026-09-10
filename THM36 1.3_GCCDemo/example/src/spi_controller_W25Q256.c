/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: spi_controller_W25Q256.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#include "thm36_1.3.h"
#include "tmc_dma.h"
#include "rcc.h"
#include "tmc_gpio.h"
#include "tmc_spi_controller.h"
#include "spi2_W25Q256.h"
/***********************************************************************
*	External SPI NORFLASH £ºwinbond W25Q256
************************************************************************/

#define W25_Q256_INS_CODE_WRITE_ENABLE  		0x06
#define W25_Q256_INS_CODE_WRITE_DISABLE 		0x04
#define W25_Q256_INS_CODE_4_BYTES_ADDRESS		0xB7
#define	W25_Q256_INS_CODE_WRITE_REG_2_STS   	0x31
#define	W25_Q256_INS_CODE_READ_REG_1_STS		0x05
#define	W25_Q256_INS_CODE_READ_REG_2_STS		0x31
#define W25_Q256_INS_CODE_ERASE_SECTOR_4K   	0x20
#define W25_Q256_INS_CODE_PAGE_PROGRAM			0x02
#define W25_Q256_INS_CODE_QUAD_PAGE_PROGRAM		0x32
#define W25_Q256_INS_CODE_FAST_READ				0x0B
#define W25_Q256_INS_CODE_READ					0x03

#define W25_Q256_R1_STS_S0_BUSY					(1UL)
#define W25_Q256_R1_STS_S1_WEL					(1UL<<1)
#define W25_Q256_R1_STS_S2_BP0					(1UL<<2)
#define W25_Q256_R1_STS_S3_BP1					(1UL<<3)
#define W25_Q256_R1_STS_S4_BP2					(1UL<<4)
#define W25_Q256_R1_STS_S5_BP3					(1UL<<5)
#define W25_Q256_R1_STS_S6_TB					(1UL<<6)
#define W25_Q256_R1_STS_S7_SRP0					(1UL<<7)

uint16_t XIP_Test(uint8_t * buf, uint32_t len);

__attribute__ ((aligned (4))) uint8_t CRCBuf[256]= {
                0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
                0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
                0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
                0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
                0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
                0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
                0x60,0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D,0x6E,0x6F,
                0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D,0x7E,0x7F,
                0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
                0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
                0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
                0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
                0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
                0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
                0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
                0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF,
                
                };
	
__attribute__ ((aligned (4))) uint8_t Flash_Buffer[256]=	 {	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
								0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};

__attribute__ ((aligned (4))) uint8_t Flash_Buffer1[512];
                                
typedef struct
{
	uint32_t	CRC_Mode;
	uint32_t	CRC_Read;
	FunctionalState	CRC_Result_Reverberate;
	FunctionalState	CRC_Data_Reverberate;
} CRC_InitTypeDef;

uint16_t CRC_CCITT_Compute(CRC_InitTypeDef* CRC_InitStruct,uint8_t *buf,uint32_t length);

#define  CRC_BIG_ENDIAN		1UL<<3
#define  CRC_LITTLE_ENDIAN	~(1UL<<3)

static uint8_t MemcpyCampare(uint8_t *pbDest, uint8_t *pbSrc, uint16_t wLen);
static void SPI_Flash_Test(void);
static void SPI_Flash_DMA_Test(void);
static void SQI_Flash_Test(void);
static void SQI_DMA_Flash_Test(void);
static void XIP_Flash_Test(void);
void TMC_SPI_Controller_XIPInit(SPI_Controller_HandleTypeDef *hspi);
void TMC_SPI_Controller_XIPDeInit(SPI_Controller_HandleTypeDef *hspi);


SPI_Controller_HandleTypeDef  hspi2;
SPI_Controller_CmdW1Def CmdW1;
SPI_Controller_CmdW3Def CmdW3;
SPI_Controller_CommandTypeDef hCmmd;
SPI_Controller_InitTypeDef   SPI2_Init;  
DMA_HandleTypeDef SPI2_DMA_Init;
DMA_HandleTypeDef SPI2_DMA_TxInit;
uint8_t temp0;
uint8_t temp1;
static void Test_Error(void);
uint8_t SPI2_Read_R2Status(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);
void SPI2_QSPI_ENABLE(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd);

/*****************************************************
* Flash W25Q256:Write Enable
******************************************************/
ErrorStatus SPI2_WriteEnable(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x06;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SQI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = NULL;
	Cmmd->Data_Length = NULL;
	Status = TMC_SPI_Controller_Command(hqspi, Cmmd);
	if(Status != TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}


/*****************************************************
* Flash W25Q256:Write Disable.
******************************************************/
ErrorStatus SPI2_WriteDisable(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = W25_Q256_INS_CODE_WRITE_DISABLE;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = NULL;
	Cmmd->Data_Length = NULL;
	Status = TMC_SPI_Controller_Command(hqspi, Cmmd);
	if(Status != TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}

/*****************************************************
* Flash W25Q256:enable 4byte address
******************************************************/
ErrorStatus SPI2_Enable4BytesAddrMode(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = W25_Q256_INS_CODE_4_BYTES_ADDRESS;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	

	Cmmd->Flash_Addr = NULL;
	Cmmd->Data_Length = NULL;
	Status = TMC_SPI_Controller_Command(hqspi, Cmmd);
	if(Status != TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}

/*****************************************************
* Flash W25Q256:Read status 1 register
******************************************************/
uint8_t SPI2_Read_R1Status(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x05;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = ENABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_SOFTWARE_MODE;
	

	Cmmd->Flash_Addr = NULL;
	Cmmd->Data_Length = NULL;
	
	TMC_SPI_Controller_Command(hqspi, Cmmd);
	return SPI_Controller_GetFlashStatus();
}

/*****************************************************
* Flash W25Q256 Read status 1 register.
******************************************************/
uint8_t SPI2_Read_R2Status(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x35;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = ENABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_SOFTWARE_MODE;
	

	Cmmd->Flash_Addr = 0;
	Cmmd->Data_Length = 0;
	
	TMC_SPI_Controller_Command(hqspi, Cmmd);
	return SPI_Controller_GetFlashStatus();

	
}

/*****************************************************
* Flash W25Q256:SQI Enable.
******************************************************/
void SPI2_QSPI_ENABLE(SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x31;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	

	Cmmd->Flash_Addr = NULL;
	Cmmd->Data_Length = 1;
	
	SPI_Controller_ConfigFlashAddress(Cmmd->Flash_Addr);
	SPI_Controller_Write_CWD1(Cmmd->CmdW1);
	SPI_Controller_ConfigReadOrWriteLength(Cmmd->Data_Length);
	SPI_Controller_Write_CWD3(Cmmd->CmdW3);
    while(SPI_Controller_GetStatus(SPI_CONTROLLER_STS_TXE) == RESET);
	SPI_Controller_WritedData(0x02);
	while(SPI_Controller_GetInterruptrStatus()== RESET);
	SPI_Controller_ClearInterruptrStatus();

}

/*****************************************************
* Flash W25Q256:Sector erase.
******************************************************/
ErrorStatus  SPI2_SectorErase(uint32_t Address,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	if (Address > 0xFFFFFF)
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_4_BYTE_ADDRESS_LEN;
	}
	else
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	}
	Cmmd->CmdW1->Continuous_Read =DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = W25_Q256_INS_CODE_ERASE_SECTOR_4K;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SQI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = 0;
	
	Status = TMC_SPI_Controller_Command(hqspi,Cmmd);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	
	while(SPI2_Read_R1Status(hqspi,Cmmd)&(W25_Q256_R1_STS_S0_BUSY|W25_Q256_R1_STS_S1_WEL));
	return SUCCESS;
}

/*****************************************************
* Flash W25Q256:Write operation.
******************************************************/
ErrorStatus SPI2_PageProgram(uint32_t Address,uint32_t *buf,uint32_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	if (Address > 0xFFFFFF)
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_4_BYTE_ADDRESS_LEN;
	}
	else
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	}
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x02;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	
	Status = TMC_SPI_Controller_Transmit(hqspi, Cmmd,buf,1000000);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	while(SPI2_Read_R1Status(hqspi,Cmmd)&(W25_Q256_R1_STS_S0_BUSY|W25_Q256_R1_STS_S1_WEL));
	return SUCCESS;
}

/*****************************************************
*Flash W25Q256 Program operation,, DMA mode,channel0
******************************************************/
ErrorStatus SPI2_DMA_PageProgram(uint32_t Address,uint32_t *buf,uint32_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	if (Address > 0xFFFFFF)
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_4_BYTE_ADDRESS_LEN;
	}
	else
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	}
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x02;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	
	
	/*DMA configuration*/
	SPI2_DMA_TxInit.Instance = DMA_Channel0;
	SPI2_DMA_TxInit.Init.DestDataAlignment = DMA_DEST_ALIGN_WORD;
	SPI2_DMA_TxInit.Init.DestInc = DMA_DEST_INC_DISABLE;
	SPI2_DMA_TxInit.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;
	SPI2_DMA_TxInit.Init.RequestSelect = DMA_REQUEST_SPI2;
	SPI2_DMA_TxInit.Init.SrcInc = DMA_SRC_INC_ENABLE;
	SPI2_DMA_TxInit.Init.SrcDataAlignment = DMA_SRC_ALIGN_WORD;
	SPI2_DMA_TxInit.Init.IT_Enable = DMA_IT_ENABLE;
	SPI2_DMA_TxInit.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	SPI2_DMA_TxInit.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;

	SPI2_DMA_TxInit.LLI =0;
	hqspi->hdma = &SPI2_DMA_TxInit;	
	hqspi->hdma->Parent = hqspi;
	
	TMC_DMA_DeInit(&SPI2_DMA_TxInit);
	TMC_DMA_Init(&SPI2_DMA_TxInit);
	
	Status = TMC_SPI_Controller_DMA_Transmit_IT(hqspi, Cmmd,buf);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	while(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY);
	while(SPI2_Read_R1Status(hqspi,Cmmd)&(W25_Q256_R1_STS_S0_BUSY|W25_Q256_R1_STS_S1_WEL));
	return SUCCESS;
}

/*****************************************************
* Flash W25Q256 SQI write operation.
******************************************************/
ErrorStatus SQI2_PageProgram(uint32_t Address,uint32_t *buf,uint32_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x32;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SQI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	
	Status = TMC_SPI_Controller_Transmit(hqspi, Cmmd,buf,10000);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	while(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY);
	while(SPI2_Read_R1Status(hqspi,Cmmd)&(W25_Q256_R1_STS_S0_BUSY|W25_Q256_R1_STS_S1_WEL));
	return SUCCESS;
}



/*****************************************************
* Flash W25Q256 SQI write DMA mode, DMA channel 0
******************************************************/
ErrorStatus SQI2_DMA_PageProgram(uint32_t Address,uint32_t *buf,uint32_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x32;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SQI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = ENABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	
	/*DMA Configuration.*/
	SPI2_DMA_Init.Instance = DMA_Channel0;						
	SPI2_DMA_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_WORD;	
	SPI2_DMA_Init.Init.DestInc = DMA_DEST_INC_DISABLE;			
	SPI2_DMA_Init.Init.Direction = DMA_CTRL_MEMORY_TO_PERIPH;		
	SPI2_DMA_Init.Init.RequestSelect = DMA_REQUEST_SPI2;		
	SPI2_DMA_Init.Init.SrcInc = DMA_SRC_INC_ENABLE;				
	SPI2_DMA_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_WORD;		
	SPI2_DMA_Init.Init.IT_Enable = DMA_IT_ENABLE;
	SPI2_DMA_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	SPI2_DMA_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
	SPI2_DMA_Init.LLI =0;
	hqspi->hdma = &SPI2_DMA_Init;	
	hqspi->hdma->Parent = hqspi;
	
	TMC_DMA_DeInit(&SPI2_DMA_Init);
	TMC_DMA_Init(&SPI2_DMA_Init);
	Status = TMC_SPI_Controller_DMA_Transmit_IT(hqspi, Cmmd,buf);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	
	while(SPI2_Read_R1Status(hqspi,Cmmd)&(W25_Q256_R1_STS_S0_BUSY|W25_Q256_R1_STS_S1_WEL));
	return SUCCESS;
}



/*****************************************************
* Flash W25Q256 fast read.
******************************************************/
ErrorStatus SPI2_FastRead(uint32_t Address, uint32_t * buf, uint16_t Length, SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	if (Address > 0xFFFFFF)
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_4_BYTE_ADDRESS_LEN;
	}
	else
	{
		Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	}
	
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 8;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = W25_Q256_INS_CODE_FAST_READ;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length*4;
	
	Status = TMC_SPI_Controller_Receive(hqspi,Cmmd,buf,10000);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}
/*****************************************************
* Flash W25Q256 Read.
******************************************************/
ErrorStatus SPI2_Read(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = W25_Q256_INS_CODE_READ;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	
	
	Status = TMC_SPI_Controller_Receive(hqspi,Cmmd,buf,10000);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}

/*****************************************************
* Flash W25Q256 read in DMA mode,channel 0.
******************************************************/
ErrorStatus SPI2_DMA_Read(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = W25_Q256_INS_CODE_READ;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	/*DMA ÅäÖÃ*/
	SPI2_DMA_Init.Instance = DMA_Channel0;						
	SPI2_DMA_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_WORD;	
	SPI2_DMA_Init.Init.DestInc = DMA_DEST_INC_ENABLE;			
	SPI2_DMA_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;		
	SPI2_DMA_Init.Init.RequestSelect = DMA_REQUEST_SPI2;		
	SPI2_DMA_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;				
	SPI2_DMA_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_WORD;		
	SPI2_DMA_Init.Init.IT_Enable = DMA_IT_ENABLE;
	SPI2_DMA_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	SPI2_DMA_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
	SPI2_DMA_Init.LLI =0;
	
	hqspi->hdma = &SPI2_DMA_Init;	
	hqspi->hdma->Parent = hqspi;
	TMC_DMA_DeInit(&SPI2_DMA_Init);
	TMC_DMA_Init(&SPI2_DMA_Init);
	
	Status = TMC_SPI_Controller_DMA_Receive_IT(hqspi,Cmmd,buf);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	while(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY);
	return SUCCESS;
}

/*****************************************************
Flash W25Q256 SQI fast read.
******************************************************/
ErrorStatus SPI2_QuadFastRead(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0x08;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x6B;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SQI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	Status = TMC_SPI_Controller_Receive(hqspi,Cmmd,buf,10000);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}

/*****************************************************
* Flash W25Q256 SQI fast read in DMA mode, channel 0.
******************************************************/
ErrorStatus SPI2_DMA_QuadFastRead(uint32_t Address, uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	Cmmd->CmdW1->Address_Length  = SPI_CONTROLLER_3_BYTE_ADDRESS_LEN;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 0x08;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = 0;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x6B;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SQI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Flash_Addr = Address;
	Cmmd->Data_Length = Length;
	/*DMA Configuration.*/
	SPI2_DMA_Init.Instance = DMA_Channel0;						
	SPI2_DMA_Init.Init.DestDataAlignment = DMA_DEST_ALIGN_WORD;	
	SPI2_DMA_Init.Init.DestInc = DMA_DEST_INC_ENABLE;			
	SPI2_DMA_Init.Init.Direction = DMA_CTRL_PERIPH_TO_MEMORY;		
	SPI2_DMA_Init.Init.RequestSelect = DMA_REQUEST_SPI2;		
	SPI2_DMA_Init.Init.SrcInc = DMA_SRC_INC_DISABLE;				
	SPI2_DMA_Init.Init.SrcDataAlignment = DMA_SRC_ALIGN_WORD;		
	SPI2_DMA_Init.Init.IT_Enable = DMA_IT_ENABLE;
	SPI2_DMA_Init.Init.SrcAHBMx_Select = DMA_AHBM1_SELECT;
	SPI2_DMA_Init.Init.DecAHBMx_Select = DMA_AHBM2_SELECT;
	SPI2_DMA_Init.LLI =0;
	
	hqspi->hdma = &SPI2_DMA_Init;	
	hqspi->hdma->Parent = hqspi;
	TMC_DMA_DeInit(&SPI2_DMA_Init);
	TMC_DMA_Init(&SPI2_DMA_Init);
	
	Status = TMC_SPI_Controller_DMA_Receive_IT(hqspi,Cmmd,buf);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	while(hqspi->State!= TMC_SPI_CONTROLLER_STATE_READY);
	return SUCCESS;
}
/*****************************************************
* Flash W25Q256 Read UID.
******************************************************/
ErrorStatus SPI2_ReadUID( uint32_t * buf, uint16_t Length,SPI_Controller_HandleTypeDef *hqspi,SPI_Controller_CommandTypeDef *Cmmd)
{
	TMC_StatusTypeDef Status;
	//CmdW1.Address_Length  = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Address_Length = SPI_CONTROLLER_NONE_ADDRESS;
	Cmmd->CmdW1->Continuous_Read = DISABLE;
	Cmmd->CmdW1->Dummy_Cycle = 32;
	Cmmd->CmdW1->Instruction_length = SPI_CONTROLLER_1_BYTE_LEN;
	
	Cmmd->CmdW3->Continuous_Read_Code = NULL;
	Cmmd->CmdW3->CS_Select = SPI_CONTROLLER_CS_0_SELECT;
	Cmmd->CmdW3->DTR_Mode = DISABLE;
	Cmmd->CmdW3->Instruction_Code = 0x4B;
	Cmmd->CmdW3->Oper_Mode = SPI_CONTROLLER_SPI_MODE;
	Cmmd->CmdW3->Read_Status = DISABLE;
	Cmmd->CmdW3->Write_Data = DISABLE;
	Cmmd->CmdW3->Read_Status_Mode = SPI_CONTROLLER_HARDWARE_MODE;
	
	Cmmd->Data_Length = 2;
	Cmmd->Flash_Addr = 0;
	Status = TMC_SPI_Controller_Receive(hqspi,Cmmd,buf,1000);
	if(Status!= TMC_OK)
	{
		return ERROR;
	}
	return SUCCESS;
}
/**
    * @brief  SPI2_Controller_W25Q256_Test
    *           XIP demo: XIP Init, Execute external Flash code
    *           SQI2_Demo_Polling: SQI Init, SQI polling mode to erase, write, read external Flash
    *           SQI2_Demo_DMA: SQI Init, SQI DMA mode to erase, write, read external Flash
    *           SPI2_Demo_Polling: SPI Init, SPI polling mode to erase, write, read external Flash
    *           SPI2_Demo_DMA: SPI Init, SPI DMA mode to erase, write, read external Flash
    * @param  None
    * @retval None
 */
void SPI2_Controller_W25Q256_Test(void)
{
	SPI2_Demo_TypefDef SPI2_Demo;

	hCmmd.CmdW1 = &CmdW1;
	hCmmd.CmdW3 = &CmdW3;
	TMC_TMC_SPI_Controller_StructInit(&hspi2);
	hspi2.Init = &SPI2_Init;
	
  
	//SPI2_Demo = XIP_Demo;				//XIP mode test
	SPI2_Demo = SQI2_Demo_Polling;		//SQI polling test
	//SPI2_Demo = SQI2_Demo_DMA;		//SQI DMA test
	//SPI2_Demo = SPI2_Demo_Polling;	//SPI Polling test
	//SPI2_Demo = SPI2_Demo_DMA;		//SPI DMA test
	
	if(SPI2_Demo == XIP_Demo)
	{
		XIP_Flash_Test();			
	}
	else if(SPI2_Demo == SQI2_Demo_Polling)
	{
		SQI_Flash_Test();
	}
	else if(SPI2_Demo == SQI2_Demo_DMA)
	{
		SQI_DMA_Flash_Test();
	}
	else if(SPI2_Demo == SPI2_Demo_Polling)
	{
		SPI_Flash_Test();
	}
	else if(SPI2_Demo == SPI2_Demo_DMA)
	{
		SPI_Flash_DMA_Test();
	}
	//while(1);
}

static void SPI_Flash_Test(void)
{
	TMC_SPI_Controller_DeInit(&hspi2);
	TMC_SPI_Controller_Init(&hspi2);
	
	/*Write Enable*/
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	/*SECTOR Erase*/
	if(SPI2_SectorErase(0,&hspi2,&hCmmd) == ERROR)
	{
		return;	
	}
	/*Write Enable*/
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;		
	}
	/*Program*/
	if(SPI2_PageProgram(0,(uint32_t*)Flash_Buffer,64,&hspi2,&hCmmd) == ERROR)
	{
		return;		
	}
	/*Write Enable*/
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;		
	}
	/*Program*/
	if(SPI2_PageProgram(256,(uint32_t*)Flash_Buffer,64,&hspi2,&hCmmd) == ERROR)
	{
		return;		
	}
	/*Read*/
	if(SPI2_Read(0, (uint32_t*)Flash_Buffer1, 128,&hspi2,&hCmmd)==ERROR)
	{
		return;
	}
	/*Compare*/
	if(MemcpyCampare(Flash_Buffer1,Flash_Buffer,256)==1)
	{
		Test_Error();
	}
	if(MemcpyCampare(Flash_Buffer1+256,Flash_Buffer,256)==1)
	{
		Test_Error();
	}
}

static void SPI_Flash_DMA_Test(void)
{
	uint32_t i;
	
	TMC_SPI_Controller_DeInit(&hspi2);
	TMC_SPI_Controller_Init(&hspi2);
	/*Write Enable*/
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		Test_Error();
	}
	/*SECTOR Erase*/
	if(SPI2_SectorErase(0,&hspi2,&hCmmd) == ERROR)
	{
		Test_Error();
	}
	
	/*Write Enable*/
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		Test_Error();	
	}
	for(i=0;i<1000000;i++);
	/*DMA Program*/
	if(SPI2_DMA_PageProgram(0,(uint32_t*)Flash_Buffer,64,&hspi2,&hCmmd) == ERROR)
	{
		Test_Error();
	}
	for(i=0;i<1000000;i++);
	/*DMA Read*/
	if(SPI2_DMA_Read(0, (uint32_t*)Flash_Buffer1, 64,&hspi2,&hCmmd)==ERROR)
	{
		Test_Error();
	}
	/*Compare*/
	if(MemcpyCampare(Flash_Buffer1,Flash_Buffer,256)==1)
	{
		Test_Error();
	}

}

static void SQI_Flash_Test(void)
{
	uint32_t i;
	
	TMC_SPI_Controller_DeInit(&hspi2);
	TMC_SPI_Controller_Init(&hspi2);
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	for(i=0;i<1000000;i++);
	if(SPI2_SectorErase(0,&hspi2,&hCmmd) == ERROR)
	{
		Test_Error();
	}
	for(i=0;i<1000000;i++);
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	for(i=0;i<1000000;i++);
	SPI2_QSPI_ENABLE(&hspi2,&hCmmd);
	for(i=0;i<1000000;i++);
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	for(i=0;i<1000000;i++);
	if(SQI2_PageProgram(0,(uint32_t*)Flash_Buffer,64,&hspi2,&hCmmd) == ERROR)
	{
		return;		
	}
	for(i=0;i<1000000;i++);
	if(SPI2_QuadFastRead(0, (uint32_t*)Flash_Buffer1, 64,&hspi2,&hCmmd)==ERROR)
	{
		return;		
	}
	if(MemcpyCampare(Flash_Buffer1,Flash_Buffer,256)==1)
	{
		Test_Error();
	}
}


static void SQI_DMA_Flash_Test(void)
{
	uint32_t i;
	
	TMC_SPI_Controller_DeInit(&hspi2);
	TMC_SPI_Controller_Init(&hspi2);
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	for(i=0;i<1000000;i++);
	if(SPI2_SectorErase(0,&hspi2,&hCmmd) == ERROR)
	{
		Test_Error();
	}
	for(i=0;i<1000000;i++);
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	for(i=0;i<1000000;i++);
	SPI2_QSPI_ENABLE(&hspi2,&hCmmd);
	for(i=0;i<1000000;i++);
	if(SPI2_WriteEnable(&hspi2,&hCmmd) == ERROR)
	{
		return;
	}
	for(i=0;i<1000000;i++);
	if(SQI2_DMA_PageProgram(0,(uint32_t*)Flash_Buffer,64,&hspi2,&hCmmd) == ERROR)
	{
		return;		
	}
	for(i=0;i<1000000;i++);
	if(SPI2_DMA_QuadFastRead(0, (uint32_t*)Flash_Buffer1, 64,&hspi2,&hCmmd)==ERROR)
	{
		return;		
	}
	if(MemcpyCampare(Flash_Buffer1,Flash_Buffer,256)==1)
	{
		Test_Error();
	}
}

static void XIP_Flash_Test(void)
{
  volatile uint8_t rtn;

	hspi2.Init->SPI_CLK_Mode = SPI_CONTROLLER_CPOL_LOW_LEVEL;
	hspi2.Init->Mode = SPI_CONTROLLER_XIP_MODE;
	hspi2.Init->SPI_Controller_Div = SPI_CONTROLLER_DIV_8;
	hspi2.Init->SPI_Controller_DMA = DISABLE;
	hspi2.Init->SPI_Controller_INT = DISABLE;
	SPI2->XIPCMD &= 0xFFF007FF;
	SPI2->XIPCMD |= 0x6B << 12;
	

	//TMC_SPI_Controller_Init(&hspi2);
	TMC_SPI_Controller_XIPInit(&hspi2);
	SPI_Controller_Init(hspi2.Init);
	hspi2.State = TMC_SPI_CONTROLLER_STATE_READY;
	hspi2.ErrorCode = TMC_SPI_CONTROLLER_ERROR_NONE;
	
	SPI2->SCKDLYCON |= Bit3_En ;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_CRC, ENABLE);
	if (XIP_Test(CRCBuf, 0x100) == 0x7E55)
	{
		rtn = 0x01;
	}
	TMC_SPI_Controller_XIPDeInit(&hspi2);
}

static void Test_Error(void)
{
	while(1);
}


static uint8_t MemcpyCampare(uint8_t *pbDest, uint8_t *pbSrc, uint16_t wLen)
{
	volatile uint32_t Number;
	for(Number=0;Number<wLen;Number++)
	{
		if(pbDest[Number]!= pbSrc[Number])
		{
			return 1;
		}
	}
	return 0;
}

void TMC_SPI_Controller_MspInit(SPI_Controller_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIOx_Init;
	GPIO_InitTypeDef GPIOE_Init;
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	RCC_SPI2ClockConfig(RCC_SPI2_CLKSRC_PLL_H,ENABLE);
	RCC_AHBClockCmd(RCC_AHBPeriph_SPI2,ENABLE);
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA, ENABLE);
	/*Set GPIOC PIN12/PIN13 PIN14 PIN15 as MOSI/MISO/CLK/SSN0 of SPI2.*/
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source12, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source13, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source14, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source15, GPIO_AF1_SPI2);
	TMC_GPIO_StructInit(&GPIOx_Init);				  
	GPIOx_Init.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
	GPIOx_Init.Mode = GPIO_MODE_AF;					  
	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
	TMC_GPIO_Init(GPIOC, &GPIOx_Init);
	
	/*Set GPIOD PIN0/PIN1 as D2/D3 of SPI2.*/
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source0, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source1, GPIO_AF1_SPI2);
	TMC_GPIO_StructInit(&GPIOx_Init);				  
	GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1; 
	GPIOx_Init.Mode = GPIO_MODE_AF;					  
	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	
//	TMC_GPIO_StructInit(&GPIOx_Init);				  
//	GPIOx_Init.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5; 
//	GPIOx_Init.Mode = GPIO_MODE_AF;					  
//	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
//	TMC_GPIO_Init(GPIOE, &GPIOx_Init);
//	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source3, GPIO_AF2_SPI2);
//	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source4, GPIO_AF2_SPI2);
//	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source5, GPIO_AF2_SPI2);
	
	NVIC_ClearPendingIRQ(DMA_IRQn);
	NVIC_EnableIRQ(DMA_IRQn);
}
	
void TMC_SPI_Controller_MspDeInit(SPI_Controller_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIOx_Init;
	RCC_AHBClockCmd(RCC_AHBPeriph_SPI2,DISABLE);
	
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOC, &GPIOx_Init);

	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1;
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
	GPIOx_Init.Mode = GPIO_MODE_IN;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOE, &GPIOx_Init);

}	

void TMC_SPI_Controller_XIPInit(SPI_Controller_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIOx_Init;
	GPIO_InitTypeDef GPIOE_Init;
	
	RCC_APB3PeriphClockCmd(RCC_APB3Periph_GPIO,ENABLE);
	RCC_SPI2ClockConfig(RCC_SPI2_CLKSRC_PLL_H,ENABLE);
	RCC_AHBClockCmd(RCC_AHBPeriph_SPI2,ENABLE);
	RCC_AHBClockCmd(RCC_AHBPeriph_DMA, ENABLE);
	/*Set GPIOC PIN12/PIN13 PIN14 PIN15 as MOSI/MISO/CLK/SSN0 of SPI2.*/
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source12, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source13, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source14, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOC, GPIO_PIN_Source15, GPIO_AF1_SPI2);
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
	GPIOx_Init.Mode = GPIO_MODE_AF;
	GPIOx_Init.Speed = GPIO_FAST_SPEED;
	TMC_GPIO_Init(GPIOC, &GPIOx_Init);
	
	/*Set GPIOD PIN0/PIN1 as D2/D3 of SPI2.*/
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source0, GPIO_AF1_SPI2);
	GPIO_PinAFConfig(GPIOD, GPIO_PIN_Source1, GPIO_AF1_SPI2);
	TMC_GPIO_StructInit(&GPIOx_Init);				  
	GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1; 
	GPIOx_Init.Mode = GPIO_MODE_AF;					  
	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	
//	TMC_GPIO_StructInit(&GPIOx_Init);				  
//	GPIOx_Init.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5; 
//	GPIOx_Init.Mode = GPIO_MODE_AF;					  
//	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
//	TMC_GPIO_Init(GPIOE, &GPIOx_Init);
//	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source3, GPIO_AF2_SPI2);
//	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source4, GPIO_AF2_SPI2);
//	GPIO_PinAFConfig(GPIOE, GPIO_PIN_Source5, GPIO_AF2_SPI2);
	
}

void TMC_SPI_Controller_XIPDeInit(SPI_Controller_HandleTypeDef *hspi)
{
	GPIO_InitTypeDef GPIOx_Init;
	RCC_AHBClockCmd(RCC_AHBPeriph_SPI2,DISABLE);	
	
	TMC_GPIO_StructInit(&GPIOx_Init);				  
	GPIOx_Init.Pin = GPIO_PIN_12|GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15; 
	GPIOx_Init.Mode = GPIO_MODE_IN;					  
	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
	GPIOx_Init.Pull = GPIO_PULLUP;
	TMC_GPIO_Init(GPIOC, &GPIOx_Init);

	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_0|GPIO_PIN_1; 	
	GPIOx_Init.Mode = GPIO_MODE_IN;					  
	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
	GPIOx_Init.Pull = GPIO_PULLUP;			  
	TMC_GPIO_Init(GPIOD, &GPIOx_Init);
	
	TMC_GPIO_StructInit(&GPIOx_Init);
	GPIOx_Init.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5; 	
	GPIOx_Init.Mode = GPIO_MODE_IN;					  
	GPIOx_Init.Speed = GPIO_FAST_SPEED;				  
	GPIOx_Init.Pull = GPIO_PULLUP;			  
	TMC_GPIO_Init(GPIOE, &GPIOx_Init);

}

void TMC_SPI_Controller_TxCpltCallback(SPI_Controller_HandleTypeDef *hspi)
{
	
}

void TMC_SPI_Controller_RxCpltCallback(SPI_Controller_HandleTypeDef *hspi)
{
	
}
void SPI2_DMA_Handle(void)
{
	if(DMA_GetITStatus(DMA_IT_GL0) == SET)
	{
		TMC_DMA_IRQHandler(hspi2.hdma);
	}
}


uint16_t XIP_Test(uint8_t * buf, uint32_t len)
{
	volatile uint16_t result = 0;
	CRC_InitTypeDef CRC_InitStruct;
	CRC_InitStruct.CRC_Data_Reverberate = DISABLE;
	CRC_InitStruct.CRC_Result_Reverberate = DISABLE;
	CRC_InitStruct.CRC_Read = CRC_LITTLE_ENDIAN;
	result = CRC_CCITT_Compute(&CRC_InitStruct, buf, len);
	return result;
}

uint16_t CRC_CCITT_Compute(CRC_InitTypeDef* CRC_InitStruct,uint8_t *buf,uint32_t length)
{
	uint32_t temp,i=0;
	volatile uint16_t Result;
	temp = CRC->CON1;
	temp &=((Bit4_Dis)&(Bit5_Dis));
	if(CRC_InitStruct->CRC_Read == CRC_LITTLE_ENDIAN)
	{
		temp|= Bit3_En;
	}
	else
	{
		temp&= Bit3_En;
	}
	if(CRC_InitStruct->CRC_Result_Reverberate == ENABLE)
	{
		temp|= Bit2_En;
	}
	else
	{
		temp&= Bit2_En;
	}
	if(CRC_InitStruct->CRC_Data_Reverberate == ENABLE)
	{
		temp|= Bit1_En;
	}
	else
	{
		temp&= Bit1_En;
	}
	CRC->CON1 = temp;
	temp=buf[0];
	if(temp!=0)
	{
		CRC->CON = temp;
	}
	for(i=0;i<length;i++)
	{
		
		CRC->DAT = buf[i];
	}
	Result = CRC->DAT;
	Result |= CRC->DAT << 8;
	return Result;
}

void SPI2_DemoMain(void)
{
	SPI2_Controller_W25Q256_Test();
}
