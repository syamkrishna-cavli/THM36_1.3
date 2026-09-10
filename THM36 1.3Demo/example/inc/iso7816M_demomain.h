/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: iso7816M_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/ 
#ifndef __ISO7816M_DEMO_H
#define __ISO7816M_DEMO_H

#include "GPIO.h"
#include "TMC_ISO7816Mx.h"

extern GPIO_InitTypeDef GPIOB_Init;
extern GPIO_InitTypeDef GPIOC_Init;
extern void Delay(void);

typedef enum 
{
	M7816x_Demo_Polling = 0,
	M7816x_Demo_IRQ	  	= 1,
	M7816x_Demo_DMA	  	= 2
} M7816x_Demo_TypefDef;

void ISO7816M2_AsynDemoMain(void);
TMC_StatusTypeDef ISO7816Mx_GetATR(M7816x_HandleTypeDef* h7816m,uint8_t *Buffer,uint8_t Size);

void M7816x_DMA_Handle(void);

void ISO7816M_Demo_Main(void);

#endif
