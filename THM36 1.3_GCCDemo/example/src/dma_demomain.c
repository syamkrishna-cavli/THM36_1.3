/********************************************************************************
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: dma_demomain.c
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
*********************************************************************************/ 
#include "thm36_1.3.h"
#include "rcc.h"
#include "tmc_dma.h"
#include "dma_demomain.h"

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

void DMA_Test(void)
{
	if(DMA_Demo_MemoryCopy(SPITestBuffer1,SPITestBuffer2,256) != SUCCESS)
	{
		while(1);    //*DMA MemoryCopy Error occured. */
	}
	if(MemcpyCampare(SPITestBuffer1,SPITestBuffer2,256)==1)
	{
		while(1);    //*The result of DMA MemoryCopy error.*/
	}
}

ErrorStatus DMA_Demo_MemoryCopy(uint8_t* Srcbuf, uint8_t* Decbuf, uint16_t Size)
{
	DMA_HandleTypeDef hdmax;
	RCC_AHBPerforClockCmdInSleepMode(RCC_AHBPeriph_DMA, ENABLE);
	
	if(Size>4095)   //The maximum size is 4095 without using the LLI.
	{
		return ERROR;
	}
	hdmax.Instance = DMA_Channel7;
	hdmax.Init.DestInc = DMA_DEST_INC_ENABLE;
	hdmax.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
	hdmax.Init.IT_Enable  = DMA_IT_ENABLE;
	hdmax.Init.Direction = DMA_CTRL_MEMORY_TO_MEMORY;
	hdmax.Init.SrcDataAlignment = DMA_SRC_ALIGN_BYTE;
	hdmax.Init.DestDataAlignment = DMA_DEST_ALIGN_BYTE;
	hdmax.Init.DestBurst = DMA_DEST_BURST_INC1;
	hdmax.Init.SrcBurst = DMA_SRC_BURST_INC1;
	hdmax.Init.SrcInc = DMA_SRC_INC_ENABLE;
	hdmax.Init.DecAHBMx_Select = DMA_AHBM1_SELECT;
	hdmax.Init.SrcAHBMx_Select = DMA_AHBM2_SELECT;
	TMC_DMA_DeInit(&hdmax);
	TMC_DMA_Init(&hdmax);
	TMC_DMA_Start(&hdmax,(uint32_t)Srcbuf,(uint32_t)Decbuf,Size);
	if(TMC_DMA_PollForTransfer(&hdmax,10000)!= TMC_OK)
	{
		return ERROR;
	}
		
	TMC_DMA_DeInit(&hdmax);
	return SUCCESS;
}

void DMA_DemoMain(void)
{
	DMA_Test();
}

