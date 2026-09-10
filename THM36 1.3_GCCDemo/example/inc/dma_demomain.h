/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: dma_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __DMA_DEMO_H
#define __DMA_DEMO_H

#include "stdint.h"

extern uint8_t SPITestBuffer1[256];
extern uint8_t SPITestBuffer2[256];
ErrorStatus DMA_Demo_MemoryCopy(uint8_t* Srcbuf,uint8_t* Decbuf,uint16_t Size);
void DMA_Test(void);

void DMA_DemoMain(void);

#endif

