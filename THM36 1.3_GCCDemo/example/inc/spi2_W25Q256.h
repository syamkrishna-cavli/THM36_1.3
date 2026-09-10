/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: SPI2_W25Q256.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/  
#ifndef __SPI2_W25Q256_H
#define __SPI2_W25Q256_H

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

typedef enum 
{
	SQI2_Demo_Polling	= 0,
	SPI2_Demo_DMA		  = 1,
	SPI2_Demo_Polling	= 2,
	SQI2_Demo_DMA 		= 3,
	XIP_Demo		  = 4
} SPI2_Demo_TypefDef;

void SPI2_DemoMain(void);


#endif
