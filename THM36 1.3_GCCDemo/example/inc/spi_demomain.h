 /********************************************************************************
* Copyright (c) 2012, Beijing Tongxin Microelectroics Co., Ltd.
* All rights reserved.
* Module: SPI demomain.h
* Author: Yang Song
* Version: V1.0
* History:
*   2012-09-24 Original version
********************************************************************************/
#ifndef __SPI1_DEMO_H
#define __SPI1_DEMO_H

#include "stdint.h"

extern uint8_t SPITestBuffer1[256];
extern uint8_t SPITestBuffer_tx[256];
extern uint8_t SPITestBuffer_rx[256];
extern uint8_t SPITestBuffer2[256];
extern uint8_t IRQ_CompleteFlag;

#define DMA_MODE 							1
#define SPI1_IRQ_RECEIVE_FLAG				0x01
#define SPI1_IRQ_TRNSMIT_FLAG				0x02
#define SPI1_IRQ_RECEIVE_AND_TRNSMIT_FLAG	0x03
#define SPI3_IRQ_RECEIVE_FLAG				0x11
#define SPI3_IRQ_TRNSMIT_FLAG				0x12
#define SPI3_IRQ_RECEIVE_AND_TRNSMIT_FLAG	0x13
#define SPI_DMA_ERR_FLAG					0x06

void SPI_DemoMain(void);
void SPI_DMA_Handle(void);

void Delay(void);

void SPI1_Demo_Main(void);

typedef enum 
{
	SPIx_Demo_Polling = 0,
	SPIx_Demo_IRQ	  = 1,
	SPIx_Demo_DMA	  = 2
} SPIx_Demo_TypefDef;

#endif
