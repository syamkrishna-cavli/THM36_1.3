/******************************************************************************** 
* Copyright (c) 2023, Tongxin Microelectroics Co., Ltd. 
* All rights reserved. 
* Module: uart5_demomain.h
* Version: V1.0 
* History: 
*   2023-05-22 Original version 
********************************************************************************/
#ifndef __UART5_DEMO_H_
#define __UART5_DEMO_H_

void UART5_DMA_Handle(void);
void UART5_DemoMain(void);

void UART5_Demo_Main(void);

typedef enum 
{
	Uart5_Demo_Polling  = 0,
	Uart5_Demo_IRQ	  	= 1,
	Uart5_Demo_DMA	  	= 2
} Uart5_Demo_TypefDef;

#endif
